#!/bin/bash

# Script to create a custom APT repository

set -e

REPO_DIR="apt-repo"
GPG_KEY_ID="your-gpg-key-id"  # Replace with your GPG key ID

echo "Setting up custom APT repository..."

# Create repository structure
mkdir -p $REPO_DIR/pool/main/a/advcad
mkdir -p $REPO_DIR/dists/stable/main/binary-amd64

# Copy .deb package
cp advcad_0.13.0_amd64.deb $REPO_DIR/pool/main/a/advcad/

# Generate Packages file
cd $REPO_DIR
dpkg-scanpackages pool/ /dev/null > dists/stable/main/binary-amd64/Packages
gzip -k dists/stable/main/binary-amd64/Packages

# Create Release file
cat > dists/stable/Release << EOF
Origin: AdvCAD Repository
Label: AdvCAD
Suite: stable
Codename: stable
Date: $(date -Ru)
Architectures: amd64
Components: main
Description: AdvCAD APT Repository
EOF

# Calculate checksums
cd dists/stable
find . -name "Packages*" -exec md5sum {} \; >> Release
find . -name "Packages*" -exec sha1sum {} \; >> Release
find . -name "Packages*" -exec sha256sum {} \; >> Release

# Sign the Release file (requires GPG key)
if [ "$GPG_KEY_ID" != "your-gpg-key-id" ]; then
    gpg --armor --detach-sign --sign -u $GPG_KEY_ID Release
fi

cd ../..

echo "APT repository created in $REPO_DIR/"
echo ""
echo "To host the repository:"
echo "1. Upload $REPO_DIR/ contents to your web server"
echo "2. Users add repository with:"
echo "   echo 'deb https://your-domain.com/apt-repo stable main' | sudo tee /etc/apt/sources.list.d/advcad.list"
echo "   sudo apt update"
echo "   sudo apt install advcad"
echo ""
echo "For GitHub Pages hosting:"
echo "1. Create gh-pages branch"
echo "2. Copy $REPO_DIR/ contents to gh-pages"
echo "3. Repository URL: https://username.github.io/repository-name/"