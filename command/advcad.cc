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


WH_GM3D_Body* TheSolidModel;
WH_TPL3D_PolyBody* TheTopology;
WH_MG3D_MeshGenerator* TheMeshGenerator;

void MakePatch 
(const string& geometryFileName,
 double patchSize)
{
  try {
    cerr << "Loading geometry file..." << endl;
    TheSolidModel 
      = WH_GM3D_IO::createBodyFromFile (geometryFileName);
    cerr << "Converting to topology..." << endl;
    TheTopology
      = WH_TPL3D_Converter_GM3D::createBody (TheSolidModel);
    cerr << "Creating mesh generator..." << endl;
    TheMeshGenerator 
      = new WH_MG3D_MeshGenerator (TheTopology->volume_s ()[0]);
    cerr << "Setting tetrahedron size..." << endl;
    TheMeshGenerator->setTetrahedronSize (patchSize);
    cerr << "Generating patch..." << endl;
    TheMeshGenerator->generatePatch ();
    cerr << "Patch generation completed successfully!" << endl;
  } catch (const std::exception& e) {
    cerr << "ERROR: Geometric processing failed: " << e.what() << endl;
    cerr << "This may be due to:" << endl;
    cerr << "  - Invalid geometry in model file" << endl;
    cerr << "  - Boolean operation complexity" << endl;
    cerr << "  - Mesh generation parameters" << endl;
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
  bool toOutputPcm = false;//Added 2006/03/19 A.Miyoshi
  if (argc == 2) {
    if (strcmp (argv[1], "-v") == 0 || strcmp (argv[1], "-version") == 0) {
      cerr << "advcad 0.11\n";//2007/07/27 A.Miyoshi
      exit (0);
    } else {
      //cerr << " argc= " << argc << endl;
      cerr << " Usage : advcad \n"
           << "     geometry_file_name patch_file_name patch_size [-pcm]\n";
      exit (1);
    }
  } else if (argc == 5) {//Added 2006/03/19 A.Miyoshi
    if (strcmp (argv[4], "-pcm") == 0){
      toOutputPcm = true;
    }else{
      //cerr << " argc= " << argc << endl;
      cerr << " Usage : advcad \n"
           << "     geometry_file_name patch_file_name patch_size [-pcm]\n";
      exit (1);
    }
  } else if (argc != 4) {
    //cerr << " argc= " << argc << endl;
    cerr << " Usage : advcad \n"
         << "     geometry_file_name patch_file_name patch_size [-pcm]\n";
    exit (1);
  }

  string geometryFileName = argv[1];
  string patchFileName = argv[2];
  double patchSize = atof (argv[3]);

  try {
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
