/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_brep.cc : boundary representation module of 3-D geometry
   modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_brep.h"



/* class WH_GM3D_Body */

WH_GM3D_Body
::WH_GM3D_Body (bool isRegular) 
{
  WH_CVR_LINE;

  _isRegular = isRegular;

  _isConsistent = false;
}

WH_GM3D_Body
::~WH_GM3D_Body () 
{
  WH_CVR_LINE;

  WH_T_Delete (_vertex_s);
  WH_T_Delete (_edge_s);
  WH_T_Delete (_face_s);
}

bool WH_GM3D_Body
::checkInvariant () const
{
  WH_CVR_LINE;

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_Body
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_T_AssureInvariant (_vertex_s);
    WH_T_AssureInvariant (_edge_s);
    WH_T_AssureInvariant (_face_s);


  
    /* count number of vertexs of each type */
    int nVertexs = (int)this->vertex_s ().size ();
    int dotVertexCount = 0;
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      WH_ASSERT(vertex_i->body () == this);
      if (vertex_i->isDot ()) {
	WH_CVR_LINE;
	dotVertexCount++;
      }
    }

    /* count number of edges of each type */
    int nEdges = (int)this->edge_s ().size ();
    int wireEdgeCount = 0;
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = this->edge_s ().begin ();
	 i_edge != this->edge_s ().end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      WH_ASSERT(edge_i->body () == this);
      if (edge_i->isWire ()) {
	WH_CVR_LINE;
	wireEdgeCount++;
      }
    }

    /* count number of faces of each type */
    int nFaces = (int)this->face_s ().size ();
    int sheetFaceCount = 0;
    int outerBoundaryFaceCount = 0;
    int innerBoundaryFaceCount = 0;
    for (vector<WH_GM3D_Face*>::const_iterator 
	   i_face = this->face_s ().begin ();
	 i_face != this->face_s ().end ();
	 i_face++) {
      WH_GM3D_Face* face_i = (*i_face);
      
      WH_ASSERT(face_i->body () == this);
      switch (face_i->faceType ()) {
      case WH_GM3D_Face::SHEET:
	WH_CVR_LINE;
	sheetFaceCount++;
	break;
      case WH_GM3D_Face::INNER_BOUNDARY:
	WH_CVR_LINE;
	innerBoundaryFaceCount++;
	break;
      case WH_GM3D_Face::OUTER_BOUNDARY:
	WH_CVR_LINE;
	outerBoundaryFaceCount++;
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }

    switch (this->bodyType ()) {
    case EMPTY:
      WH_CVR_LINE;

      WH_ASSERT(nVertexs == 0);

      WH_ASSERT(nEdges == 0);

      WH_ASSERT(nFaces == 0);

      break;
    case DOT_SET:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(0 < dotVertexCount);
      WH_ASSERT(nVertexs == dotVertexCount);

      WH_ASSERT(nEdges == 0);

      WH_ASSERT(nFaces == 0);

      break;
    case WIRE_SET:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(dotVertexCount == 0);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(0 < wireEdgeCount);
      WH_ASSERT(nEdges == wireEdgeCount);

      WH_ASSERT(nFaces == 0);

      break;
    case SHEET_SET:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(dotVertexCount == 0);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(wireEdgeCount == 0);

      WH_ASSERT(0 < nFaces);
      WH_ASSERT(0 < sheetFaceCount);
      WH_ASSERT(outerBoundaryFaceCount == 0);
      WH_ASSERT(innerBoundaryFaceCount == 0);
      WH_ASSERT(nFaces == sheetFaceCount);

      break;
    case VOLUME:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(dotVertexCount == 0);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(wireEdgeCount == 0);

      WH_ASSERT(0 < nFaces);
      WH_ASSERT(sheetFaceCount == 0);
      WH_ASSERT(0 < outerBoundaryFaceCount);
      WH_ASSERT(innerBoundaryFaceCount == 0);
      WH_ASSERT(nFaces == outerBoundaryFaceCount);

      break;
    case OTHER:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(0 <= dotVertexCount);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(0 <= wireEdgeCount);

      WH_ASSERT(0 <= nFaces);
      WH_ASSERT(0 <= sheetFaceCount);
      WH_ASSERT(0 <= outerBoundaryFaceCount);
      WH_ASSERT(0 <= innerBoundaryFaceCount);

      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
    
    if (this->isRegular ()) {
      BodyType bodyType = this->bodyType ();
      WH_ASSERT(bodyType == EMPTY
		|| bodyType == VOLUME);
      if (bodyType == VOLUME) {
	for (vector<WH_GM3D_Face*>::const_iterator 
	       i_face = this->face_s ().begin ();
	     i_face != this->face_s ().end ();
	     i_face++) {
	  WH_GM3D_Face* face_i = (*i_face);
	  WH_ASSERT(face_i->isRegular ());
	}
      }
    }

    WH_ASSERT(!WH_T_HasDuplication (this->vertex_s ()));

    /* check overlap of vertexs */
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);

      for (vector<WH_GM3D_Vertex*>::const_iterator 
	     j_vertex = i_vertex + 1;
	   j_vertex != this->vertex_s ().end ();
	   j_vertex++) {
	WH_GM3D_Vertex* vertex_j = (*j_vertex);
	
	WH_ASSERT(WH_ne (vertex_j->point (), vertex_i->point ()));
      }
    }

    WH_ASSERT(!WH_T_HasDuplication (this->edge_s ()));

    /* check intersection of edges */
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = this->edge_s ().begin ();
	 i_edge != this->edge_s ().end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      WH_Segment3D segment_i = edge_i->segment ();

      for (vector<WH_GM3D_Edge*>::const_iterator 
	     j_edge = i_edge + 1;
	   j_edge != this->edge_s ().end ();
	   j_edge++) {
	WH_GM3D_Edge* edge_j = (*j_edge);
	WH_Segment3D segment_j = edge_j->segment ();
	
	WH_SegmentIntersector3D intersector (segment_j, segment_i);  
	intersector.perform ();
	WH_ASSERT(!intersector.intersects ()
		  || intersector.intersectsAtVertexOnly ());
      }
    }

    WH_ASSERT(!WH_T_HasDuplication (this->face_s ()));
    
    /* check intersection of faces */
    /* NEED TO REDEFINE */
    
  }
  
  return true;
}

