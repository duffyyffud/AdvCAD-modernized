/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for mg3d_base.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SEGMENT3D
#include <WH/segment3d.h>
#define WH_INCLUDED_WH_SEGMENT3D
#endif

#ifndef WH_INCLUDED_WH_TRIANGLE3D
#include <WH/triangle3d.h>
#define WH_INCLUDED_WH_TRIANGLE3D
#endif

#ifndef WH_INCLUDED_WH_TETRAHEDRON3D
#include <WH/tetrahedron3d.h>
#define WH_INCLUDED_WH_TETRAHEDRON3D
#endif

#ifndef WH_INCLUDED_WH_TOPOLOGY3D
#include <WH/topology3d.h>
#define WH_INCLUDED_WH_TOPOLOGY3D
#endif

class WH_MG3D_Node;
class WH_MG3D_OriginalBoundaryEdgeSegment;
class WH_MG3D_OriginalBoundaryFaceTriangle;
class WH_MG3D_FinalBoundaryFaceTriangle;
class WH_MG3D_Tetrahedron;

class WH_MG3D_Node {
 public:
  WH_MG3D_Node 
    (const WH_Vector3D& position,
     bool isFirstOrder = true);
  virtual ~WH_MG3D_Node ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  static WH_TPL3D_Face_A* commonFace 
    (WH_MG3D_Node* node0, 
     WH_MG3D_Node* node1, 
     WH_MG3D_Node* node2);

  static WH_TPL3D_Face_A* commonFace 
    (WH_MG3D_Node* node0, 
     WH_MG3D_Node* node1);

  static WH_TPL3D_Edge_A* commonEdge 
    (WH_MG3D_Node* node0, 
     WH_MG3D_Node* node1);

  virtual void putOnVertex 
    (WH_TPL3D_Vertex_A* vertex);

  virtual void putOnEdge 
    (WH_TPL3D_Edge_A* edge,
     bool isJustOnTopologicalEntity = true);

  virtual void putOnFace 
    (WH_TPL3D_Face_A* face,
     bool isJustOnTopologicalEntity = true);

  virtual void putInsideVolume 
    (WH_TPL3D_Volume_A* volume);

  virtual void putOutsideVolume ();

  virtual void setPosition (const WH_Vector3D& position);

  virtual void setId (int id);

  virtual void projectOnTopologicalEntity ();

  WH_Vector3D position () const;

  int id () const;
  
  bool isFirstOrder () const;

  enum TopologyType {
    UNDEFINED_TOPOLOGY, 
    ON_VERTEX,
    ON_EDGE,
    ON_FACE,
    INSIDE_VOLUME,
    OUTSIDE_VOLUME
  };
  TopologyType topologyType () const;

  WH_TPL3D_Vertex_A* vertex () const;

  WH_TPL3D_Edge_A* edge () const;

  WH_TPL3D_Face_A* face () const;

  WH_TPL3D_Volume_A* volume () const;

  bool isJustOnTopologicalEntity () const;

  virtual bool hasFace (WH_TPL3D_Face_A* face) const;

  virtual void getFaces 
    (vector<WH_TPL3D_Face_A*>& face_s_OUT) const;

  virtual bool hasEdge (WH_TPL3D_Edge_A* edge) const;

  virtual void getEdges 
    (vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const;
  
  /* derived */
  
 protected:
  WH_Vector3D _position;

  int _id;
  
  bool _isFirstOrder;

  TopologyType _topologyType;

  WH_TPL3D_Vertex_A* _vertex;

  WH_TPL3D_Edge_A* _edge;

  WH_TPL3D_Face_A* _face;

  WH_TPL3D_Volume_A* _volume;

  bool _isJustOnTopologicalEntity;

  /* base */
  
  /* derived */
  
};

class WH_MG3D_OriginalBoundaryEdgeSegment {
 public:
  WH_MG3D_OriginalBoundaryEdgeSegment 
    (WH_MG3D_Node* node0,
     WH_MG3D_Node* node1,
     WH_TPL3D_Edge_A* edge);
  virtual ~WH_MG3D_OriginalBoundaryEdgeSegment ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  WH_MG3D_Node* node0 () const;
  WH_MG3D_Node* node1 () const;

  WH_TPL3D_Edge_A* edge () const;

  virtual WH_Vector3D minRange () const;
  virtual WH_Vector3D maxRange () const;

  virtual WH_Segment3D shape () const;

  /* derived */
  
 protected:
  WH_MG3D_Node* _node0;
  WH_MG3D_Node* _node1;

  WH_TPL3D_Edge_A* _edge;

  /* base */
  
