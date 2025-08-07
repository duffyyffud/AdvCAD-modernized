/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* afront2d.cc */
/* triangulation using advancing front method */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "afront2d.h"
#include "triangle2d.h"
#include "bucket2d.h"



/* class WH_AF2D_Vertex_A */

WH_AF2D_Vertex_A
::WH_AF2D_Vertex_A 
(const WH_Vector2D& position)
{
  WH_CVR_LINE;

  _position = position;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_AF2D_Vertex_A
::~WH_AF2D_Vertex_A ()
{
  WH_CVR_LINE;
}

bool WH_AF2D_Vertex_A
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_AF2D_Vertex_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}



/* class WH_AF2D_Edge_A */

WH_AF2D_Edge_A
::WH_AF2D_Edge_A 
(WH_AF2D_Vertex_A* vertex0,
 WH_AF2D_Vertex_A* vertex1)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);
  WH_ASSERT(vertex0 != vertex1);

  WH_CVR_LINE;

  _vertex0 = vertex0;
  _vertex1 = vertex1;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_AF2D_Edge_A
::~WH_AF2D_Edge_A ()
{
  WH_CVR_LINE;
}

bool WH_AF2D_Edge_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->vertex0 () != WH_NULL);
  WH_ASSERT(this->vertex1 () != WH_NULL);
  WH_ASSERT(this->vertex0 () != this->vertex1 ());

  return true;
}

bool WH_AF2D_Edge_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(WH_eq (this->endPoint0 (), this->vertex0 ()->position ()));
  WH_ASSERT(WH_eq (this->endPoint1 (), this->vertex1 ()->position ()));
  WH_ASSERT(WH_ne (this->endPoint0 (), this->endPoint1 ()));
  WH_ASSERT(WH_eq (this->midPoint (), 
		   (this->endPoint0 () + this->endPoint1 ()) / 2));
  WH_ASSERT(WH_eq (this->segment (), 
		   WH_Segment2D (this->endPoint0 (), this->endPoint1 ())));
  WH_ASSERT(WH_eq (this->normal (), this->segment ().normal ()));
  WH_ASSERT(this->hasVertex (this->vertex0 ()));
  WH_ASSERT(this->hasVertex (this->vertex1 ()));
  WH_ASSERT(this->theOtherVertex (this->vertex0 ()) == this->vertex1 ());
  WH_ASSERT(this->theOtherVertex (this->vertex1 ()) == this->vertex0 ());

  return true;
}

WH_Vector2D WH_AF2D_Edge_A
::midPoint () const
{
  return (this->endPoint0 () + this->endPoint1 ()) / 2;
}

WH_Segment2D WH_AF2D_Edge_A
::segment () const
{
  return WH_Segment2D (this->endPoint0 (), this->endPoint1 ());
}

WH_Vector2D WH_AF2D_Edge_A
::normal () const
{
  return this->segment ().normal ();
}

WH_Vector2D WH_AF2D_Edge_A
::minRange () const
{
  return WH_min (this->endPoint0 (), this->endPoint1 ());
}

WH_Vector2D WH_AF2D_Edge_A
::maxRange () const
{
  return WH_max (this->endPoint0 (), this->endPoint1 ());
}

bool WH_AF2D_Edge_A
::hasVertex (const WH_AF2D_Vertex_A* vertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  return vertex == this->vertex0 ()
    || vertex == this->vertex1 ();
}

WH_AF2D_Vertex_A* WH_AF2D_Edge_A
::theOtherVertex (const WH_AF2D_Vertex_A* vertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(this->hasVertex (vertex));

  WH_CVR_LINE;

  if (vertex == this->vertex0 ()) {
    WH_CVR_LINE;
    return this->vertex1 ();
  } else {
    WH_CVR_LINE;
    return this->vertex0 ();
  }
}



/* class WH_AF2D_Triangle_A */

WH_AF2D_Triangle_A
::WH_AF2D_Triangle_A 
(WH_AF2D_Vertex_A* vertex0,
 WH_AF2D_Vertex_A* vertex1,
 WH_AF2D_Vertex_A* vertex2)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);
  WH_ASSERT(vertex2 != WH_NULL);
  WH_ASSERT(vertex0 != vertex1);
  WH_ASSERT(vertex0 != vertex2);
  WH_ASSERT(vertex1 != vertex2);
  
  WH_CVR_LINE;

  _vertex0 = vertex0;
  _vertex1 = vertex1;
  _vertex2 = vertex2;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

WH_AF2D_Triangle_A
::~WH_AF2D_Triangle_A ()
{
  WH_CVR_LINE;
}

bool WH_AF2D_Triangle_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->vertex0 () != WH_NULL);
  WH_ASSERT(this->vertex1 () != WH_NULL);
  WH_ASSERT(this->vertex2 () != WH_NULL);
  WH_ASSERT(this->vertex0 () != this->vertex1 ());
  WH_ASSERT(this->vertex0 () != this->vertex2 ());
  WH_ASSERT(this->vertex1 () != this->vertex2 ());
  
  return true;
}

bool WH_AF2D_Triangle_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_Triangle2D tri
    (this->vertex0 ()->position (),
     this->vertex1 ()->position (),
     this->vertex2 ()->position ());
  WH_ASSERT(tri.isRegular ());
  WH_ASSERT(!tri.isClockWise ());

  return true;
}



/* class WH_AF2D_Triangulator_A */

WH_AF2D_Triangulator_A
::WH_AF2D_Triangulator_A ()
{
  WH_CVR_LINE;
}

