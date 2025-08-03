/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* mg3d.cc : 3-D tetrahedron mesh generation */

#include "mg3d.h"
#include "field3d.h"
#include "bucket3d.h"
#include "inout3d.h"
#include "tetrahedron3d.h"
#include "mg3d_delaunay2d.h"
#include "mg3d_delaunay3d.h"
#include "robust_predicates.h"



/* class WH_MG3D_MeshGenerator */

WH_MG3D_MeshGenerator
::WH_MG3D_MeshGenerator 
(WH_TPL3D_Volume_A* volume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(volume != WH_NULL);
  
  _isDone = false;
  _rangeIsSet = false;

  _volume = volume;
  _minRange = WH_Vector3D (0, 0, 0);
  _maxRange = WH_Vector3D (0, 0, 0);
  _tetrahedronSize = 1.0;
  _nodeBucket = WH_NULL;
  _obeSegBucket = WH_NULL;
  _obfTriBucket = WH_NULL;
  _faceMeshGenerator = WH_NULL;
  _inOutChecker = WH_NULL;
  _volumeTriangulator = WH_NULL;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_MeshGenerator
::~WH_MG3D_MeshGenerator () 
{
  WH_ASSERT(_faceMeshGenerator == WH_NULL);

  delete _nodeBucket;
  delete _obeSegBucket;
  delete _obfTriBucket;
  delete _inOutChecker;
  delete _volumeTriangulator;

  WH_T_Delete (_node_s);
  WH_T_Delete (_obeSeg_s);
  WH_T_Delete (_obfTri_s);
  WH_T_Delete (_tetrahedron_s);
  WH_T_Delete (_fbfTri_s);
}

bool WH_MG3D_MeshGenerator
::checkInvariant () const
{
  if (_rangeIsSet) {
    WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
    WH_ASSERT(this->nodeBucket () != WH_NULL);
    WH_ASSERT(this->obeSegBucket () != WH_NULL);
    WH_ASSERT(this->obfTriBucket () != WH_NULL);
    WH_ASSERT(this->inOutChecker () != WH_NULL);
  }

  WH_ASSERT(this->volume () != WH_NULL);
  WH_ASSERT(WH_lt (0, this->tetrahedronSize ()));
  WH_ASSERT(this->faceMeshGenerator () != WH_NULL
	    || this->faceMeshGenerator () == WH_NULL);
  WH_ASSERT(this->volumeTriangulator () != WH_NULL
	    || this->volumeTriangulator () == WH_NULL);

  return true;
}

bool WH_MG3D_MeshGenerator
::assureInvariant () const
{
  this->checkInvariant ();

  if (_faceMeshGenerator != WH_NULL) {
    _faceMeshGenerator->assureInvariant ();
  }
  if (_inOutChecker != WH_NULL) {
    _inOutChecker->assureInvariant ();
  }
  if (_volumeTriangulator != WH_NULL) {
    _volumeTriangulator->assureInvariant ();
  }
  WH_T_AssureInvariant (_node_s);
  WH_T_AssureInvariant (_obeSeg_s);
  WH_T_AssureInvariant (_obfTri_s);
  WH_T_AssureInvariant (_tetrahedron_s);
  WH_T_AssureInvariant (_fbfTri_s);

  if (_isDone) {
    for (vector<WH_MG3D_Node*>::const_iterator 
	   i_node = this->node_s ().begin ();
	 i_node != this->node_s ().end ();
	 i_node++) {
      WH_MG3D_Node* node_i = (*i_node);
      
      WH_ASSERT(node_i->topologyType () == WH_MG3D_Node::ON_VERTEX
		|| node_i->topologyType () == WH_MG3D_Node::ON_EDGE
		|| node_i->topologyType () == WH_MG3D_Node::ON_FACE
		|| node_i->topologyType () == WH_MG3D_Node::INSIDE_VOLUME);
    }

    for (vector<WH_MG3D_Tetrahedron*>::const_iterator 
	   i_tetra = this->tetrahedron_s ().begin ();
	 i_tetra != this->tetrahedron_s ().end ();
	 i_tetra++) {
      WH_MG3D_Tetrahedron* tetra_i = (*i_tetra);

      WH_ASSERT(tetra_i->hasAnySecondOrderNode ());
    }

    for (vector<WH_MG3D_FinalBoundaryFaceTriangle*>::const_iterator 
	   i_fbfTri = this->fbfTri_s ().begin ();
	 i_fbfTri != this->fbfTri_s ().end ();
	 i_fbfTri++) {
      WH_MG3D_FinalBoundaryFaceTriangle* fbfTri_i = (*i_fbfTri);
      
      WH_ASSERT(fbfTri_i->hasAnySecondOrderNode ());
    }    
  }

  return true;
}

void WH_MG3D_MeshGenerator
::setTetrahedronSize (double size)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (0, size));
  
  _tetrahedronSize = size;
}

void WH_MG3D_MeshGenerator
::generateMesh ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isDone);

  cerr << "DEBUG: About to call generateNodesOnVertexs" << endl;
  cerr.flush();

  this->generateNodesOnVertexs ();

  cout << " generateNodesOnVertexs " << endl;

  this->generateMeshAlongEdges ();

  cout << " generateMeshAlongEdges " << endl;

  this->generateMeshOverFaces ();

  cout << " generateMeshOverFaces " << endl;

  this->setRange ();

  cerr << " setRange " << endl;

  this->generateNodesNearbyBoundary ();

  cerr << " generateNodesNearbyBoundary " << endl;

  this->generateNodesOverVolume ();

  cerr << " generateNodesOverVolume " << endl;

  this->generateTetrahedronsOverVolume ();
  this->deleteOutsideVolumeNodes ();
  this->collectFinalBoundaryFaceTriangles ();

  cerr << " generateTetrahedrons " << endl;

  this->generateSecondOrderNodes ();
  this->setNodeId ();

  cerr << " generateSecondOrderNodes " << endl;

  _isDone = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_isDone);
#endif
}

