/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d_facet.cc : facet module of 2-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d_facet.h"
#include "connector2d.h"



/* class WH_GM2D_SegmentFacet */

void WH_GM2D_SegmentFacet
::divideFacetsBySegments 
(const vector<WH_Segment2D>& segmentBy_s,
 vector<WH_GM2D_SegmentFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Segment2D>::const_iterator 
	 i_seg = segmentBy_s.begin ();
       i_seg != segmentBy_s.end ();
       i_seg++) {
    WH_Segment2D segmentBy_i = (*i_seg);
    
    vector<WH_GM2D_SegmentFacet*> remainingFacet_s;
    vector<WH_GM2D_SegmentFacet*> deletedFacet_s;
    vector<WH_GM2D_SegmentFacet*> newFacet_s;
    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM2D_SegmentFacet* facet_i = (*i_facet);

      vector<WH_GM2D_SegmentFacet*> facet_s;
      if (facet_i->createDividedFacetsBySegment (segmentBy_i,
						 facet_s)) {
	WH_CVR_LINE;
	deletedFacet_s.push_back (facet_i);
	WH_T_Add (facet_s, newFacet_s);
      } else {
	WH_CVR_LINE;
	remainingFacet_s.push_back (facet_i);
      }
    }
    WH_ASSERT(deletedFacet_s.size () + remainingFacet_s.size ()
	      == facet_s_IO.size ());
    
    if (0 < newFacet_s.size ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < deletedFacet_s.size ());
      WH_ASSERT(2 <= newFacet_s.size ());
      WH_T_Delete (deletedFacet_s);
      deletedFacet_s.clear ();
      WH_T_Move (remainingFacet_s, facet_s_IO);
      WH_T_Add (newFacet_s, facet_s_IO);
    }
  }

  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < facet_s_IO.size ());
#endif
}

void WH_GM2D_SegmentFacet
::divideFacetsByPoints 
(const vector<WH_Vector2D>& pointBy_s,
 vector<WH_GM2D_SegmentFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Vector2D>::const_iterator 
	 i_point = pointBy_s.begin ();
       i_point != pointBy_s.end ();
       i_point++) {
    WH_Vector2D pointBy_i = (*i_point);
    
    vector<WH_GM2D_SegmentFacet*> remainingFacet_s;
    vector<WH_GM2D_SegmentFacet*> deletedFacet_s;
    vector<WH_GM2D_SegmentFacet*> newFacet_s;
    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM2D_SegmentFacet* facet_i = (*i_facet);

      vector<WH_GM2D_SegmentFacet*> facet_s;
      if (facet_i->createDividedFacetsByPoint (pointBy_i,
					       facet_s)) {
	WH_CVR_LINE;
	deletedFacet_s.push_back (facet_i);
	WH_T_Add (facet_s, newFacet_s);
      } else {
	WH_CVR_LINE;
	remainingFacet_s.push_back (facet_i);
      }
    }
    WH_ASSERT(deletedFacet_s.size () + remainingFacet_s.size ()
	      == facet_s_IO.size ());
    
    if (0 < newFacet_s.size ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < deletedFacet_s.size ());
      WH_ASSERT(2 <= newFacet_s.size ());
      WH_T_Delete (deletedFacet_s);
      deletedFacet_s.clear ();
      WH_T_Move (remainingFacet_s, facet_s_IO);
      WH_T_Add (newFacet_s, facet_s_IO);
    }
  }

  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < facet_s_IO.size ());
#endif
}

WH_GM2D_SegmentFacet
::WH_GM2D_SegmentFacet 
(const WH_Vector2D& firstPoint,
 const WH_Vector2D& lastPoint,
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (firstPoint, lastPoint));
  
  WH_CVR_LINE;
  
  _firstPoint = firstPoint;
  _lastPoint = lastPoint;
  _frontSideIsInsideVolume = frontSideIsInsideVolume;
  _backSideIsInsideVolume = backSideIsInsideVolume;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM2D_SegmentFacet