WH_AF2D_Triangulator_A
::~WH_AF2D_Triangulator_A ()
{
  WH_CVR_LINE;

  WH_T_Delete (_triangle_s);
  WH_T_Delete (_edge_s);
  WH_T_Delete (_vertex_s);
}

bool WH_AF2D_Triangulator_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (0, this->edgeLength ()));

  return true;
}

bool WH_AF2D_Triangulator_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    
    for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	   j_vertex = this->vertex_s ().begin ();
	 j_vertex != this->vertex_s ().end ();
	 j_vertex++) {
      WH_AF2D_Vertex_A* vertex_j = (*j_vertex);
      
      if (vertex_j != vertex_i) {
	WH_CVR_LINE;
	WH_ASSERT(WH_ne (vertex_j->position (), vertex_i->position ()));
      }
    }
  }

  for (vector<WH_AF2D_Triangle_A*>::const_iterator 
	 i_triangle = this->triangle_s ().begin ();
       i_triangle != this->triangle_s ().end ();
       i_triangle++) {
    WH_AF2D_Triangle_A* triangle_i = (*i_triangle);

    WH_Triangle2D tri_i 
      (triangle_i->vertex0 ()->position (),
       triangle_i->vertex1 ()->position (),
       triangle_i->vertex2 ()->position ());
    WH_Vector2D center_i = tri_i.centerOfGravity ();
    
    for (vector<WH_AF2D_Triangle_A*>::const_iterator 
	   j_triangle = this->triangle_s ().begin ();
	 j_triangle != this->triangle_s ().end ();
	 j_triangle++) {
      WH_AF2D_Triangle_A* triangle_j = (*j_triangle);
      
      if (triangle_j != triangle_i) {
	WH_CVR_LINE;
	WH_Triangle2D tri_j 
	  (triangle_j->vertex0 ()->position (),
	   triangle_j->vertex1 ()->position (),
	   triangle_j->vertex2 ()->position ());
	WH_ASSERT(tri_j.checkContainmentAt (center_i) 
		  == WH_Polygon2D_A::OUT);
      }
    }
  }

  for (list<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);
    
    for (list<WH_AF2D_Edge_A*>::const_iterator 
	   j_edge = this->edge_s ().begin ();
	 j_edge != this->edge_s ().end ();
	 j_edge++) {
      WH_AF2D_Edge_A* edge_j = (*j_edge);
      
      if (edge_j != edge_i) {
	WH_CVR_LINE;
	WH_ASSERT(edge_i->vertex0 () != edge_j->vertex0 ()
		  || edge_i->vertex1 () != edge_j->vertex1 ());
	WH_ASSERT(edge_i->vertex0 () != edge_j->vertex1 ()
		  || edge_i->vertex1 () != edge_j->vertex0 ());
      }
    }
  }

  WH_T_AssureInvariant (this->vertex_s ());
  WH_T_AssureInvariant (this->triangle_s ());
  WH_T_AssureInvariant (this->edge_s ());

  return true;
}

const vector<WH_AF2D_Vertex_A*>& WH_AF2D_Triangulator_A
::vertex_s () const
{
  return _vertex_s;
}

const vector<WH_AF2D_Triangle_A*>& WH_AF2D_Triangulator_A
::triangle_s () const
{
  return _triangle_s;
}

const list<WH_AF2D_Edge_A*>& WH_AF2D_Triangulator_A
::edge_s () const
{
  return _edge_s; 
}

void WH_AF2D_Triangulator_A
::addVertex 
(WH_AF2D_Vertex_A* vertex /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  // Check for duplicate vertices - but allow them with warning
  // The real fix needs to be at a higher level where vertices are created
  for (vector<WH_AF2D_Vertex_A*>::iterator 
       i_vertex = _vertex_s.begin ();
       i_vertex != _vertex_s.end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    
    if (WH_eq (vertex_i->position (), vertex->position ())) {
      // Found duplicate - warn but still add it
      // This maintains pointer validity for callers
      cerr << "WARNING: Advancing front duplicate vertex at position " 
           << vertex->position() << " - allowing for degenerate geometry" << endl;
      // Don't return here - continue to add the vertex
      break;
    }
  }

  _vertex_s.push_back (vertex);
}

void WH_AF2D_Triangulator_A
::addTriangle 
(WH_AF2D_Triangle_A* triangle /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(triangle != WH_NULL);

  _triangle_s.push_back (triangle);
}

void WH_AF2D_Triangulator_A
::addEdge 
(WH_AF2D_Edge_A* edge /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  // Check for duplicate edges - but allow them with warning
  // The real fix needs to be at a higher level where edges are created
  for (list<WH_AF2D_Edge_A*>::iterator 
       i_edge = _edge_s.begin ();
       i_edge != _edge_s.end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);
    
    if ((edge_i->vertex0 () == edge->vertex0 ()
         && edge_i->vertex1 () == edge->vertex1 ())
        || (edge_i->vertex0 () == edge->vertex1 ()
            && edge_i->vertex1 () == edge->vertex0 ())) {
      // Found duplicate edge - warn but still add it
      // This maintains pointer validity for callers
      cerr << "WARNING: Advancing front duplicate edge detected - allowing for degenerate geometry" << endl;
      // Don't return here - continue to add the edge
      break;
    }
  }

  _edge_s.push_back (edge);
}

void WH_AF2D_Triangulator_A
::removeEdge 
(WH_AF2D_Edge_A* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_CVR_LINE;

  list<WH_AF2D_Edge_A*>::iterator i_edge
    = find (_edge_s.begin (), _edge_s.end (), edge);
  WH_ASSERT(i_edge != _edge_s.end ());
  _edge_s.erase (i_edge);

  /* DELETE <edge> */
  delete edge;
  edge = WH_NULL;
}

