/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_brep_face.cc : boundary representation module of 3-D geometry
   modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_brep.h"



/* class WH_GM3D_LoopVertexUse */

WH_GM3D_LoopVertexUse
::WH_GM3D_LoopVertexUse 
(WH_GM3D_Loop* loop,
 WH_GM3D_Vertex* vertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);
  
  WH_CVR_LINE;

  _loop = loop;
  _vertex = vertex;

  _isConsistent = false;
}

WH_GM3D_LoopVertexUse
::~WH_GM3D_LoopVertexUse () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_LoopVertexUse
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->loop () != WH_NULL);
  WH_ASSERT(this->vertex () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_LoopVertexUse
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->loop ()->vertexUse_s (), this));

    WH_ASSERT(WH_T_Exists (this->vertex ()->loopVertexUse_s (), 
			   this));

    WH_Vector3D vertexPoint = this->vertex ()->point ();
    WH_Plane3D facePlane = this->loop ()->face ()->plane ();
    WH_ASSERT(facePlane.contains (vertexPoint));

    WH_Vector2D param = this->parameter ();
    WH_ASSERT(WH_eq (vertexPoint, facePlane.positionAt (param)));
  }
  
  return true;
}

void WH_GM3D_LoopVertexUse
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_LoopVertexUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Loop* WH_GM3D_LoopVertexUse
::loop () const
{
  return _loop;
}

WH_GM3D_Vertex* WH_GM3D_LoopVertexUse
::vertex () const
{
  return _vertex;
}

WH_Vector2D WH_GM3D_LoopVertexUse
::parameter () const
{
  WH_Vector3D vertexPoint = this->vertex ()->point ();
  WH_Plane3D facePlane = this->loop ()->face ()->plane ();
  return facePlane.parameterAt (vertexPoint);
}



/* class WH_GM3D_LoopEdgeUse */

WH_GM3D_LoopEdgeUse
::WH_GM3D_LoopEdgeUse 
(WH_GM3D_Loop* loop,
 WH_GM3D_Edge* edge) 
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(edge != WH_NULL);
  
  WH_CVR_LINE;

  _loop = loop;
  _edge = edge;

  _isConsistent = false;
}

WH_GM3D_LoopEdgeUse
::~WH_GM3D_LoopEdgeUse () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_LoopEdgeUse
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->loop () != WH_NULL);
  WH_ASSERT(this->edge () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_LoopEdgeUse
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->loop ()->edgeUse_s (), this));
    
    WH_ASSERT(WH_T_Exists (this->edge ()->loopEdgeUse_s (), 
			   this));

    WH_Segment3D edgeSeg = this->edge ()->segment ();
    WH_Plane3D facePlane = this->loop ()->face ()->plane ();
    WH_ASSERT(facePlane.contains (edgeSeg.p0 ()));
    WH_ASSERT(facePlane.contains (edgeSeg.p1 ()));

    WH_Segment2D paramSeg = this->parameterSegment ();
    WH_Vector3D p0 = facePlane.positionAt (paramSeg.p0 ());
    WH_Vector3D p1 = facePlane.positionAt (paramSeg.p1 ());
    WH_ASSERT(WH_eq (edgeSeg, WH_Segment3D (p0, p1)));

    vector<WH_GM3D_Face*> face_s;
    this->getConnectedFaces 
      (face_s);
    WH_ASSERT(!WH_T_HasDuplication (face_s));

    /* NEED TO REDEFINE */
    /* <face_s> may contain faces if <edge ()> is off-loop edge */
#if 0
    for (vector<WH_GM3D_Face*>::const_iterator 
	   i_face = face_s.begin ();
	 i_face != face_s.end ();
	 i_face++) {
      WH_GM3D_Face* face_i = (*i_face);

      int count = 0;

      WH_GM3D_Loop* outerLoop = face_i->outerLoop ();
      WH_ASSERT(outerLoop != this->loop ());
      for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	     i_edgeUse = outerLoop->edgeUse_s ().begin ();
	   i_edgeUse != outerLoop->edgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	if (edgeUse_i->edge () == this->edge ()) {
	  count++;
	}
      }

      for (vector<WH_GM3D_Loop*>::const_iterator 
	     i_loop = face_i->innerLoop_s ().begin ();
	   i_loop != face_i->innerLoop_s ().end ();
	   i_loop++) {
	WH_GM3D_Loop* loop_i = (*i_loop);

	WH_ASSERT(loop_i != this->loop ());
	for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	       i_edgeUse = loop_i->edgeUse_s ().begin ();
	     i_edgeUse != loop_i->edgeUse_s ().end ();
	     i_edgeUse++) {
	  WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	  if (edgeUse_i->edge () == this->edge ()) {
	    count++;
	  }
	}
      }

      WH_ASSERT(count == 1);
    }
#endif

    /* NEED TO REDEFINE */
    /* <getConnectedFaces ()> must return faces sorted around the
       edge. */

  }
  
  return true;
}

