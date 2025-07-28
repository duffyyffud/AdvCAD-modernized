/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for mg3d_delaunay3d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_MG3D
#include <WH/mg3d.h>
#define WH_INCLUDED_WH_MG3D
#endif

#ifndef WH_INCLUDED_WH_DELAUNAY3D
#include <WH/delaunay3d.h>
#define WH_INCLUDED_WH_DELAUNAY3D
#endif

class WH_Segment3D;
class WH_Triangle3D;
class WH_Tetrahedron3D;

class WH_DLN3D_Point_MG3D;
class WH_DLN3D_Tetrahedron_MG3D;
class WH_DLN3D_FaceTriangle_MG3D;
class WH_DLN3D_Triangulator_MG3D;

class WH_DLN3D_Point_MG3D : public WH_DLN3D_Point {
 public:
  WH_DLN3D_Point_MG3D 
    (const WH_Vector3D& position,
     WH_MG3D_Node* node);
  virtual ~WH_DLN3D_Point_MG3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_MG3D_Node* node () const;

  /* derived */

 protected:
  WH_MG3D_Node* _node;

  /* base */

  /* derived */

};

class WH_DLN3D_Tetrahedron_MG3D : public WH_DLN3D_Tetrahedron {
 public:
  WH_DLN3D_Tetrahedron_MG3D 
    (WH_DLN3D_Point_MG3D* point0, 
     WH_DLN3D_Point_MG3D* point1, 
     WH_DLN3D_Point_MG3D* point2, 
     WH_DLN3D_Point_MG3D* point3);
  virtual ~WH_DLN3D_Tetrahedron_MG3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_DLN3D_Point_MG3D* pointMg (int index) const;

  virtual bool hasAnyInsideVolumeNode () const;

  virtual bool hasAnyOutsideVolumeNode () const;

  enum InOutType {
    UNDEFINED, INNER, OUTER, BOUNDARY
  };
  InOutType inOutType () const;

  virtual void setInOutType (InOutType inOutType);

  virtual void setTetrahedron (WH_MG3D_Tetrahedron* tetra);

  WH_MG3D_Tetrahedron* tetrahedron () const;

  /* derived */

 protected:
  InOutType _inOutType;

  WH_MG3D_Tetrahedron* _tetrahedron;

  /* base */

  /* derived */

};

class WH_DLN3D_FaceTriangle_MG3D {
 public:
  WH_DLN3D_FaceTriangle_MG3D 
    (WH_DLN3D_Point_MG3D* point0,
     WH_DLN3D_Point_MG3D* point1,
     WH_DLN3D_Point_MG3D* point2,
     WH_DLN3D_Tetrahedron_MG3D* innerTetrahedron,
     WH_DLN3D_Tetrahedron_MG3D* outerTetrahedron);
  virtual ~WH_DLN3D_FaceTriangle_MG3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_DLN3D_Point_MG3D* point0 () const;
  WH_DLN3D_Point_MG3D* point1 () const;
  WH_DLN3D_Point_MG3D* point2 () const;

  WH_DLN3D_Tetrahedron_MG3D* innerTetrahedron () const;

  WH_DLN3D_Tetrahedron_MG3D* outerTetrahedron () const;

  virtual WH_TPL3D_Face_A* commonFace () const;

  /* derived */

 protected:
  WH_DLN3D_Point_MG3D* _point0;
  WH_DLN3D_Point_MG3D* _point1;
  WH_DLN3D_Point_MG3D* _point2;

  WH_DLN3D_Tetrahedron_MG3D* _innerTetrahedron;
  WH_DLN3D_Tetrahedron_MG3D* _outerTetrahedron;

  /* base */

  /* derived */

};

class WH_DLN3D_Triangulator_MG3D : public WH_DLN3D_Triangulator {
public:
  WH_DLN3D_Triangulator_MG3D 
    (WH_MG3D_MeshGenerator* meshGenerator,
     WH_TPL3D_Volume_A* volume);
  virtual ~WH_DLN3D_Triangulator_MG3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_MG3D_MeshGenerator* meshGenerator () const;

  WH_TPL3D_Volume_A* volume () const;

  virtual void doPostProcess ();

  const vector<WH_DLN3D_FaceTriangle_MG3D*>& faceTriangle_s () const;

  /* derived */

protected:
  WH_MG3D_MeshGenerator* _meshGenerator;

  WH_TPL3D_Volume_A* _volume;

  vector<WH_DLN3D_FaceTriangle_MG3D*> _faceTriangle_s;  /* OWN */

  /* base */
  virtual void classifyInOutOfTetrahedronsRoughly ();

  virtual void collectBoundaryFaceTriangles ();

  virtual void reclassifyInOutOfTetrahedrons ();

  virtual void createDividedTetrahedronsOnEdge 
    (WH_DLN3D_Tetrahedron* tetra,
     int edgeNumber,
     WH_DLN3D_Point* newPoint,
     vector<WH_DLN3D_Tetrahedron*>& newTetra_s_IO);
  
  virtual void divideTetrahedronsIntersectingOnEdge 
    (WH_DLN3D_Point* newPoint,
     WH_DLN3D_Point* edgePoint0,
     WH_DLN3D_Point* edgePoint1);

  virtual void createDividedTetrahedronsOnFace 
    (WH_DLN3D_Tetrahedron* tetra,
     int faceNumber,
     WH_DLN3D_Point* newPoint,
     vector<WH_DLN3D_Tetrahedron*>& newTetra_s_IO);

  virtual void divideTetrahedronsIntersectingOnFace 
    (WH_DLN3D_Point* newPoint,
     WH_DLN3D_Point* facePoint0,
     WH_DLN3D_Point* facePoint1,
     WH_DLN3D_Point* facePoint2);
  
  virtual WH_DLN3D_Point_MG3D* createPointAtIntersectionWithEdge 
    (WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg, 
     const WH_Vector3D& intersectionPoint);
  
  virtual WH_DLN3D_Point_MG3D* createPointAtIntersectionWithFace 
    (WH_MG3D_OriginalBoundaryFaceTriangle* obfTri, 
     const WH_Vector3D& intersectionPoint);
  
  virtual bool checkIntersection 
    (WH_DLN3D_Tetrahedron_MG3D* tetra);
  
  virtual void divideBoundaryTetrahedrons ();

  /* derived */
  virtual WH_DLN3D_Tetrahedron* createTetrahedron 
    (WH_DLN3D_Point* point0, 
     WH_DLN3D_Point* point1,
     WH_DLN3D_Point* point2, 
     WH_DLN3D_Point* point3);
  
};