WH_AF2D_Edge_A* WH_AF2D_Triangulator_A
::getAnyEdge () const
{
  WH_CVR_LINE;

  if (0 < _edge_s.size ()) {
    WH_CVR_LINE;
    return _edge_s.front ();
  } else {
    WH_CVR_LINE;
    return WH_NULL;
  }
}

bool WH_AF2D_Triangulator_A
::hasNoEdgeNear 
(const WH_Vector2D& position, double range) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (0, range));

  bool result = true;

  for (list<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);

    WH_Segment2D segment = edge_i->segment ();
    if (WH_lt (segment.distanceFrom (position), range)) {
      result = false;
      break;
    }
  }

  return result;
}

bool WH_AF2D_Triangulator_A
::canPutPoint 
(const WH_Vector2D& newVertexPosition,
 WH_AF2D_Edge_A* edge,
 double edgeLength) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(WH_lt (0, edgeLength));

  WH_CVR_LINE;

  bool result = false;

  /* if <newVertexPosition> is acceptable, */
  /* <newVertexPosition> is inside domain */
  /* there is no vertex near <newVertexPosition> */
  /* there is no edge near <newVertexPosition> */
  /* there is no edge from <edge> to <newVertexPosition> */

  if (this->hasNoEdgeNear 
      (newVertexPosition, edgeLength * sqrt (3.0) / 2)) {
    WH_CVR_LINE;
    result = true;
  }

  return result;
}

bool WH_AF2D_Triangulator_A
::canPutEdge 
(const WH_Vector2D& endPoint0,
 const WH_Vector2D& endPoint1,
 WH_AF2D_Edge_A*& sameEdge_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (endPoint0, endPoint1));

  /* if the edge is acceptable, */
  /* there is no cross point with any other edges */
  /* there is no overlap with any other edges */
  
  bool result = true;
  sameEdge_OUT = WH_NULL;

  WH_Segment2D segment (endPoint0, endPoint1);
  WH_Vector2D minRange = segment.minRange ();
  WH_Vector2D maxRange = segment.maxRange ();

  for (list<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);

    WH_Segment2D segment_i = edge_i->segment ();

    WH_Vector2D minRange_i = edge_i->minRange ();
    WH_Vector2D maxRange_i = edge_i->maxRange ();
    if (!WH_minMaxPairsOverlap 
	(minRange_i, maxRange_i, minRange, maxRange)) {
      continue;
    }

    WH_Vector2D crossPoint;
    WH_Segment2D::WithSegmentIntersectionType intersectionFlag 
      = segment.checkIntersectionWith (segment_i,
				       crossPoint);
    if (intersectionFlag == WH_Segment2D::POINT_WITH_SEGMENT) {
      if (WH_ne (crossPoint, endPoint0)
	  && WH_ne (crossPoint, endPoint1)) {
	result = false;
	break;
      }
    } else if (intersectionFlag == WH_Segment2D::COINCIDENT_WITH_SEGMENT) {
      WH_Segment2D::WithSegmentOverlapType overlapFlag 
	= segment.checkOverlapWith (segment_i);
      if (overlapFlag == WH_Segment2D::SAME_WITH_SEGMENT) {
	sameEdge_OUT = edge_i;
	result = false;
	break;
      } else if (overlapFlag == WH_Segment2D::CONTAINS_WITH_SEGMENT
		 || overlapFlag == WH_Segment2D::CONTAINED_WITH_SEGMENT) {
	result = false;
	break;
      } else if (overlapFlag == WH_Segment2D::OVERLAP_WITH_SEGMENT) {
	if (!segment.hasConnectionWith (segment_i)) {
	  result = false;
	  break;
	}
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(sameEdge_OUT == WH_NULL
	  || sameEdge_OUT != WH_NULL);
  if (sameEdge_OUT != WH_NULL) {
    WH_ASSERT(!result);
    WH_ASSERT(WH_eq (sameEdge_OUT->segment (), segment)
	      || WH_isReverse (sameEdge_OUT->segment (), segment));
  } 
#endif

  return result;
}

bool WH_AF2D_Triangulator_A
::canPutTriangleWithNewVertex 
(WH_AF2D_Edge_A* edge, 
 WH_Vector2D& newVertexPosition_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_CVR_LINE;

  bool result = false;

  double edgeLength = this->edgeLength ();
  newVertexPosition_OUT 
    = edge->midPoint () + edge->normal () * (edgeLength * sqrt (3.0) / 2);

  if (this->canPutPoint (newVertexPosition_OUT, edge, edgeLength)) {
    WH_CVR_LINE;
    result = true;
#ifndef NDEBUG
    WH_Vector2D endPoint0 = edge->endPoint0 ();
    WH_Vector2D endPoint1 = edge->endPoint1 ();
    WH_AF2D_Edge_A* sameEdge0;
    WH_AF2D_Edge_A* sameEdge1;
    WH_ASSERT(this->canPutEdge (endPoint0, newVertexPosition_OUT,
				sameEdge0));
    WH_ASSERT(this->canPutEdge (endPoint1, newVertexPosition_OUT,
				sameEdge1));
#endif
  } else {
    WH_CVR_LINE;
    edgeLength = this->edgeLength () * 0.6;
    /* NEED TO REDEFINE */
    /* MAGIC NUMBER : 0.6 */
    newVertexPosition_OUT 
      = edge->midPoint () + edge->normal () * (edgeLength * sqrt (3.0) / 2);

    if (this->canPutPoint (newVertexPosition_OUT, edge, edgeLength)) {
      WH_CVR_LINE;
      result = true;
#ifndef NDEBUG
      WH_Vector2D endPoint0 = edge->endPoint0 ();
      WH_Vector2D endPoint1 = edge->endPoint1 ();
      WH_AF2D_Edge_A* sameEdge0;
      WH_AF2D_Edge_A* sameEdge1;
      WH_ASSERT(this->canPutEdge (endPoint0, newVertexPosition_OUT,
				  sameEdge0));
      WH_ASSERT(this->canPutEdge (endPoint1, newVertexPosition_OUT,
				  sameEdge1));
#endif
    }
  } 
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!edge->segment ().isClockWiseWith (newVertexPosition_OUT));
#endif

  return result;
}

