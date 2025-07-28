/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d_brep.cc : boundary representation module of 2-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d_brep.h"
#include "sorter.h"



/* class WH_GM2D_Vertex */

WH_GM2D_Vertex
::WH_GM2D_Vertex 
(WH_GM2D_Body* body,
 const WH_Vector2D& point) : _point (point)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;
  
  _body = body;

  _isConsistent = false;
}

WH_GM2D_Vertex
::~WH_GM2D_Vertex () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_Vertex
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM2D_Vertex
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(WH_T_Exists (this->body ()->vertex_s (), this));

    WH_ASSERT(this->edgeVertexUsesAreSorted ());
    WH_ASSERT(!WH_T_HasDuplication (this->edgeVertexUse_s ()));
    for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	   i_vertexUse = this->edgeVertexUse_s ().begin ();
	 i_vertexUse != this->edgeVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
      WH_ASSERT(vertexUse_i->vertex () == this);
    }

    vector<WH_GM2D_Edge*> edge_s;
    this->getEdges 
      (edge_s);
    WH_ASSERT(!WH_T_HasDuplication (edge_s));
    for (vector<WH_GM2D_Edge*>::const_iterator 
	   i_edge = edge_s.begin ();
	 i_edge != edge_s.end ();
	 i_edge++) {
      WH_GM2D_Edge* edge_i = (*i_edge);
      WH_ASSERT (edge_i->firstVertexUse ()->vertex () == this
		 || edge_i->lastVertexUse ()->vertex () == this);
    }

    int nEdges = (int)this->edgeVertexUse_s ().size ();
    int wireEdgeCount = 0;
    int outerBoundaryEdgeCount = 0;
    int innerBoundaryEdgeCount = 0;
    for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	   i_vertexUse = this->edgeVertexUse_s ().begin ();
	 i_vertexUse != this->edgeVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);

      WH_GM2D_Edge* edge = vertexUse_i->edge ();
      switch (edge->edgeType ()) {
      case WH_GM2D_Edge::WIRE:
	WH_CVR_LINE;
	wireEdgeCount++;
	break;
      case WH_GM2D_Edge::INNER_BOUNDARY:
	WH_CVR_LINE;
	innerBoundaryEdgeCount++;
	break;
      case WH_GM2D_Edge::OUTER_BOUNDARY:
	WH_CVR_LINE;
	outerBoundaryEdgeCount++;
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
    WH_ASSERT(outerBoundaryEdgeCount % 2 == 0);

    WH_ASSERT(!(this->isDot () && this->isVolume ()));

    if (this->isDot ()) {
      WH_CVR_LINE;
      WH_ASSERT(nEdges == 0);
    }

    if (this->isVolume ()) {
      WH_CVR_LINE;
      WH_ASSERT(2 <= outerBoundaryEdgeCount);
    }
  }
    
  return true;
}

void WH_GM2D_Vertex
::translate (const WH_Vector2D& offset)
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

void WH_GM2D_Vertex
::rotate (const WH_Vector2D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;
  
  _point = _point.rotate (center, angle);

  _isConsistent = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->isConsistent ());
#endif
}

void WH_GM2D_Vertex
::scale (const WH_Vector2D& center, double ratio)
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

bool WH_GM2D_Vertex
::edgeVertexUsesAreSorted () const
{
  WH_CVR_LINE;

  bool result = true;
  
  if (_edgeVertexUse_s.size () < 2) {
    WH_CVR_LINE;
    return result;
  }

  /* check if <_edgeVertexUse_s> are sorted by slope of edge */
  
  WH_GM2D_EdgeVertexUse* prevVertexUse = WH_NULL;
  double prevSlope = 0.0;
  for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = _edgeVertexUse_s.begin ();
       i_vertexUse != _edgeVertexUse_s.end ();
       i_vertexUse++) {
    WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
    
    /* get the slope of the current edge */
    WH_GM2D_Edge* edge = vertexUse_i->edge ();
    WH_Vector2D direction (0, 0);
    if (edge->firstVertexUse () == vertexUse_i) {
      WH_CVR_LINE;
      direction 
	= edge->lastVertexUse ()->vertex ()->point ()
	- edge->firstVertexUse ()->vertex ()->point ();
    } else {
      WH_CVR_LINE;
      WH_ASSERT(edge->lastVertexUse () == vertexUse_i);
      direction 
	= edge->firstVertexUse ()->vertex ()->point ()
	- edge->lastVertexUse ()->vertex ()->point ();
    }
    double slope = direction.slope ();
      
    if (prevVertexUse != WH_NULL) {
      WH_CVR_LINE;
      
      if (WH_le (slope, prevSlope)) {
	/*  SHOULD NEVER PASS : WH_CVR_LINE;  */
	result = false;
	break;
      }
    }
    
    prevVertexUse = vertexUse_i;
    prevSlope = slope;
  }

  return result;
}

void WH_GM2D_Vertex
::collectEdgeVertexUses ()
{
  /* PRE-CONDITION */
  /* <_body->edge_s ()> must have been set. */
  /* <firstVertexUse ()> and <lastVertexUse ()> of the edges must have
     been set. */

  WH_CVR_LINE;

  _edgeVertexUse_s.clear ();

  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = _body->edge_s ().begin ();
       i_edge != _body->edge_s ().end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);
    
    if (edge_i->firstVertexUse ()->vertex () == this) {
      WH_CVR_LINE;
      _edgeVertexUse_s.push_back (edge_i->firstVertexUse ());
    } else if (edge_i->lastVertexUse ()->vertex () == this) {
      WH_CVR_LINE;
      _edgeVertexUse_s.push_back (edge_i->lastVertexUse ());
    }
  }
}

