#!/usr/bin/env python3
"""
Visualize Face 7 geometry and triangulation problem
"""
import matplotlib.pyplot as plt
import numpy as np
import subprocess
import re
import sys

def run_advcad_and_capture():
    """Run advcad and capture debug output"""
    try:
        result = subprocess.run(
            ['./advcad', '../sample/shaft/coil_01.gm3d', '../sample/shaft/coil_01.pch', '1'],
            cwd='/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/command',
            capture_output=True,
            text=True,
            timeout=30
        )
        return result.stderr + result.stdout
    except subprocess.TimeoutExpired:
        print("Process timed out")
        return ""
    except Exception as e:
        print(f"Error running advcad: {e}")
        return ""

def parse_face_debug_output(output):
    """Parse debug output to extract Face 7 information"""
    lines = output.split('\n')
    face7_data = {
        'nodes': [],
        'triangles': [],
        'boundary_segments': [],
        'error_triangle': None
    }
    
    in_face7 = False
    
    for line in lines:
        # Detect Face 7 processing
        if "Processing face 7/14" in line:
            in_face7 = True
            print("Found Face 7 processing...")
            continue
            
        # Stop at next face or end
        if in_face7 and ("Processing face 8/14" in line or "Face 7 failed" in line):
            break
            
        if in_face7:
            # Look for node positions in smoothing debug output
            if "pos0:" in line:
                match = re.search(r'pos0: \(([^,]+), ([^)]+)\)', line)
                if match:
                    x, y = float(match.group(1)), float(match.group(2))
                    face7_data['nodes'].append((x, y, 0))  # ID will be assigned later
                    
            if "pos1:" in line:
                match = re.search(r'pos1: \(([^,]+), ([^)]+)\)', line)
                if match:
                    x, y = float(match.group(1)), float(match.group(2))
                    face7_data['nodes'].append((x, y, 1))
                    
            if "pos2:" in line:
                match = re.search(r'pos2: \(([^,]+), ([^)]+)\)', line)
                if match:
                    x, y = float(match.group(1)), float(match.group(2))
                    face7_data['nodes'].append((x, y, 2))
        
        # Look for the error triangle
        if "triangle with negative IDs:" in line:
            match = re.search(r'\[([^,]+),([^,]+),([^]]+)\]', line)
            if match:
                id0, id1, id2 = match.group(1), match.group(2), match.group(3)
                face7_data['error_triangle'] = (id0.strip(), id1.strip(), id2.strip())
                print(f"Found error triangle: {face7_data['error_triangle']}")
    
    return face7_data

def create_test_geometry():
    """Create test geometry based on typical Face 7 patterns"""
    # Based on the coordinate ranges seen in debug output, create a representative geometry
    nodes = [
        (-0.075, 0.0, 0),      # ID 0
        (-0.075, 0.007, 1),    # ID 1  
        (-0.074, 0.013, 2),    # ID 2
        (-0.084, 0.015, 3),    # ID 3
        (-0.085, 0.007, 4),    # ID 4
        (-0.085, 0.0, 5),      # ID 5
        # Dummy point position (estimated)
        (-0.090, -0.005, -1),  # ID -1 (dummy)
    ]
    
    # Triangles that would be generated
    triangles = [
        (0, 1, 5),   # Normal triangle
        (1, 4, 5),   # Normal triangle  
        (1, 3, 4),   # Normal triangle
        (1, 2, 3),   # Normal triangle
        (0, -1, 5),  # Problematic triangle with dummy point
    ]
    
    return nodes, triangles

