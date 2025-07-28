/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for mg3d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_MG3D_BASE
#include <WH/mg3d_base.h>
#define WH_INCLUDED_WH_MG3D_BASE
#endif

template <class Type> class WH_Bucket3D;
class WH_InOutChecker3D;
class WH_MG3D_FaceMeshGenerator;
class WH_DLN3D_Triangulator_MG3D;

class WH_MG3D_MeshGenerator;

class WH_MG3D_MeshGenerator {
 public:
  WH_MG3D_MeshGenerator 
    (WH_TPL3D_Volume_A* volume);
  virtual ~WH_MG3D_MeshGenerator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setTetrahedronSize (double size);
  
  virtual void generateMesh ();

  virtual void generatePatch ();

  double tetrahedronSize () const;

  WH_TPL3D_Volume_A* volume () const;
  
  const vector<WH_MG3D_Node*>& node_s () const;

  WH_Vector3D minRange () const;
  WH_Vector3D maxRange () const;
  
  WH_Bucket3D<WH_MG3D_Node>* nodeBucket () const;

  const vector<WH_MG3D_OriginalBoundaryEdgeSegment*>& obeSeg_s () const;
  
  WH_Bucket3D<WH_MG3D_OriginalBoundaryEdgeSegment>* obeSegBucket () const;
  
  const vector<WH_MG3D_OriginalBoundaryFaceTriangle*>& obfTri_s () const;
  
  WH_Bucket3D<WH_MG3D_OriginalBoundaryFaceTriangle>* obfTriBucket () const;
  
  WH_MG3D_FaceMeshGenerator* faceMeshGenerator () const;
  
  WH_InOutChecker3D* inOutChecker () const;
  
  WH_DLN3D_Triangulator_MG3D* volumeTriangulator () const;
  
  const vector<WH_MG3D_Tetrahedron*>& tetrahedron_s () const;
  
  const vector<WH_MG3D_FinalBoundaryFaceTriangle*>& fbfTri_s () const;

  virtual WH_MG3D_Node* findNodeOnVertex 
    (WH_TPL3D_Vertex_A* vertex) const;

  virtual bool hasNodeNear 
    (const WH_Vector3D& position, double range) const;

  virtual WH_MG3D_Node* nodeAt 
    (const WH_Vector3D& position) const;

  virtual void addNode 
    (WH_MG3D_Node* node);

  /* derived */
  
 protected:
  WH_TPL3D_Volume_A* _volume;

  bool _isDone;

  bool _rangeIsSet;

  double _tetrahedronSize;
  
  vector<WH_MG3D_Node*> _node_s;  /* OWN */

  WH_Vector3D _minRange;
  WH_Vector3D _maxRange;

  WH_Bucket3D<WH_MG3D_Node>* _nodeBucket;  /* OWN */
  
  vector<WH_MG3D_OriginalBoundaryEdgeSegment*> 
    _obeSeg_s;  /* OWN */
  
  WH_Bucket3D<WH_MG3D_OriginalBoundaryEdgeSegment>* 
    _obeSegBucket;  /* OWN */
  
  vector<WH_MG3D_OriginalBoundaryFaceTriangle*> 
    _obfTri_s;  /* OWN */

  WH_Bucket3D<WH_MG3D_OriginalBoundaryFaceTriangle>* 
    _obfTriBucket;  /* OWN */
  
  WH_MG3D_FaceMeshGenerator* 
    _faceMeshGenerator;  /* OWN, TEMPORARY */
  
  WH_InOutChecker3D* _inOutChecker;  /* OWN */
  
  WH_DLN3D_Triangulator_MG3D* 
    _volumeTriangulator;  /* OWN, TEMPORARY */
  
  vector<WH_MG3D_Tetrahedron*> _tetrahedron_s;  /* OWN */

  vector<WH_MG3D_FinalBoundaryFaceTriangle*> 
    _fbfTri_s;  /* OWN */

  /* base */
  virtual void getBucketParameters 
    (const WH_Vector3D& minRange, 
     const WH_Vector3D& maxRange, 
     double cellSize, 
     WH_Vector3D& extendedMinRange_OUT, 
     WH_Vector3D& extendedMaxRange_OUT, 
     int& xCells_OUT, int& yCells_OUT, int& zCells_OUT) const;

  virtual void addObeSeg 
    (WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg);
  
  virtual void addObfTri 
    (WH_MG3D_OriginalBoundaryFaceTriangle* obfTri);

  virtual void addTetrahedron 
    (WH_MG3D_Tetrahedron* tetra);

  virtual void addFbfTri 
    (WH_MG3D_FinalBoundaryFaceTriangle* fbfTri);

  virtual void generateNodesOnVertex 
    (WH_TPL3D_Vertex_A* vertex);

  virtual void generateNodesOnVertexs ();

  virtual void generateMeshAlongEdge 
    (WH_TPL3D_Edge_A* edge);

  virtual void generateMeshAlongEdges ();
  
  virtual void generateMeshOverFace 
    (WH_TPL3D_Face_A* face);
  
  virtual void generateMeshOverFaces ();

  virtual void createInOutChecker ();

  virtual void createNodeBucket ();

  virtual void createObeSegBucket ();

  virtual void createObfTriBucket ();

  virtual void getNodeRange 
    (WH_Vector3D& minRange_OUT, WH_Vector3D& maxRange_OUT) const;

  virtual void setRange ();

  virtual void tryVolumeNodeNearbyBoundary 
    (const WH_Vector3D& position, 
     WH_MG3D_OriginalBoundaryFaceTriangle* obfTri);

  virtual void generateNodesNearbyBoundary ();

  virtual void generateNodesOverVolume ();
  
  virtual void generateTetrahedronsOverVolume ();

  virtual void deleteOutsideVolumeNodes ();

  virtual void collectFinalBoundaryFaceTriangles ();

  virtual void generateSecondOrderNodes ();

  virtual void setNodeId ();

  /* derived */
  
};
