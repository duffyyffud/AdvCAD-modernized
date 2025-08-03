#!/usr/bin/env python3
"""
Systematic Test Plan for AdvCAD with Robust CDT
Tests all shaft models with mesh size optimization to identify actual failure patterns
"""

import os
import sys
import subprocess
from pathlib import Path
import datetime
import re

def run_test(geometry_file, output_dir, advcad_path):
    """Run mesh optimization test on a single geometry file"""
    print(f"\n{'='*80}")
    print(f"Testing: {geometry_file.name}")
    print(f"{'='*80}")
    
    output_file = output_dir / f"{geometry_file.stem}_optimized.pch"
    
    # Run optimize_mesh_size.py
    cmd = [sys.executable, "optimize_mesh_size.py", str(geometry_file), str(output_file)]
    
    result = {
        'geometry': geometry_file.name,
        'timestamp': datetime.datetime.now().isoformat(),
        'success': False,
        'optimal_mesh_size': None,
        'error_patterns': [],
        'assertion_info': None,
        'face_numbers': set(),
        'mixed_triangles': False
    }
    
    try:
        proc = subprocess.run(cmd, capture_output=True, text=True, timeout=300)
        
        # Analyze output for various error patterns and face numbers
        output = proc.stdout + proc.stderr
        
        # Look for assertion failures
        assertion_match = re.search(r'ASSERTION "([^"]+)" failed: file "([^"]+)", line (\d+)', output)
        if assertion_match:
            result['assertion_info'] = {
                'condition': assertion_match.group(1),
                'file': assertion_match.group(2),
                'line': assertion_match.group(3)
            }
            result['error_patterns'].append('assertion_failure')
        
        # Look for face numbers mentioned in output
        face_matches = re.findall(r'[Ff]ace[\s_]*(\d+)', output)
        if face_matches:
            result['face_numbers'] = set(int(f) for f in face_matches)
        
        # Look for mixed triangle mentions
        if 'mixed triangle' in output.lower() or 'dummy point' in output.lower():
            result['mixed_triangles'] = True
            result['error_patterns'].append('mixed_triangles')
        
        # Look for segmentation faults
        if 'segmentation fault' in output.lower():
            result['error_patterns'].append('segfault')
        
        # Look for constraint recovery failures
        if 'constraint recovery' in output.lower() or 'failed to recover' in output.lower():
            result['error_patterns'].append('constraint_recovery_failure')
        
        # Check if optimization succeeded
        if proc.returncode == 0:
            result['success'] = True
            # Parse optimal mesh size from output
            for line in proc.stdout.split('\n'):
                if 'Optimal mesh size:' in line:
                    try:
                        result['optimal_mesh_size'] = float(line.split(':')[1].strip())
                    except:
                        pass
            print(f"✓ SUCCESS - Optimal mesh size: {result['optimal_mesh_size']}")
        else:
            print(f"✗ FAILED - No working mesh size found")
            if result['assertion_info']:
                print(f"  Assertion: {result['assertion_info']['condition']}")
                print(f"  Location: {result['assertion_info']['file']}:{result['assertion_info']['line']}")
            if result['face_numbers']:
                print(f"  Faces mentioned: {sorted(result['face_numbers'])}")
            if result['error_patterns']:
                print(f"  Error patterns: {', '.join(result['error_patterns'])}")
            
    except subprocess.TimeoutExpired:
        result['error_patterns'].append('timeout')
        print("✗ FAILED - Timeout")
    except Exception as e:
        result['error_patterns'].append('exception')
        print(f"✗ FAILED - {e}")
        
    return result

def analyze_patterns(results):
    """Analyze results to find common failure patterns"""
    patterns = {
        'assertions': {},
        'faces': {},
        'error_types': {},
        'files': {}
    }
    
    for r in results:
        # Count assertion failures by condition
        if r['assertion_info']:
            key = f"{r['assertion_info']['condition']} at {r['assertion_info']['file']}:{r['assertion_info']['line']}"
            patterns['assertions'][key] = patterns['assertions'].get(key, 0) + 1
        
        # Count face mentions
        for face in r['face_numbers']:
            patterns['faces'][face] = patterns['faces'].get(face, 0) + 1
        
        # Count error patterns
        for error in r['error_patterns']:
            patterns['error_types'][error] = patterns['error_types'].get(error, 0) + 1
        
        # Group by success/failure
        status = 'success' if r['success'] else 'failure'
        patterns['files'][status] = patterns['files'].get(status, [])
        patterns['files'][status].append(r['geometry'])
    
    return patterns

