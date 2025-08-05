# CLAUDE.md - AdvCAD Codebase Guide

## Project Overview
AdvCAD is a C++ CAD library for 3D solid modeling and mesh generation with Constrained Delaunay Triangulation. **Current Status: 84.2% Success Rate (16/19 models)** with systematic debugging breakthroughs achieved through controlled failure analysis and triangulator lifecycle management fixes.

## Critical Working Directory Rule
**ALWAYS work from project root**: `/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/`

Directory structure:
- `WH/` - Core library (125 files, 4,580 WH_ASSERT statements)
- `command/` - advcad executable
- `build/` - CMake build output
- `sample/` - Test geometries

## Build System
Both traditional Makefile and CMake supported:

**CMake (Recommended):**
```bash
cd build && make -j4
```

**Test commands:**
```bash
./build/command/advcad sample/block.gm3d output.pch 2.0          # Basic test
./build/command/advcad sample/shaft/coil_01_mm.gm3d output.pch 2.0  # Face 7 crash
```

## Systematic Debugging Breakthroughs (v0.12.1 - v0.12.4)

### 1. Domain ID Filtering Fix (v0.12.3) - **MAJOR BREAKTHROUGH**
**Issue**: Robust CDT triangles had domainId=-1, getting filtered out by `domainId==0` check
**Solution**: Added domain ID assignment (value 1) to unassigned triangles in robust CDT
**Impact**: Fixed coarse mesh failures, maintains 84.2% success rate

### 2. Zero-Length Vector Division (v0.12.1) 
**Issue**: `WH_ne (ratio, 0.0)` assertion failing in `space2d_inline.cc:109`
**Solution**: Added zero-check with graceful fallback to zero vector
**Impact**: Achieved 84.2% success rate (16/19 models)

### 3. Expanded Robust CDT Criteria (v0.12.4)
**Issue**: Small-scale geometries not triggering robust predicates
**Solution**: Detect coordinates < 1e-3 and enable robust CDT automatically
**Impact**: More faces use robust processing, improved stability

### 4. Mesh Failure Analyzer - **SYSTEMATIC TOOL**
**Tool**: `mesh_failure_analyzer.py` tests working models at various mesh sizes
**Strategy**: Find controlled failure cases instead of debugging complete failures
**Success**: Led to discovery of domain ID filtering bug

## Recent Modernization Work
1. **Robust CDT Implementation**: Added `WH_RobustCDT_Triangulator` with:
   - Exact arithmetic predicates (`robust_predicates.cc/h`)
   - Fallback strategies (ear clipping, monotone partition, fan triangulation)
   - Detailed debugging and statistics
   - Mixed triangle detection and prevention

2. **Integration**: Modified `mg3d_delaunay2d.cc` to use robust CDT for complex faces:
```cpp
// Use robust CDT for Face 7 and other problematic faces
if (_boundarySegment_s.size() > 10 || _node_s.size() > 50) {
    _triangulator = createRobustTriangulator(7);
}
```

## Architecture Overview
1. **Geometry Input**: `.gm3d` files define 3D models
2. **Face Processing**: Each face undergoes 2D triangulation in parameter space
3. **Triangulation Pipeline**:
   ```
   WH_MG3D_FaceMeshGenerator → WH_CDLN2D_Triangulator → constraint recovery
   ```
4. **Output**: `.pch` files with triangular mesh

## Critical Edit Tool Lessons
**Multi-line string matching fails** - always:
1. Use `Read` tool first to see exact formatting
2. Match single lines only
3. Check whitespace with `grep -n`

Example error pattern:
```cpp
// FAILS - multi-line with spacing assumptions
old_string: "_triangulator->perform ();\n  _triangulator->reorderTriangle ();"

// CORRECT - single line matching
old_string: "_triangulator->perform ();"
```

## Debug Strategy for Face 7
1. **Enable debug output**: Set `faceId = 7` in robust CDT
2. **Analyze constraint recovery**: Check which constraints fail
3. **Mixed triangle detection**: Identify triangles with both real/dummy points
4. **Fallback triggers**: Monitor which strategies are used

## Common Mistakes to Avoid
1. **Path errors**: Use `WH/` not `/WH/` for relative paths
2. **WH_ASSERT approach**: Don't replace with exceptions - fix root cause
3. **Multi-line Edit operations**: Always check exact formatting first
4. **Missing working directory**: Always `cd` to project root first

## Current Success Metrics (v0.12.4)
- **84.2% Success Rate**: 16/19 models pass mesh generation
- **Systematic Fixes**: Domain ID, zero-vector, precision detection implemented
- **Robust Coverage**: Most faces now use enhanced triangulation algorithms
- **Remaining Failures**: 2 advancing front issues + 1 triangulator lifecycle bug

## Remaining Debug Targets
1. **Advancing Front Failures** (2 models): `air_practice.gm3d`, `air_up2_top_01.gm3d`
   - Issue: Assertion failure in `afront2d.cc:884` with degenerate geometry
   - Cause: Severe geometric degeneracies (duplicate vertices, zero-length edges)
   
2. **Triangulator Lifecycle Bug** (1 model): `cyclic_mag_body_01.gm3d`
   - Issue: Face 5 bypasses robust CDT selection, uses stale triangulator
   - Evidence: Missing "DEBUG: Face characteristics" messages for face 5
   - Hypothesis: State management issue preventing fresh triangulator creation

## Systematic Debugging Methodology - **PROVEN EFFECTIVE**
1. **Use mesh failure analyzer** to find controlled failure cases
2. **Analyze debug output patterns** to identify algorithmic vs lifecycle issues  
3. **Target easier debugging cases** rather than complete failures
4. **Maintain success rate** while incrementally improving robustness

## Key Tools and Files

### Production Tools
- `advcad_auto.py` - Python interface with automatic mesh sizing and PCH validation
- `test_regression.py` - Comprehensive test suite (19 models, 84.2% success rate)
- `mesh_failure_analyzer.py` - Systematic boundary testing for controlled failure discovery

### Critical Source Files
- `WH/mg3d_delaunay2d.cc` - Face mesh generation with robust CDT selection logic
- `WH/robust_cdt.cc` - Enhanced triangulator with domain ID fix and fallback strategies
- `WH/space2d_inline.cc:109` - Zero-length vector division protection
- `WH/constdel2d.cc` - Original triangulator (still used by some faces)

### Debug References
- `CODEBASE_KNOWLEDGE.md` - Detailed debugging knowledge and lessons
- `GOOD_PRACTICE.md` - Enforced workflow discipline and git management
- Version tags: `v0.12.1` (breakthrough), `v0.12.2` (Python), `v0.12.3` (domain ID), `v0.12.4` (precision)

## Philosophy
- Focus on **root cause analysis** over superficial modernization
- **Debuggability** over performance for meshing algorithms
- **Systematic approach** with proper validation
- **Preserve working functionality** while fixing specific issues