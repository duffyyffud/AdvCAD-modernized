/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for poly_tpl3d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_COMMON
#include <WH/common.h>
#define WH_INCLUDED_WH_COMMON
#endif

#ifndef WH_INCLUDED_WH_SCALAR
#include <WH/scalar.h>
#define WH_INCLUDED_WH_SCALAR
#endif

#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif

#ifndef WH_INCLUDED_WH_TOPOLOGY3D
#include <WH/topology3d.h>
#define WH_INCLUDED_WH_TOPOLOGY3D
#endif

class WH_TPL3D_PolyVertex;
class WH_TPL3D_PolyCurve;
class WH_TPL3D_PolyEdge;
class WH_TPL3D_PolyLoopVertexUse;
class WH_TPL3D_PolyCurve2D;
class WH_TPL3D_PolyLoopEdgeUse;
class WH_TPL3D_PolyLoop;
class WH_TPL3D_PolySurface;
class WH_TPL3D_PolyFace;
class WH_TPL3D_PolyVolume;
class WH_TPL3D_PolyBody;

class WH_TPL3D_PolyVertex : public WH_TPL3D_Vertex_A {
 public:
  WH_TPL3D_PolyVertex 
    (WH_TPL3D_PolyBody* body,
     const WH_Vector3D& point);
  virtual ~WH_TPL3D_PolyVertex ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addConnectedFace 
    (WH_TPL3D_PolyFace* face);

  virtual void addConnectedEdge 
    (WH_TPL3D_PolyEdge* edge);

  /* derived */
  virtual WH_Vector3D point () const;
  
