/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 Copyright (C) 2006-2007 ADVENTURE Project 
 All Rights Reserved
 *********************************************************************/

/**
 * advcad.cc 
 * Ver. 0.11b 2007/07/27
 * Added a capability to output in pcm format as an option.
 * Corrected connectivity order of output pch file.
 * Corrected Makefile so that undefined reference link error will not occur.
 */

#include <WH/gm3d_io.h>
#include <WH/gm3d_tpl3d.h>
#include <WH/mg3d.h>
#include <WH/common.h>
#include <WH/geometry_analyzer.h>
#include <WH/debug_levels.h>


WH_GM3D_Body* TheSolidModel;
WH_TPL3D_PolyBody* TheTopology;
WH_MG3D_MeshGenerator* TheMeshGenerator;

void MakePatch 
(const string& geometryFileName,
 double patchSize)
{
  WH_PRINT_VERBOSE("MakePatch started");
  
  try {
    WH_PRINT_NORMAL("Loading geometry file...");
    TheSolidModel 
      = WH_GM3D_IO::createBodyFromFile (geometryFileName);
    
    // Analyze geometry and validate mesh size
    WH_PRINT_NORMAL("Analyzing geometry...");
    WH_GeometryAnalyzer::GeometryMetrics metrics = WH_GeometryAnalyzer::analyze(*TheSolidModel);
    if (g_debugLevel >= WH_DEBUG_VERBOSE) {
        metrics.print();
    }
    
    WH_PRINT_NORMAL("Validating mesh size...");
    double adjustedPatchSize = WH_GeometryAnalyzer::adjustMeshSize(patchSize, metrics);
    if (adjustedPatchSize != patchSize) {
      WH_PRINTF_NORMAL("Mesh size adjusted from %g to %g", patchSize, adjustedPatchSize);
      patchSize = adjustedPatchSize;
    }
    
    if (!WH_GeometryAnalyzer::isMeshSizeAppropriate(patchSize, metrics)) {
      WH_PRINT_WARNING("Mesh size may cause triangulation problems.");
      WH_PRINTF_WARNING("Recommended mesh size range: [%g, %g]", 
                       metrics.minimumSafeMeshSize, metrics.maximumUsefulMeshSize);
    } else {
      WH_PRINT_VERBOSE("Mesh size validation passed.");
    }
    
    WH_PRINT_NORMAL("Converting to topology...");
    TheTopology
      = WH_TPL3D_Converter_GM3D::createBody (TheSolidModel);
    WH_PRINT_VERBOSE("Creating mesh generator...");
    TheMeshGenerator 
      = new WH_MG3D_MeshGenerator (TheTopology->volume_s ()[0]);
    WH_PRINT_VERBOSE("Setting tetrahedron size...");
    TheMeshGenerator->setTetrahedronSize (patchSize);
    WH_PRINT_NORMAL("Generating patch...");
    TheMeshGenerator->generatePatch ();
    if (g_debugLevel == WH_DEBUG_SILENT) {
      // For Level 0: Just report success with triangle count
      cout << "Success: " << TheMeshGenerator->obfTri_s().size() << " triangles" << endl;
    } else {
      WH_PRINT_NORMAL("Patch generation completed successfully!");
    }
  } catch (const std::exception& e) {
    if (g_debugLevel == WH_DEBUG_SILENT) {
      cerr << "Failed: " << e.what() << endl;
    } else {
      cerr << "ERROR: Geometric processing failed: " << e.what() << endl;
      cerr << "This may be due to:" << endl;
      cerr << "  - Invalid geometry in model file" << endl;
      cerr << "  - Boolean operation complexity" << endl;
      cerr << "  - Mesh generation parameters (try mesh size in range [" 
           << (TheSolidModel ? WH_GeometryAnalyzer::analyze(*TheSolidModel).minimumSafeMeshSize : 0.001)
           << ", " << (TheSolidModel ? WH_GeometryAnalyzer::analyze(*TheSolidModel).maximumUsefulMeshSize : 1.0)
           << "])" << endl;
    }
    throw;
  }
}