void WH_MG3D_MeshGenerator
::generatePatch ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isDone);

  this->generateNodesOnVertexs ();

  cout << " generateNodesOnVertexs " << endl;

  this->generateMeshAlongEdges ();

  cout << " generateMeshAlongEdges " << endl;

  this->generateMeshOverFaces ();
  this->setNodeId ();

  cout << " generateMeshOverFaces " << endl;

  _isDone = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_isDone);
#endif
}

double WH_MG3D_MeshGenerator
::tetrahedronSize () const
{
  return _tetrahedronSize;
}

WH_TPL3D_Volume_A* WH_MG3D_MeshGenerator
::volume () const
{
  return _volume;
}
  
const vector<WH_MG3D_Node*>& WH_MG3D_MeshGenerator
::node_s () const
{
  return _node_s;
}

WH_Bucket3D<WH_MG3D_Node>* WH_MG3D_MeshGenerator
::nodeBucket () const
{
  return _nodeBucket;
}

const vector<WH_MG3D_OriginalBoundaryEdgeSegment*>& 
WH_MG3D_MeshGenerator
::obeSeg_s () const
{
  return _obeSeg_s;
}

WH_Bucket3D<WH_MG3D_OriginalBoundaryEdgeSegment>* 
WH_MG3D_MeshGenerator
::obeSegBucket () const
{
  return _obeSegBucket;
}

const vector<WH_MG3D_OriginalBoundaryFaceTriangle*>& 
WH_MG3D_MeshGenerator
::obfTri_s () const
{
  return _obfTri_s;
}

WH_Bucket3D<WH_MG3D_OriginalBoundaryFaceTriangle>* 
WH_MG3D_MeshGenerator
::obfTriBucket () const
{
  return _obfTriBucket;
}

WH_MG3D_FaceMeshGenerator* WH_MG3D_MeshGenerator
::faceMeshGenerator () const
{
  return _faceMeshGenerator;
}

WH_InOutChecker3D* WH_MG3D_MeshGenerator
::inOutChecker () const
{
  return _inOutChecker;
}

WH_Vector3D WH_MG3D_MeshGenerator
::minRange () const
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);

  return _minRange;
}

WH_Vector3D WH_MG3D_MeshGenerator
::maxRange () const
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);

  return _maxRange;
}

WH_DLN3D_Triangulator_MG3D* WH_MG3D_MeshGenerator
::volumeTriangulator () const
{
  return _volumeTriangulator;
}

const vector<WH_MG3D_Tetrahedron*>& WH_MG3D_MeshGenerator
::tetrahedron_s () const
{
  return _tetrahedron_s;
}

const vector<WH_MG3D_FinalBoundaryFaceTriangle*>& 
WH_MG3D_MeshGenerator
::fbfTri_s () const
{
  return _fbfTri_s;
}

void WH_MG3D_MeshGenerator
::getBucketParameters 
(const WH_Vector3D& minRange, 
 const WH_Vector3D& maxRange, 
 double cellSize, 
 WH_Vector3D& extendedMinRange_OUT, 
 WH_Vector3D& extendedMaxRange_OUT, 
 int& xCells_OUT, int& yCells_OUT, int& zCells_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(WH_lt (0, cellSize));

  extendedMinRange_OUT = minRange;
  extendedMaxRange_OUT = maxRange;

  /* MAGIC NUMBER : 11, 13 */
  WH_Vector3D size = maxRange - minRange;
  extendedMinRange_OUT -= size / 11;
  extendedMaxRange_OUT += size / 13;
  WH_ASSERT(WH_le (extendedMinRange_OUT, extendedMaxRange_OUT));
  
  WH_Vector3D extendedSize 
    = extendedMaxRange_OUT - extendedMinRange_OUT;
  WH_ASSERT(WH_le (WH_Vector3D::zero (), extendedSize));

  if (WH_eq (0, extendedSize.x)) {
    extendedMinRange_OUT.x -= 1.0;
    extendedMaxRange_OUT.x += 1.0;
    xCells_OUT = 1;
  } else {
    xCells_OUT = (int)ceil (extendedSize.x / cellSize + WH::eps);
    if (xCells_OUT / 2 == 0) xCells_OUT++;
  }

  if (WH_eq (0, extendedSize.y)) {
    extendedMinRange_OUT.y -= 1.0;
    extendedMaxRange_OUT.y += 1.0;
    yCells_OUT = 1;
  } else {
    yCells_OUT = (int)ceil (extendedSize.y / cellSize + WH::eps);
    if (yCells_OUT / 2 == 0) yCells_OUT++;
  }

  if (WH_eq (0, extendedSize.z)) {
    extendedMinRange_OUT.z -= 1.0;
    extendedMaxRange_OUT.z += 1.0;
    zCells_OUT = 1;
  } else {
    zCells_OUT = (int)ceil (extendedSize.z / cellSize + WH::eps);
    if (zCells_OUT / 2 == 0) zCells_OUT++;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt (extendedMinRange_OUT, extendedMaxRange_OUT));
  WH_ASSERT(WH_lt (extendedMinRange_OUT, minRange));
  WH_ASSERT(WH_lt (maxRange, extendedMaxRange_OUT));
  WH_ASSERT(0 < xCells_OUT);
  WH_ASSERT(0 < yCells_OUT);
  WH_ASSERT(0 < zCells_OUT);
#endif
}

void WH_MG3D_MeshGenerator
::addNode (WH_MG3D_Node* node)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);

  _node_s.push_back (node);

  if (_nodeBucket != WH_NULL) {
    WH_ASSERT(_rangeIsSet);
    _nodeBucket->addItemLastOn 
      (node->position (), node);
  }
}

WH_MG3D_Node* WH_MG3D_MeshGenerator
::findNodeOnVertex (WH_TPL3D_Vertex_A* vertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
  
  WH_MG3D_Node* result = WH_NULL;

  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = _node_s.begin ();
       i_node != _node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    if (node_i->vertex () == vertex) {
      result = node_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
  if (result != WH_NULL) {
    WH_ASSERT(result->topologyType () == WH_MG3D_Node::ON_VERTEX);
    WH_ASSERT(result->vertex () == vertex);
  }
#endif

  return result;
}

bool WH_MG3D_MeshGenerator
::hasNodeNear 
(const WH_Vector3D& position, double range) const
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  WH_ASSERT(WH_le (0, range));
  
  bool result = false;
  
  vector<WH_MG3D_Node*> node_s;
  _nodeBucket->getItemsWithin
    (position - WH_Vector3D (range, range, range), 
     position + WH_Vector3D (range, range, range), 
     node_s);
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = node_s.begin ();
       i_node != node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    double dist 
      = WH_distance (node_i->position (), position);
    if (WH_le (dist, range)) {
      result = true;
      break;
    }
  }
  
  return result;
}

