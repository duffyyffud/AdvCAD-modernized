#!/bin/bash

# Build script for AdvCAD Debian package.
# Run from the workspace root: bash deb/build_deb.sh

set -e

echo "Building AdvCAD Debian package..."

# Ensure the project is built first
if [ ! -f build/command/advcad ]; then
    echo "Error: advcad executable not found. Run 'cd build && make -j4' first."
    exit 1
fi

# Calculate installed size (in KB)
INSTALLED_SIZE=$(du -sk deb/debian/usr | cut -f1)

# Add Installed-Size to control file if not present
if ! grep -q "Installed-Size:" deb/debian/DEBIAN/control; then
    echo "Installed-Size: $INSTALLED_SIZE" >> deb/debian/DEBIAN/control
fi

# Set proper permissions
find deb/debian -type d -exec chmod 755 {} \;
find deb/debian -type f -exec chmod 644 {} \;
chmod 755 deb/debian/DEBIAN/postinst
chmod 755 deb/debian/DEBIAN/prerm
chmod 755 deb/debian/usr/bin/advcad

# Compress manual page
if [ -f deb/debian/usr/share/man/man1/advcad.1 ]; then
    gzip -9 deb/debian/usr/share/man/man1/advcad.1
fi

# Compress changelog
if [ -f deb/debian/usr/share/doc/advcad/changelog.Debian ]; then
    gzip -9 deb/debian/usr/share/doc/advcad/changelog.Debian
fi

# Build the package
dpkg-deb --build deb/debian deb/advcad_0.13.0_amd64.deb

echo "Package built successfully: deb/advcad_0.13.0_amd64.deb"
echo ""
echo "To install:"
echo "  sudo dpkg -i deb/advcad_0.13.0_amd64.deb"
echo ""
echo "To remove:"
echo "  sudo dpkg -r advcad"
echo ""
echo "Package info:"
dpkg-deb --info deb/advcad_0.13.0_amd64.deb