void WH_GM2D_Vertex
::sortEdgeVertexUses ()
{
  WH_CVR_LINE;

  /* sort <_edgeVertexUse_s> in ascendant order by slope of edge */

  if (_edgeVertexUse_s.size () < 2) {
    WH_CVR_LINE;
    return;
  }
  
  WH_Sorter sorter;
  sorter.initialize ((int)_edgeVertexUse_s.size ());

  int count = 0;
  for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = _edgeVertexUse_s.begin ();
       i_vertexUse != _edgeVertexUse_s.end ();
       i_vertexUse++) {
    WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
    
    /* get the slope of edge */
    WH_GM2D_Edge* edge = vertexUse_i->edge ();
    WH_Vector2D direction (0, 0);
    if (edge->firstVertexUse () == vertexUse_i) {
      WH_CVR_LINE;
      direction 
	= edge->lastVertexUse ()->vertex ()->point ()
	- edge->firstVertexUse ()->vertex ()->point ();
    } else {
      WH_CVR_LINE;
      WH_ASSERT(edge->lastVertexUse () == vertexUse_i);
      direction 
	= edge->firstVertexUse ()->vertex ()->point ()
	- edge->lastVertexUse ()->vertex ()->point ();
    }
    double slope = direction.slope ();

    sorter.setItemAt (count, vertexUse_i, slope);
    count++;
  }  

  sorter.sortInAscendantOrder ();

  _edgeVertexUse_s.clear ();
  for (int i = 0; i < sorter.nItems (); i++) {
    WH_GM2D_EdgeVertexUse* vertexUse 
      = (WH_GM2D_EdgeVertexUse*)sorter.itemAt (i);
    _edgeVertexUse_s.push_back (vertexUse);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->edgeVertexUsesAreSorted ());
#endif
}

void WH_GM2D_Vertex
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  this->collectEdgeVertexUses ();
  this->sortEdgeVertexUses ();

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM2D_Vertex
::isConsistent () const
{
  return _isConsistent;
}

WH_GM2D_Body* WH_GM2D_Vertex
::body () const
{
  return _body;
}

const vector<WH_GM2D_EdgeVertexUse*>& WH_GM2D_Vertex
::edgeVertexUse_s () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  return _edgeVertexUse_s;
}
  
WH_Vector2D WH_GM2D_Vertex
::point () const
{
  return _point;
}

void WH_GM2D_Vertex
::getEdges 
(vector<WH_GM2D_Edge*>& edge_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  edge_s_OUT.clear ();

  for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = this->edgeVertexUse_s ().begin ();
       i_vertexUse != this->edgeVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
    edge_s_OUT.push_back (vertexUse_i->edge ());
  }
}

bool WH_GM2D_Vertex
::isDot () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  return (this->edgeVertexUse_s ().size () == 0);
}

bool WH_GM2D_Vertex
::isVolume () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  int outerBoundaryEdgeCount = 0;
  for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = this->edgeVertexUse_s ().begin ();
       i_vertexUse != this->edgeVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);

    WH_GM2D_Edge* edge = vertexUse_i->edge ();
    if (edge->edgeType () == WH_GM2D_Edge::OUTER_BOUNDARY) {
      WH_CVR_LINE;
      outerBoundaryEdgeCount++;
    }
  }
  WH_ASSERT(outerBoundaryEdgeCount % 2 == 0);

  return (0 < outerBoundaryEdgeCount);
}

