# AdvCAD Codebase Knowledge - READ BEFORE EVERY ACTION

## MANDATORY WORKING DIRECTORY (ALWAYS BE HERE FIRST)
**ALWAYS cd to PROJECT ROOT before any action:**
```bash
cd /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/
```

## Directory Structure (MEMORIZE THIS)
```
/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/  <- PROJECT ROOT (ALWAYS WORK FROM HERE)
├── WH/           <- WH LIBRARY (125 files)
├── command/      <- ADVCAD EXECUTABLE 
├── build/        <- CMAKE BUILD OUTPUT
├── sample/       <- TEST MODELS
└── README        <- DOCUMENTATION
```

## Relative Paths (USE THESE FROM PROJECT ROOT)
- WH library files: `WH/mg3d_delaunay2d.cc`
- Commands: `grep -n "WH_ASSERT" WH/mg3d_delaunay2d.cc`
- Build: `cd build && make -j4`
- Test: `./build/command/advcad sample/block.gm3d output.pch 2.0`

## Key Statistics (ALREADY ESTABLISHED)
- **4,580 WH_ASSERT statements** across **70 files** (BASELINE)
- **Face 7 generates mixed triangles** - THE REAL PROBLEM
- CMake build system in place and working

## Critical Files for Face 7 Debugging
```
WH/mg3d_delaunay2d.cc/h     <- Face meshing (WHERE FACE 7 CRASHED)
WH/constdel2d.cc/h          <- Constraint recovery (KEY FOR MIXED TRIANGLES)
WH/delaunay2d.cc/h          <- Basic Delaunay
```

## Working Commands (USE THESE EXACTLY)
```bash
# From project root
./build/command/advcad sample/shaft/coil_01_mm.gm3d output.pch 2.0  # Face 7 test
./build/command/advcad sample/block.gm3d output.pch 2.0             # Basic test

# Build
cd build && make -j4
```

## Test Models (FOR VERIFICATION)
- `sample/block.gm3d` - SIMPLE, must always work
- `sample/shaft/coil_01_mm.gm3d` - COMPLEX, Face 7 crashes here

## CURRENT FOCUS: Face 7 Mixed Triangle Problem
**REAL ISSUE**: Mixed triangles (real + dummy points) should never be generated
**ROOT CAUSE**: Algorithmic bug in constrained Delaunay triangulation
**NOT SOLVED BY**: Exception handling (that was a distraction)

## Face 7 Debug Strategy
1. **Understand WHY mixed triangles form**
   - What are dummy points for?
   - When should they be removed?
   - What constraint recovery failed?

2. **Minimal targeted debugging**
   ```cpp
   if (face->id() == 7 && has_mixed_triangle(triangle)) {
       dump_triangulation_state();
       visualize_constraints();
   }
   ```

3. **Algorithm analysis before code changes**

## LESSONS LEARNED
- **WH_ASSERT → Exception was wrong approach**
- WH_ASSERT indicates unrecoverable states that shouldn't occur
- Focus on WHY the state occurs, not HOW to handle it
- Exception handling masks real bugs

## CRITICAL EDIT TOOL LESSONS (AVOID REPEATED MISTAKES)
**Multi-line string matching errors:**
- ✗ WRONG: Trying to match strings across multiple lines with exact spacing
- ✗ WRONG: Including "..." in string matching (it's not literal text)
- ✓ CORRECT: Use `Read` tool first to see exact formatting
- ✓ CORRECT: Match single lines or use `grep -n` to find exact text
- ✓ CORRECT: Check whitespace with `cat -n` to see spaces/tabs

**Example of what fails:**
```
// This FAILS because spacing might be different
old_string: "  _triangulator->perform ();
  _triangulator->reorderTriangle ();
  
  // Systematic assertion: Verify triangulator output integrity..."
```

**Correct approach:**
```bash
grep -n "perform" WH/file.cc  # Find exact line
Read file around that line    # See exact formatting
Match single line only        # Avoid multi-line spacing issues
```

## STOP WASTING TIME ON:
- Re-measuring known statistics
- WH_ASSERT modernization 
- Exception handling for algorithmic bugs
- Superficial "modernization"
- Multi-line string matching without checking exact formatting

## FOCUS ON:
- Understanding constrained Delaunay algorithm
- Face 7 specific geometry analysis
- Dummy point generation/removal logic
- Root cause of mixed triangle formation
- Always check exact formatting before Edit operations