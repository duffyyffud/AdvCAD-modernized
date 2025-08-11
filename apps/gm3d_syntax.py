"""
Syntax highlighting for AdvCAD GM3D Editor
"""

from PyQt5.QtCore import QRegExp
from PyQt5.QtGui import QSyntaxHighlighter, QTextCharFormat, QColor, QFont

class GM3DSyntaxHighlighter(QSyntaxHighlighter):
    """Syntax highlighter for .gm3d files"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        self.highlighting_rules = []
        
        # Command format (blue)
        command_format = QTextCharFormat()
        command_format.setForeground(QColor(0, 100, 200))  # Blue
        command_format.setFontWeight(QFont.Bold)
        commands = ['box', 'sheet', 'circle', 'extrude', 'revolve', 'add', 'subtract']
        for command in commands:
            pattern = QRegExp(f'\\b{command}\\b')
            self.highlighting_rules.append((pattern, command_format))
        
        # Numbers format (dark green)
        number_format = QTextCharFormat()
        number_format.setForeground(QColor(0, 128, 0))  # Dark green
        number_pattern = QRegExp(r'-?\d+\.?\d*')
        self.highlighting_rules.append((number_pattern, number_format))
        
        # Comment format (gray) - lines starting with #
        comment_format = QTextCharFormat()
        comment_format.setForeground(QColor(128, 128, 128))  # Gray
        comment_format.setFontItalic(True)
        comment_pattern = QRegExp('#.*')
        self.highlighting_rules.append((comment_pattern, comment_format))
    
    def highlightBlock(self, text):
        """Apply syntax highlighting to a block of text"""
        for pattern, format_obj in self.highlighting_rules:
            expression = QRegExp(pattern)
            index = expression.indexIn(text)
            while index >= 0:
                length = expression.matchedLength()
                self.setFormat(index, length, format_obj)
                index = expression.indexIn(text, index + length)