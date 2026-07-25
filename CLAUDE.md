# CLAUDE.md - AdvCAD Codebase Guide

## Project Overview
AdvCAD is a C++ CAD library for 3D solid modeling and mesh generation with Constrained Delaunay Triangulation. **Current Status: 100% Success Rate (19/19 models)**, verified 2026-07-21 by rebuilding and running the full model suite in `dev/test_regression.py` against `sample/` (see "Build System" below — a one-line `command/CMakeLists.txt` path bug was blocking a clean build until fixed the same day).

## Critical Working Directory Rule
**ALWAYS work from project root**: `/home/miyoshi/workspace/wsCpp/AdvCAD-modernized/`
(This project was previously at `AdvCAD-0.12b/`; that path no longer exists. Some older scripts under `dev/` still hardcode the old path — patch it locally when running them, don't rely on their built-in default.)

Directory structure (flat since commit `95f12b4`, which removed a duplicate `src/` tree):
- `WH/` - Core library (196 files, ~4,568 `WH_ASSERT` statements, counted 2026-07-21)
- `command/` - advcad executable
- `build/` - CMake build output
- `sample/` - Test geometries
- `apps/` - End-user tools (Qt `.gm3d` editor, automated mesh generation) — see below
- `dev/` - Developer tools (regression tests, failure analysis, geometry validation) — see below

## Build System
**CMake only** — the legacy Makefiles (root, `WH/`, `command/`) were deleted in commit `469a4a9`; do not look for or expect a `Makefile`-based build path.

```bash
cd build && cmake .. && make -j4
```

Note: as of commit `50ec6f8`, `optimize_mesh_size.py` lived in `scripts/`, but `command/CMakeLists.txt` still pointed at the old root-level path, so a clean `cmake ..` failed with `CMake Error: File .../optimize_mesh_size.py does not exist`. Fixed 2026-07-21 by pointing the `configure_file()` call at `scripts/optimize_mesh_size.py`. That file turned out to be a byte-identical duplicate of `apps/optimize_mesh_size.py`; fixed 2026-07-25 by deleting the `scripts/` copy and re-pointing `configure_file()` at `${CMAKE_SOURCE_DIR}/apps/optimize_mesh_size.py`, the single remaining copy. If `build/` still has stale cache from before either fix, delete `build/CMakeCache.txt` and `build/CMakeFiles/` and reconfigure.

**Test commands:**
```bash
./build/command/advcad sample/block.gm3d output.pch 2.0          # Basic test
./build/command/advcad sample/shaft/cyclic_mag_body_01.gm3d output.pch 2.0  # NOW WORKS!
```

## Systematic Debugging Breakthroughs (v0.12.1 - v0.12.6)

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

### 5. Face 5 Selection Criteria Fix (v0.12.6) - **89.5% SUCCESS**
**Issue**: Face 5 with 6 nodes/segments didn't trigger robust CDT (threshold was > 6)
**Solution**: Changed criteria from `> 6` to `>= 6`, expanded precision from 1e-3 to 1e-2
**Impact**: cyclic_mag_body_01.gm3d NOW PASSES! Success rate 89.5% (17/19 models)

### 6. Malformed Input Geometry Fix - **100% SUCCESS** (commit `efca872`)
**Issue**: The 2 remaining failures (`air_practice.gm3d`, `air_up2_top_01.gm3d`) were not algorithmic bugs — the `.gm3d` input files had duplicate vertices / zero-length edges, invalid geometry rather than a triangulator defect.
**Solution**: Fixed the geometry files directly; added `dev/validate_geometry.py` to detect duplicate vertices, zero-length edges, and degenerate polygons going forward.
**Impact**: 19/19 models pass. Re-verified 2026-07-21 (see Project Overview).

## Recent Modernization Work
1. **Robust CDT Implementation**: Added `WH_RobustCDT_Triangulator` with:
   - Exact arithmetic predicates (`robust_predicates.cc/h`)
   - Fallback strategies (ear clipping, monotone partition, fan triangulation)
   - Detailed debugging and statistics
   - Mixed triangle detection and prevention

2. **Integration**: `mg3d_delaunay2d.cc` selects robust CDT for complex faces. Current form (verified against `WH/mg3d_delaunay2d.cc:1046-1088`, 2026-07-21 — logic unchanged since v0.12.6, only refactored into named booleans):
```cpp
bool isComplexGeometry = (_boundarySegment_s.size() >= 6 || _node_s.size() >= 6);
// ...per-node check for abs(pos.x) < 1e-2 || abs(pos.y) < 1e-2 sets hasSmallScaleGeometry
if (isComplexGeometry || hasSmallScaleGeometry) {
    useRobustCDT = true;
    faceId = 7; // Assume Face 7 for debugging
}
_triangulator = useRobustCDT ? createRobustTriangulator(faceId) : new WH_CDLN2D_Triangulator();
```

3. **Multi-Level Debug System** (commits `82df3fa`, `4fc3114`): `WH/debug_levels.cc/h` implements 4 output levels — `WH_DEBUG_SILENT=0` (default; warnings/errors/progress only, suited to the web app), `NORMAL=1`, `VERBOSE=2`, `TRACE=3`. Controlled via `advcad`'s `--debug=N` command-line argument.

4. **Python Tooling Split** (commit `50ec6f8`): Python scripts are now organized by audience, not left flat at the workspace root:
   - `apps/` (end-user): `gm3d_editor.py` (Qt GUI for `.gm3d` editing with syntax highlighting and one-click mesh generation via `advcad`), `advcad_auto.py` (automated mesh sizing + PCH validation), `optimize_mesh_size.py`
   - `dev/` (developer): `test_regression.py` (the 19-model suite referenced above), `mesh_failure_analyzer.py`, `validate_geometry.py`, `visualize_face7.py`, stress/precision test generators

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

## Current Success Metrics
- **100% Success Rate**: 19/19 models pass mesh generation (commit `efca872`; re-verified 2026-07-21 by rebuilding and running `dev/test_regression.py`'s full model list against the current `sample/` tree)
- **Systematic Fixes**: Domain ID, zero-vector, precision detection, 6-node threshold, malformed-input geometry cleanup
- **Robust Coverage**: All faces >= 6 nodes now use enhanced triangulation
- **No known outstanding mesh-generation failures** as of 2026-07-21. If a model starts failing again, treat it as a regression, not an expected gap — the previous "2 advancing front failures" (`air_practice.gm3d`, `air_up2_top_01.gm3d`) were traced to invalid input geometry (duplicate vertices / zero-length edges), not an algorithmic bug, and were fixed in commit `efca872` (see Breakthrough #6 above).

## Systematic Debugging Methodology - **PROVEN EFFECTIVE**
1. **Use mesh failure analyzer** to find controlled failure cases
2. **Analyze debug output patterns** to identify algorithmic vs lifecycle issues  
3. **Target easier debugging cases** rather than complete failures
4. **Maintain success rate** while incrementally improving robustness

## Key Tools and Files

### Production Tools (`apps/` — end-user, see Recent Modernization Work #4)
- `apps/advcad_auto.py` - Python interface with automatic mesh sizing and PCH validation
- `apps/gm3d_editor.py` - Qt GUI for `.gm3d` editing, integrates directly with `advcad`
- `apps/optimize_mesh_size.py` - mesh size tuning helper (also copied into `build/command/` by CMake)

### Developer Tools (`dev/`)
- `dev/test_regression.py` - full 19-model regression suite. **Note**: hardcodes `project_root = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")` and does `os.chdir()` to it — this predates the rename to `AdvCAD-modernized`. Patch that path (e.g. in a scratch copy) before running it; do not edit the tracked file casually since other historical assumptions in it haven't been audited.
- `dev/mesh_failure_analyzer.py` - systematic boundary testing for controlled failure discovery
- `dev/validate_geometry.py` - detects duplicate vertices / zero-length edges / degenerate polygons in `.gm3d` input (added for the fix in Breakthrough #6)

### Critical Source Files
- `WH/mg3d_delaunay2d.cc` (robust CDT selection logic around line 1046-1088) - Face mesh generation
- `WH/robust_cdt.cc` - Enhanced triangulator with domain ID fix and fallback strategies
- `WH/space2d_inline.cc:109` - Zero-length vector division protection (verified still at this line, 2026-07-21)
- `WH/constdel2d.cc` - Original triangulator (still used by some faces)
- `WH/debug_levels.cc/h` - Multi-level debug output system

### Debug References
- `docs/DEBUG_META_KNOWLEDGE.md` - Transferable debugging lessons mined from resolved breakthroughs, plus a phased debugging procedure (supersedes the old `docs/CODEBASE_KNOWLEDGE.md`, deleted 2026-07-25 as a duplicate of this and `GOOD_PRACTICE.md`)
- `GOOD_PRACTICE.md` - Enforced workflow discipline and git management
- Version tags: `v0.12.1` (breakthrough), `v0.12.2` (Python), `v0.12.3` (domain ID), `v0.12.4` (precision), `v0.12.5` (lifecycle), `v0.12.6` (Face 5 fix)

## Philosophy
- Focus on **root cause analysis** over superficial modernization
- **Debuggability** over performance for meshing algorithms
- **Systematic approach** with proper validation
- **Preserve working functionality** while fixing specific issues