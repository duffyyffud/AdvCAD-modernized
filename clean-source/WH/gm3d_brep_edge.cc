/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_brep_edge.cc : boundary representation module of 3-D geometry
   modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_brep.h"



/* class WH_GM3D_EdgeVertexUse */

WH_GM3D_EdgeVertexUse
::WH_GM3D_EdgeVertexUse 
(WH_GM3D_Edge* edge,
 WH_GM3D_Vertex* vertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);
  
  WH_CVR_LINE;

  _edge = edge;
  _vertex = vertex;

  _isConsistent = false;
}

WH_GM3D_EdgeVertexUse
::~WH_GM3D_EdgeVertexUse () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_EdgeVertexUse
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->edge () != WH_NULL);
  WH_ASSERT(this->vertex () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM3D_EdgeVertexUse
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->edge ()->firstVertexUse () == this
	      || this->edge ()->lastVertexUse () == this);

    WH_ASSERT(WH_T_Exists (this->vertex ()->edgeVertexUse_s (), 
			   this));
    
    WH_Vector3D vertexPoint = this->vertex ()->point ();
    WH_Line3D edgeLine = this->edge ()->segment ().line ();
    WH_ASSERT(edgeLine.contains (vertexPoint)); 

    double param = this->parameter ();
    WH_ASSERT(WH_eq (vertexPoint, edgeLine.positionAt (param)));
    
    vector<WH_GM3D_Edge*> edge_s;
    this->getConnectedEdges 
      (edge_s);
    WH_ASSERT(!WH_T_HasDuplication (edge_s));
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = edge_s.begin ();
	 i_edge != edge_s.end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      WH_ASSERT(edge_i != this->edge ());
      WH_ASSERT(edge_i->firstVertexUse ()->vertex () == this->vertex ()
		|| edge_i->lastVertexUse ()->vertex () == this->vertex ());
    }
  }
  
  return true;
}

void WH_GM3D_EdgeVertexUse
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_isConsistent || !_isConsistent);
  
  WH_CVR_LINE;

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_EdgeVertexUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Edge* WH_GM3D_EdgeVertexUse
::edge () const
{
  return _edge;
}

WH_GM3D_Vertex* WH_GM3D_EdgeVertexUse
::vertex () const
{
  return _vertex;
}

double WH_GM3D_EdgeVertexUse
::parameter () const
{
  WH_Vector3D vertexPoint = this->vertex ()->point ();
  WH_Line3D edgeLine = this->edge ()->segment ().line ();
  return edgeLine.parameterAt (vertexPoint);
}

void WH_GM3D_EdgeVertexUse
::getConnectedEdges 
(vector<WH_GM3D_Edge*>& edge_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  edge_s_OUT.clear ();

  /* <edgeOfVertex_s> has no specific order. */
  vector<WH_GM3D_Edge*> edgeOfVertex_s;
  this->vertex ()->getEdges 
    (edgeOfVertex_s);
  WH_ASSERT(0 < edgeOfVertex_s.size ());

  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = edgeOfVertex_s.begin ();
       i_edge != edgeOfVertex_s.end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    if (edge_i != this->edge ()) {
      WH_CVR_LINE;
      edge_s_OUT.push_back (edge_i);
    } 
  }

  /* POST-CONDITION */
  /* <edge_s_OUT> has no specific order. */
#ifndef WH_PRE_ONLY
#endif
}



/* class WH_GM3D_Edge */

WH_GM3D_Edge
::WH_GM3D_Edge 
(WH_GM3D_Body* body) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;

  _body = body;
  _firstVertexUse = WH_NULL;
  _lastVertexUse = WH_NULL;

  _isConsistent = false;
}

WH_GM3D_Edge
::~WH_GM3D_Edge () 
{
  WH_CVR_LINE;
  
  delete _firstVertexUse;
  delete _lastVertexUse;
}

