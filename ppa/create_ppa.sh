#!/bin/bash

# Script to prepare AdvCAD for PPA upload.
# Run from the workspace root: bash ppa/create_ppa.sh
#
# Note (2026-07-26): this script previously copied the workspace-root
# debian/ directory, but that is a DEBIAN/+usr/ flat tree built by
# build_deb.sh for direct .deb download (see INSTALL.md) — NOT a
# debhelper source-package debian/ (control/rules/compat/changelog).
# The real debhelper debian/, recovered from the successful 0.13.0-1ubuntu1
# upload's advcad_0.13.0-1ubuntu1.debian.tar.xz, now lives at ppa/debian/
# and is what this script uses.

set -e

echo "Preparing AdvCAD for PPA upload..."

# Create source package structure
mkdir -p ppa/build/advcad-0.13.0

# Copy source files
cp -r WH command CMakeLists.txt README_en.md copyright ppa/build/advcad-0.13.0/

# Copy the debhelper packaging directory
cp -r ppa/debian ppa/build/advcad-0.13.0/

# Create orig tarball (--exclude must precede the positional args for this tar version)
tar --exclude=debian -czf ppa/advcad_0.13.0.orig.tar.gz -C ppa/build advcad-0.13.0

echo "Source package prepared in ppa/build/"
echo ""
echo "Next steps for PPA upload:"
echo "1. cd ppa/build/advcad-0.13.0"
echo "2. debuild -S -sa"
echo "3. dput ppa:your-launchpad-id/advcad ../advcad_0.13.0-1_source.changes"
echo ""
echo "Prerequisites:"
echo "- Create Launchpad account and PPA"
echo "- Install: sudo apt install devscripts debhelper"
echo "- Configure GPG key for package signing"