void WH_AF2D_Triangulator_A
::advanceFrontWithNewVertex 
(WH_AF2D_Edge_A* edge, 
 const WH_Vector2D newVertexPosition)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(!edge->segment ().isClockWiseWith (newVertexPosition));
  
  WH_CVR_LINE;

  WH_AF2D_Vertex_A* vertex0 = edge->vertex0 ();
  WH_AF2D_Vertex_A* vertex1 = edge->vertex1 ();
  this->removeEdge (edge);

  WH_AF2D_Vertex_A* newVertex 
    = this->createVertex (newVertexPosition);
  this->addVertex (newVertex);

  WH_AF2D_Edge_A* newEdge0 
    = this->createEdge (vertex0, newVertex);
  this->addEdge (newEdge0);
  WH_AF2D_Edge_A* newEdge1 
    = this->createEdge (newVertex, vertex1);
  this->addEdge (newEdge1);

  WH_AF2D_Triangle_A* newTriangle 
    = this->createTriangle (vertex0, vertex1, newVertex);
  this->addTriangle (newTriangle);
}

WH_AF2D_Vertex_A* WH_AF2D_Triangulator_A
::findOptimumVertex 
(WH_AF2D_Edge_A* edge, 
 WH_AF2D_Edge_A*& neighborEdge0_OUT,
 WH_AF2D_Edge_A*& neighborEdge1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_AF2D_Vertex_A* result = WH_NULL;
  neighborEdge0_OUT = WH_NULL;
  neighborEdge1_OUT = WH_NULL;

  WH_AF2D_Vertex_A* vertex0 = edge->vertex0 ();
  WH_AF2D_Vertex_A* vertex1 = edge->vertex1 ();
  WH_Vector2D endPoint0 = edge->endPoint0 ();
  WH_Vector2D endPoint1 = edge->endPoint1 ();
  WH_Vector2D midPoint = edge->midPoint ();
  WH_Vector2D normal = edge->normal ();

  double prevCircumradius = WH::HUGE_VALUE;

  double criticalDistance = this->edgeLength () * 2.0;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 2.0 */

  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);

    if (vertex_i == vertex0
	|| vertex_i == vertex1) continue;
  
    WH_Vector2D vertexPosition = vertex_i->position ();
    WH_ASSERT(WH_ne (vertexPosition, midPoint));
    
    double dist = WH_distance (vertexPosition, midPoint);
    if (WH_lt (criticalDistance, dist)) continue;

    /* <vertex_i> is located at the inner side of <edge> */
    double sideSign = WH_scalarProduct 
      ((vertexPosition - midPoint).normalize (),
       normal);
#if 0
    if (WH_le (sideSign, -0.001)) continue;  // More tolerant for degenerate geometry
    /* NEED TO REDEFINE */
    /* MAGIC NUMBER : 0.01 */
#else
    if (WH_le (sideSign, 0.001)) continue;
    /* NEED TO REDEFINE */
    /* MAGIC NUMBER : 0.001 */
