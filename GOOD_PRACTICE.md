# GOOD_PRACTICE.md - AdvCAD Development Practices  

This document outlines essential practices for AdvCAD development to ensure quality, maintainability, and successful debugging.

## 👁️ Fundamental Principle: The Observer Self (Fūshi Kaden)

**Inspired by Zeami's "The Transmission of the Flower of Acting Style"**

A Noh performer maintains dual consciousness: 80% focused on performing, 20% observing themselves from behind. Apply this principle to development:

### The 80/20 Cognitive Split
- **80% Technical Focus**: Problem-solving, coding, implementation details
- **20% Process Observer**: Monitors git state, workflow discipline, pattern adherence

### What the Observer Self Watches
- [ ] **Git State Awareness**: Is working directory clean? Are changes atomic?
- [ ] **Commit Discipline**: Does current work represent one logical change?
- [ ] **Rule Adherence**: Am I following established patterns and practices?
- [ ] **Context Awareness**: Have I lost track of the bigger picture?

### Observer Self Triggers
- Before making any file edit
- After completing any logical unit of work
- When feeling "in the flow" but losing process awareness
- Every 3 actions as backup reminder
- **BEFORE EVERY TOOL CALL**: Stop and think - is this necessary?

**Key Insight**: Never give 100% cognitive resources to technical work. Always reserve 20% for process observation to prevent context packing and workflow violations.

### Critical Thinking Before Tool Use
- [ ] **STOP and THINK** before every tool call
- [ ] Ask: "What exactly am I trying to accomplish?"
- [ ] Ask: "Is this tool call necessary or am I just being reactive?"
- [ ] **User instructions override everything** - if user says something, accept it immediately
- [ ] Don't try to "discover" what the user already told you

### When User Gives Corrections
- [ ] **Accept the correction immediately** - don't question or verify
- [ ] **Don't repeat the same mistake** - learn from feedback
- [ ] **Don't try to justify or explain the mistake** - just fix it
- [ ] **Adapt behavior immediately** - change approach right away

## 🏗️ AdvCAD Project Foundation

### MANDATORY: Working Directory Rule
- [ ] **ALWAYS** work from project root: `/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/`
- [ ] Use relative paths: `WH/mg3d_delaunay2d.cc`, not `/WH/mg3d_delaunay2d.cc`
- [ ] Verify working directory with `pwd` before any action
- [ ] **USE ALWAYS FULL PATH COMMAND**: Never use `cd` - use full paths in commands

### Before Starting Any Task
- [ ] Read CLAUDE.md to understand current state
- [ ] Read CODEBASE_KNOWLEDGE.md for essential patterns
- [ ] Verify build system works: `cmake --build build -j4`
- [ ] Test basic functionality: `./build/command/advcad sample/block.gm3d tmp/test.pch 2.0`

### During Development
- [ ] **Build First**: Always ensure code compiles before making changes
- [ ] **Test on Simple Cases**: Use `sample/block.gm3d` for initial testing
- [ ] **Use Read Tool Before Edit**: Avoid "File has not been read yet" errors
- [ ] **Check Exact Formatting**: Use `grep -n` to find exact text patterns
- [ ] **Single-Line Matching**: Avoid multi-line string matching in Edit tool

### Quality Gates
- [ ] Code compiles without errors: `cmake --build build -j4`
- [ ] Basic functionality works: sample/block.gm3d processes successfully
- [ ] No regressions in existing test cases
- [ ] Debug output is helpful and not excessive

### File Organization (.pch Management)
- [ ] **Use tmp/ for testing**: `./build/command/advcad input.gm3d tmp/output.pch 2.0`
- [ ] **Never create .pch in root**: All `/*.pch` files are gitignored
- [ ] **Proper locations**:
  - `tmp/` - Development and quick testing (gitignored)
  - `tests/output/` - Test script outputs (gitignored)
  - `sample/` - Reference examples (tracked)
  - `tests/data/` - Validation data (tracked)
- [ ] **Clean up regularly**: `rm tmp/*.pch` after testing
- [ ] **Descriptive names**: Use `tmp/test_[geometry]_[meshsize].pch`

### Simple Task Execution Rules
- [ ] For "test all X files": Just do it systematically, don't overthink
- [ ] For "use appropriate mesh size": Use geometry analyzer to get size, then test
- [ ] For "prioritize bugs": Test first, analyze results, then prioritize
- [ ] **Don't reinvent existing solutions** - check for existing scripts first