void WH_GM3D_LoopEdgeUse
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_LoopEdgeUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Loop* WH_GM3D_LoopEdgeUse
::loop () const
{
  return _loop;
}

WH_GM3D_Edge* WH_GM3D_LoopEdgeUse
::edge () const
{
  return _edge;
}

WH_Segment2D WH_GM3D_LoopEdgeUse
::parameterSegment () const
{
  WH_Segment3D edgeSeg = this->edge ()->segment ();
  WH_Plane3D facePlane = this->loop ()->face ()->plane ();
  WH_Vector2D param0 = facePlane.parameterAt (edgeSeg.p0 ());
  WH_Vector2D param1 = facePlane.parameterAt (edgeSeg.p1 ());
  return WH_Segment2D (param0, param1);
}

void WH_GM3D_LoopEdgeUse
::getConnectedFaces 
(vector<WH_GM3D_Face*>& face_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  /* NEED TO REDEFINE */
  /* not sorted yet */

  face_s_OUT.clear ();

  vector<WH_GM3D_Face*> faceOfEdge_s;
  this->edge ()->getFaces 
    (faceOfEdge_s);
  WH_ASSERT(0 < faceOfEdge_s.size ());

  /* NEED TO REDEFINE */
  /* <faceOfEdge_s> may contain faces if <edge ()> is off-loop edge */

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = faceOfEdge_s.begin ();
       i_face != faceOfEdge_s.end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    if (face_i != this->loop ()->face ()) {
      WH_CVR_LINE;
      face_s_OUT.push_back (face_i);
    } 
  }
}



/* class WH_GM3D_Loop */

WH_GM3D_Loop
::WH_GM3D_Loop 
(WH_GM3D_Face* face) 
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  
  WH_CVR_LINE;

  _face = face;

  _isConsistent = false;
}

WH_GM3D_Loop
::~WH_GM3D_Loop () 
{
  WH_CVR_LINE;
  
  WH_T_Delete (_vertexUse_s);
  WH_T_Delete (_edgeUse_s);
}

bool WH_GM3D_Loop
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->face () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(3 <= this->vertexUse_s ().size ());
    WH_ASSERT(this->edgeUse_s ().size ()
	      == this->vertexUse_s ().size ());
  }

  return true;
}

bool WH_GM3D_Loop
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;
  
    WH_T_AssureInvariant (_vertexUse_s);
    WH_T_AssureInvariant (_edgeUse_s);



    WH_ASSERT(this->face ()->outerLoop () == this
	      || WH_T_Exists (this->face ()->innerLoop_s (), 
			      this));
    
    WH_Vector3D minRange;
    WH_Vector3D maxRange;
    this->getRange 
      (minRange, maxRange);
    WH_ASSERT(WH_ne (minRange, maxRange));
    WH_ASSERT(WH_le (minRange, maxRange));

    WH_ASSERT(!WH_T_HasDuplication (this->vertexUse_s ()));
    for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	   i_vertexUse = this->vertexUse_s ().begin ();
	 i_vertexUse != this->vertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
      WH_ASSERT(vertexUse_i->loop () == this);
      WH_ASSERT(WH_between (vertexUse_i->vertex ()->point (),
			    minRange, maxRange));
    }
    
    WH_ASSERT(!WH_T_HasDuplication (this->edgeUse_s ()));
    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->edgeUse_s ().begin ();
	 i_edgeUse != this->edgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      WH_ASSERT(edgeUse_i->loop () == this);
    }

    WH_Polygon3D poly = this->polygon ();
    WH_ASSERT(poly.isRegular ());

    /* The plane normal of the face may not be the same as one of the
       3-D polygon of the face.  It is up to the first 3 vertexs of
       the polygon. */
    WH_ASSERT(WH_eq (poly.plane (), this->face ()->plane ())
	      || WH_isReverse (poly.plane (), this->face ()->plane ()));
    
    WH_Polygon2D paramPoly = this->parameterPolygon ();
    WH_ASSERT(paramPoly.isRegular ());
  }
  
  return true;
}

void WH_GM3D_Loop
::findAndAddEdgeUse 
(WH_GM3D_LoopVertexUse* vertexUse0, 
 WH_GM3D_LoopVertexUse* vertexUse1)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(vertexUse0 != WH_NULL);
  WH_ASSERT(vertexUse1 != WH_NULL);
  WH_ASSERT(vertexUse0 != vertexUse1);
  /* <_face->body ()->edge_s ()> must have been set. */
  /* <firstVertexUse ()> and <lastVertexUse ()> of the edges must have
     been set. */
  
  WH_GM3D_Vertex* vertex0 = vertexUse0->vertex ();
  WH_GM3D_Vertex* vertex1 = vertexUse1->vertex ();
  
  WH_GM3D_Edge* edgeFound = WH_NULL;
  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = _face->body ()->edge_s ().begin ();
       i_edge != _face->body ()->edge_s ().end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    
    WH_GM3D_Vertex* firstVertex = edge_i->firstVertexUse ()->vertex ();
    WH_GM3D_Vertex* lastVertex = edge_i->lastVertexUse ()->vertex ();
    if ((firstVertex == vertex0 && lastVertex == vertex1)
	|| (firstVertex == vertex1 && lastVertex == vertex0)) {
      edgeFound = edge_i;
      break;
    }
  }
  WH_ASSERT(edgeFound != WH_NULL);
  
  WH_GM3D_LoopEdgeUse* edgeUse 
    = this->createLoopEdgeUse (edgeFound);
  _edgeUse_s.push_back (edgeUse);
}

