/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for afront2d.cc */

#ifndef WH_HEADER_WH_AFRONT2D
#define WH_HEADER_WH_AFRONT2D

#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif

template <class Type> class WH_Bucket2D;



class WH_AF2D_Vertex_A;
class WH_AF2D_Edge_A;
class WH_AF2D_Triangle_A;
class WH_AF2D_Triangulator_A;

class WH_AF2D_OptimizedTriangulator_A;



class WH_AF2D_Vertex_A {
 public:
  WH_AF2D_Vertex_A 
    (const WH_Vector2D& position);
  virtual ~WH_AF2D_Vertex_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_Vector2D position () const;
  
  /* derived */
  
 protected:
  WH_Vector2D _position;
  
  /* base */

  /* derived */

};

class WH_AF2D_Edge_A {
 public:
  WH_AF2D_Edge_A 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1);
  virtual ~WH_AF2D_Edge_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_AF2D_Vertex_A* vertex0 () const;
  WH_AF2D_Vertex_A* vertex1 () const;

  WH_Vector2D endPoint0 () const;
  WH_Vector2D endPoint1 () const;

  virtual WH_Vector2D midPoint () const;

  virtual WH_Segment2D segment () const;

  virtual WH_Vector2D normal () const;

  virtual WH_Vector2D minRange () const;
  virtual WH_Vector2D maxRange () const;

  virtual bool hasVertex (WH_AF2D_Vertex_A* vertex) const;

  virtual WH_AF2D_Vertex_A* 
    theOtherVertex (WH_AF2D_Vertex_A* vertex) const;
  
  /* derived */
  
 protected:
  WH_AF2D_Vertex_A* _vertex0;
  WH_AF2D_Vertex_A* _vertex1;
  
  /* base */

  /* derived */

};

class WH_AF2D_Triangle_A {
 public:
  WH_AF2D_Triangle_A 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1,
     WH_AF2D_Vertex_A* vertex2);
  virtual ~WH_AF2D_Triangle_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_AF2D_Vertex_A* vertex0 () const;
  WH_AF2D_Vertex_A* vertex1 () const;
  WH_AF2D_Vertex_A* vertex2 () const;
  
  /* derived */
  
 protected:
  WH_AF2D_Vertex_A* _vertex0;
  WH_AF2D_Vertex_A* _vertex1;
  WH_AF2D_Vertex_A* _vertex2;
  
  /* base */

  /* derived */

};

class WH_AF2D_Triangulator_A {
 public:
  WH_AF2D_Triangulator_A ();
  virtual ~WH_AF2D_Triangulator_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual double edgeLength () const = 0;
  
  virtual void perform ();
  
  const vector<WH_AF2D_Vertex_A*>& vertex_s () const;

  const vector<WH_AF2D_Triangle_A*>& triangle_s () const;

  const list<WH_AF2D_Edge_A*>& edge_s () const;

  virtual WH_Vector2D minRange () const;
  virtual WH_Vector2D maxRange () const;

  /* derived */
  
 protected:
  vector<WH_AF2D_Vertex_A*> _vertex_s;

  vector<WH_AF2D_Triangle_A*> _triangle_s;

  list<WH_AF2D_Edge_A*> _edge_s;
  
  /* base */

