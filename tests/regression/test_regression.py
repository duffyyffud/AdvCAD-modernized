#!/usr/bin/env python3
"""
AdvCAD Regression Test Suite
Tests all shaft models to ensure fixes remain stable
"""

import subprocess
import os
import sys
from pathlib import Path

class RegressionTester:
    def __init__(self):
        self.project_root = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")
        self.advcad_exe = self.project_root / "command/advcad"
        self.shaft_dir = self.project_root / "tests/data/shaft"
        self.test_results = []
        
    def run_test(self, model_file, mesh_size, expected_status):
        """Run a single test case"""
        model_path = self.shaft_dir / model_file
        output_file = f"test_regression_{model_file.replace('.gm3d', '.pch')}"
        
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
                'stderr_preview': result.stderr[:200] if result.stderr else ""
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
        
        # Test cases based on current known status
        test_cases = [
            # Known working cases
            ("coil_01_mm.gm3d", 2.0, "SUCCESS"),
            ("coil_02.gm3d", 2.0, "SUCCESS"),
            ("air_up2_mid_out_01.gm3d", 2.0, "SUCCESS"),
            ("coil_01.gm3d", 2.0, "SUCCESS"),
            
            # Problematic cases
            ("air_up2_top_01.gm3d", 2.0, "ASSERTION_FAILURE"),  # Neighbor edge validation
            ("cyclic_mag_body_01.gm3d", 2.0, "MIXED_TRIANGLES"),  # Mixed triangles
            
            # Test with appropriate mesh sizes
            ("cyclic_mag_body_01.gm3d", 0.0001, "SUCCESS"),  # Should work with proper mesh size
        ]
        
        for model_file, mesh_size, expected_status in test_cases:
            print(f"\n🔍 Testing {model_file} (mesh size: {mesh_size})")
            result = self.run_test(model_file, mesh_size, expected_status)
            
            status_icon = "✅" if result['passed'] else "❌"
            print(f"{status_icon} Expected: {expected_status}, Got: {result['actual']}")
            
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