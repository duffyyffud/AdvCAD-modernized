#!/bin/bash

# Build script for AdvCAD Debian package

set -e

echo "Building AdvCAD Debian package..."

# Ensure the project is built first
if [ ! -f build/command/advcad ]; then
    echo "Error: advcad executable not found. Run 'cd build && make -j4' first."
    exit 1
fi

# Calculate installed size (in KB)
INSTALLED_SIZE=$(du -sk debian/usr | cut -f1)

# Add Installed-Size to control file if not present
if ! grep -q "Installed-Size:" debian/DEBIAN/control; then
    echo "Installed-Size: $INSTALLED_SIZE" >> debian/DEBIAN/control
fi

# Set proper permissions
find debian -type d -exec chmod 755 {} \;
find debian -type f -exec chmod 644 {} \;
chmod 755 debian/DEBIAN/postinst
chmod 755 debian/DEBIAN/prerm
chmod 755 debian/usr/bin/advcad

# Compress manual page
if [ -f debian/usr/share/man/man1/advcad.1 ]; then
    gzip -9 debian/usr/share/man/man1/advcad.1
fi

# Compress changelog
if [ -f debian/usr/share/doc/advcad/changelog.Debian ]; then
    gzip -9 debian/usr/share/doc/advcad/changelog.Debian
fi

# Build the package
dpkg-deb --build debian advcad_0.13.0_amd64.deb

echo "Package built successfully: advcad_0.13.0_amd64.deb"
echo ""
echo "To install:"
echo "  sudo dpkg -i advcad_0.13.0_amd64.deb"
echo ""
echo "To remove:"
echo "  sudo dpkg -r advcad"
echo ""
echo "Package info:"
dpkg-deb --info advcad_0.13.0_amd64.deb