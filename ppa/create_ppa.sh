#!/bin/bash

# Script to prepare AdvCAD for PPA upload

set -e

echo "Preparing AdvCAD for PPA upload..."

# Create source package structure
mkdir -p ppa_build/advcad-0.13.0

# Copy source files
cp -r WH command CMakeLists.txt Makefile README copyright ppa_build/advcad-0.13.0/

# Create debian directory in source
cp -r debian ppa_build/advcad-0.13.0/

# Create orig tarball
tar -czf ppa_build/advcad_0.13.0.orig.tar.gz -C ppa_build advcad-0.13.0 --exclude=debian

echo "Source package prepared in ppa_build/"
echo ""
echo "Next steps for PPA upload:"
echo "1. cd ppa_build/advcad-0.13.0"
echo "2. debuild -S -sa"
echo "3. dput ppa:your-launchpad-id/advcad ../advcad_0.13.0-1_source.changes"
echo ""
echo "Prerequisites:"
echo "- Create Launchpad account and PPA"
echo "- Install: sudo apt install devscripts debhelper"
echo "- Configure GPG key for package signing"