::~WH_GM2D_SegmentFacet () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_SegmentFacet
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_GM2D_SegmentFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(WH_ne (this->firstPoint (), this->lastPoint ()));
  
  return true;
}

void WH_GM2D_SegmentFacet
::reverseNormal ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->edgeType () == OUTER_BOUNDARY);

  if (_frontSideIsInsideVolume && !_backSideIsInsideVolume) {
    _frontSideIsInsideVolume = false;
    _backSideIsInsideVolume = true;
  } else if (!_frontSideIsInsideVolume && _backSideIsInsideVolume) {
    _frontSideIsInsideVolume = true;
    _backSideIsInsideVolume = false;
  } else {
    WH_ASSERT_NO_REACH;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->edgeType () == OUTER_BOUNDARY);
#endif
}

void WH_GM2D_SegmentFacet
::changeFromOuterToInner ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->edgeType () == OUTER_BOUNDARY);

  _frontSideIsInsideVolume = true;
  _backSideIsInsideVolume = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->edgeType () == INNER_BOUNDARY);
#endif
}

bool WH_GM2D_SegmentFacet
::createDividedFacetsBySegment 
(const WH_Segment2D& segmentBy, 
 vector<WH_GM2D_SegmentFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Segment2D segment = this->segment ();

  WH_Vector2D intersectionPoint;
  WH_Segment2D::WithSegmentIntersectionType intersectionFlag 
    = segment.checkIntersectionWith (segmentBy,
				     intersectionPoint);
  if (intersectionFlag == WH_Segment2D::POINT_WITH_SEGMENT) {
    WH_CVR_LINE;
    if (WH_ne (intersectionPoint, segment.p0 ())
	&& WH_ne (intersectionPoint, segment.p1 ())) {
      WH_CVR_LINE;
      WH_ASSERT(segment.justContains (intersectionPoint));

      /* <this> is divided into 2 new facets */
      result = true;
      WH_GM2D_SegmentFacet* newFacet0
	= this->createCopyBetween 
	(segment.p0 (), intersectionPoint);
      WH_GM2D_SegmentFacet* newFacet1
	= this->createCopyBetween
	(intersectionPoint, segment.p1 ());
      facet_s_OUT.push_back (newFacet0);
      facet_s_OUT.push_back (newFacet1);

    }
  } else if (intersectionFlag 
	     == WH_Segment2D::COINCIDENT_WITH_SEGMENT) {
    WH_CVR_LINE;
    WH_Segment2D::WithSegmentOverlapType overlapFlag 
      = segment.checkOverlapWith (segmentBy);
    if (overlapFlag == WH_Segment2D::OVERLAP_WITH_SEGMENT) {
      WH_CVR_LINE;
      if (segment.justContains (segmentBy.p0 ())) {
	WH_CVR_LINE;

	/* <this> is divided into 2 new facets */
	result = true;
	WH_GM2D_SegmentFacet* newFacet0
	  = this->createCopyBetween 
	  (segment.p0 (), segmentBy.p0 ());
	WH_GM2D_SegmentFacet* newFacet1
	  = this->createCopyBetween 
	  (segmentBy.p0 (), segment.p1 ());
	facet_s_OUT.push_back (newFacet0);
	facet_s_OUT.push_back (newFacet1);

      } else if (segment.justContains (segmentBy.p1 ())) {
	WH_CVR_LINE;

	/* <this> is divided into 2 new facets */
	result = true;
	WH_GM2D_SegmentFacet* newFacet0
	  = this->createCopyBetween 
	  (segment.p0 (), segmentBy.p1 ());
	WH_GM2D_SegmentFacet* newFacet1
	  = this->createCopyBetween 
	  (segmentBy.p1 (), segment.p1 ());
	facet_s_OUT.push_back (newFacet0);
	facet_s_OUT.push_back (newFacet1);

      }
    } else if (overlapFlag == WH_Segment2D::CONTAINS_WITH_SEGMENT) {
      WH_CVR_LINE;
      WH_ASSERT(segment.contains (segmentBy.p0 ()));
      WH_ASSERT(segment.contains (segmentBy.p1 ()));

      double distTo0 = WH_distance (segment.p0 (), segmentBy.p0 ());
      double distTo1 = WH_distance (segment.p0 (), segmentBy.p1 ());
      if (WH_lt (distTo0, distTo1)) {
	WH_CVR_LINE;
	/* segment.p0 -> segmentBy.p0 -> segmentBy.p1 -> segment.p1 */

	/* <this> is divided into 2 or 3 new facets */
	result = true;
	if (WH_ne (segment.p0 (), segmentBy.p0 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segment.p0 (), segmentBy.p0 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p0 (), segmentBy.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p0 (), segmentBy.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p1 (), segment.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p1 (), segment.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
	
      } else {
	WH_CVR_LINE;
	/* segment.p0 -> segmentBy.p1 -> segmentBy.p0 -> segment.p1 */
	
	/* <this> is divided into 2 or 3 new facets */
	result = true;
	if (WH_ne (segment.p0 (), segmentBy.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segment.p0 (), segmentBy.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p1 (), segmentBy.p0 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p1 (), segmentBy.p0 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p0 (), segment.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM2D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p0 (), segment.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(2 <= facet_s_OUT.size ());
  } else {
    WH_ASSERT(facet_s_OUT.size () == 0);
  }
#endif

  return result;
}

bool WH_GM2D_SegmentFacet
::createDividedFacetsByPoint 
(const WH_Vector2D& pointBy, 
 vector<WH_GM2D_SegmentFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Segment2D segment = this->segment ();
  if (segment.justContains (pointBy)) {
    WH_CVR_LINE;
    
    /* <this> is divided into 2 new facets */
    result = true;
    WH_GM2D_SegmentFacet* newFacet0
      = this->createCopyBetween 
      (segment.p0 (), pointBy);
    WH_GM2D_SegmentFacet* newFacet1
      = this->createCopyBetween
      (pointBy, segment.p1 ());
    facet_s_OUT.push_back (newFacet0);
    facet_s_OUT.push_back (newFacet1);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(2 <= facet_s_OUT.size ());
  } else {
    WH_ASSERT(facet_s_OUT.size () == 0);
  }
#endif

  return result;
}

WH_GM2D_SegmentFacet* WH_GM2D_SegmentFacet
::createCopy ()
{
  WH_GM2D_SegmentFacet* result 
    = new WH_GM2D_SegmentFacet 
    (_firstPoint, _lastPoint, 
     _frontSideIsInsideVolume, _backSideIsInsideVolume);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM2D_SegmentFacet* WH_GM2D_SegmentFacet
::createCopyBetween 
(const WH_Vector2D& firstPoint,
 const WH_Vector2D& lastPoint)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (firstPoint, lastPoint));
  
  WH_GM2D_SegmentFacet* result 
    = new WH_GM2D_SegmentFacet 
    (firstPoint, lastPoint, 
     _frontSideIsInsideVolume, _backSideIsInsideVolume);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_Vector2D WH_GM2D_SegmentFacet
::firstPoint () const
{
  return _firstPoint;
}

WH_Vector2D WH_GM2D_SegmentFacet
::lastPoint () const
{
  return _lastPoint;
}

bool WH_GM2D_SegmentFacet
::frontSideIsInsideVolume () const
{
  return _frontSideIsInsideVolume;
}

bool WH_GM2D_SegmentFacet
::backSideIsInsideVolume () const
{
  return _backSideIsInsideVolume;
}

WH_GM2D_SegmentFacet::EdgeType WH_GM2D_SegmentFacet
::edgeType () const
{
  if (_frontSideIsInsideVolume) {
    if (_backSideIsInsideVolume) {
      return INNER_BOUNDARY;
    } else {
      return OUTER_BOUNDARY;
    }
  } else {
    if (_backSideIsInsideVolume) {
      return OUTER_BOUNDARY;
    } else {
      return WIRE;
    }
  }
}

WH_Segment2D WH_GM2D_SegmentFacet
::segment () const
{
  return WH_Segment2D (_firstPoint, _lastPoint);
}

bool WH_GM2D_SegmentFacet
::getNormalToOutsideVolume 
(WH_Vector2D& normal_OUT) const
{
  WH_CVR_LINE;
  
  if (this->frontSideIsInsideVolume ()) {
    WH_CVR_LINE;
    if (this->backSideIsInsideVolume ()) {
      WH_CVR_LINE;
      return false;
    } else {
      WH_CVR_LINE;
      normal_OUT = -this->segment ().line ().normal ();
      return true;
    }
  } else {
    WH_CVR_LINE;
    if (this->backSideIsInsideVolume ()) {
      WH_CVR_LINE;
      normal_OUT = this->segment ().line ().normal ();
      return true;
    } else {
      WH_CVR_LINE;
      return false;
    }
  }
}
  
void WH_GM2D_SegmentFacet
::getRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
{
  minRange_OUT = this->firstPoint ();
  maxRange_OUT = this->lastPoint ();
  WH_sortPair (minRange_OUT, maxRange_OUT);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}



/* class WH_GM2D_FacetBody */

WH_GM2D_FacetBody
::WH_GM2D_FacetBody (bool isRegular) 
{
  WH_CVR_LINE;

  _isRegular = isRegular;
  _normalIsReversed = false;
  _inOutChecker = WH_NULL;
}

WH_GM2D_FacetBody
::~WH_GM2D_FacetBody () 
{
  WH_CVR_LINE;

  delete _inOutChecker;
  WH_T_Delete (_segmentFacet_s);
}

bool WH_GM2D_FacetBody
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_GM2D_FacetBody
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_T_AssureInvariant (_segmentFacet_s);
  


  int nVertexs = (int)this->vertexPoint_s ().size ();
  
  /* count number of edges of each type */
  int nEdges = (int)this->segmentFacet_s ().size ();
  int wireEdgeCount = 0;
  int outerBoundaryEdgeCount = 0;
  int innerBoundaryEdgeCount = 0;
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    
    switch (facet_i->edgeType ()) {
    case WH_GM2D_SegmentFacet::WIRE:
      WH_CVR_LINE;
      wireEdgeCount++;
      break;
    case WH_GM2D_SegmentFacet::INNER_BOUNDARY:
      WH_CVR_LINE;
      innerBoundaryEdgeCount++;
      break;
    case WH_GM2D_SegmentFacet::OUTER_BOUNDARY:
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
    WH_ASSERT(nEdges == 0);
    
    break;
  case WIRE_SET:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 < wireEdgeCount);
    WH_ASSERT(outerBoundaryEdgeCount == 0);
    WH_ASSERT(innerBoundaryEdgeCount == 0);
    WH_ASSERT(nEdges == wireEdgeCount);
    
    break;
  case VOLUME:
    WH_CVR_LINE;

    if (this->isRegular ()) {
      WH_ASSERT(nVertexs == 0);
    } else {
      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(nEdges == nVertexs);
    }
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(wireEdgeCount == 0);
    WH_ASSERT(0 < outerBoundaryEdgeCount);
    WH_ASSERT(innerBoundaryEdgeCount == 0);
    WH_ASSERT(nEdges == outerBoundaryEdgeCount);
    break;
  case OTHER:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);
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

  WH_ASSERT(!WH_hasDuplication (this->vertexPoint_s ()));
  
  WH_ASSERT(!WH_T_HasDuplication (this->segmentFacet_s ()));

  /* check intersection of segment facets */
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    WH_Segment2D segment_i = facet_i->segment ();

    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   j_facet = i_facet + 1;
	 j_facet != this->segmentFacet_s ().end ();
	 j_facet++) {
      WH_GM2D_SegmentFacet* facet_j = (*j_facet);
      WH_Segment2D segment_j = facet_j->segment ();

      WH_SegmentIntersector2D intersector (segment_j, segment_i);  
      intersector.perform ();
      WH_ASSERT(!intersector.intersects ()
		|| intersector.intersectsAtVertexOnly ());
    }
  }
  
  return true;
}

void WH_GM2D_FacetBody
::addVertexPoint (const WH_Vector2D& point)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isRegular ());
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_contains (this->vertexPoint_s (), point));
#endif

  _vertexPoint_s.push_back (point);
}

