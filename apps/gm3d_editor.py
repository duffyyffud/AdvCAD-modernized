#!/usr/bin/env python3
"""
AdvCAD .gm3d File Editor - Minimal Qt GUI
Simple text editor for .gm3d geometry files
"""

import sys
import os

# Enable software rendering for WSL2
if os.path.exists('/proc/version'):
    with open('/proc/version', 'r') as f:
        if 'microsoft' in f.read().lower():
            os.environ['LIBGL_ALWAYS_SOFTWARE'] = '1'
            os.environ['GALLIUM_DRIVER'] = 'llvmpipe'
            print("WSL2 detected - using software rendering")
from PyQt5.QtWidgets import (QApplication, QMainWindow, QTextEdit, QPlainTextEdit,
                             QVBoxLayout, QWidget, QMenuBar, QAction, QToolBar,
                             QFileDialog, QMessageBox, QProgressDialog, QHBoxLayout,
                             QCompleter, QTreeWidget, QTreeWidgetItem, QSplitter, QLabel,
                             QDialog, QDialogButtonBox, QFormLayout, QDoubleSpinBox, QComboBox,
                             QPushButton, QMenu)
from PyQt5.QtCore import Qt, QRegExp, QProcess, QRect, QSize, QStringListModel, QTimer
from PyQt5.QtGui import (QFont, QSyntaxHighlighter, QTextCharFormat, QColor, 
                         QPainter, QTextFormat, QTextCursor)
try:
    # Try legacy OpenGL (more compatible)
    from PyQt5.QtOpenGL import QGLWidget as OpenGLBaseWidget
    from OpenGL.GL import *
    import OpenGL.GL as gl
    import OpenGL.GLU as glu
    import numpy as np
    OPENGL_AVAILABLE = True
    OPENGL_LEGACY = True
    print("Using legacy QGLWidget for OpenGL")
except ImportError:
    try:
        # Try modern OpenGL as fallback
        from PyQt5.QtOpenGL import QOpenGLWidget as OpenGLBaseWidget
        from OpenGL.GL import *
        import OpenGL.GL as gl
        import OpenGL.GLU as glu
        import numpy as np
        OPENGL_AVAILABLE = True
        OPENGL_LEGACY = False
        print("Using modern QOpenGLWidget")
    except ImportError:
        OPENGL_AVAILABLE = False
        OPENGL_LEGACY = False
        OpenGLBaseWidget = QWidget
        print("No OpenGL support available")
import subprocess
import math

class PCHParser:
    """Parse .pch files for 3D mesh visualization"""
    
    def __init__(self):
        self.vertices = []
        self.faces = []
        
    def parse_pch_file(self, file_path):
        """Parse a .pch file and extract vertices and faces"""
        if not os.path.exists(file_path):
            return False
            
        self.vertices = []
        self.faces = []
        
        try:
            with open(file_path, 'r') as f:
                lines = f.readlines()
            
            if not lines:
                return False
            
            # First line is number of vertices
            num_vertices = int(lines[0].strip())
            
            # Read vertices
            for i in range(1, num_vertices + 1):
                if i < len(lines):
                    coords = lines[i].strip().split()
                    if len(coords) >= 3:
                        x, y, z = map(float, coords[:3])
                        self.vertices.append([x, y, z])
            
            # Find number of faces
            if num_vertices + 1 < len(lines):
                num_faces = int(lines[num_vertices + 1].strip())
                
                # Read faces
                for i in range(num_vertices + 2, num_vertices + 2 + num_faces):
                    if i < len(lines):
                        face_data = lines[i].strip().split()
                        if len(face_data) >= 3:
                            # Convert to 0-based indexing
                            face = [int(x) for x in face_data[:3]]
                            self.faces.append(face)
            
            return len(self.vertices) > 0 and len(self.faces) > 0
            
        except Exception as e:
            print(f"Error parsing PCH file: {e}")
            return False

