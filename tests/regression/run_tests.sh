#!/bin/bash
# AdvCAD Test Runner
# Builds and runs regression tests

set -e

echo "🔨 Building AdvCAD..."
cd build && make -j4 && cd ..

echo ""
echo "🧪 Running regression tests..."
cd tests/regression && python3 test_regression.py

echo ""
echo "✅ Test run complete!"