#!/usr/bin/env python3
"""Standalone test for perspective vs orthographic projection - no dependencies on gm3d_viewer.py"""

import sys
import math
from PyQt5.QtWidgets import QApplication, QMainWindow, QOpenGLWidget, QPushButton, QVBoxLayout, QWidget, QHBoxLayout, QLabel
from PyQt5.QtCore import Qt

try:
    from OpenGL import GL as gl
    from OpenGL import GLU as glu
except ImportError:
    print("PyOpenGL not installed")
    sys.exit(1)

class SimpleProjectionTest(QOpenGLWidget):
    def __init__(self):
        super().__init__()
        self.perspective_mode = False  # Start with orthographic
        self.rotation_x = 30.0
        self.rotation_y = 45.0
        self.distance = 10.0
        self.setMinimumSize(600, 600)
        
    def toggle_projection(self):
        self.perspective_mode = not self.perspective_mode
        self.update()
        return self.perspective_mode
        
    def reset_view(self):
        self.rotation_x = 30.0
        self.rotation_y = 45.0
        self.distance = 10.0
        self.update()
        
    def initializeGL(self):
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClearColor(0.1, 0.1, 0.1, 1.0)
        
    def resizeGL(self, w, h):
        gl.glViewport(0, 0, w, h)
        
    def paintGL(self):
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
        
        # Setup projection
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        
        aspect = self.width() / max(1, self.height())
        
        if self.perspective_mode:
            # PERSPECTIVE PROJECTION
            glu.gluPerspective(45.0, aspect, 0.1, 100.0)
            print(f"DEBUG: Using PERSPECTIVE projection, aspect={aspect:.2f}")
        else:
            # ORTHOGRAPHIC PROJECTION
            size = 5.0
            if aspect >= 1.0:
                gl.glOrtho(-size*aspect, size*aspect, -size, size, 0.1, 100.0)
            else:
                gl.glOrtho(-size, size, -size/aspect, size/aspect, 0.1, 100.0)
            print(f"DEBUG: Using ORTHOGRAPHIC projection, aspect={aspect:.2f}")
            
        # Setup modelview
        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()
        
        # Camera position
        glu.gluLookAt(0, 0, self.distance,  # eye
                      0, 0, 0,               # center
                      0, 1, 0)               # up
        
        # Apply rotations
        gl.glRotatef(self.rotation_x, 1, 0, 0)
        gl.glRotatef(self.rotation_y, 0, 1, 0)
        
        # Draw axes first to verify something is rendering
        self.draw_axes()
        
        # Draw a grid of cubes to show perspective effect clearly
        self.draw_cube_grid()
        
    def draw_cube_grid(self):
        """Draw a 3x3 grid of cubes at different depths to show perspective"""
        gl.glColor3f(0.6, 0.7, 0.8)
        
        for x in [-3, 0, 3]:
            for z in [-3, 0, 3]:
                gl.glPushMatrix()
                gl.glTranslatef(x, 0, z)
                self.draw_cube(0.8)
                gl.glPopMatrix()
                
    def draw_cube(self, size):
        """Draw a simple wireframe cube"""
        s = size / 2.0
        
        # Draw as wireframe to see perspective clearly
        gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_LINE)
        gl.glLineWidth(1.5)
        
        # Front face
        gl.glBegin(gl.GL_QUADS)
        gl.glVertex3f(-s, -s, s)
        gl.glVertex3f(s, -s, s)
        gl.glVertex3f(s, s, s)
        gl.glVertex3f(-s, s, s)
        gl.glEnd()
        
        # Back face
        gl.glBegin(gl.GL_QUADS)
        gl.glVertex3f(-s, -s, -s)
        gl.glVertex3f(-s, s, -s)
        gl.glVertex3f(s, s, -s)
        gl.glVertex3f(s, -s, -s)
        gl.glEnd()
        
        # Connect front and back
        gl.glBegin(gl.GL_LINES)
        gl.glVertex3f(-s, -s, s); gl.glVertex3f(-s, -s, -s)
        gl.glVertex3f(s, -s, s); gl.glVertex3f(s, -s, -s)
        gl.glVertex3f(s, s, s); gl.glVertex3f(s, s, -s)
        gl.glVertex3f(-s, s, s); gl.glVertex3f(-s, s, -s)
        gl.glEnd()
        
    def draw_axes(self):
        """Draw coordinate axes"""
        gl.glLineWidth(2.0)
        gl.glBegin(gl.GL_LINES)
        # X axis - red
        gl.glColor3f(1, 0, 0)
        gl.glVertex3f(0, 0, 0)
        gl.glVertex3f(5, 0, 0)
        # Y axis - green
        gl.glColor3f(0, 1, 0)
        gl.glVertex3f(0, 0, 0)
        gl.glVertex3f(0, 5, 0)
        # Z axis - blue
        gl.glColor3f(0, 0, 1)
        gl.glVertex3f(0, 0, 0)
        gl.glVertex3f(0, 0, 5)
        gl.glEnd()
        gl.glLineWidth(1.0)
        
    def mousePressEvent(self, event):
        self.last_pos = event.pos()
        
    def mouseMoveEvent(self, event):
        if not hasattr(self, 'last_pos'):
            return
            
        dx = event.x() - self.last_pos.x()
        dy = event.y() - self.last_pos.y()
        
        if event.buttons() & Qt.LeftButton:
            self.rotation_x += dy * 0.5
            self.rotation_y += dx * 0.5
            self.update()
            
        self.last_pos = event.pos()
        
    def wheelEvent(self, event):
        delta = event.angleDelta().y() / 120.0
        self.distance *= (1.0 - delta * 0.1)
        self.distance = max(2.0, min(50.0, self.distance))
        self.update()

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Standalone Projection Test - ORTHOGRAPHIC")
        self.setGeometry(100, 100, 800, 650)
        
        central = QWidget()
        layout = QVBoxLayout()
        
        # OpenGL widget
        self.gl_widget = SimpleProjectionTest()
        layout.addWidget(self.gl_widget)
        
        # Info label
        self.info_label = QLabel(
            "Mouse: Drag to rotate | Wheel: Zoom\n"
            "PERSPECTIVE: Far cubes should appear smaller\n"
            "ORTHOGRAPHIC: All cubes should appear same size"
        )
        self.info_label.setStyleSheet("QLabel { background-color: #222; color: #fff; padding: 10px; }")
        layout.addWidget(self.info_label)
        
        # Control buttons
        btn_layout = QHBoxLayout()
        
        self.btn_toggle = QPushButton("Toggle Projection (P)")
        self.btn_toggle.clicked.connect(self.toggle_projection)
        btn_layout.addWidget(self.btn_toggle)
        
        btn_reset = QPushButton("Reset View (R)")
        btn_reset.clicked.connect(self.gl_widget.reset_view)
        btn_layout.addWidget(btn_reset)
        
        layout.addLayout(btn_layout)
        
        central.setLayout(layout)
        self.setCentralWidget(central)
        
    def toggle_projection(self):
        is_perspective = self.gl_widget.toggle_projection()
        mode = "PERSPECTIVE" if is_perspective else "ORTHOGRAPHIC"
        self.setWindowTitle(f"Standalone Projection Test - {mode}")
        
        if is_perspective:
            self.info_label.setText(
                "Mouse: Drag to rotate | Wheel: Zoom\n"
                "PERSPECTIVE MODE: Far cubes should appear SMALLER (converging lines)\n"
                "If all cubes look the same size, perspective is NOT working!"
            )
        else:
            self.info_label.setText(
                "Mouse: Drag to rotate | Wheel: Zoom\n"
                "ORTHOGRAPHIC MODE: All cubes should appear SAME SIZE (parallel lines)\n"
                "This is the expected behavior for orthographic projection."
            )
        
    def keyPressEvent(self, event):
        if event.key() == Qt.Key_P:
            self.toggle_projection()
        elif event.key() == Qt.Key_R:
            self.gl_widget.reset_view()
        elif event.key() == Qt.Key_Escape:
            self.close()

def main():
    app = QApplication(sys.argv)
    window = TestWindow()
    window.show()
    
    print("\n" + "="*60)
    print("STANDALONE PROJECTION TEST")
    print("="*60)
    print("This tests OpenGL projection WITHOUT using gm3d_viewer.py")
    print("\nExpected behavior:")
    print("- ORTHOGRAPHIC: All 9 cubes appear same size (parallel projection)")
    print("- PERSPECTIVE: Far cubes appear smaller (converging projection)")
    print("\nControls:")
    print("- P: Toggle projection mode")
    print("- R: Reset view")
    print("- Mouse drag: Rotate")
    print("- Mouse wheel: Zoom")
    print("="*60 + "\n")
    
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()