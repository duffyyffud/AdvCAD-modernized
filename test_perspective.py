#!/usr/bin/env python3
"""Test perspective projection toggle"""

import sys
import os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)) + '/apps')

from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QPushButton, QHBoxLayout
from PyQt5.QtCore import Qt
from gm3d_viewer import MeshViewer3D

class TestWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Perspective Test")
        self.setGeometry(100, 100, 800, 600)
        
        central = QWidget()
        layout = QVBoxLayout()
        
        # 3D viewer
        self.viewer = MeshViewer3D()
        
        # Load simple test mesh - a cube
        vertices = [
            (-1, -1, -1), (1, -1, -1), (1, 1, -1), (-1, 1, -1),
            (-1, -1, 1), (1, -1, 1), (1, 1, 1), (-1, 1, 1)
        ]
        faces = [
            (0, 1, 2), (0, 2, 3),  # front
            (4, 6, 5), (4, 7, 6),  # back
            (0, 4, 5), (0, 5, 1),  # bottom
            (2, 6, 7), (2, 7, 3),  # top
            (0, 3, 7), (0, 7, 4),  # left
            (1, 5, 6), (1, 6, 2)   # right
        ]
        self.viewer.load_mesh(vertices, faces)
        
        layout.addWidget(self.viewer)
        
        # Control buttons
        btn_layout = QHBoxLayout()
        
        btn_perspective = QPushButton("Toggle Projection (P)")
        btn_perspective.clicked.connect(self.toggle_projection)
        btn_layout.addWidget(btn_perspective)
        
        btn_reset = QPushButton("Reset View (R)")
        btn_reset.clicked.connect(self.viewer.reset_view)
        btn_layout.addWidget(btn_reset)
        
        layout.addLayout(btn_layout)
        
        central.setLayout(layout)
        self.setCentralWidget(central)
        
        # Show initial mode
        self.update_title()
    
    def toggle_projection(self):
        self.viewer.toggle_projection()
        self.update_title()
    
    def update_title(self):
        mode = "Perspective" if self.viewer.perspective_mode else "Orthographic"
        self.setWindowTitle(f"Perspective Test - {mode} Mode")
    
    def keyPressEvent(self, event):
        if event.key() == Qt.Key_P:
            self.toggle_projection()
        elif event.key() == Qt.Key_R:
            self.viewer.reset_view()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = TestWindow()
    window.show()
    sys.exit(app.exec_())