void WH_GM3D_Body
::addVertex (WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->vertex_s (), vertex));
  {
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      WH_ASSERT(WH_ne (vertex_i->point (), vertex->point ()));
    }
  }
#endif

  _vertex_s.push_back (vertex);
}

void WH_GM3D_Body
::addEdge (WH_GM3D_Edge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(edge != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->edge_s (), edge));
#endif

  _edge_s.push_back (edge);
}

void WH_GM3D_Body
::addFace (WH_GM3D_Face* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(face != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->face_s (), face));
#endif

  _face_s.push_back (face);
}

void WH_GM3D_Body
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->fix ();
  }
  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = _edge_s.begin ();
       i_edge != _edge_s.end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    edge_i->fix ();
  }
  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->fix ();
  }

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::changeAsNonRegular ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  _isRegular = false;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::clear (bool isRegular) 
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = false;
  _isRegular = isRegular;

  WH_T_Delete (_face_s);
  _face_s.clear ();

  WH_T_Delete (_edge_s);
  _edge_s.clear ();

  WH_T_Delete (_vertex_s);
  _vertex_s.clear ();

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::copyEdge
(WH_GM3D_Edge* edgeFrom,
 WH_GM3D_Edge* newEdge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edgeFrom != WH_NULL);
  WH_ASSERT(edgeFrom->isConsistent ());
  WH_ASSERT(newEdge != WH_NULL);
  WH_ASSERT(!newEdge->isConsistent ());

  WH_GM3D_Vertex* firstVertex 
    = this->findVertex (edgeFrom->firstVertexUse ()->vertex ()->point ());
  WH_ASSERT(firstVertex != WH_NULL);

  WH_GM3D_Vertex* lastVertex
    = this->findVertex (edgeFrom->lastVertexUse ()->vertex ()->point ());
  WH_ASSERT(lastVertex != WH_NULL);

  newEdge->setVertexs (firstVertex, lastVertex);
}

