/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_brep_vertex.cc : boundary representation module of 3-D
   geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_brep.h"



/* class WH_GM3D_Vertex */

WH_GM3D_Vertex
::WH_GM3D_Vertex 
(WH_GM3D_Body* body,
 const WH_Vector3D& point) 
  : _point (point)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;
  
  _body = body;

  _isConsistent = false;
}

WH_GM3D_Vertex
::~WH_GM3D_Vertex () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_Vertex
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_Vertex
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->body ()->vertex_s (), this));

    WH_ASSERT(!WH_T_HasDuplication (this->edgeVertexUse_s ()));
    for (vector<WH_GM3D_EdgeVertexUse*>::const_iterator 
	   i_vertexUse = this->edgeVertexUse_s ().begin ();
	 i_vertexUse != this->edgeVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
      WH_ASSERT(vertexUse_i->vertex () == this);
    }

    WH_ASSERT(!WH_T_HasDuplication (this->loopVertexUse_s ()));
    for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	   i_vertexUse = this->loopVertexUse_s ().begin ();
	 i_vertexUse != this->loopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
      WH_ASSERT(vertexUse_i->vertex () == this);
    }
    
    WH_ASSERT(!WH_T_HasDuplication (this->offLoopVertexUse_s ()));
    WH_ASSERT(this->offLoopVertexUse_s ().size () == 0
	      || this->offLoopVertexUse_s ().size () == 1);
    for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	   i_vertexUse = this->offLoopVertexUse_s ().begin ();
	 i_vertexUse != this->offLoopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
      WH_ASSERT(vertexUse_i->vertex () == this);
    }
    
    vector<WH_GM3D_Edge*> edge_s;
    this->getEdges 
      (edge_s);
    WH_ASSERT(!WH_T_HasDuplication (edge_s));
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = edge_s.begin ();
	 i_edge != edge_s.end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      WH_ASSERT (edge_i->firstVertexUse ()->vertex () == this
		 || edge_i->lastVertexUse ()->vertex () == this);
    }
    
    vector<WH_GM3D_Face*> face_s;
    this->getFaces 
      (face_s);
    WH_ASSERT(!WH_T_HasDuplication (face_s));
    for (vector<WH_GM3D_Face*>::const_iterator 
	   i_face = face_s.begin ();
	 i_face != face_s.end ();
	 i_face++) {
      WH_GM3D_Face* face_i = (*i_face);

      int count = 0;

      WH_GM3D_Loop* outerLoop = face_i->outerLoop ();
      for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	     i_vertexUse = outerLoop->vertexUse_s ().begin ();
	   i_vertexUse != outerLoop->vertexUse_s ().end ();
	   i_vertexUse++) {
	WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	if (vertexUse_i->vertex () == this) {
	  count++;
	}
      }

      for (vector<WH_GM3D_Loop*>::const_iterator 
	     i_loop = face_i->innerLoop_s ().begin ();
	   i_loop != face_i->innerLoop_s ().end ();
	   i_loop++) {
	WH_GM3D_Loop* loop_i = (*i_loop);

	for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	       i_vertexUse = loop_i->vertexUse_s ().begin ();
	     i_vertexUse != loop_i->vertexUse_s ().end ();
	     i_vertexUse++) {
	  WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	  if (vertexUse_i->vertex () == this) {
	    count++;
	  }
	}
      }

      for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	     i_vertexUse = face_i->offLoopVertexUse_s ().begin ();
	   i_vertexUse != face_i->offLoopVertexUse_s ().end ();
	   i_vertexUse++) {
	WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
	if (vertexUse_i->vertex () == this) {
	  count++;
	}
      }

      for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	     i_edgeUse = face_i->offLoopEdgeUse_s ().begin ();
	   i_edgeUse != face_i->offLoopEdgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
	WH_GM3D_Edge* edge = edgeUse_i->edge ();
	if (edge->firstVertexUse ()->vertex () == this
	    || edge->lastVertexUse ()->vertex () == this) {
	  count++;
	}
      }
      
      WH_ASSERT(0 < count);
    }

    int sheetFaceCount = 0;
    int outerBoundaryFaceCount = 0;
    int innerBoundaryFaceCount = 0;
    for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	   i_vertexUse = this->loopVertexUse_s ().begin ();
	 i_vertexUse != this->loopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
      
      WH_GM3D_Face* face = vertexUse_i->loop ()->face ();
      switch (face->faceType ()) {
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
    WH_ASSERT(outerBoundaryFaceCount == 0
	      || 2 <= outerBoundaryFaceCount);
    
    WH_ASSERT(!(this->isDot () && this->isVolume ()));

    if (this->isDot ()) {
      WH_CVR_LINE;
      WH_ASSERT(this->edgeVertexUse_s ().size () == 0);
      WH_ASSERT(this->loopVertexUse_s ().size () == 0);
      WH_ASSERT(this->offLoopVertexUse_s ().size () == 0);
    }

    if (this->isVolume ()) {
      WH_CVR_LINE;
      WH_ASSERT(2 <= outerBoundaryFaceCount);
    }
  }
    
  return true;
}

