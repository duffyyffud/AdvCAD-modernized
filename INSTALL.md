# AdvCAD Installation Guide

## Option 1: Direct .deb Package Installation

Download the latest release from [GitHub Releases](https://github.com/advcad/advcad/releases):

```bash
# Download the package
wget https://github.com/advcad/advcad/releases/download/v0.13.0/advcad_0.13.0_amd64.deb

# Install
sudo dpkg -i advcad_0.13.0_amd64.deb

# If dependencies missing, run:
sudo apt-get install -f
```

## Option 2: PPA Repository (Ubuntu)

```bash
# Add PPA repository
sudo add-apt-repository ppa:username/advcad
sudo apt update

# Install
sudo apt install advcad
```

## Option 3: Custom APT Repository

```bash
# Add repository
echo 'deb https://your-domain.com/apt-repo stable main' | sudo tee /etc/apt/sources.list.d/advcad.list

# Add GPG key (if signed)
wget -qO - https://your-domain.com/apt-repo/KEY.gpg | sudo apt-key add -

# Install
sudo apt update
sudo apt install advcad
```

## Usage

```bash
# Basic usage
advcad input.gm3d output.pch 2.0

# View manual
man advcad

# Example with sample geometry
advcad /usr/lib/advcad/sample/block.gm3d my_mesh.pch 1.0
```

## System Requirements

- **OS**: Ubuntu 18.04+ or Debian 10+
- **Architecture**: x86_64 (amd64)
- **Dependencies**: 
  - libc6 (>= 2.14)
  - libstdc++6 (>= 9)

## Uninstallation

```bash
sudo dpkg -r advcad
```

## Building from Source

See [BUILD.md](BUILD.md) for compilation instructions.