void WritePatch 
(const string& patchFileName, bool toOutputPcm)//2006/03/19 A.Miyoshi
{
  ofstream out (patchFileName.c_str ());
  WH_ASSERT(out);

  int nNodes = TheMeshGenerator->node_s ().size ();
  //Next 3 lines added 2006/03/19 A.Miyoshi
  if(toOutputPcm){
    out << nNodes << " 0 1" << endl;
  }else{
    out << nNodes << endl;
  }//Added 2006/03/19 A.Miyoshi

  for (vector<WH_MG3D_Node*>::const_iterator 
   i_node = TheMeshGenerator->node_s ().begin ();
       i_node != TheMeshGenerator->node_s ().end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);

    WH_ASSERT(node_i->topologyType () == WH_MG3D_Node::ON_VERTEX
        || node_i->topologyType () == WH_MG3D_Node::ON_EDGE
        || node_i->topologyType () == WH_MG3D_Node::ON_FACE);
    
    WH_Vector3D position = node_i->position ();
    out << position.x << " " 
      << position.y << " " 
      << position.z << endl;
  }

  int nTriangles = TheMeshGenerator->obfTri_s ().size ();
  out << nTriangles << endl;

  for (vector<WH_MG3D_OriginalBoundaryFaceTriangle*>::const_iterator 
   i_obfTri = TheMeshGenerator->obfTri_s ().begin ();
       i_obfTri != TheMeshGenerator->obfTri_s ().end ();
       i_obfTri++) {
    WH_MG3D_OriginalBoundaryFaceTriangle* obfTri_i = (*i_obfTri);

    WH_MG3D_Node* node0 = obfTri_i->node0 ();
    WH_MG3D_Node* node1 = obfTri_i->node1 ();
    WH_MG3D_Node* node2 = obfTri_i->node2 ();
    //2006/03/19 A.Miyoshi changed order nodes from node0, node1, node2 
    //to comform with pch/pcm format
    out << node2->id () << " "  
        << node1->id () << " "  
        << node0->id () << endl;
  }
}



int main (int argc, char* argv[])
{
  // Initialize debug system
  WH_InitDebugLevel();
  
  WH_PRINTF_VERBOSE("main() started, argc=%d", argc);
  
  // Parse arguments including debug level
  bool toOutputPcm = false;//Added 2006/03/19 A.Miyoshi
  int argOffset = 0;
  
  // Check for debug argument first
  if (argc > 1 && strncmp(argv[1], "--debug=", 8) == 0) {
    int debugLevel = atoi(argv[1] + 8);
    WH_SetDebugLevel(debugLevel);
    WH_PRINTF_VERBOSE("Debug level set to %d (%s)", debugLevel, WH_GetDebugLevelName(debugLevel));
    argOffset = 1; // Skip debug argument
  }
  
  int effectiveArgc = argc - argOffset;
  
  // After debug parsing, we need 4 arguments: program + 3 args
  int remainingArgs = argc - argOffset - 1; // Subtract program name
  
  WH_PRINTF_VERBOSE("Debug: argc=%d, argOffset=%d, remainingArgs=%d", argc, argOffset, remainingArgs);
  
  if (remainingArgs == 1) {
    if (strcmp (argv[1 + argOffset], "-v") == 0 || strcmp (argv[1 + argOffset], "-version") == 0) {
      cerr << "advcad 0.12b\n";
      exit (0);
    } else {
      cerr << " Usage : advcad [--debug=N] \n"
           << "     geometry_file_name patch_file_name patch_size [-pcm]\n"
           << "     Debug levels: 0=silent, 1=normal, 2=verbose, 3=trace\n";
      exit (1);
    }
  } else if (remainingArgs == 4) {//Added 2006/03/19 A.Miyoshi
    if (strcmp (argv[4 + argOffset], "-pcm") == 0){
      toOutputPcm = true;
    }else{
      cerr << " Usage : advcad [--debug=N] \n"
           << "     geometry_file_name patch_file_name patch_size [-pcm]\n"
           << "     Debug levels: 0=silent, 1=normal, 2=verbose, 3=trace\n";
      exit (1);
    }
  } else if (remainingArgs != 3) {
    cerr << " Usage : advcad [--debug=N] \n"
         << "     geometry_file_name patch_file_name patch_size [-pcm]\n"
         << "     Debug levels: 0=silent, 1=normal, 2=verbose, 3=trace\n";
    exit (1);
  }

  string geometryFileName = argv[1 + argOffset];
  string patchFileName = argv[2 + argOffset];
  double patchSize = atof (argv[3 + argOffset]);

  try {
    WH_PRINTF_VERBOSE("About to call MakePatch with file: %s size: %g", geometryFileName.c_str(), patchSize);
    cerr.flush();
    MakePatch (geometryFileName, patchSize);
    WritePatch (patchFileName, toOutputPcm);//2006/03/19 A.Miyoshi
  } catch (const std::exception& e) {
    cerr << "FATAL ERROR: " << e.what() << endl;
    cerr << "Processing aborted for model: " << geometryFileName << endl;
    return 1;
  } catch (...) {
    cerr << "FATAL ERROR: Unknown exception occurred during processing" << endl;
    cerr << "Processing aborted for model: " << geometryFileName << endl;
    return 1;
  }
  
  return 0;
}
