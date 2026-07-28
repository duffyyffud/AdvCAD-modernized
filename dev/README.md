# AdvCAD Developer Tools

Development, debugging, and testing utilities for AdvCAD development workflow.

## Testing Tools

### 🧪 `test_regression.py`
**Comprehensive Test Suite**
- Tests all sample geometries for mesh generation
- Reports success rates and failure analysis
- Tracks performance metrics across versions

```bash
python3 dev/test_regression.py
```

### 🔍 `mesh_failure_analyzer.py`
**Systematic Mesh Failure Analysis**
- Tests models at various mesh sizes to find failure boundaries
- Generates controlled test cases for debugging
- Identifies patterns in mesh generation failures

```bash
python3 dev/mesh_failure_analyzer.py
```

## Debugging Tools

### 🎯 `validate_geometry.py`
**Geometry Validation and Analysis**
- Validates .gm3d file syntax and semantics
- Checks for geometric consistency
- Reports potential meshing issues

```bash
python3 dev/validate_geometry.py input.gm3d
```

### 👁️ `visualize_face7.py`
**Face-Level Debug Visualization**
- Specialized debugging for specific face meshing issues
- Visual analysis of triangulation problems
- Debug output parsing and analysis

```bash
python3 dev/visualize_face7.py
```

### 🐛 `face5_debug_generator.py`
**Face 5 Debug Case Generator**
- Generates test cases for Face 5 mixed triangle issues
- Creates controlled failure scenarios
- Systematic reproduction of specific bugs

```bash
python3 dev/face5_debug_generator.py
```

### 📦 `reduced_tests/`
**Manually-Reduced Face 5 Reproducers** (no generator script — hand-minimized, identified 2026-07-27)
- `cyclic_minimal.gm3d`, `cyclic_reduced_1.gm3d`, `cyclic_reduced_2.gm3d`: manual bisections of `sample/shaft/cyclic_mag_body_01.gm3d`, each with 6-vertex sheets matching the exact Face 5 mixed-triangle bug condition (Breakthrough #5, commit `7c1e226`)
- Kept as regression reproducers for that specific bug, not generated/consumed by any script

## Performance Analysis

### 🏗️ `geometric_test_generator.py`
**Automated Test Case Generation**
- Generates diverse geometric test cases
- Parametric geometry creation
- Test coverage analysis

```bash
python3 dev/geometric_test_generator.py
```

## Environment Diagnostics

### 🖥️ `test_opengl.py` / `test_opengl_legacy.py` / `test_opengl_software.py`
**PyQt5/OpenGL Display Capability Checks**
- Draw a rotating triangle via PyQt5 to verify OpenGL rendering works in the current environment
- Needed because WSL2 cannot always display OpenGL as freely as native Windows can (Akio, 2026-07-26); relevant to debugging `apps/gm3d_editor.py`'s rendering
- `test_opengl.py`: standard `QOpenGLWidget` check; `test_opengl_legacy.py`: legacy `QGLWidget` variant; `test_opengl_software.py`: software-rendering fallback for environments where hardware GL is unavailable

```bash
python3 dev/test_opengl.py
```

## Development Workflow

1. **Test Changes**: Run `test_regression.py` after modifications
2. **Debug Failures**: Use `mesh_failure_analyzer.py` for systematic analysis
3. **Validate Geometry**: Check files with `validate_geometry.py`
4. **Visualize Issues**: Use `visualize_face7.py` for specific debugging

These tools are designed for developers working on AdvCAD core functionality, debugging mesh generation issues, and ensuring code quality.