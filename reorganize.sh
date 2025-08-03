#!/bin/bash
# AdvCAD Directory Reorganization Script
# Cleans up the messy directory structure into a proper organization

set -e

echo "🧹 AdvCAD Directory Reorganization"
echo "=================================="

# Check if we're in the right directory
if [[ ! -f "CLAUDE.md" ]]; then
    echo "❌ Error: Must run from AdvCAD project root"
    exit 1
fi

echo "📁 Creating new directory structure..."

# Create new directory structure
mkdir -p src/lib/{geometry,mesh,algorithms,utils}
mkdir -p src/apps/advcad
mkdir -p tests/{regression,data/{basic,shaft},output,debug}
mkdir -p docs/{api,algorithms,user}
mkdir -p scripts

echo "📦 Moving source code..."

# Move WH library to src/lib, organizing by category
echo "  - Moving geometry classes..."
mv WH/*2d.* WH/*3d.* WH/polygon* WH/segment* WH/triangle* WH/space* src/lib/geometry/ 2>/dev/null || true

echo "  - Moving mesh generation..."
mv WH/mg3d* WH/tessellate* WH/delaunay* WH/constdel* WH/afront* src/lib/mesh/ 2>/dev/null || true

echo "  - Moving algorithms..."
mv WH/robust_* WH/inout* WH/gm*_setop* src/lib/algorithms/ 2>/dev/null || true

echo "  - Moving utilities..."
mv WH/bucket* WH/field* WH/hashtable* WH/sorter* WH/scalar* WH/common* WH/coverage* WH/connector* src/lib/utils/ 2>/dev/null || true

echo "  - Moving remaining core files..."
mv WH/* src/lib/ 2>/dev/null || true
rmdir WH 2>/dev/null || true

echo "  - Moving application..."
mv command/* src/apps/advcad/ 2>/dev/null || true
rmdir command 2>/dev/null || true

echo "🧪 Moving test infrastructure..."

# Move test files
mv test_regression.py run_tests.sh tests/regression/
mv test_all_shaft_models.py optimize_mesh_size.py scripts/
mv visualize_face7.py scripts/ 2>/dev/null || true

# Move test data
echo "  - Moving test input data..."
mv sample/shaft/* tests/data/shaft/ 2>/dev/null || true
mv sample/*.gm3d tests/data/basic/ 2>/dev/null || true
rm -rf sample

# Move test outputs
echo "  - Moving test outputs..."
mv test_*.pch output.pch tests/output/ 2>/dev/null || true
mv test_results/* tests/output/ 2>/dev/null || true
rmdir test_results 2>/dev/null || true

# Move debug files
echo "  - Moving debug files..."
mv face*.debug face*.png tests/debug/ 2>/dev/null || true

echo "📚 Moving documentation..."

# Move documentation
mv doc/* docs/user/ 2>/dev/null || true
rmdir doc 2>/dev/null || true
mv *.md docs/ 2>/dev/null || true

# Keep essential root files
mv docs/README . 2>/dev/null || true
mv docs/CLAUDE.md . 2>/dev/null || true

echo "🔧 Moving scripts..."

# Move remaining scripts
mv *.py scripts/ 2>/dev/null || true

echo "🗑️  Cleaning up build artifacts..."

# Clean up build directories properly
rm -rf build_test WH/build_test 2>/dev/null || true

# Remove object files scattered in source
find src/ -name "*.o" -delete 2>/dev/null || true
find src/ -name "*.a" -delete 2>/dev/null || true

echo "📝 Creating new CMakeLists.txt structure..."

# Create new main CMakeLists.txt
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.10)
project(AdvCAD VERSION 0.12)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add subdirectories
add_subdirectory(src/lib)
add_subdirectory(src/apps/advcad)

# Optional: Add testing
enable_testing()
add_subdirectory(tests OPTIONAL)
EOF

# Create lib CMakeLists.txt
cat > src/lib/CMakeLists.txt << 'EOF'
# WH Core Library
file(GLOB_RECURSE WH_SOURCES 
    "geometry/*.cc"
    "mesh/*.cc" 
    "algorithms/*.cc"
    "utils/*.cc"
    "*.cc"
)

add_library(WH ${WH_SOURCES})
target_include_directories(WH PUBLIC .)
EOF

# Create app CMakeLists.txt
cat > src/apps/advcad/CMakeLists.txt << 'EOF'
# AdvCAD Application
add_executable(advcad advcad.cc)
target_link_libraries(advcad WH)
EOF

echo "📋 Creating directory documentation..."

# Create README files for each major directory
cat > src/README.md << 'EOF'
# Source Code

- `lib/` - Core WH library organized by functionality
- `apps/` - Applications built using the WH library
EOF

cat > tests/README.md << 'EOF'
# Tests

- `regression/` - Automated regression test suite
- `data/` - Test input files (.gm3d)
- `output/` - Test output files (.pch) 
- `debug/` - Debug files and visualizations
EOF

cat > docs/README.md << 'EOF'
# Documentation

- `api/` - API documentation
- `algorithms/` - Algorithm explanations and research
- `user/` - User guides and tutorials
EOF

cat > scripts/README.md << 'EOF'
# Utility Scripts

- `optimize_mesh_size.py` - Mesh size optimization
- `visualize_face7.py` - Face triangulation visualization
- `test_all_shaft_models.py` - Comprehensive model testing
EOF

echo "🔄 Updating paths in scripts..."

# Update test_regression.py to use new paths
if [[ -f "tests/regression/test_regression.py" ]]; then
    sed -i 's|build/command/advcad|build/src/apps/advcad/advcad|g' tests/regression/test_regression.py
    sed -i 's|sample/shaft|tests/data/shaft|g' tests/regression/test_regression.py
fi

# Update run_tests.sh
if [[ -f "tests/regression/run_tests.sh" ]]; then
    sed -i 's|python3 test_regression.py|cd tests/regression \&\& python3 test_regression.py|g' tests/regression/run_tests.sh
fi

echo "✅ Reorganization complete!"
echo ""
echo "📊 New directory structure:"
echo "  src/lib/         - Clean organized source code"
echo "  tests/           - Centralized test infrastructure" 
echo "  docs/            - All documentation"
echo "  scripts/         - Utility scripts"
echo "  build/           - Clean build directory"
echo ""
echo "🔧 Next steps:"
echo "  1. cd build && cmake .. && make -j4"
echo "  2. Run tests: ./tests/regression/run_tests.sh"
echo "  3. Commit reorganization: git add -A && git commit"