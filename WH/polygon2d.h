/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for polygon2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif

class WH_Polygon2D_A;
class WH_Polygon2D;

class WH_Polygon2D_A {
 public:
  WH_Polygon2D_A ();
  virtual ~WH_Polygon2D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* core abstract operations */
  virtual int nVertexs () const = 0;
  virtual WH_Vector2D vertex (int iVertex) const = 0;

  virtual int nEdges () const;
  virtual WH_Segment2D edge (int iEdge) const;
  virtual int nextVertexIndex (int iVertex) const;
  virtual int prevVertexIndex (int iVertex) const;
  virtual int nextEdgeIndex (int iEdge) const;
  virtual int prevEdgeIndex (int iEdge) const;
  virtual void getVertexIndexsOfEdge 
    (int iEdge,
     int& iVertex0_OUT, int& iVertex1_OUT) const;
  virtual bool hasAnyDuplicatedVertex () const;
  virtual bool hasAnyCrossingEdge () const;
  virtual bool hasAnySpecialAngle () const;
  virtual bool isRegular () const;
  virtual bool isConvex () const;
  virtual bool isClockWise () const;
  virtual double perimeter () const;
  virtual double area () const;
  virtual WH_Vector2D centerOfGravity () const;
  virtual WH_Vector2D minRange () const;
  virtual WH_Vector2D maxRange () const;
  virtual bool hasVertexAt (const WH_Vector2D& position) const;
  virtual bool hasVertexAtEveryPointIn
    (const vector<WH_Vector2D>& point_s) const;
  enum ContainmentType {
    IN, OUT, ON
  };
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector2D& position) const;
  
  struct EdgeIntersectionData {
    enum IntersectionType {
      HAS_NO_INTERSECTION,
      HAS_INTERSECTION,
      IS_COINCIDENT
    } intersectionType;
    WH_Vector2D intersectionPoint;
    enum PositionType {
      AT_VERTEX0, 
      AT_VERTEX1,
      ON_EDGE 
    } positionType;
  };
  virtual bool intersectsWith 
    (const WH_Line2D& line,
     EdgeIntersectionData edgeData_OUT[]) const;
  virtual bool intersectsWith 
    (const WH_Segment2D& segment,
     EdgeIntersectionData edgeData_OUT[]) const;

  /* derived */

 protected:
  
  /* base */
  virtual bool 
    containsOnBoundary (const WH_Vector2D& position) const;
  virtual bool 
    containsInside (const WH_Vector2D& position) const;

  /* derived */

};

/* value-based class */
class WH_Polygon2D : public WH_Polygon2D_A {
 public:
  WH_Polygon2D ();
  WH_Polygon2D 
    (int nVertexs, const WH_Vector2D vertexs[]);
  WH_Polygon2D 
    (const vector<WH_Vector2D>& vertex_s);
  WH_Polygon2D 
    (const WH_Polygon2D& polygon);
  virtual ~WH_Polygon2D ();
  const WH_Polygon2D& operator= (const WH_Polygon2D& polygon);
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Polygon2D& polygon);

  /* base */

  /* derived */

  /* core abstract operations */
  virtual int nVertexs () const;
  virtual WH_Vector2D vertex (int iVertex) const;

 protected:
  int _nVertexs;
  WH_Vector2D* _vertexs;   /* OWN */
  /* WH_Vector2D _vertexs[_nVertexs] */
  
  /* base */
  
  /* derived */
  
};
