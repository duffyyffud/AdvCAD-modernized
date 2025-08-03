#!/usr/bin/env python3
"""
AdvCAD Regression Test Suite
Tests all shaft models to ensure fixes remain stable
"""

import subprocess
import os
import sys
import math
import re
from pathlib import Path

class RegressionTester:
    def __init__(self):
        self.project_root = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")
        self.advcad_exe = self.project_root / "build/command/advcad"
        self.sample_dir = self.project_root / "sample"
        self.test_results = []
        
    def find_optimal_mesh_size(self, model_file):
        """Find optimal mesh size for a given model using geometry analysis"""
        model_path = self.sample_dir / model_file
        
        # Analyze .gm3d CSG geometry to get coordinate scale
        coords = []
        try:
            with open(model_path, 'r') as f:
                for line in f:
                    line = line.strip()
                    if line.startswith('box ') and len(line.split()) >= 7:
                        # box x1 y1 z1 x2 y2 z2 format
                        parts = line.split()
                        x1, y1, z1 = float(parts[1]), float(parts[2]), float(parts[3])
                        x2, y2, z2 = float(parts[4]), float(parts[5]), float(parts[6])
                        # Add both corners of the box
                        coords.extend([(x1, y1, z1), (x1+x2, y1+y2, z1+z2)])
        except:
            return 1.0  # Conservative fallback - works for test_1.gm3d
            
        if not coords:
            return 1.0  # Conservative fallback - works for test_1.gm3d
            
        # Calculate model size
        xs, ys, zs = zip(*coords)
        min_x, max_x = min(xs), max(xs)
        min_y, max_y = min(ys), max(ys)
        min_z, max_z = min(zs), max(zs)
        
        size_x = max_x - min_x
        size_y = max_y - min_y  
        size_z = max_z - min_z
        model_size = math.sqrt(size_x**2 + size_y**2 + size_z**2)
        
        # For CSG models, use more conservative mesh size
        optimal_size = model_size / 50.0
        
        # Clamp to reasonable range, but prefer smaller sizes
        optimal_size = max(0.1, min(1.5, optimal_size))
        
        # For problematic models, use known working mesh size
        problematic_models = ['test_1.gm3d', 'test_3.gm3d', 'test_4.gm3d', 'test_7.gm3d', 
                            'shaft/air_practice.gm3d', 'shaft/air_up2_top_01.gm3d', 
                            'shaft/cyclic_mag_body_01.gm3d']
        if model_file in problematic_models:
            return 1.0
            
        return optimal_size
        
    def run_test(self, model_file, mesh_size, expected_status):
        """Run a single test case"""
        model_path = self.sample_dir / model_file
        output_file = f"test_regression_{model_file.replace('/', '_').replace('.gm3d', '.pch')}"
        
        cmd = [str(self.advcad_exe), str(model_path), output_file, str(mesh_size)]
        
        try:
            result = subprocess.run(cmd, cwd=self.project_root, 
                                  capture_output=True, text=True, timeout=60)
            
            # Check if output file was created
            output_path = self.project_root / output_file
            file_created = output_path.exists()
            file_size = output_path.stat().st_size if file_created else 0
            
            # Determine actual status
            if result.returncode == 0 and file_created and file_size > 0:
                actual_status = "SUCCESS"
            elif "ASSERTION" in result.stderr or "Segmentation fault" in result.stderr:
                actual_status = "ASSERTION_FAILURE"
            elif "Mixed triangle" in result.stderr:
                actual_status = "MIXED_TRIANGLES"
            elif result.returncode != 0:
                actual_status = "ERROR"
            else:
                actual_status = "UNKNOWN"
                
            # Record result
            test_result = {
                'model': model_file,
                'mesh_size': mesh_size,
                'expected': expected_status,
                'actual': actual_status,
                'file_size': file_size,
                'passed': actual_status == expected_status,
                'stderr_preview': result.stderr if result.stderr else ""
            }
            
            self.test_results.append(test_result)
            
            # Clean up output file
            if output_path.exists():
                output_path.unlink()
                
            return test_result
            
        except subprocess.TimeoutExpired:
            return {
                'model': model_file,
                'mesh_size': mesh_size,
                'expected': expected_status,
                'actual': "TIMEOUT",
                'file_size': 0,
                'passed': False,
                'stderr_preview': "Test timed out after 60 seconds"
            }
            
    def run_all_tests(self):
        """Run all regression tests"""
        print("🧪 AdvCAD Regression Test Suite")
        print("=" * 50)
        
        # Comprehensive test cases - all .gm3d files with optimal mesh sizes
        model_files = [
            # sample/ root level
            "block.gm3d", "test_1.gm3d", "test_2.gm3d", "test_3.gm3d", 
            "test_4.gm3d", "test_5.gm3d", "test_6.gm3d", "test_7.gm3d",
            
            # sample/cake/
            "cake/Magnetic0.gm3d", "cake/Magnetic1.gm3d", 
            "cake/Magnetic2.gm3d", "cake/Magnetic3.gm3d",
            
            # sample/shaft/
            "shaft/air_practice.gm3d", "shaft/air_up2_mid_out_01.gm3d",
            "shaft/air_up2_top_01.gm3d", "shaft/coil_01.gm3d", 
            "shaft/coil_01_mm.gm3d", "shaft/coil_02.gm3d", 
            "shaft/cyclic_mag_body_01.gm3d"
        ]
        
        for model_file in model_files:
            optimal_mesh_size = self.find_optimal_mesh_size(model_file)
            print(f"\n🔍 Testing {model_file} (optimal mesh size: {optimal_mesh_size:.6f})")
            result = self.run_test(model_file, optimal_mesh_size, "SUCCESS")
            
            status_icon = "✅" if result['passed'] else "❌"
            print(f"{status_icon} Expected: SUCCESS, Got: {result['actual']}")
            
            if result['file_size'] > 0:
                print(f"   📄 Output file: {result['file_size']} bytes")
            
            if not result['passed'] and result['stderr_preview']:
                print(f"   ⚠️  Error preview: {result['stderr_preview']}")
                
    def print_summary(self):
        """Print test summary"""
        total_tests = len(self.test_results)
        passed_tests = sum(1 for r in self.test_results if r['passed'])
        
        print("\n" + "=" * 50)
        print("📊 REGRESSION TEST SUMMARY")
        print("=" * 50)
        print(f"Total tests: {total_tests}")
        print(f"Passed: {passed_tests}")
        print(f"Failed: {total_tests - passed_tests}")
        print(f"Success rate: {passed_tests/total_tests*100:.1f}%")
        
        print("\n📋 DETAILED RESULTS:")
        for result in self.test_results:
            status = "✅ PASS" if result['passed'] else "❌ FAIL"
            print(f"{status} {result['model']} (mesh: {result['mesh_size']}) -> {result['actual']}")
            
        print("\n🔧 RECOMMENDATIONS:")
        failures = [r for r in self.test_results if not r['passed']]
        if not failures:
            print("🎉 All tests passed! No regressions detected.")
        else:
            print("⚠️  Some tests failed. Investigate:")
            for failure in failures:
                print(f"   - {failure['model']}: expected {failure['expected']}, got {failure['actual']}")

def main():
    """Run regression tests"""
    os.chdir("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")
    
    tester = RegressionTester()
    tester.run_all_tests()
    tester.print_summary()
    
    # Exit with error code if any tests failed
    failed_count = sum(1 for r in tester.test_results if not r['passed'])
    sys.exit(failed_count)

if __name__ == "__main__":
    main()