void WH_GM3D_Loop
::collectEdgeUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(3 <= this->vertexUse_s ().size ());
  WH_ASSERT(this->edgeUse_s ().size () == 0);
  
  WH_CVR_LINE;

  int nVertexUses = (int)_vertexUse_s.size ();

  for (int iVertexUse = 0; iVertexUse < nVertexUses - 1; iVertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse0 = _vertexUse_s[iVertexUse];
    WH_GM3D_LoopVertexUse* vertexUse1 = _vertexUse_s[iVertexUse + 1];
    this->findAndAddEdgeUse (vertexUse0, vertexUse1);
  }
  
  /* the last edge use */
  {
    WH_GM3D_LoopVertexUse* vertexUse0 = _vertexUse_s[nVertexUses - 1];
    WH_GM3D_LoopVertexUse* vertexUse1 = _vertexUse_s[0];
    this->findAndAddEdgeUse (vertexUse0, vertexUse1);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->edgeUse_s ().size () == this->vertexUse_s ().size ());
#endif
}

WH_GM3D_LoopVertexUse* WH_GM3D_Loop
::createLoopVertexUse (WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_GM3D_LoopVertexUse* result 
    = new WH_GM3D_LoopVertexUse (this, vertex);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_LoopEdgeUse* WH_GM3D_Loop
::createLoopEdgeUse (WH_GM3D_Edge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_GM3D_LoopEdgeUse* result 
    = new WH_GM3D_LoopEdgeUse (this, edge);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_GM3D_Loop
::addVertex 
(WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(this->face ()->plane ().contains (vertex->point ()));
#ifndef WH_PRE_ONLY
  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->vertexUse_s ().begin ();
       i_vertexUse != this->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_ASSERT(vertexUse_i->vertex () != vertex);
  }
#endif
  
  WH_GM3D_LoopVertexUse* vertexUse 
    = this->createLoopVertexUse (vertex);
  _vertexUse_s.push_back (vertexUse);
}

void WH_GM3D_Loop
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  WH_ASSERT(3 <= this->vertexUse_s ().size ());
  
  WH_CVR_LINE;

  if (!this->isConsistent ()) {
    WH_CVR_LINE;
    this->collectEdgeUses ();
  }

  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->vertexUse_s ().begin ();
       i_vertexUse != this->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    vertexUse_i->fix ();
  }

  for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->edgeUse_s ().begin ();
       i_edgeUse != this->edgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
    edgeUse_i->fix ();
  }

  _isConsistent = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_Loop
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Face* WH_GM3D_Loop
::face () const
{
  return _face;
}

const vector<WH_GM3D_LoopVertexUse*>& WH_GM3D_Loop
::vertexUse_s () const
{
  return _vertexUse_s;
}

const vector<WH_GM3D_LoopEdgeUse*>& WH_GM3D_Loop
::edgeUse_s () const
{
  return _edgeUse_s;
}

WH_Polygon3D WH_GM3D_Loop
::polygon () const
{
  /* PRE-CONDITION */
  WH_ASSERT(3 <= this->vertexUse_s ().size ());

  WH_CVR_LINE;

  vector<WH_Vector3D> vertex_s;
  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->vertexUse_s ().begin ();
       i_vertexUse != this->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    vertex_s.push_back (vertexUse_i->vertex ()->point ());
  }

  WH_Polygon3D result (vertex_s);

  return result;
}

WH_Polygon2D WH_GM3D_Loop
::parameterPolygon () const
{
  /* PRE-CONDITION */
  WH_ASSERT(3 <= this->vertexUse_s ().size ());

  WH_CVR_LINE;

  vector<WH_Vector2D> param_s;
  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->vertexUse_s ().begin ();
       i_vertexUse != this->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    param_s.push_back (vertexUse_i->parameter ());
  }

  WH_Polygon2D result (param_s);

  return result;
}

void WH_GM3D_Loop
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(3 <= this->vertexUse_s ().size ());
  
  WH_CVR_LINE;

  minRange_OUT = WH_Vector3D::hugeValue ();
  maxRange_OUT = -WH_Vector3D::hugeValue ();
  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->vertexUse_s ().begin ();
       i_vertexUse != this->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_Vector3D point = vertexUse_i->vertex ()->point ();
    minRange_OUT = WH_min (minRange_OUT, point);
    maxRange_OUT = WH_max (maxRange_OUT, point);
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}