### Anti-Patterns to Avoid (CRITICAL)
- [ ] **Tool spam**: Calling tools reflexively without purpose
- [ ] **Re-inventing existing solutions**: Look for existing test scripts first
- [ ] **Ignoring user corrections**: When user corrects you, accept it immediately
- [ ] **Overcomplicating simple requests**: "Test files" means just test them
- [ ] **Discovering what user already told you**: If user says "shaft has problems", don't verify
- [ ] **NEVER TRUNCATE BUILD LOGS**: Always show complete compiler output - truncated logs hide critical errors
- [ ] **Never give up too early**: If something worked before, it can work again - debug systematically

### Git Practice
- [ ] **Atomic Commits**: Each commit should represent one logical change
- [ ] **Descriptive Messages**: Explain WHY, not just WHAT
- [ ] **Test Before Commit**: Never commit broken tests
- [ ] **Clean Working Directory**: `git status` should be clean after commit

---

## 🔧 C++ Development Best Practices

### Code Quality Rules
- [ ] **Follow Existing Patterns**: Match codebase style and naming conventions
- [ ] **WH_ASSERT Philosophy**: Don't replace with exceptions - fix root cause
- [ ] **Understand Before Changing**: Algorithm bugs need algorithmic fixes, not error handling
- [ ] **Preserve Functionality**: Existing working code should remain working
- [ ] **Add Debug Output**: Use cerr for debugging, but make it conditional

### File Edit Best Practices (CRITICAL)
- [ ] **Read Before Edit**: Always use Read tool before Edit tool
- [ ] **Single Line Matching**: Match one line at a time, not multi-line strings
- [ ] **Check Formatting**: Use `grep -n` to find exact text with line numbers
- [ ] **Verify Whitespace**: Use `cat -n` to see exact spacing and tabs
- [ ] **Test Edits**: Compile after each edit to catch errors immediately

### Common Edit Tool Mistakes to Avoid
```
✗ WRONG: Multi-line string matching
old_string: "line1\n  line2\n  line3"

✓ CORRECT: Single line matching  
old_string: "line1"
new_string: "new_line1"
```

### Debugging Strategy
- [ ] **Root Cause Analysis**: Don't just fix symptoms, understand WHY issues occur
- [ ] **Minimal Changes**: Make smallest possible change to fix the issue
- [ ] **Test Edge Cases**: Use various mesh sizes and geometries
- [ ] **Document Findings**: Update CODEBASE_KNOWLEDGE.md with lessons learned

---

## 🧪 Testing and Validation

### Test Model Strategy
- [ ] **Start Simple**: Always test with `sample/block.gm3d` first
- [ ] **Progress to Complex**: Use shaft models for advanced testing
- [ ] **Multiple Mesh Sizes**: Test with various mesh sizes to find failure points
- [ ] **Known Working Cases**: Keep a set of models that should always work
- [ ] **Document Failures**: Record which models fail and at what mesh sizes

### Systematic Testing Approach
- [ ] **Use optimize_mesh_size.py**: Let the script find appropriate mesh sizes
- [ ] **Test Boundary Cases**: Very small and very large mesh sizes
- [ ] **Validate Geometry**: Check input geometry for degenerate cases
- [ ] **Monitor Debug Output**: Use debug output to understand what's happening
- [ ] **Performance Tracking**: Monitor triangle counts and processing time

### Available Test Models
```bash
# Simple models (should always work)
./build/command/advcad sample/block.gm3d tmp/test.pch 2.0

# Complex models (for advanced testing)  
./build/command/advcad sample/shaft/coil_01.gm3d tmp/coil_test.pch 0.006
./build/command/advcad sample/shaft/coil_02.gm3d tmp/coil2_test.pch 0.01
```

---

## 🔍 Debugging Triangulation Issues

### When Triangulation Fails
- [ ] **Check Mesh Size**: Is it appropriate for the geometry scale?
- [ ] **Examine Assertion**: What specific assertion is failing?
- [ ] **Look at Debug Output**: What was happening before the failure?
- [ ] **Test with Larger Mesh Size**: Does a larger mesh size work?
- [ ] **Check Input Geometry**: Are there degenerate features?

### Common Failure Patterns
1. **Degenerate Triangles**: `WH_lt (0, _radiusOfCircle)` assertion
   - Cause: Points too close together, nearly collinear
   - Solution: Increase mesh size or add collinearity checks

2. **Constraint Recovery Failure**: Mixed triangles with dummy points
   - Cause: Complex constraint geometry
   - Solution: Use robust CDT with fallback strategies

3. **Memory Issues**: Segmentation faults
   - Cause: Invalid memory access, often from algorithm bugs
   - Solution: Check array bounds, validate pointers

### Debug Output Interpretation
```cpp
DEBUG: Using robust CDT for complex face (segments: 143, nodes: 2320)
DEBUG: Starting robust triangulation for face 7
DEBUG: Triangulation completed for face 7 - mixed triangles: NO
```
This indicates robust CDT is working correctly.

---

## 🔄 Daily Development Practices