WH_GM2D_Vertex::VolumeMergeType WH_GM2D_Vertex
::volumeMergeType () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->isVolume ());

  WH_CVR_LINE;

  VolumeMergeType result = MERGE_MORE_THAN_TWO_EDGES;

  int outerBoundaryEdgeCount = 0;
  WH_GM2D_Edge* edge0 = WH_NULL;
  WH_GM2D_Edge* edge1 = WH_NULL;
  for (vector<WH_GM2D_EdgeVertexUse*>::const_iterator 
	 i_vertexUse = this->edgeVertexUse_s ().begin ();
       i_vertexUse != this->edgeVertexUse_s ().end ();
       i_vertexUse++) {
    WH_GM2D_EdgeVertexUse* vertexUse_i = (*i_vertexUse);
    
    WH_GM2D_Edge* edge = vertexUse_i->edge ();
    if (edge->edgeType () == WH_GM2D_Edge::OUTER_BOUNDARY) {
      WH_CVR_LINE;
      outerBoundaryEdgeCount++;
      if (edge0 == WH_NULL) {
	WH_CVR_LINE;
	edge0 = edge;
      } else if (edge1 == WH_NULL) {
	WH_CVR_LINE;
	edge1 = edge;
      }
    }
  }
  WH_ASSERT(outerBoundaryEdgeCount % 2 == 0);

  if (2 < outerBoundaryEdgeCount) {
    WH_CVR_LINE;
    result = MERGE_MORE_THAN_TWO_EDGES;
    
  } else if (outerBoundaryEdgeCount == 2) {
    WH_CVR_LINE;
    WH_ASSERT(edge0->edgeType () == WH_GM2D_Edge::OUTER_BOUNDARY);
    WH_ASSERT(edge1->edgeType () == WH_GM2D_Edge::OUTER_BOUNDARY);
    WH_ASSERT(edge0 != edge1);
    
    /* obtain offset segments toward outside of edges using their normal
       vector */
    
    WH_Vector2D normal0;
    bool flag = edge0->getNormalToOutsideVolume 
      (normal0);
    WH_ASSERT(flag);
    WH_Segment2D segment0 = edge0->segment ();
    WH_Segment2D offsetSegment0
      (segment0.p0 () + normal0 * 1.0, 
       segment0.p1 () + normal0 * 1.0);
    /* MAGIC NUMBER : using length 1.0 of <normal0> as offset */
    
    WH_Vector2D normal1;
    flag = edge1->getNormalToOutsideVolume 
      (normal1);
    WH_ASSERT(flag);
    WH_Segment2D segment1 = edge1->segment ();
    WH_Segment2D offsetSegment1
      (segment1.p0 () + normal1, segment1.p1 () + normal1);
    /* MAGIC NUMBER : using length 1.0 of <normal1> as offset */
    
    /* identify merge type of edges by checking intersection between two
       offset segments */
    WH_Vector2D intersectionPoint;
    WH_Segment2D::WithSegmentIntersectionType intersectionType 
      = offsetSegment0.checkIntersectionWith 
      (offsetSegment1, 
       intersectionPoint);
    switch (intersectionType) {
    case WH_Segment2D::NO_INTERSECTION_WITH_SEGMENT:
      WH_CVR_LINE;
      result = MERGE_CONVEX;
      break;
    case WH_Segment2D::POINT_WITH_SEGMENT:
      WH_CVR_LINE;
      result = MERGE_CONCAVE;
      break;
    case WH_Segment2D::PARALLEL_WITH_SEGMENT:
      WH_ASSERT_NO_REACH;
      break;
    case WH_Segment2D::COINCIDENT_WITH_SEGMENT:
      WH_CVR_LINE;
      result = MERGE_180;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }

  return result;
}



/* class WH_GM2D_EdgeVertexUse */

WH_GM2D_EdgeVertexUse
::WH_GM2D_EdgeVertexUse 
(WH_GM2D_Edge* edge,
 WH_GM2D_Vertex* vertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);
  
  WH_CVR_LINE;

  _edge = edge;
  _vertex = vertex;

  _isConsistent = false;
}

WH_GM2D_EdgeVertexUse
::~WH_GM2D_EdgeVertexUse () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_EdgeVertexUse
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

bool WH_GM2D_EdgeVertexUse
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

    WH_Vector2D vertexPoint = this->vertex ()->point ();
    WH_Line2D edgeLine = this->edge ()->segment ().line ();
    WH_ASSERT(edgeLine.contains (vertexPoint)); 

    double param = this->parameter ();
    WH_ASSERT(WH_eq (vertexPoint, edgeLine.positionAt (param)));

    vector<WH_GM2D_Edge*> edge_s;
    this->getConnectedEdges 
      (edge_s);
    WH_ASSERT(!WH_T_HasDuplication (edge_s));
    for (vector<WH_GM2D_Edge*>::const_iterator 
	   i_edge = edge_s.begin ();
	 i_edge != edge_s.end ();
	 i_edge++) {
      WH_GM2D_Edge* edge_i = (*i_edge);
      WH_ASSERT(edge_i != this->edge ());
      WH_ASSERT(edge_i->firstVertexUse ()->vertex () == this->vertex ()
		|| edge_i->lastVertexUse ()->vertex () == this->vertex ());
    }
  }
  
  return true;
}

void WH_GM2D_EdgeVertexUse
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

bool WH_GM2D_EdgeVertexUse
::isConsistent () const
{
  return _isConsistent;
}

WH_GM2D_Edge* WH_GM2D_EdgeVertexUse
::edge () const
{
  return _edge;
}

WH_GM2D_Vertex* WH_GM2D_EdgeVertexUse
::vertex () const
{
  return _vertex;
}

double WH_GM2D_EdgeVertexUse
::parameter () const
{
  WH_Vector2D vertexPoint = this->vertex ()->point ();
  WH_Line2D edgeLine = this->edge ()->segment ().line ();
  return edgeLine.parameterAt (vertexPoint);
}

void WH_GM2D_EdgeVertexUse
::getConnectedEdges 
(vector<WH_GM2D_Edge*>& edge_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  edge_s_OUT.clear ();

  vector<WH_GM2D_Edge*> edgeOfVertex_s;
  this->vertex ()->getEdges 
    (edgeOfVertex_s);
  WH_ASSERT(0 < edgeOfVertex_s.size ());

  /* <edgeOfVertex_s> is a cyclic list sorted by angle */

  /* move the latter part of <edgeOfVertex_s> into <edge_s_OUT> */
  bool thisIsFound = false;
  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = edgeOfVertex_s.begin ();
       i_edge != edgeOfVertex_s.end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);

    if (thisIsFound) {
      WH_CVR_LINE;
      edge_s_OUT.push_back (edge_i);
    }
    if (edge_i == this->edge ()) {
      WH_CVR_LINE;
      thisIsFound = true;
    } 
  }
  WH_ASSERT(thisIsFound);
  
  /* move the former part of <edgeOfVertex_s> into <edge_s_OUT> */
  thisIsFound = false;
  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = edgeOfVertex_s.begin ();
       i_edge != edgeOfVertex_s.end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);

    if (edge_i == this->edge ()) {
      WH_CVR_LINE;
      thisIsFound = true;
    } 
    if (!thisIsFound) {
      WH_CVR_LINE;
      edge_s_OUT.push_back (edge_i);
    } 
  }
  WH_ASSERT(thisIsFound);
}



