"""
Shared utilities for TreeView operations
"""

def sort_tree_alphabetically(treeview, groups_dict, group_name_key='name'):
    """
    Sort a TreeView with groups and children alphabetically.
    
    Args:
        treeview: The ttk.Treeview widget to sort
        groups_dict: Dictionary mapping group names to tree items
        group_name_key: Key name for the group identifier ('sensor_name', 'output_name', etc.)
    
    Returns:
        None (modifies treeview and groups_dict in place)
    """
    # Get all group items with their names
    groups_data = []
    for group_name, group_item in groups_dict.items():
        # Get all children (signals) for this group
        children = treeview.get_children(group_item)
        children_data = []
        
        for child in children:
            # Store child data
            child_data = {
                'text': treeview.item(child, 'text'),
                'values': treeview.item(child, 'values'),
                'tags': treeview.item(child, 'tags')
            }
            children_data.append(child_data)
        
        groups_data.append({
            group_name_key: group_name,
            'group_item': group_item,
            'group_values': treeview.item(group_item, 'values'),
            'group_tags': treeview.item(group_item, 'tags'),
            'children': sorted(children_data, key=lambda x: x['text'])  # Sort signals alphabetically
        })
    
    # Sort groups alphabetically by group name
    groups_data.sort(key=lambda x: x[group_name_key])
    
    # Clear the tree
    treeview.delete(*treeview.get_children())
    groups_dict.clear()
    
    # Rebuild tree in sorted order
    for group_data in groups_data:
        group_name = group_data[group_name_key]
        
        # Create group
        group_item = treeview.insert('', 'end', text=group_name,
                                   values=group_data['group_values'],
                                   tags=group_data['group_tags'])
        groups_dict[group_name] = group_item
        
        # Add sorted children
        for child_data in group_data['children']:
            treeview.insert(group_item, 'end', 
                          text=child_data['text'],
                          values=child_data['values'],
                          tags=child_data['tags'])

def create_common_message_parser():
    """
    Create a common message parser that can be reused across views.
    Returns a function that parses message format: Sender:Name;InfoType:Type;Content:Data
    """
    def parse_structured_message(raw_message):
        """
        Parse structured message and return components
        """
        try:
            message = raw_message.strip()
            
            # Find Sender
            if not message.startswith('Sender:'):
                return None
                
            # Split only on the first few semicolons
            parts = message.split(';', 2)  # Split into max 3 parts
            if len(parts) < 3:
                return None
            
            result = {}
            
            # Parse Sender:value
            if parts[0].startswith('Sender:'):
                result['Sender'] = parts[0][7:]  # Remove "Sender:"
            
            # Parse InfoType:value  
            if parts[1].startswith('InfoType:'):
                result['InfoType'] = parts[1][9:]  # Remove "InfoType:"
            
            # Parse Content:value (can contain semicolons)
            if parts[2].startswith('Content:'):
                result['Content'] = parts[2][8:]  # Remove "Content:"
            
            return result
            
        except Exception as e:
            return None
    
    return parse_structured_message

def extract_sensor_data_from_content(content):
    """
    Extract sensor data from Content field.
    Returns dict with parsed content parts.
    """
    content_parts = {}
    for item in content.split(';'):
        if ':' in item:
            key, value = item.split(':', 1)
            content_parts[key] = value
    return content_parts