bool WH_GM3D_Edge
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->firstVertexUse () != WH_NULL);
    WH_ASSERT(this->lastVertexUse () != WH_NULL);
    WH_ASSERT(this->firstVertexUse () != this->lastVertexUse ());
  }

  return true;
}

bool WH_GM3D_Edge
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  if (this->isConsistent ()) {
    WH_CVR_LINE;

    _firstVertexUse->assureInvariant ();
    _lastVertexUse->assureInvariant ();



    WH_ASSERT(WH_T_Exists (this->body ()->edge_s (), this));

    WH_Vector3D minRange;
    WH_Vector3D maxRange;
    this->getRange 
      (minRange, maxRange);
    WH_ASSERT(WH_ne (minRange, maxRange));
    WH_ASSERT(WH_le (minRange, maxRange));

    WH_ASSERT(this->firstVertexUse ()->edge () == this);
    WH_ASSERT(this->lastVertexUse ()->edge () == this);
    WH_ASSERT(WH_ne (this->firstVertexUse ()->parameter (),
		     this->lastVertexUse ()->parameter ()));

    WH_GM3D_Vertex* firstVertex = this->firstVertexUse ()->vertex ();
    WH_GM3D_Vertex* lastVertex = this->lastVertexUse ()->vertex ();
    WH_ASSERT(firstVertex != lastVertex);
    WH_ASSERT(WH_ne (firstVertex->point (), lastVertex->point ()));
    WH_ASSERT(WH_between (firstVertex->point (), minRange, maxRange));
    WH_ASSERT(WH_between (lastVertex->point (), minRange, maxRange));

    WH_ASSERT(WH_eq (this->segment (),
		     WH_Segment3D (firstVertex->point (),
				   lastVertex->point ())));
    
    WH_ASSERT(!WH_T_HasDuplication (this->loopEdgeUse_s ()));
    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->loopEdgeUse_s ().begin ();
	 i_edgeUse != this->loopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      WH_ASSERT(edgeUse_i->edge () == this);
    }

    WH_ASSERT(!WH_T_HasDuplication (this->offLoopEdgeUse_s ()));
    WH_ASSERT(this->offLoopEdgeUse_s ().size () == 0
	      || this->offLoopEdgeUse_s ().size () == 1);
    /* NEED TO REDEFINE */
    /* An edge can belong to only one face as an off-loop edge. */
    for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->offLoopEdgeUse_s ().begin ();
	 i_edgeUse != this->offLoopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
      WH_ASSERT(edgeUse_i->edge () == this);
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
      for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	     i_edgeUse = outerLoop->edgeUse_s ().begin ();
	   i_edgeUse != outerLoop->edgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	if (edgeUse_i->edge () == this) {
	  count++;
	}
      }
      
      for (vector<WH_GM3D_Loop*>::const_iterator 
	     i_loop = face_i->innerLoop_s ().begin ();
	   i_loop != face_i->innerLoop_s ().end ();
	   i_loop++) {
	WH_GM3D_Loop* loop_i = (*i_loop);

	for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	       i_edgeUse = loop_i->edgeUse_s ().begin ();
	     i_edgeUse != loop_i->edgeUse_s ().end ();
	     i_edgeUse++) {
	  WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	  if (edgeUse_i->edge () == this) {
	    count++;
	  }
	}
      }
      
      for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	     i_edgeUse = face_i->offLoopEdgeUse_s ().begin ();
	   i_edgeUse != face_i->offLoopEdgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
	if (edgeUse_i->edge () == this) {
	  count++;
	}
      }

      WH_ASSERT(count == 1);
    }
    /* NEED TO REDEFINE */
    /* <getFaces ()> must return faces sorted around the edge. */

    int sheetFaceCount = 0;
    int outerBoundaryFaceCount = 0;
    int innerBoundaryFaceCount = 0;
    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = this->loopEdgeUse_s ().begin ();
	 i_edgeUse != this->loopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      
      WH_GM3D_Face* face = edgeUse_i->loop ()->face ();
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
    WH_ASSERT(outerBoundaryFaceCount % 2 == 0);
    
    WH_ASSERT(!(this->isWire () && this->isVolume ()));

    if (this->isWire ()) {
      WH_CVR_LINE;
      WH_ASSERT(this->loopEdgeUse_s ().size () == 0);
      WH_ASSERT(this->offLoopEdgeUse_s ().size () == 0);
    }

    if (this->isVolume ()) {
      WH_CVR_LINE;
      WH_ASSERT(2 <= outerBoundaryFaceCount);
    }
  }
  
  return true;
}

