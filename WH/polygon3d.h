/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for polygon3d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SEGMENT3D
#include <WH/segment3d.h>
#define WH_INCLUDED_WH_SEGMENT3D
#endif

#ifndef WH_INCLUDED_WH_POLYGON2D
#include <WH/polygon2d.h>
#define WH_INCLUDED_WH_POLYGON2D
#endif

class WH_Polygon3D_A;
class WH_Polygon3D;

class WH_Polygon3D_A {
 public:
  WH_Polygon3D_A ();
  virtual ~WH_Polygon3D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* core abstract operations */
  virtual int nVertexs () const = 0;
  virtual WH_Vector3D vertex (int iVertex) const = 0;

  virtual int nEdges () const;
  virtual WH_Segment3D edge (int iEdge) const;
  virtual int nextVertexIndex (int iVertex) const;
  virtual int prevVertexIndex (int iVertex) const;
  virtual int nextEdgeIndex (int iEdge) const;
  virtual int prevEdgeIndex (int iEdge) const;
  virtual void getVertexIndexsOfEdge 
    (int iEdge,
     int& iVertex0_OUT, int& iVertex1_OUT) const;
  virtual bool hasAnyVertexOffPlane () const;
  virtual WH_Plane3D plane () const;
  virtual WH_Vector2D vertex2D (int iVertex) const;
  virtual WH_Segment2D edge2D (int iEdge) const;
  virtual WH_Polygon2D polygon2D () const;
  virtual bool hasAnyDuplicatedVertex () const;
  virtual bool hasAnyCrossingEdge () const;
  virtual bool hasAnySpecialAngle () const;
  virtual bool isRegular () const;
  virtual bool isConvex () const;
  virtual double perimeter () const;
  virtual double area () const;
  virtual WH_Vector3D centerOfGravity () const;
  virtual WH_Vector3D minRange () const;
  virtual WH_Vector3D maxRange () const;
  virtual bool hasVertexAt (const WH_Vector3D& position) const;
  virtual bool hasVertexAtEveryPointIn
    (const vector<WH_Vector3D>& point_s) const;
  enum ContainmentType {
    IN, OUT, ON
  };
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector3D& position) const;
  
  struct EdgeIntersectionData {
    enum IntersectionType {
      HAS_NO_INTERSECTION,
      HAS_INTERSECTION,
      IS_COINCIDENT
    } intersectionType;
    WH_Vector3D intersectionPoint;
    enum PositionType {
      AT_VERTEX0, 
      AT_VERTEX1,
      ON_EDGE 
    } positionType;
  };
  enum WithLineIntersectionType {
    NO_INTERSECTION_WITH_LINE, 
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  virtual WithLineIntersectionType checkIntersectionWith 
    (const WH_Line3D& line,
     WH_Vector3D& point_OUT,
     EdgeIntersectionData edgeData_OUT[]) const;
  enum WithPlaneIntersectionType {
    NO_INTERSECTION_WITH_PLANE, 
    LINE_WITH_PLANE, 
    PARALLEL_WITH_PLANE, 
    COINCIDENT_WITH_PLANE
  };
  virtual WithPlaneIntersectionType checkIntersectionWith 
    (const WH_Plane3D& plane,
     WH_Line3D& line_OUT,
     EdgeIntersectionData edgeData_OUT[]) const;
  enum WithSegmentIntersectionType {
    NO_INTERSECTION_WITH_SEGMENT, 
    POINT_WITH_SEGMENT, 
    PARALLEL_WITH_SEGMENT, 
    COINCIDENT_WITH_SEGMENT
  };
  virtual WithSegmentIntersectionType checkIntersectionWith
    (const WH_Segment3D& segment,
     WH_Vector3D& point_OUT,
     EdgeIntersectionData edgeData_OUT[]) const;

  /* derived */

 protected:

  /* base */
  virtual void moveEdgeData 
    (WH_Polygon2D_A::EdgeIntersectionData edgeData2D[],
     EdgeIntersectionData edgeData_OUT[]) const;
  virtual bool 
    containsOnBoundary (const WH_Vector3D& position) const;
  virtual bool 
    containsInside (const WH_Vector3D& position) const;
  
  /* derived */

};

/* value-based class */
class WH_Polygon3D : public WH_Polygon3D_A {
 public:
  WH_Polygon3D ();
  WH_Polygon3D 
    (int nVertexs, const WH_Vector3D vertexs[]);
  WH_Polygon3D 
    (const vector<WH_Vector3D>& vertex_s);
  WH_Polygon3D 
    (const WH_Polygon3D& polygon);
  virtual ~WH_Polygon3D ();
  const WH_Polygon3D& operator= (const WH_Polygon3D& polygon);
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Polygon3D& polygon);

  /* base */

  /* derived */

  /* core abstract operations */
  virtual int nVertexs () const;
  virtual WH_Vector3D vertex (int iVertex) const;

  virtual bool hasAnyVertexOffPlane () const;
  virtual WH_Plane3D plane () const;

 protected:
  int _nVertexs;
  WH_Vector3D* _vertexs;   /* OWN */
  /* WH_Vector3D _vertexs[_nVertexs] */
  
  bool _planeExists;
  WH_Plane3D _plane;

  /* base */
  
  /* derived */
  
};
