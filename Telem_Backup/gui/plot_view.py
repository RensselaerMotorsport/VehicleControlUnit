"""
Real-time plotting view for sensor data
"""
import tkinter as tk
from tkinter import ttk
from collections import defaultdict, deque
import threading
import time
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.animation import FuncAnimation
import numpy as np

class PlotView:
    def __init__(self, parent, config):
        self.config = config
        self.plot_data = defaultdict(lambda: deque(maxlen=1000))  # Store last 1000 points per signal
        self.plot_times = defaultdict(lambda: deque(maxlen=1000))
        self.selected_signals = set()  # Which signals to plot
        self.is_plotting = False
        
        if parent is not None:
            self.frame = ttk.Frame(parent, padding="10")
            self.setup_ui()
        else:
            self.frame = None
    
    def setup_ui(self):
        """Setup plotting interface"""
        if self.frame is None:
            return
        
        # Title
        ttk.Label(self.frame, text="Real-Time Sensor Plots", 
                 font=('TkDefaultFont', 12, 'bold')).pack(pady=(0, 10))
        
        # Control frame
        control_frame = ttk.Frame(self.frame)
        control_frame.pack(fill=tk.X, pady=(0, 10))
        
        # Signal selection frame
        signal_frame = ttk.LabelFrame(control_frame, text="Signal Selection", padding="5")
        signal_frame.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 10))
        
        # Listbox for available signals
        ttk.Label(signal_frame, text="Available Signals:").pack(anchor=tk.W)
        
        listbox_frame = ttk.Frame(signal_frame)
        listbox_frame.pack(fill=tk.BOTH, expand=True)
        
        self.signal_listbox = tk.Listbox(listbox_frame, selectmode=tk.MULTIPLE, height=8, width=30)
        self.signal_listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        signal_scrollbar = ttk.Scrollbar(listbox_frame, orient=tk.VERTICAL, 
                                        command=self.signal_listbox.yview)
        self.signal_listbox.configure(yscrollcommand=signal_scrollbar.set)
        signal_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Buttons for signal control
        button_frame = ttk.Frame(signal_frame)
        button_frame.pack(fill=tk.X, pady=(5, 0))
        
        ttk.Button(button_frame, text="Add Selected", 
                  command=self.add_selected_signals).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Remove All", 
                  command=self.clear_all_signals).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(button_frame, text="Refresh List", 
                  command=self.update_signal_list).pack(side=tk.LEFT)
        
        # Plot controls
        plot_control_frame = ttk.LabelFrame(control_frame, text="Plot Controls", padding="5")
        plot_control_frame.pack(side=tk.LEFT, fill=tk.Y, padx=(0, 10))
        
        ttk.Button(plot_control_frame, text="Start Plotting", 
                  command=self.start_plotting).pack(fill=tk.X, pady=(0, 5))
        ttk.Button(plot_control_frame, text="Stop Plotting", 
                  command=self.stop_plotting).pack(fill=tk.X, pady=(0, 5))
        ttk.Button(plot_control_frame, text="Clear Data", 
                  command=self.clear_plot_data).pack(fill=tk.X, pady=(0, 5))
        ttk.Button(plot_control_frame, text="Debug Config", 
                  command=self.debug_config).pack(fill=tk.X, pady=(0, 5))
        ttk.Button(plot_control_frame, text="Test Data", 
                  command=self.add_test_data).pack(fill=tk.X, pady=(0, 5))
        
        # Time window control
        ttk.Label(plot_control_frame, text="Time Window (s):").pack(anchor=tk.W, pady=(10, 0))
        self.time_window_var = tk.StringVar(value="30")
        ttk.Entry(plot_control_frame, textvariable=self.time_window_var, width=10).pack(fill=tk.X)
        
        # Status frame
        status_frame = ttk.LabelFrame(control_frame, text="Status", padding="5")
        status_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        
        self.status_text = tk.Text(status_frame, height=6, width=30)
        self.status_text.pack(fill=tk.BOTH, expand=True)
        
        # Plot frame
        plot_frame = ttk.LabelFrame(self.frame, text="Real-Time Plot", padding="5")
        plot_frame.pack(fill=tk.BOTH, expand=True)
        
        # Create matplotlib figure
        self.setup_matplotlib(plot_frame)
        
        # Update signal list periodically
        self.update_signal_list()
        self.frame.after(2000, self.periodic_signal_update)
    
    def setup_matplotlib(self, parent):
        """Setup matplotlib figure and canvas"""
        # Create figure
        self.fig, self.ax = plt.subplots(figsize=(10, 6), facecolor='white')
        self.ax.set_xlabel('Time (seconds ago)')
        self.ax.set_ylabel('Value')
        self.ax.set_title('Real-Time Sensor Data')
        self.ax.grid(True, alpha=0.3)
        
        # Create canvas
        self.canvas = FigureCanvasTkAgg(self.fig, parent)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)
        
        # Store plot lines
        self.plot_lines = {}
        self.colors = plt.cm.tab10(np.linspace(0, 1, 10))  # 10 different colors
    
    def update_signal_list(self):
        """Update the list of available signals using config like sensor/output views"""
        if not self.config:
            self.add_status("No config available")
            return
        
        current_signals = set()
        
        # Use the same approach as sensor_view.py - check for get_all_configured_signals method
        if hasattr(self.config, 'get_all_configured_signals'):
            try:
                all_signal_keys = self.config.get_all_configured_signals()
                for signal_key in all_signal_keys:
                    # Convert from "name_unit" format to "name (unit)" format for display
                    if '_' in signal_key:
                        parts = signal_key.rsplit('_', 1)  # Split from right to handle names with underscores
                        if len(parts) == 2:
                            name, unit = parts
                            display_name = f"{name} ({unit})"
                            current_signals.add(display_name)
                
            except Exception as e:
                self.add_status(f"Error getting configured signals: {e}")
        
        # Fallback: check signals attribute directly
        elif hasattr(self.config, 'signals') and self.config.signals:
            for signal_key in self.config.signals.keys():
                # Convert from "name_unit" format to "name (unit)" format for display
                if '_' in signal_key:
                    parts = signal_key.rsplit('_', 1)  # Split from right to handle names with underscores
                    if len(parts) == 2:
                        name, unit = parts
                        display_name = f"{name} ({unit})"
                        current_signals.add(display_name)
        else:
            self.add_status("No signal source found in config")
        
        # Update listbox only if changed
        current_listbox = set(self.signal_listbox.get(0, tk.END))
        if current_signals != current_listbox:
            self.signal_listbox.delete(0, tk.END)
            for signal in sorted(current_signals):
                self.signal_listbox.insert(tk.END, signal)
            
            # Add helpful status message
            if current_signals:
                self.add_status(f"Found {len(current_signals)} configured signals")
            else:
                self.add_status("No signals found. Connect and request config first.")
    
    def periodic_signal_update(self):
        """Periodically update signal list (less frequent)"""
        # Only update if config might have changed
        if hasattr(self.config, 'config_data') and self.config.config_data:
            self.update_signal_list()
        
        if self.frame and self.frame.winfo_exists():
            self.frame.after(5000, self.periodic_signal_update)  # Every 5 seconds instead of 2
    
    def add_selected_signals(self):
        """Add selected signals to plot"""
        selected_indices = self.signal_listbox.curselection()
        for i in selected_indices:
            signal_name = self.signal_listbox.get(i)
            if signal_name not in self.selected_signals:
                self.selected_signals.add(signal_name)
                self.add_status(f"Added: {signal_name}")
                
                # If plotting is active, immediately create plot line for this signal
                if self.is_plotting and signal_name not in self.plot_lines:
                    color = self.colors[len(self.plot_lines) % len(self.colors)]
                    line, = self.ax.plot([], [], label=signal_name, color=color, linewidth=2)
                    self.plot_lines[signal_name] = line
        
        self.update_plot_legend()
    
    def clear_all_signals(self):
        """Remove all signals from plot"""
        self.selected_signals.clear()
        self.plot_lines.clear()
        self.ax.clear()
        self.setup_plot_axes()
        self.canvas.draw()
        self.add_status("Cleared all signals")
    
    def clear_plot_data(self):
        """Clear all plot data"""
        self.plot_data.clear()
        self.plot_times.clear()
        for line in self.plot_lines.values():
            line.set_data([], [])
        self.canvas.draw()
        self.add_status("Cleared plot data")
    
    def setup_plot_axes(self):
        """Setup plot axes"""
        self.ax.set_xlabel('Time (seconds ago)')
        self.ax.set_ylabel('Value')
        self.ax.set_title('Real-Time Sensor Data')
        self.ax.grid(True, alpha=0.3)
    
    def update_plot_legend(self):
        """Update plot legend with current signals"""
        if self.selected_signals or self.plot_lines:
            # Force legend update even if no data yet
            if self.plot_lines:
                self.ax.legend()
            self.canvas.draw()
            self.add_status(f"Updated legend for {len(self.selected_signals)} selected signals")
    
    def start_plotting(self):
        """Start real-time plotting"""
        if not self.is_plotting:
            self.is_plotting = True
            
            # Pre-create plot lines for all selected signals to ensure proper labeling
            for signal_name in self.selected_signals:
                if signal_name not in self.plot_lines:
                    color = self.colors[len(self.plot_lines) % len(self.colors)]
                    line, = self.ax.plot([], [], label=signal_name, color=color, linewidth=2)
                    self.plot_lines[signal_name] = line
            
            # Update legend with all selected signals
            if self.plot_lines:
                self.ax.legend()
                self.canvas.draw()
            
            self.plot_animation = FuncAnimation(self.fig, self.update_plot, 
                                              interval=100, blit=False, cache_frame_data=False)
            self.add_status(f"Started plotting for {len(self.selected_signals)} signals")
    
    def stop_plotting(self):
        """Stop real-time plotting"""
        if self.is_plotting:
            self.is_plotting = False
            if hasattr(self, 'plot_animation'):
                self.plot_animation.event_source.stop()
            self.add_status("Stopped plotting")
    
    def update_plot(self, frame):
        """Update plot with new data (called by animation)"""
        if not self.is_plotting or not self.selected_signals:
            return
        
        try:
            time_window = float(self.time_window_var.get())
        except:
            time_window = 30.0
        
        current_time = time.time()
        
        # Update each selected signal
        for signal_name in self.selected_signals:
            if signal_name not in self.plot_data or len(self.plot_data[signal_name]) == 0:
                continue
            
            # Get data within time window
            times = list(self.plot_times[signal_name])
            values = list(self.plot_data[signal_name])
            
            if not times:
                continue
            
            # Filter data to time window
            cutoff_time = current_time - time_window
            filtered_times = []
            filtered_values = []
            
            for t, v in zip(times, values):
                if t >= cutoff_time:
                    filtered_times.append(current_time - t)  # Seconds ago
                    filtered_values.append(v)
            
            if not filtered_times:
                continue
            
            # Update or create plot line
            if signal_name not in self.plot_lines:
                color = self.colors[len(self.plot_lines) % len(self.colors)]
                line, = self.ax.plot([], [], label=signal_name, color=color, linewidth=2)
                self.plot_lines[signal_name] = line
            
            # Update line data
            self.plot_lines[signal_name].set_data(filtered_times, filtered_values)
        
        # Auto-scale axes
        self.ax.relim()
        self.ax.autoscale_view()
            
        # Always update legend when we have plot lines
        if self.plot_lines:
            self.ax.legend()
        
        # Redraw canvas
        self.canvas.draw()
    
    def process_sensor_data(self, name, value, unit, timestamp):
        """Process incoming sensor data for plotting - only if configured"""
        # Check if this signal is configured (same as sensor_view)
        if not self.config or not self.config.is_configured(name, unit):
            return
    
        try:
            value_float = float(value)
            signal_key = f"{name} ({unit})"
            
            # Store data with timestamp
            current_time = time.time()
            self.plot_data[signal_key].append(value_float)
            self.plot_times[signal_key].append(current_time)
            
            # Occasional debug (every 50 points to avoid spam)
            if len(self.plot_data[signal_key]) % 50 == 0:
                self.add_status(f"Collected {len(self.plot_data[signal_key])} points for {signal_key}")
                
        except ValueError:
            pass  # Skip non-numeric values
    
    def add_status(self, message):
        """Add status message"""
        timestamp = datetime.now().strftime("%H:%M:%S")
        self.status_text.insert(tk.END, f"[{timestamp}] {message}\n")
        self.status_text.see(tk.END)
        
        # Limit status text to last 20 lines
        lines = self.status_text.get("1.0", tk.END).split('\n')
        if len(lines) > 20:
            self.status_text.delete("1.0", f"{len(lines)-20}.0")
    
    def debug_config(self):
        """Debug config object structure to understand signal detection issue"""
        self.add_status("=== CONFIG DEBUG ===")
        
        if not self.config:
            self.add_status("ERROR: config is None")
            return
        
        self.add_status(f"Config type: {type(self.config)}")
        self.add_status(f"Config attributes: {dir(self.config)}")
        
        # Check for config_data attribute
        if hasattr(self.config, 'config_data'):
            self.add_status(f"config_data type: {type(self.config.config_data)}")
            self.add_status(f"config_data: {self.config.config_data}")
        else:
            self.add_status("No config_data attribute")
        
        # Check for signals attribute (like sensor view uses)
        if hasattr(self.config, 'signals'):
            self.add_status(f"signals type: {type(self.config.signals)}")
            self.add_status(f"signals count: {len(self.config.signals) if self.config.signals else 0}")
            if self.config.signals:
                # Show first few signals
                signal_keys = list(self.config.signals.keys())[:5]
                for key in signal_keys:
                    self.add_status(f"Signal: {key} -> {self.config.signals[key]}")
        else:
            self.add_status("No signals attribute")
        
        # Test is_configured method
        if hasattr(self.config, 'is_configured'):
            self.add_status("is_configured method exists")
        else:
            self.add_status("No is_configured method")
        
        # Test get_all_configured_signals method
        if hasattr(self.config, 'get_all_configured_signals'):
            try:
                all_signals = self.config.get_all_configured_signals()
                self.add_status(f"All configured signals: {all_signals}")
            except Exception as e:
                self.add_status(f"Error getting signals: {e}")
        else:
            self.add_status("No get_all_configured_signals method")
    
    def add_test_data(self):
        """Add some test data for testing the plot functionality"""
        import random
        import time
        
        # Simulate some sensor data
        test_signals = ["Engine", "Battery", "Wheel", "Brake"]
        test_units = ["RPM", "Voltage", "Speed", "Pressure"]
        
        for signal, unit in zip(test_signals, test_units):
            # Generate random value
            if unit == "RPM":
                value = random.randint(1000, 7000)
            elif unit == "Voltage":
                value = round(random.uniform(11.5, 13.8), 2)
            elif unit == "Speed":
                value = round(random.uniform(0, 180), 1)
            elif unit == "Pressure":
                value = round(random.uniform(0, 80), 1)
            else:
                value = round(random.uniform(0, 100), 1)
            
            timestamp = time.strftime("%H:%M:%S")
            self.process_sensor_data(signal, str(value), unit, timestamp)
        
        self.add_status("Added test data for 4 signals")