void WH_GM3D_Body
::copyLoop 
(WH_GM3D_Loop* loopFrom,
 WH_GM3D_Loop* newLoop)
{
  /* PRE-CONDITION */
  WH_ASSERT(loopFrom != WH_NULL);
  WH_ASSERT(loopFrom->isConsistent ());
  WH_ASSERT(newLoop != WH_NULL);
  WH_ASSERT(!newLoop->isConsistent ());

  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = loopFrom->vertexUse_s ().begin ();
       i_vertexUse != loopFrom->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_GM3D_Vertex* vertex
      = this->findVertex (vertexUse_i->vertex ()->point ());
    WH_ASSERT(vertex != WH_NULL);
    newLoop->addVertex (vertex);
  }
}

void WH_GM3D_Body
::copyFace 
(WH_GM3D_Face* faceFrom,
 WH_GM3D_Face* newFace)
{
  /* PRE-CONDITION */
  WH_ASSERT(faceFrom != WH_NULL);
  WH_ASSERT(faceFrom->isConsistent ());
  WH_ASSERT(newFace != WH_NULL);
  WH_ASSERT(!newFace->isConsistent ());

  WH_CVR_LINE;

  newFace->setSides (faceFrom->frontSide ()->isInsideVolume (),
		     faceFrom->backSide ()->isInsideVolume ());
  
  WH_GM3D_Loop* newOuterLoop = this->createLoop (newFace);
  this->copyLoop (faceFrom->outerLoop (), newOuterLoop);
  newFace->setOuterLoop (newOuterLoop);
  
  for (vector<WH_GM3D_Loop*>::const_iterator 
	 i_loop = faceFrom->innerLoop_s ().begin ();
       i_loop != faceFrom->innerLoop_s ().end ();
       i_loop++) {
    WH_GM3D_Loop* loop_i = (*i_loop);
    WH_GM3D_Loop* newLoop = this->createLoop (newFace);
    this->copyLoop (loop_i, newLoop);
    newFace->addInnerLoop (newLoop);
  }

  for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	 i_vertexUse = faceFrom->offLoopVertexUse_s ().begin ();
       i_vertexUse != faceFrom->offLoopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_GM3D_Vertex* vertex
      = this->findVertex (vertexUse_i->vertex ()->point ());
    WH_ASSERT(vertex != WH_NULL);
    newFace->addOffLoopVertex (vertex);
  }

  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = faceFrom->offLoopEdgeUse_s ().begin ();
       i_edgeUse != faceFrom->offLoopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);

    WH_GM3D_Edge* edge 
      = this->findEdge 
      (edgeUse_i->edge ()->firstVertexUse ()->vertex ()->point (),
       edgeUse_i->edge ()->lastVertexUse ()->vertex ()->point ());
    WH_ASSERT(edge != WH_NULL);
    newFace->addOffLoopEdge (edge);
  }
}