  /* instance creation operation */
  /* using "Abstract Factory" pattern */
  virtual WH_AF2D_Vertex_A* createVertex 
    (const WH_Vector2D& position) = 0;
  virtual WH_AF2D_Edge_A* createEdge 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1) = 0;
  virtual WH_AF2D_Triangle_A* createTriangle 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1,
     WH_AF2D_Vertex_A* vertex2) = 0;

  virtual void addVertex 
    (WH_AF2D_Vertex_A* vertex /* ADOPT */);

  virtual void addTriangle 
    (WH_AF2D_Triangle_A* triangle /* ADOPT */);

  virtual void addEdge 
    (WH_AF2D_Edge_A* edge /* ADOPT */);

  virtual void removeEdge 
    (WH_AF2D_Edge_A* edge /* DELETE */);

  virtual WH_AF2D_Edge_A* getAnyEdge () const;

  virtual bool hasNoEdgeNear 
    (const WH_Vector2D& position, double range) const;

  virtual bool canPutPoint 
    (const WH_Vector2D& newVertexPosition,
     WH_AF2D_Edge_A* edge,
     double edgeLength) const;

  virtual bool canPutEdge 
    (const WH_Vector2D& endPoint0,
     const WH_Vector2D& endPoint1,
     WH_AF2D_Edge_A*& sameEdge_OUT) const;

  virtual bool canPutTriangleWithNewVertex 
    (WH_AF2D_Edge_A* edge, 
     WH_Vector2D& newVertexPosition_OUT) const;

  virtual void advanceFrontWithNewVertex 
    (WH_AF2D_Edge_A* edge, 
     const WH_Vector2D newVertexPosition);

  virtual WH_AF2D_Vertex_A* findOptimumVertex 
    (WH_AF2D_Edge_A* edge, 
     WH_AF2D_Edge_A*& neighborEdge0_OUT,
     WH_AF2D_Edge_A*& neighborEdge1_OUT) const;

  virtual void advanceFrontWithExistingVertex 
    (WH_AF2D_Edge_A* edge, 
     WH_AF2D_Vertex_A* vertex,
     WH_AF2D_Edge_A* neighborEdge0,
     WH_AF2D_Edge_A* neighborEdge1);

  /* derived */

};



class WH_AF2D_OptimizedTriangulator_A 
: public WH_AF2D_Triangulator_A {
 public:
  WH_AF2D_OptimizedTriangulator_A ();
  virtual ~WH_AF2D_OptimizedTriangulator_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  virtual void perform ();
  virtual double edgeLength () const = 0;

 protected:
  unique_ptr<WH_Bucket2D<WH_AF2D_Vertex_A>> _vertexBucket;

  unique_ptr<WH_Bucket2D<WH_AF2D_Edge_A>> _edgeBucket;
  
  /* base */
  virtual void addVertexIntoBucket 
    (WH_AF2D_Vertex_A* vertex);
  virtual void addEdgeIntoBucket 
    (WH_AF2D_Edge_A* edge);
  virtual void removeEdgeFromBucket 
    (WH_AF2D_Edge_A* edge);
  virtual void setUpBucket ();
  
  /* derived */

  /* instance creation operation */
  /* using "Abstract Factory" pattern */
  virtual WH_AF2D_Vertex_A* createVertex 
    (const WH_Vector2D& position) = 0;
  virtual WH_AF2D_Edge_A* createEdge 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1) = 0;
  virtual WH_AF2D_Triangle_A* createTriangle 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1,
     WH_AF2D_Vertex_A* vertex2) = 0;

  virtual void addVertex 
    (WH_AF2D_Vertex_A* vertex /* ADOPT */);
  virtual void addEdge 
    (WH_AF2D_Edge_A* edge /* ADOPT */);
  virtual void removeEdge 
    (WH_AF2D_Edge_A* edge /* DELETE */);
  virtual bool hasNoEdgeNear 
    (const WH_Vector2D& position, double range) const;
  virtual bool canPutEdge 
    (const WH_Vector2D& endPoint0,
     const WH_Vector2D& endPoint1,
     WH_AF2D_Edge_A*& sameEdge_OUT) const;
  virtual WH_AF2D_Vertex_A* findOptimumVertex 
    (WH_AF2D_Edge_A* edge, 
     WH_AF2D_Edge_A*& neighborEdge0_OUT,
     WH_AF2D_Edge_A*& neighborEdge1_OUT) const;

};



#ifdef WH_INLINE_ENABLED
#include <WH/afront2d_inline.cc>
#endif

#endif /* WH_HEADER_WH_AFRONT2D */
