"""
Autocomplete functionality for AdvCAD GM3D Editor
Provides intelligent command completion for .gm3d files
"""

from PyQt5.QtWidgets import QTextEdit, QCompleter, QAbstractItemView
from PyQt5.QtCore import QStringListModel, Qt
from PyQt5.QtGui import QTextCursor

class GM3DAutoCompleter:
    """Autocomplete data provider for GM3D commands"""
    
    def __init__(self):
        # Core GM3D commands with descriptions
        self.commands = {
            'box': 'box x y z width height depth - Create rectangular box',
            'sheet': 'sheet num_points x1 y1 z1 x2 y2 z2 ... - Create planar sheet',
            'circle': 'circle cx cy cz rx ry rz nx ny nz divisions - Create circle',
            'extrude': 'extrude dx dy dz - Extrude 2D shape into 3D',
            'revolve': 'revolve cx cy cz axis_x axis_y axis_z angle - Revolve around axis',
            'add': 'add - Boolean union of two objects',
            'subtract': 'subtract - Boolean difference of two objects',
            'intersect': 'intersect - Boolean intersection of two objects'
        }
        
        # Common coordinate patterns
        self.coordinate_patterns = [
            '0.0 0.0 0.0',
            '1.0 0.0 0.0',
            '0.0 1.0 0.0', 
            '0.0 0.0 1.0',
            '-1.0 0.0 0.0',
            '0.0 -1.0 0.0',
            '0.0 0.0 -1.0'
        ]
        
        # Common radius vectors for circle command
        self.radius_vectors = [
            '1.0 0.0 0.0',   # Unit radius in X direction
            '0.0 1.0 0.0',   # Unit radius in Y direction
            '0.0 0.0 1.0',   # Unit radius in Z direction
            '10.0 0.0 0.0',  # 10-unit radius in X direction
            '0.0 10.0 0.0',  # 10-unit radius in Y direction
            '5.0 0.0 0.0',   # 5-unit radius in X direction
            '0.0 5.0 0.0'    # 5-unit radius in Y direction
        ]
        
        # Common normal vectors for circle command (defines plane)
        self.normal_vectors = [
            '0.0 0.0 1.0',   # XY plane (most common)
            '1.0 0.0 0.0',   # YZ plane
            '0.0 1.0 0.0',   # XZ plane
            '0.0 0.0 -1.0',  # XY plane (inverted)
            '-1.0 0.0 0.0',  # YZ plane (inverted)
            '0.0 -1.0 0.0'   # XZ plane (inverted)
        ]
        
        # Common parameter values
        self.parameter_values = [
            '360',  # Full rotation
            '180',  # Half rotation
            '90',   # Quarter rotation
            '2.0',  # Common mesh size
            '1.0',  # Unit value
            '0.5',  # Half unit
            '32',   # Circle divisions
            '16',   # Lower circle divisions
            '64'    # Higher circle divisions
        ]
    
    def get_completions(self, text_before_cursor):
        """Get completion suggestions based on current context"""
        completions = []
        
        # Get current line and word
        lines = text_before_cursor.split('\n')
        current_line = lines[-1] if lines else ''
        words = current_line.split()
        
        if not words:
            # Start of line - suggest commands
            return list(self.commands.keys())
        
        last_word = words[-1]
        
        # If we're typing a command at start of line
        if len(words) == 1:
            matching_commands = [cmd for cmd in self.commands.keys() 
                               if cmd.startswith(last_word.lower())]
            return matching_commands
        
        # Context-sensitive completions based on command
        command = words[0].lower()
        
        if command in self.commands:
            # Get position in command parameters
            param_pos = len(words) - 1
            
            if command == 'box':
                if param_pos <= 6:  # x y z width height depth
                    return ['1.0', '2.0', '0.5', '10.0']
            
            elif command == 'circle':
                if param_pos == 1:  # Start of center coordinates - suggest triplets
                    return self.coordinate_patterns
                elif param_pos == 4:  # Start of radius vector - suggest triplets
                    return self.radius_vectors
                elif param_pos == 7:  # Start of normal vector - suggest triplets
                    return self.normal_vectors
                elif param_pos == 10:  # divisions
                    return ['16', '32', '64']
            
            elif command == 'extrude':
                if param_pos <= 3:  # dx dy dz
                    return ['0.0', '1.0', '2.0', '5.0']
            
            elif command == 'revolve':
                if param_pos <= 3:  # cx cy cz
                    return ['0.0']
                elif param_pos <= 6:  # axis vector
                    return self.coordinate_patterns
                elif param_pos == 7:  # angle
                    return ['360', '180', '90', '270']
            
            elif command in ['add', 'subtract', 'intersect']:
                # No additional parameters needed
                return []
        
        # Default numeric completions
        if last_word.replace('.', '').replace('-', '').isdigit():
            return self.parameter_values
        
        return []