void WH_GM3D_Body
::copyFrom (WH_GM3D_Body* body)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;

  this->clear (body->isRegular ());

  _isConsistent = false;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = body->vertex_s ().begin ();
       i_vertex != body->vertex_s ().end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    WH_GM3D_Vertex* newVertex 
      = this->createVertex (vertex_i->point ());
    this->addVertex (newVertex);
  }

  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = body->edge_s ().begin ();
       i_edge != body->edge_s ().end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    WH_GM3D_Edge* newEdge = this->createEdge ();
    this->copyEdge (edge_i, newEdge);
    this->addEdge (newEdge);
  }

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = body->face_s ().begin ();
       i_face != body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    WH_GM3D_Face* newFace = this->createFace (face_i->plane ());
    this->copyFace (face_i, newFace);
    this->addFace (newFace);
  }

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::translate (const WH_Vector3D& offset)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->translate (offset);
  }
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->updatePlane ();
  }

  this->fix ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::rotateByXAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->rotateByXAxis (center, angle);
  }
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->updatePlane ();
  }

  this->fix ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::rotateByYAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->rotateByYAxis (center, angle);
  }
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->updatePlane ();
  }

  this->fix ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::rotateByZAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->rotateByZAxis (center, angle);
  }
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->updatePlane ();
  }

  this->fix ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM3D_Body
::scale (const WH_Vector3D& center, double ratio)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    vertex_i->scale (center, ratio);
  }
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    face_i->updatePlane ();
  }

  this->fix ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM3D_Vertex* WH_GM3D_Body
::createVertex 
(const WH_Vector3D& point)
{
  WH_GM3D_Vertex* result 
    = new WH_GM3D_Vertex (this, point);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_Edge* WH_GM3D_Body
::createEdge ()
{
  WH_GM3D_Edge* result 
    = new WH_GM3D_Edge (this);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_Face* WH_GM3D_Body
::createFace 
(const WH_Plane3D& plane)
{
  WH_GM3D_Face* result 
    = new WH_GM3D_Face (this, plane);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_Loop* WH_GM3D_Body
::createLoop (WH_GM3D_Face* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  WH_ASSERT(face->body () == this);

  WH_GM3D_Loop* result 
    = new WH_GM3D_Loop (face);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

bool WH_GM3D_Body
::isConsistent () const
{
  return _isConsistent;
}

bool WH_GM3D_Body
::isRegular () const
{
  return _isRegular;
}

const vector<WH_GM3D_Vertex*>& WH_GM3D_Body
::vertex_s () const
{
  return _vertex_s;
}

const vector<WH_GM3D_Edge*>& WH_GM3D_Body
::edge_s () const
{
  return _edge_s;
}

const vector<WH_GM3D_Face*>& WH_GM3D_Body
::face_s () const
{
  return _face_s;
}

WH_GM3D_Body::BodyType WH_GM3D_Body
::bodyType () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  BodyType result = OTHER;

  int nVertexs = (int)this->vertex_s ().size ();
  int nEdges = (int)this->edge_s ().size ();
  int nFaces = (int)this->face_s ().size ();

  if (nVertexs == 0
      && nEdges == 0
      && nFaces == 0) {
    WH_CVR_LINE;
    result = EMPTY;
  } else if (nEdges == 0
	     && nFaces == 0) {
    WH_CVR_LINE;
    result = DOT_SET;
  } else if (nFaces == 0) {
    WH_CVR_LINE;

    int dotVertexCount = 0;
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      if (vertex_i->isDot ()) {
	WH_CVR_LINE;                          /* NOT COVERED */
	dotVertexCount++;
      }
    }

    if (0 < dotVertexCount) {
      WH_CVR_LINE;                          /* NOT COVERED */
      result = OTHER;
    } else {
      WH_CVR_LINE;
      result = WIRE_SET;
    }
  } else {
    WH_CVR_LINE;

    int dotVertexCount = 0;
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      if (vertex_i->isDot ()) {
	WH_CVR_LINE;                          /* NOT COVERED */
	dotVertexCount++;
      }
    }

    int wireEdgeCount = 0;
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = this->edge_s ().begin ();
	 i_edge != this->edge_s ().end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      if (edge_i->isWire ()) {
	WH_CVR_LINE;                          /* NOT COVERED */
	wireEdgeCount++;
      }
    }

    if (0 < dotVertexCount || 0 < wireEdgeCount) {
      WH_CVR_LINE;                          /* NOT COVERED */
      result = OTHER;
    } else {
      WH_CVR_LINE;

      int sheetFaceCount = 0;
      int outerBoundaryFaceCount = 0;
      int innerBoundaryFaceCount = 0;
      for (vector<WH_GM3D_Face*>::const_iterator 
	     i_face = this->face_s ().begin ();
	   i_face != this->face_s ().end ();
	   i_face++) {
	WH_GM3D_Face* face_i = (*i_face);
	
	switch (face_i->faceType ()) {
	case WH_GM3D_Face::SHEET:
	  WH_CVR_LINE;
	  sheetFaceCount++;
	  break;
	case WH_GM3D_Face::INNER_BOUNDARY:
	  WH_CVR_LINE;
	  innerBoundaryFaceCount++;
	  break;
	case WH_GM3D_Face::OUTER_BOUNDARY:
	  WH_CVR_LINE;
	  outerBoundaryFaceCount++;
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
      }

      if (0 < sheetFaceCount 
	  && outerBoundaryFaceCount == 0 
	  && innerBoundaryFaceCount == 0) {
	WH_CVR_LINE;
	result = SHEET_SET;
      } else if (sheetFaceCount == 0
		 && 0 < outerBoundaryFaceCount 
		 && innerBoundaryFaceCount == 0) {
	WH_CVR_LINE;
	result = VOLUME;
      } else {
	WH_CVR_LINE;
	result = OTHER;
      }
    }

  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case EMPTY:
    WH_ASSERT(nVertexs == 0);
    WH_ASSERT(nEdges == 0);
    WH_ASSERT(nFaces == 0);
    break;
  case DOT_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(nEdges == 0);
    WH_ASSERT(nFaces == 0);
    break;
  case WIRE_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(nFaces == 0);
    break;
  case SHEET_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 < nFaces);
    break;
  case VOLUME:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 < nFaces);
    break;
  case OTHER:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 <= nFaces);
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
#endif  

  return result;
}

