#!/usr/bin/env python3
"""
AdvCAD Mesh Size Optimizer

This script automatically tests various mesh sizes for a given geometry
to find the optimal parameter that produces successful triangulation.

Usage:
    python optimize_mesh_size.py [geometry_file] [output_file]
    
Example:
    python optimize_mesh_size.py ../sample/shaft/coil_01.gm3d ../sample/shaft/coil_01.pch

The script will:
1. Analyze the geometry to estimate appropriate mesh size range
2. Test multiple mesh sizes in decreasing order
3. Find the largest mesh size that works successfully
4. Report the optimal mesh size and triangulation statistics
"""

import os
import sys
import subprocess
import tempfile
import re
import math
from pathlib import Path

class MeshSizeOptimizer:
    def __init__(self, geometry_file, output_file, advcad_path="./advcad"):
        self.geometry_file = Path(geometry_file)
        self.output_file = Path(output_file)
        self.advcad_path = advcad_path
        
        if not self.geometry_file.exists():
            raise FileNotFoundError(f"Geometry file not found: {geometry_file}")
            
    def analyze_geometry(self):
        """
        Analyze the geometry file to estimate coordinate scale and feature sizes.
        Returns estimated min/max coordinates and suggested mesh size range.
        """
        print(f"Analyzing geometry: {self.geometry_file}")
        
        # Parse the .gm3d file to extract coordinate information
        coords = []
        try:
            with open(self.geometry_file, 'r') as f:
                for line in f:
                    line = line.strip()
                    # Look for coordinate lines (skip comments)
                    if line and not line.startswith('#') and not line.startswith('/'):
                        parts = line.split()
                        
                        # Handle .gm3d format: "sheet N x1 y1 z1 x2 y2 z2 ..." 
                        if len(parts) > 0 and parts[0] == 'sheet' and len(parts) >= 4:
                            # Skip "sheet" and count, then process coordinates in groups of 3
                            try:
                                coord_start = 2  # Skip "sheet" and count
                                for i in range(coord_start, len(parts) - 2, 3):
                                    if i + 2 < len(parts):
                                        x, y, z = float(parts[i]), float(parts[i+1]), float(parts[i+2])
                                        coords.append((x, y, z))
                            except (ValueError, IndexError):
                                continue
                        
                        # Also try to find simple coordinate lines (x y z format)
                        elif len(parts) >= 3:
                            try:
                                x, y, z = float(parts[0]), float(parts[1]), float(parts[2])
                                coords.append((x, y, z))
                            except ValueError:
                                continue
        except Exception as e:
            print(f"Warning: Could not parse geometry file: {e}")
            return None
            
        if not coords:
            print("Warning: No coordinates found in geometry file")
            return None
            
        # Calculate bounding box and feature sizes
        xs, ys, zs = zip(*coords)
        min_x, max_x = min(xs), max(xs)
        min_y, max_y = min(ys), max(ys)
        min_z, max_z = min(zs), max(zs)
        
        size_x = max_x - min_x
        size_y = max_y - min_y
        size_z = max_z - min_z
        
        # Overall model size
        model_size = math.sqrt(size_x**2 + size_y**2 + size_z**2)
        
        # Estimate coordinate unit
        if model_size > 100:
            coord_unit = "mm"
        elif model_size > 1:
            coord_unit = "m"
        else:
            coord_unit = "unknown"
            
        print(f"Geometry analysis:")
        print(f"  Bounding box: ({min_x:.6f}, {min_y:.6f}, {min_z:.6f}) to ({max_x:.6f}, {max_y:.6f}, {max_z:.6f})")
        print(f"  Model size: {model_size:.6f} (estimated unit: {coord_unit})")
        print(f"  Feature sizes: X={size_x:.6f}, Y={size_y:.6f}, Z={size_z:.6f}")
        
        return {
            'bounding_box': ((min_x, min_y, min_z), (max_x, max_y, max_z)),
            'model_size': model_size,
            'feature_sizes': (size_x, size_y, size_z),
            'coord_unit': coord_unit
        }
    
    def generate_mesh_sizes(self, geometry_info=None):
        """
        Generate a list of mesh sizes to test, from large to small.
        """
        if geometry_info:
            model_size = geometry_info['model_size']
            # Start with mesh sizes from 1/5 to 1/1000 of model size
            start_size = model_size / 5.0
            end_size = model_size / 1000.0
        else:
            # Default range if geometry analysis failed
            start_size = 1.0
            end_size = 0.0001
            
        # Generate logarithmic sequence
        mesh_sizes = []
        current = start_size
        while current >= end_size:
            mesh_sizes.append(current)
            current *= 0.7  # Reduce by factor of 0.7 each step
            
        return mesh_sizes
    
    def test_mesh_size(self, mesh_size, quiet=False):
        """
        Test a specific mesh size and return success status with details.
        """
        # Create temporary output file
        with tempfile.NamedTemporaryFile(suffix='.pch', delete=False) as tmp_file:
            tmp_output = tmp_file.name
            
        try:
            # Run advcad with the specified mesh size
            cmd = [self.advcad_path, str(self.geometry_file), tmp_output, str(mesh_size)]
            
            if not quiet:
                print(f"Testing mesh size {mesh_size:.6f}...", end=' ')
            
            result = subprocess.run(
                cmd, 
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE, 
                text=True, 
                timeout=60  # 60 second timeout
            )
            
            # Check if the command succeeded
            if result.returncode == 0:
                # Parse output for statistics - always parse stderr for stats but filter DEBUG
                stdout_lines = result.stdout.strip().split('\n')
                stderr_lines = result.stderr.strip().split('\n')
                
                # Filter out DEBUG messages from stderr but keep other important info
                filtered_stderr = [line for line in stderr_lines if not line.strip().startswith('DEBUG:')]
                
                output_lines = stdout_lines + filtered_stderr
                
                # Parse the .pch file to get actual triangle and node counts
                triangle_count = 0
                node_count = 0
                try:
                    with open(tmp_output, 'r') as pch_file:
                        lines = pch_file.readlines()
                        if len(lines) >= 1:
                            # First line is number of nodes
                            node_count = int(lines[0].strip())
                            
                            # Skip node coordinates and find triangle count
                            if len(lines) >= node_count + 2:
                                triangle_count = int(lines[node_count + 1].strip())
                except Exception:
                    # Fallback: count by file structure
                    triangle_count = 0
                    node_count = 0
                
                # Check if output file was created and has reasonable size
                if os.path.exists(tmp_output) and os.path.getsize(tmp_output) > 0:
                    file_size = os.path.getsize(tmp_output)
                    if not quiet:
                        print(f"SUCCESS (triangles: {triangle_count}, nodes: {node_count}, size: {file_size} bytes)")
                    return {
                        'success': True,
                        'mesh_size': mesh_size,
                        'triangle_count': triangle_count,
                        'node_count': node_count,
                        'output_size': file_size,
                        'stdout': result.stdout,
                        'stderr': '\n'.join(filtered_stderr)
                    }
                else:
                    if not quiet:
                        print("FAILED (no output generated)")
                    return {
                        'success': False,
                        'mesh_size': mesh_size,
                        'error': 'No output file generated',
                        'stdout': result.stdout,
                        'stderr': '\n'.join(filtered_stderr)
                    }
            else:
                # Command failed
                if not quiet:
                    print(f"FAILED (exit code: {result.returncode})")
                
                # Filter DEBUG from stderr for failed runs too
                stderr_lines = result.stderr.strip().split('\n')
                filtered_stderr = [line for line in stderr_lines if not line.strip().startswith('DEBUG:')]
                
                return {
                    'success': False,
                    'mesh_size': mesh_size,
                    'exit_code': result.returncode,
                    'error': '\n'.join(filtered_stderr),
                    'stdout': result.stdout,
                    'stderr': '\n'.join(filtered_stderr)
                }
                
        except subprocess.TimeoutExpired:
            print("FAILED (timeout)")
            return {
                'success': False,
                'mesh_size': mesh_size,
                'error': 'Timeout after 60 seconds'
            }
        except Exception as e:
            print(f"FAILED (exception: {e})")
            return {
                'success': False,
                'mesh_size': mesh_size,
                'error': str(e)
            }
        finally:
            # Clean up temporary file
            if os.path.exists(tmp_output):
                os.unlink(tmp_output)
    
    def optimize(self):
        """
        Find the optimal mesh size by testing multiple values.
        """
        print("="*60)
        print("AdvCAD Mesh Size Optimizer")
        print("="*60)
        
        # Analyze geometry
        geometry_info = self.analyze_geometry()
        
        # Generate mesh sizes to test
        mesh_sizes = self.generate_mesh_sizes(geometry_info)
        
        print(f"\nTesting {len(mesh_sizes)} mesh sizes from {mesh_sizes[0]:.6f} to {mesh_sizes[-1]:.6f}")
        print("-" * 60)
        
        successful_results = []
        failed_results = []
        
        # Test each mesh size (quietly to suppress DEBUG output)
        for i, mesh_size in enumerate(mesh_sizes):
            print(f"Testing mesh size {mesh_size:.6f}...", end=' ')
            result = self.test_mesh_size(mesh_size, quiet=True)
            
            if result['success']:
                print(f"SUCCESS (triangles: {result['triangle_count']}, nodes: {result['node_count']}, size: {result['output_size']} bytes)")
                successful_results.append(result)
            else:
                if 'exit_code' in result:
                    print(f"FAILED (exit code: {result['exit_code']})")
                else:
                    print(f"FAILED ({result.get('error', 'unknown error')})")
                failed_results.append(result)
                
            # If we found a working mesh size, we can be more selective about smaller ones
            if successful_results and len(failed_results) > 3:
                print("Found working mesh size, skipping remaining tests")
                break 
        
        print("-" * 60)
        
        if successful_results:
            # Find the largest (most efficient) mesh size that works
            optimal_result = max(successful_results, key=lambda x: x['mesh_size'])
            
            print("OPTIMIZATION SUCCESSFUL!")
            print(f"Optimal mesh size: {optimal_result['mesh_size']:.6f}")
            print(f"Triangles generated: {optimal_result['triangle_count']}")
            print(f"Nodes generated: {optimal_result['node_count']}")
            print(f"Output file size: {optimal_result['output_size']} bytes")
            
            # Generate the final output with optimal mesh size
            print(f"\nGenerating final output: {self.output_file}")
            final_cmd = [self.advcad_path, str(self.geometry_file), str(self.output_file), str(optimal_result['mesh_size'])]
            
            try:
                final_result = subprocess.run(final_cmd, capture_output=True, text=True, timeout=60)
                if final_result.returncode == 0:
                    print("Final output generated successfully!")
                else:
                    print(f"Warning: Final generation failed (exit code: {final_result.returncode})")
                    print(f"Error: {final_result.stderr}")
            except Exception as e:
                print(f"Warning: Final generation failed with exception: {e}")
            
            # Show recommendation
            print("\nRECOMMENDATION:")
            print(f"Use mesh size {optimal_result['mesh_size']:.6f} for this geometry")
            
            if geometry_info:
                ratio = optimal_result['mesh_size'] / geometry_info['model_size']
                print(f"This is {ratio:.1%} of the total model size")
                
            return optimal_result
        else:
            print("OPTIMIZATION FAILED!")
            print("No mesh size produced successful triangulation")
            
            if failed_results:
                print("\nErrors encountered:")
                for result in failed_results[:5]:  # Show first 5 errors
                    error_msg = result.get('error', 'Unknown error')
                    print(f"  Mesh size {result['mesh_size']:.6f}: {error_msg}")
                    
                if len(failed_results) > 5:
                    print(f"  ... and {len(failed_results) - 5} more")
                    
            print("\nSUGGESTIONS:")
            print("1. Check if geometry file is valid")
            print("2. Try different coordinate units (convert to mm if using meters)")
            print("3. Check for degenerate geometry or self-intersections")
            print("4. Manual mesh size range: try values between 0.001 and 0.1")
            
            return None

def main():
    if len(sys.argv) < 3:
        print("Usage: python optimize_mesh_size.py <geometry_file> <output_file>")
        print("Example: python optimize_mesh_size.py ../sample/shaft/coil_01.gm3d ../sample/shaft/coil_01.pch")
        sys.exit(1)
        
    geometry_file = sys.argv[1]
    output_file = sys.argv[2]
    
    # Check if advcad is available
    advcad_path = "./advcad"
    if not os.path.exists(advcad_path):
        # Try alternative paths
        for alt_path in ["advcad", "../command/advcad", "./command/advcad"]:
            if os.path.exists(alt_path):
                advcad_path = alt_path
                break
        else:
            print("Error: advcad executable not found")
            print("Make sure you're running this script from the correct directory")
            sys.exit(1)
    
    try:
        optimizer = MeshSizeOptimizer(geometry_file, output_file, advcad_path)
        result = optimizer.optimize()
        
        if result:
            sys.exit(0)  # Success
        else:
            sys.exit(1)  # Failure
            
    except KeyboardInterrupt:
        print("\nOptimization interrupted by user")
        sys.exit(1)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()