/* class WH_GM3D_OffLoopVertexUse */

WH_GM3D_OffLoopVertexUse
::WH_GM3D_OffLoopVertexUse 
(WH_GM3D_Face* face,
 WH_GM3D_Vertex* vertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);
  
  WH_CVR_LINE;

  _face = face;
  _vertex = vertex;

  _isConsistent = false;
}

WH_GM3D_OffLoopVertexUse
::~WH_GM3D_OffLoopVertexUse () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_OffLoopVertexUse
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->face () != WH_NULL);
  WH_ASSERT(this->vertex () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_OffLoopVertexUse
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->face ()->offLoopVertexUse_s (), 
			   this));

    WH_ASSERT(WH_T_Exists (this->vertex ()->offLoopVertexUse_s (), 
			   this));

    WH_Vector3D vertexPoint = this->vertex ()->point ();
    WH_Plane3D facePlane = this->face ()->plane ();
    WH_ASSERT(facePlane.contains (vertexPoint));

    WH_Vector2D param = this->parameter ();
    WH_ASSERT(WH_eq (vertexPoint, facePlane.positionAt (param)));
  }
  
  return true;
}

void WH_GM3D_OffLoopVertexUse
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_OffLoopVertexUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Face* WH_GM3D_OffLoopVertexUse
::face () const
{
  return _face;
}

WH_GM3D_Vertex* WH_GM3D_OffLoopVertexUse
::vertex () const
{
  return _vertex;
}

WH_Vector2D WH_GM3D_OffLoopVertexUse
::parameter () const
{
  WH_Vector3D vertexPoint = this->vertex ()->point ();
  WH_Plane3D facePlane = this->face ()->plane ();
  return facePlane.parameterAt (vertexPoint);
}



/* class WH_GM3D_OffLoopEdgeUse */

WH_GM3D_OffLoopEdgeUse
::WH_GM3D_OffLoopEdgeUse 
(WH_GM3D_Face* face,
 WH_GM3D_Edge* edge) 
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  WH_ASSERT(edge != WH_NULL);
  
  WH_CVR_LINE;

  _face = face;
  _edge = edge;

  _isConsistent = false;
}

WH_GM3D_OffLoopEdgeUse
::~WH_GM3D_OffLoopEdgeUse () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_OffLoopEdgeUse
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->face () != WH_NULL);
  WH_ASSERT(this->edge () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_OffLoopEdgeUse
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->face ()->offLoopEdgeUse_s (), 
			   this));
    
    WH_ASSERT(WH_T_Exists (this->edge ()->offLoopEdgeUse_s (), 
			   this));

    WH_Segment3D edgeSeg = this->edge ()->segment ();
    WH_Plane3D facePlane = this->face ()->plane ();
    WH_ASSERT(facePlane.contains (edgeSeg.p0 ()));
    WH_ASSERT(facePlane.contains (edgeSeg.p1 ()));

    WH_Segment2D paramSeg = this->parameterSegment ();
    WH_Vector3D p0 = facePlane.positionAt (paramSeg.p0 ());
    WH_Vector3D p1 = facePlane.positionAt (paramSeg.p1 ());
    WH_ASSERT(WH_eq (edgeSeg, WH_Segment3D (p0, p1)));
  }
  
  return true;
}

void WH_GM3D_OffLoopEdgeUse
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_OffLoopEdgeUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Face* WH_GM3D_OffLoopEdgeUse
::face () const
{
  return _face;
}

WH_GM3D_Edge* WH_GM3D_OffLoopEdgeUse
::edge () const
{
  return _edge;
}

WH_Segment2D WH_GM3D_OffLoopEdgeUse
::parameterSegment () const
{
  WH_Segment3D edgeSeg = this->edge ()->segment ();
  WH_Plane3D facePlane = this->face ()->plane ();
  WH_Vector2D param0 = facePlane.parameterAt (edgeSeg.p0 ());
  WH_Vector2D param1 = facePlane.parameterAt (edgeSeg.p1 ());
  return WH_Segment2D (param0, param1);
}



/* class WH_GM3D_FaceSide */

WH_GM3D_FaceSide
::WH_GM3D_FaceSide 
(WH_GM3D_Face* face,
 bool isInsideVolume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  
  WH_CVR_LINE;

  _face = face;
  _isInsideVolume = isInsideVolume;

  _isConsistent = false;
}

WH_GM3D_FaceSide
::~WH_GM3D_FaceSide () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_FaceSide
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->face () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;
  }

  return true;
}

bool WH_GM3D_FaceSide
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->face ()->frontSide () == this
	      || this->face ()->backSide () == this);
  }
  
  return true;
}

