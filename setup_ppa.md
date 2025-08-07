# Ubuntu PPA Setup for AdvCAD

## Prerequisites
1. Create Launchpad account: https://launchpad.net/
2. Generate GPG key and upload to Ubuntu keyserver
3. Install packaging tools:
   ```bash
   sudo apt install devscripts debhelper dh-make
   ```

## Steps to Create PPA

### 1. Create PPA on Launchpad
- Go to https://launchpad.net/~your-username
- Click "Create a new PPA"
- Name: `advcad`
- Description: "AdvCAD - Advanced CAD library for 3D solid modeling"

### 2. Prepare Source Package
```bash
# Create source directory
mkdir ppa-source && cd ppa-source

# Extract source
cp -r ../WH ../command ../CMakeLists.txt ../Makefile ../README ../copyright .
cp -r ../debian .

# Create orig tarball
cd ..
tar --exclude=debian -czf advcad_0.13.0.orig.tar.gz ppa-source/
cd ppa-source
```

### 3. Build Source Package
```bash
# Update changelog for PPA
dch -v 0.13.0-1ubuntu1 "Initial PPA release"

# Build source package
debuild -S -sa

# Upload to PPA
cd ..
dput ppa:your-username/advcad advcad_0.13.0-1ubuntu1_source.changes
```

### 4. Users Install Via PPA
```bash
sudo add-apt-repository ppa:your-username/advcad
sudo apt update
sudo apt install advcad
```

## Advantages
- Official Ubuntu infrastructure
- Automatic builds for multiple Ubuntu versions
- Trusted by users
- Easy installation via apt
- No hosting costs