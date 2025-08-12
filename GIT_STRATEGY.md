# Git Branch Management & Release Strategy for AdvCAD

## 🎯 Branch Strategy

### Main Branches

1. **`main`** (or `master`)
   - Production-ready, stable releases only
   - Protected branch - no direct commits
   - Each merge = new stable version tag (v0.16.0, v0.17.0)

2. **`develop`**
   - Integration branch for next release
   - All feature branches merge here first
   - Regular testing and validation
   - Release candidates tagged here (v0.16.0-rc1)

### Feature Branches
```
feature/gui-viewer      (3D viewer improvements)
feature/editor-tools    (Editor enhancements)
fix/mesh-generation    (Bug fixes)
```
- Branch from `develop`
- Merge back to `develop` when complete
- Delete after merge

### Release Branches
```
release/v0.16.0
```
- Branch from `develop` when features complete
- Only bug fixes allowed
- Merge to both `main` and `develop`
- Tag on `main` after merge

## 📦 Version & Tag Strategy

### Version Numbering
```
v[MAJOR].[MINOR].[PATCH][-SUFFIX]

v0.16.0         - Stable release
v0.16.1         - Patch/hotfix
v0.16.0-rc1     - Release candidate
v0.16.0-beta    - Beta testing
```

### Tag Categories

**Stable Releases** (on `main`):
- `v0.16.0` - GUI Enhancements Release
- `v0.17.0` - Next major feature set
- **Purpose**: Production-ready code for end users

**Development Milestones** (on `develop`):
- `v0.16.0-rc1` - Release candidate
- `v0.16.0-beta` - Beta version
- **Purpose**: Testing versions before stable release

**Safety Tags** (on development branches):
- `v0.15.1-feature-working` - Working milestone for rollback safety
- `v0.15.2-fix-applied` - Safe checkpoint after fix
- `v0.15.3-tested-stable` - Verified working state
- **Purpose**: Granular rollback points during complex development

**Feature Checkpoints** (on feature branches):
- `checkpoint-viewer-quaternion`
- `checkpoint-projection-toggle`
- **Purpose**: Temporary markers during active development

## 🔄 Workflow Example

```
main ────────●─────────────────●────────
             ↑ v0.16.0         ↑ v0.17.0
              \               /
release/v0.16 ●─────●────────●
                    ↑ v0.16.0-rc1
                     \
develop ──●───●───●───●───●───●───●────
          ↑           ↑       ↑
         /           /       /
feature/─●───●      /       /
               \   /       /
feature/────────●─●       /
                     \   /
fix/──────────────────●─●
```

## 🔄 Safety Tags & Rollback Strategy

### Why Multiple Tags During Development

**Complex development requires safety nets:**
- 3D graphics changes can introduce subtle regressions
- Camera control modifications affect user experience
- Mesh generation fixes must not break existing functionality
- Each working state is valuable for troubleshooting

### Rollback Capabilities

**Rolling back to any tag:**
```bash
# List all tags to find rollback point
git tag --sort=-version:refname

# Rollback to specific tag (e.g., v0.15.4-axes-labels)
git checkout v0.15.4-axes-labels

# Create new branch from that point
git checkout -b rollback-to-v0.15.4

# Or reset current branch (DESTRUCTIVE - use with caution)
git reset --hard v0.15.4-axes-labels
git push origin current-branch --force
```

**Safety tag examples:**
- `v0.15.1-quaternion-working` - Quaternion rotation implemented and tested
- `v0.15.2-projection-toggle` - Projection switching verified
- `v0.15.3-camera-controls` - Camera controls finalized
- `v0.15.4-axes-labels` - Coordinate labels working
- `v0.15.5-geometry-centered` - Rotation centering fixed
- `v0.15.6-intuitive-controls` - Camera-local rotation implemented

### Development Safety Strategy

1. **Tag frequently** during complex development phases
2. **Test thoroughly** before each safety tag
3. **Use descriptive names** that explain what's working
4. **Keep granular history** for bisection and debugging
5. **Consolidate later** into official releases

### Best Practices for Safety Tags

**Good safety tag names:**
- ✅ `v0.15.x-feature-working` - Clear what's functional
- ✅ `v0.15.x-fix-applied` - Indicates fix completion
- ✅ `v0.15.x-tested-stable` - Verified functionality

**Poor safety tag names:**
- ❌ `v0.15.x-temp` - Unclear purpose
- ❌ `v0.15.x-backup` - Not descriptive
- ❌ `v0.15.x-final` - Misleading (nothing is final)

## 🚀 Migration Plan

### Current State
- Branch: `v0.15.0-gui-enhancements` (poor name)
- Many incremental tags: v0.15.1 through v0.15.6

