# GM3D Editor Manual

**AdvCAD .gm3d File Editor with 3D Preview**  
Version: 0.15.0  
A modern PyQt5-based editor for AdvCAD geometry files with real-time 3D mesh visualization.

## Table of Contents

1. [Overview](#overview)
2. [Installation & Requirements](#installation--requirements)
3. [Getting Started](#getting-started)
4. [User Interface](#user-interface)
5. [File Operations](#file-operations)
6. [Autocomplete System](#autocomplete-system)
7. [Primitive Shape Tools](#primitive-shape-tools)
8. [3D Visualization](#3d-visualization)
9. [CSG Tree Visualization](#csg-tree-visualization)
10. [Mesh Generation](#mesh-generation)
11. [Settings & Configuration](#settings--configuration)
12. [Keyboard Shortcuts](#keyboard-shortcuts)
13. [GM3D Command Reference](#gm3d-command-reference)
14. [Troubleshooting](#troubleshooting)

---

## Overview

The GM3D Editor is a comprehensive development environment for creating and editing AdvCAD geometry files (.gm3d). It provides:

- **Intelligent text editing** with syntax highlighting and autocomplete
- **Real-time 3D mesh preview** with OpenGL visualization
- **CSG operation tree visualization** for understanding model structure
- **Primitive shape creation tools** for rapid prototyping
- **Integrated mesh generation** with configurable timeout and mesh size
- **Modular architecture** for maintainability and extensibility

### Key Features

- ✅ **Syntax highlighting** for .gm3d commands
- ✅ **Position-aware autocomplete** with parameter guidance
- ✅ **Interactive 3D mesh preview** with wireframe/solid modes
- ✅ **Primitive shape dialogs** (box, sphere, cylinder, torus)
- ✅ **CSG tree visualization** with click-to-navigate
- ✅ **Configurable timeout and mesh size settings**
- ✅ **Undo/redo functionality** with keyboard shortcuts
- ✅ **Coordinate axes display** with orientation feedback
- ✅ **Timeout recovery dialog** with automatic mesh size adjustment

---

## Installation & Requirements

### System Requirements

- **Python 3.6+**
- **PyQt5** for GUI framework
- **PyOpenGL** for 3D visualization (optional, fallback available)
- **AdvCAD executable** built and available at expected path

### Required Python Packages

```bash
pip install PyQt5 PyOpenGL PyOpenGL_accelerate
```

### File Structure

```
apps/
├── gm3d_editor.py          # Main editor application
├── gm3d_dialogs.py         # Primitive shape creation dialogs
├── gm3d_parsers.py         # CSG and PCH file parsing
├── gm3d_viewer.py          # 3D OpenGL mesh viewer
├── gm3d_syntax.py          # Syntax highlighting
└── gm3d_autocomplete.py    # Intelligent autocomplete system
```

### Running the Editor

```bash
# From project root directory
cd /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/
python3 apps/gm3d_editor.py
```

---

## Getting Started

### First Launch

1. **Start the editor**: Run `python3 apps/gm3d_editor.py`
2. **Sample file loads automatically**: `sample/block.gm3d` if available
3. **Explore the interface**: Three-panel layout with tree, editor, and 3D preview
4. **Generate a mesh**: Click "Generate Mesh" to see 3D visualization

### Creating Your First Model

1. **Start a new file**: `Ctrl+N` or File → New
2. **Use autocomplete**: Type `box` and press Tab to see parameter suggestions
3. **Complete the command**: `box 0 0 0 2 2 2` creates a 2×2×2 cube
4. **Generate mesh**: Click "Generate Mesh" or use toolbar button
5. **View in 3D**: Mesh appears in right panel with interactive controls

---

## User Interface

### Three-Panel Layout

```
┌─────────────┬──────────────────┬─────────────────┐
│             │                  │                 │
│ CSG Tree    │   Text Editor    │  3D Preview     │
│ View        │   with           │  with           │
│             │   Autocomplete   │  Controls       │
│             │                  │                 │
└─────────────┴──────────────────┴─────────────────┘
```

#### Left Panel: CSG Tree View
- **Hierarchical visualization** of model operations
- **Click to navigate**: Click any operation to jump to corresponding line
- **Real-time updates** as you edit code
- **Operation types**: Primitives, operations, boolean operations

#### Center Panel: Text Editor
- **Syntax highlighting** for .gm3d commands
- **Intelligent autocomplete** with parameter guidance
- **Undo/redo support** with Ctrl+Z/Ctrl+Y
- **Cursor position tracking** synchronized with CSG tree

#### Right Panel: 3D Mesh Preview
- **Real-time visualization** of generated meshes
- **Interactive controls**: rotate, pan, zoom
- **Wireframe/solid toggle** for different viewing modes
- **Coordinate axes display** showing current orientation

### Menu System

#### File Menu
- **New** (Ctrl+N): Create new .gm3d file
- **Open** (Ctrl+O): Load existing .gm3d file
- **Save** (Ctrl+S): Save current file
- **Save As** (Ctrl+Shift+S): Save with new name
- **Exit** (Ctrl+Q): Close application

#### Edit Menu
- **Undo** (Ctrl+Z): Undo last edit
- **Redo** (Ctrl+Y): Redo last undone edit

#### View Menu
- **Toggle Wireframe** (W): Switch between wireframe and solid view
- **Reset 3D View** (R): Reset camera to default position

#### Settings Menu
- **Configure Time Limit**: Set mesh generation timeout (1-600 seconds)
- **Default Mesh Size**: Set default mesh resolution (0.001-100.0)

#### Help Menu
- **Autocomplete Help**: Show detailed autocomplete usage guide

### Toolbar

#### Main Toolbar
- **Generate Mesh**: Run AdvCAD mesh generation on current file

#### Primitive Shapes Toolbar
- **Cylinder**: Create cylinder primitive dialog
- **Sphere**: Create sphere primitive dialog  
- **Box**: Create box primitive dialog
- **Torus**: Create torus primitive dialog

---

## File Operations

### Supported File Formats

- **.gm3d files**: AdvCAD geometry definition files (primary format)
- **.pch files**: Generated mesh files (output from AdvCAD)

### File Management

#### Creating New Files
- **Menu**: File → New
- **Shortcut**: Ctrl+N
- **Result**: Clear editor with new untitled file

#### Opening Files
- **Menu**: File → Open
- **Shortcut**: Ctrl+O
- **Filters**: Shows .gm3d files by default, "All Files" available
- **Auto-load**: Updates CSG tree and 3D preview if mesh exists

#### Saving Files
- **Save**: Ctrl+S (saves to current file)
- **Save As**: Ctrl+Shift+S (prompts for new filename)
- **Auto-save**: Required before mesh generation

### File Integration

- **CSG tree updates** automatically when file content changes
- **3D preview refreshes** when new mesh is generated
- **Syntax highlighting** applies immediately to opened files

---

## Autocomplete System

### Overview

The GM3D Editor features an **intelligent, position-aware autocomplete system** that understands the structure of each GM3D command and provides contextual suggestions.

### Command Structure Registration

Each command is registered with its complete parameter structure:

```python
'box': 'box x y z width height depth - Create rectangular box'
'circle': 'circle cx cy cz rx ry rz nx ny nz divisions'
'extrude': 'extrude dx dy dz - Extrude 2D shape into 3D'
'revolve': 'revolve cx cy cz axis_x axis_y axis_z angle - Revolve around axis'
```

### Position-Aware Parameter Completion

The system knows exactly which parameter you're typing based on cursor position:

#### For `circle` Command:
- **Positions 1-3**: Center coordinates (cx, cy, cz)  
  → Suggests: `['0.0', '1.0', '-1.0', '2.0', '5.0']`
- **Positions 4-6**: Radius vector (rx, ry, rz) - defines radius magnitude and direction  
  → Suggests: `['1.0 0.0 0.0', '0.0 1.0 0.0', '10.0 0.0 0.0', '5.0 0.0 0.0']`
- **Positions 7-9**: Normal vector (nx, ny, nz) - defines plane orientation  
  → Suggests: `['0.0 0.0 1.0', '1.0 0.0 0.0', '0.0 1.0 0.0']` (XY, YZ, XZ planes)
- **Position 10**: Divisions  
  → Suggests: `['16', '32', '64']`

#### For `box` Command:
- **Positions 1-6**: All parameters (x, y, z, width, height, depth)  
  → Suggests: `['1.0', '2.0', '0.5', '10.0']`

#### For `revolve` Command:
- **Positions 1-3**: Center point (cx, cy, cz)  
  → Suggests: `['0.0']`
- **Positions 4-6**: Rotation axis (axis_x, axis_y, axis_z)  
  → Suggests: coordinate patterns
- **Position 7**: Angle  
  → Suggests: `['360', '180', '90', '270']`

### Usage Instructions

#### Triggering Autocomplete
- **Auto-trigger**: Completions appear automatically as you type
- **Manual trigger**: Ctrl+Space to force show completions
- **Context-sensitive**: Suggestions change based on command and position

#### Interacting with Completions
- **Navigation**: Arrow keys to move through suggestions
- **Accept**: Tab or Enter to accept selected completion
- **Dismiss**: Escape to close completion popup
- **Filtering**: Continue typing to filter suggestions

#### Example Workflow
1. Type `circle` → Command completion appears
2. Press Tab → Command inserted, space added automatically
3. Type `0` → Center coordinate suggestions: `['0.0', '1.0', '-1.0', '2.0', '5.0']`
4. Select `0.0` and continue with center coordinates
5. At positions 4-6 → Radius vector suggestions: `['1.0 0.0 0.0', '0.0 1.0 0.0', '10.0 0.0 0.0']`
6. At positions 7-9 → Normal vector suggestions: `['0.0 0.0 1.0', '1.0 0.0 0.0', '0.0 1.0 0.0']`
7. At position 10 → Division suggestions: `['16', '32', '64']`

### Predefined Data Patterns

#### Coordinate Patterns
```
'0.0 0.0 0.0'    # Origin
'1.0 0.0 0.0'    # X-axis unit vector
'0.0 1.0 0.0'    # Y-axis unit vector
'0.0 0.0 1.0'    # Z-axis unit vector
'-1.0 0.0 0.0'   # Negative X-axis
'0.0 -1.0 0.0'   # Negative Y-axis
'0.0 0.0 -1.0'   # Negative Z-axis
```

#### Radius Vectors (for circle command)
```
'1.0 0.0 0.0'    # Unit radius in X direction
'0.0 1.0 0.0'    # Unit radius in Y direction
'10.0 0.0 0.0'   # 10-unit radius in X direction
'0.0 10.0 0.0'   # 10-unit radius in Y direction
'5.0 0.0 0.0'    # 5-unit radius in X direction
'0.0 5.0 0.0'    # 5-unit radius in Y direction
```

#### Normal Vectors (for circle command - defines plane)
```
'0.0 0.0 1.0'    # XY plane (most common)
'1.0 0.0 0.0'    # YZ plane
'0.0 1.0 0.0'    # XZ plane
'0.0 0.0 -1.0'   # XY plane (inverted)
'-1.0 0.0 0.0'   # YZ plane (inverted)
'0.0 -1.0 0.0'   # XZ plane (inverted)
```

#### Common Values
```
'360'  # Full rotation
'180'  # Half rotation
'90'   # Quarter rotation
'2.0'  # Common mesh size
'1.0'  # Unit value
'0.5'  # Half unit
'32'   # Standard circle divisions
'16'   # Low-resolution circles
'64'   # High-resolution circles
```

---

## Primitive Shape Tools

### Overview

The GM3D Editor provides interactive dialogs for creating common geometric primitives with proper parameter validation and code generation.

### Available Primitives

#### Box Primitive
**Purpose**: Create rectangular boxes/cubes

**Parameters**:
- **Width (X)**: Size along X-axis
- **Depth (Y)**: Size along Y-axis  
- **Height (Z)**: Size along Z-axis
- **Center Type**: Bottom center or geometric center
- **Center Coordinates**: X, Y, Z position

**Generated Code Example**:
```
# Box primitive (2.0×2.0×2.0)
box 0.0 0.0 0.0 2.0 2.0 2.0
```

#### Sphere Primitive
**Purpose**: Create spheres using revolution

**Parameters**:
- **Radius**: Sphere radius
- **Center Coordinates**: X, Y, Z position

**Generated Code Example**:
```
# Sphere primitive (r=1.0)
circle 0.0 0.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0 32
revolve 0.0 0.0 0.0 0 0 1 360
```

#### Cylinder Primitive
**Purpose**: Create cylinders using extrusion

**Parameters**:
- **Radius**: Cylinder radius
- **Height**: Cylinder height
- **Orientation**: Z-axis (vertical), X-axis, or Y-axis
- **Center Type**: Bottom center or geometric center
- **Center Coordinates**: X, Y, Z position

**Generated Code Example**:
```
# Cylinder primitive (r=1.0, h=2.0, axis=Z)
circle 0.0 0.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0 32
extrude 0 0 2.0
```

#### Torus Primitive
**Purpose**: Create torus shapes using revolution

**Parameters**:
- **Major Radius**: Distance from center to tube center
- **Minor Radius**: Tube radius
- **Orientation**: Z-axis (horizontal), X-axis, or Y-axis
- **Center Coordinates**: X, Y, Z position

**Generated Code Example**:
```
# Torus primitive (major_r=2.0, minor_r=0.5)
circle 2.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 16
revolve 0.0 0.0 0.0 0 0 1 360
```

### Using Primitive Tools

#### Accessing Primitive Dialogs
- **Toolbar buttons**: Click respective primitive button
- **Result**: Dialog opens with parameter controls

#### Dialog Controls
- **Numeric inputs**: Spin boxes with validation and range limits
- **Dropdown selections**: Orientation and center type options
- **Coordinate inputs**: X, Y, Z position controls
- **Preview**: Live parameter validation

#### Code Generation
- **OK button**: Generates appropriate .gm3d code
- **Insertion**: Code inserted at current cursor position
- **Formatting**: Proper indentation and comments added
- **Tree update**: CSG tree refreshes automatically

### CAD Conventions

#### Coordinate System
- **X-axis**: Width (Red in 3D view)
- **Y-axis**: Depth (Green in 3D view)
- **Z-axis**: Height (Blue in 3D view)

#### Center Types
- **Bottom Center**: Object sits on XY plane, center at bottom
- **Geometric Center**: Object center at specified coordinates

#### Orientation Options
- **Z-axis**: Vertical orientation (cylinders stand up)
- **X-axis**: Horizontal along X-axis
- **Y-axis**: Horizontal along Y-axis

---

## 3D Visualization

### Overview

The 3D viewer provides real-time visualization of generated meshes using OpenGL with interactive navigation controls.

### Viewing Modes

#### Wireframe Mode
- **Activation**: Press 'W' or View → Toggle Wireframe
- **Appearance**: Green lines showing triangle edges
- **Use case**: Understanding mesh topology

#### Solid Mode
- **Activation**: Press 'S' or click wireframe toggle
- **Appearance**: Light blue filled surfaces with lighting
- **Use case**: Visualizing final appearance

### Navigation Controls

#### Mouse Controls
- **Left Click + Drag**: Rotate view around model
- **Right Click + Drag**: Pan/translate view
- **Mouse Wheel**: Zoom in/out (if supported)

#### Keyboard Controls
- **W**: Toggle wireframe mode
- **S**: Toggle solid mode
- **R**: Reset view to default position
- **F**: Fit model to view (auto-zoom and center)
- **+**: Zoom in
- **-**: Zoom out

#### View Reset
- **Function**: Returns camera to default position
- **Default angles**: X-rotation: 0°, Y-rotation: 0°
- **Default zoom**: 1.0
- **Default pan**: (0, 0)

#### Fit to View
- **Function**: Automatically centers and scales model
- **Calculation**: Based on model bounding box
- **Optimal viewing**: Sets appropriate zoom for model size
- **Angles**: Slight tilt for better perspective

### Coordinate Axes

#### Display
- **Location**: Fixed position in lower-left area
- **Colors**: X=Red, Y=Green, Z=Blue (standard CAD colors)
- **Behavior**: Rotates with model to show current orientation
- **Size**: Scaled appropriately for viewport

#### Purpose
- **Orientation feedback**: Shows current view angle
- **CAD standard**: Follows right-hand coordinate system
- **Reference**: Always visible for spatial orientation

### Mesh Loading

#### Automatic Loading
- **Trigger**: Successful mesh generation
- **Source**: .pch files generated by AdvCAD
- **Display**: Immediate update in 3D viewer
- **Statistics**: Vertex and triangle count reported

#### Manual Loading
- **File requirement**: Generate mesh first using toolbar button
- **Mesh files**: .pch format with vertex and face data
- **Error handling**: Warning if file cannot be loaded

### Performance

#### Optimization
- **Culling**: Back-face culling in solid mode
- **Lighting**: Efficient OpenGL lighting in solid mode
- **Rendering**: Optimized triangle rendering

#### Compatibility
- **Legacy OpenGL**: QGLWidget support for older systems
- **Modern OpenGL**: QOpenGLWidget for newer systems
- **Software fallback**: CPU rendering in WSL2/headless environments
- **Graceful degradation**: Placeholder when OpenGL unavailable

---

## CSG Tree Visualization

### Overview

The CSG (Constructive Solid Geometry) Tree View provides a hierarchical visualization of your model's construction operations.

### Tree Structure

#### Node Types
- **Primitives**: Basic shapes (box, circle, sheet)
- **Operations**: Transformations (extrude, revolve)
- **Boolean Operations**: Combinations (add, subtract, intersect)

#### Display Information
- **Command**: Operation type (BOX, CIRCLE, EXTRUDE, etc.)
- **Parameters**: Key parameter summary
- **Type**: Operation category (primitive, operation, boolean)

### Interactive Features

#### Navigation
- **Click to jump**: Click any tree item to navigate to corresponding line
- **Automatic highlighting**: Current cursor line highlighted in tree
- **Real-time sync**: Tree updates as you edit code

#### Tree Management
- **Auto-expand**: All items expanded by default
- **Real-time updates**: Refreshes on every text change
- **Error indication**: Invalid operations shown with error messages

### Node Details

#### Primitive Nodes
```
BOX      | [0,0,0] × [2,2,2]        | primitive
CIRCLE   | center[0,0,0]            | primitive
SHEET    | 4 vertices: [0,0,0]...   | primitive
```

#### Operation Nodes
```
EXTRUDE  | [0,0,2]                  | operation
REVOLVE  | around axis              | operation
```

#### Boolean Nodes
```
ADD      | ∪ Union                  | boolean
SUBTRACT | − Difference             | boolean
```

### Parsing Logic

#### Stack-Based Processing
- **Order**: Operations processed in sequence
- **Stack simulation**: Mimics AdvCAD's processing order
- **Hierarchical construction**: Shows operation relationships

#### Error Detection
- **Insufficient operands**: Boolean operations without enough inputs
- **Invalid syntax**: Malformed commands highlighted
- **Parameter validation**: Missing or invalid parameters noted

---

## Mesh Generation

### Overview

Integrated mesh generation using the AdvCAD backend with configurable settings and error recovery.

### Prerequisites

#### AdvCAD Executable
- **Required path**: `/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad`
- **Build status**: Must be compiled and available
- **Error handling**: Clear error message if not found

#### File Requirements
- **Saved file**: Must save .gm3d file before mesh generation
- **Auto-save**: Prompts to save if file modified
- **Valid syntax**: GM3D commands must be syntactically correct

### Generation Process

#### Command Execution
```bash
advcad input.gm3d output.pch mesh_size
```

#### Parameters
- **Input**: Current .gm3d file
- **Output**: `filename_output.pch` in same directory
- **Mesh size**: Configurable resolution parameter

#### Progress Indication
- **Progress dialog**: Shows generation status with timeout info
- **Cancellation**: Cancel button available (may not interrupt AdvCAD)
- **Timeout display**: Shows current timeout limit

### Settings Configuration

#### Time Limit Setting
- **Range**: 1-600 seconds (1 second to 10 minutes)
- **Default**: 60 seconds
- **Access**: Settings → Configure Time Limit
- **Purpose**: Prevents infinite hangs on complex geometry

#### Default Mesh Size
- **Range**: 0.001-100.0
- **Default**: 2.0
- **Decimals**: 3 decimal places supported
- **Access**: Settings → Default Mesh Size
- **Purpose**: Controls mesh resolution vs. generation time

### Timeout Recovery

#### Timeout Dialog
When mesh generation exceeds time limit, an intelligent recovery dialog appears:

**Features**:
- **Problem explanation**: Clear description of timeout issue
- **Suggested solutions**: Automatic recommendations
- **New mesh size**: Suggests doubling current mesh size
- **New time limit**: Suggests doubling timeout (max 10 minutes)
- **Retry option**: Apply new settings temporarily and retry

**Example**:
```
Mesh generation timed out after 60 seconds.

This often happens when the mesh size is too small for complex geometry.
Try increasing the mesh size or extending the time limit.

New Mesh Size: 4.0 (suggested: doubled from 2.0)
New Time Limit: 120 seconds (suggested: doubled from 60)

[Retry with New Settings] [Cancel]
```

#### Settings Behavior
- **Temporary application**: New settings used only for retry
- **Original restoration**: Previous settings restored after retry
- **Persistent changes**: Use Settings menu for permanent changes

### Success Handling

#### Mesh Loading
- **Automatic**: Generated mesh loaded immediately into 3D viewer
- **Statistics**: Vertex and triangle count reported
- **Success dialog**: Shows output file path and AdvCAD messages

#### Output Files
- **Location**: Same directory as .gm3d file
- **Naming**: `originalname_output.pch`
- **Format**: AdvCAD .pch triangular mesh format

### Error Handling

#### Common Errors
- **AdvCAD not found**: Clear path and build instructions
- **Syntax errors**: AdvCAD error output displayed
- **File permissions**: Write permission issues reported
- **Invalid geometry**: Geometric errors from AdvCAD shown

#### Error Display
- **Return codes**: Non-zero exit codes handled
- **Error output**: AdvCAD stderr displayed to user
- **Truncation**: Long error messages truncated to 500 characters

---

## Settings & Configuration

### Overview

Configurable settings for mesh generation, timeouts, and editor behavior.

### Available Settings

#### Time Limit Configuration
- **Purpose**: Set maximum time for mesh generation
- **Access**: Settings → Configure Time Limit
- **Range**: 1-600 seconds
- **Default**: 60 seconds
- **Units**: Seconds with suffix display

#### Default Mesh Size
- **Purpose**: Set default mesh resolution parameter
- **Access**: Settings → Default Mesh Size  
- **Range**: 0.001-100.0
- **Default**: 2.0
- **Precision**: 3 decimal places
- **Step size**: 0.1 increments

### Settings Dialogs

#### Time Limit Dialog
```
┌─────────────────────────────────┐
│ Configure Time Limit            │
├─────────────────────────────────┤
│ Time Limit: [60] seconds ▼      │
│                                 │
│           [OK]    [Cancel]      │
└─────────────────────────────────┘
```

#### Mesh Size Dialog
```
┌─────────────────────────────────┐
│ Configure Default Mesh Size     │
├─────────────────────────────────┤
│ Default Mesh Size: [2.000] ▼    │
│                                 │
│           [OK]    [Cancel]      │
└─────────────────────────────────┘
```

### Settings Persistence

#### Storage
- **Session-based**: Settings maintained during application session
- **Default restoration**: Revert to defaults on application restart
- **Future enhancement**: File-based persistence planned

#### Application
- **Immediate effect**: Changes applied immediately
- **Confirmation dialogs**: Success messages show new values
- **Mesh generation**: New settings used for subsequent generations

### Advanced Configuration

#### Editor Preferences
- **Font**: Consolas 12pt (hardcoded)
- **Syntax highlighting**: Enabled by default
- **Autocomplete**: Always enabled
- **Undo levels**: Default Qt limits

#### 3D Viewer Settings
- **Background color**: Dark gray (0.2, 0.2, 0.2)
- **Wireframe color**: Green
- **Solid color**: Light blue (0.7, 0.7, 0.9)
- **Lighting**: Standard OpenGL lighting

---

## Keyboard Shortcuts

### File Operations
| Shortcut | Function | Menu Location |
|----------|----------|---------------|
| Ctrl+N | New file | File → New |
| Ctrl+O | Open file | File → Open |
| Ctrl+S | Save file | File → Save |
| Ctrl+Shift+S | Save As | File → Save As |
| Ctrl+Q | Exit application | File → Exit |

### Edit Operations
| Shortcut | Function | Menu Location |
|----------|----------|---------------|
| Ctrl+Z | Undo | Edit → Undo |
| Ctrl+Y | Redo | Edit → Redo |
| Ctrl+Space | Force show autocomplete | - |

### View Operations
| Shortcut | Function | Menu Location |
|----------|----------|---------------|
| W | Toggle wireframe | View → Toggle Wireframe |
| R | Reset 3D view | View → Reset 3D View |

### 3D Navigation
| Shortcut | Function | Context |
|----------|----------|---------|
| Left Click + Drag | Rotate view | 3D viewer |
| Right Click + Drag | Pan view | 3D viewer |
| Mouse Wheel | Zoom | 3D viewer |
| S | Solid mode | 3D viewer |
| F | Fit to view | 3D viewer |
| + | Zoom in | 3D viewer |
| - | Zoom out | 3D viewer |

### Autocomplete Controls
| Shortcut | Function | Context |
|----------|----------|---------|
| Tab | Accept completion | Completion popup |
| Enter | Accept completion | Completion popup |
| Escape | Close completion | Completion popup |
| Arrow Keys | Navigate completions | Completion popup |

---

## GM3D Command Reference

### Basic Primitives

#### box
**Syntax**: `box x y z width height depth`  
**Purpose**: Create rectangular box  
**Parameters**:
- `x, y, z`: Bottom-left corner coordinates
- `width, height, depth`: Box dimensions

**Example**:
```
box 0 0 0 2 2 2    # 2×2×2 cube at origin
```

#### sheet
**Syntax**: `sheet num_points x1 y1 z1 x2 y2 z2 ...`  
**Purpose**: Create planar sheet from vertices  
**Parameters**:
- `num_points`: Number of vertices
- `x1 y1 z1, ...`: Vertex coordinates

**Example**:
```
sheet 4 0 0 0 1 0 0 1 1 0 0 1 0    # Square sheet
```

#### circle
**Syntax**: `circle cx cy cz rx ry rz nx ny nz divisions`  
**Purpose**: Create circular sheet  
**Parameters**:
- `cx, cy, cz`: Center coordinates
- `rx, ry, rz`: Radius vector (defines radius magnitude and direction)
- `nx, ny, nz`: Normal vector (defines plane orientation)
- `divisions`: Number of segments

**Examples**:
```
circle 0 0 0    10.0 0.0 0.0    0.0 0.0 1.0    16    # Circle in XY plane, radius 10 in X direction
circle 2 0 0    0.0 1.0 0.0     0.0 0.0 1.0    32    # Circle at (2,0,0) in XY plane, radius 1 in Y direction
```

### Operations

#### extrude
**Syntax**: `extrude dx dy dz`  
**Purpose**: Extrude 2D shape into 3D  
**Parameters**:
- `dx, dy, dz`: Extrusion vector

**Example**:
```
circle 0 0 0 1.0 0.0 0.0 0.0 0.0 1.0 32
extrude 0 0 2    # Create cylinder
```

#### revolve
**Syntax**: `revolve cx cy cz axis_x axis_y axis_z angle`  
**Purpose**: Revolve shape around axis  
**Parameters**:
- `cx, cy, cz`: Revolution center
- `axis_x, axis_y, axis_z`: Revolution axis
- `angle`: Revolution angle in degrees

**Example**:
```
circle 1 0 0 0.5 0.0 0.0 0.0 0.0 1.0 16
revolve 0 0 0 0 0 1 360    # Create torus
```

### Boolean Operations

#### add
**Syntax**: `add`  
**Purpose**: Boolean union of two objects  
**Operation**: Combines two objects on stack

**Example**:
```
box 0 0 0 2 2 1
box 0 0 1 2 2 1
add    # Stack two boxes
```

#### subtract
**Syntax**: `subtract`  
**Purpose**: Boolean difference of two objects  
**Operation**: Subtracts second object from first

**Example**:
```
box 0 0 0 2 2 2
box 0.5 0.5 0.5 1 1 1
subtract    # Create hollow box
```

#### intersect
**Syntax**: `intersect`  
**Purpose**: Boolean intersection of two objects  
**Operation**: Keeps only overlapping volume

### Command Guidelines

#### Stack-Based Processing
- **Order matters**: Commands processed in sequence
- **Stack operations**: Boolean operations use last two objects
- **Planning required**: Consider operation order carefully

#### Coordinate System
- **Right-handed**: Standard mathematical convention
- **Units**: Dimensionless (scale as needed)
- **Precision**: Floating-point numbers supported

#### Best Practices
- **Comments**: Use `#` for documentation
- **Formatting**: Consistent spacing for readability
- **Validation**: Use autocomplete to avoid syntax errors
- **Testing**: Generate mesh frequently to verify geometry

---

## Troubleshooting

### Common Issues

#### Installation Problems

**Issue**: `ModuleNotFoundError: No module named 'PyQt5'`  
**Solution**: Install PyQt5: `pip install PyQt5`

**Issue**: `ModuleNotFoundError: No module named 'OpenGL'`  
**Solution**: Install PyOpenGL: `pip install PyOpenGL PyOpenGL_accelerate`

**Issue**: "OpenGL not available - 3D preview disabled"  
**Solution**: Install OpenGL libraries or use software rendering

#### Runtime Errors

**Issue**: `AdvCAD executable not found`  
**Solution**: 
1. Build AdvCAD: `cmake --build build -j4`
2. Verify path: `/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad`

**Issue**: Mesh generation timeout  
**Solution**: 
1. Increase mesh size (larger values = faster generation)
2. Increase timeout limit in Settings
3. Use timeout recovery dialog suggestions

**Issue**: 3D viewer shows nothing after mesh generation  
**Solution**:
1. Check AdvCAD output for errors
2. Verify .pch file was created
3. Try simpler geometry first

#### Autocomplete Issues

**Issue**: Autocomplete not appearing  
**Solution**:
1. Press Ctrl+Space to force show
2. Ensure cursor is at command position
3. Check for syntax errors in current line

**Issue**: Wrong parameter suggestions  
**Solution**:
1. Verify command spelling
2. Check parameter position
3. Restart if problem persists

#### 3D Viewer Problems

**Issue**: Black/empty 3D viewer  
**Solution**:
1. Check OpenGL installation
2. Try software rendering (WSL2 users)
3. Update graphics drivers

**Issue**: Poor performance in 3D viewer  
**Solution**:
1. Use wireframe mode for large meshes
2. Reduce mesh resolution
3. Enable hardware acceleration

### Debugging Tips

#### Verbose Output
- **Console messages**: Check terminal for debug information
- **AdvCAD output**: Read mesh generation messages carefully
- **Error details**: Note specific error messages and line numbers

#### Systematic Approach
1. **Start simple**: Test with basic primitives first
2. **Build incrementally**: Add complexity gradually
3. **Verify each step**: Generate mesh after each major change
4. **Use CSG tree**: Verify operation sequence visually

#### File Management
- **Backup files**: Save working versions before major changes
- **Clear output**: Remove old .pch files to avoid confusion
- **Path verification**: Ensure all file paths are correct

### Performance Optimization

#### Mesh Generation
- **Appropriate mesh size**: Balance quality vs. generation time
- **Geometry simplification**: Avoid unnecessary complexity
- **Parameter tuning**: Use timeout recovery suggestions

#### Editor Performance
- **File size**: Large files may slow syntax highlighting
- **Real-time updates**: CSG tree updates on every change
- **Memory usage**: Restart application for very long sessions

### Getting Help

#### Resources
- **This manual**: Comprehensive feature documentation
- **Help menu**: Autocomplete usage guide
- **Sample files**: Examine provided examples
- **AdvCAD documentation**: Backend engine reference

#### Community Support
- **Issue reporting**: Report bugs with detailed reproduction steps
- **Feature requests**: Suggest improvements with use cases
- **Code contributions**: Modular architecture supports extensions

---

## Appendices

### A. File Format Specifications

#### .gm3d Format
- **Text-based**: Human-readable command sequences
- **Line-oriented**: One command per line
- **Comments**: Lines starting with `#`
- **Case-insensitive**: Commands not case-sensitive
- **Whitespace**: Space-separated parameters

#### .pch Format
- **Binary/text**: Triangular mesh data
- **Structure**: Vertex count, vertices, face count, faces
- **Coordinates**: Floating-point vertex positions
- **Indices**: Integer face vertex indices (1-based)

### B. Extension Points

#### Modular Architecture
- **gm3d_dialogs.py**: Add new primitive dialogs
- **gm3d_parsers.py**: Extend command parsing
- **gm3d_syntax.py**: Add syntax highlighting rules
- **gm3d_autocomplete.py**: Register new commands

#### Plugin System
- **Future enhancement**: Plugin architecture planned
- **Extension points**: Commands, dialogs, parsers, viewers
- **API considerations**: Stable interfaces for extensions

### C. Version History

#### v0.15.0 - Current
- ✅ Complete modular architecture
- ✅ Intelligent autocomplete system
- ✅ All primitive shape tools
- ✅ Settings and timeout management
- ✅ Comprehensive 3D visualization

#### Previous Versions
- **v0.14.2**: Water-tight mesh fixes
- **v0.14.1**: GUI enhancements
- **v0.12.x**: Core functionality development

---

**Document Version**: 1.0  
**Last Updated**: August 2025  
**Editor Version**: 0.15.0  
**Maintained by**: AdvCAD Development Team