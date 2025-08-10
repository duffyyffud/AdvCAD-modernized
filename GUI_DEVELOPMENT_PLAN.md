# GUI Development Plan for gm3d_editor.py

## Current Status (v0.14.2)
The GUI is fully functional with tree pane, text editor, and 3D visualization. All core features are implemented and working.

## Implemented Features ✅
- **Three-panel layout**: CSG tree, text editor, 3D mesh viewer
- **Syntax highlighting**: Commands (blue), numbers (green), comments (gray)
- **CSG tree parsing**: Real-time synchronization with editor content
- **3D visualization**: OpenGL-based mesh rendering with rotation/zoom
- **Auto-completion**: Command suggestions while typing
- **Mesh generation**: F5 hotkey with progress dialog
- **File operations**: Complete New/Open/Save workflow
- **Wireframe toggle**: Switch between solid and wireframe display

## Potential Enhancements

### 1. Primitive Shape Tools
- [ ] **Shape toolbar** - buttons for common primitives
- [ ] **Cylinder wizard** - dialog with radius, height, segments
- [ ] **Sphere wizard** - dialog with radius, subdivisions
- [ ] **Box wizard** - dialog with width, height, depth
- [ ] **Cone wizard** - dialog with base radius, top radius, height
- [ ] **Torus wizard** - dialog with major/minor radius
- [ ] **Insert at cursor** - add generated code at current position
- [ ] **Preview before insert** - show 3D preview of primitive
- [ ] **Parameter sliders** - visual adjustment of dimensions
- [ ] **Snap to grid** - round dimensions to nice values
- [ ] **Coordinate system helper** - show XYZ orientation

### 2. Editor Improvements
- [ ] **Undo/Redo stack** with Ctrl+Z/Ctrl+Y
- [ ] **Find/Replace** dialog (Ctrl+F/Ctrl+H)
- [ ] **Line numbers** in editor margin
- [ ] **Code folding** for nested CSG operations

### 3. Error Validation & Feedback
- [ ] **Real-time syntax validation** - check .gm3d syntax as user types
- [ ] **Error highlighting** - mark syntax errors with red squiggles
- [ ] **Parameter validation** - check numeric ranges and constraints
- [ ] **Stack validation warnings** - detect invalid CSG operation sequences
- [ ] **Error panel** - list all errors with line numbers
- [ ] **Quick fixes** - suggest corrections for common errors
- [ ] **Hover tooltips** - show error details on mouseover
- [ ] **Pre-mesh validation** - check geometry before running advcad

### 4. 3D Viewer Enhancements
- [ ] **Mesh statistics** display (vertices, triangles, volume)
- [ ] **Axis indicators** (X/Y/Z arrows)
- [ ] **Grid floor** for spatial reference
- [ ] **Measurement tools** - distance, angle, dimensions
- [ ] **Export mesh** to different formats (STL, OBJ)
- [ ] **Multiple view modes** - top, front, side, isometric
- [ ] **Lighting controls** - adjust ambient/directional lights

### 5. Tree View Features
- [ ] **Drag & drop** to reorder operations
- [ ] **Context menu** - duplicate, delete, disable operations
- [ ] **Operation preview** - highlight affected geometry
- [ ] **Parameter editing** directly in tree
- [ ] **Search/filter** tree nodes

### 6. Workflow Improvements
- [ ] **Template library** - common shapes and patterns
- [ ] **Recent files** menu
- [ ] **Auto-save** functionality
- [ ] **Session management** - save/restore window layout
- [ ] **Batch processing** - generate meshes for multiple files
- [ ] **Live preview** - update mesh as you type (with debouncing)

### 7. GenAI Chat Interface
- [ ] **Collapsible bottom panel** - show/hide with button or F1
- [ ] **Context-aware geometry help** - understand current .gm3d file
- [ ] **Natural language to CSG** - describe shape, get .gm3d code
- [ ] **Error explanation** - explain why mesh generation failed
- [ ] **Geometry suggestions** - recommend fixes for invalid operations
- [ ] **Parameter optimization** - suggest better mesh sizes
- [ ] **Japanese language support** - bilingual interface
- [ ] **Code generation** - create complex shapes from description
- [ ] **Tutorial mode** - step-by-step guidance for beginners
- [ ] **API integration** - support multiple LLM backends (OpenAI, Claude, local)

### 8. Advanced Features
- [ ] **CSG operation wizard** - guided shape creation
- [ ] **Parametric modeling** - variables and expressions
- [ ] **Animation timeline** - animate parameter changes
- [ ] **Version control integration** - show git diff
- [ ] **Plugin system** - extend functionality
- [ ] **Documentation browser** - integrated help

### 9. Quality of Life
- [ ] **Dark theme** option
- [ ] **Customizable keybindings**
- [ ] **Toolbar customization**
- [ ] **Status bar** with cursor position, mesh stats
- [ ] **Split view** - edit multiple files
- [ ] **Tab support** for multiple documents

## Implementation Priority

### Phase 1: Core Usability & Basic Shapes (High Priority)
1. Primitive shape tools (cylinder, sphere, box)
2. Error validation & highlighting
3. Undo/Redo stack
4. Line numbers
5. Mesh statistics display
6. Recent files menu

### Phase 2: Enhanced Visualization
1. Axis indicators
2. Grid floor
3. Multiple view modes
4. Export mesh formats
5. Lighting controls

### Phase 3: Advanced Editing
1. Template library
2. Live preview
3. Code folding
4. Context menus
5. Stack validation warnings

### Phase 4: AI-Powered Features
1. GenAI chat interface (bottom panel)
2. Natural language to CSG conversion
3. Context-aware geometry help
4. Error explanation with fixes
5. Japanese language support

### Phase 5: Professional Features
1. Parametric modeling
2. Plugin system
3. Animation timeline
4. Batch processing
5. Documentation browser

## Example: Primitive Generation

### Cylinder Generation
When user clicks "Cylinder" button and enters radius=1.0, height=2.0:
```gm3d
# Cylinder primitive
circle 0 0 0 1.0
extrude 0 0 2.0
```

### Sphere Generation  
When user clicks "Sphere" button and enters radius=1.5:
```gm3d
# Sphere primitive (using revolution)
sheet 0 -1.5 0 0 1.5 0
circle 0 0 0 1.5
revolve 0 0 0 0 0 1 360
```

### Box Generation
When user clicks "Box" button and enters width=2, height=3, depth=1:
```gm3d
# Box primitive
box 0 0 0 2 3 1
```

## Technical Considerations

### Dependencies
- Current: PyQt5, PyOpenGL, numpy
- Potential: trimesh (mesh processing), pygments (better syntax highlighting)

### Performance
- Implement mesh decimation for large models
- Use VBOs for OpenGL rendering optimization
- Debounce live preview updates
- Cache parsed CSG trees

### Compatibility
- Maintain backward compatibility with existing .gm3d files
- Support both legacy QGLWidget and modern QOpenGLWidget
- Test on Windows, Linux, macOS

## Testing Strategy
- Unit tests for CSG parser
- Integration tests for mesh generation
- UI automation tests with pytest-qt
- Performance benchmarks for large models
- User acceptance testing with sample workflows

## Documentation Needs
- User manual with screenshots
- Tutorial for common operations
- API documentation for plugin developers
- Keyboard shortcuts reference
- Troubleshooting guide

## Notes
This plan represents potential enhancements beyond the current fully-functional implementation. The GUI already provides a complete workflow for .gm3d editing and mesh visualization. Any enhancements should maintain the simplicity and reliability of the current design.