void WH_GM2D_FacetBody
::addSegmentFacet (WH_GM2D_SegmentFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(facet != WH_NULL);
#ifndef NDEBUG
  if (this->isRegular ()) {
    WH_ASSERT(facet->edgeType () 
	      == WH_GM2D_SegmentFacet::OUTER_BOUNDARY);
  }
#endif
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->segmentFacet_s (), facet));
#endif
  
  _segmentFacet_s.push_back (facet);
}

void WH_GM2D_FacetBody
::regularizeFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isRegular ());
  WH_ASSERT(this->bodyType () == VOLUME);
  WH_ASSERT(0 < this->segmentFacet_s ().size ());

  WH_CVR_LINE;

  WH_CNCT2D_SegmentConnector connector;
  
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = _segmentFacet_s.begin ();
       i_facet != _segmentFacet_s.end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);

    WH_CNCT2D_SegmentNode* node 
      = new WH_CNCT2D_SegmentNode (facet_i->segment ());
    connector.addNode (node);
  }

  connector.connect ();
  connector.identifyClusters ();
  connector.sortClusters ();
  
  vector<WH_GM2D_SegmentFacet*> newFacet_s;
  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = connector.cluster_s ().begin ();
       i_cluster != connector.cluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_SegmentCluster* cluster_i 
      = (WH_CNCT2D_SegmentCluster*)(*i_cluster);

    vector<WH_Segment2D> segment_s;
    vector<int> endId_s;
    cluster_i->getRegularizedSegments 
      (segment_s, endId_s);
    WH_ASSERT(2 < segment_s.size ());
    
    for (int iSeg = 0; iSeg < (int)segment_s.size (); iSeg++) {
      WH_Segment2D seg = segment_s[iSeg];
      int startId = 0;
      if (0 < iSeg) {
	WH_CVR_LINE;
	startId = endId_s[iSeg - 1] + 1;
      }
      WH_ASSERT(0 <= startId);
      WH_ASSERT(startId < (int)cluster_i->node_s ().size ());
      WH_CNCT_Node_A* startNode = cluster_i->node_s ()[startId];
      
      WH_ASSERT(0 <= startNode->id ());
      WH_ASSERT(startNode->id () < (int)_segmentFacet_s.size ());
      WH_GM2D_SegmentFacet* oldFacet 
	= _segmentFacet_s[startNode->id ()];
      
      /* create a new segment facet over <seg> with the same
	 orientation as <oldFacet> at <startId> */
      WH_GM2D_SegmentFacet* newFacet 
	= oldFacet->createCopyBetween (seg.p0 (), seg.p1 ()); 
      newFacet_s.push_back (newFacet);
    }
  }

  WH_T_Delete (_segmentFacet_s);
  WH_T_Move (newFacet_s, _segmentFacet_s);
  newFacet_s.clear ();
}

