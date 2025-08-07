/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for triangle2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_POLYGON2D
#include <WH/polygon2d.h>
#define WH_INCLUDED_WH_POLYGON2D
#endif

class WH_Triangle2D_A;
class WH_Triangle2D;

class WH_Triangle2D_A : public WH_Polygon2D_A {
 public:
  static const int nTriangleVertexs;
  static const int nTriangleEdges;
  static const int edgeVertexMap[3][2];

  WH_Triangle2D_A ();
  virtual ~WH_Triangle2D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual double signedArea () const;
  virtual void getCircumcenter 
    (WH_Vector2D& center_OUT, double& radius_OUT) const;
  virtual void getInternalCenter 
    (WH_Vector2D& center_OUT, double& radius_OUT) const;

  /* derived */

  /* core abstract operations */
  virtual WH_Vector2D vertex (int iVertex) const = 0;

  virtual int nVertexs () const;
  virtual int nEdges () const;
  virtual WH_Segment2D edge (int iEdge) const;
  virtual bool isConvex () const;
  virtual bool isClockWise () const;
  virtual double area () const;
  virtual WH_Vector2D centerOfGravity () const;
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector2D& position) const;

 protected:
  
  /* base */

  /* derived */

};

/* value-based class */
class WH_Triangle2D : public WH_Triangle2D_A {
 public:
  WH_Triangle2D ();
  WH_Triangle2D 
    (const WH_Vector2D& v0, 
     const WH_Vector2D& v1,
     const WH_Vector2D& v2);
  virtual ~WH_Triangle2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Triangle2D& triangle);

  /* base */

  /* derived */

  /* core abstract operations */
  virtual WH_Vector2D vertex (int iVertex) const;
  
 protected:
  WH_Vector2D _vertexs[3];
  
  /* base */

  /* derived */

};