/* class WH_GM2D_EdgeSide */

WH_GM2D_EdgeSide
::WH_GM2D_EdgeSide 
(WH_GM2D_Edge* edge,
 bool isInsideVolume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  
  WH_CVR_LINE;

  _edge = edge;
  _isInsideVolume = isInsideVolume;

  _isConsistent = false;
}

WH_GM2D_EdgeSide
::~WH_GM2D_EdgeSide () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_EdgeSide
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->edge () != WH_NULL);

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM2D_EdgeSide
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->edge ()->frontSide () == this
	      || this->edge ()->backSide () == this);
  }
  
  return true;
}

void WH_GM2D_EdgeSide
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

bool WH_GM2D_EdgeSide
::isConsistent () const
{
  return _isConsistent;
}

WH_GM2D_Edge* WH_GM2D_EdgeSide
::edge () const
{
  return _edge;
}

bool WH_GM2D_EdgeSide
::isInsideVolume () const
{
  return _isInsideVolume;
}



/* class WH_GM2D_Edge */

WH_GM2D_Edge
::WH_GM2D_Edge 
(WH_GM2D_Body* body) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;

  _body = body;
  _firstVertexUse = WH_NULL;
  _lastVertexUse = WH_NULL;
  _frontSide = WH_NULL;
  _backSide = WH_NULL;

  _isConsistent = false;
}

WH_GM2D_Edge
::~WH_GM2D_Edge () 
{
  WH_CVR_LINE;
  
  delete _firstVertexUse;
  delete _lastVertexUse;
  delete _frontSide;
  delete _backSide;
}

bool WH_GM2D_Edge
::checkInvariant () const
{
  WH_CVR_LINE;

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_ASSERT(this->body () != WH_NULL);
    WH_ASSERT(this->firstVertexUse () != WH_NULL);
    WH_ASSERT(this->lastVertexUse () != WH_NULL);
    WH_ASSERT(this->frontSide () != WH_NULL);
    WH_ASSERT(this->backSide () != WH_NULL);
    
    WH_GM2D_Vertex* firstVertex = this->firstVertexUse ()->vertex ();
    WH_GM2D_Vertex* lastVertex = this->lastVertexUse ()->vertex ();
    WH_ASSERT(firstVertex != lastVertex);
  
    WH_ASSERT(WH_ne (firstVertex->point (), lastVertex->point ()));
  }

  return true;
}

bool WH_GM2D_Edge
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  if (this->isConsistent ()) {
    WH_CVR_LINE;

    _firstVertexUse->assureInvariant ();
    _lastVertexUse->assureInvariant ();
    
    _frontSide->assureInvariant ();
    _backSide->assureInvariant ();



    WH_ASSERT(WH_T_Exists (this->body ()->edge_s (), this));

    WH_Vector2D minRange;
    WH_Vector2D maxRange;
    this->getRange 
      (minRange, maxRange);
    WH_ASSERT(WH_ne (minRange, maxRange));
    WH_ASSERT(WH_le (minRange, maxRange));

    WH_ASSERT(this->firstVertexUse ()->edge () == this);
    WH_ASSERT(this->lastVertexUse ()->edge () == this);
    WH_ASSERT(WH_ne (this->firstVertexUse ()->parameter (),
		     this->lastVertexUse ()->parameter ()));

    WH_GM2D_Vertex* firstVertex = this->firstVertexUse ()->vertex ();
    WH_GM2D_Vertex* lastVertex = this->lastVertexUse ()->vertex ();
    WH_ASSERT(firstVertex != lastVertex);
    WH_ASSERT(WH_ne (firstVertex->point (), lastVertex->point ()));
    WH_ASSERT(WH_between (firstVertex->point (), minRange, maxRange));
    WH_ASSERT(WH_between (lastVertex->point (), minRange, maxRange));

    WH_ASSERT(WH_eq (this->segment (),
		     WH_Segment2D (firstVertex->point (),
				   lastVertex->point ())));

    WH_ASSERT(this->frontSide ()->edge () == this);
    WH_ASSERT(this->backSide ()->edge () == this);

    WH_Vector2D normal;
    bool normalFlag = this->getNormalToOutsideVolume 
      (normal);
    
    switch (this->edgeType ()) {
    case WIRE:
      WH_ASSERT(!normalFlag);
      break;
    case INNER_BOUNDARY:
      WH_ASSERT(!normalFlag);
      break;
    case OUTER_BOUNDARY:
      WH_ASSERT(normalFlag);
      WH_ASSERT(WH_eq (normal, this->segment ().line ().normal ())
		|| WH_isReverse (normal, this->segment ().line ().normal ()));
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    };

    {
      WH_GM2D_EdgeVertexUse* vertexUse = this->firstVertexUse ();

      WH_GM2D_EdgeSide* frontSide 
	= this->connectedEdgeSide (this->frontSide (), vertexUse);
      if (frontSide != WH_NULL) {
	WH_ASSERT(frontSide->edge () != this);
	WH_ASSERT(frontSide->edge ()->firstVertexUse ()->vertex () 
		  == vertexUse->vertex ()
		  || frontSide->edge ()->lastVertexUse ()->vertex () 
		  == vertexUse->vertex ());
      }
      
      WH_GM2D_EdgeSide* backSide 
	= this->connectedEdgeSide (this->backSide (), vertexUse);
      if (backSide != WH_NULL) {
	WH_ASSERT(backSide->edge () != this);
	WH_ASSERT(backSide->edge ()->firstVertexUse ()->vertex () 
		  == vertexUse->vertex ()
		  || backSide->edge ()->lastVertexUse ()->vertex () 
		  == vertexUse->vertex ());
      }
    }
    
    {
      WH_GM2D_EdgeVertexUse* vertexUse = this->lastVertexUse ();

      WH_GM2D_EdgeSide* frontSide 
	= this->connectedEdgeSide (this->frontSide (), vertexUse);
      if (frontSide != WH_NULL) {
	WH_ASSERT(frontSide->edge () != this);
	WH_ASSERT(frontSide->edge ()->firstVertexUse ()->vertex () 
		  == vertexUse->vertex ()
		  || frontSide->edge ()->lastVertexUse ()->vertex () 
		  == vertexUse->vertex ());
      }
      
      WH_GM2D_EdgeSide* backSide 
	= this->connectedEdgeSide (this->backSide (), vertexUse);
      if (backSide != WH_NULL) {
	WH_ASSERT(backSide->edge () != this);
	WH_ASSERT(backSide->edge ()->firstVertexUse ()->vertex () 
		  == vertexUse->vertex ()
		  || backSide->edge ()->lastVertexUse ()->vertex () 
		  == vertexUse->vertex ());
      }
    }

  }
  
  return true;
}

