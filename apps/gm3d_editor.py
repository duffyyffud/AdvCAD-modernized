#!/usr/bin/env python3
"""
Advanced CAD .gm3d File Editor with 3D Preview
A modern PyQt5-based editor for AdvCAD geometry files with real-time 3D mesh visualization.

Features:
- Syntax highlighting for .gm3d files
- Interactive 3D mesh preview with OpenGL
- CSG tree visualization
- Primitive shape creation dialogs
- Mesh generation integration
- Configurable timeout and mesh size settings
"""

import sys
import os
import subprocess
import platform

# Add apps directory to Python path for module imports
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# WSL2 OpenGL fix
if platform.system() == "Linux" and "microsoft" in platform.uname().release.lower():
    if not any(x in os.environ for x in ['DISPLAY', 'WAYLAND_DISPLAY']):
        os.environ['LIBGL_ALWAYS_SOFTWARE'] = '1'
        os.environ['GALLIUM_DRIVER'] = 'llvmpipe'
        print("WSL2 detected - using software rendering")

from PyQt5.QtWidgets import (QApplication, QMainWindow, QVBoxLayout, 
                             QWidget, QAction, QToolBar, QFileDialog, QMessageBox, 
                             QProgressDialog, QHBoxLayout, QTreeWidget, QTreeWidgetItem, 
                             QSplitter, QLabel, QDialog, QDialogButtonBox, QFormLayout, 
                             QPushButton, QSpinBox)
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QFont, QTextCursor

# Import our modular components
from gm3d_dialogs import (PrimitiveShapeDialog, CylinderDialog, SphereDialog, 
                         BoxDialog, TorusDialog)
from gm3d_parsers import CSGParser, PCHParser
from gm3d_viewer import MeshViewer3D, OPENGL_AVAILABLE
from gm3d_syntax import GM3DSyntaxHighlighter
from gm3d_autocomplete import GM3DTextEdit

