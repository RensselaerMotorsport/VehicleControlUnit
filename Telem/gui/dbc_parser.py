"""
DBC file parser for CAN message decoding
"""
import re
from typing import Dict, List, Optional, Tuple

class DBCSignal:
    def __init__(self, name: str, start_bit: int, length: int, byte_order: int, 
                 signed: bool, scale: float, offset: float, min_val: float, 
                 max_val: float, unit: str, receiver: str):
        self.name = name
        self.start_bit = start_bit
        self.length = length
        self.byte_order = byte_order  # 0=Motorola (big-endian), 1=Intel (little-endian)
        self.signed = signed
        self.scale = scale
        self.offset = offset
        self.min_val = min_val
        self.max_val = max_val
        self.unit = unit
        self.receiver = receiver

class DBCMessage:
    def __init__(self, can_id: int, name: str, dlc: int, sender: str):
        self.can_id = can_id
        self.name = name
        self.dlc = dlc
        self.sender = sender
        self.signals: List[DBCSignal] = []

class DBCParser:
    def __init__(self):
        self.messages: Dict[int, DBCMessage] = {}
        self.comments: Dict[str, str] = {}
        
    def parse_file(self, file_path: str) -> bool:
        """Parse a DBC file and populate message definitions"""
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
            
            self.messages.clear()
            self.comments.clear()
            
            # Parse messages
            self._parse_messages(content)
            # Parse signals
            self._parse_signals(content)
            # Parse comments
            self._parse_comments(content)
            
            return True
        except Exception as e:
            print(f"Error parsing DBC file: {e}")
            return False
    
    def _parse_messages(self, content: str):
        """Parse BO_ lines (message definitions)"""
        # Pattern: BO_ <CAN-ID> <MessageName>: <MessageSize> <Transmitter>
        pattern = r'BO_\s+(\d+)\s+(\w+):\s*(\d+)\s+(\w+)'
        
        for match in re.finditer(pattern, content):
            can_id = int(match.group(1))
            name = match.group(2)
            dlc = int(match.group(3))
            sender = match.group(4)
            
            self.messages[can_id] = DBCMessage(can_id, name, dlc, sender)
    
    def _parse_signals(self, content: str):
        """Parse SG_ lines (signal definitions)"""
        # Pattern: SG_ <SignalName> : <StartBit>|<SignalSize>@<ByteOrder><ValueType> (<Factor>,<Offset>) [<Minimum>|<Maximum>] "<Unit>" <Receiver>
        pattern = r'SG_\s+(\w+)\s*:\s*(\d+)\|(\d+)@(\d+)([\+\-])\s*\(([^,]+),([^)]+)\)\s*\[([^|]*)\|([^\]]*)\]\s*"([^"]*)"\s*(\w+)'
        
        current_message_id = None
        lines = content.split('\n')
        
        for line in lines:
            line = line.strip()
            
            # Check if this is a message line to track current message
            bo_match = re.match(r'BO_\s+(\d+)', line)
            if bo_match:
                current_message_id = int(bo_match.group(1))
                continue
            
            # Parse signal line
            sg_match = re.match(pattern, line)
            if sg_match and current_message_id and current_message_id in self.messages:
                name = sg_match.group(1)
                start_bit = int(sg_match.group(2))
                length = int(sg_match.group(3))
                byte_order = int(sg_match.group(4))
                signed = sg_match.group(5) == '-'
                scale = float(sg_match.group(6))
                offset = float(sg_match.group(7))
                min_val = float(sg_match.group(8)) if sg_match.group(8) else 0.0
                max_val = float(sg_match.group(9)) if sg_match.group(9) else 0.0
                unit = sg_match.group(10)
                receiver = sg_match.group(11)
                
                signal = DBCSignal(name, start_bit, length, byte_order, signed, 
                                 scale, offset, min_val, max_val, unit, receiver)
                
                self.messages[current_message_id].signals.append(signal)
    
    def _parse_comments(self, content: str):
        """Parse CM_ lines (comments)"""
        # Message comments: CM_ BO_ <CAN-ID> "<CommentText>";
        bo_pattern = r'CM_\s+BO_\s+(\d+)\s+"([^"]+)"'
        for match in re.finditer(bo_pattern, content):
            can_id = int(match.group(1))
            comment = match.group(2)
            self.comments[f"BO_{can_id}"] = comment
        
        # Signal comments: CM_ SG_ <CAN-ID> <SignalName> "<CommentText>";
        sg_pattern = r'CM_\s+SG_\s+(\d+)\s+(\w+)\s+"([^"]+)"'
        for match in re.finditer(sg_pattern, content):
            can_id = int(match.group(1))
            signal_name = match.group(2)
            comment = match.group(3)
            self.comments[f"SG_{can_id}_{signal_name}"] = comment
    
    def decode_message(self, can_id: int, data: bytes) -> Optional[Dict]:
        """Decode a CAN message using DBC definitions"""
        if can_id not in self.messages:
            return None
        
        message = self.messages[can_id]
        decoded = {
            'message_name': message.name,
            'sender': message.sender,
            'signals': {}
        }
        
        # Convert data to 64-bit integer for bit manipulation
        if len(data) > 8:
            data = data[:8]  # Truncate to 8 bytes max
        
        data_int = 0
        for i, byte in enumerate(data):
            data_int |= (byte << (8 * i))
        
        # Decode each signal
        for signal in message.signals:
            raw_value = self._extract_signal_value(data_int, signal)
            physical_value = raw_value * signal.scale + signal.offset
            
            decoded['signals'][signal.name] = {
                'raw': raw_value,
                'value': physical_value,
                'unit': signal.unit,
                'valid': signal.min_val <= physical_value <= signal.max_val
            }
        
        return decoded
    
    def _extract_signal_value(self, data_int: int, signal: DBCSignal) -> int:
        """Extract signal value from CAN data"""
        if signal.byte_order == 1:  # Intel (little-endian)
            # For Intel byte order, start_bit is the LSB position
            mask = (1 << signal.length) - 1
            value = (data_int >> signal.start_bit) & mask
        else:  # Motorola (big-endian)
            # For Motorola byte order, start_bit is the MSB position
            # Convert MSB position to LSB position
            start_bit_lsb = signal.start_bit - signal.length + 1
            mask = (1 << signal.length) - 1
            value = (data_int >> start_bit_lsb) & mask
        
        # Handle signed values
        if signal.signed and value & (1 << (signal.length - 1)):
            # Two's complement conversion
            value -= 1 << signal.length
        
        return value
    
    def get_message_info(self, can_id: int) -> Optional[Dict]:
        """Get message information"""
        if can_id not in self.messages:
            return None
        
        message = self.messages[can_id]
        return {
            'name': message.name,
            'dlc': message.dlc,
            'sender': message.sender,
            'signals': [s.name for s in message.signals],
            'comment': self.comments.get(f"BO_{can_id}", "")
        }
    
    def get_all_messages(self) -> Dict[int, str]:
        """Get all message IDs and names"""
        return {can_id: msg.name for can_id, msg in self.messages.items()}