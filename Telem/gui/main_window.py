"""
Main window container with connection controls and tabs
"""
import tkinter as tk
from tkinter import ttk, messagebox
import sys
import os
from datetime import datetime  # ADD THIS LINE

# Add parent directory to path for imports
sys.path.append(os.path.dirname(os.path.dirname(__file__)))

from connection_manager import ConnectionManager
from message_parser import ConfigManager
from .sensor_view import SensorView
from .raw_view import RawView
from .output_view import OutputView
from .can_view import CANView
from .plot_view import PlotView  # Add import at top
from .tree_utils import create_common_message_parser, extract_sensor_data_from_content

class MainWindow:
    def __init__(self, root):
        self.root = root
        self.root.title("STM32 Telemetry Monitor")
        self.root.geometry("1400x900")
        
        self.connection = ConnectionManager()
        self.config = ConfigManager()
        
        # Create shared message parser for efficiency
        self.parse_message = create_common_message_parser()
        
        self.setup_ui()
        self.start_processing()
        
        # Initialize memory monitoring
        print("🔍 Memory monitoring started - status will be printed every 10 seconds")
        
    def setup_ui(self):
        """Setup main UI with connection controls and tabs"""
        # Top frame - connection controls
        self.setup_connection_frame()
        
        # Main content - notebook tabs
        self.setup_tabs()
        
        # Initialize
        self.refresh_ports()
    
    def setup_connection_frame(self):
        """Setup connection controls"""
        top_frame = ttk.Frame(self.root, padding="10")
        top_frame.pack(fill=tk.X)
        
        # Port selection
        ttk.Label(top_frame, text="Port:").pack(side=tk.LEFT)
        self.port_var = tk.StringVar(value="Select Port")
        
        self.port_menu = ttk.Menubutton(top_frame, textvariable=self.port_var, width=10)
        self.port_menu.pack(side=tk.LEFT, padx=5)
        
        port_menu = tk.Menu(self.port_menu, tearoff=0)
        self.port_menu['menu'] = port_menu
        self.port_menu_widget = port_menu
        
        # Baud rate
        ttk.Label(top_frame, text="Baud:").pack(side=tk.LEFT, padx=(10, 0))
        self.baud_var = tk.StringVar(value="115200")
        self.baud_combo = ttk.Combobox(top_frame, textvariable=self.baud_var, width=8)
        self.baud_combo['values'] = ["115200", "230400"]
        self.baud_combo['state'] = 'readonly'
        self.baud_combo.pack(side=tk.LEFT, padx=5)
        
        # Connection buttons
        self.connect_btn = ttk.Button(top_frame, text="Connect", command=self.toggle_connection)
        self.connect_btn.pack(side=tk.LEFT, padx=10)
        
        ttk.Button(top_frame, text="Refresh", command=self.refresh_ports).pack(side=tk.LEFT, padx=5)
        ttk.Button(top_frame, text="Get Config", command=self.get_config).pack(side=tk.LEFT, padx=10)
        
        # Status
        self.status_label = ttk.Label(top_frame, text="Disconnected", foreground="red")
        self.status_label.pack(side=tk.RIGHT)
    
    def setup_tabs(self):
        """Setup notebook with different views"""
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Create individual views
        self.sensor_view = SensorView(None, self.config)
        self.raw_view = RawView(self.notebook)
        self.output_view = OutputView(None, self.config)
        self.can_view = CANView(None)
        self.plot_view = PlotView(None, self.config)  # ADD THIS LINE
        
        # Create combined view: Sensors + Outputs vertical, CAN to the right
        combined_frame = ttk.Frame(self.notebook, padding="5")
        
        # Main horizontal split
        main_paned = ttk.PanedWindow(combined_frame, orient=tk.HORIZONTAL)
        main_paned.pack(fill=tk.BOTH, expand=True)
        
        # Left side: vertical split for sensors + outputs
        left_paned = ttk.PanedWindow(main_paned, orient=tk.VERTICAL)
        
        # Add sensor view to top half - RECREATE the frame with proper parent
        sensor_frame = ttk.LabelFrame(left_paned, text="Sensors", padding="5")
        self.sensor_view.frame = sensor_frame
        self.sensor_view.setup_ui()
        left_paned.add(sensor_frame, weight=3)
        
        # Add output view to bottom half - RECREATE the frame with proper parent
        output_frame = ttk.LabelFrame(left_paned, text="Outputs", padding="5")
        self.output_view.frame = output_frame
        self.output_view.setup_ui()
        left_paned.add(output_frame, weight=1)
        
        main_paned.add(left_paned, weight=2)
        
        # Right side: CAN messages - RECREATE the frame with proper parent
        can_frame = ttk.LabelFrame(main_paned, text="CAN Messages", padding="5")
        self.can_view.frame = can_frame
        self.can_view.setup_ui()
        main_paned.add(can_frame, weight=1)
        
        # Setup plot view frame - ADD THIS BLOCK
        plot_frame = ttk.Frame(self.notebook, padding="5")
        self.plot_view.frame = plot_frame
        self.plot_view.setup_ui()
        
        # Add tabs
        self.notebook.add(combined_frame, text="Monitor")
        self.notebook.add(self.raw_view.frame, text="Raw Data")
        self.notebook.add(plot_frame, text="Plots")  # ADD THIS LINE
    
    def toggle_connection(self):
        """Toggle connection"""
        if self.connection.is_connected:
            self.disconnect()
        else:
            self.connect()
    
    def connect(self):
        """Connect to device"""
        port = self.port_var.get()
        baud = int(self.baud_var.get())
        
        if not port or port == "Select Port":
            messagebox.showerror("Error", "Please select a port")
            return
        
        success, message = self.connection.connect(port, baud)
        if success:
            self.connect_btn.configure(text="Disconnect")
            self.status_label.configure(text="Connected", foreground="green")
            self.raw_view.add_message(f"✓ {message}")
        else:
            messagebox.showerror("Connection Error", message)
    
    def disconnect(self):
        """Disconnect from device"""
        self.connection.disconnect()
        self.connect_btn.configure(text="Connect")
        self.status_label.configure(text="Disconnected", foreground="red")
        self.raw_view.add_message("✗ Disconnected")
    
    def refresh_ports(self):
        """Refresh port list"""
        ports = self.connection.get_available_ports()
        
        self.port_menu_widget.delete(0, 'end')
        
        for port in ports:
            self.port_menu_widget.add_command(label=port, 
                                             command=lambda p=port: self.port_var.set(p))
        
        if ports and self.port_var.get() == "Select Port":
            self.port_var.set(ports[0])
    
    def get_config(self):
        """Request telemetry configuration"""
        success, message = self.connection.send_command("CONFIG_REQUEST")
        self.raw_view.add_message(f">> {message}")
    
    def process_message(self, raw_message):
        """Process and route messages to appropriate views"""
        # Always log to raw view
        self.raw_view.add_message(f"<< {raw_message}")
        
        # Route config messages
        if 'TelemetryConfig' in raw_message:
            # Store previous config state
            was_complete = self.config.config_complete
            
            self.sensor_view.process_config_message(raw_message, self.config)
            
            # Check if config just became complete
            if not was_complete and self.config.config_complete:
                # Sort sensor and output trees alphabetically when config is complete
                self.sensor_view.sort_sensor_tree()
                self.output_view.sort_output_tree()
                print("Config complete - sorted sensor and output trees alphabetically")
            
            # Update plot signal list after config is loaded
            if hasattr(self, 'plot_view'):
                self.plot_view.update_signal_list()
            return
        
        # Route sensor messages
        if 'InfoType:SENSOR_VALUE' in raw_message:
            self.sensor_view.process_sensor_message(raw_message)
            
            # Send to plot view - optimized parsing using shared utilities
            parsed = self.parse_message(raw_message)
            if parsed:
                sender = parsed.get('Sender', '')
                content = parsed.get('Content', '')
                
                if sender and content:
                    content_parts = extract_sensor_data_from_content(content)
                    value = content_parts.get('Value', '')
                    unit = content_parts.get('Unit', '')
                    
                    if value and unit:
                        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                        self.plot_view.process_sensor_data(sender, value, unit, timestamp)
            
            return
        
        # Route output messages
        if 'InfoType:OUTPUT_VALUE' in raw_message:
            self.output_view.process_output_message(raw_message)
            return
        
        # Route CAN messages
        if 'InfoType:CAN_TX' in raw_message or 'InfoType:CAN_RX' in raw_message:
            self.can_view.process_can_message(raw_message)
            return
        
        # Route warning messages
        if 'InfoType:WARNING' in raw_message or 'InfoType:ERROR' in raw_message:
            self.raw_view.add_message(f"   ⚠️  {raw_message}")
            return
        
        # Route system status
        if 'InfoType:SYSTEM_STATUS' in raw_message:
            return
        
        # Route debug messages
        if 'InfoType:DEBUG' in raw_message:
            return
    
    def start_processing(self):
        """Main processing loop - optimized for high message throughput"""
        messages_processed = 0
        # Increased batch size to handle high STM32 message rates
        max_messages = 100  # Increased from 25 to handle queue overflow
        
        while messages_processed < max_messages:
            message = self.connection.get_message()
            if message:
                self.process_message(message)
                messages_processed += 1
            else:
                break
        
        # High-throughput interval for queue management
        self.root.after(50, self.start_processing)  # Reduced from 100ms to 50ms for higher throughput
        
        # Memory monitoring - log every 10 seconds
        self.memory_check_counter = getattr(self, 'memory_check_counter', 0) + 1
        if self.memory_check_counter >= 200:  # 200 * 50ms = 10 seconds
            self.memory_check_counter = 0
            self.log_memory_usage()
    
    def log_memory_usage(self):
        """Log memory usage of key components to help identify leaks"""
        try:
            queue_size = self.connection.data_queue.qsize()
            can_messages = len(self.can_view.can_tree.get_children()) if hasattr(self.can_view, 'can_tree') else 0
            sensor_pending = len(self.sensor_view.pending_updates) if hasattr(self.sensor_view, 'pending_updates') else 0
            output_pending = len(self.output_view.pending_updates) if hasattr(self.output_view, 'pending_updates') else 0
            
            # Always log current memory usage for monitoring
            print(f"MEMORY STATUS: Queue={queue_size}, CAN={can_messages}, SensorPending={sensor_pending}, OutputPending={output_pending}")
            
            # Warning if any values are concerning
            if queue_size > 1000 or can_messages > 500 or sensor_pending > 100 or output_pending > 100:
                print(f"⚠️  MEMORY WARNING: High usage detected!")
        except Exception as e:
            print(f"Memory check failed: {e}")