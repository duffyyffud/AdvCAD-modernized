# CMAKE Migration & PCH Robustness Plan

## Overview
Migration from traditional Makefile to CMake build system while implementing PCH file validation for mesh integrity checking. **TARGET: Clean flat structure with src/WH/ and src/command/ only.**

## Current Status
- **Phase:** Directory structure cleanup (TOP PRIORITY)
- **Issue:** Files scattered in subdirectories (src/lib/geometry/, src/lib/utils/, etc.)
- **Target:** Simple flat structure: src/WH/ (all library files) and src/command/ (advcad executable)
- **Blocking:** Must clean up directory mess before CMake can work

## Tasks

### Phase 1: Directory Structure Cleanup (TOP PRIORITY)

#### 🔄 Task 1: Restore flat directory structure
- **Status:** IN PROGRESS
- **Issue:** Files scattered in src/lib/geometry/, src/lib/utils/, src/lib/algorithms/, src/lib/mesh/
- **Target Structure:** 
  - `src/WH/` - All library source files (flat)
  - `src/command/` - advcad executable source
- **Action:** Move all .cc/.h files to flat src/WH/, eliminate subdirectories

#### ⏳ Task 2: Fix CMake for flat src/WH structure  
- **Status:** PENDING
- **Dependencies:** Task 1 complete
- **Action:** Update CMakeLists.txt to build from clean flat structure

#### ⏳ Task 3: Test build with shaft models
- **Status:** PENDING
- **Action:** Verify ./build/command/advcad works with all test models
- **Success Criteria:** All 6 shaft models build and run without crashes

### Phase 2: PCH Validation Integration (MEDIUM PRIORITY)

#### ✅ Task 4: Create PchReader.py script
- **Status:** COMPLETED
- **Location:** scripts/PchReader.py
- **Features:** Connectivity integrity checking, mixed triangle detection

#### ⏳ Task 5: Integrate PchReader into regression tests
- **Status:** PENDING
- **Action:** Add PCH validation to tests/regression/test_regression.py
- **Dependencies:** Working build system (Phase 1)

#### ⏳ Task 6: Enforce validation in test suite
- **Status:** PENDING
- **Action:** Make regression tests fail on connectivity issues
- **Success Criteria:** Face 7 mixed triangles caught automatically

#### ⏳ Task 7: Test PchReader on existing files
- **Status:** PENDING (LOW PRIORITY)
- **Action:** Validate existing .pch files in tests/data/shaft/

## Current Blockers

1. **CMake Configuration:** Include paths not working for subdirectory structure
2. **Build Dependencies:** Cannot test PCH validation until build works

## Next Steps

1. Fix CMakeLists.txt for actual directory structure
2. Systematically resolve include path errors
3. Test build with shaft models
4. Integrate PCH validation into test suite

## Success Criteria

- [ ] CMake build completes successfully
- [ ] All shaft models generate valid .pch files
- [ ] PCH validation integrated into regression tests
- [ ] Mixed triangle issues caught automatically
- [ ] Clean, maintainable build system

## Files Modified

- `src/lib/CMakeLists.txt` - CMake configuration
- `scripts/PchReader.py` - PCH validation tool
- `tests/regression/test_regression.py` - Will be updated for PCH validation

## Observer Self Notes

- Keep discipline: systematic approach over rushing
- Economic principle: fix what exists rather than rebuild
- 80% implementation, 20% process observation
- Working directory rule: always from project root