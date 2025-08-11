"""
3D Viewer for AdvCAD GM3D Editor
Handles OpenGL-based 3D mesh visualization
"""

import os
import sys

# Try to import OpenGL components
try:
    # Try legacy OpenGL (more compatible)
    from PyQt5.QtOpenGL import QGLWidget as OpenGLBaseWidget
    import OpenGL.GL as gl
    import OpenGL.GLU as glu
    OPENGL_AVAILABLE = True
    OPENGL_LEGACY = True
    print("Using legacy QGLWidget for OpenGL")
except ImportError:
    try:
        # Try modern OpenGL as fallback
        from PyQt5.QtOpenGL import QOpenGLWidget as OpenGLBaseWidget
        import OpenGL.GL as gl
        import OpenGL.GLU as glu
        OPENGL_AVAILABLE = True
        OPENGL_LEGACY = False
        print("Using modern QOpenGLWidget")
    except ImportError:
        from PyQt5.QtWidgets import QWidget
        OPENGL_AVAILABLE = False
        OPENGL_LEGACY = False
        OpenGLBaseWidget = QWidget
        print("OpenGL not available - 3D preview disabled")

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