void WH_GM2D_FacetBody
::reverseNormal ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->bodyType () == VOLUME);
  
  WH_CVR_LINE;

  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    facet_i->reverseNormal ();
  }

  if (_normalIsReversed) {
    WH_CVR_LINE;
    _normalIsReversed = false;
  } else {
    WH_CVR_LINE;
    _normalIsReversed = true;
  }
}

void WH_GM2D_FacetBody
::setUpInOutCheck ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->bodyType () == VOLUME
	    || this->bodyType () == OTHER);
  WH_ASSERT(_inOutChecker == WH_NULL);
  
  WH_CVR_LINE;

  double minLength = WH::HUGE_VALUE;
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    minLength = WH_min (minLength, facet_i->segment ().length ());
  }
  
  _inOutChecker = new WH_InOutChecker2D (minLength);
  WH_ASSERT(_inOutChecker != WH_NULL);
  
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);

    if (facet_i->edgeType () == WH_GM2D_SegmentFacet::OUTER_BOUNDARY) {
      WH_Vector2D normal;
      facet_i->getNormalToOutsideVolume 
	(normal);
      if (this->normalIsReversed ()) {
	WH_CVR_LINE;
	normal = -normal;
      }
      _inOutChecker->addEdge 
	(facet_i->firstPoint (), facet_i->lastPoint (),
	 normal);
    }
  }
  _inOutChecker->setUp ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_inOutChecker != WH_NULL);