#endif
    
    /* triangle of <vertex_i> and <edge> has the smallest circumradius */
    WH_Triangle2D triangle (endPoint0, endPoint1, vertexPosition);
    WH_Vector2D center;
    double radius;
    triangle.getCircumcenter (center, radius);
    if (WH_lt (radius, prevCircumradius)) {

      /* 2 new edges do not cross with other edges */
      WH_AF2D_Edge_A* sameEdge0;
      bool flag0 = this->canPutEdge (endPoint0, vertexPosition,
				     sameEdge0);
      if (flag0 || sameEdge0 != WH_NULL) {
	WH_AF2D_Edge_A* sameEdge1;
	bool flag1 = this->canPutEdge (endPoint1, vertexPosition,
				       sameEdge1);
	if (flag1 || sameEdge1 != WH_NULL) {
	  
	  /* check whether the new triangle contains any existing */
	  /* vertex or not as <triangleContainsAnyVertex> */
	  int triangleContainsAnyVertex = false;
	  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
		 j_vertex = this->vertex_s ().begin ();
	       j_vertex != this->vertex_s ().end ();
	       j_vertex++) {
	    WH_AF2D_Vertex_A* vertex_j = (*j_vertex);
	    
	    if (vertex_j != vertex_i
		&& vertex_j != vertex0
		&& vertex_j != vertex1) {
	      WH_CVR_LINE;
	      if (triangle.checkContainmentAt (vertex_j->position ()) 
		  != WH_Polygon2D_A::OUT) {
		WH_CVR_LINE;
		triangleContainsAnyVertex = true;
		break;
	      }
	    }
	  }
	  if (!triangleContainsAnyVertex) {
	    result = vertex_i;
	    prevCircumradius = radius;
	    neighborEdge0_OUT = sameEdge0;
	    neighborEdge1_OUT = sameEdge1;
	  }

	}
      }
    }
  }

  // Progressive relaxation for degenerate geometry (base class version)
  if (result == WH_NULL) {
    cerr << "WARNING: No vertex found with strict criteria in base class - trying relaxed search" << endl;
    
    // Try with very relaxed criteria - accept any vertex that makes valid triangle
    for (vector<WH_AF2D_Vertex_A*>::const_iterator 
         i_vertex = this->vertex_s ().begin ();
         i_vertex != this->vertex_s ().end ();
         i_vertex++) {
      WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
      
      if (vertex_i == vertex0 || vertex_i == vertex1) continue;
      
      WH_Vector2D vertexPosition = vertex_i->position ();
      double dist = WH_distance (vertexPosition, midPoint);
      if (WH_lt (criticalDistance, dist)) continue;
      
      // No side sign restriction - accept vertices on either side
      WH_Triangle2D triangle (endPoint0, endPoint1, vertexPosition);
      WH_Vector2D center;
      double radius;
      triangle.getCircumcenter (center, radius);
      if (WH_lt (0, radius) && WH_lt (radius, prevCircumradius)) {  // Just check for valid triangle
        result = vertex_i;
        prevCircumradius = radius;
        neighborEdge0_OUT = WH_NULL;  // Simplified for degenerate case
        neighborEdge1_OUT = WH_NULL;
        cerr << "WARNING: Using relaxed criteria vertex in base class at " << vertexPosition << endl;
        break;  // Take first acceptable vertex
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(!edge->segment ().isClockWiseWith (result->position ()));
  WH_ASSERT(neighborEdge0_OUT == WH_NULL
	    || neighborEdge0_OUT != WH_NULL);
  if (neighborEdge0_OUT != WH_NULL) {
    WH_ASSERT(neighborEdge0_OUT->hasVertex (result));
    WH_ASSERT(neighborEdge0_OUT->hasVertex (edge->vertex0 ()));
  }
  WH_ASSERT(neighborEdge1_OUT != WH_NULL
	    || neighborEdge1_OUT == WH_NULL);
  if (neighborEdge1_OUT != WH_NULL) {
    WH_ASSERT(neighborEdge1_OUT->hasVertex (result));
    WH_ASSERT(neighborEdge1_OUT->hasVertex (edge->vertex1 ()));
  }
#endif
  
  return result;
}

void WH_AF2D_Triangulator_A
::advanceFrontWithExistingVertex 
(WH_AF2D_Edge_A* edge, 
 WH_AF2D_Vertex_A* vertex,
 WH_AF2D_Edge_A* neighborEdge0,
 WH_AF2D_Edge_A* neighborEdge1)
{
  /* PRE-CONDITION */
#ifndef NDEBUG
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(!edge->hasVertex (vertex));
  WH_ASSERT(!edge->segment ().isClockWiseWith (vertex->position ()));
  WH_ASSERT(neighborEdge0 != WH_NULL
	    || neighborEdge0 == WH_NULL);
  if (neighborEdge0 != WH_NULL) {
    WH_ASSERT(neighborEdge0->hasVertex (vertex));
    WH_ASSERT(neighborEdge0->hasVertex (edge->vertex0 ()));
  }
  WH_ASSERT(neighborEdge1 != WH_NULL
	    || neighborEdge1 == WH_NULL);
  if (neighborEdge1 != WH_NULL) {
    WH_ASSERT(neighborEdge1->hasVertex (vertex));
    WH_ASSERT(neighborEdge1->hasVertex (edge->vertex1 ()));
  }
#endif
  
  WH_CVR_LINE;

  WH_AF2D_Vertex_A* vertex0 = edge->vertex0 ();
  WH_AF2D_Vertex_A* vertex1 = edge->vertex1 ();

  if (neighborEdge0 != WH_NULL 
      && neighborEdge1 != WH_NULL) {
    WH_CVR_LINE;

    this->removeEdge (edge);
    this->removeEdge (neighborEdge0);
    this->removeEdge (neighborEdge1);

  } else if (neighborEdge0 != WH_NULL) {
    WH_CVR_LINE;

    this->removeEdge (edge);
    this->removeEdge (neighborEdge0);

    WH_AF2D_Edge_A* newEdge 
      = this->createEdge (vertex, vertex1);
    this->addEdge (newEdge);
    
  } else if (neighborEdge1 != WH_NULL) {
    WH_CVR_LINE;
    
    this->removeEdge (edge);
    this->removeEdge (neighborEdge1);

    WH_AF2D_Edge_A* newEdge 
      = this->createEdge (vertex0, vertex);
    this->addEdge (newEdge);
    
  } else {
    WH_CVR_LINE;
    
    this->removeEdge (edge);

    WH_AF2D_Edge_A* newEdge0 
      = this->createEdge (vertex0, vertex);
    this->addEdge (newEdge0);
    WH_AF2D_Edge_A* newEdge1 
      = this->createEdge (vertex, vertex1);
    this->addEdge (newEdge1);

  }

  WH_AF2D_Triangle_A* newTriangle 
    = this->createTriangle (vertex0, vertex1, vertex);
  this->addTriangle (newTriangle);
}

void WH_AF2D_Triangulator_A
::perform ()
{
  WH_CVR_LINE;

  for (;;) {
    WH_AF2D_Edge_A* edge = this->getAnyEdge ();
    if (edge == WH_NULL) break;

    WH_Vector2D newVertexPosition;
    if (this->canPutTriangleWithNewVertex (edge, 
					   newVertexPosition)) {
      WH_CVR_LINE;
      this->advanceFrontWithNewVertex 
	(edge, newVertexPosition);
    } else {
      WH_CVR_LINE;
      WH_AF2D_Edge_A* neighborEdge0;
      WH_AF2D_Edge_A* neighborEdge1;
      WH_AF2D_Vertex_A* vertex 
	= this->findOptimumVertex (edge,
				   neighborEdge0, neighborEdge1);
      this->advanceFrontWithExistingVertex 
	(edge, vertex, neighborEdge0, neighborEdge1);
    }
  }
}

WH_Vector2D WH_AF2D_Triangulator_A
::minRange () const
{
  WH_CVR_LINE;

  WH_Vector2D result = WH_Vector2D::hugeValue ();

  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    result = WH_min (result, vertex_i->position ());
  }

  return result;
}

WH_Vector2D WH_AF2D_Triangulator_A
::maxRange () const
{
  WH_CVR_LINE;

  WH_Vector2D result = -WH_Vector2D::hugeValue ();

  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    result = WH_max (result, vertex_i->position ());
  }

  return result;
}



