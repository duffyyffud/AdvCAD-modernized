# AdvCAD PPA Setup Guide

## Prerequisites

### 1. Create Launchpad Account
- Go to https://launchpad.net/ and create account
- Complete email verification

### 2. Generate GPG Key
```bash
# Generate GPG key
gpg --gen-key
# Follow prompts: Real name, email, passphrase

# List keys to get key ID
gpg --list-secret-keys --keyid-format LONG
# Copy the key ID (after sec rsa4096/)

# Export public key to Ubuntu keyserver
gpg --keyserver keyserver.ubuntu.com --send-keys YOUR_KEY_ID

# Upload to Launchpad
gpg --armor --export YOUR_KEY_ID
# Copy output and paste into Launchpad GPG Keys section
```

### 3. Install Build Tools
```bash
sudo apt install devscripts debhelper dh-make dput-ng
```

## Create PPA on Launchpad

1. Go to https://launchpad.net/~your-username
2. Click "Create a new PPA"
3. Fill details:
   - **Name**: `advcad`
   - **Display name**: `AdvCAD Repository`
   - **Description**: 
     ```
     AdvCAD - Advanced CAD library for 3D solid modeling and mesh generation
     
     Features:
     - Constrained Delaunay Triangulation with robust predicates
     - 97.5% success rate on stress tests
     - Automatic fallback strategies for complex geometries
     - Support for small-scale precision-sensitive geometries
     ```

## Build and Upload Source Package

### 1. Prepare Source Package
The source package is already prepared in `ppa-source/` directory with:
- Source code (WH/, command/, CMakeLists.txt, etc.)
- Debian packaging files (debian/)
- Orig tarball: `advcad_0.13.0.orig.tar.gz`

### 2. Build Source Package
```bash
cd ppa-source/

# Update changelog for your details
dch -v 0.13.0-1ubuntu1 "Initial PPA release"
# Edit to add your name and email

# Build source package
debuild -S -sa

# This creates in parent directory:
# - advcad_0.13.0-1ubuntu1.dsc
# - advcad_0.13.0-1ubuntu1.tar.xz  
# - advcad_0.13.0-1ubuntu1_source.changes
```

### 3. Upload to PPA
```bash
# Upload to your PPA
dput ppa:your-username/advcad ../advcad_0.13.0-1ubuntu1_source.changes
```

## Users Install from PPA

Once uploaded and built, users can install with:

```bash
# Add PPA
sudo add-apt-repository ppa:your-username/advcad
sudo apt update

# Install AdvCAD
sudo apt install advcad

# Use AdvCAD
advcad input.gm3d output.pch 2.0
man advcad
```

## Troubleshooting

### Build Failures
- Check build logs on Launchpad PPA page
- Common issues: missing dependencies, CMake configuration
- Update `debian/control` Build-Depends if needed

### GPG Issues
```bash
# If key not found error
gpg --keyserver keyserver.ubuntu.com --recv-keys YOUR_KEY_ID

# Re-upload key
gpg --keyserver keyserver.ubuntu.com --send-keys YOUR_KEY_ID
```

### Upload Errors
```bash
# Check dput configuration
cat ~/.dput.cf

# Force upload
dput --force ppa:your-username/advcad ../advcad_0.13.0-1ubuntu1_source.changes
```

## Multiple Ubuntu Versions

To support multiple Ubuntu versions (focal, jammy, kinetic, etc.):

1. Update changelog for each version:
```bash
dch -v 0.13.0-1ubuntu1~focal1 "Build for Ubuntu 20.04 Focal"
dch -v 0.13.0-1ubuntu1~jammy1 "Build for Ubuntu 22.04 Jammy"
```

2. Build and upload for each version

## Repository URL

After setup, users access your PPA at:
- **Web**: https://launchpad.net/~your-username/+archive/ubuntu/advcad
- **APT**: `ppa:your-username/advcad`

## Files Created

- `advcad_0.13.0.orig.tar.gz` - Source tarball
- `ppa-source/` - Source package directory
- `ppa-source/debian/` - Debian packaging files
- `PPA_SETUP.md` - This guide