#!/usr/bin/env python3
"""
Geometric Test Generator for AdvCAD Triangulation Testing

Generates parametric .gm3d models to test specific triangulation scenarios:
- Simple geometries for baseline testing
- Complex geometries for stress testing  
- Edge cases for robustness testing
- Precision-sensitive geometries for numerical accuracy testing
"""

import os
import sys
import numpy as np
from pathlib import Path

class GeometricTestGenerator:
    def __init__(self, output_dir="generated_tests"):
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True)
        
    def create_holed_beam(self, name, thickness=10.0, width=50.0, length=200.0, 
                         hole_center_z=100.0, radius=15.0):
        """Generate holed beam based on holed_beam_meshing_tester.py logic"""
        script = f'box 0. 0. 0.  {thickness} {width} {length}\n'
        script += f'circle  -1. 0 {hole_center_z}  0 0 {radius}  1 0 0 36\n'
        script += f'extrude {thickness + 1.} 0 0\n'
        script += 'subtract\n'
        
        output_path = self.output_dir / f"{name}.gm3d"
        with open(output_path, 'w') as f:
            f.write(script)
        
        print(f"Generated: {output_path}")
        return output_path
        
    def create_simple_box(self, name, x=10.0, y=10.0, z=10.0):
        """Generate simple box for baseline testing"""
        script = f'box 0. 0. 0.  {x} {y} {z}\n'
        
        output_path = self.output_dir / f"{name}.gm3d"
        with open(output_path, 'w') as f:
            f.write(script)
            
        print(f"Generated: {output_path}")
        return output_path
        
    def create_precision_test(self, name, scale=1e-3):
        """Generate small-scale geometry to test precision handling"""
        x, y, z = 1.0 * scale, 2.0 * scale, 3.0 * scale
        script = f'box 0. 0. 0.  {x} {y} {z}\n'
        
        output_path = self.output_dir / f"{name}.gm3d"
        with open(output_path, 'w') as f:
            f.write(script)
            
        print(f"Generated: {output_path} (scale={scale})")
        return output_path
        
    def create_complex_boolean(self, name):
        """Generate complex boolean operations likely to cause mixed triangles"""
        script = '''box 0. 0. 0.  20. 20. 20.
box 5. 5. -5.  10. 10. 30.
subtract
box 8. 8. 8.  4. 4. 4.
subtract
'''
        output_path = self.output_dir / f"{name}.gm3d"
        with open(output_path, 'w') as f:
            f.write(script)
            
        print(f"Generated: {output_path}")
        return output_path
        
    def generate_test_suite(self):
        """Generate comprehensive test suite for triangulation debugging"""
        test_models = []
        
        # 1. Baseline tests
        test_models.append(self.create_simple_box("simple_box_10", 10, 10, 10))
        test_models.append(self.create_simple_box("simple_box_1", 1, 1, 1))
        
        # 2. Precision tests (trigger small-scale geometry detection)
        test_models.append(self.create_precision_test("precision_1e3", 1e-3))
        test_models.append(self.create_precision_test("precision_1e4", 1e-4))
        test_models.append(self.create_precision_test("precision_1e5", 1e-5))
        
        # 3. Holed beam variations
        test_models.append(self.create_holed_beam("holed_beam_center", 
                                                 hole_center_z=100.0, radius=15.0))
        test_models.append(self.create_holed_beam("holed_beam_edge", 
                                                 hole_center_z=25.0, radius=10.0))
        test_models.append(self.create_holed_beam("holed_beam_large", 
                                                 hole_center_z=100.0, radius=25.0))
        
        # 4. Complex boolean operations  
        test_models.append(self.create_complex_boolean("complex_boolean"))
        
        return test_models
        
    def test_model_with_advcad(self, model_path, mesh_size=1.0):
        """Test a generated model with AdvCAD"""
        output_pch = str(model_path).replace('.gm3d', '.pch')
        
        import subprocess
        advcad_path = "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad"
        
        try:
            result = subprocess.run([
                advcad_path, str(model_path), output_pch, str(mesh_size)
            ], capture_output=True, text=True, timeout=30)
            
            if result.returncode == 0:
                return "SUCCESS", len(result.stdout.splitlines())
            else:
                # Extract error type from output
                if "mixed triangle" in result.stderr.lower():
                    return "MIXED_TRIANGLES", result.stderr
                elif "assertion" in result.stderr.lower():
                    return "ASSERTION_FAILURE", result.stderr
                else:
                    return "OTHER_FAILURE", result.stderr
                    
        except subprocess.TimeoutExpired:
            return "TIMEOUT", "Process timed out"
        except Exception as e:
            return "ERROR", str(e)

def main():
    """Generate test suite and run triangulation tests"""
    generator = GeometricTestGenerator()
    
    print("🔧 Generating geometric test suite...")
    test_models = generator.generate_test_suite()
    
    print(f"\n📊 Testing {len(test_models)} generated models...")
    results = []
    
    for model_path in test_models:
        model_name = model_path.stem
        print(f"Testing {model_name}...", end=" ")
        
        status, details = generator.test_model_with_advcad(model_path)
        results.append((model_name, status, details))
        
        if status == "SUCCESS":
            print("✅ PASS")
        elif status == "MIXED_TRIANGLES":
            print("❌ MIXED_TRIANGLES")
        elif status == "ASSERTION_FAILURE":  
            print("❌ ASSERTION_FAILURE")
        else:
            print(f"❌ {status}")
    
    # Summary
    success_count = sum(1 for _, status, _ in results if status == "SUCCESS")
    total_count = len(results)
    success_rate = success_count / total_count * 100
    
    print(f"\n📈 Generated Test Results:")
    print(f"Success Rate: {success_rate:.1f}% ({success_count}/{total_count})")
    
    # Detailed results
    print(f"\n📋 Detailed Results:")
    for model_name, status, details in results:
        print(f"{'✅' if status == 'SUCCESS' else '❌'} {model_name}: {status}")
    
    return results

if __name__ == "__main__":
    main()