#!/usr/bin/env python3
"""
Convert DBC file to C byte array (equivalent to xxd -i)
"""

import sys
import os

def dbc_to_c_array(dbc_file, output_file):
    """Convert DBC file to C byte array"""
    
    # Read the DBC file
    try:
        with open(dbc_file, 'rb') as f:
            dbc_data = f.read()
    except Exception as e:
        print(f"Error reading DBC file: {e}")
        return False
    
    # Get variable names
    base_name = os.path.splitext(os.path.basename(dbc_file))[0]
    array_name = f"{base_name.replace('-', '_')}_dbc"
    len_name = f"{base_name.replace('-', '_')}_dbc_len"
    
    # Generate C array
    header_content = f"""unsigned char {array_name}[] = {{"""
    
    # Add bytes in groups of 12 for readability
    for i, byte in enumerate(dbc_data):
        if i % 12 == 0:
            header_content += "\n  "
        header_content += f"0x{byte:02x}"
        if i < len(dbc_data) - 1:
            header_content += ", "
    
    header_content += f"\n}};\nunsigned int {len_name} = {len(dbc_data)};\n"
    
    # Write to file
    try:
        with open(output_file, 'w') as f:
            f.write(header_content)
        print(f"Generated {output_file} with {len(dbc_data)} bytes")
        return True
    except Exception as e:
        print(f"Error writing file: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python dbc_to_c_array.py <input.dbc> <output.h>")
        sys.exit(1)
    
    dbc_file = sys.argv[1]
    output_file = sys.argv[2]
    
    if dbc_to_c_array(dbc_file, output_file):
        print("Success!")
    else:
        print("Failed!")
        sys.exit(1)