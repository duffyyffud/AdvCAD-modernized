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
        """Get the appropriate format based on parameter position (0-based)"""
        if param_index < 3:  # Parameters 1-3
            return self.group1_format
        elif param_index < 6:  # Parameters 4-6
            return self.group2_format
        elif param_index < 9:  # Parameters 7-9
            return self.group3_format
        else:  # Parameters 10+
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
        
        # Apply position-based coloring to numeric parameters
        numeric_tokens = []
        for i, token in enumerate(tokens[1:]):  # Skip command token
            if re.match(r'-?\d+\.?\d*$', token):  # Check if token is a number
                numeric_tokens.append((i, token))
        
        # Color each numeric parameter based on its position
        for param_index, token in numeric_tokens:
            format_to_use = self.get_format_for_position(param_index)
            
            # Find this token in the original text
            token_pattern = re.escape(token)
            matches = list(re.finditer(r'\b' + token_pattern + r'\b', text))
            
            # Use the appropriate match (account for previous tokens)
            if len(matches) > param_index:
                match = matches[param_index + 1] if param_index + 1 < len(matches) else matches[-1]
                self.setFormat(match.start(), match.end() - match.start(), format_to_use)