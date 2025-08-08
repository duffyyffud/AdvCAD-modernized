#!/usr/bin/env python3
"""
Face 5 Debug Generator

Creates targeted test cases to debug why face 5 bypasses robust CDT selection.
Face 5 characteristics: 6 segments, 6 nodes, small-scale geometry
"""

import subprocess
from pathlib import Path

class Face5DebugGenerator:
    def __init__(self):
        self.output_dir = Path("face5_debug_tests")
        self.output_dir.mkdir(exist_ok=True)
        
    def create_hexagonal_model(self, name, scale=1.0):
        """Create hexagonal geometry similar to face 5 complexity"""
        # Create a hexagonal prism - should have 6 segments per face
        script = f'''box 0. 0. 0.  {1.0*scale} {1.0*scale} {0.1*scale}
circle 0.5 0.5 -0.05  0 0 {0.3*scale} 1 0 0 6
extrude 0 0 {0.2*scale}
subtract
'''
        
        output_path = self.output_dir / f"{name}.gm3d"
        with open(output_path, 'w') as f:
            f.write(script)
        
        return output_path
        
    def create_6_segment_face(self, name, scale=1.0):
        """Create geometry specifically to generate 6-segment faces"""
        # Create a box with a hexagonal hole - forces 6-segment faces
        script = f'''box 0. 0. 0.  {2.0*scale} {2.0*scale} {0.5*scale}
circle {1.0*scale} {1.0*scale} -0.1  0 0 {0.6*scale} 1 0 0 6
extrude 0 0 {0.7*scale}
subtract
'''
        
        output_path = self.output_dir / f"{name}.gm3d"  
        with open(output_path, 'w') as f:
            f.write(script)
            
        return output_path
        
    def test_model_detailed(self, model_path, mesh_size=1.0):
        """Test model with detailed debug output"""
        output_pch = str(model_path).replace('.gm3d', '.pch')
        advcad_path = "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/command/advcad"
        
        try:
            result = subprocess.run([
                advcad_path, str(model_path), output_pch, str(mesh_size)
            ], capture_output=True, text=True, timeout=30, cwd="/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")
            
            # Analyze debug output for triangulation selection messages
            debug_lines = result.stderr.split('\n') if result.stderr else []
            
            face_characteristics = []
            robust_cdt_usage = []
            triangulator_assertions = []
            
            for line in debug_lines:
                if "Face characteristics" in line:
                    face_characteristics.append(line.strip())
                elif "Using robust CDT" in line:
                    robust_cdt_usage.append(line.strip()) 
                elif "generateTriangles() entry" in line:
                    triangulator_assertions.append(line.strip())
                elif "triangulator()=" in line:
                    triangulator_assertions.append(line.strip())
            
            analysis = {
                'returncode': result.returncode,
                'face_characteristics': face_characteristics,
                'robust_cdt_usage': robust_cdt_usage, 
                'triangulator_assertions': triangulator_assertions,
                'output_lines': len(debug_lines)
            }
            
            if result.returncode == 0:
                return "SUCCESS", analysis
            else:
                return "FAILURE", analysis
                
        except subprocess.TimeoutExpired:
            return "TIMEOUT", {'error': 'Process timed out'}
        except Exception as e:
            return "ERROR", {'error': str(e)}

def main():
    """Generate and test face 5 debug cases"""
    generator = Face5DebugGenerator()
    
    print("🔍 Generating Face 5 debug test cases...")
    
    # Generate test cases at different scales
    test_cases = [
        # Normal scale - should work
        generator.create_hexagonal_model("hex_normal", 1.0),
        generator.create_6_segment_face("6seg_normal", 1.0),
        
        # Small scale - should trigger robust CDT (like face 5)
        generator.create_hexagonal_model("hex_small", 1e-3),
        generator.create_6_segment_face("6seg_small", 1e-3),
        
        # Very small scale - might fail like face 5
        generator.create_hexagonal_model("hex_tiny", 1e-4), 
        generator.create_6_segment_face("6seg_tiny", 1e-4),
    ]
    
    print(f"\n📊 Testing {len(test_cases)} debug models...")
    
    for model_path in test_cases:
        model_name = model_path.stem
        print(f"\n🔬 Testing {model_name}...")
        
        status, analysis = generator.test_model_detailed(model_path)
        
        print(f"   Status: {status}")
        if isinstance(analysis, dict):
            print(f"   Face characteristics found: {len(analysis.get('face_characteristics', []))}")
            print(f"   Robust CDT usage found: {len(analysis.get('robust_cdt_usage', []))}")
            print(f"   Triangulator assertions: {len(analysis.get('triangulator_assertions', []))}")
            
            # Show specific messages
            for msg in analysis.get('face_characteristics', []):
                print(f"     📐 {msg}")
            for msg in analysis.get('robust_cdt_usage', []):
                print(f"     🛠️ {msg}")
            
            # Show any triangulator state issues
            for msg in analysis.get('triangulator_assertions', []):
                if "triangulator()=" in msg:
                    print(f"     ⚙️ {msg}")

if __name__ == "__main__":
    main()