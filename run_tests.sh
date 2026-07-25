#!/bin/bash
# AdvCAD Test Runner
# Builds and runs regression tests

set -e

echo "🔨 Building AdvCAD..."
cd build && make -j4 && cd ..

echo ""
echo "🧪 Running regression tests..."
python3 dev/test_regression.py

echo ""
echo "✅ Test run complete!"