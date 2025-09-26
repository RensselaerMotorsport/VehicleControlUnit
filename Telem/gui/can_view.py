"""
CAN message monitoring view with DBC decoding support
"""
import tkinter as tk
from tkinter import ttk, filedialog, messagebox
from datetime import datetime
from .dbc_parser import DBCParser

class CANView:
    def __init__(self, parent, connection_manager=None):
        self.can_messages = {}  # id -> last_message_data
        self.connection_manager = connection_manager
        self.dbc_parser = DBCParser()
        self.dbc_loaded = False
        
        self.frame = ttk.Frame(parent, padding="10")
        self.setup_ui()
    
    def setup_ui(self):
        """Setup CAN message display"""
        # Title and controls
        top_frame = ttk.Frame(self.frame)
        top_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(top_frame, text="CAN Messages", font=('TkDefaultFont', 12, 'bold')).pack(side=tk.LEFT)
        
        # Controls
        controls_frame = ttk.Frame(top_frame)
        controls_frame.pack(side=tk.RIGHT)
        
        ttk.Button(controls_frame, text="Clear", command=self.clear).pack(side=tk.RIGHT, padx=2)
        self.pause_btn = ttk.Button(controls_frame, text="Pause", command=self.toggle_pause)
        self.pause_btn.pack(side=tk.RIGHT, padx=2)
        
        # Add Load DBC button
        self.load_dbc_btn = ttk.Button(controls_frame, text="Load DBC", command=self.load_dbc_file)
        self.load_dbc_btn.pack(side=tk.RIGHT, padx=2)
        
        # Add Send Test CAN button
        if self.connection_manager:
            self.test_btn = ttk.Button(controls_frame, text="Send Test CAN", command=self.send_test_can)
            self.test_btn.pack(side=tk.RIGHT, padx=2)
        
        # DBC status label
        self.dbc_status_label = ttk.Label(top_frame, text="No DBC loaded", foreground="gray")
        self.dbc_status_label.pack(side=tk.LEFT, padx=10)
        
        self.paused = False
        
        # CAN message tree with direction and message name
        columns = ('ID', 'Name', 'Dir', 'DLC', 'Data', 'Decoded', 'Count', 'Last Seen')
        self.can_tree = ttk.Treeview(self.frame, columns=columns, show='headings')
        
        self.can_tree.heading('ID', text='CAN ID')
        self.can_tree.heading('Name', text='Message')
        self.can_tree.heading('Dir', text='Dir')  # TX/RX
        self.can_tree.heading('DLC', text='DLC')
        self.can_tree.heading('Data', text='Data (Hex)')
        self.can_tree.heading('Decoded', text='Decoded Signals')
        self.can_tree.heading('Count', text='Count')
        self.can_tree.heading('Last Seen', text='Last Seen')
        
        # Set column widths
        self.can_tree.column('ID', width=80)
        self.can_tree.column('Name', width=120)
        self.can_tree.column('Dir', width=40)  # TX/RX
        self.can_tree.column('DLC', width=50)
        self.can_tree.column('Data', width=150)
        self.can_tree.column('Decoded', width=300)
        self.can_tree.column('Count', width=60)
        self.can_tree.column('Last Seen', width=100)
        
        self.can_tree.pack(fill=tk.BOTH, expand=True)
        
        # Add scrollbar
        scrollbar = ttk.Scrollbar(self.frame, orient=tk.VERTICAL, command=self.can_tree.yview)
        self.can_tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Placeholder message for when no CAN messages
        self.no_can_label = ttk.Label(self.frame, 
                                     text="No CAN messages received yet.\n\nConnect to STM32 to see CAN traffic.",
                                     foreground="gray",
                                     justify=tk.CENTER)
        self.no_can_label.place(relx=0.5, rely=0.5, anchor=tk.CENTER)

    def process_can_message(self, raw_message):
        """Process CAN_TX or CAN_RX message"""
        # Debug: Always print CAN messages to see if any are coming through
        print(f"CAN DEBUG: Received message: {raw_message}")
        
        if self.paused:
            return
        
        # Parse CAN message: Sender:CAN;InfoType:CAN_TX;Content:ID:0x123;DLC:8;Data:0102030405060708
        # or: Sender:CAN;InfoType:CAN_RX;Content:ID:0x123;DLC:8;Data:0102030405060708
        
        # Determine direction
        direction = "TX" if "CAN_TX" in raw_message else "RX" if "CAN_RX" in raw_message else "??"
        
        # Parse content
        if 'Content:' in raw_message:
            content = raw_message.split('Content:', 1)[1]
            parts = {}
            for item in content.split(';'):
                if ':' in item:
                    key, value = item.split(':', 1)
                    parts[key] = value
            
            can_id = parts.get('ID', '')
            dlc = parts.get('DLC', '')
            data = parts.get('Data', '')
            
            if can_id and dlc and data:
                self.update_can_message(can_id, dlc, data, direction)

    def update_can_message(self, can_id, dlc, data, direction):
        """Update CAN message display with direction and DBC decoding"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        
        # Hide the "no messages" label on first message
        if hasattr(self, 'no_can_label') and len(self.can_tree.get_children()) == 0:
            self.no_can_label.destroy()
            del self.no_can_label
        
        # Convert CAN ID to integer for DBC lookup
        try:
            if can_id.startswith('0x') or can_id.startswith('0X'):
                can_id_int = int(can_id, 16)
            else:
                can_id_int = int(can_id)
        except ValueError:
            can_id_int = 0
        
        # Get message name from DBC
        message_name = ""
        decoded_signals = ""
        
        if self.dbc_loaded:
            msg_info = self.dbc_parser.get_message_info(can_id_int)
            if msg_info:
                message_name = msg_info['name']
                
                # Decode the message if we have data
                if data and len(data) >= 2:  # At least 1 byte (2 hex chars)
                    try:
                        # Convert hex string to bytes
                        data_bytes = bytes.fromhex(data)
                        decoded = self.dbc_parser.decode_message(can_id_int, data_bytes)
                        
                        if decoded and decoded['signals']:
                            # Format decoded signals for display
                            signal_strs = []
                            for sig_name, sig_data in decoded['signals'].items():
                                value_str = f"{sig_data['value']:.2f}" if isinstance(sig_data['value'], float) else str(sig_data['value'])
                                unit_str = f" {sig_data['unit']}" if sig_data['unit'] else ""
                                signal_strs.append(f"{sig_name}={value_str}{unit_str}")
                            decoded_signals = "; ".join(signal_strs)
                    except Exception as e:
                        decoded_signals = f"Decode error: {e}"
        
        # Find existing message
        existing_item = None
        for item in self.can_tree.get_children():
            if self.can_tree.item(item, 'values')[0] == can_id:
                existing_item = item
                break
        
        if existing_item:
            # Update existing message
            current_values = self.can_tree.item(existing_item, 'values')
            count = int(current_values[6]) + 1
            
            # Update with direction info and decoded data
            self.can_tree.item(existing_item, 
                              values=(can_id, message_name, direction, dlc, data, decoded_signals, count, timestamp))
        else:
            # Check if we have too many CAN IDs (memory management)
            if len(self.can_tree.get_children()) >= 1000:
                # Remove oldest entry
                oldest_item = self.can_tree.get_children()[0]
                self.can_tree.delete(oldest_item)
            
            # Add new message
            self.can_tree.insert('', 'end', 
                               values=(can_id, message_name, direction, dlc, data, decoded_signals, 1, timestamp))
            
            # Sort by CAN ID
            self.sort_can_messages()
    
    def sort_can_messages(self):
        """Sort CAN messages by ID"""
        items = [(self.can_tree.item(item, 'values')[0], item) 
                for item in self.can_tree.get_children()]
        
        # Sort by CAN ID (handle hex format)
        items.sort(key=lambda x: int(x[0], 16) if x[0].startswith('0x') else int(x[0]))
        
        # Reorder in tree
        for index, (_, item) in enumerate(items):
            self.can_tree.move(item, '', index)
    
    def clear(self):
        """Clear all CAN messages"""
        for item in self.can_tree.get_children():
            self.can_tree.delete(item)
        self.can_messages.clear()
    
    def toggle_pause(self):
        """Toggle pause state"""
        self.paused = not self.paused
        self.pause_btn.configure(text="Resume" if self.paused else "Pause")
    
    def send_test_can(self):
        """Send a test CAN message"""
        if self.connection_manager and self.connection_manager.is_connected:
            # Send command to STM32 to trigger test CAN message
            # The STM32 should have a command handler for this
            success, message = self.connection_manager.send_command("SEND_TEST_CAN")
            print(f"Test CAN command: {message}")
        else:
            # If not connected, generate test messages locally for GUI testing
            print("Generating test CAN messages locally for GUI testing...")
            
            # Generate test TX message
            test_tx_msg = "Sender:CAN;InfoType:CAN_TX;Content:ID:0x123;DLC:8;Data:AABBCCDD55667788"
            self.process_can_message(test_tx_msg)
            
            # Generate test RX message  
            test_rx_msg = "Sender:CAN;InfoType:CAN_RX;Content:ID:0x456;DLC:8;Data:1122334455667700"
            self.process_can_message(test_rx_msg)
            
            # Generate more test messages with different IDs
            test_messages = [
                "Sender:CAN;InfoType:CAN_TX;Content:ID:0x100;DLC:4;Data:DEADBEEF",
                "Sender:CAN;InfoType:CAN_RX;Content:ID:0x200;DLC:8;Data:CAFEBABE12345678",
                "Sender:CAN;InfoType:CAN_TX;Content:ID:0x555;DLC:2;Data:ABCD",
                "Sender:CAN;InfoType:CAN_RX;Content:ID:0x777;DLC:6;Data:123456789ABC"
            ]
            
            for msg in test_messages:
                self.process_can_message(msg)
    
    def load_dbc_file(self):
        """Load a DBC file for message decoding"""
        file_path = filedialog.askopenfilename(
            title="Select DBC File",
            filetypes=[("DBC files", "*.dbc"), ("All files", "*.*")],
            initialdir="../Core/Inc/Files"  # Default to the project DBC directory
        )
        
        if file_path:
            if self.dbc_parser.parse_file(file_path):
                self.dbc_loaded = True
                filename = file_path.split('/')[-1].split('\\')[-1]  # Get just the filename
                message_count = len(self.dbc_parser.get_all_messages())
                self.dbc_status_label.configure(
                    text=f"DBC: {filename} ({message_count} messages)", 
                    foreground="green"
                )
                print(f"Successfully loaded DBC file: {filename}")
                print(f"Found {message_count} message definitions")
                
                # Show a brief summary
                messages = self.dbc_parser.get_all_messages()
                if messages:
                    msg_list = [f"0x{can_id:X}: {name}" for can_id, name in list(messages.items())[:5]]
                    preview = "\n".join(msg_list)
                    if len(messages) > 5:
                        preview += f"\n... and {len(messages) - 5} more"
                    
                    messagebox.showinfo("DBC Loaded", f"Successfully loaded DBC file!\n\nMessages:\n{preview}")
            else:
                messagebox.showerror("Error", "Failed to parse DBC file. Please check the file format.")
        
    def get_dbc_info(self) -> str:
        """Get information about loaded DBC file"""
        if not self.dbc_loaded:
            return "No DBC file loaded"
        
        messages = self.dbc_parser.get_all_messages()
        return f"DBC loaded with {len(messages)} messages"