### Start Each Session
- [ ] **Check Working Directory**: `cd /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/`
- [ ] **Git Status Check**: `git status` to see current state
- [ ] **Build Verification**: `cd build && make -j4` to ensure code compiles
- [ ] **Basic Test**: Run a simple model to verify functionality

### During Development
- [ ] **Read Before Edit**: Always use Read tool before editing files
- [ ] **Compile After Changes**: Test compilation after each significant change
- [ ] **Small Incremental Changes**: Make minimal changes and test frequently
- [ ] **Document Debug Output**: Add meaningful debug messages for complex operations

### Code Quality for C++
- [ ] **Match WH Library Style**: Follow existing naming and structure patterns
- [ ] **Handle Degenerate Cases**: Check for zero-area triangles, collinear points
- [ ] **Use Robust Predicates**: Prefer robust geometric tests over simple arithmetic
- [ ] **Preserve Performance**: Don't add unnecessary overhead to hot paths
- [ ] **Add Helpful Assertions**: Use WH_ASSERT for invariants that should never fail

### Git Practices for AdvCAD
- [ ] **Descriptive Commit Messages**: Explain the geometric/algorithmic reason for changes
- [ ] **Test Before Commit**: Ensure at least sample/block.gm3d works
- [ ] **Include Claude Credit**: Add "🤖 Generated with Claude Code" to commit messages
- [ ] **Separate Logical Changes**: Don't mix unrelated fixes in one commit

### Documentation Updates
- [ ] **Update CLAUDE.md**: Add new functionality and lessons learned
- [ ] **Update CODEBASE_KNOWLEDGE.md**: Add debugging insights and failure patterns
- [ ] **Comment Complex Algorithms**: Explain geometric reasoning, not just code
- [ ] **Document Test Cases**: Note which models work with which mesh sizes

---

## 🚨 Red Flags - Stop and Reassess

### Code Red Flags
- **Build fails**: Compilation errors after changes
- **Basic tests fail**: sample/block.gm3d stops working
- **Segmentation faults**: New crashes in previously working code
- **Assertion failures**: New WH_ASSERT failures in working cases
- **Performance degradation**: Triangulation becomes noticeably slower

### Process Red Flags
- **Editing without reading**: Getting "File has not been read yet" errors
- **Multi-line string matching**: Edit tool failing on whitespace issues
- **Working from wrong directory**: Path errors with WH/ files
- **Committing without testing**: Not verifying build and basic functionality
- **Making multiple unrelated changes**: Mixing bug fixes with new features
- **Truncated build logs**: Missing critical compiler errors due to incomplete output

### When Red Flags Appear
1. **Stop and build**: `cmake --build build -j4` - ENSURE COMPLETE OUTPUT
2. **Test basic functionality**: `./build/command/advcad sample/block.gm3d tmp/test.pch 2.0`
3. **Check git status**: Understand what changed
4. **Read error messages carefully**: Don't ignore compiler warnings - NEVER TRUNCATE
5. **Make minimal fixes**: Don't add more complexity while debugging

---

## 📝 Task Completion Checklist

### Before Committing Changes
- [ ] Code compiles without errors or warnings: `cmake --build build -j4`
- [ ] Basic test passes: `sample/block.gm3d` processes successfully to `tmp/`
- [ ] No regressions in previously working test cases
- [ ] No .pch files in root directory (`git status` shows no `*.pch`)
- [ ] Git working directory is clean (`git status`)
- [ ] Commit message explains the geometric/algorithmic change
- [ ] CLAUDE.md updated if new functionality was added

### Before Asking for Help
- [ ] Read CLAUDE.md to understand current state
- [ ] Read CODEBASE_KNOWLEDGE.md for common patterns
- [ ] Checked that working directory is correct
- [ ] Tried compiling and tested basic functionality
- [ ] Looked at exact error messages and debug output

---

## 💡 AdvCAD Development Philosophy

**Focus on root causes, not symptoms.**
- Don't just handle errors - understand why they occur
- Geometric algorithms need geometric fixes, not exception handling
- Small mesh sizes failing? Fix the geometry analysis, not just the assertion

**Test with appropriate mesh sizes.**
- Use optimize_mesh_size.py to find appropriate ranges
- Very small mesh sizes often reveal numerical precision issues
- Document which mesh sizes work for which geometries

**Preserve existing functionality.**
- The WH library has decades of development
- Changes should enhance, not break existing capabilities
- When in doubt, add new functionality rather than modifying existing

**When debugging triangulation:**
1. Check the mesh size is appropriate for the geometry
2. Look at debug output to understand what's happening
3. Test with simpler geometries first
4. Use robust geometric predicates for numerical stability
5. **Always use Read tool before Edit tool** - avoid "File has not been read yet" errors