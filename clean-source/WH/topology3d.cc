/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* topology3d.cc : 3-D geometry model topology framework */

#include "topology3d.h"



/* class WH_TPL3D_Vertex_A */

WH_TPL3D_Vertex_A
::WH_TPL3D_Vertex_A () 
{
  _body = WH_NULL;

  /* fill
     _body
     _connectedFace_s
     _connectedEdge_s 
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Vertex_A
::~WH_TPL3D_Vertex_A () 
{
}

bool WH_TPL3D_Vertex_A
::checkInvariant () const
{
  WH_ASSERT(this->body () != WH_NULL);

  return true;
}

bool WH_TPL3D_Vertex_A
::assureInvariant () const
{
  this->checkInvariant ();

  return true;
}



/* class WH_TPL3D_Curve_A */

WH_TPL3D_Curve_A
::WH_TPL3D_Curve_A () 
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Curve_A
::~WH_TPL3D_Curve_A () 
{
}

bool WH_TPL3D_Curve_A
::checkInvariant () const
{
  return true;
}

bool WH_TPL3D_Curve_A
::assureInvariant () const
{
  this->checkInvariant ();

  return true;
}



/* class WH_TPL3D_Edge_A */

WH_TPL3D_Edge_A
::WH_TPL3D_Edge_A () 
{
  _body = WH_NULL;
  _vertex0 = WH_NULL;
  _vertex1 = WH_NULL;
  _curve = WH_NULL;

  /* fill
     _body
     _vertex0 
     _vertex1 
     _connectedFace_s 
     _curve
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Edge_A
::~WH_TPL3D_Edge_A () 
{
  delete _curve;
}

bool WH_TPL3D_Edge_A
::checkInvariant () const
{
  WH_ASSERT(this->body () != WH_NULL);
  
  return true;
}

bool WH_TPL3D_Edge_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(this->curve () != WH_NULL);
  if (this->isPeriodic ()) {
    WH_ASSERT(this->curve ()->isPeriodic ());
    WH_ASSERT(this->vertex0 () == WH_NULL
	      && this->vertex1 () == WH_NULL);
  } else {
    WH_ASSERT(this->vertex0 () != WH_NULL
	      && this->vertex1 () != WH_NULL);
  }
  WH_ASSERT(WH_ne (this->parameter0 (), this->parameter1 ()));
  WH_ASSERT(WH_lt (0, this->length ()));

  if (this->isWire ()) {
    WH_ASSERT(this->connectedFace_s ().size () == 0);
  }
  if (this->isPeriodic ()) {
    double length;
    bool flag = this->curve ()->getLength 
      (length);
    WH_ASSERT(flag);
    WH_ASSERT(WH_eq (length, this->length ()));
  } else {
    WH_Vector3D point0 = this->vertex0 ()->point ();
    WH_Vector3D point1 = this->vertex1 ()->point ();
    WH_ASSERT(this->curve ()->contains (point0));
    WH_ASSERT(this->curve ()->contains (point1));
    
    double parameter0 = this->parameter0 ();
    double parameter1 = this->parameter1 ();
    WH_ASSERT(WH_eq (this->curve ()->positionAt (parameter0), 
		     point0));
    WH_ASSERT(WH_eq (this->curve ()->positionAt (parameter1), 
		     point1));
    WH_ASSERT(WH_eq 
	      (this->curve ()->lengthBetween (parameter0, parameter1),
	       this->length ()));
  }

  _curve->assureInvariant ();

  return true;
}

bool WH_TPL3D_Edge_A
::isWire () const
{
  return (_connectedFace_s.size () == 0);
}

bool WH_TPL3D_Edge_A
::isPeriodic () const
{
  return (_curve->isPeriodic () 
	  && _vertex0 == WH_NULL
	  && _vertex1 == WH_NULL);
}

double WH_TPL3D_Edge_A
::parameter0 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->vertex0 () != WH_NULL);

  return _curve->parameterAt (_vertex0->point ());
}

double WH_TPL3D_Edge_A
::parameter1 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->vertex1 () != WH_NULL);
  
  return _curve->parameterAt (_vertex1->point ());
}

double WH_TPL3D_Edge_A
::length () const
{
  if (this->isPeriodic ()) {
    double length;
    bool flag = _curve->getLength 
      (length);
    WH_ASSERT(flag);
    return length;
  } else {
    return _curve->lengthBetween 
      (this->parameter0 (), this->parameter1 ());
  }
}



/* class WH_TPL3D_LoopVertexUse_A */

WH_TPL3D_LoopVertexUse_A
::WH_TPL3D_LoopVertexUse_A () 
{
  _loop = WH_NULL;
  _vertex = WH_NULL;

  /* fill
     _loop
     _vertex
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_LoopVertexUse_A
::~WH_TPL3D_LoopVertexUse_A () 
{
}

bool WH_TPL3D_LoopVertexUse_A
::checkInvariant () const
{
  WH_ASSERT(this->loop () != WH_NULL);
  WH_ASSERT(this->vertex () != WH_NULL);

  return true;
}

bool WH_TPL3D_LoopVertexUse_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_Vector2D parameter = this->parameter ();
  WH_Vector3D point = this->vertex ()->point ();
  WH_TPL3D_Surface_A* surface 
    = this->loop ()->face ()->surface ();
  WH_ASSERT(WH_eq (surface->positionAt (parameter), point));

  return true;
}

WH_Vector2D WH_TPL3D_LoopVertexUse_A
::parameter () const
{
  return _loop->face ()->surface ()
    ->parameterAt (_vertex->point ());
}



/* class WH_TPL3D_Curve2D_A */

WH_TPL3D_Curve2D_A
::WH_TPL3D_Curve2D_A () 
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Curve2D_A
::~WH_TPL3D_Curve2D_A () 
{
}

bool WH_TPL3D_Curve2D_A
::checkInvariant () const
{
  return true;
}

bool WH_TPL3D_Curve2D_A
::assureInvariant () const
{
  this->checkInvariant ();

  return true;
}



/* class WH_TPL3D_LoopEdgeUse_A */

WH_TPL3D_LoopEdgeUse_A
::WH_TPL3D_LoopEdgeUse_A () 
{
  _loop = WH_NULL;
  _edge = WH_NULL;
  _curve = WH_NULL;

  /* fill
     _loop
     _edge
     _curve
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_LoopEdgeUse_A
::~WH_TPL3D_LoopEdgeUse_A () 
{
  delete _curve;
}

bool WH_TPL3D_LoopEdgeUse_A
::checkInvariant () const
{
  WH_ASSERT(this->loop () != WH_NULL);
  WH_ASSERT(this->edge () != WH_NULL);

  return true;
}

bool WH_TPL3D_LoopEdgeUse_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(this->curve () != WH_NULL);
  if (this->isPeriodic ()) {
    WH_ASSERT(this->curve ()->isPeriodic ());
    WH_ASSERT(this->edge ()->isPeriodic ());
    double length;
    bool flag = this->curve ()->getLength 
      (length);
    WH_ASSERT(flag);
    WH_ASSERT(WH_eq (length, this->lengthOnSurface ()));
  } else {
    WH_ASSERT(!this->curve ()->isPeriodic ());
    WH_ASSERT(!this->edge ()->isPeriodic ());
    WH_Vector3D point0 = this->edge ()->vertex0 ()->point ();
    WH_Vector3D point1 = this->edge ()->vertex1 ()->point ();
    WH_TPL3D_Surface_A* surface 
      = this->loop ()->face ()->surface ();
    WH_ASSERT(surface->contains (point0));
    WH_ASSERT(surface->contains (point1));
    
    double cParam0 = this->parameterOnCurve0 ();
    double cParam1 = this->parameterOnCurve1 ();
    WH_ASSERT(WH_eq 
	      (this->curve ()->lengthBetween (cParam0, cParam1),
	       this->lengthOnSurface ()));
    
    WH_Vector2D sParam0 = this->parameterOnSurface0 ();
    WH_Vector2D sParam1 = this->parameterOnSurface1 ();
    WH_ASSERT(this->curve ()->contains (sParam0));
    WH_ASSERT(this->curve ()->contains (sParam1));
    WH_ASSERT(WH_eq (this->curve ()->positionAt (cParam0), 
		     sParam0));
    WH_ASSERT(WH_eq (this->curve ()->positionAt (cParam1), 
		     sParam1));
    WH_ASSERT(WH_eq (surface->positionAt (sParam0), 
		     point0));
    WH_ASSERT(WH_eq (surface->positionAt (sParam1), 
		     point1));
  }

  _curve->assureInvariant ();

  return true;
}

bool WH_TPL3D_LoopEdgeUse_A
::isPeriodic () const
{
  return (_edge->isPeriodic ());
}

double WH_TPL3D_LoopEdgeUse_A
::parameterOnCurve0 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->edge ()->vertex0 () != WH_NULL);

  return _curve->parameterAt (this->parameterOnSurface0 ());
}

double WH_TPL3D_LoopEdgeUse_A
::parameterOnCurve1 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->edge ()->vertex1 () != WH_NULL);

  return _curve->parameterAt (this->parameterOnSurface1 ());
}

WH_Vector2D WH_TPL3D_LoopEdgeUse_A
::parameterOnSurface0 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->edge ()->vertex0 () != WH_NULL);

  return _loop->face ()->surface ()
    ->parameterAt (_edge->vertex0 ()->point ());
}

WH_Vector2D WH_TPL3D_LoopEdgeUse_A
::parameterOnSurface1 () const
{
  /* PRE-CONDITION */
  if (!this->isPeriodic ());
  if (this->edge ()->vertex1 () != WH_NULL);

  return _loop->face ()->surface ()
    ->parameterAt (_edge->vertex1 ()->point ());
}

double WH_TPL3D_LoopEdgeUse_A
::lengthOnSurface () const
{
  if (this->isPeriodic ()) {
    double length;
    bool flag = _curve->getLength 
      (length);
    WH_ASSERT(flag);
    return length;
  } else {
    return _curve->lengthBetween 
      (this->parameterOnCurve0 (), this->parameterOnCurve1 ());
  }
}



/* class WH_TPL3D_Loop_A */

WH_TPL3D_Loop_A
::WH_TPL3D_Loop_A () 
{
  _face = WH_NULL;

  /* fill
     _face
     _edgeUse_s
     _vertexUse_s 
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Loop_A
::~WH_TPL3D_Loop_A () 
{
  WH_T_Delete (_vertexUse_s);
  WH_T_Delete (_edgeUse_s);
}

bool WH_TPL3D_Loop_A
::checkInvariant () const
{
  WH_ASSERT(this->face () != WH_NULL);

  return true;
}

bool WH_TPL3D_Loop_A
::assureInvariant () const
{
  this->checkInvariant ();

  switch (this->loopType ()) {
  case NORMAL:
    WH_ASSERT(0 < this->vertexUse_s ().size ());
    WH_ASSERT(0 < this->edgeUse_s ().size ());
    WH_ASSERT(this->vertexUse_s ().size () 
	    == this->edgeUse_s ().size ());
    {
      const vector<WH_TPL3D_LoopVertexUse_A*>& vertexUse_s 
	= this->vertexUse_s ();
      const vector<WH_TPL3D_LoopEdgeUse_A*>& edgeUse_s 
	= this->edgeUse_s ();

      for (int i_vertexUse = 0; 
	   i_vertexUse < (int)vertexUse_s.size ();
	   i_vertexUse++) {
	WH_TPL3D_LoopVertexUse_A* vertexUse0 
	  = vertexUse_s[i_vertexUse];
	WH_TPL3D_LoopVertexUse_A* vertexUse1 = WH_NULL;
	if (i_vertexUse == (int)vertexUse_s.size () - 1) {
	  vertexUse1 = vertexUse_s[0];
	} else {
	  vertexUse1 = vertexUse_s[i_vertexUse + 1];
	}
	WH_TPL3D_Vertex_A* vertex0 = vertexUse0->vertex ();
	WH_TPL3D_Vertex_A* vertex1 = vertexUse1->vertex ();

	WH_TPL3D_LoopEdgeUse_A* edgeUse 
	  = edgeUse_s[i_vertexUse];
	WH_TPL3D_Edge_A* edge = edgeUse->edge ();

	WH_ASSERT((edge->vertex0 () == vertex0 
		   && edge->vertex1 () == vertex1) 
		  || (edge->vertex0 () == vertex1 
		      && edge->vertex1 () == vertex0));
      }
    }
    break;
  case SINGLE_VERTEX:
    WH_ASSERT(this->vertexUse_s ().size () == 1);
    WH_ASSERT(this->edgeUse_s ().size () == 0);
    break;
  case SINGLE_EDGE:
    WH_ASSERT(this->vertexUse_s ().size () == 0);
    WH_ASSERT(this->edgeUse_s ().size () == 1);
    {
      const vector<WH_TPL3D_LoopEdgeUse_A*>& edgeUse_s 
	= this->edgeUse_s ();
      WH_TPL3D_LoopEdgeUse_A* edgeUse = edgeUse_s[0];
      WH_ASSERT(edgeUse->isPeriodic ());
    }
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }

  WH_T_AssureInvariant (_vertexUse_s);
  WH_T_AssureInvariant (_edgeUse_s);

  return true;
}

WH_TPL3D_Loop_A::LoopType WH_TPL3D_Loop_A
::loopType () const
{
  if (_vertexUse_s.size () == 0) {
    WH_ASSERT(_edgeUse_s.size () == 1);
    return SINGLE_EDGE;
  } else if (_edgeUse_s.size () == 0) {
    WH_ASSERT(_vertexUse_s.size () == 1);
    return SINGLE_VERTEX;
  } else {
    WH_ASSERT(_vertexUse_s.size () == _edgeUse_s.size ());
    return NORMAL;
  }
}

void WH_TPL3D_Loop_A
::getVertexs 
(vector<WH_TPL3D_Vertex_A*>& vertex_s_OUT) const
{
  vertex_s_OUT.clear ();

  for (vector<WH_TPL3D_LoopVertexUse_A*>::const_iterator 
	 i_vertexUse = _vertexUse_s.begin ();
       i_vertexUse != _vertexUse_s.end ();
       i_vertexUse++) {
    WH_TPL3D_LoopVertexUse_A* vertexUse_i = (*i_vertexUse);

    WH_TPL3D_Vertex_A* vertex = vertexUse_i->vertex ();
    vector<WH_TPL3D_Vertex_A*>::iterator i_vertexFound 
      = find (vertex_s_OUT.begin (), vertex_s_OUT.end (), 
	      vertex);
    if (i_vertexFound == vertex_s_OUT.end ()) {
      vertex_s_OUT.push_back (vertex);
    }
  }
}

void WH_TPL3D_Loop_A
::getEdges 
(vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const
{
  edge_s_OUT.clear ();

  for (vector<WH_TPL3D_LoopEdgeUse_A*>::const_iterator 
	 i_edgeUse = _edgeUse_s.begin ();
       i_edgeUse != _edgeUse_s.end ();
       i_edgeUse++) {
    WH_TPL3D_LoopEdgeUse_A* edgeUse_i = (*i_edgeUse);

    WH_TPL3D_Edge_A* edge = edgeUse_i->edge ();
    vector<WH_TPL3D_Edge_A*>::iterator i_edgeFound
      = find (edge_s_OUT.begin (), edge_s_OUT.end (), 
	      edge);
    if (i_edgeFound == edge_s_OUT.end ()) {
      edge_s_OUT.push_back (edge);
    }
  }
}



/* class WH_TPL3D_Surface_A */

WH_TPL3D_Surface_A
::WH_TPL3D_Surface_A () 
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Surface_A
::~WH_TPL3D_Surface_A () 
{
}

bool WH_TPL3D_Surface_A
::checkInvariant () const
{
  return true;
}

bool WH_TPL3D_Surface_A
::assureInvariant () const
{
  this->checkInvariant ();

  return true;
}



/* class WH_TPL3D_Face_A */

WH_TPL3D_Face_A
::WH_TPL3D_Face_A () 
{
  _body = WH_NULL;
  _outerLoop = WH_NULL;
  _surface = WH_NULL;

  /* fill
     _body
     _outerLoop
     _innerLoop_s
     _otherLoop_s
     _surface
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Face_A
::~WH_TPL3D_Face_A () 
{
  delete _outerLoop;
  WH_T_Delete (_innerLoop_s);
  WH_T_Delete (_otherLoop_s);
  delete _surface;
}

bool WH_TPL3D_Face_A
::checkInvariant () const
{
  WH_ASSERT(this->body () != WH_NULL);

  return true;
}

bool WH_TPL3D_Face_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(this->surface () != WH_NULL);
  if (0 < this->otherLoop_s ().size ()) {
    WH_ASSERT(this->outerLoop () == WH_NULL);
    WH_ASSERT(this->innerLoop_s ().size () == 0);
  } else {
    WH_ASSERT(this->outerLoop () != WH_NULL
	      || this->outerLoop () == WH_NULL);
  }

  _surface->assureInvariant ();
  _outerLoop->assureInvariant ();
  WH_T_AssureInvariant (_innerLoop_s);
  WH_T_AssureInvariant (_otherLoop_s);

  return true;
}

void WH_TPL3D_Face_A
::getLoops 
(vector<WH_TPL3D_Loop_A*>& loop_s_OUT) const
{
  loop_s_OUT.erase 
    (loop_s_OUT.begin (), loop_s_OUT.end ());
  
  if (_outerLoop != WH_NULL) {
    loop_s_OUT.push_back (_outerLoop);
  }
  copy (_innerLoop_s.begin (), _innerLoop_s.end (),
	back_inserter (loop_s_OUT));
  copy (_otherLoop_s.begin (), _otherLoop_s.end (),
	back_inserter (loop_s_OUT));
}



/* class WH_TPL3D_Volume_A */

WH_TPL3D_Volume_A
::WH_TPL3D_Volume_A () 
{
  _body = WH_NULL;

  /* fill
     _body
     _face_s
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Volume_A
::~WH_TPL3D_Volume_A () 
{
}

bool WH_TPL3D_Volume_A
::checkInvariant () const
{
  WH_ASSERT(this->body () != WH_NULL);

  return true;
}

bool WH_TPL3D_Volume_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(0 < this->face_s ().size ());
  
  return true;
}

void WH_TPL3D_Volume_A
::getVertexs 
(vector<WH_TPL3D_Vertex_A*>& vertex_s_OUT) const
{
  vertex_s_OUT.clear ();

  for (vector<WH_TPL3D_Face_A*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_TPL3D_Face_A* face_i = (*i_face);

    vector<WH_TPL3D_Loop_A*> loop_s;
    face_i->getLoops 
      (loop_s);
    for (vector<WH_TPL3D_Loop_A*>::const_iterator 
	   i_loop = loop_s.begin ();
	 i_loop != loop_s.end ();
	 i_loop++) {
      WH_TPL3D_Loop_A* loop_i = (*i_loop);

      vector<WH_TPL3D_Vertex_A*> vertex_s;
      loop_i->getVertexs 
	(vertex_s);
      for (vector<WH_TPL3D_Vertex_A*>::const_iterator 
	     i_vertex = vertex_s.begin ();
	   i_vertex != vertex_s.end ();
	   i_vertex++) {
	WH_TPL3D_Vertex_A* vertex_i = (*i_vertex);

	vector<WH_TPL3D_Vertex_A*>::iterator i_vertexFound
	  = find (vertex_s_OUT.begin (), vertex_s_OUT.end (), 
		  vertex_i);
	if (i_vertexFound == vertex_s_OUT.end ()) {
	  vertex_s_OUT.push_back (vertex_i);
	}
      }
    }
  }
}

void WH_TPL3D_Volume_A
::getEdges 
(vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const
{
  edge_s_OUT.clear ();

  for (vector<WH_TPL3D_Face_A*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_TPL3D_Face_A* face_i = (*i_face);

    vector<WH_TPL3D_Loop_A*> loop_s;
    face_i->getLoops 
      (loop_s);
    for (vector<WH_TPL3D_Loop_A*>::const_iterator 
	   i_loop = loop_s.begin ();
	 i_loop != loop_s.end ();
	 i_loop++) {
      WH_TPL3D_Loop_A* loop_i = (*i_loop);

      vector<WH_TPL3D_Edge_A*> edge_s;
      loop_i->getEdges 
	(edge_s);
      for (vector<WH_TPL3D_Edge_A*>::const_iterator 
	     i_edge = edge_s.begin ();
	   i_edge != edge_s.end ();
	   i_edge++) {
	WH_TPL3D_Edge_A* edge_i = (*i_edge);

	vector<WH_TPL3D_Edge_A*>::iterator i_edgeFound
	  = find (edge_s_OUT.begin (), edge_s_OUT.end (), 
		  edge_i);
	if (i_edgeFound == edge_s_OUT.end ()) {
	  edge_s_OUT.push_back (edge_i);
	}
      }
    }
  }
}



/* class WH_TPL3D_Body_A */

WH_TPL3D_Body_A
::WH_TPL3D_Body_A () 
{
  /* fill
     _vertex_s
     _edge_s
     _face_s
     _volume_s
  */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_TPL3D_Body_A
::~WH_TPL3D_Body_A () 
{
  WH_T_Delete (_volume_s);
  WH_T_Delete (_face_s);
  WH_T_Delete (_edge_s);
  WH_T_Delete (_vertex_s);
}

bool WH_TPL3D_Body_A
::checkInvariant () const
{
  return true;
}

bool WH_TPL3D_Body_A
::assureInvariant () const
{
  this->checkInvariant ();

  WH_T_AssureInvariant (_vertex_s);
  WH_T_AssureInvariant (_edge_s);
  WH_T_AssureInvariant (_face_s);
  WH_T_AssureInvariant (_volume_s);

  return true;
}



#ifndef WH_INLINE_ENABLED
#include "topology3d_inline.cc"
#endif