class MeshViewer3D(OpenGLBaseWidget):
    """3D mesh viewer widget using OpenGL
    
    Controls:
    - Left Click + Drag: Rotate view
    - Right Click + Drag: Pan/translate view  
    - Mouse Wheel: Zoom (if supported)
    - W: Wireframe mode
    - S: Solid mode
    - R: Reset view
    - F: Fit to view
    - +/-: Zoom in/out (keyboard alternative)
    """
    
    def __init__(self):
        super().__init__()
        self.vertices = []
        self.faces = []
        self.rotation_x = -15  # Default rotation to show body from desired angle
        self.rotation_y = 15   # Default rotation to show body from desired angle
        self.zoom = 1.0
        self.pan_x = 0.0
        self.pan_y = 0.0
        self.wireframe_mode = True
        self.last_pos = None
        self.mouse_button = None
        
        # Enable keyboard focus
        self.setFocusPolicy(2)  # Qt.StrongFocus
        
    def load_mesh(self, vertices, faces):
        """Load mesh data for visualization"""
        self.vertices = vertices
        self.faces = faces
        if OPENGL_LEGACY:
            self.updateGL()  # Legacy method
        else:
            self.update()    # Modern method
        
    def initializeGL(self):
        """Initialize OpenGL settings"""
        if not OPENGL_AVAILABLE:
            return
            
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClearColor(0.2, 0.2, 0.2, 1.0)  # Dark gray background
        gl.glEnable(gl.GL_LIGHT0)
        gl.glEnable(gl.GL_LIGHTING)
        gl.glColorMaterial(gl.GL_FRONT_AND_BACK, gl.GL_AMBIENT_AND_DIFFUSE)
        gl.glEnable(gl.GL_COLOR_MATERIAL)
        
    def paintGL(self):
        """Render the 3D scene"""
        if not OPENGL_AVAILABLE:
            return
            
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
        gl.glLoadIdentity()
        
        # Apply transformations
        gl.glTranslatef(self.pan_x, self.pan_y, -5.0 * self.zoom)
        gl.glRotatef(self.rotation_x, 1.0, 0.0, 0.0)
        gl.glRotatef(self.rotation_y, 0.0, 1.0, 0.0)
        
        # Center the mesh
        if self.vertices:
            # Calculate bounding box
            if self.vertices:
                min_x = min(v[0] for v in self.vertices)
                max_x = max(v[0] for v in self.vertices)
                min_y = min(v[1] for v in self.vertices)
                max_y = max(v[1] for v in self.vertices)
                min_z = min(v[2] for v in self.vertices)
                max_z = max(v[2] for v in self.vertices)
                
                center_x = (min_x + max_x) / 2
                center_y = (min_y + max_y) / 2
                center_z = (min_z + max_z) / 2
                
                gl.glTranslatef(-center_x, -center_y, -center_z)
        
        # Draw mesh
        if self.wireframe_mode:
            gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_LINE)
            gl.glColor3f(0.0, 1.0, 0.0)  # Green wireframe
        else:
            gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_FILL)
            gl.glColor3f(0.7, 0.7, 0.9)  # Light blue solid
        
        # Render triangles
        if self.faces and self.vertices:
            # Debug: print(f"Rendering {len(self.faces)} faces, {len(self.vertices)} vertices, wireframe={self.wireframe_mode}")
            
            if self.wireframe_mode:
                # Wireframe mode - disable culling for all lines
                gl.glDisable(gl.GL_CULL_FACE)
            else:
                # Solid mode - enable proper face rendering
                gl.glEnable(gl.GL_CULL_FACE)
                gl.glCullFace(gl.GL_BACK)
                gl.glFrontFace(gl.GL_CCW)
                
                # Enable lighting for better solid appearance  
                gl.glEnable(gl.GL_LIGHTING)
                gl.glEnable(gl.GL_LIGHT0)
                gl.glLightfv(gl.GL_LIGHT0, gl.GL_POSITION, [1.0, 1.0, 1.0, 0.0])
                gl.glLightfv(gl.GL_LIGHT0, gl.GL_AMBIENT, [0.2, 0.2, 0.2, 1.0])
                gl.glLightfv(gl.GL_LIGHT0, gl.GL_DIFFUSE, [0.8, 0.8, 0.8, 1.0])
                
                # Set material properties
                gl.glMaterialfv(gl.GL_FRONT, gl.GL_AMBIENT_AND_DIFFUSE, [0.7, 0.7, 0.9, 1.0])
            
            # Render all triangles with both front and back faces
            gl.glBegin(gl.GL_TRIANGLES)
            for face in self.faces:
                if len(face) >= 3:  # Ensure valid triangle
                    # Calculate normal for lighting
                    if not self.wireframe_mode and len(face) == 3:
                        v0 = self.vertices[face[0]] if face[0] < len(self.vertices) else [0,0,0]
                        v1 = self.vertices[face[1]] if face[1] < len(self.vertices) else [0,0,0] 
                        v2 = self.vertices[face[2]] if face[2] < len(self.vertices) else [0,0,0]
                        
                        # Calculate normal vector
                        edge1 = [v1[0]-v0[0], v1[1]-v0[1], v1[2]-v0[2]]
                        edge2 = [v2[0]-v0[0], v2[1]-v0[1], v2[2]-v0[2]]
                        normal = [
                            edge1[1]*edge2[2] - edge1[2]*edge2[1],
                            edge1[2]*edge2[0] - edge1[0]*edge2[2], 
                            edge1[0]*edge2[1] - edge1[1]*edge2[0]
                        ]
                        
                        # Normalize
                        length = (normal[0]**2 + normal[1]**2 + normal[2]**2)**0.5
                        if length > 0:
                            normal = [normal[0]/length, normal[1]/length, normal[2]/length]
                            gl.glNormal3f(normal[0], normal[1], normal[2])
                    
                    # Render triangle vertices
                    for vertex_idx in face:
                        if vertex_idx < len(self.vertices):
                            vertex = self.vertices[vertex_idx]
                            gl.glVertex3f(vertex[0], vertex[1], vertex[2])
            gl.glEnd()
            
            # Disable lighting after solid rendering
            if not self.wireframe_mode:
                gl.glDisable(gl.GL_LIGHTING)
        
        # Draw coordinate axes (always visible)
        self.draw_coordinate_axes()
            
    def resizeGL(self, width, height):
        """Handle widget resize"""
        if not OPENGL_AVAILABLE:
            return
            
        gl.glViewport(0, 0, width, height)
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        
        # Set up perspective projection
        aspect = width / height if height != 0 else 1
        glu.gluPerspective(45, aspect, 0.1, 100.0)
        gl.glMatrixMode(gl.GL_MODELVIEW)
        
    def draw_coordinate_axes(self):
        """Draw coordinate axes (X=Red, Y=Green, Z=Blue)"""
        if not OPENGL_AVAILABLE:
            return
            
        # Save current matrix
        gl.glPushMatrix()
        
        # Standard CAD-style coordinate axes: fixed center, rotating arrows
        gl.glLoadIdentity()
        gl.glTranslatef(-0.8, -1.2, -4.0)  # Adjust Y position for taller viewport
        
        # Apply same rotations as the main model to show orientation
        gl.glRotatef(self.rotation_x, 1.0, 0.0, 0.0)
        gl.glRotatef(self.rotation_y, 0.0, 1.0, 0.0)
        
        gl.glScalef(0.8, 0.8, 0.8)  # Make appropriately sized
        
        # Disable lighting for axes
        gl.glDisable(gl.GL_LIGHTING)
        gl.glDisable(gl.GL_DEPTH_TEST)
        
        # Draw axes as lines
        gl.glBegin(gl.GL_LINES)
        
        # X-axis (Red)
        gl.glColor3f(1.0, 0.0, 0.0)
        gl.glVertex3f(0.0, 0.0, 0.0)
        gl.glVertex3f(0.25, 0.0, 0.0)
        
        # Y-axis (Green)
        gl.glColor3f(0.0, 1.0, 0.0)
        gl.glVertex3f(0.0, 0.0, 0.0)
        gl.glVertex3f(0.0, 0.25, 0.0)
        
        # Z-axis (Blue)
        gl.glColor3f(0.0, 0.0, 1.0)
        gl.glVertex3f(0.0, 0.0, 0.0)
        gl.glVertex3f(0.0, 0.0, 0.25)
        
        gl.glEnd()
        
        # Add axis labels
        gl.glColor3f(1.0, 1.0, 1.0)  # White text
        
        # X label
        gl.glRasterPos3f(0.3, 0.0, 0.0)
        self.render_text("X")
        
        # Y label  
        gl.glRasterPos3f(0.0, 0.3, 0.0)
        self.render_text("Y")
        
        # Z label
        gl.glRasterPos3f(0.0, 0.0, 0.3)
        self.render_text("Z")
        
        # Re-enable depth test
        gl.glEnable(gl.GL_DEPTH_TEST)
        
        # Restore matrix
        gl.glPopMatrix()
        
    def render_text(self, text):
        """Render text at current raster position"""
        if not OPENGL_AVAILABLE:
            return
        # Simple text rendering using OpenGL bitmap characters
        try:
            from OpenGL.GLUT import glutBitmapCharacter, GLUT_BITMAP_HELVETICA_12, glutInit
            import sys
            # Initialize GLUT if not already done
            if not hasattr(self, '_glut_initialized'):
                glutInit(sys.argv)
                self._glut_initialized = True
            
            for char in text:
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord(char))
        except ImportError:
            # Fallback: draw simple representation if GLUT not available
            pass
        except Exception:
            # Fallback for any GLUT errors in WSL/headless environments
            pass
        
    def mousePressEvent(self, event):
        """Handle mouse press for rotation and panning"""
        self.last_pos = event.pos()
        self.mouse_button = event.button()
        
    def mouseMoveEvent(self, event):
        """Handle mouse drag for rotation and panning"""
        if self.last_pos:
            dx = event.x() - self.last_pos.x()
            dy = event.y() - self.last_pos.y()
            
            if self.mouse_button == 1:  # Left click - rotate
                self.rotation_x += dy * 0.5
                self.rotation_y += dx * 0.5
            elif self.mouse_button == 2:  # Right click - pan
                self.pan_x += dx * 0.01
                self.pan_y -= dy * 0.01  # Invert Y for natural feel
            
            self.last_pos = event.pos()
            if OPENGL_LEGACY:
                self.updateGL()
            else:
                self.update()
            
    def wheelEvent(self, event):
        """Handle mouse wheel for zoom"""
        delta = event.angleDelta().y()
        print(f"Wheel event: delta={delta}, current zoom={self.zoom}")
        if delta > 0:
            self.zoom *= 0.9  # Zoom in
        else:
            self.zoom *= 1.1  # Zoom out
            
        self.zoom = max(0.1, min(10.0, self.zoom))  # Clamp zoom
        print(f"New zoom: {self.zoom}")
        if OPENGL_LEGACY:
            self.updateGL()
        else:
            self.update()
    
    def keyPressEvent(self, event):
        """Handle keyboard shortcuts"""
        key = event.text().lower()
        if key == 'w':
            self.toggle_wireframe()
        elif key == 's':
            self.toggle_solid()
        elif key == 'r':
            self.reset_view()
        elif key == 'f':
            self.fit_to_view()
        elif key == '+':
            self.zoom *= 0.8  # Zoom in
            self.zoom = max(0.1, min(10.0, self.zoom))
            if OPENGL_LEGACY:
                self.updateGL()
            else:
                self.update()
        elif key == '-':
            self.zoom *= 1.25  # Zoom out
            self.zoom = max(0.1, min(10.0, self.zoom))
            if OPENGL_LEGACY:
                self.updateGL()
            else:
                self.update()
    
    def reset_view(self):
        """Reset camera to default position"""
        self.rotation_x = 0
        self.rotation_y = 0
        self.zoom = 1.0
        self.pan_x = 0.0
        self.pan_y = 0.0
        if OPENGL_LEGACY:
            self.updateGL()
        else:
            self.update()
    
    def toggle_solid(self):
        """Toggle solid rendering mode"""
        self.wireframe_mode = False
        if OPENGL_LEGACY:
            self.updateGL()
        else:
            self.update()
    
    def fit_to_view(self):
        """Fit mesh to view - calculate optimal zoom and center"""
        if not self.vertices:
            return
            
        # Calculate bounding box
        min_x = min(v[0] for v in self.vertices)
        max_x = max(v[0] for v in self.vertices)
        min_y = min(v[1] for v in self.vertices)
        max_y = max(v[1] for v in self.vertices)
        min_z = min(v[2] for v in self.vertices)
        max_z = max(v[2] for v in self.vertices)
        
        # Calculate size and center
        size_x = max_x - min_x
        size_y = max_y - min_y
        size_z = max_z - min_z
        max_size = max(size_x, size_y, size_z)
        
        # Set zoom to fit mesh in view
        if max_size > 0:
            self.zoom = 10.0 / max_size
            self.zoom = max(0.1, min(10.0, self.zoom))
        
        # Reset rotation and pan
        self.rotation_x = -20  # Slight tilt for better view
        self.rotation_y = 45   # Angle for better perspective
        self.pan_x = 0.0
        self.pan_y = 0.0
        
        print(f"Fit to view: zoom={self.zoom}, bounds=({min_x:.1f},{max_x:.1f})x({min_y:.1f},{max_y:.1f})x({min_z:.1f},{max_z:.1f})")
        
        if OPENGL_LEGACY:
            self.updateGL()
        else:
            self.update()
        
    def toggle_wireframe(self):
        """Toggle between wireframe and solid display"""
        self.wireframe_mode = not self.wireframe_mode
        if OPENGL_LEGACY:
            self.updateGL()
        else:
            self.update()

