"""
Parser classes for AdvCAD GM3D Editor
Handles CSG tree parsing and PCH file parsing
"""

import os

class CSGNode:
    """Node in the CSG tree representing an operation or primitive"""
    def __init__(self, command, line, params="", node_type="primitive"):
        self.command = command
        self.line = line
        self.params = params
        self.type = node_type
        self.children = []
        self.parent = None
    
    def add_child(self, child):
        """Add a child node"""
        self.children.append(child)
        child.parent = self

class CSGParser:
    """Parse .gm3d files to build hierarchical CSG tree"""
    
    def __init__(self):
        self.stack = []
        self.root = None
        self.all_nodes = []
    
    def parse(self, content):
        """Parse .gm3d content and build hierarchical tree"""
        self.stack = []
        self.all_nodes = []
        
        lines = content.strip().split('\n')
        for line_num, line in enumerate(lines, 1):
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            
            parts = line.split()
            if not parts:
                continue
            
            command = parts[0].lower()
            
            # Create nodes for different commands
            if command == 'box':
                if len(parts) >= 7:
                    params = f"[{parts[1]},{parts[2]},{parts[3]}] × [{parts[4]},{parts[5]},{parts[6]}]"
                    node = CSGNode('BOX', line_num, params, 'primitive')
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'sheet':
                if len(parts) >= 2:
                    num_points = parts[1]
                    # Show first vertex coordinates if available
                    if len(parts) >= 5:
                        first_vertex = f"[{parts[2]},{parts[3]},{parts[4]}]..."
                        params = f"{num_points} vertices: {first_vertex}"
                    else:
                        params = f"{num_points} vertices"
                    node = CSGNode('SHEET', line_num, params, 'primitive')
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'circle':
                params = 'circular sheet'
                if len(parts) >= 4:
                    params = f"center[{parts[1]},{parts[2]},{parts[3]}]"
                node = CSGNode('CIRCLE', line_num, params, 'primitive')
                self.stack.append(node)
                self.all_nodes.append(node)
            
            elif command == 'extrude':
                if len(parts) >= 4:
                    vector = f"[{parts[1]},{parts[2]},{parts[3]}]"
                    node = CSGNode('EXTRUDE', line_num, vector, 'operation')
                    # Extrude takes one object from stack
                    if self.stack:
                        child = self.stack.pop()
                        node.add_child(child)
                    self.stack.append(node)
                    self.all_nodes.append(node)
            
            elif command == 'revolve':
                node = CSGNode('REVOLVE', line_num, 'around axis', 'operation')
                # Revolve takes one object from stack
                if self.stack:
                    child = self.stack.pop()
                    node.add_child(child)
                self.stack.append(node)
                self.all_nodes.append(node)
            
            elif command == 'add':
                # Boolean union - takes two objects
                node = CSGNode('ADD', line_num, '∪ Union', 'boolean')
                if len(self.stack) >= 2:
                    # Pop two items and add as children
                    child2 = self.stack.pop()
                    child1 = self.stack.pop()
                    node.add_child(child1)
                    node.add_child(child2)
                    self.stack.append(node)
                else:
                    node.params = 'ERROR: insufficient operands'
                self.all_nodes.append(node)
            
            elif command == 'subtract':
                # Boolean difference - takes two objects
                node = CSGNode('SUBTRACT', line_num, '− Difference', 'boolean')
                if len(self.stack) >= 2:
                    # Pop two items and add as children
                    child2 = self.stack.pop()  # Subtracted object
                    child1 = self.stack.pop()  # Base object
                    node.add_child(child1)
                    node.add_child(child2)
                    self.stack.append(node)
                else:
                    node.params = 'ERROR: insufficient operands'
                self.all_nodes.append(node)
        
        # The final stack should have the root object(s)
        if self.stack:
            self.root = self.stack[-1]  # Last operation is usually the root

class PCHParser:
    """Parse .pch files for 3D mesh visualization"""
    
    def __init__(self):
        self.vertices = []
        self.faces = []
        
    def parse_pch_file(self, file_path):
        """Parse a .pch file and extract vertices and faces"""
        if not os.path.exists(file_path):
            return False
            
        self.vertices = []
        self.faces = []
        
        try:
            with open(file_path, 'r') as f:
                lines = f.readlines()
            
            if not lines:
                return False
            
            # First line is number of vertices
            num_vertices = int(lines[0].strip())
            
            # Read vertices
            for i in range(1, num_vertices + 1):
                if i < len(lines):
                    coords = lines[i].strip().split()
                    if len(coords) >= 3:
                        x, y, z = map(float, coords[:3])
                        self.vertices.append([x, y, z])
            
            # Find number of faces
            if num_vertices + 1 < len(lines):
                num_faces = int(lines[num_vertices + 1].strip())
                
                # Read faces
                for i in range(num_vertices + 2, num_vertices + 2 + num_faces):
                    if i < len(lines):
                        face_data = lines[i].strip().split()
                        if len(face_data) >= 3:
                            # Convert to 0-based indexing
                            face = [int(x) for x in face_data[:3]]
                            self.faces.append(face)
            
            return len(self.vertices) > 0 and len(self.faces) > 0
            
        except Exception as e:
            print(f"Error parsing PCH file: {e}")
            return False