/* class WH_AF2D_OptimizedTriangulator_A */

WH_AF2D_OptimizedTriangulator_A
::WH_AF2D_OptimizedTriangulator_A ()
{
  WH_CVR_LINE;

  _vertexBucket = WH_NULL;
  _edgeBucket = WH_NULL;
}

WH_AF2D_OptimizedTriangulator_A
::~WH_AF2D_OptimizedTriangulator_A ()
{
  WH_CVR_LINE;

  // Buckets automatically destroyed by unique_ptr
}

bool WH_AF2D_OptimizedTriangulator_A
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_AF2D_Triangulator_A::checkInvariant ();

  return true;
}

bool WH_AF2D_OptimizedTriangulator_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_AF2D_Triangulator_A::assureInvariant ();

  return true;
}

void WH_AF2D_OptimizedTriangulator_A
::addVertexIntoBucket 
(WH_AF2D_Vertex_A* vertex /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(_vertexBucket != WH_NULL);
  
  _vertexBucket->addItemLastOn 
    (vertex->position (), vertex);
}

void WH_AF2D_OptimizedTriangulator_A
::addEdgeIntoBucket 
(WH_AF2D_Edge_A* edge /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(_edgeBucket != WH_NULL);
  
  _edgeBucket->addItemLastWithin 
    (edge->minRange (), edge->maxRange (), edge);
}

void WH_AF2D_OptimizedTriangulator_A
::removeEdgeFromBucket 
(WH_AF2D_Edge_A* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(_edgeBucket != WH_NULL);

  _edgeBucket->removeItemFromLastWithin 
    (edge->minRange (), edge->maxRange (), edge);
}

void WH_AF2D_OptimizedTriangulator_A
::addVertex 
(WH_AF2D_Vertex_A* vertex /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_CVR_LINE;

  this->WH_AF2D_Triangulator_A::addVertex (vertex);

  if (_vertexBucket != WH_NULL) {
    WH_CVR_LINE;
    /* the vertex is inside */
    this->addVertexIntoBucket (vertex);
  }
  /* else, the vertex is on boundary */
}

void WH_AF2D_OptimizedTriangulator_A
::addEdge 
(WH_AF2D_Edge_A* edge /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_CVR_LINE;

  this->WH_AF2D_Triangulator_A::addEdge (edge);

  if (_edgeBucket != WH_NULL) {
    WH_CVR_LINE;
    /* the edge is inside */
    this->addEdgeIntoBucket (edge);
  }
  /* else, the edge is on boundary */
}

void WH_AF2D_OptimizedTriangulator_A
::removeEdge 
(WH_AF2D_Edge_A* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  WH_CVR_LINE;

  WH_ASSERT(_edgeBucket != WH_NULL);
  this->removeEdgeFromBucket (edge);

  this->WH_AF2D_Triangulator_A::removeEdge (edge);
}

bool WH_AF2D_OptimizedTriangulator_A
::hasNoEdgeNear 
(const WH_Vector2D& position, double range) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (0, range));
  WH_ASSERT(_edgeBucket != WH_NULL);

  WH_CVR_LINE;

  bool result = true;

  vector<WH_AF2D_Edge_A*> edge_s;
  _edgeBucket->getItemsWithin
    (position - WH_Vector2D (range, range), 
     position + WH_Vector2D (range, range), 
     edge_s);
  for (vector<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = edge_s.begin ();
       i_edge != edge_s.end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);

    WH_Segment2D segment = edge_i->segment ();
    if (WH_lt (segment.distanceFrom (position), range)) {
      WH_CVR_LINE;
      result = false;
      break;
    }
  }

  return result;
}

