"""
Simple message parser for STM32 telemetry
"""

def parse_message(raw_message):
    """
    Parse structured message: Sender:Name;InfoType:Type;Content:Data
    Content can contain semicolons, so we need special handling
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
        
        # Parse Content:value (everything else)
        if parts[2].startswith('Content:'):
            result['Content'] = parts[2][8:]  # Remove "Content:"
        
        return result if len(result) == 3 else None
        
    except:
        return None

class ConfigManager:
    """Simple config manager"""
    def __init__(self):
        self.signals = {}
        self.config_complete = False
        
        # Temporarily add some mock data for testing plot view (remove in production)
        # self.add_mock_signals()
    
    def process_config_message(self, content):
        """Process telemetry config message"""
        print(f"Processing config content: '{content}'")  # Debug
        
        try:
            # Parse TotalSignals:5
            if 'TotalSignals:' in content:
                count = int(content.split('TotalSignals:')[1].split(';')[0])
                print(f"Expecting {count} signals")
                return
            
            # Parse ConfigComplete:1
            if 'ConfigComplete:' in content:
                self.config_complete = True
                print(f"Config complete! Got {len(self.signals)} signals")
                return
            
            # Parse signal config
            if 'Signal:' in content:
                print(f"Processing signal config: {content}")
                parts = {}
                for item in content.split(';'):
                    if ':' in item:
                        key, value = item.split(':', 1)
                        parts[key] = value
                
                signal_name = parts.get('Signal', '')
                unit = parts.get('Unit', '')
                
                print(f"Extracted: Signal='{signal_name}', Unit='{unit}'")
                
                if signal_name and unit:
                    key = f"{signal_name}_{unit}"
                    self.signals[key] = {
                        'name': signal_name,
                        'unit': unit,
                        'min': float(parts.get('Min', 0)),
                        'max': float(parts.get('Max', 100)),
                        'category': parts.get('Category', ''),
                        'decimals': int(parts.get('Decimals', 2))
                    }
                    print(f"Added signal: {signal_name} ({unit})")
        
        except Exception as e:
            print(f"Config parse error: {e}")
    
    def is_configured(self, signal_name, unit):
        """Check if signal is configured"""
        key = f"{signal_name}_{unit}"
        return key in self.signals
    
    def get_signal_config(self, signal_name, unit):
        """Get full configuration for a signal"""
        key = f"{signal_name}_{unit}"
        return self.signals.get(key, {
            'name': signal_name,
            'unit': unit,
            'min': 0,
            'max': 100,
            'category': 'Unknown',
            'decimals': 2
        })
    
    def get_all_configured_signals(self):
        """Get list of all configured signals"""
        return list(self.signals.keys())
    
    def add_mock_signals(self):
        """Add mock signals for testing plot view"""
        mock_signals = {
            'Engine_RPM': {
                'name': 'Engine',
                'unit': 'RPM',
                'min': 0,
                'max': 8000,
                'category': 'Engine',
                'decimals': 0
            },
            'Engine_Temp': {
                'name': 'Engine',
                'unit': 'Temp',
                'min': 0,
                'max': 120,
                'category': 'Engine',
                'decimals': 1
            },
            'Battery_Voltage': {
                'name': 'Battery',
                'unit': 'Voltage',
                'min': 10.0,
                'max': 14.0,
                'category': 'Power',
                'decimals': 2
            },
            'Wheel_Speed': {
                'name': 'Wheel',
                'unit': 'Speed',
                'min': 0,
                'max': 200,
                'category': 'Vehicle',
                'decimals': 1
            },
            'Brake_Pressure': {
                'name': 'Brake',
                'unit': 'Pressure',
                'min': 0,
                'max': 100,
                'category': 'Brake',
                'decimals': 1
            }
        }
        
        self.signals.update(mock_signals)
        print(f"Added {len(mock_signals)} mock signals for testing")