WH_MG3D_Node* WH_MG3D_MeshGenerator
::nodeAt (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  
  WH_MG3D_Node* result = WH_NULL;
  
  vector<WH_MG3D_Node*> node_s;
  _nodeBucket->getItemsOn (position, 
			   node_s);
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = node_s.begin ();
       i_node != node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    if (WH_eq (node_i->position (), position)) {
      result = node_i;
      break;
    }
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

void WH_MG3D_MeshGenerator
::addObeSeg 
(WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg)
{
  /* PRE-CONDITION */
  WH_ASSERT(obeSeg != WH_NULL);

  _obeSeg_s.push_back (obeSeg);
}

void WH_MG3D_MeshGenerator
::addObfTri 
(WH_MG3D_OriginalBoundaryFaceTriangle* obfTri)
{
  /* PRE-CONDITION */
  WH_ASSERT(obfTri != WH_NULL);

  _obfTri_s.push_back (obfTri);
}

void WH_MG3D_MeshGenerator
::addTetrahedron 
(WH_MG3D_Tetrahedron* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);

  _tetrahedron_s.push_back (tetra);
}

void WH_MG3D_MeshGenerator
::addFbfTri 
(WH_MG3D_FinalBoundaryFaceTriangle* fbfTri)
{
  /* PRE-CONDITION */
  WH_ASSERT(fbfTri != WH_NULL);

  _fbfTri_s.push_back (fbfTri);
}

void WH_MG3D_MeshGenerator
::generateNodesOnVertex (WH_TPL3D_Vertex_A* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
  
  WH_MG3D_Node* node 
    = new WH_MG3D_Node (vertex->point ());
  WH_ASSERT(node != WH_NULL);
  node->putOnVertex (vertex);
  this->addNode (node);
}

void WH_MG3D_MeshGenerator
::generateNodesOnVertexs ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->node_s ().size () == 0);
  
  cerr << "DEBUG: generateNodesOnVertexs start" << endl;
  
  vector<WH_TPL3D_Vertex_A*> vertex_s;
  this->volume ()->getVertexs 
    (vertex_s);
    
  cerr << "DEBUG: got " << vertex_s.size() << " vertices" << endl;

  for (vector<WH_TPL3D_Vertex_A*>::const_iterator 
	 i_vertex = vertex_s.begin ();
       i_vertex != vertex_s.end ();
       i_vertex++) {
    WH_TPL3D_Vertex_A* vertex_i = (*i_vertex);
    
    static int vertexCount = 0;
    cerr << "DEBUG: processing vertex #" << vertexCount++ << endl;
    
    this->generateNodesOnVertex (vertex_i);
  }
}

void WH_MG3D_MeshGenerator
::generateMeshAlongEdge (WH_TPL3D_Edge_A* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  
  // Debug: Track where crash occurs
  static int edgeCount = 0;
  cerr << "DEBUG: generateMeshAlongEdge entry #" << edgeCount++ << endl;
  
  /* MAGIC NUMBER */
  double interval = _tetrahedronSize * 1.0;

  double length = edge->length ();
  int uDivs = (int)ceil (length / interval + WH::eps);
  if (uDivs <= 0) uDivs = 1;

  double uStart = edge->parameter0 ();
  double uEnd = edge->parameter1 ();

  double prevU = uStart;

  WH_Vector3D prevPoint = edge->curve ()->positionAt (prevU);
  
  // Detect if robust comparison is needed: small scale + complex geometry
  double geometryScale = max(edge->vertex0()->point().length(), edge->vertex1()->point().length());
  bool hasSmallScale = (geometryScale < 1.0);
  
  // Detect complexity: use edge length relative to tetrahedron size as complexity indicator
  bool hasComplexGeometry = (edge->length() < _tetrahedronSize * 0.1);
  
  bool useRobustComparison = (hasSmallScale && hasComplexGeometry);
  
  // Debug output to see if robust predicates are triggering
  static int debugCount = 0;
  if (debugCount < 5) {
    cerr << "DEBUG: geometryScale=" << geometryScale 
         << " hasSmallScale=" << hasSmallScale
         << " edgeLength=" << edge->length()
         << " tetraSize=" << _tetrahedronSize
         << " hasComplexGeometry=" << hasComplexGeometry
         << " useRobust=" << useRobustComparison << endl;
    debugCount++;
  }
  
  if (useRobustComparison) {
    // Use robust 3D predicate for small-scale complex geometries
    WH_ASSERT(WH_RobustPredicates::points_equal_robust_3d(prevPoint, edge->vertex0()->point()));
  } else {
    // Use standard comparison for simple or large-scale geometries
    WH_ASSERT(WH_eq (prevPoint, edge->vertex0 ()->point ()));
  }

  /* get the node on <edge->vertex0 ()> */
  WH_MG3D_Node* prevNode 
    = this->findNodeOnVertex (edge->vertex0 ());
  WH_ASSERT(prevNode != WH_NULL);

  for (int i_u = 0; i_u < uDivs; i_u++) {
    double currentU 
      = uStart + (uEnd - uStart) / uDivs * (i_u + 1);

    WH_Vector3D currentPoint 
      = edge->curve ()->positionAt (currentU);

    WH_MG3D_Node* currentNode = WH_NULL; 
    if (i_u == uDivs - 1) {

      /* get the node on <edge->vertex1 ()> */
      if (useRobustComparison) {
        WH_ASSERT(WH_RobustPredicates::points_equal_robust_3d(currentPoint, edge->vertex1()->point()));
      } else {
        WH_ASSERT(WH_eq (currentPoint, edge->vertex1 ()->point ()));
      }
      currentNode 
	= this->findNodeOnVertex (edge->vertex1 ());
      WH_ASSERT(currentNode != WH_NULL);

    } else {

      /* generate a new node on <edge> */
      WH_MG3D_Node* node 
	= new WH_MG3D_Node (currentPoint);
      WH_ASSERT(node != WH_NULL);
      node->putOnEdge (edge);
      this->addNode (node);
      
      currentNode = node;
    }
    WH_ASSERT(currentNode != WH_NULL);

    /* generate a new edge segment between <prevNode> and
       <currentNode> */
    WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg
      = new WH_MG3D_OriginalBoundaryEdgeSegment 
      (prevNode, currentNode, edge);
    WH_ASSERT(obeSeg != WH_NULL);
    this->addObeSeg (obeSeg);

    prevU = currentU;
    prevPoint = currentPoint;
    prevNode = currentNode;
  }
}