### Migration Steps

1. **Create proper branch structure:**
```bash
# Create develop from current work
git checkout -b develop

# Push develop branch
git push origin develop

# Create main if doesn't exist
git checkout master  # or main
git push origin main
```

2. **Clean up tags:**
```bash
# Create consolidated release candidate
git tag v0.16.0-rc1 HEAD

# Document feature set in tag message
git tag -a v0.16.0-rc1 -m "Release Candidate: GUI Enhancements
- 3D viewer with quaternion rotation
- Perspective/orthographic projection toggle  
- Camera-local controls
- Geometry-centered rotation
- Coordinate axes with labels"
```

3. **After testing, create stable release:**
```bash
# On main branch
git checkout main
git merge --no-ff develop
git tag v0.16.0
```

## 📋 Branch Naming Conventions

### Feature branches
- `feature/description` - New features
- `fix/issue-description` - Bug fixes
- `refactor/component` - Code refactoring
- `docs/topic` - Documentation only

### Bad names (avoid these)
- ❌ `v0.15.0-gui-enhancements` - Version in branch name
- ❌ `gui-fixes-final` - Vague, temporal
- ❌ `johns-branch` - Personal names

### Good names (use these)
- ✅ `feature/3d-viewer`
- ✅ `fix/mesh-water-tight`
- ✅ `release/v0.16.0`

## 🏷️ Development vs Stable Distinction

### Stable (on `main`)
- Full version tags only (v0.16.0)
- Thoroughly tested
- Documentation updated
- No known critical bugs
- Water-tight mesh generation verified
- All GUI features functional

### Development (on `develop`/features)
- Suffix tags (-rc1, -beta, -alpha)
- Active development
- May have known issues
- Experimental features
- Debug output may be verbose

## 📝 Commit Message Standards

### Format
```
<type>: <subject>

<body>

<footer>
```

### Types
- `feat`: New feature
- `fix`: Bug fix
- `refactor`: Code refactoring
- `docs`: Documentation only
- `test`: Test additions/changes
- `perf`: Performance improvements

### Examples
```
feat: Add quaternion rotation to 3D viewer

Implement trackball-style rotation using quaternions to prevent
gimbal lock and numerical drift. Replaces Euler angle system.

🤖 Generated with Claude Code
Co-Authored-By: Claude <noreply@anthropic.com>
```

## 🔍 Git Commands Reference

### Daily Workflow
```bash
# Start new feature
git checkout develop
git pull origin develop
git checkout -b feature/my-feature

# Work and commit
git add .
git commit -m "feat: Add feature description"

# Merge back
git checkout develop
git merge --no-ff feature/my-feature
git push origin develop
```

### Release Process
```bash
# Start release
git checkout -b release/v0.16.0 develop

# Fix bugs only, then finish
git checkout main
git merge --no-ff release/v0.16.0
git tag v0.16.0
git push origin main --tags

# Merge back to develop
git checkout develop
git merge --no-ff release/v0.16.0
```

### Hotfix Process
```bash
# Critical fix on production
git checkout -b hotfix/v0.16.1 main

# Fix and test
git commit -m "fix: Critical bug description"

# Merge to main
git checkout main
git merge --no-ff hotfix/v0.16.1
git tag v0.16.1

# Merge to develop
git checkout develop
git merge --no-ff hotfix/v0.16.1
```

## ✅ Benefits of This Strategy

1. **Clear separation** of stable/development code
2. **Easy rollback** to any stable version
3. **Parallel development** without conflicts
4. **Professional workflow** matching industry standards
5. **Follows GOOD_PRACTICE.md** atomic principles
6. **Observer Self compliance** - always know where you are
7. **Clean history** with meaningful commits
8. **Traceable features** through branch names

## 📅 Version History Reference

### v0.12.x Series
- Original baseline with water-tight mesh (72 triangles)
- Debugging era with various CDT experiments

### v0.13.x Series
- Intermediate improvements
- Testing framework additions

### v0.14.x Series
- v0.14.0: Major refactoring
- v0.14.1: Modern GUI and tools
- v0.14.2: Pipeline fix - water-tight mesh restored

### v0.15.x Series (Current Development)
- v0.15.0: GUI framework established
- v0.15.1-6: Incremental viewer improvements
- To be consolidated into v0.16.0 stable

### v0.16.0 (Upcoming Stable)
- Complete GUI with 3D viewer
- Quaternion rotation system
- Perspective/orthographic projection
- Camera-local controls
- Geometry-centered rotation
- Coordinate axes with labels

---

*This strategy ensures professional Git management while maintaining the AdvCAD project's focus on geometric accuracy and algorithmic correctness.*