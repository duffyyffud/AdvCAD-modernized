"""
Dialog classes for AdvCAD GM3D Editor
Handles primitive shape creation dialogs
"""

from PyQt5.QtWidgets import (QDialog, QDialogButtonBox, QFormLayout, 
                             QDoubleSpinBox, QComboBox, QVBoxLayout)

class PrimitiveShapeDialog(QDialog):
    """Base dialog for creating primitive shapes"""
    
    def __init__(self, parent=None, title="Create Primitive"):
        super().__init__(parent)
        self.setWindowTitle(title)
        self.setModal(True)
        
        # Main layout
        layout = QVBoxLayout()
        
        # Form layout for parameters
        self.form_layout = QFormLayout()
        layout.addLayout(self.form_layout)
        
        # Dialog buttons
        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.accepted.connect(self.accept)
        buttons.rejected.connect(self.reject)
        layout.addWidget(buttons)
        
        self.setLayout(layout)
    
    def get_gm3d_code(self):
        """Override in subclasses to generate .gm3d code"""
        raise NotImplementedError()

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
        
        # Orientation selector
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
        """Generate .gm3d code for cylinder using extrude"""
        cx = self.center_x.value()
        cy = self.center_y.value()
        cz = self.center_z.value()
        radius = self.radius_spin.value()
        height = self.height_spin.value()
        num_div = 32  # Number of divisions for circle
        
        orientation = self.orientation_combo.currentIndex()
        center_type = self.center_type.currentIndex()
        
        if orientation == 0:  # Z-axis (vertical)
            # Circle in XY plane
            x_vec = f"{radius} 0.0 0.0"
            z_vec = "0.0 0.0 1.0"
            # Adjust center for bottom vs geometric center
            if center_type == 0:  # Bottom center
                # Circle at bottom, extrude upward
                extrude_x, extrude_y, extrude_z = 0, 0, height
            else:  # Geometric center
                # Circle at bottom, but center is in middle
                cz -= height / 2
                extrude_x, extrude_y, extrude_z = 0, 0, height
            circle_normal = "Z"
        elif orientation == 1:  # X-axis
            # Circle in YZ plane
            x_vec = f"0.0 {radius} 0.0"
            z_vec = "1.0 0.0 0.0"
            # Adjust center for bottom vs geometric center
            if center_type == 0:  # Bottom center
                extrude_x, extrude_y, extrude_z = height, 0, 0
            else:  # Geometric center
                cx -= height / 2
                extrude_x, extrude_y, extrude_z = height, 0, 0
            circle_normal = "X"
        else:  # Y-axis
            # Circle in XZ plane
            x_vec = f"{radius} 0.0 0.0"
            z_vec = "0.0 1.0 0.0"
            # Adjust center for bottom vs geometric center
            if center_type == 0:  # Bottom center
                extrude_x, extrude_y, extrude_z = 0, height, 0
            else:  # Geometric center
                cy -= height / 2
                extrude_x, extrude_y, extrude_z = 0, height, 0
            circle_normal = "Y"
        
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
        
        center_type = self.center_type.currentIndex()
        
        if center_type == 0:  # Bottom center
            # Box bottom left corner relative to bottom center
            x1 = cx - w/2  # Left edge from center
            y1 = cy - d/2  # Back edge from center
            z1 = cz        # Bottom at center Z
        else:  # Geometric center
            # Box bottom left corner relative to geometric center
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