WH_GM2D_EdgeVertexUse* WH_GM2D_Edge
::createEdgeVertexUse (WH_GM2D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_GM2D_EdgeVertexUse* result 
    = new WH_GM2D_EdgeVertexUse (this, vertex);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM2D_EdgeSide* WH_GM2D_Edge
::createEdgeSide (bool isInsideVolume)
{
  WH_GM2D_EdgeSide* result 
    = new WH_GM2D_EdgeSide (this, isInsideVolume);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_GM2D_Edge
::setVertexs
(WH_GM2D_Vertex* firstVertex,
 WH_GM2D_Vertex* lastVertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(this->firstVertexUse () == WH_NULL);
  WH_ASSERT(this->lastVertexUse () == WH_NULL);
  WH_ASSERT(firstVertex != WH_NULL);
  WH_ASSERT(lastVertex != WH_NULL);
  WH_ASSERT(firstVertex != lastVertex);
  WH_ASSERT(WH_ne (firstVertex->point (), lastVertex->point ()));
  
  _firstVertexUse = this->createEdgeVertexUse (firstVertex);
  _lastVertexUse = this->createEdgeVertexUse (lastVertex);
}

void WH_GM2D_Edge
::setSides 
(bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isConsistent ());
  WH_ASSERT(this->frontSide () == WH_NULL);
  WH_ASSERT(this->backSide () == WH_NULL);
  
  _frontSide = this->createEdgeSide (frontSideIsInsideVolume);
  _backSide = this->createEdgeSide (backSideIsInsideVolume);
}

void WH_GM2D_Edge
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  WH_ASSERT(this->firstVertexUse () != WH_NULL);
  WH_ASSERT(this->lastVertexUse () != WH_NULL);
  WH_ASSERT(this->frontSide () != WH_NULL);
  WH_ASSERT(this->backSide () != WH_NULL);
  
  WH_CVR_LINE;

  _firstVertexUse->fix ();
  _lastVertexUse->fix ();

  _frontSide->fix ();
  _backSide->fix ();

  _isConsistent = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_GM2D_Edge
::isConsistent () const
{
  return _isConsistent;
}

WH_GM2D_Body* WH_GM2D_Edge
::body () const
{
  return _body;
}

WH_GM2D_EdgeVertexUse* WH_GM2D_Edge
::firstVertexUse () const
{
  return _firstVertexUse;
}

WH_GM2D_EdgeVertexUse* WH_GM2D_Edge
::lastVertexUse () const
{
  return _lastVertexUse;
}

WH_GM2D_EdgeSide* WH_GM2D_Edge
::frontSide () const
{
  return _frontSide;
}

WH_GM2D_EdgeSide* WH_GM2D_Edge
::backSide () const
{
  return _backSide;
}

WH_GM2D_Edge::EdgeType WH_GM2D_Edge
::edgeType () const
{
  if (this->frontSide ()->isInsideVolume ()) {
    WH_CVR_LINE;
    if (this->backSide ()->isInsideVolume ()) {
      WH_CVR_LINE;
      return INNER_BOUNDARY;
    } else {
      WH_CVR_LINE;
      return OUTER_BOUNDARY;
    }
  } else {
    WH_CVR_LINE;
    if (this->backSide ()->isInsideVolume ()) {
      WH_CVR_LINE;
      return OUTER_BOUNDARY;
    } else {
      WH_CVR_LINE;
      return WIRE;
    }
  }
}

WH_Segment2D WH_GM2D_Edge
::segment () const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (this->firstVertexUse ()->vertex ()->point (), 
		   this->lastVertexUse ()->vertex ()->point ()));
  
  return WH_Segment2D 
    (this->firstVertexUse ()->vertex ()->point (), 
     this->lastVertexUse ()->vertex ()->point ());
}