class CSGNode:
    """Node in the CSG tree representing an operation or primitive"""
    def __init__(self, command, line, params="", node_type="primitive"):
        self.command = command
        self.line = line
        self.params = params
        self.type = node_type
        self.children = []
        self.parent = None
    
    def add_child(self, child):
        """Add a child node"""
        self.children.append(child)
        child.parent = self

class CSGParser:
    """Parse .gm3d files to build hierarchical CSG tree"""
    
    def __init__(self):
        self.stack = []
        self.root = None
        self.all_nodes = []
    
    def parse(self, content):
        """Parse .gm3d content and build hierarchical tree"""
        self.stack = []
        self.all_nodes = []
        
        lines = content.strip().split('\n')
        for line_num, line in enumerate(lines, 1):
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            
            parts = line.split()
            if not parts:
                continue
            
            command = parts[0].lower()
            
            # Create nodes for different commands
            if command == 'box':
                if len(parts) >= 7:
                    params = f"[{parts[1]},{parts[2]},{parts[3]}] × [{parts[4]},{parts[5]},{parts[6]}]"
                    node = CSGNode('BOX', line_num, params, 'primitive')
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'sheet':
                if len(parts) >= 2:
                    num_points = parts[1]
                    # Show first vertex coordinates if available
                    if len(parts) >= 5:
                        first_vertex = f"[{parts[2]},{parts[3]},{parts[4]}]..."
                        params = f"{num_points} vertices: {first_vertex}"
                    else:
                        params = f"{num_points} vertices"
                    node = CSGNode('SHEET', line_num, params, 'primitive')
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'circle':
                params = 'circular sheet'
                if len(parts) >= 4:
                    params = f"center[{parts[1]},{parts[2]},{parts[3]}]"
                node = CSGNode('CIRCLE', line_num, params, 'primitive')
                self.stack.append(node)
                self.all_nodes.append(node)
            
            elif command == 'extrude':
                if len(parts) >= 4:
                    vector = f"[{parts[1]},{parts[2]},{parts[3]}]"
                    node = CSGNode('EXTRUDE', line_num, vector, 'operation')
                    # Extrude takes one object from stack
                    if self.stack:
                        child = self.stack.pop()
                        node.add_child(child)
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'revolve':
                node = CSGNode('REVOLVE', line_num, 'around axis', 'operation')
                # Revolve takes one object from stack
                if self.stack:
                    child = self.stack.pop()
                    node.add_child(child)
                self.stack.append(node)
                self.all_nodes.append(node)
            
            elif command == 'add':
                # Boolean union - takes two objects
                node = CSGNode('ADD', line_num, '∪ Union', 'boolean')
                if len(self.stack) >= 2:
                    # Pop two items and add as children
                    child2 = self.stack.pop()
                    child1 = self.stack.pop()
                    node.add_child(child1)
                    node.add_child(child2)
                    self.stack.append(node)
                else:
                    node.params = 'ERROR: insufficient operands'
                self.all_nodes.append(node)
            
            elif command == 'subtract':
                # Boolean difference - takes two objects
                node = CSGNode('SUBTRACT', line_num, '− Difference', 'boolean')
                if len(self.stack) >= 2:
                    # Pop two items and add as children
                    child2 = self.stack.pop()  # Subtracted object
                    child1 = self.stack.pop()  # Base object
                    node.add_child(child1)
                    node.add_child(child2)
                    self.stack.append(node)
                else:
                    node.params = 'ERROR: insufficient operands'
                self.all_nodes.append(node)
        
        # The final stack should have the root object(s)
        if self.stack:
            self.root = self.stack[-1]  # Last operation is usually the root
        
        return self.all_nodes

