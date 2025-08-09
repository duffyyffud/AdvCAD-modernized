#!/usr/bin/env python3
"""
OpenGL test with software rendering fallback for WSL2
"""

import sys
import os

# Force software rendering for WSL2
os.environ['LIBGL_ALWAYS_SOFTWARE'] = '1'
os.environ['GALLIUM_DRIVER'] = 'llvmpipe'

from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QLabel
from PyQt5.QtCore import QTimer

print("Testing OpenGL with software rendering...")
print(f"LIBGL_ALWAYS_SOFTWARE = {os.environ.get('LIBGL_ALWAYS_SOFTWARE')}")

try:
    from PyQt5.QtOpenGL import QGLWidget
    print("✓ QGLWidget import successful")
except ImportError as e:
    print(f"✗ QGLWidget import failed: {e}")
    sys.exit(1)

try:
    from OpenGL.GL import *
    from OpenGL.GLU import *
    print("✓ PyOpenGL import successful")
except ImportError as e:
    print(f"✗ PyOpenGL import failed: {e}")
    sys.exit(1)

import math

class SimpleSoftwareGLWidget(QGLWidget):
    """OpenGL widget with software rendering for WSL2"""
    
    def __init__(self):
        super().__init__()
        self.rotation = 0.0
        self.setMinimumSize(400, 400)
        
        # Timer for animation
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_rotation)
        self.timer.start(50)  # 20 FPS
        
    def update_rotation(self):
        self.rotation += 2.0
        self.updateGL()  # Legacy method for QGLWidget
        
    def initializeGL(self):
        """Initialize OpenGL with basic settings"""
        print("initializeGL called")
        try:
            # Basic initialization
            glClearColor(0.0, 0.0, 0.2, 1.0)  # Dark blue background
            glEnable(GL_DEPTH_TEST)
            glShadeModel(GL_SMOOTH)
            
            # Check renderer info
            renderer = glGetString(GL_RENDERER)
            version = glGetString(GL_VERSION)
            print(f"OpenGL Renderer: {renderer}")
            print(f"OpenGL Version: {version}")
            
            if b'llvmpipe' in renderer.lower():
                print("✓ Using software rendering (llvmpipe)")
            
            print("✓ OpenGL initialization successful")
        except Exception as e:
            print(f"✗ OpenGL initialization failed: {e}")
            
    def paintGL(self):
        """Render with immediate mode (compatible with software rendering)"""
        try:
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
            glLoadIdentity()
            
            # Simple 2D triangle for better compatibility
            glMatrixMode(GL_PROJECTION)
            glLoadIdentity()
            glOrtho(-2, 2, -2, 2, -1, 1)  # Simple orthographic projection
            glMatrixMode(GL_MODELVIEW)
            glLoadIdentity()
            
            # Rotate
            glRotatef(self.rotation, 0.0, 0.0, 1.0)
            
            # Draw triangle with immediate mode (most compatible)
            glBegin(GL_TRIANGLES)
            glColor3f(1.0, 0.0, 0.0)  # Red
            glVertex2f(0.0, 1.0)
            glColor3f(0.0, 1.0, 0.0)  # Green  
            glVertex2f(-1.0, -1.0)
            glColor3f(0.0, 0.0, 1.0)  # Blue
            glVertex2f(1.0, -1.0)
            glEnd()
            
            # Also draw a simple line box to ensure something renders
            glColor3f(1.0, 1.0, 1.0)  # White
            glBegin(GL_LINE_LOOP)
            glVertex2f(-1.5, -1.5)
            glVertex2f(1.5, -1.5)
            glVertex2f(1.5, 1.5)
            glVertex2f(-1.5, 1.5)
            glEnd()
            
            glFlush()  # Force render
            
        except Exception as e:
            print(f"✗ OpenGL rendering error: {e}")
            
    def resizeGL(self, width, height):
        """Handle resize with simple viewport"""
        try:
            glViewport(0, 0, width, height)
        except Exception as e:
            print(f"✗ OpenGL resize error: {e}")

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("PyQt5 Software OpenGL Test - WSL2")
        self.setGeometry(100, 100, 600, 600)
        
        # Central widget
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout(central_widget)
        
        # Info label
        info_label = QLabel("Software Rendering Test: Should see rotating triangle + white box")
        layout.addWidget(info_label)
        
        # OpenGL widget
        try:
            self.gl_widget = SimpleSoftwareGLWidget()
            layout.addWidget(self.gl_widget)
            print("✓ OpenGL widget created successfully")
        except Exception as e:
            error_label = QLabel(f"OpenGL widget creation failed: {e}")
            layout.addWidget(error_label)
            print(f"✗ OpenGL widget creation failed: {e}")

def main():
    print("Starting PyQt5 Software OpenGL test for WSL2...")
    print("=" * 50)
    
    # Check if running in WSL
    if os.path.exists('/proc/version'):
        with open('/proc/version', 'r') as f:
            if 'microsoft' in f.read().lower():
                print("✓ Running in WSL2 environment")
    
    app = QApplication(sys.argv)
    
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