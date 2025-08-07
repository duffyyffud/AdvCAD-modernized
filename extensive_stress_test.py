#!/usr/bin/env python3
"""
Extensive stress test for AdvCAD using various geometric patterns
"""

import subprocess
from pathlib import Path
import random
import math
import sys

def create_multi_box_model(filename: str, num_boxes: int = 10, scale_range=(0.001, 0.1)):
    """Create model with multiple overlapping boxes"""
    with open(filename, 'w') as f:
        for i in range(num_boxes):
            # Random position and size within working precision range
            x = random.uniform(0, 0.1)
            y = random.uniform(0, 0.1) 
            z = random.uniform(0, 0.1)
            w = random.uniform(scale_range[0], scale_range[1])
            h = random.uniform(scale_range[0], scale_range[1])
            d = random.uniform(scale_range[0], scale_range[1])
            
            f.write(f"box {x} {y} {z}  {w} {h} {d}\n")
            if i > 0:
                # Add boolean operations
                f.write("add\n" if i % 3 == 0 else "subtract\n")

def create_complex_csg_model(filename: str):
    """Create complex CSG model with many operations"""
    with open(filename, 'w') as f:
        # Base structure
        f.write("box 0 0 0  0.05 0.05 0.05\n")
        
        # Add multiple features
        operations = ["add", "subtract", "subtract", "add"]
        
        for i, op in enumerate(operations):
            # Create feature
            x = random.uniform(0.01, 0.04)
            y = random.uniform(0.01, 0.04)
            z = random.uniform(0.01, 0.04)
            size = random.uniform(0.005, 0.02)
            
            f.write(f"box {x} {y} {z}  {size} {size} {size}\n")
            f.write(f"{op}\n")

def create_thin_structure_model(filename: str):
    """Create thin structures that might cause precision issues"""
    with open(filename, 'w') as f:
        # Very thin box (high aspect ratio)
        f.write("box 0 0 0  0.05 0.001 0.05\n")
        f.write("box 0.01 0 0.01  0.001 0.01 0.001\n")
        f.write("add\n")

def create_nested_features_model(filename: str):
    """Create model with nested small features"""
    with open(filename, 'w') as f:
        # Main body
        f.write("box 0 0 0  0.02 0.02 0.02\n")
        
        # Nested holes of decreasing size
        sizes = [0.008, 0.004, 0.002, 0.001]
        for i, size in enumerate(sizes):
            offset = 0.005 + i * 0.001
            f.write(f"box {offset} {offset} {offset}  {size} {size} {size}\n")
            f.write("subtract\n")

def test_model(model_file: str, mesh_sizes: list = [0.1, 0.01, 0.001]) -> dict:
    """Test a model with various mesh sizes"""
    results = {
        'model': model_file,
        'tests': []
    }
    
    for mesh_size in mesh_sizes:
        output_file = f"test_{Path(model_file).stem}_{mesh_size}.pch"
        
        try:
            result = subprocess.run([
                "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad",
                model_file,
                output_file,
                str(mesh_size)
            ], capture_output=True, text=True, timeout=30)
            
            success = result.returncode == 0 and Path(output_file).exists()
            
            # Get file size if successful
            file_size = Path(output_file).stat().st_size if success and Path(output_file).exists() else 0
            
            # Extract error info
            error_info = ""
            if not success and "ASSERTION" in result.stderr:
                for line in result.stderr.split('\n'):
                    if "ASSERTION" in line and "failed" in line:
                        error_info = line.strip()
                        break
            
            results['tests'].append({
                'mesh_size': mesh_size,
                'success': success,
                'file_size': file_size,
                'error': error_info if not success else None
            })
            
            # Cleanup
            try:
                Path(output_file).unlink()
            except:
                pass
                
        except subprocess.TimeoutExpired:
            results['tests'].append({
                'mesh_size': mesh_size,
                'success': False,
                'file_size': 0,
                'error': "TIMEOUT"
            })
        except Exception as e:
            results['tests'].append({
                'mesh_size': mesh_size,
                'success': False,
                'file_size': 0,
                'error': f"EXCEPTION: {e}"
            })
    
    return results

def run_extensive_tests():
    """Run extensive stress tests"""
    print("🚀 AdvCAD Extensive Stress Test Suite")
    print("="*60)
    
    # Create test directory
    test_dir = Path("stress_tests")
    test_dir.mkdir(exist_ok=True)
    
    # Generate various test models
    test_models = []
    
    print("🏗️  Generating test models...")
    
    # 1. Multiple overlapping boxes
    for i in range(3):
        filename = test_dir / f"multi_box_{i+1}.gm3d"
        create_multi_box_model(str(filename), num_boxes=5+i*3)
        test_models.append(str(filename))
        print(f"   Generated: {filename.name}")
    
    # 2. Complex CSG models
    for i in range(3):
        filename = test_dir / f"complex_csg_{i+1}.gm3d"
        create_complex_csg_model(str(filename))
        test_models.append(str(filename))
        print(f"   Generated: {filename.name}")
    
    # 3. Thin structures
    for i in range(2):
        filename = test_dir / f"thin_struct_{i+1}.gm3d"
        create_thin_structure_model(str(filename))
        test_models.append(str(filename))
        print(f"   Generated: {filename.name}")
    
    # 4. Nested features
    for i in range(2):
        filename = test_dir / f"nested_feat_{i+1}.gm3d"
        create_nested_features_model(str(filename))
        test_models.append(str(filename))
        print(f"   Generated: {filename.name}")
    
    print(f"\n🧪 Testing {len(test_models)} models with multiple mesh sizes...")
    print("="*60)
    
    all_results = []
    mesh_sizes = [0.1, 0.01, 0.005, 0.001]
    
    for i, model in enumerate(test_models):
        print(f"\n[{i+1:2d}/{len(test_models)}] Testing {Path(model).name}")
        results = test_model(model, mesh_sizes)
        all_results.append(results)
        
        # Show immediate results
        for test in results['tests']:
            status = "✅" if test['success'] else "❌"
            mesh = test['mesh_size']
            if test['success']:
                size = test['file_size']
                print(f"   Mesh {mesh:6.3f}: {status} ({size:,} bytes)")
            else:
                error = test['error'][:50] + "..." if test['error'] and len(test['error']) > 50 else test['error']
                print(f"   Mesh {mesh:6.3f}: {status} {error}")
    
    # Summary
    print("\n" + "="*60)
    print("📊 EXTENSIVE STRESS TEST SUMMARY")
    print("="*60)
    
    total_tests = sum(len(r['tests']) for r in all_results)
    successful_tests = sum(sum(1 for t in r['tests'] if t['success']) for r in all_results)
    
    success_rate = (successful_tests / total_tests * 100) if total_tests > 0 else 0
    
    print(f"Total models tested: {len(test_models)}")
    print(f"Total test cases: {total_tests}")
    print(f"Successful tests: {successful_tests}")
    print(f"Success rate: {success_rate:.1f}%")
    
    # Show failures by error type
    errors = {}
    for results in all_results:
        for test in results['tests']:
            if not test['success'] and test['error']:
                error_type = test['error'].split('"')[1] if '"' in test['error'] else test['error'][:30]
                errors[error_type] = errors.get(error_type, 0) + 1
    
    if errors:
        print(f"\n🔍 Failure modes:")
        for error, count in sorted(errors.items(), key=lambda x: x[1], reverse=True):
            print(f"   {count:2d}x: {error}")
    
    return all_results

if __name__ == "__main__":
    run_extensive_tests()