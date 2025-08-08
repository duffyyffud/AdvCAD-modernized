# AdvCAD User Applications

End-user tools for working with AdvCAD geometry and mesh generation.

## Applications

### 🎨 `gm3d_editor.py`
**GUI Editor for .gm3d Geometry Files**
- Visual text editor with syntax highlighting
- File operations (New, Open, Save)
- Real-time .gm3d command validation

```bash
python3 apps/gm3d_editor.py
```

### ⚙️ `advcad_auto.py`
**Automated Mesh Generation**
- Intelligent mesh sizing with validation
- Batch processing capabilities
- Automatic .pch output management

```bash
python3 apps/advcad_auto.py input.gm3d output.pch [mesh_size]
```

### 🔧 `optimize_mesh_size.py`
**Mesh Size Optimization Tool**
- Finds optimal mesh parameters for geometry
- Quality analysis and recommendations
- Performance vs accuracy balance

```bash
python3 apps/optimize_mesh_size.py input.gm3d
```

## Installation

```bash
# Install PyQt5 for GUI editor
pip install PyQt5

# Make applications executable
chmod +x apps/*.py

# Add to PATH (optional)
export PATH="$PATH:$(pwd)/apps"
```

## Usage Examples

```bash
# Edit geometry file
python3 apps/gm3d_editor.py

# Generate mesh automatically
python3 apps/advcad_auto.py sample/block.gm3d tmp/output.pch 2.0

# Optimize mesh parameters
python3 apps/optimize_mesh_size.py sample/shaft/coil_01.gm3d
```

These tools are designed for end-users who want to work with AdvCAD geometry and mesh generation without diving into development details.