void WH_GM3D_Vertex
::translate (const WH_Vector3D& offset)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.translate (offset);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Vertex
::rotateByXAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.rotateByXAxis (center, angle);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Vertex
::rotateByYAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.rotateByYAxis (center, angle);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Vertex
::rotateByZAxis (const WH_Vector3D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.rotateByZAxis (center, angle);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Vertex
::scale (const WH_Vector3D& center, double ratio)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.scale (center, ratio);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Vertex
::collectEdgeVertexUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  /* <_body->edge_s ()> must have been set. */
  /* <firstVertexUse ()> and <lastVertexUse ()> of the edges must have
     been set. */

  WH_CVR_LINE;

  _edgeVertexUse_s.clear ();

  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = _body->edge_s ().begin ();
       i_edge != _body->edge_s ().end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    
    WH_ASSERT(edge_i->firstVertexUse () != WH_NULL);
    WH_ASSERT(edge_i->lastVertexUse () != WH_NULL);

    if (edge_i->firstVertexUse ()->vertex () == this) {
      WH_CVR_LINE;
      _edgeVertexUse_s.push_back (edge_i->firstVertexUse ());
    } else if (edge_i->lastVertexUse ()->vertex () == this) {
      WH_CVR_LINE;
      _edgeVertexUse_s.push_back (edge_i->lastVertexUse ());
    }
  }
}

void WH_GM3D_Vertex
::collectLoopVertexUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  /* <_body->face_s ()> must have been set. */
  /* <outerLoop ()> and <innerLoop_s ()> of the faces must have been
     set. */
  /* <vertexUse_s ()> of the loops must have been set. */

  WH_CVR_LINE;

  _loopVertexUse_s.clear ();

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _body->face_s ().begin ();
       i_face != _body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);

    WH_GM3D_Loop* outerLoop = face_i->outerLoop ();
    WH_ASSERT(outerLoop != WH_NULL);
    WH_ASSERT(3 <= outerLoop->vertexUse_s ().size ());
    
    for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	   i_vertexUse = outerLoop->vertexUse_s ().begin ();
	 i_vertexUse != outerLoop->vertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
      if (vertexUse_i->vertex () == this) {
	WH_CVR_LINE;
	_loopVertexUse_s.push_back (vertexUse_i);
      }
    }

    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = face_i->innerLoop_s ().begin ();
	 i_loop != face_i->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);

      WH_ASSERT(3 <= loop_i->vertexUse_s ().size ());
      for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	     i_vertexUse = loop_i->vertexUse_s ().begin ();
	   i_vertexUse != loop_i->vertexUse_s ().end ();
	   i_vertexUse++) {
	WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	if (vertexUse_i->vertex () == this) {
	  WH_CVR_LINE;
	  _loopVertexUse_s.push_back (vertexUse_i);
	}
      }
    }
  }
}