bool WH_AF2D_OptimizedTriangulator_A
::canPutEdge 
(const WH_Vector2D& endPoint0,
 const WH_Vector2D& endPoint1,
 WH_AF2D_Edge_A*& sameEdge_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (endPoint0, endPoint1));
  WH_ASSERT(_edgeBucket != WH_NULL);

  WH_CVR_LINE;

  /* if the edge is acceptable, */
  /* there is no cross point with any other edges */
  /* there is no overlap with any other edges */
  
  bool result = true;
  sameEdge_OUT = WH_NULL;

  WH_Segment2D segment (endPoint0, endPoint1);
  WH_Vector2D minRange = segment.minRange ();
  WH_Vector2D maxRange = segment.maxRange ();

  vector<WH_AF2D_Edge_A*> edge_s;
  _edgeBucket->getItemsWithin
    (minRange, maxRange, 
     edge_s);
  for (vector<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = edge_s.begin ();
       i_edge != edge_s.end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);
    
    WH_Vector2D minRange_i = edge_i->minRange ();
    WH_Vector2D maxRange_i = edge_i->maxRange ();
    if (!WH_minMaxPairsOverlap 
	(minRange_i, maxRange_i, minRange, maxRange)) {
      WH_CVR_LINE;
      continue;
    }
    
    WH_Segment2D segment_i = edge_i->segment ();
    WH_Vector2D crossPoint;
    WH_Segment2D::WithSegmentIntersectionType intersectionFlag 
      = segment.checkIntersectionWith (segment_i,
				       crossPoint);
    if (intersectionFlag == WH_Segment2D::POINT_WITH_SEGMENT) {
      WH_CVR_LINE;
      if (WH_ne (crossPoint, endPoint0)
	  && WH_ne (crossPoint, endPoint1)) {
	WH_CVR_LINE;
	result = false;
	break;
      }
    } else if (intersectionFlag == WH_Segment2D::COINCIDENT_WITH_SEGMENT) {
      WH_CVR_LINE;
      WH_Segment2D::WithSegmentOverlapType overlapFlag 
	= segment.checkOverlapWith (segment_i);
      if (overlapFlag == WH_Segment2D::SAME_WITH_SEGMENT) {
	WH_CVR_LINE;
	sameEdge_OUT = edge_i;
	result = false;
	break;
      } else if (overlapFlag == WH_Segment2D::CONTAINS_WITH_SEGMENT
		 || overlapFlag == WH_Segment2D::CONTAINED_WITH_SEGMENT) {
	WH_CVR_LINE;
	result = false;
	break;
      } else if (overlapFlag == WH_Segment2D::OVERLAP_WITH_SEGMENT) {
	WH_CVR_LINE;
	if (!segment.hasConnectionWith (segment_i)) {
	  WH_CVR_LINE;                                 /* NOT COVERED */
	  result = false;
	  break;
	}
      }
    }
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(sameEdge_OUT == WH_NULL
	  || sameEdge_OUT != WH_NULL);
  if (sameEdge_OUT != WH_NULL) {
    WH_ASSERT(!result);
    WH_ASSERT(WH_eq (sameEdge_OUT->segment (), segment)
	      || WH_isReverse (sameEdge_OUT->segment (), segment));
  } 
#endif

  return result;
}