def main():
    print("AdvCAD Systematic Test Plan")
    print("Testing all shaft models without preconceptions")
    print(f"Started at: {datetime.datetime.now()}")
    
    # Setup paths
    shaft_dir = Path("sample/shaft")
    output_dir = Path("test_results")
    output_dir.mkdir(exist_ok=True)
    
    # Find advcad executable
    advcad_path = Path("build/command/advcad")
    if not advcad_path.exists():
        print(f"Error: advcad not found at {advcad_path}")
        sys.exit(1)
    
    # Get all .gm3d files
    gm3d_files = sorted(shaft_dir.glob("*.gm3d"))
    
    print(f"\nFound {len(gm3d_files)} geometry files to test:")
    for f in gm3d_files:
        print(f"  - {f.name}")
    
    # Run tests
    results = []
    for geometry_file in gm3d_files:
        result = run_test(geometry_file, output_dir, advcad_path)
        results.append(result)
    
    # Analyze patterns
    patterns = analyze_patterns(results)
    
    # Summary report
    print(f"\n{'='*80}")
    print("TEST SUMMARY")
    print(f"{'='*80}")
    
    successful = [r for r in results if r['success']]
    failed = [r for r in results if not r['success']]
    
    print(f"\nTotal tests: {len(results)}")
    print(f"Successful: {len(successful)} ({len(successful)/len(results)*100:.1f}%)")
    print(f"Failed: {len(failed)} ({len(failed)/len(results)*100:.1f}%)")
    
    if patterns['assertions']:
        print("\n📍 ASSERTION FAILURES:")
        for assertion, count in sorted(patterns['assertions'].items(), key=lambda x: -x[1]):
            print(f"  {count}x: {assertion}")
    
    if patterns['faces']:
        print("\n📍 FACES MENTIONED IN ERRORS:")
        for face, count in sorted(patterns['faces'].items(), key=lambda x: -x[1]):
            print(f"  Face {face}: {count} times")
    
    if patterns['error_types']:
        print("\n📍 ERROR PATTERNS:")
        for error, count in sorted(patterns['error_types'].items(), key=lambda x: -x[1]):
            print(f"  {error}: {count} times")
    
    if 'success' in patterns['files']:
        print("\n✓ SUCCESSFUL TESTS:")
        for r in successful:
            print(f"  {r['geometry']}: mesh size = {r['optimal_mesh_size']:.6f}")
    
    if 'failure' in patterns['files']:
        print("\n✗ FAILED TESTS:")
        for r in failed:
            faces = f" (faces: {sorted(r['face_numbers'])})" if r['face_numbers'] else ""
            print(f"  {r['geometry']}{faces}")
    
    # Save detailed results
    results_file = output_dir / f"test_results_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.txt"
    with open(results_file, 'w') as f:
        f.write("AdvCAD Systematic Test Results\n")
        f.write("=" * 80 + "\n\n")
        
        for r in results:
            f.write(f"Geometry: {r['geometry']}\n")
            f.write(f"Timestamp: {r['timestamp']}\n")
            f.write(f"Success: {r['success']}\n")
            f.write(f"Optimal mesh size: {r['optimal_mesh_size']}\n")
            if r['assertion_info']:
                f.write(f"Assertion: {r['assertion_info']['condition']}\n")
                f.write(f"Location: {r['assertion_info']['file']}:{r['assertion_info']['line']}\n")
            if r['face_numbers']:
                f.write(f"Faces mentioned: {sorted(r['face_numbers'])}\n")
            if r['error_patterns']:
                f.write(f"Error patterns: {', '.join(r['error_patterns'])}\n")
            f.write("-" * 40 + "\n\n")
    
    print(f"\nDetailed results saved to: {results_file}")
    
    # Data-driven recommendations
    print(f"\n{'='*80}")
    print("DATA-DRIVEN RECOMMENDATIONS")
    print(f"{'='*80}")
    
    if patterns['assertions']:
        print("\n1. Most common assertion failures:")
        top_assertion = max(patterns['assertions'].items(), key=lambda x: x[1])
        print(f"   '{top_assertion[0]}' occurred {top_assertion[1]} times")
        print("   This should be investigated first.")
    
    if patterns['faces'] and len(patterns['faces']) > 1:
        print("\n2. Multiple faces are involved in failures:")
        for face, count in sorted(patterns['faces'].items(), key=lambda x: -x[1])[:3]:
            print(f"   Face {face}: {count} occurrences")
        print("   The issue may not be specific to one face.")
    elif patterns['faces'] and len(patterns['faces']) == 1:
        face = list(patterns['faces'].keys())[0]
        print(f"\n2. Face {face} appears to be the only problematic face.")
    
    if patterns['error_types']:
        print("\n3. Error pattern analysis:")
        for error, count in patterns['error_types'].items():
            print(f"   {error}: {count} cases")
    
    print("\n4. Next debugging steps:")
    print("   - Focus on the most common assertion failure")
    print("   - Test with robust CDT enabled to see if it helps")
    print("   - Add targeted debugging for the specific failure patterns found")

if __name__ == "__main__":
    main()