  /* derived */
  
};

class WH_MG3D_OriginalBoundaryFaceTriangle {
 public:
  WH_MG3D_OriginalBoundaryFaceTriangle 
    (WH_MG3D_Node* node0,
     WH_MG3D_Node* node1,
     WH_MG3D_Node* node2,
     WH_TPL3D_Face_A* face);
  virtual ~WH_MG3D_OriginalBoundaryFaceTriangle ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  WH_MG3D_Node* node0 () const;
  WH_MG3D_Node* node1 () const;
  WH_MG3D_Node* node2 () const;

  WH_TPL3D_Face_A* face () const;

  virtual WH_Vector3D minRange () const;
  virtual WH_Vector3D maxRange () const;

  virtual WH_Triangle3D shape () const;

  /* derived */
  
 protected:
  WH_MG3D_Node* _node0;
  WH_MG3D_Node* _node1;
  WH_MG3D_Node* _node2;

  WH_TPL3D_Face_A* _face;

  /* base */
  
  /* derived */
  
};

class WH_MG3D_FinalBoundaryFaceTriangle {
 public:
  static const int nFirstOrderNodes;
  static const int nSecondOrderNodes;

  WH_MG3D_FinalBoundaryFaceTriangle 
    (WH_MG3D_Node* node0,
     WH_MG3D_Node* node1,
     WH_MG3D_Node* node2,
     WH_MG3D_Tetrahedron* tetrahedron,
     int tetrahedronFaceId,
     WH_TPL3D_Face_A* face);
  virtual ~WH_MG3D_FinalBoundaryFaceTriangle ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  WH_MG3D_Node* firstOrderNode (int index) const;
  
  bool hasFirstOrderNode (WH_MG3D_Node* node) const;
  
  bool hasAnySecondOrderNode () const;

  WH_MG3D_Node* secondOrderNode (int index) const;

  bool hasSecondOrderNode (WH_MG3D_Node* node) const;

  WH_MG3D_Tetrahedron* tetrahedron () const;

  int tetrahedronFaceId () const;

  WH_TPL3D_Face_A* face () const;

  /* derived */
  
 protected:
  WH_MG3D_Node* _firstOrderNodes[3];

  WH_MG3D_Tetrahedron* _tetrahedron;

  int _tetrahedronFaceId;

  WH_TPL3D_Face_A* _face;

  /* base */
  
  /* derived */
  
};

class WH_MG3D_Tetrahedron {
 public:
  static const int nFirstOrderNodes;
  static const int nSecondOrderNodes;

  WH_MG3D_Tetrahedron 
    (WH_MG3D_Node* node0,
     WH_MG3D_Node* node1,
     WH_MG3D_Node* node2,
     WH_MG3D_Node* node3,
     WH_TPL3D_Volume_A* volume);
  virtual ~WH_MG3D_Tetrahedron ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setNeighbors 
    (WH_MG3D_Tetrahedron* neighbor0,
     WH_MG3D_Tetrahedron* neighbor1,
     WH_MG3D_Tetrahedron* neighbor2,
     WH_MG3D_Tetrahedron* neighbor3);

  virtual void setSecondOrderNodes 
    (WH_MG3D_Node* node0,
     WH_MG3D_Node* node1,
     WH_MG3D_Node* node2,
     WH_MG3D_Node* node3,
     WH_MG3D_Node* node4,
     WH_MG3D_Node* node5);

  WH_MG3D_Node* firstOrderNode (int index) const;
  
  bool hasFirstOrderNode (WH_MG3D_Node* node) const;
  
  WH_MG3D_Node* firstOrderNodeOnEdge 
    (int edgeId, int vertexOnEdgeId) const;
  
  WH_MG3D_Node* firstOrderNodeOnFace 
    (int faceId, int vertexOnFaceId) const;

  bool hasAnySecondOrderNode () const;
  
  WH_MG3D_Node* secondOrderNode (int index) const;
  
  bool hasSecondOrderNode (WH_MG3D_Node* node) const;

  WH_MG3D_Node* secondOrderNodeOnEdge 
    (int edgeId) const;

  WH_MG3D_Node* secondOrderNodeOnFace 
    (int faceId, int edgeOnFaceId) const;

  WH_MG3D_Tetrahedron* neighbor (int faceId) const;

  WH_TPL3D_Volume_A* volume () const;

  /* derived */
  
 protected:
  WH_MG3D_Node* _firstOrderNodes[4];

  WH_MG3D_Node* _secondOrderNodes[6];

  WH_MG3D_Tetrahedron* _neighbors[4];

  WH_TPL3D_Volume_A* _volume;

  /* base */
  
  /* derived */
  
};

