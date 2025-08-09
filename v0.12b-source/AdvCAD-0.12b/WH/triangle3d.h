/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for triangle3d.cc */

#ifndef WH_HEADER_WH_TRIANGLE3D
#define WH_HEADER_WH_TRIANGLE3D

#ifndef WH_INCLUDED_WH_POLYGON3D
#include <WH/polygon3d.h>
#define WH_INCLUDED_WH_POLYGON3D
#endif

#ifndef WH_INCLUDED_WH_TRIANGLE2D
#include <WH/triangle2d.h>
#define WH_INCLUDED_WH_TRIANGLE2D
#endif



class WH_Triangle3D_A;
class WH_Triangle3D;



class WH_Triangle3D_A : public WH_Polygon3D_A {
 public:
  static const int nTriangleVertexs;
  static const int nTriangleEdges;
  static const int edgeVertexMap[3][2];

  WH_Triangle3D_A ();
  virtual ~WH_Triangle3D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void getCircumcenter 
    (WH_Vector3D& center_OUT, double& radius_OUT) const;
  virtual void getInternalCenter 
    (WH_Vector3D& center_OUT, double& radius_OUT) const;
  virtual double signedTetrahedronVolumeWith 
    (const WH_Vector3D& position) const;
  virtual bool isClockWiseWith (const WH_Vector3D& position) const;
  virtual bool hasConnectionAtVertexWith 
    (const WH_Triangle3D_A& triangle) const;
  virtual bool hasConnectionAtEdgeWith 
    (const WH_Triangle3D_A& triangle) const;
  virtual bool hasIntersectionWith 
    (const WH_Line3D& line,
     WH_Vector3D& point0_OUT, WH_Vector3D& point1_OUT) const;
  enum WithTriangleIntersectionType {
    NO_INTERSECTION_WITH_TRIANGLE,
    PARALLEL_WITH_TRIANGLE,
    INTERSECTION_WITH_TRIANGLE,
    OVERLAP_WITH_TRIANGLE
  };
  virtual WithTriangleIntersectionType 
    checkIntersectionWithTriangle 
    (const WH_Triangle3D_A& triangle,
     bool& intersectsAtVertexOnly_OUT) const;
  
  /* derived */

  /* core abstract operations */
  virtual WH_Vector3D vertex (int iVertex) const = 0;

  virtual int nVertexs () const;
  virtual int nEdges () const;
  virtual WH_Segment3D edge (int iEdge) const;
  virtual bool isConvex () const;
  virtual double area () const;
  virtual WH_Vector3D centerOfGravity () const;
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector3D& position) const;

 protected:
  
  /* base */

  /* derived */

};

/* non-inline free functions using WH_Triangle3D_A */

bool WH_eq (const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1);
bool WH_ne (const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1);
int WH_nMatchedVertexs 
(const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1,
 bool& reverseFlag_OUT,
 int& vertexIdOfTriangle0_OUT, int& vertexIdOfTriangle1_OUI,
 int& edgeIdOfTriangle0_OUT, int& edgeIdOfTriangle1_OUI);
bool WH_isSame 
(const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1);



/* value-based class */
class WH_Triangle3D : public WH_Triangle3D_A {
 public:
  WH_Triangle3D ();
  WH_Triangle3D 
    (const WH_Vector3D& v0, 
     const WH_Vector3D& v1,
     const WH_Vector3D& v2);
  virtual ~WH_Triangle3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Triangle3D& triangle);

  /* base */

  /* derived */

  /* core abstract operations */
  virtual WH_Vector3D vertex (int iVertex) const;

  virtual bool hasAnyVertexOffPlane () const;
  virtual WH_Plane3D plane () const;

 protected:
  WH_Vector3D _vertexs[3];

  bool _planeExists;
  WH_Plane3D _plane;
  
  /* base */

  /* derived */

};



#endif /* WH_HEADER_WH_TRIANGLE3D */