class PrimitiveShapeDialog(QDialog):
    """Base dialog for creating primitive shapes"""
    
    def __init__(self, parent=None, title="Shape Parameters"):
        super().__init__(parent)
        self.setWindowTitle(title)
        self.setModal(True)
        self.resize(300, 200)
        
        # Main layout
        layout = QVBoxLayout(self)
        
        # Form layout for parameters
        self.form_layout = QFormLayout()
        layout.addLayout(self.form_layout)
        
        # Dialog buttons
        button_box = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        button_box.accepted.connect(self.accept)
        button_box.rejected.connect(self.reject)
        layout.addWidget(button_box)
        
    def get_gm3d_code(self):
        """Override in subclasses to return .gm3d code"""
        return ""

class CylinderDialog(PrimitiveShapeDialog):
    """Dialog for creating cylinder primitives"""
    
    def __init__(self, parent=None):
        super().__init__(parent, "Create Cylinder")
        
        # Radius parameter
        self.radius_spin = QDoubleSpinBox()
        self.radius_spin.setRange(0.01, 100.0)
        self.radius_spin.setValue(1.0)
        self.radius_spin.setDecimals(2)
        self.form_layout.addRow("Radius:", self.radius_spin)
        
        # Height parameter
        self.height_spin = QDoubleSpinBox()
        self.height_spin.setRange(0.01, 100.0)
        self.height_spin.setValue(2.0)
        self.height_spin.setDecimals(2)
        self.form_layout.addRow("Height:", self.height_spin)
        
        # Orientation (axis) selector
        self.orientation_combo = QComboBox()
        self.orientation_combo.addItems(["Z-axis (vertical)", "X-axis", "Y-axis"])
        self.form_layout.addRow("Orientation:", self.orientation_combo)
        
        # Center type selector
        self.center_type = QComboBox()
        self.center_type.addItems(["Bottom center", "Geometric center"])
        self.form_layout.addRow("Center type:", self.center_type)
        
        # Center coordinates
        self.center_x = QDoubleSpinBox()
        self.center_x.setRange(-100.0, 100.0)
        self.center_x.setValue(0.0)
        self.form_layout.addRow("Center X:", self.center_x)
        
        self.center_y = QDoubleSpinBox()
        self.center_y.setRange(-100.0, 100.0)
        self.center_y.setValue(0.0)
        self.form_layout.addRow("Center Y:", self.center_y)
        
        self.center_z = QDoubleSpinBox()
        self.center_z.setRange(-100.0, 100.0)
        self.center_z.setValue(0.0)
        self.form_layout.addRow("Center Z:", self.center_z)
    
    def get_gm3d_code(self):
        """Generate .gm3d code for cylinder"""
        cx = self.center_x.value()
        cy = self.center_y.value()
        cz = self.center_z.value()
        radius = self.radius_spin.value()
        height = self.height_spin.value()
        num_div = 32  # Number of divisions for circle polygon
        
        # Determine orientation and vectors based on selected axis
        orientation = self.orientation_combo.currentIndex()
        if orientation == 0:  # Z-axis (vertical)
            x_vec = f"{radius} 0 0"  # X-axis vector with radius length
            z_vec = "0 0 1"  # Z-axis normal
            extrude_x, extrude_y, extrude_z = 0, 0, height
            circle_normal = "Z"
        elif orientation == 1:  # X-axis
            x_vec = f"0 {radius} 0"  # Y-axis vector with radius length
            z_vec = "1 0 0"  # X-axis normal
            extrude_x, extrude_y, extrude_z = height, 0, 0
            circle_normal = "X"
        else:  # Y-axis  
            x_vec = f"{radius} 0 0"  # X-axis vector with radius length
            z_vec = "0 1 0"  # Y-axis normal
            extrude_x, extrude_y, extrude_z = 0, height, 0
            circle_normal = "Y"
        
        # Adjust center position if geometric center is selected
        if self.center_type.currentIndex() == 1:  # Geometric center
            # Offset the circle position by half the extrusion
            cx -= extrude_x / 2
            cy -= extrude_y / 2
            cz -= extrude_z / 2
        
        return f"""# Cylinder primitive (r={radius}, h={height}, axis={circle_normal})
circle {cx} {cy} {cz} {x_vec} {z_vec} {num_div}
extrude {extrude_x} {extrude_y} {extrude_z}"""