void WH_GM3D_FaceSide
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_FaceSide
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Face* WH_GM3D_FaceSide
::face () const
{
  return _face;
}

bool WH_GM3D_FaceSide
::isInsideVolume () const
{
  return _isInsideVolume;
}



/* class WH_GM3D_Face */

WH_GM3D_Face
::WH_GM3D_Face 
(WH_GM3D_Body* body,
 const WH_Plane3D& plane) 
  : _plane (plane)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;

  _body = body;
  _outerLoop = WH_NULL;
  _frontSide = WH_NULL;
  _backSide = WH_NULL;

  _isConsistent = false;
}

WH_GM3D_Face
::~WH_GM3D_Face () 
{
  WH_CVR_LINE;
  
  delete _outerLoop;
  WH_T_Delete (_innerLoop_s);
  WH_T_Delete (_offLoopVertexUse_s);
  WH_T_Delete (_offLoopEdgeUse_s);
  delete _frontSide;
  delete _backSide;
}

bool WH_GM3D_Face
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->outerLoop () != WH_NULL);
    WH_ASSERT(this->frontSide () != WH_NULL);
    WH_ASSERT(this->backSide () != WH_NULL);
    WH_ASSERT(this->frontSide () != this->backSide ());
  }

  return true;
}

bool WH_GM3D_Face
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  if (this->isConsistent ()) {
    WH_CVR_LINE;

    _outerLoop->assureInvariant ();
    WH_T_AssureInvariant (_innerLoop_s);
    WH_T_AssureInvariant (_offLoopVertexUse_s);
    WH_T_AssureInvariant (_offLoopEdgeUse_s);

    _frontSide->assureInvariant ();
    _backSide->assureInvariant ();



    WH_ASSERT(WH_T_Exists (this->body ()->face_s (), this));
    
    WH_ASSERT(!WH_T_HasDuplication (this->innerLoop_s ()));
    WH_ASSERT(!WH_T_Exists (this->innerLoop_s (), 
			    this->outerLoop ()));

    WH_ASSERT(this->outerLoop ()->face () == this);

    WH_Vector3D outerMinRange;
    WH_Vector3D outerMaxRange;
    this->outerLoop ()->getRange 
      (outerMinRange, outerMaxRange);
    
    WH_Polygon3D outerPoly = this->outerLoop ()->polygon ();
    WH_ASSERT(outerPoly.isRegular ());
    WH_ASSERT(WH_eq (outerPoly.plane (), this->plane ()) 
	      || WH_isReverse (outerPoly.plane (), this->plane ()));

    WH_Polygon2D outerParamPoly 
      = this->outerLoop ()->parameterPolygon ();
    WH_ASSERT(outerParamPoly.isRegular ());
    WH_Vector2D outerParamMinRange = outerParamPoly.minRange ();
    WH_Vector2D outerParamMaxRange = outerParamPoly.maxRange ();

    /* inner loops */
    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = this->innerLoop_s ().begin ();
	 i_loop != this->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);

      WH_ASSERT(loop_i->face () == this);
      
      WH_Vector3D minRange;
      WH_Vector3D maxRange;
      loop_i->getRange 
	(minRange, maxRange);
      WH_ASSERT(WH_between (minRange, outerMinRange, outerMaxRange));
      WH_ASSERT(WH_between (maxRange, outerMinRange, outerMaxRange));
      
      WH_Polygon3D poly = loop_i->polygon ();
      WH_ASSERT(poly.isRegular ());
      WH_ASSERT(WH_eq (poly.plane (), this->plane ()) 
		|| WH_isReverse (poly.plane (), this->plane ()));
      WH_ASSERT(WH_between (poly.minRange (), 
			    outerMinRange, outerMaxRange));
      WH_ASSERT(WH_between (poly.maxRange (), 
			    outerMinRange, outerMaxRange));

      WH_Polygon2D paramPoly = loop_i->parameterPolygon ();
      WH_ASSERT(paramPoly.isRegular ());
      WH_ASSERT(WH_justBetween (paramPoly.minRange (), 
				outerParamMinRange, outerParamMaxRange));
      WH_ASSERT(WH_justBetween (paramPoly.maxRange (), 
				outerParamMinRange, outerParamMaxRange));

      for (int iVertex = 0; iVertex < paramPoly.nVertexs (); iVertex++) {
	WH_Vector2D point = paramPoly.vertex (iVertex);
	WH_ASSERT(outerParamPoly.checkContainmentAt (point)
		  == WH_Polygon2D::IN);
      }
    }
      
    /* off-loop vertex uses */
    WH_ASSERT(!WH_T_HasDuplication (this->offLoopVertexUse_s ()));
    for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	   i_vertexUse = this->offLoopVertexUse_s ().begin ();
	 i_vertexUse != this->offLoopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
      
      WH_ASSERT(vertexUse_i->face () == this);
      WH_Vector2D param = vertexUse_i->parameter ();
      WH_ASSERT(outerParamPoly.checkContainmentAt (param)
		== WH_Polygon2D::IN);

      for (vector<WH_GM3D_Loop*>::const_iterator 
	     i_loop = this->innerLoop_s ().begin ();
	   i_loop != this->innerLoop_s ().end ();
	   i_loop++) {
	WH_GM3D_Loop* loop_i = (*i_loop);
	WH_Polygon2D paramPoly = loop_i->parameterPolygon ();
	WH_ASSERT(paramPoly.checkContainmentAt (param)
		  == WH_Polygon2D::OUT);
      }
    }

    /* off-loop edge uses */
    WH_ASSERT(!WH_T_HasDuplication (this->offLoopEdgeUse_s ()));
    for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->offLoopEdgeUse_s ().begin ();
	 i_edgeUse != this->offLoopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
      
      WH_ASSERT(edgeUse_i->face () == this);
      WH_Segment2D paramSeg = edgeUse_i->parameterSegment ();
      WH_Polygon2D::ContainmentType flag 
	= outerParamPoly.checkContainmentAt (paramSeg.p0 ());
      WH_ASSERT(flag == WH_Polygon2D::IN
		|| flag == WH_Polygon2D::ON);
      flag = outerParamPoly.checkContainmentAt (paramSeg.p1 ());
      WH_ASSERT(flag == WH_Polygon2D::IN
		|| flag == WH_Polygon2D::ON);
      flag = outerParamPoly.checkContainmentAt (paramSeg.midPoint ());
      WH_ASSERT(flag == WH_Polygon2D::IN);
      
      for (vector<WH_GM3D_Loop*>::const_iterator 
	     i_loop = this->innerLoop_s ().begin ();
	   i_loop != this->innerLoop_s ().end ();
	   i_loop++) {
	WH_GM3D_Loop* loop_i = (*i_loop);

	WH_Polygon2D paramPoly = loop_i->parameterPolygon ();
	flag = paramPoly.checkContainmentAt (paramSeg.p0 ());
	WH_ASSERT(flag == WH_Polygon2D::OUT
		  || flag == WH_Polygon2D::ON);
	flag = paramPoly.checkContainmentAt (paramSeg.p1 ());
	WH_ASSERT(flag == WH_Polygon2D::OUT
		  || flag == WH_Polygon2D::ON);
	flag = paramPoly.checkContainmentAt (paramSeg.midPoint ());
	WH_ASSERT(flag == WH_Polygon2D::OUT);
      }
    }

    WH_ASSERT(this->frontSide ()->face () == this);
    WH_ASSERT(this->backSide ()->face () == this);

    vector<WH_GM3D_Vertex*> vertex_s;
    this->getVertexs 
      (vertex_s);
    WH_ASSERT(!WH_T_HasDuplication (vertex_s));
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = vertex_s.begin ();
	 i_vertex != vertex_s.end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      vector<WH_GM3D_Face*> face_s;
      vertex_i->getFaces 
	(face_s);
      WH_ASSERT(WH_T_Exists (face_s, this));
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
      vector<WH_GM3D_Face*> face_s;
      edge_i->getFaces 
	(face_s);
      WH_ASSERT(WH_T_Exists (face_s, this));
    }

    WH_Vector3D normal;
    bool normalFlag = this->getNormalToOutsideVolume 
      (normal);

    switch (this->faceType ()) {
    case SHEET:
      WH_ASSERT(!normalFlag);
      break;
    case INNER_BOUNDARY:
      WH_ASSERT(!normalFlag);
      break;
    case OUTER_BOUNDARY:
      WH_ASSERT(normalFlag);
      WH_ASSERT(WH_eq (normal, this->plane ().normal ())
		|| WH_isReverse (normal, this->plane ().normal ()));
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    };

    if (this->isRegular ()) {
      WH_ASSERT(this->offLoopVertexUse_s ().size () == 0);
      WH_ASSERT(this->offLoopEdgeUse_s ().size () == 0);
    } else {
      WH_ASSERT(0 < this->offLoopVertexUse_s ().size ()
		|| 0 < this->offLoopEdgeUse_s ().size ());
    }

    /* NEED TO REDEFINE */
    /* <connectedFaceSide ()> */
  }
  
  return true;
}