#endif
}

WH_GM2D_SegmentFacet* WH_GM2D_FacetBody
::createSegmentFacet 
(const WH_Vector2D& firstPoint,
 const WH_Vector2D& lastPoint,
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (firstPoint, lastPoint));

  WH_GM2D_SegmentFacet* result 
    = new WH_GM2D_SegmentFacet 
    (firstPoint, lastPoint, 
     frontSideIsInsideVolume, backSideIsInsideVolume);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

bool WH_GM2D_FacetBody
::isRegular () const
{
  return _isRegular;
}

bool WH_GM2D_FacetBody
::normalIsReversed () const
{
  return _normalIsReversed;
}

const vector<WH_Vector2D>& WH_GM2D_FacetBody
::vertexPoint_s () const
{
  return _vertexPoint_s;
}
  
const vector<WH_GM2D_SegmentFacet*>& WH_GM2D_FacetBody
::segmentFacet_s () const
{
  return _segmentFacet_s;
}

WH_GM2D_FacetBody::BodyType WH_GM2D_FacetBody
::bodyType () const
{
  WH_CVR_LINE;

  BodyType result = OTHER;

  int nVertexs = (int)this->vertexPoint_s ().size ();
  int nEdges = (int)this->segmentFacet_s ().size ();

  if (nVertexs == 0
      && nEdges == 0) {
    WH_CVR_LINE;
    result = EMPTY;
  } else if (nEdges == 0) {
    WH_CVR_LINE;
    result = DOT_SET;
  } else {
    WH_CVR_LINE;

    int wireEdgeCount = 0;
    int outerBoundaryEdgeCount = 0;
    int innerBoundaryEdgeCount = 0;
    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   i_facet = this->segmentFacet_s ().begin ();
	 i_facet != this->segmentFacet_s ().end ();
	 i_facet++) {
      WH_GM2D_SegmentFacet* facet_i = (*i_facet);
      
      switch (facet_i->edgeType ()) {
      case WH_GM2D_SegmentFacet::WIRE:
	WH_CVR_LINE;
	wireEdgeCount++;
	break;
      case WH_GM2D_SegmentFacet::INNER_BOUNDARY:
	WH_CVR_LINE;
	innerBoundaryEdgeCount++;
	break;
      case WH_GM2D_SegmentFacet::OUTER_BOUNDARY:
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
    if (this->isRegular ()) {
      WH_ASSERT(nVertexs == 0);
    } else {
      WH_ASSERT(0 < nVertexs);
    }
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

WH_InOutChecker2D::ContainmentType WH_GM2D_FacetBody
::checkContainmentAt 
(const WH_Vector2D& point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(_inOutChecker != WH_NULL);
  
  WH_CVR_LINE;

  WH_InOutChecker2D::ContainmentType result 
    = _inOutChecker->checkContainmentAt (point);
  
  if (this->normalIsReversed ()) {
    WH_CVR_LINE;
    
    switch (result) {
    case WH_InOutChecker2D::IN:
      WH_CVR_LINE;
      result = WH_InOutChecker2D::OUT;
      break;
    case WH_InOutChecker2D::OUT:
      WH_CVR_LINE;
      result = WH_InOutChecker2D::IN;
      break;
    case WH_InOutChecker2D::ON:
      WH_CVR_LINE;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }

  return result;
}



/* test coverage completed */