class SphereDialog(PrimitiveShapeDialog):
    """Dialog for creating sphere primitives"""
    
    def __init__(self, parent=None):
        super().__init__(parent, "Create Sphere")
        
        # Radius parameter
        self.radius_spin = QDoubleSpinBox()
        self.radius_spin.setRange(0.01, 100.0)
        self.radius_spin.setValue(1.0)
        self.radius_spin.setDecimals(2)
        self.form_layout.addRow("Radius:", self.radius_spin)
        
        # Center coordinates
        self.center_x = QDoubleSpinBox()
        self.center_x.setRange(-100.0, 100.0)
        self.center_x.setValue(0.0)
        self.form_layout.addRow("Center X:", self.center_x)
        
        self.center_y = QDoubleSpinBox()
        self.center_y.setRange(-100.0, 100.0)
        self.center_y.setValue(0.0)
        self.form_layout.addRow("Center Y:", self.center_y)
        
        self.center_z = QDoubleSpinBox()
        self.center_z.setRange(-100.0, 100.0)
        self.center_z.setValue(0.0)
        self.form_layout.addRow("Center Z:", self.center_z)
    
    def get_gm3d_code(self):
        """Generate .gm3d code for sphere using revolution"""
        cx = self.center_x.value()
        cy = self.center_y.value()
        cz = self.center_z.value()
        radius = self.radius_spin.value()
        
        return f"""# Sphere primitive (r={radius})
circle {cx} {cy} {cz} 1.0 0.0 0.0 0.0 0.0 1.0 32
revolve {cx} {cy} {cz} 0 0 1 360"""