WH_GM3D_OffLoopVertexUse* WH_GM3D_Face
::createOffLoopVertexUse (WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_GM3D_OffLoopVertexUse* result 
    = new WH_GM3D_OffLoopVertexUse (this, vertex);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_OffLoopEdgeUse* WH_GM3D_Face
::createOffLoopEdgeUse (WH_GM3D_Edge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_GM3D_OffLoopEdgeUse* result 
    = new WH_GM3D_OffLoopEdgeUse (this, edge);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_FaceSide* WH_GM3D_Face
::createFaceSide (bool isInsideVolume)
{
  WH_GM3D_FaceSide* result 
    = new WH_GM3D_FaceSide (this, isInsideVolume);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_GM3D_Face
::setSides
(bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(this->frontSide () == WH_NULL);
  WH_ASSERT(this->backSide () == WH_NULL);
  
  _frontSide = this->createFaceSide (frontSideIsInsideVolume);
  _backSide = this->createFaceSide (backSideIsInsideVolume);
}

void WH_GM3D_Face
::setOuterLoop (WH_GM3D_Loop* loop)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(this->outerLoop () == WH_NULL);
  WH_ASSERT(loop != WH_NULL);

  _outerLoop = loop;
}

void WH_GM3D_Face
::addInnerLoop (WH_GM3D_Loop* loop)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(loop != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->innerLoop_s (), loop));
#endif

  _innerLoop_s.push_back (loop);
}