void WH_GM3D_Vertex
::collectOffLoopVertexUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  /* <_body->face_s ()> must have been set. */
  /* <offLoopVertexUse_s ()> of the faces must have been set. */

  WH_CVR_LINE;

  _offLoopVertexUse_s.clear ();

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _body->face_s ().begin ();
       i_face != _body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);

    for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	   i_vertexUse = face_i->offLoopVertexUse_s ().begin ();
	 i_vertexUse != face_i->offLoopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
      if (vertexUse_i->vertex () == this) {
	WH_CVR_LINE;
	_offLoopVertexUse_s.push_back (vertexUse_i);
      }
    }
  }
}

void WH_GM3D_Vertex
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  this->collectEdgeVertexUses ();
  this->collectLoopVertexUses ();
  this->collectOffLoopVertexUses ();

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_Vertex
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Body* WH_GM3D_Vertex
::body () const
{
  return _body;
}

const vector<WH_GM3D_EdgeVertexUse*>& WH_GM3D_Vertex
::edgeVertexUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _edgeVertexUse_s;
}
  
const vector<WH_GM3D_LoopVertexUse*>& WH_GM3D_Vertex
::loopVertexUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _loopVertexUse_s;
}
  
const vector<WH_GM3D_OffLoopVertexUse*>& WH_GM3D_Vertex
::offLoopVertexUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _offLoopVertexUse_s;
}
  
WH_Vector3D WH_GM3D_Vertex
::point () const
{
  return _point;
}

void WH_GM3D_Vertex
::getEdges 
(vector<WH_GM3D_Edge*>& edge_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  edge_s_OUT.clear ();

  for (vector<WH_GM3D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = this->edgeVertexUse_s ().begin ();
       i_vertexUse != this->edgeVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
    edge_s_OUT.push_back (vertexUse_i->edge ());
  }

  /* POST-CONDITION */
  /* <edge_s_OUT> has no specific order. */
#ifndef WH_PRE_ONLY
#endif
}

void WH_GM3D_Vertex
::getFaces 
(vector<WH_GM3D_Face*>& face_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  face_s_OUT.clear ();

  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->loopVertexUse_s ().begin ();
       i_vertexUse != this->loopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    face_s_OUT.push_back (vertexUse_i->loop ()->face ());
  }

  for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	 i_vertexUse = this->offLoopVertexUse_s ().begin ();
       i_vertexUse != this->offLoopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
    face_s_OUT.push_back (vertexUse_i->face ());
  }

  for (vector<WH_GM3D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = this->edgeVertexUse_s ().begin ();
       i_vertexUse != this->edgeVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);

    WH_GM3D_Edge* edge = vertexUse_i->edge ();
    for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	   i_edgeUse = edge->offLoopEdgeUse_s ().begin ();
	 i_edgeUse != edge->offLoopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
      WH_T_AddIfNotExists (face_s_OUT, edgeUse_i->face ());
    }
  }

  /* POST-CONDITION */
  /* <face_s_OUT> has no specific order. */
#ifndef WH_PRE_ONLY
#endif
}

bool WH_GM3D_Vertex
::isDot () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return (this->edgeVertexUse_s ().size () == 0
	  && this->loopVertexUse_s ().size () == 0
	  && this->offLoopVertexUse_s ().size () == 0);
}

bool WH_GM3D_Vertex
::isVolume () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  int outerBoundaryFaceCount = 0;
  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->loopVertexUse_s ().begin ();
       i_vertexUse != this->loopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_GM3D_Face* face = vertexUse_i->loop ()->face ();
    if (face->faceType () == WH_GM3D_Face::OUTER_BOUNDARY) {
      WH_CVR_LINE;
      outerBoundaryFaceCount++;
    }
  }
  WH_ASSERT (outerBoundaryFaceCount == 0
	     || 2 <= outerBoundaryFaceCount);
  
  return (0 < outerBoundaryFaceCount);
}



/* test coverage completed */