class BoxDialog(PrimitiveShapeDialog):
    """Dialog for creating box primitives"""
    
    def __init__(self, parent=None):
        super().__init__(parent, "Create Box")
        
        # Dimensions
        self.width_spin = QDoubleSpinBox()
        self.width_spin.setRange(0.01, 100.0)
        self.width_spin.setValue(2.0)
        self.width_spin.setDecimals(2)
        self.form_layout.addRow("Width (X):", self.width_spin)
        
        self.height_spin = QDoubleSpinBox()
        self.height_spin.setRange(0.01, 100.0)
        self.height_spin.setValue(2.0)
        self.height_spin.setDecimals(2)
        self.form_layout.addRow("Depth (Y):", self.height_spin)
        
        self.depth_spin = QDoubleSpinBox()
        self.depth_spin.setRange(0.01, 100.0)
        self.depth_spin.setValue(2.0)
        self.depth_spin.setDecimals(2)
        self.form_layout.addRow("Height (Z):", self.depth_spin)
        
        # Center type selector
        self.center_type = QComboBox()
        self.center_type.addItems(["Bottom center", "Geometric center"])
        self.form_layout.addRow("Center type:", self.center_type)
        
        # Center coordinates
        self.center_x = QDoubleSpinBox()
        self.center_x.setRange(-100.0, 100.0)
        self.center_x.setValue(0.0)
        self.form_layout.addRow("Center X:", self.center_x)
        
        self.center_y = QDoubleSpinBox()
        self.center_y.setRange(-100.0, 100.0)
        self.center_y.setValue(0.0)
        self.form_layout.addRow("Center Y:", self.center_y)
        
        self.center_z = QDoubleSpinBox()
        self.center_z.setRange(-100.0, 100.0)
        self.center_z.setValue(0.0)
        self.form_layout.addRow("Center Z:", self.center_z)
    
    def get_gm3d_code(self):
        """Generate .gm3d code for box"""
        cx = self.center_x.value()
        cy = self.center_y.value()
        cz = self.center_z.value()
        w = self.width_spin.value()     # X-axis (width)
        d = self.height_spin.value()    # Y-axis (depth)  
        h = self.depth_spin.value()     # Z-axis (height)
        
        # Adjust center position based on center type
        if self.center_type.currentIndex() == 0:  # Bottom center
            # For bottom center: user coords are center of bottom face
            # Box corner should be offset by half width/depth, but at same Z level
            x1 = cx - w/2  # Left edge from center
            y1 = cy - d/2  # Back edge from center  
            z1 = cz        # Bottom at specified Z (box extends upward)
        else:  # Geometric center
            # For geometric center: user coords are center of entire box volume
            x1 = cx - w/2  # Left edge from center
            y1 = cy - d/2  # Back edge from center
            z1 = cz - h/2  # Bottom edge from center
        
        return f"""# Box primitive ({w}×{d}×{h})
box {x1} {y1} {z1} {w} {d} {h}"""

class TorusDialog(PrimitiveShapeDialog):
    """Dialog for creating torus primitives"""
    
    def __init__(self, parent=None):
        super().__init__(parent, "Create Torus")
        
        # Major radius (distance from center to tube center)
        self.major_radius_spin = QDoubleSpinBox()
        self.major_radius_spin.setRange(0.01, 100.0)
        self.major_radius_spin.setValue(2.0)
        self.major_radius_spin.setDecimals(2)
        self.form_layout.addRow("Major Radius:", self.major_radius_spin)
        
        # Minor radius (tube radius)
        self.minor_radius_spin = QDoubleSpinBox()
        self.minor_radius_spin.setRange(0.01, 100.0)
        self.minor_radius_spin.setValue(0.5)
        self.minor_radius_spin.setDecimals(2)
        self.form_layout.addRow("Minor Radius:", self.minor_radius_spin)
        
        # Orientation selector
        self.orientation_combo = QComboBox()
        self.orientation_combo.addItems(["Z-axis (horizontal)", "X-axis", "Y-axis"])
        self.form_layout.addRow("Orientation:", self.orientation_combo)
        
        # Center coordinates
        self.center_x = QDoubleSpinBox()
        self.center_x.setRange(-100.0, 100.0)
        self.center_x.setValue(0.0)
        self.form_layout.addRow("Center X:", self.center_x)
        
        self.center_y = QDoubleSpinBox()
        self.center_y.setRange(-100.0, 100.0)
        self.center_y.setValue(0.0)
        self.form_layout.addRow("Center Y:", self.center_y)
        
        self.center_z = QDoubleSpinBox()
        self.center_z.setRange(-100.0, 100.0)
        self.center_z.setValue(0.0)
        self.form_layout.addRow("Center Z:", self.center_z)
    
    def get_gm3d_code(self):
        """Generate .gm3d code for torus using revolve"""
        cx = self.center_x.value()
        cy = self.center_y.value()
        cz = self.center_z.value()
        major_r = self.major_radius_spin.value()
        minor_r = self.minor_radius_spin.value()
        
        orientation = self.orientation_combo.currentIndex()
        
        if orientation == 0:  # Z-axis (horizontal torus)
            # Small circle at distance major_r from center along X axis
            circle_cx = cx + major_r
            circle_cy = cy
            circle_cz = cz
            # Circle in YZ plane
            x_vec = "0.0 1.0 0.0"
            z_vec = "0.0 0.0 1.0"
            # Revolve around Z axis
            revolve_axis = "0 0 1"
        elif orientation == 1:  # X-axis
            # Small circle at distance major_r from center along Y axis
            circle_cx = cx
            circle_cy = cy + major_r
            circle_cz = cz
            # Circle in XZ plane
            x_vec = "0.0 0.0 1.0"
            z_vec = "1.0 0.0 0.0"
            # Revolve around X axis
            revolve_axis = "1 0 0"
        else:  # Y-axis
            # Small circle at distance major_r from center along X axis
            circle_cx = cx + major_r
            circle_cy = cy
            circle_cz = cz
            # Circle in XY plane
            x_vec = "1.0 0.0 0.0"
            z_vec = "0.0 1.0 0.0"
            # Revolve around Y axis
            revolve_axis = "0 1 0"
        
        return f"""# Torus primitive (major_r={major_r}, minor_r={minor_r})
circle {circle_cx} {circle_cy} {circle_cz} {x_vec} {z_vec} 16
revolve {cx} {cy} {cz} {revolve_axis} 360"""

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