void WH_GM3D_Face
::addOffLoopVertex (WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(this->plane ().contains (vertex->point ()));
#ifndef WH_PRE_ONLY
  for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	 i_vertexUse = this->offLoopVertexUse_s ().begin ();
       i_vertexUse != this->offLoopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
    WH_ASSERT(vertexUse_i->vertex () != vertex);
  }
#endif

  WH_GM3D_OffLoopVertexUse* vertexUse 
    = this->createOffLoopVertexUse (vertex);
  _offLoopVertexUse_s.push_back (vertexUse);
}

void WH_GM3D_Face
::addOffLoopEdge (WH_GM3D_Edge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(this->plane ().contains (edge->segment ().p0 ()));
  WH_ASSERT(this->plane ().contains (edge->segment ().p1 ()));
#ifndef WH_PRE_ONLY
  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->offLoopEdgeUse_s ().begin ();
       i_edgeUse != this->offLoopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
    WH_ASSERT(edgeUse_i->edge () != edge);
  }
#endif

  WH_GM3D_OffLoopEdgeUse* edgeUse 
    = this->createOffLoopEdgeUse (edge);
  _offLoopEdgeUse_s.push_back (edgeUse);
}

void WH_GM3D_Face
::updatePlane ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());

  _plane = this->outerLoop ()->polygon ().plane ();

  _isConsistent = false;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM3D_Face
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  WH_ASSERT(this->outerLoop () != WH_NULL);
  WH_ASSERT(this->frontSide () != WH_NULL);
  WH_ASSERT(this->backSide () != WH_NULL);
  
  WH_CVR_LINE;

  _outerLoop->fix ();
  
  for (vector<WH_GM3D_Loop*>::const_iterator 
	 i_loop = _innerLoop_s.begin ();
       i_loop != _innerLoop_s.end ();
       i_loop++) {
    WH_GM3D_Loop* loop_i = (*i_loop);
    loop_i->fix ();
  }

  for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	 i_vertexUse = _offLoopVertexUse_s.begin ();
       i_vertexUse != _offLoopVertexUse_s.end ();
       i_vertexUse++) {
    WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
    vertexUse_i->fix ();
  }

  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = _offLoopEdgeUse_s.begin ();
       i_edgeUse != _offLoopEdgeUse_s.end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
    edgeUse_i->fix ();
  }

  _frontSide->fix ();
  _backSide->fix ();

  _isConsistent = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_Face
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Body* WH_GM3D_Face
::body () const
{
  return _body;
}

WH_GM3D_Loop* WH_GM3D_Face
::outerLoop () const
{
  return _outerLoop;
}
  
const vector<WH_GM3D_Loop*>& WH_GM3D_Face
::innerLoop_s () const
{
  return _innerLoop_s;
}

const vector<WH_GM3D_OffLoopVertexUse*>& WH_GM3D_Face
::offLoopVertexUse_s () const
{
  return _offLoopVertexUse_s;
}

const vector<WH_GM3D_OffLoopEdgeUse*>& WH_GM3D_Face
::offLoopEdgeUse_s () const
{
  return _offLoopEdgeUse_s;
}

WH_GM3D_FaceSide* WH_GM3D_Face
::frontSide () const
{
  return _frontSide;
}

WH_GM3D_FaceSide* WH_GM3D_Face
::backSide () const
{
  return _backSide;
}

WH_Plane3D WH_GM3D_Face
::plane () const
{
  return _plane;
}

bool WH_GM3D_Face
::isRegular () const
{
  return this->offLoopVertexUse_s ().size () == 0
    && this->offLoopEdgeUse_s ().size () == 0;
}