void WH_MG3D_MeshGenerator
::generateMeshAlongEdges ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->obeSeg_s ().size () == 0);

  cerr << "DEBUG: generateMeshAlongEdges start" << endl;
  
  vector<WH_TPL3D_Edge_A*> edge_s;
  this->volume ()->getEdges 
    (edge_s);
    
  cerr << "DEBUG: got " << edge_s.size() << " edges" << endl;

  for (vector<WH_TPL3D_Edge_A*>::const_iterator 
	 i_edge = edge_s.begin ();
       i_edge != edge_s.end ();
       i_edge++) {
    WH_TPL3D_Edge_A* edge_i = (*i_edge);
    this->generateMeshAlongEdge (edge_i);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < this->obeSeg_s ().size ());
#endif
}

void WH_MG3D_MeshGenerator
::generateMeshOverFace (WH_TPL3D_Face_A* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  WH_ASSERT(this->faceMeshGenerator () == WH_NULL);
  
  cerr << "DEBUG: Creating face mesh generator..." << endl;
  _faceMeshGenerator 
    = new WH_MG3D_FaceMeshGenerator (this, face);
  WH_ASSERT(_faceMeshGenerator != WH_NULL);

  cerr << "DEBUG: Starting face mesh generation..." << endl;
  try {
    _faceMeshGenerator->generateMesh ();
    cerr << "DEBUG: Face mesh generation completed" << endl;
  } catch (const std::exception& e) {
    cerr << "ERROR: Face mesh generation failed: " << e.what() << endl;
    delete _faceMeshGenerator;
    _faceMeshGenerator = WH_NULL;
    throw;
  } catch (...) {
    cerr << "ERROR: Face mesh generation failed with unknown error" << endl;
    delete _faceMeshGenerator;
    _faceMeshGenerator = WH_NULL;
    throw;
  }
  
  WH_ASSERT(_faceMeshGenerator->assureInvariant ());

  cerr << "DEBUG: Generating boundary face triangles..." << endl;
  /* generate original boundary face triangles */
  int triangle_count = 0;
  for (vector<WH_MG3D_FaceTriangle*>::const_iterator 
	 i_tri = _faceMeshGenerator->triangle_s ().begin ();
       i_tri != _faceMeshGenerator->triangle_s ().end ();
       i_tri++) {
    WH_MG3D_FaceTriangle* tri_i = (*i_tri);
    triangle_count++;
    cerr << "DEBUG: Processing triangle " << triangle_count << endl;
    
    try {
      WH_MG3D_Node* node0 = tri_i->node0 ()->node3D ();
      WH_MG3D_Node* node1 = tri_i->node1 ()->node3D ();
      WH_MG3D_Node* node2 = tri_i->node2 ()->node3D ();

      WH_MG3D_OriginalBoundaryFaceTriangle* obfTri
        = new WH_MG3D_OriginalBoundaryFaceTriangle 
        (node0, node1, node2, face);
      this->addObfTri (obfTri);
      cerr << "DEBUG: Triangle " << triangle_count << " processed successfully" << endl;
    } catch (const std::exception& e) {
      cerr << "ERROR: Triangle " << triangle_count << " failed: " << e.what() << endl;
      delete _faceMeshGenerator;
      _faceMeshGenerator = WH_NULL;
      throw;
    }
  }

  cerr << "DEBUG: Cleaning up face mesh generator..." << endl;
  delete _faceMeshGenerator;
  _faceMeshGenerator = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->faceMeshGenerator () == WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::generateMeshOverFaces ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->obfTri_s ().size () == 0);

  int face_count = 0;
  int total_faces = this->volume ()->face_s ().size ();
  cout << "DEBUG: Starting generateMeshOverFaces, total faces: " << total_faces << endl;

  for (vector<WH_TPL3D_Face_A*>::const_iterator 
	 i_face = this->volume ()->face_s ().begin ();
       i_face != this->volume ()->face_s ().end ();
       i_face++) {
    WH_TPL3D_Face_A* face_i = (*i_face);
    face_count++;
    cerr << "DEBUG: Processing face " << face_count << "/" << total_faces << endl;
    
    try {
      this->generateMeshOverFace (face_i);
      cerr << "DEBUG: Face " << face_count << " completed successfully" << endl;
    } catch (const std::exception& e) {
      cerr << "ERROR: Face " << face_count << " failed: " << e.what() << endl;
      throw;
    } catch (...) {
      cerr << "ERROR: Face " << face_count << " failed with unknown error" << endl;
      throw;
    }
  }

  cout << "DEBUG: generateMeshOverFaces completed successfully" << endl;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < this->obfTri_s ().size ());
#endif
}

