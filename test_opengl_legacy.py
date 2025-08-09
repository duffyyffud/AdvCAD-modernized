#!/usr/bin/env python3
"""
Simple OpenGL test with PyQt5 using legacy QGLWidget - draws a rotating triangle
"""

import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QLabel
from PyQt5.QtCore import QTimer

# Test OpenGL imports
print("Testing legacy OpenGL imports...")
try:
    from PyQt5.QtOpenGL import QGLWidget
    print("✓ QGLWidget import successful")
except ImportError as e:
    print(f"✗ QGLWidget import failed: {e}")
    sys.exit(1)

try:
    from OpenGL.GL import *
    import OpenGL.GL as gl
    print("✓ PyOpenGL import successful")
except ImportError as e:
    print(f"✗ PyOpenGL import failed: {e}")
    print("Run: pip install PyOpenGL PyOpenGL_accelerate")
    sys.exit(1)

import math

class SimpleLegacyOpenGLWidget(QGLWidget):
    """Minimal OpenGL widget using legacy QGLWidget that draws a rotating triangle"""
    
    def __init__(self):
        super().__init__()
        self.rotation = 0.0
        
        # Timer for animation
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_rotation)
        self.timer.start(50)  # 20 FPS
        
    def update_rotation(self):
        self.rotation += 2.0
        self.updateGL()
        
    def initializeGL(self):
        """Initialize OpenGL"""
        print("initializeGL called")
        try:
            gl.glClearColor(0.0, 0.0, 0.2, 1.0)  # Dark blue background
            gl.glEnable(gl.GL_DEPTH_TEST)
            print("✓ OpenGL initialization successful")
        except Exception as e:
            print(f"✗ OpenGL initialization failed: {e}")
            
    def paintGL(self):
        """Render the scene"""
        try:
            gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
            gl.glLoadIdentity()
            
            # Move back and rotate
            gl.glTranslatef(0.0, 0.0, -3.0)
            gl.glRotatef(self.rotation, 0.0, 0.0, 1.0)
            
            # Draw a simple triangle
            gl.glBegin(gl.GL_TRIANGLES)
            gl.glColor3f(1.0, 0.0, 0.0)  # Red
            gl.glVertex3f(0.0, 1.0, 0.0)
            gl.glColor3f(0.0, 1.0, 0.0)  # Green  
            gl.glVertex3f(-1.0, -1.0, 0.0)
            gl.glColor3f(0.0, 0.0, 1.0)  # Blue
            gl.glVertex3f(1.0, -1.0, 0.0)
            gl.glEnd()
            
        except Exception as e:
            print(f"✗ OpenGL rendering error: {e}")
            
    def resizeGL(self, width, height):
        """Handle resize"""
        try:
            gl.glViewport(0, 0, width, height)
            gl.glMatrixMode(gl.GL_PROJECTION)
            gl.glLoadIdentity()
            
            # Set perspective
            aspect = width / height if height != 0 else 1
            gl.gluPerspective(45, aspect, 0.1, 100.0)
            gl.glMatrixMode(gl.GL_MODELVIEW)
            
        except Exception as e:
            print(f"✗ OpenGL resize error: {e}")

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("PyQt5 Legacy OpenGL Test - Rotating Triangle")
        self.setGeometry(100, 100, 800, 600)
        
        # Central widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        
        # Info label
        info_label = QLabel("Legacy OpenGL Test: You should see a rotating triangle")
        layout.addWidget(info_label)
        
        # OpenGL widget
        try:
            self.gl_widget = SimpleLegacyOpenGLWidget()
            layout.addWidget(self.gl_widget)
            print("✓ Legacy OpenGL widget created successfully")
        except Exception as e:
            error_label = QLabel(f"OpenGL widget creation failed: {e}")
            layout.addWidget(error_label)
            print(f"✗ OpenGL widget creation failed: {e}")

def main():
    print("Starting PyQt5 Legacy OpenGL test...")
    app = QApplication(sys.argv)
    
    # Check OpenGL support
    try:
        window = TestWindow()
        window.show()
        print("✓ Window shown, starting event loop...")
        return app.exec_()
    except Exception as e:
        print(f"✗ Application failed: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())