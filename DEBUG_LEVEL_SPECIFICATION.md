# Debug Level Specification for AdvCAD

## Debug Level 0 (WH_DEBUG_SILENT) - Required Progress Messages

The following messages MUST be visible at debug level 0 for user progress tracking:

### Main Pipeline Stages
- `generateNodesOnVertexs`
- `generateMeshAlongEdges`
- `generateMeshOverFaces`

### Results
- Triangle counts (final mesh statistics)

## Classification Guidelines

- **Level 0 (SILENT)**: Essential progress + results + warnings
  - Main pipeline stage announcements (above)
  - Final triangle counts
  - Critical warnings/errors

- **Level 1 (NORMAL)**: + detailed status messages
  - Secondary progress information
  - Algorithm selection messages (high-level)

- **Level 2 (VERBOSE)**: + debug information
  - Internal algorithmic details
  - Constraint recovery specifics
  - Face-by-face processing details

- **Level 3 (TRACE)**: + full trace including loops and arrays
  - Loop iterations
  - Array contents
  - Detailed geometric calculations

## User Requirements
User specifically requested that debug level 0 should show approximately 3-4 progress reports focusing on the main pipeline stages and final results, not internal algorithmic debugging details.