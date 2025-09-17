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
        self.data_queue = queue.Queue(maxsize=5000)  # Limit queue to 5000 messages to prevent memory leak
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
                            try:
                                self.data_queue.put_nowait(line)  # Non-blocking put
                            except queue.Full:
                                # Queue is full, drop oldest message and add new one
                                try:
                                    self.data_queue.get_nowait()  # Remove oldest
                                    self.data_queue.put_nowait(line)  # Add new
                                except queue.Empty:
                                    pass  # Queue somehow became empty, skip
                else:
                    # Small sleep to prevent busy waiting and reduce CPU load
                    import time
                    time.sleep(0.001)  # 1ms sleep when no data available
            except:
                break
    
    def get_message(self):
        """Get next message"""
        try:
            return self.data_queue.get_nowait()
        except queue.Empty:
            return None