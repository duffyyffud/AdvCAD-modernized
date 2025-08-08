#!/usr/bin/env python3
"""
Geometry Validator for AdvCAD .gm3d files

Validates input geometry files for common issues that cause crashes:
- Duplicate vertices in sheet definitions
- Zero-length edges
- Degenerate polygons
"""

import sys
import os
import re
from typing import List, Tuple, Dict
import math

def parse_sheet_line(line: str) -> List[Tuple[float, float, float]]:
    """Parse a sheet definition line to extract vertices."""
    # Pattern: sheet N x1 y1 z1 x2 y2 z2 ...
    parts = line.strip().split()
    if not parts or parts[0] != 'sheet':
        return []
    
    try:
        num_vertices = int(parts[1])
        vertices = []
        
        # Extract vertex coordinates (3 floats per vertex)
        coord_start = 2
        for i in range(num_vertices):
            idx = coord_start + i * 3
            if idx + 2 < len(parts):
                x = float(parts[idx])
                y = float(parts[idx + 1])
                z = float(parts[idx + 2])
                vertices.append((x, y, z))
        
        return vertices
    except (ValueError, IndexError):
        return []

def distance_3d(v1: Tuple[float, float, float], v2: Tuple[float, float, float]) -> float:
    """Calculate 3D distance between two vertices."""
    return math.sqrt((v1[0] - v2[0])**2 + (v1[1] - v2[1])**2 + (v1[2] - v2[2])**2)

def validate_geometry_file(filepath: str) -> Dict[str, any]:
    """
    Validate a .gm3d geometry file.
    
    Returns dict with:
    - 'valid': bool
    - 'errors': list of error messages
    - 'warnings': list of warning messages
    - 'stats': dict of statistics
    """
    result = {
        'valid': True,
        'errors': [],
        'warnings': [],
        'stats': {
            'total_sheets': 0,
            'total_vertices': 0,
            'duplicate_vertices': 0,
            'zero_length_edges': 0,
            'min_edge_length': float('inf'),
            'degenerate_sheets': 0
        }
    }
    
    if not os.path.exists(filepath):
        result['valid'] = False
        result['errors'].append(f"File not found: {filepath}")
        return result
    
    try:
        with open(filepath, 'r') as f:
            lines = f.readlines()
    except Exception as e:
        result['valid'] = False
        result['errors'].append(f"Error reading file: {e}")
        return result
    
    # Analyze each line
    for line_num, line in enumerate(lines, 1):
        if line.strip().startswith('sheet'):
            result['stats']['total_sheets'] += 1
            vertices = parse_sheet_line(line)
            
            if not vertices:
                result['warnings'].append(f"Line {line_num}: Could not parse sheet definition")
                continue
            
            result['stats']['total_vertices'] += len(vertices)
            
            # Check for duplicate vertices
            unique_vertices = set()
            duplicates = []
            for i, v in enumerate(vertices):
                v_rounded = (round(v[0], 9), round(v[1], 9), round(v[2], 9))
                if v_rounded in unique_vertices:
                    duplicates.append((i, v))
                    result['stats']['duplicate_vertices'] += 1
                unique_vertices.add(v_rounded)
            
            if duplicates:
                result['valid'] = False
                dup_info = ', '.join([f"vertex {i+1} at {v}" for i, v in duplicates])
                result['errors'].append(f"Line {line_num}: Sheet has duplicate vertices: {dup_info}")
                result['stats']['degenerate_sheets'] += 1
            
            # Check for zero-length edges
            for i in range(len(vertices)):
                v1 = vertices[i]
                v2 = vertices[(i + 1) % len(vertices)]
                edge_length = distance_3d(v1, v2)
                
                if edge_length < 1e-9:
                    result['valid'] = False
                    result['errors'].append(
                        f"Line {line_num}: Zero-length edge between vertex {i+1} "
                        f"({v1}) and vertex {((i+1)%len(vertices))+1} ({v2})"
                    )
                    result['stats']['zero_length_edges'] += 1
                elif edge_length < 1e-6:
                    result['warnings'].append(
                        f"Line {line_num}: Very short edge ({edge_length:.2e}) between "
                        f"vertex {i+1} and vertex {((i+1)%len(vertices))+1}"
                    )
                
                if edge_length < result['stats']['min_edge_length']:
                    result['stats']['min_edge_length'] = edge_length
            
            # Check for degenerate polygon (all vertices collinear or in very small area)
            if len(unique_vertices) < 3:
                result['valid'] = False
                result['errors'].append(
                    f"Line {line_num}: Degenerate polygon with only {len(unique_vertices)} unique vertices"
                )
                result['stats']['degenerate_sheets'] += 1
    
    return result

def main():
    if len(sys.argv) < 2:
        print("Usage: validate_geometry.py <geometry.gm3d> [geometry2.gm3d ...]")
        sys.exit(1)
    
    files = sys.argv[1:]
    total_valid = 0
    total_invalid = 0
    
    for filepath in files:
        print(f"\n{'='*60}")
        print(f"Validating: {filepath}")
        print('='*60)
        
        result = validate_geometry_file(filepath)
        
        if result['valid']:
            print("✅ VALID GEOMETRY")
            total_valid += 1
        else:
            print("❌ INVALID GEOMETRY")
            total_invalid += 1
        
        # Print errors
        if result['errors']:
            print("\n🔴 ERRORS:")
            for error in result['errors']:
                print(f"  - {error}")
        
        # Print warnings
        if result['warnings']:
            print("\n⚠️  WARNINGS:")
            for warning in result['warnings']:
                print(f"  - {warning}")
        
        # Print statistics
        print("\n📊 STATISTICS:")
        stats = result['stats']
        print(f"  - Total sheets: {stats['total_sheets']}")
        print(f"  - Total vertices: {stats['total_vertices']}")
        print(f"  - Duplicate vertices: {stats['duplicate_vertices']}")
        print(f"  - Zero-length edges: {stats['zero_length_edges']}")
        print(f"  - Degenerate sheets: {stats['degenerate_sheets']}")
        if stats['min_edge_length'] != float('inf'):
            print(f"  - Minimum edge length: {stats['min_edge_length']:.2e}")
    
    # Summary
    print(f"\n{'='*60}")
    print("SUMMARY")
    print('='*60)
    print(f"Total files validated: {len(files)}")
    print(f"Valid: {total_valid}")
    print(f"Invalid: {total_invalid}")
    
    if total_invalid > 0:
        print("\n⚠️  Some geometry files have issues that will cause AdvCAD to crash.")
        print("Fix the duplicate vertices and zero-length edges before processing.")
        sys.exit(1)
    else:
        print("\n✅ All geometry files are valid.")

if __name__ == "__main__":
    main()