class GM3DEditor(QMainWindow):
    """Main application window for GM3D file editing"""
    
    def __init__(self):
        super().__init__()
        self.current_file = None
        # Default settings
        self.time_limit_seconds = 60  # Default 60 seconds
        self.default_mesh_size = 2.0  # Default mesh size
        self.setup_ui()
        
    def setup_ui(self):
        """Initialize the user interface"""
        self.setWindowTitle("AdvCAD .gm3d Editor with 3D Preview")
        self.setGeometry(100, 100, 1600, 800)
        
        # Create main splitter for layout
        self.main_splitter = QSplitter(Qt.Horizontal)
        
        # Left panel - CSG Tree View
        self.tree_widget = QTreeWidget()
        self.tree_widget.setHeaderLabels(['CSG Operation Tree', 'Parameters', 'Type'])
        self.tree_widget.setMinimumWidth(300)
        self.tree_widget.itemClicked.connect(self.on_tree_item_clicked)
        
        # Center panel - text editor with autocomplete
        self.text_editor = GM3DTextEdit()
        self.text_editor.setFont(QFont("Consolas", 12))
        self.text_editor.textChanged.connect(self.update_csg_tree)
        self.text_editor.cursorPositionChanged.connect(self.on_cursor_position_changed)
        
        # Add syntax highlighting
        self.highlighter = GM3DSyntaxHighlighter(self.text_editor.document())
        
        # Initialize parsers BEFORE setting up panels
        self.csg_parser = CSGParser()
        self.pch_parser = PCHParser()
        
        # Right panel - 3D Mesh Preview
        self.setup_3d_panel()
        
        # Add panels to splitter
        self.main_splitter.addWidget(self.tree_widget)
        self.main_splitter.addWidget(self.text_editor)
        self.main_splitter.addWidget(self.mesh_panel)
        
        # Set splitter proportions (tree: 20%, editor: 50%, 3D: 30%)
        self.main_splitter.setSizes([300, 800, 500])
        
        # Set as central widget
        self.setCentralWidget(self.main_splitter)
        
        # Menu bar
        self.setup_menu()
        
        # Tool bar
        self.setup_toolbar()
        
        # Load working memory file if available
        working_file = "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/sample/test_autocomplete.gm3d"
        if os.path.exists(working_file):
            self.load_file(working_file)
    
    def setup_3d_panel(self):
        """Setup 3D mesh preview panel"""
        self.mesh_panel = QWidget()
        layout = QVBoxLayout()
        self.mesh_panel.setLayout(layout)
        
        if OPENGL_AVAILABLE:
            # Create 3D viewer
            self.mesh_viewer = MeshViewer3D()
            layout.addWidget(self.mesh_viewer)
        else:
            # OpenGL not available - show placeholder
            fallback_label = QLabel("OpenGL not available\nInstall PyOpenGL for 3D preview")
            fallback_label.setAlignment(Qt.AlignCenter)
            fallback_label.setStyleSheet("color: red; font-style: italic;")
            layout.addWidget(fallback_label)
            self.mesh_viewer = None
        
        # Control buttons
        button_layout = QHBoxLayout()
        layout.addLayout(button_layout)
    
    def setup_menu(self):
        """Create menu bar"""
        menubar = self.menuBar()
        
        # File menu
        file_menu = menubar.addMenu('File')
        
        # New file action
        new_action = QAction('New', self)
        new_action.setShortcut('Ctrl+N')
        new_action.triggered.connect(self.new_file)
        file_menu.addAction(new_action)
        
        # Open file action
        open_action = QAction('Open...', self)
        open_action.setShortcut('Ctrl+O')
        open_action.triggered.connect(self.open_file)
        file_menu.addAction(open_action)
        
        # Save file action
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
        
        # Edit menu
        edit_menu = menubar.addMenu('Edit')
        
        # Undo action
        undo_action = QAction('Undo', self)
        undo_action.setShortcut('Ctrl+Z')
        undo_action.triggered.connect(self.text_editor.undo)
        edit_menu.addAction(undo_action)
        
        # Redo action
        redo_action = QAction('Redo', self)
        redo_action.setShortcut('Ctrl+Y')
        redo_action.triggered.connect(self.text_editor.redo)
        edit_menu.addAction(redo_action)
        
        # View menu
        view_menu = menubar.addMenu('View')
        
        if OPENGL_AVAILABLE:
            # Wireframe toggle
            wireframe_action = QAction('Toggle Wireframe', self)
            wireframe_action.setShortcut('W')
            wireframe_action.triggered.connect(self.toggle_wireframe_mode)
            view_menu.addAction(wireframe_action)
            
            # Reset 3D view
            reset_view_action = QAction('Reset 3D View', self)
            reset_view_action.setShortcut('R')
            reset_view_action.triggered.connect(self.reset_3d_view)
            view_menu.addAction(reset_view_action)
        
        # Settings menu
        settings_menu = menubar.addMenu('Settings')
        
        # Time limit setting
        time_limit_action = QAction('Configure Time Limit...', self)
        time_limit_action.triggered.connect(self.configure_time_limit)
        settings_menu.addAction(time_limit_action)
        
        # Default mesh size setting
        mesh_size_action = QAction('Default Mesh Size...', self)
        mesh_size_action.triggered.connect(self.configure_default_mesh_size)
        settings_menu.addAction(mesh_size_action)
        
        # Help menu
        help_menu = menubar.addMenu('Help')
        
        # Autocomplete help
        autocomplete_help_action = QAction('Autocomplete Help', self)
        autocomplete_help_action.triggered.connect(self.show_autocomplete_help)
        help_menu.addAction(autocomplete_help_action)
    
    def setup_toolbar(self):
        """Create toolbar with common actions"""
        toolbar = self.addToolBar('Main')
        
        # Primitive shapes toolbar
        shapes_toolbar = self.addToolBar('Primitives')
        
        # Cylinder button
        cylinder_action = QAction('Cylinder', self)
        cylinder_action.setToolTip('Create a cylinder primitive')
        cylinder_action.triggered.connect(self.create_cylinder)
        shapes_toolbar.addAction(cylinder_action)
        
        # Sphere button
        sphere_action = QAction('Sphere', self)
        sphere_action.setToolTip('Create a sphere primitive')
        sphere_action.triggered.connect(self.create_sphere)
        shapes_toolbar.addAction(sphere_action)
        
        # Box button
        box_action = QAction('Box', self)
        box_action.setToolTip('Create a box primitive')
        box_action.triggered.connect(self.create_box)
        shapes_toolbar.addAction(box_action)
        
        # Torus button
        torus_action = QAction('Torus', self)
        torus_action.setToolTip('Create a torus primitive')
        torus_action.triggered.connect(self.create_torus)
        shapes_toolbar.addAction(torus_action)
        
        # Separator
        toolbar.addSeparator()
        
        # Run mesh generation
        run_action = QAction('Generate Mesh', self)
        run_action.setToolTip('Generate mesh from .gm3d file')
        run_action.triggered.connect(self.generate_mesh)
        toolbar.addAction(run_action)
    
    def new_file(self):
        """Create new file"""
        self.text_editor.clear()
        self.current_file = None
        self.setWindowTitle("AdvCAD .gm3d Editor - New File")
    
    def open_file(self):
        """Open file dialog and load selected file"""
        file_path, _ = QFileDialog.getOpenFileName(
            self, "Open .gm3d File", "", "GM3D Files (*.gm3d);;All Files (*)"
        )
        if file_path:
            self.load_file(file_path)
    
    def load_file(self, file_path):
        """Load file content into editor"""
        try:
            with open(file_path, 'r') as f:
                content = f.read()
            self.text_editor.setPlainText(content)
            self.current_file = file_path
            self.setWindowTitle(f"AdvCAD .gm3d Editor - {os.path.basename(file_path)}")
            
            # Update CSG tree
            QTimer.singleShot(100, self.update_csg_tree)
            
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Could not open file:\n{str(e)}")
    
    def save_file(self):
        """Save current file"""
        if not self.current_file:
            self.save_file_as()
            return
        
        try:
            with open(self.current_file, 'w') as f:
                f.write(self.text_editor.toPlainText())
            # Silent save for working memory files
            if not self.current_file.endswith('test_autocomplete.gm3d'):
                QMessageBox.information(self, "Saved", f"File saved: {self.current_file}")
        except Exception as e:
            QMessageBox.critical(self, "Error", f"Could not save file:\n{str(e)}")
    
    def save_file_as(self):
        """Save file with new name"""
        file_path, _ = QFileDialog.getSaveFileName(
            self, "Save .gm3d File", "", "GM3D Files (*.gm3d);;All Files (*)"
        )
        if file_path:
            self.current_file = file_path
            self.save_file()
            self.setWindowTitle(f"AdvCAD .gm3d Editor - {os.path.basename(file_path)}")
    
    def generate_mesh(self):
        """Generate mesh using AdvCAD backend"""
        if not self.current_file:
            # Need to save first
            QMessageBox.information(self, "Info", "Please save the file first before generating mesh.")
            self.save_file_as()
            if not self.current_file:
                return
        else:
            # Save current changes before generating mesh
            self.save_file()
        
        # Check if AdvCAD executable exists
        advcad_path = "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad"
        if not os.path.exists(advcad_path):
            QMessageBox.critical(self, "Error", f"AdvCAD executable not found at:\n{advcad_path}\n\nPlease build AdvCAD first.")
            return
        
        # Generate output filename
        base_name = os.path.splitext(self.current_file)[0]
        output_file = f"{base_name}_output.pch"
        
        # Use configured default mesh size
        mesh_size = str(self.default_mesh_size)
        
        try:
            # Run AdvCAD command
            cmd = [advcad_path, self.current_file, output_file, mesh_size]
            
            # Show progress dialog with time limit info
            progress = QProgressDialog(f"Generating mesh (timeout: {self.time_limit_seconds}s)...", "Cancel", 0, 0, self)
            progress.setWindowModality(Qt.WindowModal)
            progress.show()
            
            # Run the command with configured timeout
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=self.time_limit_seconds)
            progress.close()
            
            if result.returncode == 0:
                # Load the generated mesh in 3D viewer
                self.load_mesh_in_viewer(output_file)
                QMessageBox.information(self, "Success", f"Mesh generated successfully!\n\nOutput: {output_file}\n\nAdvCAD output:\n{result.stdout[:500]}")
            else:
                QMessageBox.critical(self, "Mesh Generation Failed", f"AdvCAD returned error code {result.returncode}\n\nError output:\n{result.stderr[:500]}")
                
        except subprocess.TimeoutExpired:
            progress.close()
            # Show timeout dialog with mesh size adjustment option
            self.show_timeout_dialog(mesh_size)
        except Exception as e:
            progress.close()
            QMessageBox.critical(self, "Error", f"Failed to run mesh generation:\n{str(e)}")
    
    def load_mesh_in_viewer(self, pch_file_path):
        """Load generated mesh in 3D viewer"""
        if not self.mesh_viewer:
            return
            
        if self.pch_parser.parse_pch_file(pch_file_path):
            print(f"Loaded mesh: {len(self.pch_parser.vertices)} vertices, {len(self.pch_parser.faces)} faces")
            self.mesh_viewer.load_mesh(self.pch_parser.vertices, self.pch_parser.faces)
        else:
            QMessageBox.warning(self, "Warning", f"Could not load mesh from {pch_file_path}")
    
    def update_csg_tree(self):
        """Update CSG tree visualization"""
        content = self.text_editor.toPlainText()
        self.csg_parser.parse(content)
        
        # Clear existing tree
        self.tree_widget.clear()
        
        # Add nodes to tree widget
        for node in self.csg_parser.all_nodes:
            item = QTreeWidgetItem([node.command, node.params, node.type])
            item.setData(0, Qt.UserRole, node.line)  # Store line number
            self.tree_widget.addTopLevelItem(item)
        
        # Expand all items
        self.tree_widget.expandAll()
    
    def on_tree_item_clicked(self, item):
        """Handle tree item click to navigate to code line"""
        line_number = item.data(0, Qt.UserRole)
        if line_number:
            # Move cursor to the corresponding line
            cursor = self.text_editor.textCursor()
            cursor.movePosition(QTextCursor.Start)
            for _ in range(line_number - 1):
                cursor.movePosition(QTextCursor.Down)
            self.text_editor.setTextCursor(cursor)
    
    def on_cursor_position_changed(self):
        """Handle cursor position change to highlight corresponding tree item"""
        cursor = self.text_editor.textCursor()
        current_line = cursor.blockNumber() + 1
        
        # Find and highlight corresponding tree item
        for i in range(self.tree_widget.topLevelItemCount()):
            item = self.tree_widget.topLevelItem(i)
            line_number = item.data(0, Qt.UserRole)
            if line_number == current_line:
                self.tree_widget.setCurrentItem(item)
                break
    
    def toggle_wireframe_mode(self):
        """Toggle wireframe display mode"""
        if self.mesh_viewer:
            self.mesh_viewer.toggle_wireframe()
    
    def reset_3d_view(self):
        """Reset 3D view to default"""
        if self.mesh_viewer:
            self.mesh_viewer.reset_view()
    
    def create_cylinder(self):
        """Create cylinder primitive via dialog"""
        dialog = CylinderDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            gm3d_code = dialog.get_gm3d_code()
            self.insert_code_at_cursor(gm3d_code)
    
    def create_sphere(self):
        """Create sphere primitive via dialog"""
        dialog = SphereDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            gm3d_code = dialog.get_gm3d_code()
            self.insert_code_at_cursor(gm3d_code)
    
    def create_box(self):
        """Create box primitive via dialog"""
        dialog = BoxDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            gm3d_code = dialog.get_gm3d_code()
            self.insert_code_at_cursor(gm3d_code)
    
    def create_torus(self):
        """Create torus primitive via dialog"""
        dialog = TorusDialog(self)
        if dialog.exec_() == QDialog.Accepted:
            gm3d_code = dialog.get_gm3d_code()
            self.insert_code_at_cursor(gm3d_code)
    
    def insert_code_at_cursor(self, code):
        """Insert generated code at current cursor position"""
        cursor = self.text_editor.textCursor()
        
        # Add newlines if not at start of document
        if cursor.position() > 0:
            cursor.movePosition(QTextCursor.EndOfLine)
            code = "\n\n" + code
        
        cursor.insertText(code)
        
        # Update CSG tree after insertion
        self.update_csg_tree()
    
    def configure_time_limit(self):
        """Show dialog to configure time limit for mesh generation"""
        dialog = QDialog(self)
        dialog.setWindowTitle("Configure Time Limit")
        dialog.setModal(True)
        
        layout = QVBoxLayout()
        
        # Time limit input
        form_layout = QFormLayout()
        time_spin = QSpinBox()
        time_spin.setRange(1, 600)  # 1 second to 10 minutes
        time_spin.setValue(self.time_limit_seconds)
        time_spin.setSuffix(" seconds")
        form_layout.addRow("Time Limit:", time_spin)
        
        layout.addLayout(form_layout)
        
        # Buttons
        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(dialog.accept)
        buttons.rejected.connect(dialog.reject)
        layout.addWidget(buttons)
        
        dialog.setLayout(layout)
        
        if dialog.exec_() == QDialog.Accepted:
            self.time_limit_seconds = time_spin.value()
            QMessageBox.information(self, "Settings Updated", 
                                   f"Time limit set to {self.time_limit_seconds} seconds")
    
    def configure_default_mesh_size(self):
        """Show dialog to configure default mesh size"""
        from PyQt5.QtWidgets import QDoubleSpinBox
        
        dialog = QDialog(self)
        dialog.setWindowTitle("Configure Default Mesh Size")
        dialog.setModal(True)
        
        layout = QVBoxLayout()
        
        # Mesh size input
        form_layout = QFormLayout()
        mesh_spin = QDoubleSpinBox()
        mesh_spin.setRange(0.001, 100.0)
        mesh_spin.setValue(self.default_mesh_size)
        mesh_spin.setDecimals(3)
        mesh_spin.setSingleStep(0.1)
        form_layout.addRow("Default Mesh Size:", mesh_spin)
        
        layout.addLayout(form_layout)
        
        # Buttons
        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(dialog.accept)
        buttons.rejected.connect(dialog.reject)
        layout.addWidget(buttons)
        
        dialog.setLayout(layout)
        
        if dialog.exec_() == QDialog.Accepted:
            self.default_mesh_size = mesh_spin.value()
            QMessageBox.information(self, "Settings Updated", 
                                   f"Default mesh size set to {self.default_mesh_size}")
    
    def show_timeout_dialog(self, current_mesh_size):
        """Show dialog when mesh generation times out, allowing mesh size adjustment"""
        from PyQt5.QtWidgets import QDoubleSpinBox
        
        dialog = QDialog(self)
        dialog.setWindowTitle("Mesh Generation Timeout")
        dialog.setModal(True)
        
        layout = QVBoxLayout()
        
        # Timeout message
        msg_label = QLabel(f"Mesh generation timed out after {self.time_limit_seconds} seconds.\n\n"
                          f"This often happens when the mesh size is too small for complex geometry.\n"
                          f"Try increasing the mesh size or extending the time limit.")
        layout.addWidget(msg_label)
        
        # Options
        form_layout = QFormLayout()
        
        # New mesh size
        mesh_spin = QDoubleSpinBox()
        mesh_spin.setRange(0.001, 100.0)
        mesh_spin.setValue(float(current_mesh_size) * 2)  # Suggest doubling the mesh size
        mesh_spin.setDecimals(3)
        mesh_spin.setSingleStep(0.1)
        form_layout.addRow("New Mesh Size:", mesh_spin)
        
        # New time limit
        time_spin = QSpinBox()
        time_spin.setRange(1, 600)
        time_spin.setValue(min(self.time_limit_seconds * 2, 600))  # Suggest doubling, max 10 min
        time_spin.setSuffix(" seconds")
        form_layout.addRow("New Time Limit:", time_spin)
        
        layout.addLayout(form_layout)
        
        # Buttons
        button_layout = QHBoxLayout()
        retry_btn = QPushButton("Retry with New Settings")
        cancel_btn = QPushButton("Cancel")
        button_layout.addWidget(retry_btn)
        button_layout.addWidget(cancel_btn)
        layout.addLayout(button_layout)
        
        dialog.setLayout(layout)
        
        # Connect buttons
        retry_btn.clicked.connect(dialog.accept)
        cancel_btn.clicked.connect(dialog.reject)
        
        if dialog.exec_() == QDialog.Accepted:
            # Temporarily update settings for this run
            new_mesh_size = mesh_spin.value()
            new_time_limit = time_spin.value()
            
            # Save current settings
            old_mesh_size = self.default_mesh_size
            old_time_limit = self.time_limit_seconds
            
            # Apply new settings temporarily
            self.default_mesh_size = new_mesh_size
            self.time_limit_seconds = new_time_limit
            
            # Retry mesh generation
            self.generate_mesh()
            
            # Restore original settings
            self.default_mesh_size = old_mesh_size
            self.time_limit_seconds = old_time_limit
    
    def show_autocomplete_help(self):
        """Show autocomplete help dialog"""
        help_text = """
<h2>Autocomplete Help</h2>

<p>The GM3D editor provides intelligent autocomplete for commands and parameters:</p>

<h3>How to Use Autocomplete:</h3>
<ul>
<li><b>Start typing</b> - Completions appear automatically</li>
<li><b>Ctrl+Space</b> - Force show completions</li>
<li><b>Arrow keys</b> - Navigate completion list</li>
<li><b>Enter/Tab</b> - Accept selected completion</li>
<li><b>Escape</b> - Close completion list</li>
</ul>

<h3>Supported Commands:</h3>
<ul>
<li><b>box</b> - Create rectangular box</li>
<li><b>circle</b> - Create circular sheet</li>
<li><b>extrude</b> - Extrude 2D shape into 3D</li>
<li><b>revolve</b> - Revolve around axis</li>
<li><b>add</b> - Boolean union</li>
<li><b>subtract</b> - Boolean difference</li>
</ul>

<h3>Smart Parameter Completion:</h3>
<ul>
<li><b>Circle Command</b> - Context-aware suggestions for center, radius vector, normal vector, divisions</li>
<li><b>Coordinates</b> - Common coordinate values and vectors</li>
<li><b>Radius Vectors</b> - Standard radius directions (1.0 0.0 0.0, 0.0 1.0 0.0, etc.)</li>
<li><b>Normal Vectors</b> - Plane orientations (0.0 0.0 1.0 for XY plane, etc.)</li>
<li><b>Angles</b> - 90, 180, 270, 360 degrees</li>
<li><b>Divisions</b> - 16, 32, 64 for circles</li>
<li><b>Sizes</b> - 0.5, 1.0, 2.0 for dimensions</li>
</ul>

<p><i>Autocomplete adapts to command context for relevant suggestions!</i></p>
        """
        
        QMessageBox.information(self, "Autocomplete Help", help_text)

def main():
    app = QApplication(sys.argv)
    
    # Check if PyQt5 is available
    try:
        window = GM3DEditor()
        window.show()
        return app.exec_()
    except Exception as e:
        print(f"Error starting application: {e}")
        return 1

if __name__ == '__main__':
    sys.exit(main())