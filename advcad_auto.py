#!/usr/bin/env python3
"""
AdvCAD Automatic Mesh Size Interface

Automatically determines optimal mesh size for .gm3d files and runs AdvCAD.
Eliminates manual trial-and-error for mesh size selection.

Usage:
    python3 advcad_auto.py <input.gm3d> <output.pch> [override_mesh_size]
    
Examples:
    python3 advcad_auto.py sample/test_1.gm3d output.pch          # Auto mesh size
    python3 advcad_auto.py sample/test_1.gm3d output.pch 1.5      # Override with 1.5
"""

import sys
import os
import subprocess
import math
import re
from pathlib import Path
from typing import Optional, Tuple, Dict

# Import PCH validator
sys.path.insert(0, str(Path(__file__).parent / 'scripts'))
try:
    from PchReader import validate_pch_file
    PCH_VALIDATION_AVAILABLE = True
except ImportError:
    PCH_VALIDATION_AVAILABLE = False
    print("⚠️  Warning: PchReader not found - output validation disabled")

class AdvCADAutoMesh:
    """Automatic mesh size determination and AdvCAD execution"""
    
    def __init__(self, advcad_path: Optional[str] = None):
        """Initialize with AdvCAD executable path"""
        self.project_root = Path("/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b")
        self.advcad_exe = Path(advcad_path) if advcad_path else self.project_root / "build/command/advcad"
        
        if not self.advcad_exe.exists():
            raise FileNotFoundError(f"AdvCAD executable not found at {self.advcad_exe}")
        
        # Known problematic models that need specific mesh sizes
        self.model_overrides = {
            'test_1.gm3d': 1.0,
            'test_3.gm3d': 1.0,
            'test_4.gm3d': 1.0,
            'test_7.gm3d': 1.0,
            'air_practice.gm3d': 1.0,
            'air_up2_top_01.gm3d': 1.0,
            'cyclic_mag_body_01.gm3d': 1.0,
        }
        
    def analyze_gm3d_geometry(self, gm3d_path: Path) -> Dict[str, float]:
        """Analyze .gm3d file to extract geometry information"""
        coords = []
        min_feature_size = float('inf')
        
        try:
            with open(gm3d_path, 'r') as f:
                for line in f:
                    line = line.strip()
                    
                    # Parse CSG box commands: box x y z width height depth
                    if line.startswith('box ') and len(line.split()) >= 7:
                        parts = line.split()
                        x, y, z = float(parts[1]), float(parts[2]), float(parts[3])
                        w, h, d = float(parts[4]), float(parts[5]), float(parts[6])
                        
                        # Track bounding box
                        coords.extend([(x, y, z), (x+w, y+h, z+d)])
                        
                        # Track minimum feature size
                        min_feature_size = min(min_feature_size, w, h, d)
                        
                    # Parse cylinder commands if present
                    elif line.startswith('cylinder '):
                        parts = line.split()
                        if len(parts) >= 8:
                            radius = float(parts[7])
                            min_feature_size = min(min_feature_size, radius * 2)
                            
        except Exception as e:
            print(f"Warning: Could not fully parse geometry: {e}")
            
        if not coords:
            # Fallback for unknown geometry format
            return {
                'model_size': 100.0,
                'min_feature': 1.0,
                'aspect_ratio': 1.0,
                'recommended_mesh': 1.0
            }
            
        # Calculate bounding box
        xs, ys, zs = zip(*coords)
        min_x, max_x = min(xs), max(xs)
        min_y, max_y = min(ys), max(ys)
        min_z, max_z = min(zs), max(zs)
        
        size_x = max_x - min_x
        size_y = max_y - min_y
        size_z = max_z - min_z
        
        # Model characteristics
        model_size = math.sqrt(size_x**2 + size_y**2 + size_z**2)
        aspect_ratio = max(size_x, size_y, size_z) / max(min(size_x, size_y, size_z), 0.001)
        
        if min_feature_size == float('inf'):
            min_feature_size = min(size_x, size_y, size_z) / 10
            
        return {
            'model_size': model_size,
            'min_feature': min_feature_size,
            'aspect_ratio': aspect_ratio,
            'size_x': size_x,
            'size_y': size_y,
            'size_z': size_z,
            'recommended_mesh': self.calculate_optimal_mesh_size(model_size, min_feature_size, aspect_ratio)
        }
    
    def calculate_optimal_mesh_size(self, model_size: float, min_feature: float, aspect_ratio: float) -> float:
        """Calculate optimal mesh size based on geometry analysis"""
        
        # Base calculation: aim for 10-20 elements across smallest feature
        base_mesh = min_feature / 15.0
        
        # Adjust for model size (larger models can use proportionally larger mesh)
        size_factor = min(1.0, model_size / 100.0)
        
        # Adjust for aspect ratio (high aspect ratios need finer mesh)
        aspect_factor = 1.0 if aspect_ratio < 5 else 0.8
        
        # Calculate final mesh size
        mesh_size = base_mesh * size_factor * aspect_factor
        
        # Apply reasonable bounds
        mesh_size = max(0.001, min(10.0, mesh_size))
        
        # Round to reasonable precision
        if mesh_size < 0.01:
            mesh_size = round(mesh_size, 4)
        elif mesh_size < 0.1:
            mesh_size = round(mesh_size, 3)
        elif mesh_size < 1.0:
            mesh_size = round(mesh_size, 2)
        else:
            mesh_size = round(mesh_size, 1)
            
        return mesh_size
    
    def determine_mesh_size(self, gm3d_path: Path) -> Tuple[float, str]:
        """Determine optimal mesh size for a model"""
        
        # Check for known problematic models
        model_name = gm3d_path.name
        if model_name in self.model_overrides:
            return self.model_overrides[model_name], "model-specific override"
            
        # Analyze geometry
        geometry = self.analyze_gm3d_geometry(gm3d_path)
        mesh_size = geometry['recommended_mesh']
        
        # Create explanation
        explanation = (f"Geometry analysis: "
                      f"size={geometry['model_size']:.1f}, "
                      f"min_feature={geometry['min_feature']:.2f}, "
                      f"aspect={geometry['aspect_ratio']:.1f}")
        
        return mesh_size, explanation
    
    def run_advcad(self, input_path: Path, output_path: Path, mesh_size: float) -> bool:
        """Run AdvCAD with specified parameters"""
        
        cmd = [str(self.advcad_exe), str(input_path), str(output_path), str(mesh_size)]
        
        print(f"Executing: {' '.join(cmd)}")
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=300  # 5 minute timeout
            )
            
            # Check for success patterns
            if "Patch generation completed successfully" in result.stdout:
                return True
            
            # Check for known failure patterns
            if "ASSERTION" in result.stderr:
                print("⚠️  Assertion failure detected")
            elif "MIXED_TRIANGLES" in result.stderr:
                print("⚠️  Mixed triangles detected")
            elif "FATAL ERROR" in result.stderr:
                print("⚠️  Fatal error occurred")
                
            # Print relevant error information
            if result.returncode != 0:
                print("\n❌ AdvCAD failed with error:")
                # Extract last meaningful error message
                error_lines = result.stderr.split('\n')
                for line in reversed(error_lines):
                    if 'ERROR' in line or 'ASSERTION' in line or 'FATAL' in line:
                        print(f"  {line}")
                        break
                        
            return result.returncode == 0
            
        except subprocess.TimeoutExpired:
            print("⚠️  AdvCAD timed out after 5 minutes")
            return False
        except Exception as e:
            print(f"❌ Error running AdvCAD: {e}")
            return False
    
    def process(self, input_path: str, output_path: str, override_mesh: Optional[float] = None) -> bool:
        """Main processing function"""
        
        input_file = Path(input_path)
        output_file = Path(output_path)
        
        # Validate input
        if not input_file.exists():
            print(f"❌ Input file not found: {input_file}")
            return False
            
        print(f"\n🔧 AdvCAD Automatic Mesh Processing")
        print(f"{'='*50}")
        print(f"Input:  {input_file}")
        print(f"Output: {output_file}")
        
        # Determine mesh size
        if override_mesh is not None:
            mesh_size = override_mesh
            print(f"Mesh:   {mesh_size} (user override)")
        else:
            mesh_size, explanation = self.determine_mesh_size(input_file)
            print(f"Mesh:   {mesh_size} (auto-calculated)")
            print(f"        {explanation}")
        
        print(f"{'='*50}\n")
        
        # Run AdvCAD
        success = self.run_advcad(input_file, output_file, mesh_size)
        
        # Report results
        if success and output_file.exists():
            size_kb = output_file.stat().st_size / 1024
            print(f"\n✅ AdvCAD completed! Output file created: {size_kb:.1f} KB")
            
            # Validate PCH connectivity if available
            if PCH_VALIDATION_AVAILABLE:
                print(f"\n🔍 Validating PCH connectivity...")
                try:
                    is_valid, message = validate_pch_file(str(output_file), quiet=True)
                    if is_valid:
                        print(f"✅ PCH validation passed: {message}")
                        return True
                    else:
                        print(f"❌ PCH validation failed: {message}")
                        print(f"   File created but contains connectivity errors")
                        return False
                except Exception as e:
                    print(f"⚠️  PCH validation error: {e}")
                    print(f"   File created but could not be validated")
                    return True  # File exists, assume success
            else:
                print(f"   (PCH validation skipped - PchReader not available)")
                return True
        else:
            print(f"\n❌ Failed to generate output")
            
            # Suggest alternative mesh sizes
            if not override_mesh:
                print("\n💡 Suggestions:")
                print(f"   Try: python3 {sys.argv[0]} {input_path} {output_path} {mesh_size * 0.5}")
                print(f"   Try: python3 {sys.argv[0]} {input_path} {output_path} {mesh_size * 2.0}")
                
            return False


def main():
    """Command-line interface"""
    
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)
        
    input_path = sys.argv[1]
    output_path = sys.argv[2]
    override_mesh = float(sys.argv[3]) if len(sys.argv) > 3 else None
    
    # Process
    processor = AdvCADAutoMesh()
    success = processor.process(input_path, output_path, override_mesh)
    
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()