void WH_MG3D_MeshGenerator
::createInOutChecker ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_rangeIsSet);
  WH_ASSERT(this->inOutChecker () == WH_NULL);
  WH_ASSERT(4 <= this->obfTri_s ().size ());

  /* MAGIC NUMBER */
  double size = _tetrahedronSize * 0.5;
  _inOutChecker = new WH_InOutChecker3D (size);
  WH_ASSERT(_inOutChecker != WH_NULL);
  
  for (vector<WH_MG3D_OriginalBoundaryFaceTriangle*>::const_iterator 
	 i_obfTri = _obfTri_s.begin ();
       i_obfTri != _obfTri_s.end ();
       i_obfTri++) {
    WH_MG3D_OriginalBoundaryFaceTriangle* obfTri_i = (*i_obfTri);

    WH_TPL3D_Face_A* face = obfTri_i->face ();
    if (face->faceType () == WH_TPL3D_Face_A::OUTER_BOUNDARY) {
      /* register triangle into <_inOutChecker> */

      WH_TPL3D_Surface_A* surface = face->surface ();
      WH_Vector3D point0 = obfTri_i->node0 ()->position ();
      WH_Vector3D point1 = obfTri_i->node1 ()->position ();
      WH_Vector3D point2 = obfTri_i->node2 ()->position ();
      WH_Vector3D center = (point0 + point1 + point2) / 3;
      WH_Vector2D centerParam = surface->parameterAt (center);
      WH_Vector3D centerNormal = surface->normalAt (centerParam);
      if (!face->orientationIsSameAsSurfaceNormal ()) {
	centerNormal *= -1.0;
      }

      WH_Triangle3D tri (point0, point1, point2);
      WH_Vector3D normal = tri.plane ().normal ();
      double sp = WH_scalarProduct (normal, centerNormal);
      if (WH_lt (sp, 0)) {
	normal *= -1.0;
      }

      _inOutChecker->addFace (point0, point1, point2, normal);
    }
  }
  
  _inOutChecker->setUp ();
  WH_ASSERT(_inOutChecker->assureInvariant ());

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_inOutChecker != WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::getNodeRange 
(WH_Vector3D& minRange_OUT, WH_Vector3D& maxRange_OUT) const
{
  WH_Vector3D size = _maxRange - _minRange;
  WH_Vector3D outerMargin 
    (_tetrahedronSize, _tetrahedronSize, _tetrahedronSize);
  outerMargin = WH_max (outerMargin, size * 0.1);
  minRange_OUT = _minRange - outerMargin;
  maxRange_OUT = _maxRange + outerMargin;
}

void WH_MG3D_MeshGenerator
::createNodeBucket ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_rangeIsSet);
  WH_ASSERT(4 <= this->node_s ().size ());
  WH_ASSERT(this->nodeBucket () == WH_NULL);
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  WH_Vector3D nodeMinRange, nodeMaxRange;
  this->getNodeRange 
    (nodeMinRange, nodeMaxRange);
  
  WH_Vector3D extendedMinRange, extendedMaxRange;
  int xCells, yCells, zCells;
  this->getBucketParameters 
    (nodeMinRange, nodeMaxRange, _tetrahedronSize,
     extendedMinRange, extendedMaxRange, xCells, yCells, zCells);
  
  _nodeBucket = new WH_Bucket3D<WH_MG3D_Node>
    (extendedMinRange, extendedMaxRange, xCells, yCells, zCells);
  WH_ASSERT(_nodeBucket != WH_NULL);
  
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = _node_s.begin ();
       i_node != _node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    _nodeBucket->addItemLastOn 
      (node_i->position (), node_i);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->nodeBucket () != WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::createObeSegBucket ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_rangeIsSet);
  WH_ASSERT(0 < this->obeSeg_s ().size ());
  WH_ASSERT(this->obeSegBucket () == WH_NULL);
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  WH_Vector3D extendedMinRange, extendedMaxRange;
  int xCells, yCells, zCells;
  this->getBucketParameters 
    (_minRange, _maxRange, _tetrahedronSize,
     extendedMinRange, extendedMaxRange, xCells, yCells, zCells);

  _obeSegBucket = new WH_Bucket3D<WH_MG3D_OriginalBoundaryEdgeSegment>
    (extendedMinRange, extendedMaxRange, xCells, yCells, zCells);
  WH_ASSERT(_obeSegBucket != WH_NULL);
  
  for (vector<WH_MG3D_OriginalBoundaryEdgeSegment*>::const_iterator 
	 i_obeSeg = _obeSeg_s.begin ();
       i_obeSeg != _obeSeg_s.end ();
       i_obeSeg++) {
    WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg_i = (*i_obeSeg);
    _obeSegBucket->addItemLastWithin 
      (obeSeg_i->minRange (), obeSeg_i->maxRange (), obeSeg_i);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->obeSegBucket () != WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::createObfTriBucket ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_rangeIsSet);
  WH_ASSERT(0 < this->obfTri_s ().size ());
  WH_ASSERT(this->obfTriBucket () == WH_NULL);
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  WH_Vector3D extendedMinRange, extendedMaxRange;
  int xCells, yCells, zCells;
  this->getBucketParameters 
    (_minRange, _maxRange, _tetrahedronSize,
     extendedMinRange, extendedMaxRange, xCells, yCells, zCells);

  _obfTriBucket = new WH_Bucket3D<WH_MG3D_OriginalBoundaryFaceTriangle>
    (extendedMinRange, extendedMaxRange, xCells, yCells, zCells);
  WH_ASSERT(_obfTriBucket != WH_NULL);
  
  for (vector<WH_MG3D_OriginalBoundaryFaceTriangle*>::const_iterator 
	 i_obfTri = _obfTri_s.begin ();
       i_obfTri != _obfTri_s.end ();
       i_obfTri++) {
    WH_MG3D_OriginalBoundaryFaceTriangle* obfTri_i = (*i_obfTri);
    _obfTriBucket->addItemLastWithin 
      (obfTri_i->minRange (), obfTri_i->maxRange (), obfTri_i);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->obfTriBucket () != WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::setRange ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_rangeIsSet);
  WH_ASSERT(4 <= this->node_s ().size ());
  WH_ASSERT(6 <= this->obeSeg_s ().size ());
  WH_ASSERT(4 <= this->obfTri_s ().size ());
  WH_ASSERT(this->inOutChecker () == WH_NULL);
  WH_ASSERT(this->nodeBucket () == WH_NULL);
  WH_ASSERT(this->obeSegBucket () == WH_NULL);
  WH_ASSERT(this->obfTriBucket () == WH_NULL);
  
  this->createInOutChecker ();

  /* set range from <_node_s> on vertices, edges and faces */

  _minRange = WH_Vector3D::hugeValue ();
  _maxRange = -WH_Vector3D::hugeValue ();
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = _node_s.begin ();
       i_node != _node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    _minRange = WH_min (node_i->position (), _minRange);
    _maxRange = WH_max (node_i->position (), _maxRange);
  }
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  this->createNodeBucket ();
  this->createObeSegBucket ();
  this->createObfTriBucket ();

  _rangeIsSet = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->inOutChecker () != WH_NULL);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  WH_ASSERT(this->obeSegBucket () != WH_NULL);
  WH_ASSERT(this->obfTriBucket () != WH_NULL);
