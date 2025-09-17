"""
Raw data view for debugging
"""
import tkinter as tk
from tkinter import ttk

class RawView:
    def __init__(self, parent):
        self.frame = ttk.Frame(parent, padding="10")
        self.setup_ui()
    
    def setup_ui(self):
        """Setup raw data display"""
        # Text widget for raw messages
        self.text_widget = tk.Text(self.frame, wrap=tk.WORD, height=20)
        self.text_widget.pack(fill=tk.BOTH, expand=True)
        
        # Add scrollbar
        scrollbar = ttk.Scrollbar(self.frame, orient=tk.VERTICAL, command=self.text_widget.yview)
        self.text_widget.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
        
        # Controls frame
        controls_frame = ttk.Frame(self.frame)
        controls_frame.pack(fill=tk.X, pady=(10, 0))
        
        ttk.Button(controls_frame, text="Clear", command=self.clear).pack(side=tk.LEFT)
        ttk.Button(controls_frame, text="Save Log", command=self.save_log).pack(side=tk.LEFT, padx=5)
    
    def add_message(self, message):
        """Add message to raw display"""
        self.text_widget.insert(tk.END, f"{message}\n")
        self.text_widget.see(tk.END)
        
        # Keep only last 1000 lines
        lines = int(self.text_widget.index(tk.END).split('.')[0])
        if lines > 1000:
            self.text_widget.delete(1.0, "500.0")
    
    def clear(self):
        """Clear the display"""
        self.text_widget.delete(1.0, tk.END)
    
    def save_log(self):
        """Save log to file"""
        from tkinter import filedialog
        from datetime import datetime
        
        filename = filedialog.asksaveasfilename(
            defaultextension=".txt",
            filetypes=[("Text files", "*.txt"), ("All files", "*.*")],
            initialname=f"telemetry_log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
        )
        
        if filename:
            try:
                with open(filename, 'w') as f:
                    f.write(self.text_widget.get(1.0, tk.END))
                print(f"Log saved to {filename}")
            except Exception as e:
                print(f"Error saving log: {e}")