WH_GM3D_Vertex* WH_GM3D_Body
::findVertex
(const WH_Vector3D& point) const
{
  WH_CVR_LINE;

  WH_GM3D_Vertex* result = WH_NULL;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    if (WH_eq (vertex_i->point (), point)) {
      WH_CVR_LINE;
      result = vertex_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

WH_GM3D_Edge* WH_GM3D_Body
::findEdge
(const WH_Vector3D& point0,
 const WH_Vector3D& point1) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (point0, point1));

  WH_CVR_LINE;

  WH_GM3D_Edge* result = WH_NULL;

  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    WH_Segment3D seg = edge_i->segment ();
    if (seg.hasVertexAt (point0) && seg.hasVertexAt (point1)) {
      WH_CVR_LINE;
      result = edge_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

WH_GM3D_Face* WH_GM3D_Body
::findFace
(const vector<WH_Vector3D>& point_s) const
{
  /* PRE-CONDITION */
  WH_ASSERT(3 <= point_s.size ());

  WH_CVR_LINE;

  WH_GM3D_Face* result = WH_NULL;

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = this->face_s ().begin ();
       i_face != this->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    WH_Polygon3D poly = face_i->outerLoop ()->polygon ();
    if (poly.hasVertexAtEveryPointIn (point_s)) {
      WH_CVR_LINE;
      result = face_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

void WH_GM3D_Body
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->vertex_s ().size ());
  
  WH_CVR_LINE;
  
  minRange_OUT = WH_Vector3D::hugeValue ();
  maxRange_OUT = -WH_Vector3D::hugeValue ();
  
  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    minRange_OUT = WH_min (minRange_OUT, vertex_i->point ());
    maxRange_OUT = WH_max (maxRange_OUT, vertex_i->point ());
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
#endif  
}



/* not yet covered 
WH_GM3D_Body::BodyType WH_GM3D_Body
::bodyType () const
*/