void WH_GM3D_Edge
::collectLoopEdgeUses ()
{
  /* PRE-CONDITION */
  /* <_body->face_s ()> must have been set. */
  /* <outerLoop ()> and <innerLoop_s ()> of the faces must have been
     set. */
  /* <edgeUse_s ()> of the loops must have been set. */

  WH_CVR_LINE;

  _loopEdgeUse_s.clear ();

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _body->face_s ().begin ();
       i_face != _body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);

    WH_GM3D_Loop* outerLoop = face_i->outerLoop ();
    WH_ASSERT(outerLoop != WH_NULL);
    WH_ASSERT(3 <= outerLoop->edgeUse_s ().size ());
    
    for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	   i_edgeUse = outerLoop->edgeUse_s ().begin ();
	 i_edgeUse != outerLoop->edgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
      if (edgeUse_i->edge () == this) {
	WH_CVR_LINE;
	_loopEdgeUse_s.push_back (edgeUse_i);
      }
    }

    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = face_i->innerLoop_s ().begin ();
	 i_loop != face_i->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);
      WH_ASSERT(3 <= loop_i->edgeUse_s ().size ());

      for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	     i_edgeUse = loop_i->edgeUse_s ().begin ();
	   i_edgeUse != loop_i->edgeUse_s ().end ();
	   i_edgeUse++) {
	WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
	if (edgeUse_i->edge () == this) {
	  WH_CVR_LINE;
	  _loopEdgeUse_s.push_back (edgeUse_i);
	}
      }
    }
  }

  /* NEED TO REDEFINE */
  /* not sorted yet */
}

void WH_GM3D_Edge
::collectOffLoopEdgeUses ()
{
  /* PRE-CONDITION */
  /* <_body->face_s ()> must have been set. */
  /* <offLoopEdgeUse_s ()> of the faces must have been set. */

  WH_CVR_LINE;

  _offLoopEdgeUse_s.clear ();

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _body->face_s ().begin ();
       i_face != _body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);

    for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	   i_edgeUse = face_i->offLoopEdgeUse_s ().begin ();
	 i_edgeUse != face_i->offLoopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
      if (edgeUse_i->edge () == this) {
	WH_CVR_LINE;
	_offLoopEdgeUse_s.push_back (edgeUse_i);
      }
    }
  }

  /* NEED TO REDEFINE */
  /* not sorted yet */
}

