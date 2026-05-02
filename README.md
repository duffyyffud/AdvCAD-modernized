# AdvCAD (Adventure CAD)

A C++17 CAD library and command-line tool for 3D solid modeling and triangular mesh generation, built around polygon-based boolean operations and Constrained Delaunay Triangulation.

## Quick Start

```bash
# Build (CMake, recommended)
cd build && make -j4
# Binary: build/command/advcad

# Run on a sample
./build/command/advcad sample/block.gm3d /tmp/block.pch 2.0
```

The binary prints a recommended mesh-size range at start-up; out-of-range values are accepted but emit `WARNING: Mesh size N exceeds recommended maximum …`. See [Mesh size guidance](#mesh-size-guidance) below.

## Repository Layout

This repository follows a hub-and-spoke documentation pattern: this file is the entry point, and each subdirectory has its own README explaining local contents.

| Path | Purpose | Local README |
|---|---|---|
| `WH/` | Core C++ library (geometry, triangulation, robust CDT). ~125 files. | — |
| `command/` | `advcad` CLI source. | — |
| `build/` | CMake build tree. Binary at `build/command/advcad`. | — |
| `sample/` | `.gm3d` test geometries (`block`, `test_*`, `cake/`, `shaft/`). | [`sample/README`](sample/README) |
| `apps/` | End-user tools (mesh automation, GUI editor, optimizer). | [`apps/README.md`](apps/README.md) |
| `dev/` | Developer tools (regression suite, failure analyzer, validators). | [`dev/README.md`](dev/README.md) |
| `scripts/` | PCH I/O helpers (`PchReader.py`, `PchWriter.py`). | [`scripts/README.md`](scripts/README.md) |
| `tests/` | Test data and regression fixtures. | [`tests/README.md`](tests/README.md) |
| `docs/` | Manual, PCH format spec, codebase knowledge. | [`docs/README.md`](docs/README.md) |

## Build System

Both CMake (recommended) and the original Makefile are supported.

```bash
# CMake — debug build with extra diagnostics
cmake -DCMAKE_BUILD_TYPE=Debug -DWH_DEBUG_ENABLED=ON -B build
cmake --build build -j4

# CMake — release build
cmake -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build -j4

# Traditional Makefile
cd WH && make && cd ../command && make
```

Requirements: CMake ≥ 3.15, C++17 compiler (GCC 7+, Clang 5+).

## CLI Usage

```
advcad <geometry_file> <patch_file> <patch_size> [-pcm]
```

| Argument | Meaning |
|---|---|
| `geometry_file` | Input `.gm3d` file |
| `patch_file` | Output mesh (`.pch` or `.pcm`) |
| `patch_size` | Target edge length for mesh elements |
| `-pcm` | Output PCM instead of PCH |

`.gm3d` directives include `sheet`, `extrude`, `subtract`. See [`docs/manual.txt`](docs/manual.txt) and `sample/` for examples.

## Mesh Size Guidance

Mesh size selection is the most common source of failure. The algorithms are unit-agnostic, but `patch_size` must be sized to **geometric features**, not absolute coordinates.

| Model scale | Typical `patch_size` range |
|---|---|
| Millimeter parts (0–100 mm) | 1.0 – 10.0 |
| Meter scale (0–1 m) | 0.01 – 0.1 |
| Sub-mm features (≤ 0.02 m) | 0.001 – 0.004 |

Rule of thumb: start at 10–20 % of the smallest significant feature, then refine. Or let `apps/optimize_mesh_size.py` do it for you:

```bash
python3 apps/optimize_mesh_size.py sample/shaft/coil_01.gm3d optimized.pch
```

Common symptoms:
- **Segfault / assertion** → `patch_size` is too fine for the geometry.
- **Mixed-triangle error** → `patch_size` is too coarse for small features.
- **Empty output** → invalid input or wildly inappropriate `patch_size`.

## Testing

```bash
# Full regression run (19 sample models)
python3 dev/test_regression.py

# Failure-boundary analysis
python3 dev/mesh_failure_analyzer.py
```

**Status (verified on `claude/verify-repo-behavior-YcNzN`):** 19/19 models exit `rc=0` at the heuristic `mesh=1.0`, but `shaft/air_practice.gm3d` and `shaft/air_up2_top_01.gm3d` only succeed because that mesh size is far above the recommended max — outputs degenerate to ≤ 17 triangles. Effective coverage is **17/19**. See [`TODO.md`](TODO.md) for the remaining work.

## Modernization Notes

- C++17 exception types: `WH_Exception`, `WH_NullPointerException`, `WH_BoundsException`, `WH_InvalidArgumentException`.
- Robust CDT path (`WH/robust_cdt.cc`) with exact-arithmetic predicates and ear-clipping / monotone-partition fallbacks. Triggered for faces with ≥ 6 nodes/segments or coordinates < 1e-2.
- Protective fallbacks replace previous assertion crashes: zero-vector normalization, division-by-zero in 2D vector ops, near-collinear / degenerate triangle handling.

## Documentation

- [`CLAUDE.md`](CLAUDE.md) — Codebase guide for AI/agent sessions and contributors (architecture, debug strategies, current metrics).
- [`ROADMAP.md`](ROADMAP.md) — Project direction and phase plan (robustness first, GUI utilities second, modernization deprioritized).
- [`TODO.md`](TODO.md) — Aggregated outstanding tasks (single source of truth).
- [`CHANGELOG.md`](CHANGELOG.md) — Release history.
- [`GOOD_PRACTICE.md`](GOOD_PRACTICE.md) — Workflow and git discipline.
- [`docs/manual.txt`](docs/manual.txt) — Detailed manual.
- [`docs/pch_format.txt`](docs/pch_format.txt) — PCH file format spec.
- [`docs/CODEBASE_KNOWLEDGE.md`](docs/CODEBASE_KNOWLEDGE.md) — Detailed debugging knowledge.
- [`PLANS.md`](PLANS.md) — Consolidated implementation plans (CMake migration & PCH validation, degenerate triangle handling, failure boundary mapping, zero-vector fix).

## Exit Codes

| Code | Meaning |
|---|---|
| `0` | Success |
| `1` | General error (algorithm failure, invalid input) |
| `-11` (139) | Segmentation fault — usually `patch_size` mismatched with geometry scale |
