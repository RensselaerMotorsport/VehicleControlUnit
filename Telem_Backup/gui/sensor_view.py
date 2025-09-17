"""
Sensor monitoring view with grouped display
"""
import tkinter as tk
from tkinter import ttk
from datetime import datetime
from message_parser import parse_message

class SensorView:
    def __init__(self, parent, config):
        self.config = config
        self.sensor_groups = {}
        
        self.frame = ttk.Frame(parent, padding="10")
        self.setup_ui()
    
    def setup_ui(self):
        """Setup sensor display"""
        if self.frame is None:
            return
        
        # Title and controls
        title_frame = ttk.Frame(self.frame)
        title_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(title_frame, text="Sensors", font=('TkDefaultFont', 12, 'bold')).pack(side=tk.LEFT)
        ttk.Button(title_frame, text="Sort A-Z", command=self.sort_sensor_tree).pack(side=tk.RIGHT)
        
        # Tree frame
        tree_frame = ttk.Frame(self.frame)
        tree_frame.pack(fill=tk.BOTH, expand=True)
        
        # Sensor tree
        columns = ('Value', 'Unit', 'Range', 'Category', 'Time')
        self.sensor_tree = ttk.Treeview(tree_frame, columns=columns, show='tree headings')
        
        self.sensor_tree.heading('#0', text='Sensor / Signal')
        self.sensor_tree.heading('Value', text='Current Value')
        self.sensor_tree.heading('Unit', text='Unit')
        self.sensor_tree.heading('Range', text='Valid Range')
        self.sensor_tree.heading('Category', text='Category')
        self.sensor_tree.heading('Time', text='Last Update')
        
        # Set column widths
        self.sensor_tree.column('#0', width=200)
        self.sensor_tree.column('Value', width=100)
        self.sensor_tree.column('Unit', width=60)
        self.sensor_tree.column('Range', width=120)
        self.sensor_tree.column('Category', width=100)
        self.sensor_tree.column('Time', width=100)
        
        self.sensor_tree.pack(fill=tk.BOTH, expand=True, side=tk.LEFT)
        
        # ONLY vertical scrollbar (same as output view)
        scrollbar = ttk.Scrollbar(tree_frame, orient=tk.VERTICAL, command=self.sensor_tree.yview)
        self.sensor_tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Placeholder message (same as output view)
        self.no_sensors_label = ttk.Label(tree_frame, 
                                         text="No sensors detected yet.\n\nConnect to STM32 and request config to see available sensors.",
                                         foreground="gray",
                                         justify=tk.CENTER)
        self.no_sensors_label.place(relx=0.5, rely=0.5, anchor=tk.CENTER)
    
    def process_sensor_message(self, raw_message):
        """Process sensor data message"""
        #print(f"SENSOR DEBUG: Processing message: {raw_message}")
        
        # Parse sensor message: Sender:Name;InfoType:SENSOR_VALUE;Content:Value:X;Unit:Y
        parts = {}
        for item in raw_message.split(';'):
            if ':' in item:
                key, value = item.split(':', 1)
                parts[key] = value
        
        #print(f"SENSOR DEBUG: Parsed parts: {parts}")
        
        sender = parts.get('Sender', '')
        content = parts.get('Content', '')
        unit = parts.get('Unit', '')  # Get unit from top level, not content!
        
        #print(f"SENSOR DEBUG: Sender='{sender}', Content='{content}', Unit='{unit}'")
        
        # Parse the Content part for Value: Value:X
        if sender and content and unit:  # Need all three!
            content_parts = {}
            for item in content.split(';'):
                if ':' in item:
                    key, value = item.split(':', 1)
                    content_parts[key] = value
            
            #print(f"SENSOR DEBUG: Content parts: {content_parts}")
            
            value = content_parts.get('Value', '')
            
            #print(f"SENSOR DEBUG: Final - Value='{value}', Unit='{unit}'")
            
            if value and unit:
                timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                #print(f"SENSOR DEBUG: Calling update_sensor({sender}, {value}, {unit}, {timestamp})")
                self.update_sensor(sender, value, unit, timestamp)
            else:
                print("SENSOR DEBUG: Missing value or unit!")
    
    def process_config_message(self, raw_message, config):
        """Process configuration message"""
        parsed = parse_message(raw_message)
        if not parsed:
            return
        
        sender = parsed.get('Sender', '')
        info_type = parsed.get('InfoType', '')
        content = parsed.get('Content', '')
        
        if sender == 'TelemetryConfig' and info_type in ['DEBUG', 'CONFIG']:
            config.process_config_message(content)
    
    def update_sensor(self, name, value, unit, timestamp):
        """Update sensor with grouping by sensor name"""
        # Check if this sensor is configured
        if not self.config.is_configured(name, unit):
            return
        
        # Hide the "no sensors" message on first sensor (same as output view)
        if hasattr(self, 'no_sensors_label') and len(self.sensor_tree.get_children()) == 0:
            self.no_sensors_label.destroy()
            del self.no_sensors_label
        
        # Get signal configuration
        signal_config = self.config.get_signal_config(name, unit)
        
        # Format range display
        range_text = f"{signal_config['min']:.1f} - {signal_config['max']:.1f}"
        category = signal_config.get('category', 'Unknown')
        
        # Check for warnings (value out of range)
        try:
            value_float = float(value)
            is_warning = value_float < signal_config['min'] or value_float > signal_config['max']
        except:
            is_warning = False
        
        # Find or create sensor group
        group_item = self.get_or_create_sensor_group(name)
        
        # Look for existing signal under this group
        existing_signal = None
        for child in self.sensor_tree.get_children(group_item):
            child_text = self.sensor_tree.item(child, 'text')
            if child_text == unit:
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
            self.update_group_summary(name)
    
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
        """Update group header with summary info"""
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
                             values=(status, '', '', '', ''),
                             tags=(group_tag,))
            
            # Style for warning groups
            self.sensor_tree.tag_configure('group_warning', 
                                         background='#ffe0e0', 
                                         font=('TkDefaultFont', 9, 'bold'))
    
    def sort_sensor_tree(self):
        """Sort sensor groups and signals alphabetically"""
        # Get all group items with their sensor names
        groups_data = []
        for sensor_name, group_item in self.sensor_groups.items():
            # Get all children (signals) for this group
            children = self.sensor_tree.get_children(group_item)
            children_data = []
            
            for child in children:
                # Store child data
                child_data = {
                    'text': self.sensor_tree.item(child, 'text'),
                    'values': self.sensor_tree.item(child, 'values'),
                    'tags': self.sensor_tree.item(child, 'tags')
                }
                children_data.append(child_data)
            
            groups_data.append({
                'sensor_name': sensor_name,
                'group_item': group_item,
                'group_values': self.sensor_tree.item(group_item, 'values'),
                'group_tags': self.sensor_tree.item(group_item, 'tags'),
                'children': sorted(children_data, key=lambda x: x['text'])  # Sort signals alphabetically
            })
        
        # Sort groups alphabetically by sensor name
        groups_data.sort(key=lambda x: x['sensor_name'])
        
        # Clear the tree
        self.sensor_tree.delete(*self.sensor_tree.get_children())
        self.sensor_groups.clear()
        
        # Rebuild tree in sorted order
        for group_data in groups_data:
            sensor_name = group_data['sensor_name']
            
            # Create group
            group_item = self.sensor_tree.insert('', 'end', text=sensor_name,
                                                values=group_data['group_values'],
                                                tags=group_data['group_tags'])
            self.sensor_groups[sensor_name] = group_item
            
            # Add sorted children
            for child_data in group_data['children']:
                self.sensor_tree.insert(group_item, 'end', 
                                      text=child_data['text'],
                                      values=child_data['values'],
                                      tags=child_data['tags'])