/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm2d_brep.cc */

#pragma once
#ifndef WH_INCLUDED_WH_POLYGON2D
#include <WH/polygon2d.h>
#define WH_INCLUDED_WH_POLYGON2D
#endif

class WH_GM2D_Vertex;
class WH_GM2D_EdgeVertexUse;
class WH_GM2D_EdgeSide;
class WH_GM2D_Edge;
class WH_GM2D_Body;

class WH_GM2D_Vertex {
 public:
  WH_GM2D_Vertex 
    (WH_GM2D_Body* body,
     const WH_Vector2D& point);
  virtual ~WH_GM2D_Vertex ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void translate (const WH_Vector2D& offset);
  /* called from WH_GM2D_Body */

  virtual void rotate (const WH_Vector2D& center, double angle);
  /* called from WH_GM2D_Body */

  virtual void scale (const WH_Vector2D& center, double ratio);
  /* called from WH_GM2D_Body */

  virtual void fix ();
  /* called from WH_GM2D_Body */

  bool isConsistent () const;

  WH_GM2D_Body* body () const;

  const vector<WH_GM2D_EdgeVertexUse*>& edgeVertexUse_s () const;
  
  WH_Vector2D point () const;

  virtual void getEdges 
    (vector<WH_GM2D_Edge*>& edge_s_OUT) const;

  virtual bool isDot () const;

  virtual bool isVolume () const;

  enum VolumeMergeType {
    MERGE_CONVEX, 
    MERGE_CONCAVE, 
    MERGE_180,
    MERGE_MORE_THAN_TWO_EDGES
  };
  virtual VolumeMergeType volumeMergeType () const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM2D_Body* _body;  /* owned by */

  vector<WH_GM2D_EdgeVertexUse*> _edgeVertexUse_s;
  /* not own, reverse reference */
  
  WH_Vector2D _point;

  /* base */
  virtual bool edgeVertexUsesAreSorted () const;

  virtual void collectEdgeVertexUses ();

  virtual void sortEdgeVertexUses ();
  
  /* derived */
  
};

class WH_GM2D_EdgeVertexUse {
 public:
  WH_GM2D_EdgeVertexUse 
    (WH_GM2D_Edge* edge,
     WH_GM2D_Vertex* vertex);
  virtual ~WH_GM2D_EdgeVertexUse ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM2D_Edge */

  bool isConsistent () const;

  WH_GM2D_Edge* edge () const;
  
  WH_GM2D_Vertex* vertex () const;

  virtual double parameter () const;

  virtual void getConnectedEdges 
    (vector<WH_GM2D_Edge*>& edge_s_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM2D_Edge* _edge;  /* owned by */
  
  WH_GM2D_Vertex* _vertex;
  
  /* base */
  
  /* derived */
  
};

class WH_GM2D_EdgeSide {
 public:
  WH_GM2D_EdgeSide 
    (WH_GM2D_Edge* edge,
     bool isInsideVolume);
  virtual ~WH_GM2D_EdgeSide ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void fix ();
  /* called from WH_GM2D_Edge */

  bool isConsistent () const;

  WH_GM2D_Edge* edge () const;

  bool isInsideVolume () const;
  
  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM2D_Edge* _edge;  /* owned by */

  bool _isInsideVolume;
  
  /* base */
  
  /* derived */
  
};

class WH_GM2D_Edge {
 public:
  WH_GM2D_Edge 
    (WH_GM2D_Body* body);
  virtual ~WH_GM2D_Edge ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setVertexs 
    (WH_GM2D_Vertex* firstVertex,
     WH_GM2D_Vertex* lastVertex);
  
  virtual void setSides 
    (bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);

  virtual void fix ();
  /* called from WH_GM2D_Body */

  bool isConsistent () const;

  WH_GM2D_Body* body () const;
  
  WH_GM2D_EdgeVertexUse* firstVertexUse () const;
  WH_GM2D_EdgeVertexUse* lastVertexUse () const;
  
  WH_GM2D_EdgeSide* frontSide () const;
  WH_GM2D_EdgeSide* backSide () const;
  
  /* 
             front
     first -------- last
             back     
   */

  enum EdgeType {
    WIRE,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };
  virtual EdgeType edgeType () const;

  virtual WH_Segment2D segment () const;

  virtual bool getNormalToOutsideVolume 
    (WH_Vector2D& normal_OUT) const;
  
  virtual void getRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;

  virtual WH_GM2D_EdgeSide* connectedEdgeSide 
    (WH_GM2D_EdgeSide* side,
     WH_GM2D_EdgeVertexUse* vertexUse) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  WH_GM2D_Body* _body;   /* owned by */
  
  WH_GM2D_EdgeVertexUse* _firstVertexUse;   /* own */
  WH_GM2D_EdgeVertexUse* _lastVertexUse;   /* own */
  
  WH_GM2D_EdgeSide* _frontSide;   /* own */
  WH_GM2D_EdgeSide* _backSide;   /* own */
  
  /* base */

  /* factory method */
  virtual WH_GM2D_EdgeVertexUse* createEdgeVertexUse 
    (WH_GM2D_Vertex* vertex);
  virtual WH_GM2D_EdgeSide* createEdgeSide 
    (bool isInsideVolume);

  /* derived */
  
};

class WH_GM2D_Body {
 public:
  WH_GM2D_Body (bool isRegular);
  virtual ~WH_GM2D_Body ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addVertex (WH_GM2D_Vertex* vertex  /* ADOPT */);

  virtual void addEdge (WH_GM2D_Edge* edge  /* ADOPT */);

  virtual void fix ();

  virtual void changeAsNonRegular ();

  virtual void clear (bool isRegular);

  virtual void copyFrom (WH_GM2D_Body* body);

  virtual void translate (const WH_Vector2D& offset);

  virtual void rotate (const WH_Vector2D& center, double angle);

  virtual void scale (const WH_Vector2D& center, double ratio);

  /* factory method */
  virtual WH_GM2D_Vertex* createVertex 
    (const WH_Vector2D& point);
  virtual WH_GM2D_Edge* createEdge ();

  bool isConsistent () const;

  bool isRegular () const;

  const vector<WH_GM2D_Vertex*>& vertex_s () const;
  
  const vector<WH_GM2D_Edge*>& edge_s () const;

  enum BodyType {
    EMPTY,
    DOT_SET,
    WIRE_SET,
    VOLUME,
    OTHER
  };
  virtual BodyType bodyType () const;

  virtual WH_GM2D_Vertex* findVertex 
    (const WH_Vector2D& point) const;

  virtual WH_GM2D_Edge* findEdge 
    (const WH_Vector2D& point0,
     const WH_Vector2D& point1) const;

  virtual void getRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;

  /* derived */
  
 protected:
  bool _isConsistent;

  bool _isRegular;

  vector<WH_GM2D_Vertex*> _vertex_s;   /* own */
  
  vector<WH_GM2D_Edge*> _edge_s;   /* own */

  /* base */

  /* derived */
  
};