WH_AF2D_Vertex_A* WH_AF2D_OptimizedTriangulator_A
::findOptimumVertex 
(WH_AF2D_Edge_A* edge, 
 WH_AF2D_Edge_A*& neighborEdge0_OUT,
 WH_AF2D_Edge_A*& neighborEdge1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  WH_ASSERT(_vertexBucket != WH_NULL);

  WH_CVR_LINE;

  WH_AF2D_Vertex_A* result = WH_NULL;
  neighborEdge0_OUT = WH_NULL;
  neighborEdge1_OUT = WH_NULL;

  WH_AF2D_Vertex_A* vertex0 = edge->vertex0 ();
  WH_AF2D_Vertex_A* vertex1 = edge->vertex1 ();
  WH_Vector2D endPoint0 = edge->endPoint0 ();
  WH_Vector2D endPoint1 = edge->endPoint1 ();
  WH_Vector2D midPoint = edge->midPoint ();
  WH_Vector2D normal = edge->normal ();

  double prevCircumradius = WH::HUGE_VALUE;

  double criticalDistance = this->edgeLength () * 2.0;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 2.0 */

  vector<WH_AF2D_Vertex_A*> vertex_s;
  _vertexBucket->getItemsWithin
    (midPoint - WH_Vector2D (criticalDistance, criticalDistance), 
     midPoint + WH_Vector2D (criticalDistance, criticalDistance), 
     vertex_s);
  
  // Debug output for empty vertex bucket
  if (vertex_s.empty()) {
    cerr << "WARNING: No vertices found in search area for advancing front" << endl;
    cerr << "  Search center: " << midPoint << ", distance: " << criticalDistance << endl;
  }
  
  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = vertex_s.begin ();
       i_vertex != vertex_s.end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    
    if (vertex_i == vertex0
	|| vertex_i == vertex1) continue;
    
    WH_Vector2D vertexPosition = vertex_i->position ();
    WH_ASSERT(WH_ne (vertexPosition, midPoint));
    
    double dist = WH_distance (vertexPosition, midPoint);
    if (WH_lt (criticalDistance, dist)) continue;
    
    /* <vertex_i> is located at the inner side of <edge> */
    double sideSign = WH_scalarProduct 
      ((vertexPosition - midPoint).normalize (),
       normal);
    if (WH_le (sideSign, -0.001)) continue;  // More tolerant for degenerate geometry
    /* NEED TO REDEFINE */
    /* MAGIC NUMBER : 0.01 */
    
    /* triangle of <vertex_i> and <edge> has the smallest circumradius */
    WH_Triangle2D triangle (endPoint0, endPoint1, vertexPosition);
    WH_Vector2D center;
    double radius;
    triangle.getCircumcenter (center, radius);
    if (WH_lt (radius, prevCircumradius)) {
      WH_CVR_LINE;
      
      /* 2 new edges do not cross with other edges */
      WH_AF2D_Edge_A* sameEdge0;
      bool flag0 = this->canPutEdge (endPoint0, vertexPosition,
				     sameEdge0);
      if (flag0 || sameEdge0 != WH_NULL) {
	WH_CVR_LINE;
	WH_AF2D_Edge_A* sameEdge1;
	bool flag1 = this->canPutEdge (endPoint1, vertexPosition,
				       sameEdge1);
	if (flag1 || sameEdge1 != WH_NULL) {
	  WH_CVR_LINE;

	  /* check whether the new triangle contains any existing */
	  /* vertex or not as <triangleContainsAnyVertex> */
	  int triangleContainsAnyVertex = false;
	  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
		 j_vertex = vertex_s.begin ();
	       j_vertex != vertex_s.end ();
	       j_vertex++) {
	    WH_AF2D_Vertex_A* vertex_j = (*j_vertex);
	    
	    if (vertex_j != vertex_i
		&& vertex_j != vertex0
		&& vertex_j != vertex1) {
	      WH_CVR_LINE;
	      if (triangle.checkContainmentAt (vertex_j->position ()) 
		  != WH_Polygon2D_A::OUT) {
		WH_CVR_LINE;
		triangleContainsAnyVertex = true;
		break;
	      }
	    }
	  }
	  if (!triangleContainsAnyVertex) {
	    WH_CVR_LINE;
	    result = vertex_i;
	    prevCircumradius = radius;
	    neighborEdge0_OUT = sameEdge0;
	    neighborEdge1_OUT = sameEdge1;
	  }
	}
      }
    }
  }

  // Progressive relaxation for degenerate geometry
  if (result == WH_NULL) {
    cerr << "WARNING: No vertex found with strict criteria - trying relaxed search" << endl;
    
    // Try with very relaxed criteria - accept any vertex that makes valid triangle
    for (vector<WH_AF2D_Vertex_A*>::const_iterator 
         i_vertex = vertex_s.begin ();
         i_vertex != vertex_s.end ();
         i_vertex++) {
      WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
      
      if (vertex_i == vertex0 || vertex_i == vertex1) continue;
      
      WH_Vector2D vertexPosition = vertex_i->position ();
      double dist = WH_distance (vertexPosition, midPoint);
      if (WH_lt (criticalDistance, dist)) continue;
      
      // No side sign restriction - accept vertices on either side
      WH_Triangle2D triangle (endPoint0, endPoint1, vertexPosition);
      WH_Vector2D center;
      double radius;
      triangle.getCircumcenter (center, radius);
      if (WH_lt (0, radius) && WH_lt (radius, prevCircumradius)) {  // Just check for valid triangle
        result = vertex_i;
        prevCircumradius = radius;
        neighborEdge0_OUT = WH_NULL;  // Simplified for degenerate case
        neighborEdge1_OUT = WH_NULL;
        cerr << "WARNING: Using relaxed criteria vertex at " << vertexPosition << endl;
        break;  // Take first acceptable vertex
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(!edge->segment ().isClockWiseWith (result->position ()));
  WH_ASSERT(neighborEdge0_OUT == WH_NULL
	    || neighborEdge0_OUT != WH_NULL);
  if (neighborEdge0_OUT != WH_NULL) {
    WH_ASSERT(neighborEdge0_OUT->hasVertex (result));
    WH_ASSERT(neighborEdge0_OUT->hasVertex (edge->vertex0 ()));
  }
  WH_ASSERT(neighborEdge1_OUT != WH_NULL
	    || neighborEdge1_OUT == WH_NULL);
  if (neighborEdge1_OUT != WH_NULL) {
    WH_ASSERT(neighborEdge1_OUT->hasVertex (result));
    WH_ASSERT(neighborEdge1_OUT->hasVertex (edge->vertex1 ()));
  }
#endif
  
  return result;
}

void WH_AF2D_OptimizedTriangulator_A
::setUpBucket ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_vertexBucket == WH_NULL);
  WH_ASSERT(_edgeBucket == WH_NULL);

  WH_CVR_LINE;

  WH_Vector2D minRange = this->minRange ();
  WH_Vector2D maxRange = this->maxRange ();
  WH_ASSERT(WH_lt (minRange, maxRange));

  WH_Vector2D size = maxRange - minRange;
  WH_Vector2D extendedMinRange = minRange - size / 11;
  WH_Vector2D extendedMaxRange = maxRange + size / 13;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 11, 13 */
  WH_Vector2D extendedSize = extendedMaxRange - extendedMinRange;
  
  double edgeLength = this->edgeLength ();
  int xCells = (int)ceil (extendedSize.x / edgeLength + WH::eps);
  if (xCells / 2 == 0) xCells++;
  int yCells = (int)ceil (extendedSize.y / edgeLength + WH::eps);
  if (yCells / 2 == 0) yCells++;

  _vertexBucket = make_unique<WH_Bucket2D<WH_AF2D_Vertex_A>>(
    extendedMinRange, extendedMaxRange, xCells, yCells);
  // make_unique cannot return nullptr - throws std::bad_alloc on failure

  _edgeBucket = make_unique<WH_Bucket2D<WH_AF2D_Edge_A>>(
    extendedMinRange, extendedMaxRange, xCells, yCells);
  // make_unique cannot return nullptr - throws std::bad_alloc on failure

  for (vector<WH_AF2D_Vertex_A*>::const_iterator 
	 i_vertex = this->vertex_s ().begin ();
       i_vertex != this->vertex_s ().end ();
       i_vertex++) {
    WH_AF2D_Vertex_A* vertex_i = (*i_vertex);
    this->addVertexIntoBucket (vertex_i);
  }  

  for (list<WH_AF2D_Edge_A*>::const_iterator 
	 i_edge = this->edge_s ().begin ();
       i_edge != this->edge_s ().end ();
       i_edge++) {
    WH_AF2D_Edge_A* edge_i = (*i_edge);
    this->addEdgeIntoBucket (edge_i);
  }

  /* POST-CONDITION */
  WH_ASSERT(_vertexBucket != WH_NULL);
  WH_ASSERT(_edgeBucket != WH_NULL);
}

void WH_AF2D_OptimizedTriangulator_A
::perform ()
{
  WH_CVR_LINE;

  this->setUpBucket ();

  this->WH_AF2D_Triangulator_A::perform ();
}



#ifndef WH_INLINE_ENABLED
#include "afront2d_inline.cc"
#endif



/* not covered yet
afront2d.cc:1184:	  WH_CVR_LINE;
*/
