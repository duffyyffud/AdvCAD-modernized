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
python3 apps/optimize_mesh_size.py input.gm3d output.pch
```

## Installation

Python dependencies (`numpy`, `pyqt5`) are pinned in the workspace-root `Pipfile`/`Pipfile.lock` (`python_version = "3.11"`) and managed via `pipenv`:

```bash
# Install dependencies from Pipfile (run from workspace root)
pipenv install

# Run a tool inside the pipenv environment
pipenv run python3 apps/gm3d_editor.py

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
python3 apps/optimize_mesh_size.py sample/shaft/coil_01.gm3d sample/shaft/coil_01.pch
```

These tools are designed for end-users who want to work with AdvCAD geometry and mesh generation without diving into development details.