#endif
}

void WH_MG3D_MeshGenerator
::tryVolumeNodeNearbyBoundary 
(const WH_Vector3D& position, 
 WH_MG3D_OriginalBoundaryFaceTriangle* obfTri)
{
  /* PRE-CONDITION */
  WH_ASSERT(obfTri != WH_NULL);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  
  /* MAGIC NUMBER */
  double range = _tetrahedronSize * 1.0;
  
  bool anyNodeNearIsFound = false;
  
  vector<WH_MG3D_Node*> node_s;
  _nodeBucket->getItemsWithin
    (position - WH_Vector3D (range, range, range), 
     position + WH_Vector3D (range, range, range), 
     node_s);
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = node_s.begin ();
       i_node != node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);

    if (node_i == obfTri->node0 ()
	|| node_i == obfTri->node1 ()
	|| node_i == obfTri->node2 ()) {
      continue;
    }
    double dist 
      = WH_distance (node_i->position (), position);
    switch (node_i->topologyType ()) {
    case WH_MG3D_Node::ON_VERTEX:
    case WH_MG3D_Node::ON_EDGE:
    case WH_MG3D_Node::ON_FACE:
      /* MAGIC NUMBER */
      if (WH_le (dist, _tetrahedronSize * 0.99)) {
	anyNodeNearIsFound = true;
      }
      break;
    case WH_MG3D_Node::INSIDE_VOLUME:
    case WH_MG3D_Node::OUTSIDE_VOLUME:
      /* MAGIC NUMBER */
      if (WH_le (dist, _tetrahedronSize * 0.49)) {
	anyNodeNearIsFound = true;
      }
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
    if (anyNodeNearIsFound) break;
  }

  if (!anyNodeNearIsFound) {
    WH_MG3D_Node* node = new WH_MG3D_Node (position);
    WH_ASSERT(node != WH_NULL);
    
    WH_InOutChecker3D::ContainmentType flag 
      = _inOutChecker->checkContainmentAt (position);
    switch (flag) {
    case WH_InOutChecker3D::IN:
      node->putInsideVolume (_volume);
      break;
    case WH_InOutChecker3D::OUT:
      node->putOutsideVolume ();
      break;
    case WH_InOutChecker3D::ON:
      WH_ASSERT_NO_REACH;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }

    this->addNode (node);
  }
}

void WH_MG3D_MeshGenerator
::generateNodesNearbyBoundary ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  WH_ASSERT(this->inOutChecker () != WH_NULL);
  
  for (vector<WH_MG3D_OriginalBoundaryFaceTriangle*>::const_iterator 
	 i_obfTri = _obfTri_s.begin ();
       i_obfTri != _obfTri_s.end ();
       i_obfTri++) {
    WH_MG3D_OriginalBoundaryFaceTriangle* obfTri_i = (*i_obfTri);
    
    WH_Vector3D point0 = obfTri_i->node0 ()->position ();
    WH_Vector3D point1 = obfTri_i->node1 ()->position ();
    WH_Vector3D point2 = obfTri_i->node2 ()->position ();
    WH_Vector3D center = (point0 + point1 + point2) / 3;
    WH_Triangle3D tri (point0, point1, point2);
    WH_Vector3D normal = tri.plane ().normal ();
    
    double height = _tetrahedronSize * sqrt (6.0) / 3;
    WH_Vector3D upNodePoint = center + normal * height;
    WH_Vector3D downNodePoint = center - normal * height;
    
    this->tryVolumeNodeNearbyBoundary (upNodePoint, obfTri_i);
    this->tryVolumeNodeNearbyBoundary (downNodePoint, obfTri_i);
  }
}

void WH_MG3D_MeshGenerator
::generateNodesOverVolume ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  WH_ASSERT(this->inOutChecker () != WH_NULL);

  WH_Vector3D extendedMinRange;
  WH_Vector3D extendedMaxRange;
  this->getNodeRange 
    (extendedMinRange, extendedMaxRange);
  WH_Vector3D extendedSize = extendedMaxRange - extendedMinRange;

  /* MAGIC NUMBER */
  double interval = _tetrahedronSize * 0.5;

  int xCells = (int)ceil (extendedSize.x / interval + WH::eps);
  if (xCells <= 0) xCells = 1;
  int yCells = (int)ceil (extendedSize.y / interval + WH::eps);
  if (yCells <= 0) yCells = 1;
  int zCells = (int)ceil (extendedSize.z / interval + WH::eps);
  if (zCells <= 0) zCells = 1;
  WH_UssField3D field (extendedMinRange, extendedMaxRange,
		       xCells, yCells, zCells);
  
  /* MAGIC NUMBER */
  double range = _tetrahedronSize * 0.99;
  
  for (int gx = 0; gx < field.xGrids (); gx++) {
    for (int gy = 0; gy < field.yGrids (); gy++) {
      for (int gz = 0; gz < field.zGrids (); gz++) {
	WH_Vector3D position = field.positionAt (gx, gy, gz);

	if (!this->hasNodeNear (position, range)) {
	  WH_InOutChecker3D::ContainmentType flag 
	    = _inOutChecker->checkContainmentAt (position);
	  switch (flag) {
	  case WH_InOutChecker3D::IN:
	    {
	      WH_MG3D_Node* node = new WH_MG3D_Node (position);
	      WH_ASSERT(node != WH_NULL);
	      node->putInsideVolume (_volume);
	      this->addNode (node);
	    }
	    break;
	  case WH_InOutChecker3D::OUT:
	    /* nothing */
	    break;
	  case WH_InOutChecker3D::ON:
	    WH_ASSERT_NO_REACH;
	    break;
	  default:
	    WH_ASSERT_NO_REACH;
	    break;
	  }
	}
      }
    }
  }
}

