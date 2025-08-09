/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for topology3d.cc */

#ifndef WH_HEADER_WH_TOPOLOGY3D
#define WH_HEADER_WH_TOPOLOGY3D

#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif



class WH_TPL3D_Vertex_A;
class WH_TPL3D_Curve_A;
class WH_TPL3D_Edge_A;
class WH_TPL3D_LoopVertexUse_A;
class WH_TPL3D_Curve2D_A;
class WH_TPL3D_LoopEdgeUse_A;
class WH_TPL3D_Loop_A;
class WH_TPL3D_Surface_A;
class WH_TPL3D_Face_A;
class WH_TPL3D_Volume_A;
class WH_TPL3D_Body_A;



class WH_TPL3D_Vertex_A {
 public:
  WH_TPL3D_Vertex_A ();
  virtual ~WH_TPL3D_Vertex_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Body_A* body () const;

  const vector<WH_TPL3D_Face_A*>& connectedFace_s () const;

  const vector<WH_TPL3D_Edge_A*>& connectedEdge_s () const;

  virtual WH_Vector3D point () const = 0;

  /* derived */
  
 protected:
  WH_TPL3D_Body_A* _body;

  vector<WH_TPL3D_Face_A*> _connectedFace_s;

  vector<WH_TPL3D_Edge_A*> _connectedEdge_s;
  
  /* base */

  /* derived */

};

class WH_TPL3D_Curve_A {
 public:
  WH_TPL3D_Curve_A ();
  virtual ~WH_TPL3D_Curve_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual bool isLine () const = 0;

  virtual bool isPeriodic () const = 0;

  virtual WH_Vector3D positionAt 
    (double parameter) const = 0;

  virtual WH_Vector3D directionAt 
    (double parameter) const = 0;

  virtual double lengthBetween 
    (double parameter0, double parameter1) const = 0;

  virtual bool contains 
    (const WH_Vector3D& position) const = 0;

  virtual double parameterAt 
    (const WH_Vector3D& position) const = 0;

  virtual bool getLength 
    (double& length_OUT) const = 0;
  /* return true if <isPeriodic ()> */

  virtual bool getMinParameter 
    (double& parameter_OUT) const = 0;
  /* return true if <isPeriodic ()> */

  virtual bool getMaxParameter 
    (double& parameter_OUT) const = 0;
  /* return true if <isPeriodic ()> */

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_Edge_A {
 public:
  WH_TPL3D_Edge_A ();
  virtual ~WH_TPL3D_Edge_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Body_A* body () const;

  WH_TPL3D_Vertex_A* vertex0 () const;

  WH_TPL3D_Vertex_A* vertex1 () const;

  const vector<WH_TPL3D_Face_A*>& connectedFace_s () const;

  WH_TPL3D_Curve_A* curve () const;

  virtual bool isWire () const;

  virtual bool isPeriodic () const;

  virtual double parameter0 () const;
  /* valid only if <!isPeriodic ()> */
  
  virtual double parameter1 () const;
  /* valid only if <!isPeriodic ()> */

  virtual double length () const;

  /* derived */
  
 protected:
  WH_TPL3D_Body_A* _body;

  WH_TPL3D_Vertex_A* _vertex0;
  WH_TPL3D_Vertex_A* _vertex1;

  vector<WH_TPL3D_Face_A*> _connectedFace_s;
  
  WH_TPL3D_Curve_A* _curve;  /* OWN */

  /* base */

  /* derived */

};

class WH_TPL3D_LoopVertexUse_A {
 public:
  WH_TPL3D_LoopVertexUse_A ();
  virtual ~WH_TPL3D_LoopVertexUse_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Loop_A* loop () const;

  WH_TPL3D_Vertex_A* vertex () const;

  virtual WH_Vector2D parameter () const;

  /* derived */
  
 protected:
  WH_TPL3D_Loop_A* _loop;
  WH_TPL3D_Vertex_A* _vertex;
  
  /* base */

  /* derived */

};

class WH_TPL3D_Curve2D_A {
 public:
  WH_TPL3D_Curve2D_A ();
  virtual ~WH_TPL3D_Curve2D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual bool isLine () const = 0;

  virtual bool isPeriodic () const = 0;

  virtual WH_Vector2D positionAt 
    (double parameter) const = 0;

  virtual WH_Vector2D directionAt 
    (double parameter) const = 0;

  virtual double lengthBetween 
    (double parameter0, double parameter1) const = 0;

  virtual bool contains 
    (const WH_Vector2D& position) const = 0;

  virtual double parameterAt 
    (const WH_Vector2D& position) const = 0;

  virtual bool getLength 
    (double& length_OUT) const = 0;
  /* return true if <isPeriodic ()> */
  
  virtual bool getMinParameter 
    (double& parameter_OUT) const = 0;
  /* return true if <isPeriodic ()> */
  
