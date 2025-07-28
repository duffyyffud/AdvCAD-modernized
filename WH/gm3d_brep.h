/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_brep.cc */

#ifndef WH_HEADER_WH_GM3D_BREP
#define WH_HEADER_WH_GM3D_BREP

#ifndef WH_INCLUDED_WH_TRIANGLE3D
#include <WH/triangle3d.h>
#define WH_INCLUDED_WH_TRIANGLE3D
#endif



class WH_GM3D_Vertex;
class WH_GM3D_EdgeVertexUse;
class WH_GM3D_Edge;
class WH_GM3D_LoopVertexUse;
class WH_GM3D_LoopEdgeUse;
class WH_GM3D_Loop;
class WH_GM3D_OffLoopVertexUse;
class WH_GM3D_OffLoopEdgeUse;
class WH_GM3D_FaceSide;
class WH_GM3D_Face;
class WH_GM3D_Body;



class WH_GM3D_Vertex {
 public:
  WH_GM3D_Vertex 
    (WH_GM3D_Body* body,
     const WH_Vector3D& point);
  virtual ~WH_GM3D_Vertex ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void translate (const WH_Vector3D& offset);
  /* called from WH_GM3D_Body */

  virtual void rotateByXAxis (const WH_Vector3D& center, double angle);
  virtual void rotateByYAxis (const WH_Vector3D& center, double angle);
  virtual void rotateByZAxis (const WH_Vector3D& center, double angle);
  /* called from WH_GM3D_Body */

  virtual void scale (const WH_Vector3D& center, double ratio);
  /* called from WH_GM3D_Body */

  virtual void fix ();
  /* called from WH_GM3D_Body */

  bool isConsistent () const;

  WH_GM3D_Body* body () const;

  const vector<WH_GM3D_EdgeVertexUse*>& edgeVertexUse_s () const;

  const vector<WH_GM3D_LoopVertexUse*>& loopVertexUse_s () const;
  
  const vector<WH_GM3D_OffLoopVertexUse*>& offLoopVertexUse_s () const;

  WH_Vector3D point () const;

  virtual void getEdges 
    (vector<WH_GM3D_Edge*>& edge_s_OUT) const;

  virtual void getFaces 
    (vector<WH_GM3D_Face*>& face_s_OUT) const;

  virtual bool isDot () const;

  virtual bool isVolume () const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Body* _body;  /* owned by */

  vector<WH_GM3D_EdgeVertexUse*> _edgeVertexUse_s;
  /* not own, reverse reference */
  
  vector<WH_GM3D_LoopVertexUse*> _loopVertexUse_s;
  /* not own, reverse reference */
  
  vector<WH_GM3D_OffLoopVertexUse*> _offLoopVertexUse_s;
  /* not own, reverse reference */

  WH_Vector3D _point;

  /* base */
  virtual void collectEdgeVertexUses ();

  virtual void collectLoopVertexUses ();

  virtual void collectOffLoopVertexUses ();
  
  /* derived */
  
};

class WH_GM3D_EdgeVertexUse {
 public:
  WH_GM3D_EdgeVertexUse 
    (WH_GM3D_Edge* edge,
     WH_GM3D_Vertex* vertex);
  virtual ~WH_GM3D_EdgeVertexUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Edge */

  bool isConsistent () const;

  WH_GM3D_Edge* edge () const;
  
  WH_GM3D_Vertex* vertex () const;

  virtual double parameter () const;