void WH_MG3D_MeshGenerator
::generateTetrahedronsOverVolume ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_rangeIsSet);
  WH_ASSERT(this->nodeBucket () != WH_NULL);
  WH_ASSERT(this->inOutChecker () != WH_NULL);
  WH_ASSERT(this->volumeTriangulator () == WH_NULL);
  WH_ASSERT(this->tetrahedron_s ().size () == 0);
  
  _volumeTriangulator 
    = new WH_DLN3D_Triangulator_MG3D (this, _volume);
  WH_ASSERT(_volumeTriangulator != WH_NULL);
  
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    WH_DLN3D_Point* point 
      = new WH_DLN3D_Point_MG3D (node_i->position (), node_i);
    WH_ASSERT(point != WH_NULL);
    _volumeTriangulator->addPoint (point);
  }

  _volumeTriangulator->perform ();
  WH_ASSERT(_volumeTriangulator->assureInvariant ());

  cerr << " _volumeTriangulator->perform () " << endl;

  _volumeTriangulator->doPostProcess ();

  cerr << " _volumeTriangulator->doPostProcess () " << endl;

  /* generate tetrahedrons */
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _volumeTriangulator->tetrahedron_s ().begin ();
       i_tetra != _volumeTriangulator->tetrahedron_s ().end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);

    if (tetraMg_i->inOutType () != WH_DLN3D_Tetrahedron_MG3D::INNER) {
      continue;
    }
    
    WH_ASSERT(!tetraMg_i->isDummy ());
    WH_ASSERT(!tetraMg_i->hasAnyOutsideVolumeNode ());

#ifndef NDEBUG
    {
      for (int iNode = 0; 
	   iNode < WH_MG3D_Tetrahedron::nFirstOrderNodes;
	   iNode++) {
	WH_DLN3D_Point_MG3D* point = tetraMg_i->pointMg (iNode);
	WH_ASSERT(!point->isDummy ());
	WH_MG3D_Node* node = point->node ();
	WH_ASSERT(node != WH_NULL);
	WH_ASSERT(node->topologyType () == WH_MG3D_Node::ON_VERTEX
		  || node->topologyType () == WH_MG3D_Node::ON_EDGE
		  || node->topologyType () == WH_MG3D_Node::ON_FACE
		  || node->topologyType () == WH_MG3D_Node::INSIDE_VOLUME);
      }
    }
#endif

    WH_MG3D_Node* node0 = tetraMg_i->pointMg (0)->node ();
    WH_MG3D_Node* node1 = tetraMg_i->pointMg (1)->node ();
    WH_MG3D_Node* node2 = tetraMg_i->pointMg (2)->node ();
    WH_MG3D_Node* node3 = tetraMg_i->pointMg (3)->node ();

    WH_MG3D_Tetrahedron* volumeTetra
      = new WH_MG3D_Tetrahedron 
      (node0, node1, node2, node3, _volume);
    WH_ASSERT(volumeTetra != WH_NULL);
    tetraMg_i->setTetrahedron (volumeTetra);
    this->addTetrahedron (volumeTetra);
  }

  /* get neighbor information of tetrahedrons */
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _volumeTriangulator->tetrahedron_s ().begin ();
       i_tetra != _volumeTriangulator->tetrahedron_s ().end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);

    if (tetraMg_i->inOutType () != WH_DLN3D_Tetrahedron_MG3D::INNER) {
      continue;
    }
    
    WH_ASSERT(!tetraMg_i->isDummy ());
    WH_ASSERT(!tetraMg_i->hasAnyOutsideVolumeNode ());

    WH_MG3D_Tetrahedron* volumeTetra = tetraMg_i->tetrahedron ();
    WH_ASSERT(volumeTetra != WH_NULL);

    WH_MG3D_Tetrahedron* volumeNeighbor0 = WH_NULL;
    WH_MG3D_Tetrahedron* volumeNeighbor1 = WH_NULL;
    WH_MG3D_Tetrahedron* volumeNeighbor2 = WH_NULL;
    WH_MG3D_Tetrahedron* volumeNeighbor3 = WH_NULL;

    if (tetraMg_i->neighborAt (0) != WH_NULL) {
      WH_DLN3D_Tetrahedron_MG3D* neighbor 
	= dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetraMg_i->neighborAt (0));
      WH_ASSERT(neighbor != WH_NULL);
      if (neighbor->inOutType () == WH_DLN3D_Tetrahedron_MG3D::INNER) {
	volumeNeighbor0 = neighbor->tetrahedron ();
	WH_ASSERT(volumeNeighbor0 != WH_NULL);
      }
    }

    if (tetraMg_i->neighborAt (1) != WH_NULL) {
      WH_DLN3D_Tetrahedron_MG3D* neighbor 
	= dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetraMg_i->neighborAt (1));
      WH_ASSERT(neighbor != WH_NULL);
      if (neighbor->inOutType () == WH_DLN3D_Tetrahedron_MG3D::INNER) {
	volumeNeighbor1 = neighbor->tetrahedron ();
	WH_ASSERT(volumeNeighbor1 != WH_NULL);
      }
    }

    if (tetraMg_i->neighborAt (2) != WH_NULL) {
      WH_DLN3D_Tetrahedron_MG3D* neighbor 
	= dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetraMg_i->neighborAt (2));
      WH_ASSERT(neighbor != WH_NULL);
      if (neighbor->inOutType () == WH_DLN3D_Tetrahedron_MG3D::INNER) {
	volumeNeighbor2 = neighbor->tetrahedron ();
	WH_ASSERT(volumeNeighbor2 != WH_NULL);
      }
    }

    if (tetraMg_i->neighborAt (3) != WH_NULL) {
      WH_DLN3D_Tetrahedron_MG3D* neighbor 
	= dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetraMg_i->neighborAt (3));
      WH_ASSERT(neighbor != WH_NULL);
      if (neighbor->inOutType () == WH_DLN3D_Tetrahedron_MG3D::INNER) {
	volumeNeighbor3 = neighbor->tetrahedron ();
	WH_ASSERT(volumeNeighbor3 != WH_NULL);
      }
    }
    
    volumeTetra->setNeighbors 
      (volumeNeighbor0, 
       volumeNeighbor1, 
       volumeNeighbor2, 
       volumeNeighbor3);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->volumeTriangulator () != WH_NULL);
  WH_ASSERT(0 < this->tetrahedron_s ().size ());