WH_GM3D_EdgeVertexUse* WH_GM3D_Edge
::createEdgeVertexUse (WH_GM3D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_GM3D_EdgeVertexUse* result 
    = new WH_GM3D_EdgeVertexUse (this, vertex);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_GM3D_Edge
::setVertexs 
(WH_GM3D_Vertex* firstVertex, 
 WH_GM3D_Vertex* lastVertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(this->firstVertexUse () == WH_NULL);
  WH_ASSERT(this->lastVertexUse () == WH_NULL);
  WH_ASSERT(firstVertex != WH_NULL);
  WH_ASSERT(lastVertex != WH_NULL);
  if (firstVertex == lastVertex) {
    cerr << "WARNING: BREP edge created with identical vertices - this may indicate degenerate geometry" << endl;
    cerr << "  Continuing processing but edge may be problematic..." << endl;
  }
  if (firstVertex != lastVertex && WH_eq (firstVertex->point (), lastVertex->point ())) {
    cerr << "WARNING: BREP edge vertices have identical coordinates" << endl;
  }
  
  _firstVertexUse = this->createEdgeVertexUse (firstVertex);
  _lastVertexUse = this->createEdgeVertexUse (lastVertex);
}

void WH_GM3D_Edge
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  WH_ASSERT(this->firstVertexUse () != WH_NULL);
  WH_ASSERT(this->lastVertexUse () != WH_NULL);
  
  WH_CVR_LINE;

  _firstVertexUse->fix ();
  _lastVertexUse->fix ();

  this->collectLoopEdgeUses ();
  this->collectOffLoopEdgeUses ();

  _isConsistent = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM3D_Edge
::isConsistent () const
{
  return _isConsistent;
}

WH_GM3D_Body* WH_GM3D_Edge
::body () const
{
  return _body;
}

WH_GM3D_EdgeVertexUse* WH_GM3D_Edge
::firstVertexUse () const
{
  return _firstVertexUse;
}

WH_GM3D_EdgeVertexUse* WH_GM3D_Edge
::lastVertexUse () const
{
  return _lastVertexUse;
}

const vector<WH_GM3D_LoopEdgeUse*>& WH_GM3D_Edge
::loopEdgeUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _loopEdgeUse_s;
}

const vector<WH_GM3D_OffLoopEdgeUse*>& WH_GM3D_Edge
::offLoopEdgeUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _offLoopEdgeUse_s;
}

WH_Segment3D WH_GM3D_Edge
::segment () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->firstVertexUse () != WH_NULL); 
  WH_ASSERT(this->lastVertexUse () != WH_NULL); 
  WH_ASSERT(WH_ne (this->firstVertexUse ()->vertex ()->point (), 
		   this->lastVertexUse ()->vertex ()->point ()));
  
  return WH_Segment3D 
    (this->firstVertexUse ()->vertex ()->point (), 
     this->lastVertexUse ()->vertex ()->point ());
}

void WH_GM3D_Edge
::getFaces 
(vector<WH_GM3D_Face*>& face_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  /* NEED TO REDEFINE */
  /* <loopEdgeUse_s ()> is not sorted yet */

  WH_CVR_LINE;

  face_s_OUT.clear ();

  for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->loopEdgeUse_s ().begin ();
       i_edgeUse != this->loopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
    face_s_OUT.push_back (edgeUse_i->loop ()->face ());
  }

  for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->offLoopEdgeUse_s ().begin ();
       i_edgeUse != this->offLoopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
    face_s_OUT.push_back (edgeUse_i->face ());
  }
}

bool WH_GM3D_Edge
::isWire () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return (this->loopEdgeUse_s ().size () == 0
	  && this->offLoopEdgeUse_s ().size () == 0);
}

bool WH_GM3D_Edge
::isVolume () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  int outerBoundaryFaceCount = 0;
  for (vector<WH_GM3D_LoopEdgeUse*>::const_iterator 
	 i_edgeUse = this->loopEdgeUse_s ().begin ();
       i_edgeUse != this->loopEdgeUse_s ().end ();
       i_edgeUse++) {
    WH_GM3D_LoopEdgeUse* edgeUse_i = (*i_edgeUse);
    WH_GM3D_Face* face = edgeUse_i->loop ()->face ();
    if (face->faceType () == WH_GM3D_Face::OUTER_BOUNDARY) {
      WH_CVR_LINE;
      outerBoundaryFaceCount++;
    }
  }
  WH_ASSERT(outerBoundaryFaceCount % 2 == 0);

  return (0 < outerBoundaryFaceCount);
}

void WH_GM3D_Edge
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->firstVertexUse () != WH_NULL); 
  WH_ASSERT(this->lastVertexUse () != WH_NULL); 
  
  minRange_OUT = this->firstVertexUse ()->vertex ()->point ();
  maxRange_OUT = this->lastVertexUse ()->vertex ()->point ();
  WH_sortPair (minRange_OUT, maxRange_OUT);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}



/* test coverage completed */

