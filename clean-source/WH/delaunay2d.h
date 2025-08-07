/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for delaunay2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif

class WH_DLN2D_Point;
class WH_DLN2D_Segment;
class WH_DLN2D_Triangle;
class WH_DLN2D_Triangulator;

class WH_DLN2D_Point {
 public:
  WH_DLN2D_Point 
    (const WH_Vector2D& position);
  virtual ~WH_DLN2D_Point ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void setId (int id);

  int id () const;
  
  WH_Vector2D position () const;
  
  bool isDummy () const;

  /* derived */

 protected:
  int _id;

  WH_Vector2D _position;
  
  /* base */

  /* derived */

};

/* value-based class */
/* no inheritance */
class WH_DLN2D_Segment {
 public:
  WH_DLN2D_Segment 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1);
  bool checkInvariant () const;
  bool assureInvariant () const;

  /* base */
  void addEdge (WH_DLN2D_Segment* segment);

  void setFront (WH_DLN2D_Triangle* front);

  void setRear (WH_DLN2D_Triangle* rear);

  WH_DLN2D_Point* point0 () const;
  WH_DLN2D_Point* point1 () const;

  WH_DLN2D_Segment* edge0 () const;
  WH_DLN2D_Segment* edge1 () const;

  WH_DLN2D_Triangle* front () const;
  WH_DLN2D_Triangle* rear () const;

  bool hasPoint (WH_DLN2D_Point* point) const;

 protected:
  WH_DLN2D_Point* _point0;
  WH_DLN2D_Point* _point1;

  WH_DLN2D_Segment* _edge0;
  WH_DLN2D_Segment* _edge1;

  WH_DLN2D_Triangle* _front;
  WH_DLN2D_Triangle* _rear;

  /* base */

  /* derived */

};

class WH_DLN2D_Triangle {
 public:
  WH_DLN2D_Triangle 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1, 
     WH_DLN2D_Point* point2);
  virtual ~WH_DLN2D_Triangle ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  void setIterator 
    (list<WH_DLN2D_Triangle*>::iterator iterator);

  void setNeighborAt 
    (int edgeNumber, WH_DLN2D_Triangle* tri);

  void clearNeighborAt (int edgeNumber);

  virtual void clearNeighbor ();

  virtual void setNeighbor (WH_DLN2D_Triangle* tri);

  virtual void reorderPoints ();

  void setMark ();

  void clearMark ();

  WH_DLN2D_Point* point (int vertexNumber) const;

  bool hasPoint (WH_DLN2D_Point* point) const;

  list<WH_DLN2D_Triangle*>::iterator iterator () const;

  WH_DLN2D_Triangle* neighborAt (int edgeNumber) const;

  bool isDummy () const;

  bool hasMark () const;

  bool isNeighborOf (const WH_DLN2D_Triangle* tri) const;

  int edgeNumberOfNeighbor (const WH_DLN2D_Triangle* tri) const;

  int edgeNumberOf (const WH_DLN2D_Segment* seg) const;

  bool includesWithinCircle (const WH_DLN2D_Point* point) const;

 protected:
  WH_Vector2D _centerOfCircle;

  double _radiusOfCircle;

  WH_DLN2D_Point* _points[3];

  WH_DLN2D_Triangle* _neighbors[3];

  bool _markFlag;

  list<WH_DLN2D_Triangle*>::iterator _iterator;

  /* base */

  /* derived */

};

class WH_DLN2D_Triangulator {
 public:
  WH_DLN2D_Triangulator ();
  virtual ~WH_DLN2D_Triangulator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addPoint (WH_DLN2D_Point* point  /* ADOPT */);

  virtual void perform ();

  virtual void deleteTriangles 
    (const vector<WH_DLN2D_Triangle*>& tri_s  /* ORPHAN */);

  virtual void reorderTriangle ();
  
  const vector<WH_DLN2D_Point*>& point_s () const;

  const list<WH_DLN2D_Triangle*>& triangle_s () const;

  virtual void getRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;

  /* derived */
  
 protected:
  vector<WH_DLN2D_Point*> _cornerDummyPoint_s;  /* own */

  vector<WH_DLN2D_Point*> _point_s;  /* own */
  
  list<WH_DLN2D_Triangle*> _triangle_s;   /* own */

  WH_DLN2D_Point* _currentPoint;   /* not own */

  vector<WH_DLN2D_Triangle*> _deletedTriangle_s;  
  /* not own */

  vector<WH_DLN2D_Segment*> _surroundingSegment_s;  
  /* not own */

  /* base */

  /* factory method */
  virtual WH_DLN2D_Point* createDummyPoint 
    (int id, const WH_Vector2D& position);
  virtual WH_DLN2D_Triangle* createTriangle 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1,
     WH_DLN2D_Point* point2);

  virtual void addTriangle 
    (WH_DLN2D_Triangle* tri  /* ADOPT */);

  virtual void removeTriangle 
    (WH_DLN2D_Triangle* tri  /* ORPHAN */);

  virtual void createDummyPoints ();

  virtual void createInitialTriangles ();

  virtual void prepare ();

  virtual WH_DLN2D_Triangle* 
    pickUpFirstTriangle () const;

  virtual void searchNeighbor 
    (WH_DLN2D_Triangle* tri, int edgeNumber);

  virtual void markTriangle 
    (WH_DLN2D_Triangle* tri);

  virtual bool checkSegment ();

  virtual void connectSegment ();

  virtual void deleteOldTriangle ();

  virtual void makeNewTriangle ();

  virtual bool insertPoint (WH_DLN2D_Point* point);

  virtual void makeTriangle ();

  /* derived */

};

#ifdef WH_INLINE_ENABLED
#include <WH/delaunay2d_inline.cc>
#endif

