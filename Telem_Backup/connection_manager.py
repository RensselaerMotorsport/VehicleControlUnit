"""
Minimal serial connection manager
"""
import serial
import serial.tools.list_ports
import threading
import queue
import time

class ConnectionManager:
    def __init__(self):
        self.serial_port = None
        self.is_connected = False
        self.data_queue = queue.Queue()
        self.read_thread = None
        
    def get_available_ports(self):
        """Get available COM ports"""
        return [port.device for port in serial.tools.list_ports.comports()]
    
    def connect(self, port, baud_rate):
        """Connect to serial port"""
        try:
            if self.is_connected:
                self.disconnect()
            
            self.serial_port = serial.Serial(port, baud_rate, timeout=0.1)
            self.is_connected = True
            
            # Start reading in background
            self.read_thread = threading.Thread(target=self._read_data, daemon=True)
            self.read_thread.start()
            
            return True, f"Connected to {port}"
            
        except Exception as e:
            return False, f"Connection failed: {e}"
    
    def disconnect(self):
        """Disconnect"""
        self.is_connected = False
        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None
        return True, "Disconnected"
    
    def send_command(self, command):
        """Send command"""
        if self.is_connected and self.serial_port:
            try:
                self.serial_port.write(f"{command}\n".encode())
                return True, f"Sent: {command}"
            except Exception as e:
                return False, f"Send failed: {e}"
        return False, "Not connected"
    
    def _read_data(self):
        """Fastest possible reading"""
        buffer = ""
        
        while self.is_connected:
            try:
                if self.serial_port and self.serial_port.in_waiting > 0:
                    # Read everything available
                    data = self.serial_port.read(self.serial_port.in_waiting)
                    buffer += data.decode('utf-8', errors='ignore')
                    
                    # Process complete lines
                    while '\n' in buffer:
                        line, buffer = buffer.split('\n', 1)
                        line = line.strip()
                        if line:
                            self.data_queue.put(line)
                # NO SLEEP - just keep checking
            except:
                break
    
    def get_message(self):
        """Get next message"""
        try:
            return self.data_queue.get_nowait()
        except queue.Empty:
            return None