"""
CAN message monitoring view
"""
import tkinter as tk
from tkinter import ttk
from datetime import datetime

class CANView:
    def __init__(self, parent):
        self.can_messages = {}  # id -> last_message_data
        
        self.frame = ttk.Frame(parent, padding="10")
        self.setup_ui()
    
    def setup_ui(self):
        """Setup CAN message display"""
        # Title and controls
        top_frame = ttk.Frame(self.frame)
        top_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(top_frame, text="CAN Messages", font=('TkDefaultFont', 12, 'bold')).pack(side=tk.LEFT)
        
        # Controls
        ttk.Button(top_frame, text="Clear", command=self.clear).pack(side=tk.RIGHT)
        self.pause_btn = ttk.Button(top_frame, text="Pause", command=self.toggle_pause)
        self.pause_btn.pack(side=tk.RIGHT, padx=5)
        
        self.paused = False
        
        # CAN message tree with direction
        columns = ('ID', 'Dir', 'DLC', 'Data', 'Count', 'Last Seen')
        self.can_tree = ttk.Treeview(self.frame, columns=columns, show='headings')
        
        self.can_tree.heading('ID', text='CAN ID')
        self.can_tree.heading('Dir', text='Dir')  # TX/RX
        self.can_tree.heading('DLC', text='DLC')
        self.can_tree.heading('Data', text='Data (Hex)')
        self.can_tree.heading('Count', text='Count')
        self.can_tree.heading('Last Seen', text='Last Seen')
        
        # Set column widths
        self.can_tree.column('ID', width=80)
        self.can_tree.column('Dir', width=40)  # TX/RX
        self.can_tree.column('DLC', width=50)
        self.can_tree.column('Data', width=200)
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
        """Update CAN message display with direction"""
        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
        
        # Hide the "no messages" label on first message
        if hasattr(self, 'no_can_label') and len(self.can_tree.get_children()) == 0:
            self.no_can_label.destroy()
            del self.no_can_label
        
        # Find existing message
        existing_item = None
        for item in self.can_tree.get_children():
            if self.can_tree.item(item, 'values')[0] == can_id:
                existing_item = item
                break
        
        if existing_item:
            # Update existing message
            current_values = self.can_tree.item(existing_item, 'values')
            count = int(current_values[4]) + 1
            
            # Update with direction info
            self.can_tree.item(existing_item, 
                              values=(can_id, direction, dlc, data, count, timestamp))
        else:
            # Add new message
            self.can_tree.insert('', 'end', 
                               values=(can_id, direction, dlc, data, 1, timestamp))
            
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