# Tests

- `regression/` - Automated regression test suite
- `data/` - Test input files (.gm3d) and reference outputs (.pch) - **TRACKED**
- `output/` - Temporary test output files (.pch) - **IGNORED BY GIT**
- `debug/` - Debug files and visualizations

## File Organization

**Tracked (.pch files saved in git):**
- `data/` - Reference/validation outputs for regression testing

**Ignored (.pch files not saved in git):**
- `output/` - Temporary test script outputs
- `/tmp/` - Development and quick testing
- Root level - Any `/*.pch` files