class GM3DEditor(QMainWindow):
    def __init__(self):
        super().__init__()
        self.current_file = None
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
        
        # Center panel - text editor
        self.text_editor = QTextEdit()
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
        
        # Add widgets to splitter
        self.main_splitter.addWidget(self.tree_widget)
        self.main_splitter.addWidget(self.text_editor)
        self.main_splitter.addWidget(self.viewer_panel)
        self.main_splitter.setSizes([300, 700, 500])
        
        self.setCentralWidget(self.main_splitter)
    
    def setup_3d_panel(self):
        """Set up the 3D preview panel"""
        self.viewer_panel = QWidget()
        layout = QVBoxLayout(self.viewer_panel)
        
        # 3D viewer widget (title label removed to save space)
        if OPENGL_AVAILABLE:
            self.mesh_viewer = MeshViewer3D()
            self.mesh_viewer.setMinimumSize(400, 300)
            layout.addWidget(self.mesh_viewer)
        else:
            # Fallback if OpenGL not available
            fallback_label = QLabel("OpenGL not available\nInstall PyOpenGL for 3D preview")
            fallback_label.setAlignment(Qt.AlignCenter)
            fallback_label.setStyleSheet("color: red; font-style: italic;")
            layout.addWidget(fallback_label)
            self.mesh_viewer = None
        
        # Control buttons
        button_layout = QHBoxLayout()
        
        if self.mesh_viewer:
            # Wireframe toggle
            wireframe_btn = QAction("Toggle Wireframe", self)
            wireframe_btn.triggered.connect(self.toggle_wireframe_mode)
            
            # Reset view
            reset_btn = QAction("Reset View", self)
            reset_btn.triggered.connect(self.reset_3d_view)
        
        layout.addLayout(button_layout)
        
        # Menu bar
        self.setup_menu()
        
        # Tool bar
        self.setup_toolbar()
        
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
        run_action.setShortcut('F5')
        run_action.triggered.connect(self.generate_mesh)
        toolbar.addAction(run_action)
    
    def new_file(self):
        """Create a new file"""
        self.text_editor.clear()
        self.tree_widget.clear()
        self.current_file = None
        self.setWindowTitle("AdvCAD .gm3d Editor with CSG Tree - New File")
    
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
                self.setWindowTitle(f"AdvCAD .gm3d Editor with CSG Tree - {os.path.basename(file_path)}")
                # Update CSG tree after loading
                self.update_csg_tree()
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
        
        # Default mesh size
        mesh_size = "2.0"
        
        try:
            # Run AdvCAD command
            cmd = [advcad_path, self.current_file, output_file, mesh_size]
            
            # Show progress dialog
            progress = QProgressDialog("Generating mesh...", "Cancel", 0, 0, self)
            progress.setWindowModality(Qt.WindowModal)
            progress.show()
            
            # Run the command
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=60)
            progress.close()
            
            if result.returncode == 0:
                # Load the generated mesh in 3D viewer
                self.load_mesh_in_viewer(output_file)
                QMessageBox.information(self, "Success", f"Mesh generated successfully!\n\nOutput: {output_file}\n\nAdvCAD output:\n{result.stdout[:500]}")
            else:
                QMessageBox.critical(self, "Mesh Generation Failed", f"AdvCAD returned error code {result.returncode}\n\nError output:\n{result.stderr[:500]}")
                
        except subprocess.TimeoutExpired:
            progress.close()
            QMessageBox.critical(self, "Timeout", "Mesh generation timed out (60 seconds)")
        except Exception as e:
            progress.close()
            QMessageBox.critical(self, "Error", f"Failed to run mesh generation:\n{str(e)}")
    
    def update_csg_tree(self):
        """Update CSG tree view based on current text content"""
        content = self.text_editor.toPlainText()
        if not content.strip():
            self.tree_widget.clear()
            return
        
        # Parse the content
        self.csg_parser.parse(content)
        
        # Clear and rebuild tree
        self.tree_widget.clear()
        
        # Build the tree hierarchy
        if self.csg_parser.root:
            # Add the final result as root
            root_item = self.build_tree_item(self.csg_parser.root)
            self.tree_widget.addTopLevelItem(root_item)
        
        # Add any remaining stack items (incomplete operations)
        for node in self.csg_parser.stack:
            if node != self.csg_parser.root:
                item = self.build_tree_item(node)
                item.setText(0, f"[Stack] {item.text(0)}")
                item.setForeground(0, QColor(150, 150, 150))  # Gray for incomplete
                self.tree_widget.addTopLevelItem(item)
        
        # Expand all items
        self.tree_widget.expandAll()
        
        # Resize columns to content
        for i in range(3):
            self.tree_widget.resizeColumnToContents(i)
    
    def build_tree_item(self, node):
        """Recursively build tree items from CSG nodes"""
        # Create tree item for this node
        item = QTreeWidgetItem()
        item.setText(0, f"L{node.line}: {node.command}")
        item.setText(1, node.params)
        
        # Store line number for highlighting
        item.setData(0, Qt.UserRole, node.line)
        
        # Color code by operation type
        if node.type == 'primitive':
            item.setForeground(0, QColor(0, 100, 200))  # Blue for primitives
            item.setText(2, "Primitive")
        elif node.type == 'operation':
            item.setForeground(0, QColor(0, 150, 0))    # Green for operations
            item.setText(2, f"{len(node.children)} input(s)")
        elif node.type == 'boolean':
            item.setForeground(0, QColor(200, 0, 0))    # Red for boolean ops
            item.setText(2, f"Boolean ({len(node.children)} operands)")
        
        # Highlight errors
        if 'ERROR' in node.params:
            item.setBackground(1, QColor(255, 200, 200))  # Light red background
        
        # Recursively add children
        for child in node.children:
            child_item = self.build_tree_item(child)
            item.addChild(child_item)
        
        return item
    
    def on_cursor_position_changed(self):
        """Highlight corresponding tree item when cursor position changes in editor"""
        cursor = self.text_editor.textCursor()
        current_line = cursor.blockNumber() + 1  # QTextEdit uses 0-based line numbers
        
        # Find and highlight the corresponding tree item
        self.highlight_tree_item_by_line(current_line)
    
    def highlight_tree_item_by_line(self, line_number):
        """Find and highlight tree item corresponding to the given line number"""
        # Clear current selection
        self.tree_widget.clearSelection()
        
        # Recursively search for the item with matching line number
        def find_item_by_line(item, target_line):
            # Check current item
            item_line = item.data(0, Qt.UserRole)
            if item_line == target_line:
                return item
            
            # Check children
            for i in range(item.childCount()):
                child = item.child(i)
                result = find_item_by_line(child, target_line)
                if result:
                    return result
            
            return None
        
        # Search through all top-level items
        for i in range(self.tree_widget.topLevelItemCount()):
            top_item = self.tree_widget.topLevelItem(i)
            found_item = find_item_by_line(top_item, line_number)
            if found_item:
                # Select and highlight the item
                found_item.setSelected(True)
                self.tree_widget.setCurrentItem(found_item)
                
                # Expand parents to make it visible
                parent = found_item.parent()
                while parent:
                    parent.setExpanded(True)
                    parent = parent.parent()
                
                # Scroll to make it visible
                self.tree_widget.scrollToItem(found_item)
                break
    
    def on_tree_item_clicked(self, item, column):
        """Highlight corresponding line in editor when tree item is clicked"""
        line_num = item.data(0, Qt.UserRole)
        if line_num:
            # Move cursor to the line
            cursor = self.text_editor.textCursor()
            cursor.movePosition(QTextCursor.Start)
            
            # Move to specific line
            for _ in range(line_num - 1):
                cursor.movePosition(QTextCursor.Down)
            
            # Select the entire line
            cursor.movePosition(QTextCursor.StartOfLine)
            cursor.movePosition(QTextCursor.EndOfLine, QTextCursor.KeepAnchor)
            
            # Set the cursor and ensure it's visible
            self.text_editor.setTextCursor(cursor)
            self.text_editor.ensureCursorVisible()
            self.text_editor.setFocus()
    
    def load_mesh_in_viewer(self, pch_file_path):
        """Load mesh from .pch file into 3D viewer"""
        if not self.mesh_viewer or not OPENGL_AVAILABLE:
            print("DEBUG: No mesh viewer or OpenGL not available")
            return
        
        print(f"DEBUG: Loading mesh from {pch_file_path}")
        if os.path.exists(pch_file_path):
            print(f"DEBUG: PCH file exists, size: {os.path.getsize(pch_file_path)} bytes")
        else:
            print(f"DEBUG: PCH file does not exist: {pch_file_path}")
            return
            
        if self.pch_parser.parse_pch_file(pch_file_path):
            self.mesh_viewer.load_mesh(self.pch_parser.vertices, self.pch_parser.faces)
            print(f"DEBUG: Loaded mesh: {len(self.pch_parser.vertices)} vertices, {len(self.pch_parser.faces)} faces")
            # Force update the 3D viewer
            self.mesh_viewer.update()
            self.mesh_viewer.repaint()
        else:
            print(f"DEBUG: Failed to parse PCH file: {pch_file_path}")
    
    def toggle_wireframe_mode(self):
        """Toggle wireframe display mode"""
        if self.mesh_viewer:
            self.mesh_viewer.toggle_wireframe()
    
    def reset_3d_view(self):
        """Reset 3D camera view"""
        if self.mesh_viewer:
            self.mesh_viewer.rotation_x = 0
            self.mesh_viewer.rotation_y = 0
            self.mesh_viewer.zoom = 1.0
            self.mesh_viewer.update()
    
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