bool WH_GM2D_Edge
::getNormalToOutsideVolume 
(WH_Vector2D& normal_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  if (this->frontSide ()->isInsideVolume ()) {
    WH_CVR_LINE;
    if (this->backSide ()->isInsideVolume ()) {
      WH_CVR_LINE;
      return false;
    } else {
      WH_CVR_LINE;
      normal_OUT = -this->segment ().line ().normal ();
      return true;
    }
  } else {
    WH_CVR_LINE;
    if (this->backSide ()->isInsideVolume ()) {
      WH_CVR_LINE;
      normal_OUT = this->segment ().line ().normal ();
      return true;
    } else {
      WH_CVR_LINE;
      return false;
    }
  }
}
  
void WH_GM2D_Edge
::getRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
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

WH_GM2D_EdgeSide* WH_GM2D_Edge
::connectedEdgeSide 
(WH_GM2D_EdgeSide* side,
 WH_GM2D_EdgeVertexUse* vertexUse) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(side != WH_NULL);
  WH_ASSERT(side == this->frontSide () 
	    || side == this->backSide ());
  WH_ASSERT(vertexUse != WH_NULL);
  WH_ASSERT(vertexUse == this->firstVertexUse () 
	    || vertexUse == this->lastVertexUse ());
  
  WH_CVR_LINE;

  WH_GM2D_EdgeSide* result = WH_NULL;

  vector<WH_GM2D_Edge*> connectedEdge_s;
  vertexUse->getConnectedEdges 
    (connectedEdge_s);

  if (0 < connectedEdge_s.size ()) {
    WH_CVR_LINE;
    if (side == this->frontSide ()) {
      WH_CVR_LINE;
      if (vertexUse == this->firstVertexUse ()) {
	WH_CVR_LINE;
	WH_GM2D_Edge* neighborEdge = connectedEdge_s.front (); 
	if (neighborEdge->firstVertexUse ()->vertex () 
	    == vertexUse->vertex ()) {
	  WH_CVR_LINE;
	  result = neighborEdge->backSide ();
	} else {
	  WH_CVR_LINE;
	  result = neighborEdge->frontSide ();
	}
      } else {
	WH_GM2D_Edge* neighborEdge = connectedEdge_s.back (); 
	if (neighborEdge->firstVertexUse ()->vertex () 
	    == vertexUse->vertex ()) {
	  WH_CVR_LINE;
	  result = neighborEdge->frontSide ();
	} else {
	  WH_CVR_LINE;
	  result = neighborEdge->backSide ();
	}
      }
    } else {
      WH_CVR_LINE;
      if (vertexUse == this->firstVertexUse ()) {
	WH_CVR_LINE;
	WH_GM2D_Edge* neighborEdge = connectedEdge_s.back (); 
	if (neighborEdge->firstVertexUse ()->vertex () 
	    == vertexUse->vertex ()) {
	  WH_CVR_LINE;
	  result = neighborEdge->frontSide ();
	} else {
	  WH_CVR_LINE;
	  result = neighborEdge->backSide ();
	}
      } else {
	WH_CVR_LINE;
	WH_GM2D_Edge* neighborEdge = connectedEdge_s.front (); 
	if (neighborEdge->firstVertexUse ()->vertex () 
	    == vertexUse->vertex ()) {
	  WH_CVR_LINE;
	  result = neighborEdge->backSide ();
	} else {
	  WH_CVR_LINE;
	  result = neighborEdge->frontSide ();
	}
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}



/* class WH_GM2D_Body */

WH_GM2D_Body
::WH_GM2D_Body (bool isRegular) 
{
  WH_CVR_LINE;

  _isConsistent = false;
  _isRegular = isRegular;
}

WH_GM2D_Body
::~WH_GM2D_Body () 
{
  WH_CVR_LINE;

  WH_T_Delete (_vertex_s);
  WH_T_Delete (_edge_s);
}

bool WH_GM2D_Body
::checkInvariant () const
{
  WH_CVR_LINE;

  if (this->isConsistent ()) {
    WH_CVR_LINE;

  }

  return true;
}

bool WH_GM2D_Body
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isConsistent ()) {
    WH_CVR_LINE;

    WH_T_AssureInvariant (_vertex_s);
    WH_T_AssureInvariant (_edge_s);
  


    /* count number of vertexs of each type */
    int nVertexs = (int)this->vertex_s ().size ();
    int dotVertexCount = 0;
    for (vector<WH_GM2D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM2D_Vertex* vertex_i = (*i_vertex);

      WH_ASSERT(vertex_i->body () == this);
      if (vertex_i->isDot ()) {
	WH_CVR_LINE;
	dotVertexCount++;
      }
    }

    /* count number of edges of each type */
    int nEdges = (int)this->edge_s ().size ();
    int wireEdgeCount = 0;
    int outerBoundaryEdgeCount = 0;
    int innerBoundaryEdgeCount = 0;
    for (vector<WH_GM2D_Edge*>::const_iterator 
	   i_edge = this->edge_s ().begin ();
	 i_edge != this->edge_s ().end ();
	 i_edge++) {
      WH_GM2D_Edge* edge_i = (*i_edge);
      
      WH_ASSERT(edge_i->body () == this);
      switch (edge_i->edgeType ()) {
      case WH_GM2D_Edge::WIRE:
	WH_CVR_LINE;
	wireEdgeCount++;
	break;
      case WH_GM2D_Edge::INNER_BOUNDARY:
	WH_CVR_LINE;
	innerBoundaryEdgeCount++;
	break;
      case WH_GM2D_Edge::OUTER_BOUNDARY:
	WH_CVR_LINE;
	outerBoundaryEdgeCount++;
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

      break;
    case DOT_SET:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(0 < dotVertexCount);
      WH_ASSERT(nVertexs == dotVertexCount);

      WH_ASSERT(nEdges == 0);

      break;
    case WIRE_SET:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(dotVertexCount == 0);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(0 < wireEdgeCount);
      WH_ASSERT(outerBoundaryEdgeCount == 0);
      WH_ASSERT(innerBoundaryEdgeCount == 0);
      WH_ASSERT(nEdges == wireEdgeCount);

      break;
    case VOLUME:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(dotVertexCount == 0);

      WH_ASSERT(0 < nEdges);
      WH_ASSERT(wireEdgeCount == 0);
      WH_ASSERT(0 < outerBoundaryEdgeCount);
      WH_ASSERT(innerBoundaryEdgeCount == 0);
      WH_ASSERT(nEdges == outerBoundaryEdgeCount);

      WH_ASSERT(nEdges == nVertexs);

      break;
    case OTHER:
      WH_CVR_LINE;

      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(0 <= dotVertexCount);
      
      WH_ASSERT(0 < nEdges);
      WH_ASSERT(0 <= wireEdgeCount);
      WH_ASSERT(0 <= outerBoundaryEdgeCount);
      WH_ASSERT(0 <= innerBoundaryEdgeCount);

      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
    
    if (this->isRegular ()) {
      WH_ASSERT(this->bodyType () == EMPTY
		|| this->bodyType () == VOLUME);
    }

    WH_ASSERT(!WH_T_HasDuplication (this->vertex_s ()));

    /* check overlap of vertexs */
    for (vector<WH_GM2D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM2D_Vertex* vertex_i = (*i_vertex);

      for (vector<WH_GM2D_Vertex*>::const_iterator 
	     j_vertex = i_vertex + 1;
	   j_vertex != this->vertex_s ().end ();
	   j_vertex++) {
	WH_GM2D_Vertex* vertex_j = (*j_vertex);
	
	WH_ASSERT(WH_ne (vertex_j->point (), vertex_i->point ()));
      }
    }

    WH_ASSERT(!WH_T_HasDuplication (this->edge_s ()));

    /* check intersection of edges */
    for (vector<WH_GM2D_Edge*>::const_iterator 
	   i_edge = this->edge_s ().begin ();
	 i_edge != this->edge_s ().end ();
	 i_edge++) {
      WH_GM2D_Edge* edge_i = (*i_edge);
      WH_Segment2D segment_i = edge_i->segment ();

      for (vector<WH_GM2D_Edge*>::const_iterator 
	     j_edge = i_edge + 1;
	   j_edge != this->edge_s ().end ();
	   j_edge++) {
	WH_GM2D_Edge* edge_j = (*j_edge);
	WH_Segment2D segment_j = edge_j->segment ();

	WH_SegmentIntersector2D intersector (segment_j, segment_i);  
	intersector.perform ();
	WH_ASSERT(!intersector.intersects ()
		  || intersector.intersectsAtVertexOnly ());
      }
    }

  }
  
  return true;
}

void WH_GM2D_Body
::addVertex (WH_GM2D_Vertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->vertex_s (), vertex));
  {
    for (vector<WH_GM2D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM2D_Vertex* vertex_i = (*i_vertex);
      WH_ASSERT(WH_ne (vertex_i->point (), vertex->point ()));
    }
  }
#endif

  _vertex_s.push_back (vertex);
}