def visualize_face7_geometry(nodes, triangles, error_triangle=None):
    """Visualize the Face 7 geometry and triangulation"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 7))
    
    # Plot 1: Nodes and their IDs
    ax1.set_title("Face 7 Nodes and IDs", fontsize=14, fontweight='bold')
    
    for x, y, node_id in nodes:
        color = 'red' if node_id < 0 else 'blue'
        marker = 's' if node_id < 0 else 'o'
        ax1.plot(x, y, marker, color=color, markersize=10)
        ax1.annotate(f'ID: {node_id}', (x, y), xytext=(5, 5), 
                    textcoords='offset points', fontsize=10, fontweight='bold')
    
    ax1.grid(True, alpha=0.3)
    ax1.set_xlabel('X coordinate')
    ax1.set_ylabel('Y coordinate')
    ax1.legend(['Real nodes', 'Dummy nodes'] if any(id < 0 for _, _, id in nodes) else ['Nodes'])
    
    # Plot 2: Triangulation with problem triangle highlighted
    ax2.set_title("Face 7 Triangulation (Problem Triangle in Red)", fontsize=14, fontweight='bold')
    
    # Create node lookup
    node_dict = {node_id: (x, y) for x, y, node_id in nodes}
    
    for triangle in triangles:
        id0, id1, id2 = triangle
        
        # Skip triangles with missing nodes
        if id0 not in node_dict or id1 not in node_dict or id2 not in node_dict:
            continue
            
        x_coords = [node_dict[id0][0], node_dict[id1][0], node_dict[id2][0], node_dict[id0][0]]
        y_coords = [node_dict[id0][1], node_dict[id1][1], node_dict[id2][1], node_dict[id0][1]]
        
        # Highlight problematic triangle
        is_problem = (error_triangle and 
                     set(map(str, triangle)) == set(error_triangle))
        
        color = 'red' if is_problem or any(id < 0 for id in triangle) else 'blue'
        linewidth = 3 if is_problem else 1
        alpha = 0.7 if is_problem else 0.3
        
        ax2.plot(x_coords, y_coords, color=color, linewidth=linewidth, alpha=alpha)
        
        # Fill problematic triangles
        if is_problem or any(id < 0 for id in triangle):
            ax2.fill(x_coords, y_coords, color='red', alpha=0.2)
        
        # Add triangle center label
        center_x = sum(node_dict[id][0] for id in triangle if id in node_dict) / 3
        center_y = sum(node_dict[id][1] for id in triangle if id in node_dict) / 3
        ax2.annotate(f'[{id0},{id1},{id2}]', (center_x, center_y), 
                    ha='center', va='center', fontsize=8, 
                    bbox=dict(boxstyle="round,pad=0.3", facecolor='yellow', alpha=0.7))
    
    # Plot nodes on triangulation view
    for x, y, node_id in nodes:
        color = 'red' if node_id < 0 else 'blue'
        marker = 's' if node_id < 0 else 'o'
        ax2.plot(x, y, marker, color=color, markersize=8)
        ax2.annotate(f'{node_id}', (x, y), xytext=(3, 3), 
                    textcoords='offset points', fontsize=9, fontweight='bold')
    
    ax2.grid(True, alpha=0.3)
    ax2.set_xlabel('X coordinate')
    ax2.set_ylabel('Y coordinate')
    ax2.set_aspect('equal')
    
    plt.tight_layout()
    return fig

def analyze_geometry_issues(nodes, triangles, error_triangle):
    """Analyze potential geometry issues"""
    print("\n" + "="*60)
    print("GEOMETRY ANALYSIS")
    print("="*60)
    
    # Node analysis
    real_nodes = [(x, y, id) for x, y, id in nodes if id >= 0]
    dummy_nodes = [(x, y, id) for x, y, id in nodes if id < 0]
    
    print(f"Real nodes: {len(real_nodes)}")
    print(f"Dummy nodes: {len(dummy_nodes)}")
    
    # Triangle analysis
    normal_triangles = [t for t in triangles if all(id >= 0 for id in t)]
    mixed_triangles = [t for t in triangles if any(id < 0 for id in t) and any(id >= 0 for id in t)]
    dummy_triangles = [t for t in triangles if all(id < 0 for id in t)]
    
    print(f"\nTriangle Analysis:")
    print(f"Normal triangles: {len(normal_triangles)}")
    print(f"Mixed triangles (PROBLEM): {len(mixed_triangles)}")
    print(f"Pure dummy triangles: {len(dummy_triangles)}")
    
    if mixed_triangles:
        print(f"\nPROBLEMATIC MIXED TRIANGLES:")
        for tri in mixed_triangles:
            print(f"  Triangle {tri}: contains both real and dummy points")
    
    # Geometric analysis
    if len(real_nodes) > 0:
        x_coords = [x for x, y, id in real_nodes]
        y_coords = [y for x, y, id in real_nodes]
        
        print(f"\nGeometric Properties:")
        print(f"X range: [{min(x_coords):.6f}, {max(x_coords):.6f}] (span: {max(x_coords)-min(x_coords):.6f})")
        print(f"Y range: [{min(y_coords):.6f}, {max(y_coords):.6f}] (span: {max(y_coords)-min(y_coords):.6f})")
        
        # Check for degenerate cases
        if max(x_coords) - min(x_coords) < 1e-10:
            print("WARNING: Very small X span - possible degenerate geometry")
        if max(y_coords) - min(y_coords) < 1e-10:
            print("WARNING: Very small Y span - possible degenerate geometry")

def main():
    print("Analyzing Face 7 triangulation problem...")
    
    # Try to get real debug output first
    print("Running advcad to capture debug output...")
    output = run_advcad_and_capture()
    
    if output:
        face7_data = parse_face_debug_output(output)
        if face7_data['nodes']:
            print(f"Captured {len(face7_data['nodes'])} nodes from debug output")
            nodes = face7_data['nodes']
            triangles = []  # We'll use test triangles
            error_triangle = face7_data['error_triangle']
        else:
            print("No nodes found in debug output, using test geometry")
            nodes, triangles = create_test_geometry()
            error_triangle = ('0', '-1', '5')
    else:
        print("Using test geometry (couldn't capture debug output)")
        nodes, triangles = create_test_geometry()
        error_triangle = ('0', '-1', '5')
    
    # Remove duplicates and ensure we have the error triangle
    unique_nodes = []
    seen_positions = set()
    for x, y, node_id in nodes:
        pos_key = (round(x, 8), round(y, 8))  # Round to avoid floating point issues
        if pos_key not in seen_positions:
            unique_nodes.append((x, y, node_id))
            seen_positions.add(pos_key)
    
    # Add test triangles if we don't have any
    if not triangles:
        _, triangles = create_test_geometry()
    
    print(f"Visualizing {len(unique_nodes)} nodes and {len(triangles)} triangles")
    
    # Analyze issues
    analyze_geometry_issues(unique_nodes, triangles, error_triangle)
    
    # Create visualization
    fig = visualize_face7_geometry(unique_nodes, triangles, error_triangle)
    
    # Save the plot
    output_file = '/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/face7_analysis.png'
    fig.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\nVisualization saved to: {output_file}")
    
    # Don't show interactive plot, just save
    print("Plot saved successfully!")

if __name__ == "__main__":
    main()