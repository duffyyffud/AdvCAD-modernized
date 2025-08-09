# tmp/ - Temporary Files Directory

This directory is for temporary .pch output files during development and testing.

## Usage

```bash
# Quick testing
./build/command/advcad input.gm3d tmp/output.pch 2.0

# Debug testing with different levels
./build/command/advcad --debug=2 input.gm3d tmp/debug.pch 1.0
```

## Guidelines

- **All files here are ignored by git**
- Use descriptive names: `tmp/test_block_small.pch`
- Clean up periodically: `rm tmp/*.pch`
- For reference outputs, use `sample/` or `tests/data/`

## File Organization

Suggested naming patterns:
- `tmp/test_[geometry]_[meshsize].pch` - Testing files
- `tmp/debug_[issue].pch` - Debugging specific issues  
- `tmp/experiment_[feature].pch` - Feature development