void WH_GM2D_Body
::addEdge (WH_GM2D_Edge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->edge_s (), edge));
#endif

  _edge_s.push_back (edge);
}

void WH_GM2D_Body
::fix ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent () || !this->isConsistent ());
  
  WH_CVR_LINE;

  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    vertex_i->fix ();
  }
  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = _edge_s.begin ();
       i_edge != _edge_s.end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);
    edge_i->fix ();
  }

  _isConsistent = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM2D_Body
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

void WH_GM2D_Body
::clear (bool isRegular) 
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = false;
  _isRegular = isRegular;

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

void WH_GM2D_Body
::copyFrom (WH_GM2D_Body* body)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(body != WH_NULL);
  
  WH_CVR_LINE;

  this->clear (body->isRegular ());

  _isConsistent = false;

  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = body->vertex_s ().begin ();
       i_vertex != body->vertex_s ().end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    WH_GM2D_Vertex* newVertex 
      = this->createVertex (vertex_i->point ());
    this->addVertex (newVertex);
  }

  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = body->edge_s ().begin ();
       i_edge != body->edge_s ().end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);
    
    WH_GM2D_Edge* newEdge = this->createEdge ();
    WH_GM2D_Vertex* firstVertex 
      = this->findVertex (edge_i->firstVertexUse ()->vertex ()->point ());
    WH_ASSERT(firstVertex != WH_NULL);
    WH_GM2D_Vertex* lastVertex 
      = this->findVertex (edge_i->lastVertexUse ()->vertex ()->point ());
    WH_ASSERT(lastVertex != WH_NULL);
    newEdge->setVertexs (firstVertex, lastVertex);
    newEdge->setSides 
      (edge_i->frontSide ()->isInsideVolume (), 
       edge_i->backSide ()->isInsideVolume ());
    this->addEdge (newEdge);
  }

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM2D_Body
::translate (const WH_Vector2D& offset)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = false;

  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    vertex_i->translate (offset);
  }

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM2D_Body
::rotate (const WH_Vector2D& center, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = false;

  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    vertex_i->rotate (center, angle);
  }

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_GM2D_Body
::scale (const WH_Vector2D& center, double ratio)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());
  
  WH_CVR_LINE;

  _isConsistent = false;

  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    vertex_i->scale (center, ratio);
  }

  this->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isConsistent ());
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM2D_Vertex* WH_GM2D_Body
::createVertex 
(const WH_Vector2D& point)
{
  WH_GM2D_Vertex* result 
    = new WH_GM2D_Vertex (this, point);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM2D_Edge* WH_GM2D_Body
::createEdge ()
{
  WH_GM2D_Edge* result 
    = new WH_GM2D_Edge (this);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

bool WH_GM2D_Body
::isConsistent () const
{
  return _isConsistent;
}

bool WH_GM2D_Body
::isRegular () const
{
  return _isRegular;
}

const vector<WH_GM2D_Vertex*>& WH_GM2D_Body
::vertex_s () const
{
  return _vertex_s;
}

const vector<WH_GM2D_Edge*>& WH_GM2D_Body
::edge_s () const
{
  return _edge_s;
}

WH_GM2D_Body::BodyType WH_GM2D_Body
::bodyType () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isConsistent ());

  WH_CVR_LINE;

  BodyType result = OTHER;

  int nVertexs = (int)this->vertex_s ().size ();
  int nEdges = (int)this->edge_s ().size ();

  if (nVertexs == 0
      && nEdges == 0) {
    WH_CVR_LINE;
    result = EMPTY;
  } else if (nEdges == 0) {
    WH_CVR_LINE;
    result = DOT_SET;
  } else {
    WH_CVR_LINE;

    int dotVertexCount = 0;
    for (vector<WH_GM2D_Vertex*>::const_iterator 
	   i_vertex = this->vertex_s ().begin ();
	 i_vertex != this->vertex_s ().end ();
	 i_vertex++) {
      WH_GM2D_Vertex* vertex_i = (*i_vertex);
      if (vertex_i->isDot ()) {
	WH_CVR_LINE;
	dotVertexCount++;
      }
    }

    if (0 < dotVertexCount) {
      WH_CVR_LINE;
      result = OTHER;
    } else {
      WH_CVR_LINE;

      int wireEdgeCount = 0;
      int outerBoundaryEdgeCount = 0;
      int innerBoundaryEdgeCount = 0;
      for (vector<WH_GM2D_Edge*>::const_iterator 
	     i_edge = this->edge_s ().begin ();
	   i_edge != this->edge_s ().end ();
	   i_edge++) {
	WH_GM2D_Edge* edge_i = (*i_edge);
	
	switch (edge_i->edgeType ()) {
	case WH_GM2D_Edge::WIRE:
	  WH_CVR_LINE;
	  wireEdgeCount++;
	  break;
	case WH_GM2D_Edge::INNER_BOUNDARY:
	  WH_CVR_LINE;
	  innerBoundaryEdgeCount++;
	  break;
	case WH_GM2D_Edge::OUTER_BOUNDARY:
	  WH_CVR_LINE;
	  outerBoundaryEdgeCount++;
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
      }

      if (0 < wireEdgeCount 
	  && outerBoundaryEdgeCount == 0 
	  && innerBoundaryEdgeCount == 0) {
	WH_CVR_LINE;
	result = WIRE_SET;
      } else if (wireEdgeCount == 0
		 && 0 < outerBoundaryEdgeCount 
		 && innerBoundaryEdgeCount == 0) {
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
    break;
  case DOT_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(nEdges == 0);
    break;
  case WIRE_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    break;
  case VOLUME:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    break;
  case OTHER:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
#endif  

  return result;
}

WH_GM2D_Vertex* WH_GM2D_Body
::findVertex 
(const WH_Vector2D& point) const
{
  WH_CVR_LINE;

  WH_GM2D_Vertex* result = WH_NULL;
  
  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
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

WH_GM2D_Edge* WH_GM2D_Body
::findEdge 
(const WH_Vector2D& point0,
 const WH_Vector2D& point1) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (point0, point1));

  WH_CVR_LINE;

  WH_GM2D_Edge* result = WH_NULL;
  
  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);
    WH_Segment2D seg = edge_i->segment ();
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

void WH_GM2D_Body
::getRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->vertex_s ().size ());
  
  WH_CVR_LINE;
  
  minRange_OUT = WH_Vector2D::hugeValue ();
  maxRange_OUT = -WH_Vector2D::hugeValue ();
  
  for (vector<WH_GM2D_Vertex*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_GM2D_Vertex* vertex_i = (*i_vertex);
    minRange_OUT = WH_min (minRange_OUT, vertex_i->point ());
    maxRange_OUT = WH_max (maxRange_OUT, vertex_i->point ());
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
#endif  
}



/* not covered yet */



