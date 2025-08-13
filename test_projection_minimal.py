#!/usr/bin/env python3
"""Minimal test: Two circles at different depths to verify projection switching"""

import sys
import math
from PyQt5.QtWidgets import QApplication, QMainWindow, QOpenGLWidget, QPushButton, QHBoxLayout, QWidget, QVBoxLayout
from PyQt5.QtCore import Qt

try:
    from OpenGL import GL as gl
    from OpenGL import GLU as glu
except ImportError:
    print("PyOpenGL not installed")
    sys.exit(1)

class MinimalProjectionTest(QOpenGLWidget):
    def __init__(self):
        super().__init__()
        self.perspective_mode = False  # Start with orthographic
        self.setMinimumSize(400, 400)
        
    def toggle_projection(self):
        self.perspective_mode = not self.perspective_mode
        self.update()
        return self.perspective_mode
        
    def initializeGL(self):
        gl.glEnable(gl.GL_DEPTH_TEST)
        gl.glClearColor(0.2, 0.2, 0.2, 1.0)
        
    def resizeGL(self, w, h):
        gl.glViewport(0, 0, w, h)
        
    def paintGL(self):
        gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
        gl.glEnable(gl.GL_DEPTH_TEST)
        
        # Setup projection matrix
        gl.glMatrixMode(gl.GL_PROJECTION)
        gl.glLoadIdentity()
        
        if self.perspective_mode:
            glu.gluPerspective(45.0, 1.0, 1.0, 10.0)
        else:
            gl.glOrtho(-2, 2, -2, 2, -5, 5)
        
        # Setup modelview matrix
        gl.glMatrixMode(gl.GL_MODELVIEW)
        gl.glLoadIdentity()
        
        if self.perspective_mode:
            gl.glTranslatef(0, 0, -5)  # Move camera back for perspective
        
        # Overlapping triangles at different depths
        gl.glLineWidth(4.0)
        gl.glPolygonMode(gl.GL_FRONT_AND_BACK, gl.GL_LINE)
        
        # NEAR triangle (red) at z=1
        gl.glColor3f(1.0, 0.0, 0.0)
        gl.glBegin(gl.GL_TRIANGLES)
        gl.glVertex3f(-1.0, -0.5, 1.0)
        gl.glVertex3f(1.0, -0.5, 1.0)
        gl.glVertex3f(0.0, 0.5, 1.0)
        gl.glEnd()
        
        # FAR triangle (blue) at z=-1
        gl.glColor3f(0.0, 0.0, 1.0)
        gl.glBegin(gl.GL_TRIANGLES)
        gl.glVertex3f(-1.0, -0.5, -1.0)
        gl.glVertex3f(1.0, -0.5, -1.0)
        gl.glVertex3f(0.0, 0.5, -1.0)
        gl.glEnd()
        
        mode = "PERSPECTIVE" if self.perspective_mode else "ORTHOGRAPHIC"
        print(f"{mode}: Red=NEAR, Blue=FAR")
        
    def draw_simple_circle(self, radius):
        """Draw a simple filled circle"""
        segments = 16
        gl.glBegin(gl.GL_TRIANGLE_FAN)
        gl.glVertex2f(0, 0)  # Center
        for i in range(segments + 1):
            angle = 2.0 * math.pi * i / segments
            x = radius * math.cos(angle)
            y = radius * math.sin(angle)
            gl.glVertex2f(x, y)
        gl.glEnd()
        
    def draw_circle_at_z(self, z, radius):
        """Draw a circle at specified Z depth"""
        segments = 32
        gl.glLineWidth(3.0)
        gl.glBegin(gl.GL_LINE_LOOP)
        for i in range(segments):
            angle = 2.0 * math.pi * i / segments
            x = radius * math.cos(angle)
            y = radius * math.sin(angle)
            gl.glVertex3f(x, y, z)
        gl.glEnd()
        
        # Also draw as filled circle for visibility
        gl.glBegin(gl.GL_TRIANGLE_FAN)
        gl.glVertex3f(0, 0, z)  # Center
        for i in range(segments + 1):
            angle = 2.0 * math.pi * i / segments
            x = radius * math.cos(angle)
            y = radius * math.sin(angle)
            gl.glVertex3f(x, y, z)
        gl.glEnd()

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Minimal Projection Test - ORTHOGRAPHIC")
        self.setGeometry(100, 100, 500, 550)
        
        central = QWidget()
        layout = QVBoxLayout()
        
        # OpenGL widget
        self.gl_widget = MinimalProjectionTest()
        layout.addWidget(self.gl_widget)
        
        # Control button
        self.btn_toggle = QPushButton("Toggle Projection (Press P)")
        self.btn_toggle.clicked.connect(self.toggle_projection)
        layout.addWidget(self.btn_toggle)
        
        central.setLayout(layout)
        self.setCentralWidget(central)
        
        print("\n" + "="*60)
        print("MINIMAL PROJECTION TEST")
        print("="*60)
        print("RED circle = NEAR (z=2)")
        print("BLUE circle = FAR (z=-2)")
        print("\nEXPECTED BEHAVIOR:")
        print("- ORTHOGRAPHIC: Both circles SAME size")
        print("- PERSPECTIVE: Blue circle SMALLER than red")
        print("="*60 + "\n")
        
    def toggle_projection(self):
        is_perspective = self.gl_widget.toggle_projection()
        mode = "PERSPECTIVE" if is_perspective else "ORTHOGRAPHIC"
        self.setWindowTitle(f"Minimal Projection Test - {mode}")
        
        if is_perspective:
            self.btn_toggle.setText("Toggle to ORTHOGRAPHIC (Press P)")
            print("\n>>> PERSPECTIVE MODE: Blue circle should be SMALLER")
        else:
            self.btn_toggle.setText("Toggle to PERSPECTIVE (Press P)")
            print("\n>>> ORTHOGRAPHIC MODE: Both circles should be SAME SIZE")
        
    def keyPressEvent(self, event):
        if event.key() == Qt.Key_P:
            self.toggle_projection()
        elif event.key() == Qt.Key_Escape:
            self.close()

def main():
    app = QApplication(sys.argv)
    window = TestWindow()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()