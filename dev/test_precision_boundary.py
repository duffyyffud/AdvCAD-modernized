#!/usr/bin/env python3
"""
Test precision boundaries to find the exact scale where AdvCAD starts failing
"""

import subprocess
from pathlib import Path
import sys

def create_box_model(scale: float, output_file: str):
    """Create a simple box model at given scale"""
    with open(output_file, 'w') as f:
        x = scale * 1.0
        y = scale * 2.0
        z = scale * 3.0
        f.write(f"box 0. 0. 0.  {x} {y} {z}\n")

def test_scale(scale: float) -> tuple[bool, str]:
    """Test if a scale works with AdvCAD"""
    model_file = f"precision_test_{scale}.gm3d"
    output_file = f"precision_test_{scale}.pch"
    
    # Create the model
    create_box_model(scale, model_file)
    
    try:
        # Test with AdvCAD
        result = subprocess.run([
            "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad",
            model_file,
            output_file,
            "0.1"
        ], capture_output=True, text=True, timeout=10)
        
        success = result.returncode == 0 and Path(output_file).exists()
        
        # Cleanup
        for f in [model_file, output_file]:
            try:
                Path(f).unlink()
            except:
                pass
                
        if success:
            return True, "SUCCESS"
        else:
            # Extract error type
            if "ASSERTION" in result.stderr:
                for line in result.stderr.split('\n'):
                    if "ASSERTION" in line and "failed" in line:
                        return False, line.strip()
            return False, "ERROR"
            
    except subprocess.TimeoutExpired:
        return False, "TIMEOUT"
    except Exception as e:
        return False, f"EXCEPTION: {e}"

def find_precision_boundary():
    """Find the exact scale where failures start occurring"""
    print("🔍 Finding precision boundary for AdvCAD...")
    print("="*60)
    
    # Test a range of scales to find the boundary
    scales_to_test = [
        1.0, 0.1, 0.01, 0.001,  # Should work
        1e-4, 5e-5, 1e-5, 5e-6, 1e-6, 1e-7, 1e-8  # Progressively smaller
    ]
    
    results = []
    
    for scale in scales_to_test:
        success, error = test_scale(scale)
        status = "✅" if success else "❌"
        print(f"Scale {scale:8.0e}: {status} {error if not success else 'SUCCESS'}")
        
        results.append({
            'scale': scale,
            'success': success,
            'error': error
        })
    
    # Find boundary
    print("\n" + "="*60)
    print("PRECISION BOUNDARY ANALYSIS")
    print("="*60)
    
    working_scales = [r for r in results if r['success']]
    failing_scales = [r for r in results if not r['success']]
    
    if working_scales and failing_scales:
        largest_working = max(r['scale'] for r in working_scales)
        smallest_failing = min(r['scale'] for r in failing_scales)
        
        print(f"✅ Largest working scale: {largest_working:.0e}")
        print(f"❌ Smallest failing scale: {smallest_failing:.0e}")
        print(f"🎯 Precision boundary is between {largest_working:.0e} and {smallest_failing:.0e}")
        
        # Show failure modes
        print(f"\n🔍 Failure modes at small scales:")
        for r in failing_scales:
            print(f"   Scale {r['scale']:8.0e}: {r['error']}")
    else:
        print("No clear boundary found in tested range")

def generate_boundary_test_models():
    """Generate models at the boundary for detailed analysis"""
    print(f"\n🧪 Generating boundary test models...")
    
    boundary_scales = [1e-3, 5e-4, 1e-4, 5e-5, 1e-5]
    
    for scale in boundary_scales:
        model_name = f"boundary_test_{scale:.0e}.gm3d"
        create_box_model(scale, f"generated_tests/{model_name}")
        print(f"Generated: generated_tests/{model_name}")

if __name__ == "__main__":
    find_precision_boundary()
    generate_boundary_test_models()