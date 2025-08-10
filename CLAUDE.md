# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview
AdvCAD is a C++ CAD library for 3D solid modeling and mesh generation with Constrained Delaunay Triangulation. **Current Status: v0.14.2 with Water-Tight Mesh Fix Applied** - produces perfect 72-triangle water-tight meshes by correctly placing robust logic in the constraint recovery pipeline stage.

## Critical Working Directory Rule
**ALWAYS work from project root**: `/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/`

Directory structure:
- `WH/` - Core library (125 files, 4,580 WH_ASSERT statements)
- `command/` - advcad executable
- `build/` - CMake build output
- `sample/` - Test geometries

## Essential Commands

### Build System
**CMake (Recommended):**
```bash
cmake --build /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build -j4
```

### Core Testing
```bash
# Basic water-tight mesh test (should produce 72 triangles)
/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/sample/block.gm3d /tmp/test.pch 2.0

# v0.14.1 command syntax (includes debug levels)
/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad --debug=1 geometry.gm3d output.pch 2.0
```

### GUI Tools
```bash
# Complete GUI with tree pane and 3D visualization
python3 /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/apps/gm3d_editor.py

# Automated mesh optimization
python3 /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/apps/advcad_auto.py
```

## Critical Algorithmic Fix (v0.14.2) - **WATER-TIGHT MESH SOLUTION**

### Pipeline Location Fix - **ROOT CAUSE DISCOVERED**
**Issue**: v0.12.1+ produced 29 triangles (non-water-tight) vs v0.12.0's 72 triangles (water-tight) for block.gm3d
**Root Cause**: Robust CDT logic was applied at the WRONG pipeline stage - during face triangulation instead of constraint recovery
**Solution**: Moved robust logic from `mg3d_delaunay2d.cc` (face triangulation) to `constdel2d.cc` (constraint recovery)
**Impact**: Restores perfect water-tight mesh generation with 72 triangles matching v0.12.0 behavior

### The Fix Details
1. **WH/mg3d_delaunay2d.cc**: Always use standard `WH_CDLN2D_Triangulator()` 
2. **WH/constdel2d.cc**: Enhanced `recoverConstraintSegment()` with robust logic for complex constraints (>5 intersecting triangles)
3. **Result**: Maintains triangle density while providing robust constraint enforcement where needed

This fix demonstrates that algorithmic modernization must respect the PIPELINE ARCHITECTURE - robust techniques belong in constraint recovery, not initial triangulation.

## Core Architecture

### Triangulation Pipeline (Critical Understanding)
The mesh generation follows a strict 3-stage pipeline:
```
Edge Discretization → Face Triangulation → Constraint Recovery
```

1. **Edge Discretization**: Divides edges into segments based on mesh size (e.g., 32 segments for block.gm3d)
2. **Face Triangulation**: Creates initial triangulation in 2D parameter space (should produce consistent triangle density)
3. **Constraint Recovery**: Enforces boundary segments exist in final mesh (where robust techniques belong)

**Key Insight**: Modifying face triangulation stage affects triangle density and water-tight properties. Robust techniques should be applied during constraint recovery to maintain mesh density while ensuring geometric constraints.

## Architecture Overview
1. **Geometry Input**: `.gm3d` files define 3D models
2. **Face Processing**: Each face undergoes 2D triangulation in parameter space
3. **Triangulation Pipeline**:
   ```
   WH_MG3D_FaceMeshGenerator → WH_CDLN2D_Triangulator → constraint recovery
   ```
4. **Output**: `.pch` files with triangular mesh

## Critical Development Rules

### Observer Self Principle (from GOOD_PRACTICE.md)
Maintain **80% technical focus / 20% process observation**:
- Always use full paths, never `cd` commands
- Check git status before major changes  
- Accept user corrections immediately without questioning
- Think before every tool call - is it necessary?

### Edit Tool Requirements
**Multi-line string matching fails** - always:
1. Use `Read` tool first to see exact formatting
2. Match single lines only  
3. Avoid redirect syntax errors that cause runtime argument problems

### Shell Command Safety
**CRITICAL**: Avoid `2>&1` redirects in commands - they cause argument parsing errors
```bash
# WRONG - causes "Usage:" error
advcad file.gm3d output.pch 2.0 2>&1

# CORRECT - run command directly  
advcad file.gm3d output.pch 2.0
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

## Current Status (v0.14.2)
- **Water-tight mesh generation**: ✅ FIXED - produces 72 triangles for block.gm3d  
- **Complete GUI tools**: Tree pane, text editor, 3D visualization with OpenGL
- **Pipeline fix applied**: Robust logic correctly placed in constraint recovery
- **All v0.14.1 functionality preserved**: GUI, Python tools, debug system

## Version History Highlights
- **v0.12.0**: Original water-tight behavior (72 triangles)
- **v0.12.1-v0.12.6**: Debugging era with robust CDT experiments (29 triangles - broken)
- **v0.14.1**: Modern GUI and tools but inherited mesh issue  
- **v0.14.2**: ✅ CURRENT - Combines v0.14.1 features with water-tight mesh fix

## Systematic Debugging Methodology - **PROVEN EFFECTIVE**
1. **Use mesh failure analyzer** to find controlled failure cases
2. **Analyze debug output patterns** to identify algorithmic vs lifecycle issues  
3. **Target easier debugging cases** rather than complete failures
4. **Maintain success rate** while incrementally improving robustness

## Key Tools and Files

### Production Tools
- `advcad_auto.py` - Python interface with automatic mesh sizing and PCH validation
- `test_regression.py` - Comprehensive test suite (19 models, 89.5% success rate)
- `mesh_failure_analyzer.py` - Systematic boundary testing for controlled failure discovery

### Critical Source Files
- `WH/mg3d_delaunay2d.cc:1046-1054` - Face triangulation (now always uses standard triangulator)
- `WH/constdel2d.cc:471-528` - Constraint recovery with robust logic for complex cases
- `command/advcad.cc` - Main executable with v0.14.1 debug flag support
- `apps/gm3d_editor.py` - Complete GUI with tree pane and 3D visualization

### Key Tools
- `apps/advcad_auto.py` - Automated mesh generation with size optimization
- `test_regression.py` - 19-model test suite for validation
- `GOOD_PRACTICE.md` - Observer Self principle and development discipline

## Philosophy
- Focus on **root cause analysis** over superficial modernization
- **Debuggability** over performance for meshing algorithms
- **Systematic approach** with proper validation
- **Preserve working functionality** while fixing specific issues