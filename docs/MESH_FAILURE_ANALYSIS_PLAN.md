# Mesh Size Failure Analysis Plan

## Objective
Map out failure boundaries for currently passing models to discover algorithm weaknesses and edge cases using relative mesh size factors.

## Background
- **Current Status**: 84.2% success rate (16/19 models passing)
- **Problem**: We only know "a mesh size that works" for each model
- **Opportunity**: Working models may fail at different mesh sizes, revealing algorithm limits
- **Value**: Create reproducible test cases for debugging core algorithms

## Methodology

### Test Size Calculation (Relative Factors)
Instead of absolute mesh sizes, use **relative factors** multiplied by each model's optimal mesh size:

```
Optimal mesh size (model-specific) × Factor = Test mesh size
```

**Example**:
- `block.gm3d`: optimal = 0.14 → test sizes: [0.0014, 0.007, 0.014, ..., 1.4]
- `test_1.gm3d`: optimal = 1.0 → test sizes: [0.01, 0.05, 0.1, ..., 10.0]

### Test Factors Range
```
[0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0]
```

- **0.01-0.1**: Extremely fine (numerical precision stress test)
- **0.25-0.75**: Finer than optimal (algorithm robustness)  
- **1.0**: Optimal (should work - baseline)
- **1.5-3.0**: Coarser than optimal (feature capture test)
- **5.0-10.0**: Very coarse (geometric detail loss)

### Target Models (16 Working Models)
```
Basic: block.gm3d
Tests: test_1.gm3d through test_7.gm3d  
Cake: cake/Magnetic0-3.gm3d
Shaft: shaft/air_up2_mid_out_01.gm3d, shaft/coil_01*.gm3d, shaft/coil_02.gm3d
```

## Implementation Plan

### Phase 1: Tool Development
Create `mesh_failure_analyzer.py` with:

1. **Relative Mesh Calculation**
   ```python
   def get_test_mesh_sizes(self, model_file):
       base_mesh = self.calculate_optimal_mesh_size(model_file)
       factors = [0.01, 0.05, 0.1, 0.25, 0.5, 0.75, 1.0, 1.5, 2.0, 3.0, 5.0, 10.0]
       return [base_mesh * factor for factor in factors]
   ```

2. **Failure Categorization**
   - `ZERO_LENGTH_VECTOR`: Division by zero in 2D operations
   - `ASSERTION_FAILURE`: Other WH_ASSERT failures with location
   - `MIXED_TRIANGLES`: Face 7 issue (real + dummy points)
   - `TIMEOUT`: Computational explosion (mesh too fine)
   - `MEMORY_ERROR`: Out of memory allocation
   - `OTHER`: Unclassified failures

3. **Boundary Detection**
   - Find transition points: SUCCESS → FAIL and FAIL → SUCCESS
   - Identify safe operating ranges for each model
   - Document critical failure thresholds

### Phase 2: Systematic Testing
**Test Matrix**: 16 models × 12 factors = **192 total tests**

**Expected Output Format**:
```
Model: test_1.gm3d
Optimal: 1.0

Factor | Mesh Size | Status  | Failure Type        | Location
-------|-----------|---------|--------------------|-----------------
0.01   | 0.01      | FAIL    | TIMEOUT            | generateMeshOverFaces
0.05   | 0.05      | FAIL    | MEMORY_ERROR       | triangle allocation  
0.1    | 0.10      | FAIL    | ZERO_LENGTH_VECTOR | space2d_inline.cc:109
0.5    | 0.50      | SUCCESS | -                  | -
1.0    | 1.00      | SUCCESS | -                  | - (baseline)
1.5    | 1.50      | SUCCESS | -                  | -
2.0    | 2.00      | FAIL    | ZERO_LENGTH_VECTOR | space2d_inline.cc:109

Safe range: [0.5, 1.5] (factors)
Critical transitions: 0.1→0.5 (fail→success), 1.5→2.0 (success→fail)
```

### Phase 3: Analysis & Insights

1. **Pattern Recognition**
   - Models sensitive to fine mesh (numerical issues)
   - Models sensitive to coarse mesh (feature loss)
   - Models with narrow safe ranges (algorithm brittleness)

2. **Algorithm Stress Points**
   - Which mesh factors trigger which failure modes?
   - Are failures consistent across similar models?
   - Do certain geometric features correlate with failure patterns?

3. **Debugging Value**
   - Reproducible test cases for each failure mode
   - Boundary conditions for algorithm development
   - Safe operating parameter documentation

### Phase 4: Deliverables

1. **`mesh_failure_analyzer.py`** - Analysis tool
   ```bash
   python3 mesh_failure_analyzer.py --model test_1.gm3d    # Single model
   python3 mesh_failure_analyzer.py --all                 # All 16 models  
   python3 mesh_failure_analyzer.py --report              # Generate report
   ```

2. **`mesh_failure_report.md`** - Comprehensive results
   - Failure pattern summary
   - Safe range documentation  
   - Critical transition analysis
   - Algorithm improvement recommendations

3. **`mesh_failure_results.json`** - Raw data for further analysis

## Expected Discoveries

### Failure Patterns
- **Ultra-fine mesh failures**: Numerical precision limits, memory exhaustion
- **Ultra-coarse mesh failures**: Feature under-resolution, geometric degeneracy  
- **Model-specific sensitivities**: Some models more robust than others
- **Algorithm bottlenecks**: Specific failure modes at predictable boundaries

### Debugging Value
- **Reproducible edge cases** for algorithm development
- **Stress test suite** for future improvements
- **Production guidelines** for safe mesh size selection
- **Algorithm robustness metrics** (safe range width)

## Success Criteria

1. **Complete failure mapping** for all 16 working models
2. **Identify 3+ distinct failure patterns** across the test matrix
3. **Create reproducible test cases** for each failure mode
4. **Document safe operating ranges** for production use
5. **Generate actionable insights** for algorithm improvements

## Integration with Existing Work

- **Leverages**: Current 84.2% success rate and mesh size optimizer
- **Extends**: From "finding working mesh" to "mapping failure boundaries"  
- **Enables**: Systematic debugging of the remaining 15.8% failure rate
- **Provides**: Foundation for pushing beyond 84.2% toward 100% success

This analysis turns our successful models into debugging tools by systematically finding their breaking points.