  virtual bool getMaxParameter 
    (double& parameter_OUT) const = 0;
  /* return true if <isPeriodic ()> */

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_LoopEdgeUse_A {
 public:
  WH_TPL3D_LoopEdgeUse_A ();
  virtual ~WH_TPL3D_LoopEdgeUse_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Loop_A* loop () const;

  WH_TPL3D_Edge_A* edge () const;

  WH_TPL3D_Curve2D_A* curve () const;

  virtual bool isPeriodic () const;

  virtual double parameterOnCurve0 () const;
  /* valid only if <!isPeriodic ()> */

  virtual double parameterOnCurve1 () const;
  /* valid only if <!isPeriodic ()> */

  virtual WH_Vector2D parameterOnSurface0 () const;
  /* valid only if <!isPeriodic ()> */

  virtual WH_Vector2D parameterOnSurface1 () const;
  /* valid only if <!isPeriodic ()> */

  virtual double lengthOnSurface () const;

  /* derived */
  
 protected:
  WH_TPL3D_Loop_A* _loop;

  WH_TPL3D_Edge_A* _edge;

  WH_TPL3D_Curve2D_A* _curve;  /* OWN */
  
  /* base */

  /* derived */

};

class WH_TPL3D_Loop_A {
 public:
  enum LoopType {
    NORMAL,
    SINGLE_VERTEX,
    SINGLE_EDGE
  };

  WH_TPL3D_Loop_A ();
  virtual ~WH_TPL3D_Loop_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Face_A* face () const;

  const vector<WH_TPL3D_LoopVertexUse_A*>& vertexUse_s () const;

  const vector<WH_TPL3D_LoopEdgeUse_A*>& edgeUse_s () const;

  virtual LoopType loopType () const;

  virtual void getVertexs 
    (vector<WH_TPL3D_Vertex_A*>& vertex_s_OUT) const;

  virtual void getEdges 
    (vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const;

  /* derived */
  
 protected:
  WH_TPL3D_Face_A* _face;

  vector<WH_TPL3D_LoopVertexUse_A*> _vertexUse_s;  /* OWN */
  
  vector<WH_TPL3D_LoopEdgeUse_A*> _edgeUse_s;  /* OWN */

  /* base */

  /* derived */

};

class WH_TPL3D_Surface_A {
 public:
  WH_TPL3D_Surface_A ();
  virtual ~WH_TPL3D_Surface_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual bool isPlane () const = 0;

  virtual bool isPeriodic () const = 0;

  virtual WH_Vector3D positionAt 
    (const WH_Vector2D& parameter) const = 0;

  virtual WH_Vector3D normalAt 
    (const WH_Vector2D& parameter) const = 0;

  virtual double uLengthBetween 
    (double vParameter, 
     double uParameter0, double uParameter1) const = 0;

  virtual double vLengthBetween 
    (double uParameter, 
     double vParameter0, double vParameter1) const = 0;

  virtual bool contains 
    (const WH_Vector3D& position) const = 0;

  virtual WH_Vector2D parameterAt 
    (const WH_Vector3D& position) const = 0;

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_Face_A {
 public:
  enum FaceType {
    SHEET,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };

  WH_TPL3D_Face_A ();
  virtual ~WH_TPL3D_Face_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Body_A* body () const;

  WH_TPL3D_Loop_A* outerLoop () const;

  const vector<WH_TPL3D_Loop_A*>& innerLoop_s () const;

  const vector<WH_TPL3D_Loop_A*>& otherLoop_s () const;

  WH_TPL3D_Surface_A* surface () const;

  virtual void getLoops 
    (vector<WH_TPL3D_Loop_A*>& loop_s_OUT) const;

  virtual FaceType faceType () const = 0;

  virtual bool orientationIsSameAsSurfaceNormal () const = 0;
  /* valid only if <faceType () == OUTER_BOUNARY> */

  /* derived */
  
 protected:
  WH_TPL3D_Body_A* _body;

  WH_TPL3D_Loop_A* _outerLoop;  /* OWN */

  vector<WH_TPL3D_Loop_A*> _innerLoop_s;  /* OWN */

  vector<WH_TPL3D_Loop_A*> _otherLoop_s;  /* OWN */

  WH_TPL3D_Surface_A* _surface;  /* OWN */
  
  /* base */

  /* derived */

};

class WH_TPL3D_Volume_A {
 public:
  WH_TPL3D_Volume_A ();
  virtual ~WH_TPL3D_Volume_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_TPL3D_Body_A* body () const;

  const vector<WH_TPL3D_Face_A*>& face_s () const;

  virtual void getVertexs 
    (vector<WH_TPL3D_Vertex_A*>& vertex_s_OUT) const;

  virtual void getEdges 
    (vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const;

  /* derived */
  
 protected:
  WH_TPL3D_Body_A* _body;

  vector<WH_TPL3D_Face_A*> _face_s;
  
  /* base */

  /* derived */

};

class WH_TPL3D_Body_A {
 public:
  WH_TPL3D_Body_A ();
  virtual ~WH_TPL3D_Body_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  const vector<WH_TPL3D_Vertex_A*>& vertex_s () const;

  const vector<WH_TPL3D_Edge_A*>& edge_s () const;

  const vector<WH_TPL3D_Face_A*>& face_s () const;

  const vector<WH_TPL3D_Volume_A*>& volume_s () const;

  /* derived */
  
 protected:
  vector<WH_TPL3D_Vertex_A*> _vertex_s;  /* OWN */

  vector<WH_TPL3D_Edge_A*> _edge_s;  /* OWN */

  vector<WH_TPL3D_Face_A*> _face_s;  /* OWN */

  vector<WH_TPL3D_Volume_A*> _volume_s;  /* OWN */
  
  /* base */

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/topology3d_inline.cc>
#endif

#endif /* WH_HEADER_WH_TOPOLOGY3D */
