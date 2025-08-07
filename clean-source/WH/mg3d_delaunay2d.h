/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for mg3d_delaunay2d.cc */

#pragma once


#ifndef WH_INCLUDED_WH_MG3D
#include <WH/mg3d.h>
#define WH_INCLUDED_WH_MG3D
#endif

template <class Type> class WH_Bucket2D;
class WH_InOutChecker2D;
class WH_CDLN2D_Triangulator;



class WH_MG3D_FaceNode;
class WH_MG3D_FaceBoundarySegment;
class WH_MG3D_FaceTriangle;
class WH_MG3D_FaceMeshGenerator;



class WH_MG3D_FaceNode {
 public:
  WH_MG3D_FaceNode 
    (const WH_Vector2D& position, WH_MG3D_Node* node3D, int id);
  virtual ~WH_MG3D_FaceNode ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void clearWeight ();

  virtual void addWeight (const WH_Vector2D& center);

  virtual void movePosition ();

  WH_Vector2D position () const;

  WH_MG3D_Node* node3D () const;
  
  int id () const;

  /* derived */
  
 protected:
  WH_Vector2D _position;
  
  WH_MG3D_Node* _node3D;

  int _id;

  int _weight;

  WH_Vector2D _sum;
  
  /* base */

  /* derived */

};

class WH_MG3D_FaceBoundarySegment {
 public:
  WH_MG3D_FaceBoundarySegment 
    (WH_MG3D_FaceNode* node0,
     WH_MG3D_FaceNode* node1);
  virtual ~WH_MG3D_FaceBoundarySegment ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* 
            inside     
     node0 -------- node1
            outside
   */

  /* base */
  WH_MG3D_FaceNode* node0 () const;
  WH_MG3D_FaceNode* node1 () const;

  WH_Vector2D outsideNormal () const;
  
  /* derived */
  
 protected:
  WH_MG3D_FaceNode* _node0;
  WH_MG3D_FaceNode* _node1;
  
  /* base */

  /* derived */

};

class WH_MG3D_FaceTriangle {
 public:
  WH_MG3D_FaceTriangle 
    (WH_MG3D_FaceNode* node0,
     WH_MG3D_FaceNode* node1,
     WH_MG3D_FaceNode* node2);
  virtual ~WH_MG3D_FaceTriangle ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addWeight ();

  WH_MG3D_FaceNode* node0 () const;
  WH_MG3D_FaceNode* node1 () const;
  WH_MG3D_FaceNode* node2 () const;
  
  /* derived */
  
 protected:
  WH_MG3D_FaceNode* _node0;
  WH_MG3D_FaceNode* _node1;
  WH_MG3D_FaceNode* _node2;
  
  /* base */

  /* derived */

};

class WH_MG3D_FaceMeshGenerator {
 public:
  WH_MG3D_FaceMeshGenerator 
    (WH_MG3D_MeshGenerator* meshGenerator,
     WH_TPL3D_Face_A* face);
  virtual ~WH_MG3D_FaceMeshGenerator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void generateMesh ();

  WH_MG3D_MeshGenerator* meshGenerator () const;

  WH_TPL3D_Face_A* face () const;

  const vector<WH_MG3D_FaceNode*>& node_s () const;
  
  // Modern bounds-checked node access
  WH_MG3D_FaceNode* getNodeAt(int index) const;

  const vector<WH_MG3D_FaceBoundarySegment*>& boundarySegment_s () const;

  WH_Vector2D minRange () const;
  WH_Vector2D maxRange () const;

  WH_Bucket2D<WH_MG3D_FaceNode>* nodeBucket () const;

  WH_InOutChecker2D* inOutChecker () const;
  
  WH_CDLN2D_Triangulator* triangulator () const;

  const vector<WH_MG3D_FaceTriangle*>& triangle_s () const;

  /* derived */

 protected:
  WH_MG3D_MeshGenerator* _meshGenerator;

  WH_TPL3D_Face_A* _face;

  bool _rangeIsSet;

  vector<WH_MG3D_FaceNode*> _node_s;  /* OWN */

  vector<WH_MG3D_FaceBoundarySegment*> _boundarySegment_s;  /* OWN */

  WH_Vector2D _minRange;
  WH_Vector2D _maxRange;

  WH_Bucket2D<WH_MG3D_FaceNode>* _nodeBucket;  /* OWN */

  WH_InOutChecker2D* _inOutChecker;  /* OWN */
  
  WH_CDLN2D_Triangulator* _triangulator;   /* OWN */

  vector<WH_MG3D_FaceTriangle*> _triangle_s;  /* OWN */

  /* base */
  virtual WH_Vector3D positionAt 
    (const WH_Vector2D& parameter);
  
  virtual WH_Vector2D parameterAt 
    (const WH_Vector3D& position);
  
  virtual WH_MG3D_FaceNode* makeBoundaryNode 
    (WH_MG3D_Node* node3D);

  virtual WH_MG3D_FaceNode* findNodeFrom 
    (WH_MG3D_Node* node3D,
     const vector<WH_MG3D_FaceNode*>& node_s);

  virtual void makeBoundarySegment 
    (WH_MG3D_FaceNode* node0, WH_MG3D_FaceNode* node1);

  virtual void defineBoundaryAlongLoop 
    (WH_TPL3D_Loop_A* loop, bool isOuterLoop);

  virtual void defineBoundary ();

  virtual void getBucketParameters 
    (const WH_Vector2D& minRange, 
     const WH_Vector2D& maxRange, 
     double cellSize, 
     WH_Vector2D& extendedMinRange_OUT, 
     WH_Vector2D& extendedMaxRange_OUT, 
     int& xCells_OUT, int& yCells_OUT) const;

  virtual bool hasNodeNear 
    (const WH_Vector2D& position, double range) const;

  virtual void createInOutChecker ();

  virtual void getNodeRange 
    (WH_Vector2D& minRange_OUT, WH_Vector2D& maxRange_OUT) const;
  
  virtual void createNodeBucket ();
  
  virtual void setRange ();
  
  virtual void makeInternalNode 
    (const WH_Vector2D& position);
  
  virtual void generateInternalNodes ();

  virtual void generateTriangles ();
  
  virtual void doSmoothing ();

  /* derived */

};