  virtual void getConnectedEdges 
    (vector<WH_GM3D_Edge*>& edge_s_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Edge* _edge;  /* owned by */
  
  WH_GM3D_Vertex* _vertex;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_Edge {
 public:
  WH_GM3D_Edge 
    (WH_GM3D_Body* body);
  virtual ~WH_GM3D_Edge ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setVertexs 
    (WH_GM3D_Vertex* firstVertex, 
     WH_GM3D_Vertex* lastVertex);

  virtual void fix ();
  /* called from WH_GM3D_Body */

  bool isConsistent () const;

  WH_GM3D_Body* body () const;
  
  WH_GM3D_EdgeVertexUse* firstVertexUse () const;
  WH_GM3D_EdgeVertexUse* lastVertexUse () const;

  const vector<WH_GM3D_LoopEdgeUse*>& loopEdgeUse_s () const;
  
  const vector<WH_GM3D_OffLoopEdgeUse*>& offLoopEdgeUse_s () const;

  virtual WH_Segment3D segment () const;
  
  virtual void getFaces 
    (vector<WH_GM3D_Face*>& face_s_OUT) const;

  virtual bool isWire () const;

  virtual bool isVolume () const;

  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Body* _body;   /* owned by */
  
  WH_GM3D_EdgeVertexUse* _firstVertexUse;   /* own */
  WH_GM3D_EdgeVertexUse* _lastVertexUse;   /* own */
  
  vector<WH_GM3D_LoopEdgeUse*> _loopEdgeUse_s;
  /* not own, reverse reference */

  vector<WH_GM3D_OffLoopEdgeUse*> _offLoopEdgeUse_s;
  /* not own, reverse reference */
  
  /* base */
  virtual void collectLoopEdgeUses ();

  virtual void collectOffLoopEdgeUses ();

  /* factory method */
  virtual WH_GM3D_EdgeVertexUse* createEdgeVertexUse 
    (WH_GM3D_Vertex* vertex);

  /* derived */
  
};

class WH_GM3D_LoopVertexUse {
 public:
  WH_GM3D_LoopVertexUse 
    (WH_GM3D_Loop* loop,
     WH_GM3D_Vertex* vertex);
  virtual ~WH_GM3D_LoopVertexUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Loop */

  bool isConsistent () const;

  WH_GM3D_Loop* loop () const;
  
  WH_GM3D_Vertex* vertex () const;

  virtual WH_Vector2D parameter () const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Loop* _loop;  /* owned by */
  
  WH_GM3D_Vertex* _vertex;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_LoopEdgeUse {
 public:
  WH_GM3D_LoopEdgeUse 
    (WH_GM3D_Loop* loop,
     WH_GM3D_Edge* edge);
  virtual ~WH_GM3D_LoopEdgeUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Loop */

  bool isConsistent () const;

  WH_GM3D_Loop* loop () const;
  
  WH_GM3D_Edge* edge () const;

  virtual WH_Segment2D parameterSegment () const;

  virtual void getConnectedFaces 
    (vector<WH_GM3D_Face*>& face_s_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Loop* _loop;  /* owned by */
  
  WH_GM3D_Edge* _edge;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_Loop {
 public:
  WH_GM3D_Loop 
    (WH_GM3D_Face* face);
  virtual ~WH_GM3D_Loop ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addVertex (WH_GM3D_Vertex* vertex);

  virtual void fix ();
  /* called from WH_GM3D_Face */

  bool isConsistent () const;

  WH_GM3D_Face* face () const;
  
  const vector<WH_GM3D_LoopVertexUse*>& vertexUse_s () const;

  const vector<WH_GM3D_LoopEdgeUse*>& edgeUse_s () const;

  virtual WH_Polygon3D polygon () const;
  
  virtual WH_Polygon2D parameterPolygon () const;
  
  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Face* _face;   /* owned by */
  
  vector<WH_GM3D_LoopVertexUse*> _vertexUse_s;   /* own */

  vector<WH_GM3D_LoopEdgeUse*> _edgeUse_s;   /* own */
  
  /* base */
  virtual void findAndAddEdgeUse 
    (WH_GM3D_LoopVertexUse* vertexUse0, 
     WH_GM3D_LoopVertexUse* vertexUse1);

  virtual void collectEdgeUses ();

  /* factory method */
  virtual WH_GM3D_LoopVertexUse* createLoopVertexUse 
    (WH_GM3D_Vertex* vertex);
  virtual WH_GM3D_LoopEdgeUse* createLoopEdgeUse 
    (WH_GM3D_Edge* edge);

  /* derived */
  
};

class WH_GM3D_OffLoopVertexUse {
 public:
  WH_GM3D_OffLoopVertexUse 
    (WH_GM3D_Face* face,
     WH_GM3D_Vertex* vertex);
  virtual ~WH_GM3D_OffLoopVertexUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Face */

  bool isConsistent () const;

  WH_GM3D_Face* face () const;
  
  WH_GM3D_Vertex* vertex () const;

  virtual WH_Vector2D parameter () const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Face* _face;  /* owned by */
  
  WH_GM3D_Vertex* _vertex;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_OffLoopEdgeUse {
 public:
  WH_GM3D_OffLoopEdgeUse 
    (WH_GM3D_Face* face,
     WH_GM3D_Edge* edge);
  virtual ~WH_GM3D_OffLoopEdgeUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Face */

  bool isConsistent () const;

  WH_GM3D_Face* face () const;
  
  WH_GM3D_Edge* edge () const;

  virtual WH_Segment2D parameterSegment () const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Face* _face;  /* owned by */
  
  WH_GM3D_Edge* _edge;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_FaceSide {
 public:
  WH_GM3D_FaceSide 
    (WH_GM3D_Face* face,
     bool isInsideVolume);
  virtual ~WH_GM3D_FaceSide ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM3D_Face */

  bool isConsistent () const;

  WH_GM3D_Face* face () const;

  bool isInsideVolume () const;
  
  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Face* _face;  /* owned by */

  bool _isInsideVolume;
  
  /* base */
  
  /* derived */
  
};

class WH_GM3D_Face {
 public:
  WH_GM3D_Face 
    (WH_GM3D_Body* body,
     const WH_Plane3D& plane);
  virtual ~WH_GM3D_Face ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setSides 
    (bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);
  /* Normal of <plane ()> is oriented toward front side. */

  virtual void setOuterLoop (WH_GM3D_Loop* loop);

  virtual void addInnerLoop (WH_GM3D_Loop* loop);
  /* Vertex order of each inner loop can be either same or reverse of
     the outer loop. */

  virtual void addOffLoopVertex (WH_GM3D_Vertex* vertex);

  virtual void addOffLoopEdge (WH_GM3D_Edge* edge);

  virtual void updatePlane ();
  /* called from WH_GM3D_Body */

  virtual void fix ();
  /* called from WH_GM3D_Body */

  bool isConsistent () const;

  WH_GM3D_Body* body () const;
  
  WH_GM3D_Loop* outerLoop () const;
  
  const vector<WH_GM3D_Loop*>& innerLoop_s () const;

  const vector<WH_GM3D_OffLoopVertexUse*>& offLoopVertexUse_s () const;

  const vector<WH_GM3D_OffLoopEdgeUse*>& offLoopEdgeUse_s () const;

  WH_GM3D_FaceSide* frontSide () const;
  WH_GM3D_FaceSide* backSide () const;
  
  WH_Plane3D plane () const;

  bool isRegular () const;

  virtual void getVertexs 
    (vector<WH_GM3D_Vertex*>& vertex_s_OUT) const;

  virtual void getEdges 
    (vector<WH_GM3D_Edge*>& edge_s_OUT) const;

  enum FaceType {
    SHEET,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };
  virtual FaceType faceType () const;

  virtual bool getNormalToOutsideVolume 
    (WH_Vector3D& normal_OUT) const;
  
  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;

  virtual WH_GM3D_FaceSide* connectedFaceSide 
    (WH_GM3D_FaceSide* side,
     WH_GM3D_LoopEdgeUse* edgeUse) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM3D_Body* _body;   /* owned by */
  
  WH_GM3D_Loop* _outerLoop;   /* own */
  
  vector<WH_GM3D_Loop*> _innerLoop_s;   /* own */

  vector<WH_GM3D_OffLoopVertexUse*> _offLoopVertexUse_s;  /* own */

  vector<WH_GM3D_OffLoopEdgeUse*> _offLoopEdgeUse_s;  /* own */

  WH_GM3D_FaceSide* _frontSide;   /* own */
  WH_GM3D_FaceSide* _backSide;   /* own */
  
  WH_Plane3D _plane;

  /* base */

  /* factory method */
  virtual WH_GM3D_OffLoopVertexUse* createOffLoopVertexUse 
    (WH_GM3D_Vertex* vertex);
  virtual WH_GM3D_OffLoopEdgeUse* createOffLoopEdgeUse 
    (WH_GM3D_Edge* edge);
  virtual WH_GM3D_FaceSide* createFaceSide 
    (bool isInsideVolume);

  /* derived */
  
};

class WH_GM3D_Body {
 public:
  WH_GM3D_Body (bool isRegular);
  virtual ~WH_GM3D_Body ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addVertex (WH_GM3D_Vertex* vertex  /* ADOPT */);

  virtual void addEdge (WH_GM3D_Edge* edge  /* ADOPT */);

  virtual void addFace (WH_GM3D_Face* face  /* ADOPT */);

  virtual void fix ();

  virtual void changeAsNonRegular ();

  virtual void clear (bool isRegular);

  virtual void copyFrom (WH_GM3D_Body* body);

  virtual void translate (const WH_Vector3D& offset);

  virtual void rotateByXAxis (const WH_Vector3D& center, double angle);
  virtual void rotateByYAxis (const WH_Vector3D& center, double angle);
  virtual void rotateByZAxis (const WH_Vector3D& center, double angle);

  virtual void scale (const WH_Vector3D& center, double ratio);

  /* factory method */
  virtual WH_GM3D_Vertex* createVertex 
    (const WH_Vector3D& point);
  virtual WH_GM3D_Edge* createEdge ();
  virtual WH_GM3D_Face* createFace 
    (const WH_Plane3D& plane);
  virtual WH_GM3D_Loop* createLoop
    (WH_GM3D_Face* face);

  bool isConsistent () const;

  bool isRegular () const;

  const vector<WH_GM3D_Vertex*>& vertex_s () const;
  
  const vector<WH_GM3D_Edge*>& edge_s () const;

  const vector<WH_GM3D_Face*>& face_s () const;

  enum BodyType {
    EMPTY,
    DOT_SET,
    WIRE_SET,
    SHEET_SET,
    VOLUME,
    OTHER
  };
  virtual BodyType bodyType () const;

  virtual WH_GM3D_Vertex* findVertex
    (const WH_Vector3D& point) const;

  virtual WH_GM3D_Edge* findEdge
    (const WH_Vector3D& point0,
     const WH_Vector3D& point1) const;

  virtual WH_GM3D_Face* findFace 
    (const vector<WH_Vector3D>& point_s) const;

  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  bool _isRegular;

  vector<WH_GM3D_Vertex*> _vertex_s;   /* own */
  
  vector<WH_GM3D_Edge*> _edge_s;   /* own */
  
  vector<WH_GM3D_Face*> _face_s;   /* own */

  /* base */
  virtual void copyEdge
    (WH_GM3D_Edge* edgeFrom,
     WH_GM3D_Edge* newEdge);

  virtual void copyLoop
    (WH_GM3D_Loop* loopFrom,
     WH_GM3D_Loop* newLoop);
    
  virtual void copyFace 
    (WH_GM3D_Face* faceFrom,
     WH_GM3D_Face* newFace);

  /* derived */
  
};



#endif /* WH_HEADER_WH_GM3D_BREP */