void WH_GM3D_Face
::getVertexs 
(vector<WH_GM3D_Vertex*>& vertex_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->outerLoop () != WH_NULL);

  WH_CVR_LINE;

  vertex_s_OUT.clear ();

  for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	 i_vertexUse = this->outerLoop ()->vertexUse_s ().begin ();
       i_vertexUse != this->outerLoop ()->vertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
    vertex_s_OUT.push_back (vertexUse_i->vertex ());
  }
  
  for (vector<WH_GM3D_Loop*>::const_iterator 
	 i_loop = this->innerLoop_s ().begin ();
       i_loop != this->innerLoop_s ().end ();
       i_loop++) {
    WH_GM3D_Loop* loop_i = (*i_loop);
    
    for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	   i_vertexUse = loop_i->vertexUse_s ().begin ();
	 i_vertexUse != loop_i->vertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
      vertex_s_OUT.push_back (vertexUse_i->vertex ());
    }
  }

  for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	 i_vertexUse = this->offLoopVertexUse_s ().begin ();
       i_vertexUse != this->offLoopVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
    vertex_s_OUT.push_back (vertexUse_i->vertex ());
  }

  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->offLoopEdgeUse_s ().begin ();
       i_edgeUse != this->offLoopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
    WH_GM3D_Edge* edge = edgeUse_i->edge ();
    WH_T_AddIfNotExists (vertex_s_OUT, 
			 edge->firstVertexUse ()->vertex ());
    WH_T_AddIfNotExists (vertex_s_OUT, 
			 edge->lastVertexUse ()->vertex ());
  }
}

void WH_GM3D_Face
::getEdges 
(vector<WH_GM3D_Edge*>& edge_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->outerLoop () != WH_NULL);

  WH_CVR_LINE;

  edge_s_OUT.clear ();

  for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->outerLoop ()->edgeUse_s ().begin ();
       i_edgeUse != this->outerLoop ()->edgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
    edge_s_OUT.push_back (edgeUse_i->edge ());
  }
  
  for (vector<WH_GM3D_Loop*>::const_iterator 
	 i_loop = this->innerLoop_s ().begin ();
       i_loop != this->innerLoop_s ().end ();
       i_loop++) {
    WH_GM3D_Loop* loop_i = (*i_loop);
    
    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = loop_i->edgeUse_s ().begin ();
	 i_edgeUse != loop_i->edgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      edge_s_OUT.push_back (edgeUse_i->edge ());
    }
  }

  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->offLoopEdgeUse_s ().begin ();
       i_edgeUse != this->offLoopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
    edge_s_OUT.push_back (edgeUse_i->edge ());
  }
}

WH_GM3D_Face::FaceType WH_GM3D_Face
::faceType () const
{
  WH_CVR_LINE;

  if (this->frontSide ()->isInsideVolume ()) {
    if (this->backSide ()->isInsideVolume ()) {
      return INNER_BOUNDARY;
    } else {
      return OUTER_BOUNDARY;
    }
  } else {
    if (this->backSide ()->isInsideVolume ()) {
      return OUTER_BOUNDARY;
    } else {
      return SHEET;
    }
  }
}

bool WH_GM3D_Face
::getNormalToOutsideVolume 
(WH_Vector3D& normal_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  /* Normal of <plane ()> is oriented toward front side. */

  if (this->frontSide ()->isInsideVolume ()) {
    if (this->backSide ()->isInsideVolume ()) {
      return false;
    } else {
      normal_OUT = -this->plane ().normal ();
      return true;
    }
  } else {
    if (this->backSide ()->isInsideVolume ()) {
      normal_OUT = this->plane ().normal ();
      return true;
    } else {
      return false;
    }
  }
}
  
void WH_GM3D_Face
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->outerLoop () != WH_NULL);
  
  this->outerLoop ()->getRange 
    (minRange_OUT, maxRange_OUT);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}

WH_GM3D_FaceSide* WH_GM3D_Face
::connectedFaceSide 
(WH_GM3D_FaceSide* side,
 WH_GM3D_LoopEdgeUse* edgeUse) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(side != WH_NULL);
  WH_ASSERT(side == this->frontSide () 
	    || side == this->backSide ());
  WH_ASSERT(edgeUse != WH_NULL);
#ifndef NDEBUG
  {
    bool found = false;

    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->outerLoop ()->edgeUse_s ().begin ();
	 i_edgeUse != this->outerLoop ()->edgeUse_s ().end ();
       i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      if (edgeUse_i == edgeUse) {
	found = true;
      }
    }
    
    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = this->innerLoop_s ().begin ();
	 i_loop != this->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);
      
      for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	     i_edgeUse = loop_i->edgeUse_s ().begin ();
	   i_edgeUse != loop_i->edgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	if (edgeUse_i == edgeUse) {
	  found = true;
	}
      }
    }

    WH_ASSERT(found);
  }
#endif
  
  WH_CVR_LINE;                          /* NOT COVERED */

  WH_GM3D_FaceSide* result = WH_NULL;

  vector<WH_GM3D_Face*> connectedFace_s;
  edgeUse->getConnectedFaces 
    (connectedFace_s);

  /* NEED TO REDEFINE */
  WH_ASSERT_NO_REACH;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}



/* not yet covered 
WH_GM3D_FaceSide* WH_GM3D_Face
::connectedFaceSide 
*/