class GM3DTextEdit(QTextEdit):
    """Enhanced QTextEdit with GM3D autocomplete functionality"""
    
    def __init__(self, parent=None):
        super().__init__(parent)
        
        # Initialize autocomplete
        self.autocompleter = GM3DAutoCompleter()
        self.completer = QCompleter()
        self.completer.setWidget(self)
        self.completer.setCompletionMode(QCompleter.PopupCompletion)
        self.completer.setCaseSensitivity(Qt.CaseInsensitive)
        self.completer.activated.connect(self.insert_completion)
        
        # Model for completions
        self.completion_model = QStringListModel()
        self.completer.setModel(self.completion_model)
        
    def keyPressEvent(self, event):
        """Handle key press events for autocomplete"""
        # Handle completer popup
        if self.completer.popup().isVisible():
            # Handle TAB key to accept completion
            if event.key() == Qt.Key_Tab:
                # Get current selection and insert it
                current_completion = self.completer.currentCompletion()
                if current_completion:
                    self.insert_completion(current_completion)
                    self.completer.popup().hide()
                event.accept()
                return
            
            # Keys that should close completer
            if event.key() in (Qt.Key_Enter, Qt.Key_Return, Qt.Key_Escape, 
                              Qt.Key_Backtab):
                event.ignore()
                return
        
        # Call parent implementation
        super().keyPressEvent(event)
        
        # Trigger autocomplete on certain conditions
        text = event.text()
        if text and (text.isalnum() or text in (' ', '.')):
            self.update_completions()
        elif event.key() == Qt.Key_Space and event.modifiers() == Qt.ControlModifier:
            # Ctrl+Space to force show completions
            self.update_completions()
    
    def update_completions(self):
        """Update completion suggestions based on current cursor position"""
        cursor = self.textCursor()
        
        # Get text before cursor
        text_before_cursor = self.toPlainText()[:cursor.position()]
        
        # Get completions from autocompleter
        completions = self.autocompleter.get_completions(text_before_cursor)
        
        if not completions:
            self.completer.popup().hide()
            return
        
        # Update completion model
        self.completion_model.setStringList(completions)
        
        # Get current word to complete
        lines = text_before_cursor.split('\n')
        current_line = lines[-1] if lines else ''
        words = current_line.split()
        current_word = words[-1] if words else ''
        
        # Set completion prefix
        self.completer.setCompletionPrefix(current_word)
        
        # Show popup if we have matches
        if self.completer.completionCount() > 0:
            # Position popup
            cursor_rect = self.cursorRect()
            cursor_rect.setWidth(self.completer.popup().sizeHintForColumn(0) + 
                                self.completer.popup().verticalScrollBar().sizeHint().width())
            self.completer.complete(cursor_rect)
        else:
            self.completer.popup().hide()
    
    def insert_completion(self, completion):
        """Insert the selected completion"""
        cursor = self.textCursor()
        
        # Get text before cursor
        text_before_cursor = self.toPlainText()[:cursor.position()]
        lines = text_before_cursor.split('\n')
        current_line = lines[-1] if lines else ''
        words = current_line.split()
        
        if words:
            # Replace current word
            current_word = words[-1]
            # Move cursor back to start of current word
            for _ in range(len(current_word)):
                cursor.movePosition(QTextCursor.Left)
            # Delete current word
            for _ in range(len(current_word)):
                cursor.deleteChar()
        
        # Insert completion
        cursor.insertText(completion)
        
        # Add space after command completions or triplet completions
        if completion in self.autocompleter.commands or ' ' in completion:
            cursor.insertText(' ')
        
        self.setTextCursor(cursor)