 protected:
  WH_Vector3D _point;
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyCurve : public WH_TPL3D_Curve_A {
 public:
  WH_TPL3D_PolyCurve 
    (const WH_Line3D& line);
  virtual ~WH_TPL3D_PolyCurve ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_Line3D line () const;

  /* derived */
  virtual bool isLine () const;
  virtual bool isPeriodic () const;
  virtual WH_Vector3D positionAt 
    (double parameter) const;
  virtual WH_Vector3D directionAt 
    (double parameter) const;
  virtual double lengthBetween 
    (double parameter0, double parameter1) const;
  virtual bool contains 
    (const WH_Vector3D& position) const;
  virtual double parameterAt 
    (const WH_Vector3D& position) const;
  virtual bool getLength 
    (double& length_OUT) const;
  virtual bool getMinParameter 
    (double& parameter_OUT) const;
  virtual bool getMaxParameter 
    (double& parameter_OUT) const;
  
 protected:
  WH_Line3D _line;
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyEdge : public WH_TPL3D_Edge_A {
 public:
  WH_TPL3D_PolyEdge 
    (WH_TPL3D_PolyBody* body,
     WH_TPL3D_PolyVertex* vertex0,
     WH_TPL3D_PolyVertex* vertex1);
  virtual ~WH_TPL3D_PolyEdge ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addConnectedFace 
    (WH_TPL3D_PolyFace* face);

  virtual void updateConnection ();

  /* derived */
  
 protected:

  /* base */

  /* derived */

};

class WH_TPL3D_PolyLoopVertexUse : public WH_TPL3D_LoopVertexUse_A {
 public:
  WH_TPL3D_PolyLoopVertexUse 
    (WH_TPL3D_PolyLoop* loop,
     WH_TPL3D_PolyVertex* vertex);
  virtual ~WH_TPL3D_PolyLoopVertexUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyCurve2D : public WH_TPL3D_Curve2D_A {
 public:
  WH_TPL3D_PolyCurve2D 
    (const WH_Line2D& line);
  virtual ~WH_TPL3D_PolyCurve2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_Line2D line () const;

  /* derived */
  virtual bool isLine () const;
  virtual bool isPeriodic () const;
  virtual WH_Vector2D positionAt 
    (double parameter) const;
  virtual WH_Vector2D directionAt 
    (double parameter) const;
  virtual double lengthBetween 
    (double parameter0, double parameter1) const;
  virtual bool contains 
    (const WH_Vector2D& position) const;
  virtual double parameterAt 
    (const WH_Vector2D& position) const;
  virtual bool getLength 
    (double& length_OUT) const;
  virtual bool getMinParameter 
    (double& parameter_OUT) const;
  virtual bool getMaxParameter 
    (double& parameter_OUT) const;
  
 protected:
  WH_Line2D _line;
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyLoopEdgeUse : public WH_TPL3D_LoopEdgeUse_A {
 public:
  WH_TPL3D_PolyLoopEdgeUse 
    (WH_TPL3D_PolyLoop* loop,
     WH_TPL3D_PolyEdge* edge);
  virtual ~WH_TPL3D_PolyLoopEdgeUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyLoop : public WH_TPL3D_Loop_A {
 public:
  WH_TPL3D_PolyLoop 
    (WH_TPL3D_PolyFace* face);
  virtual ~WH_TPL3D_PolyLoop ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addVertex 
    (WH_TPL3D_PolyVertex* vertex);

  virtual void makeEdgeUses ();

  virtual void updateConnection ();

  /* derived */
  
 protected:

  /* base */

  /* derived */

};

class WH_TPL3D_PolySurface : public WH_TPL3D_Surface_A {
 public:
  WH_TPL3D_PolySurface 
    (const WH_Plane3D& plane);
  virtual ~WH_TPL3D_PolySurface ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_Plane3D plane () const;

  /* derived */
  virtual bool isPlane () const;
  virtual bool isPeriodic () const;
  virtual WH_Vector3D positionAt 
    (const WH_Vector2D& parameter) const;
  virtual WH_Vector3D normalAt 
    (const WH_Vector2D& parameter) const;
  virtual double uLengthBetween 
    (double vParameter, 
     double uParameter0, double uParameter1) const;
  virtual double vLengthBetween 
    (double uParameter, 
     double vParameter0, double vParameter1) const;
  virtual bool contains 
    (const WH_Vector3D& position) const;
  virtual WH_Vector2D parameterAt 
    (const WH_Vector3D& position) const;
  
 protected:
  WH_Plane3D _plane;
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyFace : public WH_TPL3D_Face_A {
 public:
  WH_TPL3D_PolyFace 
    (WH_TPL3D_PolyBody* body,
     FaceType faceType);
  virtual ~WH_TPL3D_PolyFace ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void setOuterLoop 
    (WH_TPL3D_PolyLoop* loop,   /* adopt */
     const WH_Vector3D& outsideNormal);

  virtual void addInnerLoop 
    (WH_TPL3D_PolyLoop* loop   /* adopt */);

  virtual void makeEdgeUses ();

  virtual void updateConnection ();

  /* derived */
  virtual FaceType faceType () const;
  virtual bool orientationIsSameAsSurfaceNormal () const;
  
 protected:
  FaceType _faceType;
  bool _orientationIsSameAsSurfaceNormal;
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyVolume : public WH_TPL3D_Volume_A {
 public:
  WH_TPL3D_PolyVolume 
    (WH_TPL3D_PolyBody* body);
  virtual ~WH_TPL3D_PolyVolume ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addFace (WH_TPL3D_PolyFace* face);

  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

class WH_TPL3D_PolyBody : public WH_TPL3D_Body_A {
 public:
  WH_TPL3D_PolyBody ();
  virtual ~WH_TPL3D_PolyBody ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addVertex 
    (WH_TPL3D_PolyVertex* vertex   /* adopt */);

  virtual void addEdge 
    (WH_TPL3D_PolyEdge* edge   /* adopt */);

  virtual WH_TPL3D_PolyEdge* findEdge 
    (WH_TPL3D_PolyVertex* vertex0, 
     WH_TPL3D_PolyVertex* vertex1);

  virtual void addFace 
    (WH_TPL3D_PolyFace* face   /* adopt */);

  virtual void addVolume 
    (WH_TPL3D_PolyVolume* volume   /* adopt */);

  virtual void makeEdgeUses ();

  virtual void updateConnection ();
  
  /* derived */
  
 protected:
  
  /* base */

  /* derived */

};

