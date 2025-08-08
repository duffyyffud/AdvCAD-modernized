#!/usr/bin/env python3
"""
AdvCAD .gm3d File Editor - Minimal Qt GUI
Simple text editor for .gm3d geometry files
"""

import sys
import os
from PyQt5.QtWidgets import (QApplication, QMainWindow, QTextEdit, 
                             QVBoxLayout, QWidget, QMenuBar, QAction,
                             QFileDialog, QMessageBox)
from PyQt5.QtCore import Qt, QRegExp
from PyQt5.QtGui import QFont, QSyntaxHighlighter, QTextCharFormat, QColor

class GM3DEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        self.current_file = None
        self.setup_ui()
        
    def setup_ui(self):
        """Initialize the user interface"""
        self.setWindowTitle("AdvCAD .gm3d Editor")
        self.setGeometry(100, 100, 800, 600)
        
        # Central widget - text editor
        self.text_editor = QTextEdit()
        self.text_editor.setFont(QFont("Consolas", 12))
        self.setCentralWidget(self.text_editor)
        
        # Menu bar
        self.setup_menu()
        
        # Load sample file if available
        sample_file = "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/sample/block.gm3d"
        if os.path.exists(sample_file):
            self.load_file(sample_file)
    
    def setup_menu(self):
        """Create menu bar with File operations"""
        menubar = self.menuBar()
        
        # File menu
        file_menu = menubar.addMenu('File')
        
        # New action
        new_action = QAction('New', self)
        new_action.setShortcut('Ctrl+N')
        new_action.triggered.connect(self.new_file)
        file_menu.addAction(new_action)
        
        # Open action
        open_action = QAction('Open...', self)
        open_action.setShortcut('Ctrl+O')
        open_action.triggered.connect(self.open_file)
        file_menu.addAction(open_action)
        
        # Save action
        save_action = QAction('Save', self)
        save_action.setShortcut('Ctrl+S')
        save_action.triggered.connect(self.save_file)
        file_menu.addAction(save_action)
        
        # Save As action
        save_as_action = QAction('Save As...', self)
        save_as_action.setShortcut('Ctrl+Shift+S')
        save_as_action.triggered.connect(self.save_file_as)
        file_menu.addAction(save_as_action)
        
        file_menu.addSeparator()
        
        # Exit action
        exit_action = QAction('Exit', self)
        exit_action.setShortcut('Ctrl+Q')
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)
    
    def new_file(self):
        """Create a new file"""
        self.text_editor.clear()
        self.current_file = None
        self.setWindowTitle("AdvCAD .gm3d Editor - New File")
    
    def open_file(self):
        """Open a .gm3d file"""
        file_path, _ = QFileDialog.getOpenFileName(
            self, 
            "Open .gm3d File", 
            "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/sample/",
            "GM3D Files (*.gm3d);;All Files (*)"
        )
        
        if file_path:
            self.load_file(file_path)
    
    def load_file(self, file_path):
        """Load content from file"""
        try:
            with open(file_path, 'r') as file:
                content = file.read()
                self.text_editor.setPlainText(content)
                self.current_file = file_path
                self.setWindowTitle(f"AdvCAD .gm3d Editor - {os.path.basename(file_path)}")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Could not open file:\n{str(e)}")
    
    def save_file(self):
        """Save current file"""
        if self.current_file:
            self.save_to_file(self.current_file)
        else:
            self.save_file_as()
    
    def save_file_as(self):
        """Save file with new name"""
        file_path, _ = QFileDialog.getSaveFileName(
            self,
            "Save .gm3d File",
            "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/tmp/",
            "GM3D Files (*.gm3d);;All Files (*)"
        )
        
        if file_path:
            self.save_to_file(file_path)
    
    def save_to_file(self, file_path):
        """Save content to file"""
        try:
            with open(file_path, 'w') as file:
                file.write(self.text_editor.toPlainText())
                self.current_file = file_path
                self.setWindowTitle(f"AdvCAD .gm3d Editor - {os.path.basename(file_path)}")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Could not save file:\n{str(e)}")

def main():
    app = QApplication(sys.argv)
    
    # Check if PyQt5 is available
    try:
        window = GM3DEditor()
        window.show()
        return app.exec_()
    except Exception as e:
        print(f"Error starting application: {e}")
        print("Make sure PyQt5 is installed: pip install PyQt5")
        return 1

if __name__ == "__main__":
    sys.exit(main())