#endif
}

void WH_MG3D_MeshGenerator
::deleteOutsideVolumeNodes ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->tetrahedron_s ().size ());

  /* delete all the outside volume nodes and store the rest of nodes
     into <otherNode_s> */
  vector<WH_MG3D_Node*> otherNode_s;
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = _node_s.begin ();
       i_node != _node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);
    if (node_i->topologyType () == WH_MG3D_Node::OUTSIDE_VOLUME) {
      delete node_i;
      node_i = WH_NULL;
    } else {
      otherNode_s.push_back (node_i);
    }
  }

  _node_s.resize (0);
  copy (otherNode_s.begin (), otherNode_s.end (),
	back_inserter(_node_s));
}

void WH_MG3D_MeshGenerator
::collectFinalBoundaryFaceTriangles ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->tetrahedron_s ().size ());
  WH_ASSERT(this->fbfTri_s ().size () == 0);
  
  for (vector<WH_MG3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_MG3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (int iFace = 0; 
	 iFace < WH_Tetrahedron3D_A::nTetrahedronFaces; 
	 iFace++) {
      WH_MG3D_Tetrahedron* neighbor 
	= tetra_i->neighbor (iFace);
      
      if (neighbor == WH_NULL) {
	WH_MG3D_Node* node0 
	  = tetra_i->firstOrderNodeOnFace (iFace, 0); 
	WH_MG3D_Node* node1 
	  = tetra_i->firstOrderNodeOnFace (iFace, 1); 
	WH_MG3D_Node* node2 
	  = tetra_i->firstOrderNodeOnFace (iFace, 2); 

	WH_TPL3D_Face_A* face 
	  = WH_MG3D_Node::commonFace (node0, node1, node2);

	WH_MG3D_FinalBoundaryFaceTriangle* fbfTri 
	  = new WH_MG3D_FinalBoundaryFaceTriangle 
	  (node0, node1, node2, tetra_i, iFace, face);
	WH_ASSERT(fbfTri != WH_NULL);

	this->addFbfTri (fbfTri);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < this->fbfTri_s ().size ());
#endif
}

void WH_MG3D_MeshGenerator
::generateSecondOrderNodes ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->tetrahedron_s ().size ());
  WH_ASSERT(0 < this->fbfTri_s ().size ());
  
  for (vector<WH_MG3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_MG3D_Tetrahedron* tetra_i = (*i_tetra);

    WH_MG3D_Node* secondOrderNodes[WH_Tetrahedron3D_A::nTetrahedronEdges];
    for (int iEdge = 0; 
	 iEdge < WH_Tetrahedron3D_A::nTetrahedronEdges;
	 iEdge++) {
      WH_MG3D_Node* node0 = tetra_i->firstOrderNode 
	(WH_Tetrahedron3D_A::edgeVertexMap[iEdge][0]);
      WH_MG3D_Node* node1 = tetra_i->firstOrderNode 
	(WH_Tetrahedron3D_A::edgeVertexMap[iEdge][1]);
      WH_Vector3D midPoint = (node0->position () + node1->position ()) / 2;

      WH_MG3D_Node* secondOrderNode = this->nodeAt (midPoint);
      if (secondOrderNode == WH_NULL) {
	bool isFirstOrder = false;
	secondOrderNode = new WH_MG3D_Node (midPoint, isFirstOrder);
	WH_ASSERT(secondOrderNode != WH_NULL);

	if (node0->topologyType () == WH_MG3D_Node::INSIDE_VOLUME
	    || node1->topologyType () == WH_MG3D_Node::INSIDE_VOLUME) {
	  secondOrderNode->putInsideVolume (_volume);
	} else {

	  /* NEED TO REDEFINE */
	  /* working only if curve of edge is straight line and
             surface of face is plane only */

	  WH_TPL3D_Edge_A* commonEdge 
	    = WH_MG3D_Node::commonEdge (node0, node1);
	  if (commonEdge != WH_NULL) {
	    secondOrderNode->putOnEdge (commonEdge);
	  } else {
	    WH_TPL3D_Face_A* commonFace 
	      = WH_MG3D_Node::commonFace (node0, node1);
	    if (commonFace != WH_NULL) {
	      secondOrderNode->putOnFace (commonFace);
	    } else {
	      secondOrderNode->putInsideVolume (_volume);
	    }
	  }
	}

	this->addNode (secondOrderNode);
      } else {
	WH_ASSERT(!secondOrderNode->isFirstOrder ());
      }
      WH_ASSERT(secondOrderNode != WH_NULL);

      secondOrderNodes[iEdge] = secondOrderNode;
    }

    tetra_i->setSecondOrderNodes 
      (secondOrderNodes[0],
       secondOrderNodes[1],
       secondOrderNodes[2],
       secondOrderNodes[3],
       secondOrderNodes[4],
       secondOrderNodes[5]);
  }
}

void WH_MG3D_MeshGenerator
::setNodeId ()
{
  /* set node ID, only if it is on vertex, edge, face or inside volume */

  int currentId = 0;
  for (vector<WH_MG3D_Node*>::const_iterator 
	 i_node = _node_s.begin ();
       i_node != _node_s.end ();
       i_node++) {
    WH_MG3D_Node* node_i = (*i_node);

    switch (node_i->topologyType ()) {
    case WH_MG3D_Node::UNDEFINED_TOPOLOGY:
      WH_ASSERT_NO_REACH;
      break;
    case WH_MG3D_Node::ON_VERTEX:
    case WH_MG3D_Node::ON_EDGE:
    case WH_MG3D_Node::ON_FACE:
    case WH_MG3D_Node::INSIDE_VOLUME:
      node_i->setId (currentId);
      currentId++;
      break;
    case WH_MG3D_Node::OUTSIDE_VOLUME:
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }
}





