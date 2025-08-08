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

## Performance Analysis

### ⚡ `test_large_mesh_sizes.py`
**Large-Scale Mesh Testing**
- Performance testing with large mesh sizes
- Memory usage analysis
- Scalability testing

```bash
python3 dev/test_large_mesh_sizes.py
```

### 📊 `test_precision_boundary.py`
**Precision Boundary Analysis**
- Tests geometric precision limits
- Identifies numerical stability issues
- Boundary condition analysis

```bash
python3 dev/test_precision_boundary.py
```

### 🔬 `extensive_stress_test.py`
**Comprehensive Stress Testing**
- Long-running stability tests
- Edge case generation and testing
- System resource monitoring

```bash
python3 dev/extensive_stress_test.py
```

### 🏗️ `geometric_test_generator.py`
**Automated Test Case Generation**
- Generates diverse geometric test cases
- Parametric geometry creation
- Test coverage analysis

```bash
python3 dev/geometric_test_generator.py
```

## Development Workflow

1. **Test Changes**: Run `test_regression.py` after modifications
2. **Debug Failures**: Use `mesh_failure_analyzer.py` for systematic analysis
3. **Validate Geometry**: Check files with `validate_geometry.py`
4. **Visualize Issues**: Use `visualize_face7.py` for specific debugging
5. **Stress Test**: Run `extensive_stress_test.py` before releases

These tools are designed for developers working on AdvCAD core functionality, debugging mesh generation issues, and ensuring code quality.