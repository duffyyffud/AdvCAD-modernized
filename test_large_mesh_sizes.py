#!/usr/bin/env python3
"""
Test AdvCAD with large mesh sizes to find failure boundaries
"""

import subprocess
import os
from pathlib import Path
import glob

def test_mesh_size(model_file, mesh_size):
    """Test a model with specific mesh size"""
    advcad = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad")
    output_file = Path(f"test_mesh_{mesh_size}.pch")
    
    if output_file.exists():
        output_file.unlink()
    
    try:
        result = subprocess.run(
            [str(advcad), str(model_file), str(output_file), str(mesh_size)],
            capture_output=True,
            text=True,
            timeout=10  # Short timeout for large meshes
        )
        
        success = result.returncode == 0 and output_file.exists()
        
        if output_file.exists():
            size = output_file.stat().st_size
            output_file.unlink()
        else:
            size = 0
            
        return {
            'success': success,
            'file_size': size,
            'stderr': result.stderr if not success else None
        }
    except subprocess.TimeoutExpired:
        return {
            'success': False,
            'file_size': 0,
            'stderr': 'TIMEOUT'
        }

def test_large_sizes():
    """Test various models with large mesh sizes"""
    # Get all .gm3d files
    sample_dir = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/sample")
    models = []
    models.extend(sample_dir.glob("*.gm3d"))
    models.extend(sample_dir.glob("**/*.gm3d"))
    
    # Test with progressively larger mesh sizes
    test_sizes = [1.0, 2.0, 5.0, 10.0, 20.0, 50.0, 100.0, 200.0, 500.0, 1000.0]
    
    print("="*60)
    print("LARGE MESH SIZE TESTING")
    print("="*60)
    
    failures = []
    
    for model in sorted(models):
        print(f"\n📄 Testing: {model.relative_to(sample_dir)}")
        print("-"*40)
        
        for size in test_sizes:
            result = test_mesh_size(model, size)
            status = "✅" if result['success'] else "❌"
            print(f"   Mesh {size:7.1f}: {status}", end="")
            
            if result['success']:
                if result['file_size'] > 0:
                    print(f" ({result['file_size']:,} bytes)")
                else:
                    print(" (0 bytes - possible issue)")
            else:
                if result['stderr'] and 'ASSERTION' in result['stderr']:
                    # Extract assertion info
                    for line in result['stderr'].split('\n'):
                        if 'ASSERTION' in line:
                            assertion = line.strip()
                            print(f"\n      → {assertion[:80]}")
                            failures.append({
                                'model': model.relative_to(sample_dir),
                                'mesh_size': size,
                                'error': assertion
                            })
                            break
                elif result['stderr'] == 'TIMEOUT':
                    print(" (TIMEOUT - mesh too coarse)")
                else:
                    print(" (ERROR)")
                # Stop testing larger sizes after first failure
                print(f"   🛑 Failed at size: {size}")
                break
        else:
            print(f"   ✅ All sizes up to {test_sizes[-1]} work!")
    
    # Summary
    print("\n" + "="*60)
    print("SUMMARY")
    print("="*60)
    
    if failures:
        print(f"\n❌ Found {len(failures)} failures with large mesh sizes:")
        for f in failures:
            print(f"   - {f['model']} at mesh {f['mesh_size']}")
            print(f"     {f['error'][:100]}")
    else:
        print("\n✅ All models work with all tested mesh sizes!")

if __name__ == "__main__":
    test_large_sizes()