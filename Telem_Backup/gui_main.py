"""
Minimal STM32 telemetry GUI
"""
import tkinter as tk
from tkinter import ttk, messagebox
from connection_manager import ConnectionManager
from message_parser import parse_message, ConfigManager

class SimpleSTM32GUI:
    def __init__(self, root):
        self.root = root
        self.root.title("STM32 Telemetry Monitor")
        self.root.geometry("1200x800")
        
        self.connection = ConnectionManager()
        self.config = ConfigManager()
        self.sensor_groups = {}  # Add this line
        
        self.setup_ui()
        self.start_processing()
        
    def setup_ui(self):
        """Setup simple UI"""
        # Top frame - connection
        top_frame = ttk.Frame(self.root, padding="10")
        top_frame.pack(fill=tk.X)
        
        # Port selection with menu instead of combobox
        ttk.Label(top_frame, text="Port:").pack(side=tk.LEFT)
        self.port_var = tk.StringVar(value="Select Port")
        
        self.port_menu = ttk.Menubutton(top_frame, textvariable=self.port_var, width=10)
        self.port_menu.pack(side=tk.LEFT, padx=5)
        
        # Create menu
        port_menu = tk.Menu(self.port_menu, tearoff=0)
        self.port_menu['menu'] = port_menu
        
        # Store reference to update later
        self.port_menu_widget = port_menu
        
        # Baud rate
        ttk.Label(top_frame, text="Baud:").pack(side=tk.LEFT, padx=(10, 0))
        self.baud_var = tk.StringVar(value="115200")
        self.baud_combo = ttk.Combobox(top_frame, textvariable=self.baud_var, width=8)
        self.baud_combo['values'] = ["115200", "230400"]
        self.baud_combo['state'] = 'readonly'
        self.baud_combo.pack(side=tk.LEFT, padx=5)
        
        # Buttons
        self.connect_btn = ttk.Button(top_frame, text="Connect", command=self.toggle_connection)
        self.connect_btn.pack(side=tk.LEFT, padx=10)
        
        ttk.Button(top_frame, text="Refresh", command=self.refresh_ports).pack(side=tk.LEFT, padx=5)
        ttk.Button(top_frame, text="Get Config", command=self.get_config).pack(side=tk.LEFT, padx=10)
        ttk.Button(top_frame, text="Show Config", command=self.show_config).pack(side=tk.LEFT, padx=5)
        
        # Status
        self.status_label = ttk.Label(top_frame, text="Disconnected", foreground="red")
        self.status_label.pack(side=tk.RIGHT)
        
        # Main content - notebook tabs
        self.notebook = ttk.Notebook(self.root)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # Raw data tab
        self.setup_raw_tab()
        
        # Sensor data tab
        self.setup_sensor_tab()
        
        # Initialize
        self.refresh_ports()
    
    def setup_raw_tab(self):
        """Setup raw data display"""
        raw_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(raw_frame, text="Raw Data")
        
        # Text display
        self.raw_text = tk.Text(raw_frame, height=30, width=80)
        scrollbar = ttk.Scrollbar(raw_frame, orient=tk.VERTICAL, command=self.raw_text.yview)
        self.raw_text.configure(yscrollcommand=scrollbar.set)
        
        self.raw_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Clear button
        ttk.Button(raw_frame, text="Clear", 
                  command=lambda: self.raw_text.delete(1.0, tk.END)).pack(pady=5)
    
    def setup_sensor_tab(self):
        """Setup enhanced sensor display with grouping"""
        sensor_frame = ttk.Frame(self.notebook, padding="10")
        self.notebook.add(sensor_frame, text="Sensors")
        
        # Tree for sensor data with grouping
        columns = ('Value', 'Unit', 'Range', 'Category', 'Time')
        self.sensor_tree = ttk.Treeview(sensor_frame, columns=columns, show='tree headings')
        
        self.sensor_tree.heading('#0', text='Sensor / Signal')
        self.sensor_tree.heading('Value', text='Current Value')
        self.sensor_tree.heading('Unit', text='Unit')
        self.sensor_tree.heading('Range', text='Range')
        self.sensor_tree.heading('Category', text='Category')
        self.sensor_tree.heading('Time', text='Last Update')
        
        # Set column widths
        self.sensor_tree.column('#0', width=200)  # Wider for grouped names
        self.sensor_tree.column('Value', width=100)
        self.sensor_tree.column('Unit', width=60)
        self.sensor_tree.column('Range', width=120)
        self.sensor_tree.column('Category', width=100)
        self.sensor_tree.column('Time', width=100)
        
        self.sensor_tree.pack(fill=tk.BOTH, expand=True)
        
        # Add scrollbar
        scrollbar_sensor = ttk.Scrollbar(sensor_frame, orient=tk.VERTICAL, command=self.sensor_tree.yview)
        self.sensor_tree.configure(yscrollcommand=scrollbar_sensor.set)
        scrollbar_sensor.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Store group items for easy lookup
        self.sensor_groups = {}  # sensor_name -> tree_item_id
    
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
        
        if not port:
            messagebox.showerror("Error", "Please select a port")
            return
        
        success, message = self.connection.connect(port, baud)
        if success:
            self.connect_btn.configure(text="Disconnect")
            self.status_label.configure(text="Connected", foreground="green")
            self.add_raw_message(f"✓ {message}\n")
        else:
            messagebox.showerror("Connection Error", message)
    
    def disconnect(self):
        """Disconnect from device"""
        self.connection.disconnect()
        self.connect_btn.configure(text="Connect")
        self.status_label.configure(text="Disconnected", foreground="red")
        self.add_raw_message("✗ Disconnected\n")
    
    def refresh_ports(self):
        """Refresh port list for menu"""
        print("DEBUG: refresh_ports() called")
        ports = self.connection.get_available_ports()
        
        # Clear existing menu items
        self.port_menu_widget.delete(0, 'end')
        
        # Add new items
        for port in ports:
            self.port_menu_widget.add_command(label=port, 
                                             command=lambda p=port: self.port_var.set(p))
        
        if ports and self.port_var.get() == "Select Port":
            self.port_var.set(ports[0])
    
    def get_config(self):
        """Request telemetry configuration"""
        success, message = self.connection.send_command("CONFIG_REQUEST")
        self.add_raw_message(f">> {message}\n")
    
    def add_raw_message(self, message):
        """Add message to raw display"""
        self.raw_text.insert(tk.END, message)
        self.raw_text.see(tk.END)
        
        # Keep only last 1000 lines
        lines = int(self.raw_text.index(tk.END).split('.')[0])
        if lines > 1000:
            self.raw_text.delete(1.0, "500.0")
    
    def update_sensor(self, name, value, unit, timestamp):
        """Update sensor with grouping by sensor name"""
        # Check if this sensor is configured
        if not self.config.is_configured(name, unit):
            self.add_raw_message(f"   [WARNING: {name} ({unit}) not configured]\n")
            return
        
        # Get signal configuration
        signal_config = self.config.get_signal_config(name, unit)
        
        # Format range display
        range_text = f"{signal_config['min']:.1f} - {signal_config['max']:.1f}"
        category = signal_config.get('category', 'Unknown')
        
        # Check for warnings (value out of range)
        try:
            value_float = float(value)
            is_warning = value_float < signal_config['min'] or value_float > signal_config['max']
            
            if is_warning:
                self.add_raw_message(f"   [WARNING: {name} value {value} outside range {range_text}]\n")
        except:
            is_warning = False
        
        # Find or create sensor group
        group_item = self.get_or_create_sensor_group(name)
        
        # Look for existing signal under this group
        signal_key = f"{name}_{unit}"
        existing_signal = None
        
        for child in self.sensor_tree.get_children(group_item):
            child_text = self.sensor_tree.item(child, 'text')
            if child_text == unit:  # Signal identified by unit under the group
                existing_signal = child
                break
        
        # Configure warning style
        tag = 'warning' if is_warning else 'normal'
        self.sensor_tree.tag_configure('warning', background='#ffcccc')
        self.sensor_tree.tag_configure('normal', background='')
        
        if existing_signal:
            # Update existing signal
            self.sensor_tree.item(existing_signal, 
                             values=(value, unit, range_text, category, timestamp), 
                             tags=(tag,))
        else:
            # Add new signal under the group
            self.sensor_tree.insert(group_item, 'end', text=unit,
                               values=(value, unit, range_text, category, timestamp),
                               tags=(tag,))
            # Expand the group to show signals
            self.sensor_tree.item(group_item, open=True)
            self.update_group_summary(name)  # Update group summary info

    def get_or_create_sensor_group(self, sensor_name):
        """Get existing sensor group or create new one"""
        if sensor_name in self.sensor_groups:
            return self.sensor_groups[sensor_name]
        
        # Create new group
        group_item = self.sensor_tree.insert('', 'end', text=sensor_name, 
                                            values=('', '', '', '', ''),
                                            tags=('group',))
        
        # Style for group headers
        self.sensor_tree.tag_configure('group', background='#f0f0f0', font=('TkDefaultFont', 9, 'bold'))
        
        self.sensor_groups[sensor_name] = group_item
        return group_item
    
    def update_group_summary(self, sensor_name):
        """Update group header with summary info - no timestamp"""
        if sensor_name not in self.sensor_groups:
            return
        
        group_item = self.sensor_groups[sensor_name]
        children = self.sensor_tree.get_children(group_item)
        
        if children:
            # Count signals and warnings
            total_signals = len(children)
            warnings = 0
            
            for child in children:
                tags = self.sensor_tree.item(child, 'tags')
                if 'warning' in tags:
                    warnings += 1
            
            # Update group header - no timestamp
            status = f"({total_signals} signals"
            if warnings > 0:
                status += f", {warnings} warnings"
            status += ")"
            
            group_tag = 'group_warning' if warnings > 0 else 'group'
            self.sensor_tree.item(group_item, 
                             values=(status, '', '', '', ''),  # Empty timestamp
                             tags=(group_tag,))
            
            # Style for warning groups
            self.sensor_tree.tag_configure('group_warning', 
                                         background='#ffe0e0', 
                                         font=('TkDefaultFont', 9, 'bold'))
    
    def process_message(self, raw_message):
        """Process incoming message"""
        # Always show in raw data
        self.add_raw_message(f"<< {raw_message}\n")
        
        # Handle sensor messages (different format than config)
        if 'SENSOR_VALUE' in raw_message:
            # Parse sensor message: Sender:Name;InfoType:SENSOR_VALUE;Value:X;Unit:Y
            parts = {}
            for item in raw_message.split(';'):
                if ':' in item:
                    key, value = item.split(':', 1)
                    parts[key] = value
        
            sender = parts.get('Sender', '')
            value = parts.get('Value', '')
            unit = parts.get('Unit', '')
            
            if sender and value and unit:
                from datetime import datetime
                # Add milliseconds to timestamp
                timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]  # Remove last 3 digits for milliseconds
                self.update_sensor(sender, value, unit, timestamp)
            return
        
        # Try to parse config messages (with Content field)
        parsed = parse_message(raw_message)
        if not parsed:
            self.add_raw_message(f"   [PARSE FAILED]\n")
            return
        
        sender = parsed.get('Sender', '')
        info_type = parsed.get('InfoType', '')
        content = parsed.get('Content', '')
        
        # Handle telemetry config
        if sender == 'TelemetryConfig' and info_type in ['DEBUG', 'CONFIG']:
            print(f"CONFIG RAW: '{raw_message}'")
            print(f"CONFIG CONTENT: '{content}'")
            
            self.add_raw_message(f"   [PROCESSING CONFIG MESSAGE]\n")
            self.config.process_config_message(content)
            return
    
    def start_processing(self):
        """Much faster processing loop"""
        # Process ALL available messages each cycle
        messages_processed = 0
        while messages_processed < 50:  # Process up to 50 messages per cycle
            message = self.connection.get_message()
            if message:
                self.process_message(message)
                messages_processed += 1
            else:
                break  # No more messages
        
        # Schedule next update much faster
        self.root.after(100, self.start_processing)  # 20 FPS

    def show_config(self):
        """Show configuration summary"""
        signals = self.config.get_all_configured_signals()
        if not signals:
            self.add_raw_message(">> No signals configured yet. Run 'Get Config' first.\n")
            return
        
        self.add_raw_message(f">> Configured Signals ({len(signals)}):\n")
        for signal_key in signals:
            config = self.config.signals[signal_key]
            self.add_raw_message(f"   • {config['name']} ({config['unit']}) "
                               f"Range: {config['min']}-{config['max']} "
                               f"Category: {config.get('category', 'N/A')}\n")

def main():
    root = tk.Tk()
    app = SimpleSTM32GUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()