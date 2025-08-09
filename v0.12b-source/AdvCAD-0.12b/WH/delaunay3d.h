/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for delaunay3d.cc */

#ifndef WH_HEADER_WH_DELAUNAY3D
#define WH_HEADER_WH_DELAUNAY3D

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif



class WH_DLN3D_Point;
class WH_DLN3D_Triangle;
class WH_DLN3D_Tetrahedron;
class WH_DLN3D_Triangulator;



class WH_DLN3D_Point {
 public:
  WH_DLN3D_Point 
    (const WH_Vector3D& position);
  virtual ~WH_DLN3D_Point ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void setId (int id);

  int id () const;
  
  WH_Vector3D position () const;

  bool isDummy () const;

  /* derived */
  
 protected:
  int _id;

  WH_Vector3D _position;
  
  /* base */

  /* derived */

};

/* value-based class */
/* no inheritance */
class WH_DLN3D_Triangle {
 public:
  WH_DLN3D_Triangle 
    (WH_DLN3D_Point* point0, 
     WH_DLN3D_Point* point1, 
     WH_DLN3D_Point* point2,
     WH_DLN3D_Tetrahedron* tetrahedron, 
     int faceNumber);
  bool checkInvariant () const;
  bool assureInvariant () const;

  /* base */
  void setEdgeAt 
    (int edgeNumber, WH_DLN3D_Triangle* tri);

  void setFront (WH_DLN3D_Tetrahedron* front);

  void setRear (WH_DLN3D_Tetrahedron* rear);

  WH_DLN3D_Tetrahedron* tetrahedron () const;

  int faceNumber () const;

  WH_DLN3D_Point* point (int vertexNumber) const;

  bool hasPoint (WH_DLN3D_Point* point) const;

  WH_DLN3D_Triangle* edgeAt (int edgeNumber) const;

  WH_DLN3D_Tetrahedron* front () const;

  WH_DLN3D_Tetrahedron* rear () const;

  int frontFaceNumber () const;

  /* derived */
  
 protected:
  WH_DLN3D_Tetrahedron* _tetrahedron;

  int _faceNumber;

  WH_DLN3D_Point* _points[3];

  WH_DLN3D_Triangle* _edges[3];

  WH_DLN3D_Tetrahedron* _front;

  int _frontFaceNumber;

  WH_DLN3D_Tetrahedron* _rear;

  /* base */

  /* derived */

};

class WH_DLN3D_Tetrahedron {
 public:
  WH_DLN3D_Tetrahedron 
    (WH_DLN3D_Point* point0, 
     WH_DLN3D_Point* point1, 
     WH_DLN3D_Point* point2, 
     WH_DLN3D_Point* point3);
  virtual ~WH_DLN3D_Tetrahedron ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  void setIterator 
    (list<WH_DLN3D_Tetrahedron*>::iterator iterator);

  void setNeighborAt 
    (int faceNumber, WH_DLN3D_Tetrahedron* tetra);

  void clearNeighborAt (int faceNumber);

  virtual void clearNeighbor ();

  virtual void setNeighbor (WH_DLN3D_Tetrahedron* tetra);

  virtual void reorderPoints ();

  void setMark ();

  void clearMark ();

  WH_DLN3D_Point* point (int vertexNumber) const;

  bool hasPoint (WH_DLN3D_Point* point) const;

  list<WH_DLN3D_Tetrahedron*>::iterator iterator () const;

  WH_DLN3D_Tetrahedron* neighborAt (int faceNumber) const;

  bool isDummy () const;

  bool hasMark () const;

  bool isNeighborOf (const WH_DLN3D_Tetrahedron* tetra) const;

  int faceNumberOfNeighbor (const WH_DLN3D_Tetrahedron* tetra) const;

  int faceNumberOf (const WH_DLN3D_Triangle* tri) const;

  bool includesWithinSphere (const WH_DLN3D_Point* point) const;

  bool isSliver ();

  /* derived */

 protected:
  WH_Vector3D _centerOfSphere;

  double _radiusOfSphere;

  WH_DLN3D_Point* _points[4];

  WH_DLN3D_Tetrahedron* _neighbors[4];

  bool _markFlag;

  list<WH_DLN3D_Tetrahedron*>::iterator _iterator;

  /* base */

  /* derived */

};

class WH_DLN3D_Triangulator {
 public:
  WH_DLN3D_Triangulator ();
  virtual ~WH_DLN3D_Triangulator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addPoint (WH_DLN3D_Point* point  /* ADOPT */);

  virtual void perform ();

  virtual void deleteTetrahedrons 
    (const vector<WH_DLN3D_Tetrahedron*>& tetra_s  /* ORPHAN */);

  virtual void reorderTetrahedron ();
  
  const vector<WH_DLN3D_Point*>& point_s () const;

  const list<WH_DLN3D_Tetrahedron*>& tetrahedron_s () const;
  
  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;

  /* derived */
  
 protected:
  struct Segment {
    Segment (WH_DLN3D_Triangle* tri, 
	     int edgeNumber,
	     WH_DLN3D_Point* point0, 
	     WH_DLN3D_Point* point1) {
      this->triangle = tri;
      this->edgeNumber = edgeNumber;
      this->point0 = point0;
      this->point1 = point1;
    }
    
    WH_DLN3D_Triangle* triangle;
    
    int edgeNumber;
    
    WH_DLN3D_Point* point0;
    WH_DLN3D_Point* point1;
  };

  vector<WH_DLN3D_Point*> _cornerDummyPoint_s;  /* own */

  vector<WH_DLN3D_Point*> _otherDummyPoint_s;  /* own */

  vector<WH_DLN3D_Point*> _point_s;  /* own */
  
  list<WH_DLN3D_Tetrahedron*> _tetrahedron_s;   /* own */

  WH_DLN3D_Point* _currentPoint;   /* not own */

  vector<WH_DLN3D_Tetrahedron*> _deletedTetrahedron_s;  
  /* not own */

  vector<WH_DLN3D_Triangle*> _surroundingTriangle_s;  
  /* not own */

  /* base */

  /* factory method */
  virtual WH_DLN3D_Point* createDummyPoint 
    (int id, const WH_Vector3D& position);
  virtual WH_DLN3D_Tetrahedron* createTetrahedron 
    (WH_DLN3D_Point* point0, 
     WH_DLN3D_Point* point1,
     WH_DLN3D_Point* point2, 
     WH_DLN3D_Point* point3);

  virtual void addTetrahedron 
    (WH_DLN3D_Tetrahedron* tetra  /* ADOPT */);

  virtual void removeTetrahedron 
    (WH_DLN3D_Tetrahedron* tetra  /* ORPHAN */);

  virtual void createDummyPoints ();

  virtual void createInitialTetrahedrons ();

  virtual void prepare ();

  virtual WH_DLN3D_Tetrahedron* 
    pickUpFirstTetrahedron () const;

  virtual void searchNeighbor 
    (WH_DLN3D_Tetrahedron* tetra, int faceNumber);

  virtual void markTetrahedron 
    (WH_DLN3D_Tetrahedron* tetra);

  virtual bool checkTriangle ();

  virtual void connectEdgeOfTriangle ();

  virtual void deleteOldTetrahedron ();

  virtual void makeNewTetrahedron ();

  virtual bool insertPoint (WH_DLN3D_Point* point);

  virtual void makeTetrahedron ();

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/delaunay3d_inline.cc>
#endif

#endif /* WH_HEADER_WH_DELAUNAY3D */
