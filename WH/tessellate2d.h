/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for tessellate2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_AFRONT2D
#include <WH/afront2d.h>
#define WH_INCLUDED_WH_AFRONT2D
#endif

/* classes derived from afront2d classes */
class WH_AF2D_Vertex_TSLT;
class WH_AF2D_Triangulator_TSLT;

class WH_TSLT_Tessellator2D;

class WH_AF2D_Vertex_TSLT : public WH_AF2D_Vertex_A {
 public:
  WH_AF2D_Vertex_TSLT 
    (const WH_Vector2D& position,
     int loopId, 
     int loopVertexId);
  virtual ~WH_AF2D_Vertex_TSLT ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  int loopId () const;

  int loopVertexId () const;

  /* derived */
  
 protected:
  int _loopId;

  int _loopVertexId;

  /* base */

  /* derived */

};

class WH_AF2D_Triangulator_TSLT : public WH_AF2D_Triangulator_A {
 public:
  WH_AF2D_Triangulator_TSLT ();
  virtual ~WH_AF2D_Triangulator_TSLT ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void beginLoop ();
  virtual void addLoopVertex (const WH_Vector2D& position);
  virtual void endLoop ();

  /* derived */
  virtual void perform ();
  virtual double edgeLength () const;
  
 protected:
  /* for vertex numbering */
  int _currentLoopId;
  int _currentLoopVertexId;

  /* for definition of initial inner and outer loops */
  WH_AF2D_Vertex_A* _firstVertex;
  WH_AF2D_Vertex_A* _prevVertex;

  /* base */

  /* derived */

  /* instance creation operation */
  /* using "Abstract Factory" pattern */
  virtual WH_AF2D_Vertex_A* createVertex 
    (const WH_Vector2D& position);
  virtual WH_AF2D_Edge_A* createEdge 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1);
  virtual WH_AF2D_Triangle_A* createTriangle 
    (WH_AF2D_Vertex_A* vertex0,
     WH_AF2D_Vertex_A* vertex1,
     WH_AF2D_Vertex_A* vertex2);
};

/* value-based class */
/* heavy weight */
class WH_TSLT_Tessellator2D {
 public:
  WH_TSLT_Tessellator2D ();
  virtual ~WH_TSLT_Tessellator2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void beginLoop ();
  virtual void addLoopVertex (const WH_Vector2D& position);
  virtual void endLoop ();

  virtual void perform ();
  
  struct Loop {
    vector<WH_Vector2D> vertex_s;
  };
  const vector<Loop*>& loop_s () const;

  struct Triangle {
    int loopId0; 
    int vertexId0;
    int loopId1; 
    int vertexId1;
    int loopId2; 
    int vertexId2;
  };
  const vector<Triangle*>& triangle_s () const;
  
  /* derived */
  
 protected:
  vector<Loop*> _loop_s;  /* own */
  
  vector<Triangle*> _triangle_s;  /* own */

  WH_AF2D_Triangulator_TSLT* _triangulator;  /* own */

  Loop* _currentLoop;  /* own */

  /* base */

  /* derived */

};
