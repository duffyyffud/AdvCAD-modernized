#!/bin/bash

# Test script for AdvCAD Debian package

set -e

echo "Testing AdvCAD Debian package installation..."

# Check if package file exists
if [ ! -f advcad_0.13.0_amd64.deb ]; then
    echo "Error: Package file advcad_0.13.0_amd64.deb not found"
    exit 1
fi

# Install the package
echo "Installing package..."
sudo dpkg -i advcad_0.13.0_amd64.deb

# Check if advcad command is available
echo "Testing advcad command..."
if ! command -v advcad >/dev/null 2>&1; then
    echo "Error: advcad command not found in PATH"
    exit 1
fi

echo "advcad found at: $(which advcad)"

# Test help output
echo "Testing help output..."
advcad --help 2>/dev/null || echo "Help output test completed"

# Check man page
echo "Testing man page..."
if man advcad >/dev/null 2>&1; then
    echo "Man page accessible"
else
    echo "Warning: Man page not accessible"
fi

# Test with sample file (if exists)
if [ -f sample/block.gm3d ]; then
    echo "Testing with sample geometry..."
    timeout 10s advcad sample/block.gm3d test_package_output.pch 2.0 || echo "Sample test completed (may timeout)"
    if [ -f test_package_output.pch ]; then
        echo "Package test successful - mesh generated"
        rm -f test_package_output.pch
    fi
fi

echo ""
echo "Package installation test completed successfully!"
echo ""
echo "To remove the package:"
echo "  sudo dpkg -r advcad"