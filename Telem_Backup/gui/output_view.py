"""
Output view for displaying STM32 output status
"""
import tkinter as tk
from tkinter import ttk
from datetime import datetime

class OutputView:
    def __init__(self, parent, config):
        self.config = config
        self.output_groups = {}
        
        if parent is not None:
            self.frame = ttk.Frame(parent, padding="10")
            self.setup_ui()
        else:
            self.frame = None
    
    def setup_ui(self):
        """Setup output display - EXACT copy of sensor view"""
        if self.frame is None:
            return
        
        # Title and controls
        title_frame = ttk.Frame(self.frame)
        title_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(title_frame, text="Outputs", font=('TkDefaultFont', 12, 'bold')).pack(side=tk.LEFT)
        ttk.Button(title_frame, text="Sort A-Z", command=self.sort_output_tree).pack(side=tk.RIGHT)
        
        # Tree frame
        tree_frame = ttk.Frame(self.frame)
        tree_frame.pack(fill=tk.BOTH, expand=True)
        
        # Output tree - EXACT same as sensor tree
        columns = ('Value', 'Unit', 'Range', 'Category', 'Time')
        self.output_tree = ttk.Treeview(tree_frame, columns=columns, show='tree headings')
        
        self.output_tree.heading('#0', text='Output / Signal')
        self.output_tree.heading('Value', text='Current Value')
        self.output_tree.heading('Unit', text='Unit')
        self.output_tree.heading('Range', text='Valid Range')
        self.output_tree.heading('Category', text='Category')
        self.output_tree.heading('Time', text='Last Update')
        
        # Column widths - EXACT same
        self.output_tree.column('#0', width=200)
        self.output_tree.column('Value', width=100)
        self.output_tree.column('Unit', width=60)
        self.output_tree.column('Range', width=120)
        self.output_tree.column('Category', width=100)
        self.output_tree.column('Time', width=100)
        
        self.output_tree.pack(fill=tk.BOTH, expand=True, side=tk.LEFT)
        
        # Scrollbar - EXACT same
        scrollbar = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self.output_tree.yview)
        self.output_tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Placeholder - EXACT same
        self.no_outputs_label = ttk.Label(tree_frame, 
                                         text="No outputs detected yet.\n\nConnect to STM32 and request config to see available outputs.",
                                         foreground="gray",
                                         justify=tk.CENTER)
        self.no_outputs_label.place(relx=0.5, rely=0.5, anchor=tk.CENTER)
    
    def process_output_message(self, raw_message):
        """Process OUTPUT_VALUE message - EXACT same parsing as sensor"""
        parts = {}
        for item in raw_message.split(';'):
            if ':' in item:
                key, value = item.split(':', 1)
                parts[key] = value
        
        sender = parts.get('Sender', '')
        content = parts.get('Content', '')
        unit = parts.get('Unit', '')
        
        if sender and content and unit:
            content_parts = {}
            for item in content.split(';'):
                if ':' in item:
                    key, value = item.split(':', 1)
                    content_parts[key] = value
            
            value = content_parts.get('Value', '')
            
            if value and unit:
                timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                self.update_output(sender, value, unit, timestamp)
    
    def update_output(self, name, value, unit, timestamp):
        """Update output - EXACT copy of sensor update_sensor method"""
        # Check if this output is configured - SAME as sensor
        if not self.config.is_configured(name, unit):
            return
        
        # Get signal configuration - SAME as sensor
        signal_config = self.config.get_signal_config(name, unit)
        
        # Format range display - SAME as sensor
        range_text = f"{signal_config['min']:.1f} - {signal_config['max']:.1f}"
        category = signal_config.get('category', 'Unknown')
        
        # Check for warnings - SAME as sensor
        try:
            value_float = float(value)
            is_warning = value_float < signal_config['min'] or value_float > signal_config['max']
        except:
            is_warning = False
        
        # Hide placeholder on first output - SAME as sensor
        if hasattr(self, 'no_outputs_label') and len(self.output_tree.get_children()) == 0:
            self.no_outputs_label.destroy()
            del self.no_outputs_label
        
        # Find or create output group - SAME as sensor
        group_item = self.get_or_create_output_group(name)
        
        # Look for existing signal - SAME as sensor
        existing_signal = None
        for child in self.output_tree.get_children(group_item):
            child_text = self.output_tree.item(child, 'text')
            if child_text == unit:
                existing_signal = child
                break
        
        # Configure warning style - SAME as sensor
        tag = 'warning' if is_warning else 'normal'
        self.output_tree.tag_configure('warning', background='#ffcccc')
        self.output_tree.tag_configure('normal', background='')
        
        if existing_signal:
            # Update existing signal - SAME as sensor
            self.output_tree.item(existing_signal, 
                             values=(value, unit, range_text, category, timestamp), 
                             tags=(tag,))
        else:
            # Add new signal - SAME as sensor
            self.output_tree.insert(group_item, 'end', text=unit,
                               values=(value, unit, range_text, category, timestamp),
                               tags=(tag,))
            self.output_tree.item(group_item, open=True)
            self.update_group_summary(name)
    
    def get_or_create_output_group(self, output_name):
        """Get or create output group - EXACT copy of sensor method"""
        if output_name in self.output_groups:
            return self.output_groups[output_name]
        
        group_item = self.output_tree.insert('', 'end', text=output_name, 
                                            values=('', '', '', '', ''),
                                            tags=('group',))
        
        self.output_tree.tag_configure('group', background='#f0f0f0', font=('TkDefaultFont', 9, 'bold'))
        
        self.output_groups[output_name] = group_item
        return group_item
    
    def update_group_summary(self, output_name):
        """Update group summary - EXACT copy of sensor method"""
        if output_name not in self.output_groups:
            return
        
        group_item = self.output_groups[output_name]
        children = self.output_tree.get_children(group_item)
        
        if children:
            total_signals = len(children)
            last_update = None
            
            for child in children:
                values = self.output_tree.item(child, 'values')
                if values and len(values) > 4:
                    last_update = values[4]
            
            summary = f"({total_signals} signals)"
            self.output_tree.item(group_item, 
                             values=(summary, '', '', '', last_update or ''),
                             tags=('group',))
    
    def sort_output_tree(self):
        """Sort output groups and signals alphabetically"""
        # Get all group items with their output names
        groups_data = []
        for output_name, group_item in self.output_groups.items():
            # Get all children (signals) for this group
            children = self.output_tree.get_children(group_item)
            children_data = []
            
            for child in children:
                # Store child data
                child_data = {
                    'text': self.output_tree.item(child, 'text'),
                    'values': self.output_tree.item(child, 'values'),
                    'tags': self.output_tree.item(child, 'tags')
                }
                children_data.append(child_data)
            
            groups_data.append({
                'output_name': output_name,
                'group_item': group_item,
                'group_values': self.output_tree.item(group_item, 'values'),
                'group_tags': self.output_tree.item(group_item, 'tags'),
                'children': sorted(children_data, key=lambda x: x['text'])  # Sort signals alphabetically
            })
        
        # Sort groups alphabetically by output name
        groups_data.sort(key=lambda x: x['output_name'])
        
        # Clear the tree
        self.output_tree.delete(*self.output_tree.get_children())
        self.output_groups.clear()
        
        # Rebuild tree in sorted order
        for group_data in groups_data:
            output_name = group_data['output_name']
            
            # Create group
            group_item = self.output_tree.insert('', 'end', text=output_name,
                                                values=group_data['group_values'],
                                                tags=group_data['group_tags'])
            self.output_groups[output_name] = group_item
            
            # Add sorted children
            for child_data in group_data['children']:
                self.output_tree.insert(group_item, 'end', 
                                      text=child_data['text'],
                                      values=child_data['values'],
                                      tags=child_data['tags'])
