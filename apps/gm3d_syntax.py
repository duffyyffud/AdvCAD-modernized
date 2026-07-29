"""
Advanced syntax highlighting for AdvCAD GM3D Editor with vector-specific colors
"""

import re
from PyQt5.QtCore import QRegExp
from PyQt5.QtGui import QSyntaxHighlighter, QTextCharFormat, QColor, QFont

class GM3DSyntaxHighlighter(QSyntaxHighlighter):
    """Advanced syntax highlighter for .gm3d files with vector-specific coloring"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        # Define color formats for parameter position groups
        self.command_format = QTextCharFormat()
        self.command_format.setForeground(QColor(0, 100, 200))  # Blue
        self.command_format.setFontWeight(QFont.Bold)
        
        self.group1_format = QTextCharFormat()
        self.group1_format.setForeground(QColor(220, 20, 20))  # Red - parameters 1-3
        
        self.group2_format = QTextCharFormat()
        self.group2_format.setForeground(QColor(0, 150, 0))  # Green - parameters 4-6
        
        self.group3_format = QTextCharFormat()
        self.group3_format.setForeground(QColor(20, 20, 220))  # Blue - parameters 7-9
        
        self.group4_format = QTextCharFormat()
        self.group4_format.setForeground(QColor(200, 100, 0))  # Orange - parameters 10+
        
        self.comment_format = QTextCharFormat()
        self.comment_format.setForeground(QColor(128, 128, 128))  # Gray
        self.comment_format.setFontItalic(True)
    
    def get_format_for_position(self, param_index):
        """Get the appropriate format based on parameter group (0-based)"""
        group = param_index // 3  # Integer division to get group number
        if group == 0:  # Group 1: parameters 0-2 (first triplet)
            return self.group1_format
        elif group == 1:  # Group 2: parameters 3-5 (second triplet)
            return self.group2_format
        elif group == 2:  # Group 3: parameters 6-8 (third triplet)
            return self.group3_format
        else:  # Group 4+: parameters 9+ (remaining)
            return self.group4_format
    
    def highlightBlock(self, text):
        """Apply context-aware syntax highlighting to a block of text"""
        # Handle comments first
        if text.strip().startswith('#'):
            self.setFormat(0, len(text), self.comment_format)
            return
        
        # Split line into tokens
        tokens = text.split()
        if not tokens:
            return
        
        current_pos = 0
        command = tokens[0].lower()
        
        # Highlight command
        command_match = re.search(r'\b' + re.escape(tokens[0]) + r'\b', text)
        if command_match:
            self.setFormat(command_match.start(), command_match.end() - command_match.start(), self.command_format)
        
        # Get all parameters after command (assume all are numeric) 
        params = tokens[1:]
        
        # Apply test code logic: group parameters into triplets and color each triplet
        param_positions = []
        current_search_pos = 0
        
        # First pass: find all parameter positions in text
        for param in params:
            param_match = re.search(r'\b' + re.escape(param) + r'\b', text[current_search_pos:])
            if param_match:
                start_pos = current_search_pos + param_match.start()
                param_positions.append((start_pos, len(param)))
                current_search_pos = start_pos + len(param)
        
        # Second pass: apply triplet-based coloring using test code logic
        for i in range(0, len(params), 3):
            triplet = params[i:i+3]  # Get up to 3 parameters (same as test code)
            format_to_use = self.get_format_for_position(i)  # Color based on triplet group
            
            # Color each parameter in this triplet with the same color
            for j, param in enumerate(triplet):
                param_index = i + j  # Actual position in parameter list
                if param_index < len(param_positions):
                    start_pos, length = param_positions[param_index]
                    self.setFormat(start_pos, length, format_to_use)