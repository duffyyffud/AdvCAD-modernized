# CLAUDE.md - AdvCAD Codebase Guide

## Project Overview
AdvCAD is a C++ CAD library for 3D solid modeling and mesh generation with Constrained Delaunay Triangulation. The primary focus is on **Face 7 mixed triangle debugging** - a critical issue where triangulation produces mixed triangles containing both real points (ID ≥ 0) and dummy points (ID < 0).

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

## Face 7 Mixed Triangle Problem
**The Core Issue**: Face 7 generates mixed triangles containing both real and dummy points, causing crashes.

**Root Cause**: Algorithmic failure in Constrained Delaunay Triangulation, specifically in constraint recovery.

**Key Files:**
- `WH/mg3d_delaunay2d.cc/h` - Face mesh generation (crash location)
- `WH/constdel2d.cc/h` - Constraint recovery implementation
- `WH/robust_cdt.cc/h` - Upgraded robust CDT implementation

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

## Key Statistics (Established)
- 4,580 WH_ASSERT statements across 70 files
- Face 7 crashes with mixed triangles
- CMake build system functional
- Robust CDT implementation added but needs testing

## Next Steps for Face 7
1. Complete robust CDT integration testing
2. Analyze Face 7 specific geometry causing failures  
3. Test fallback strategies with real crash case
4. Verify mixed triangle elimination

## Important Context Files
- `CODEBASE_KNOWLEDGE.md` - Detailed debugging knowledge and lessons
- `README` - Build instructions and usage
- Sample crash: `sample/shaft/coil_01_mm.gm3d` with Face 7 issue

## Philosophy
- Focus on **root cause analysis** over superficial modernization
- **Debuggability** over performance for meshing algorithms
- **Systematic approach** with proper validation
- **Preserve working functionality** while fixing specific issues