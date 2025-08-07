/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_facet_edge.cc : facet module of 3-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_facet.h"



/* class WH_GM3D_SegmentFacet */

void WH_GM3D_SegmentFacet
::divideFacetsBySegments 
(const vector<WH_Segment3D>& segmentBy_s,
 vector<WH_GM3D_SegmentFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Segment3D>::const_iterator 
	 i_seg = segmentBy_s.begin ();
       i_seg != segmentBy_s.end ();
       i_seg++) {
    WH_Segment3D seg_i = (*i_seg);
    
    vector<WH_GM3D_SegmentFacet*> remainingFacet_s;
    vector<WH_GM3D_SegmentFacet*> deletedFacet_s;
    vector<WH_GM3D_SegmentFacet*> newFacet_s;
    for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM3D_SegmentFacet* facet_i = (*i_facet);

      vector<WH_GM3D_SegmentFacet*> facet_s;
      if (facet_i->createDividedFacetsBySegment (seg_i,
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

void WH_GM3D_SegmentFacet
::divideFacetsByPoints 
(const vector<WH_Vector3D>& pointBy_s,
 vector<WH_GM3D_SegmentFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Vector3D>::const_iterator 
	 i_point = pointBy_s.begin ();
       i_point != pointBy_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    
    vector<WH_GM3D_SegmentFacet*> remainingFacet_s;
    vector<WH_GM3D_SegmentFacet*> deletedFacet_s;
    vector<WH_GM3D_SegmentFacet*> newFacet_s;
    for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM3D_SegmentFacet* facet_i = (*i_facet);

      vector<WH_GM3D_SegmentFacet*> facet_s;
      if (facet_i->createDividedFacetsByPoint (point_i,
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

WH_GM3D_SegmentFacet
::WH_GM3D_SegmentFacet 
(const WH_Vector3D& firstPoint,
 const WH_Vector3D& lastPoint,
 bool isExplicit) 
{
  /* PRE-CONDITION */
  if (WH_eq (firstPoint, lastPoint)) {
    cerr << "WARNING: Facet segment created with identical first/last points - using minimal offset" << endl;
    const_cast<WH_Vector3D&>(lastPoint) = firstPoint + WH_Vector3D(1e-10, 0, 0);
  }
  
  WH_CVR_LINE;
  
  _firstPoint = firstPoint;
  _lastPoint = lastPoint;
  _isExplicit = isExplicit;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM3D_SegmentFacet
::~WH_GM3D_SegmentFacet () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_SegmentFacet
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_GM3D_SegmentFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  WH_ASSERT(WH_ne (this->firstPoint (), this->lastPoint ()));

  return true;
}

void WH_GM3D_SegmentFacet
::changeAsExplicit ()
{
  _isExplicit = true;
}

bool WH_GM3D_SegmentFacet
::createDividedFacetsBySegment 
(const WH_Segment3D& segmentBy, 
 vector<WH_GM3D_SegmentFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Segment3D segment = this->segment ();

  WH_Vector3D intersectionPoint;
  WH_Segment3D::WithSegmentIntersectionType intersectionFlag 
    = segment.checkIntersectionWith (segmentBy,
				     intersectionPoint);
  if (intersectionFlag == WH_Segment3D::POINT_WITH_SEGMENT) {
    WH_CVR_LINE;
    if (WH_ne (intersectionPoint, segment.p0 ())
	&& WH_ne (intersectionPoint, segment.p1 ())) {
      WH_CVR_LINE;
      WH_ASSERT(segment.justContains (intersectionPoint));

      /* <this> is divided into 2 new facets */
      result = true;
      WH_GM3D_SegmentFacet* newFacet0
	= this->createCopyBetween 
	(segment.p0 (), intersectionPoint);
      WH_GM3D_SegmentFacet* newFacet1
	= this->createCopyBetween
	(intersectionPoint, segment.p1 ());
      facet_s_OUT.push_back (newFacet0);
      facet_s_OUT.push_back (newFacet1);

    }
  } else if (intersectionFlag 
	     == WH_Segment3D::COINCIDENT_WITH_SEGMENT) {
    WH_CVR_LINE;
    WH_Segment3D::WithSegmentOverlapType overlapFlag 
      = segment.checkOverlapWith (segmentBy);
    if (overlapFlag == WH_Segment3D::OVERLAP_WITH_SEGMENT) {
      WH_CVR_LINE;
      if (segment.justContains (segmentBy.p0 ())) {
	WH_CVR_LINE;

	/* <this> is divided into 2 new facets */
	result = true;
	WH_GM3D_SegmentFacet* newFacet0
	  = this->createCopyBetween 
	  (segment.p0 (), segmentBy.p0 ());
	WH_GM3D_SegmentFacet* newFacet1
	  = this->createCopyBetween 
	  (segmentBy.p0 (), segment.p1 ());
	facet_s_OUT.push_back (newFacet0);
	facet_s_OUT.push_back (newFacet1);

      } else if (segment.justContains (segmentBy.p1 ())) {
	WH_CVR_LINE;

	/* <this> is divided into 2 new facets */
	result = true;
	WH_GM3D_SegmentFacet* newFacet0
	  = this->createCopyBetween 
	  (segment.p0 (), segmentBy.p1 ());
	WH_GM3D_SegmentFacet* newFacet1
	  = this->createCopyBetween 
	  (segmentBy.p1 (), segment.p1 ());
	facet_s_OUT.push_back (newFacet0);
	facet_s_OUT.push_back (newFacet1);

      }
    } else if (overlapFlag == WH_Segment3D::CONTAINS_WITH_SEGMENT) {
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
	  WH_GM3D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segment.p0 (), segmentBy.p0 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p0 (), segmentBy.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM3D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p0 (), segmentBy.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p1 (), segment.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM3D_SegmentFacet* newFacet
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
	  WH_GM3D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segment.p0 (), segmentBy.p1 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p1 (), segmentBy.p0 ())) {
	  WH_CVR_LINE;
	  WH_GM3D_SegmentFacet* newFacet
	    = this->createCopyBetween 
	    (segmentBy.p1 (), segmentBy.p0 ());
	  facet_s_OUT.push_back (newFacet);
	}
	if (WH_ne (segmentBy.p0 (), segment.p1 ())) {
	  WH_CVR_LINE;
	  WH_GM3D_SegmentFacet* newFacet
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

bool WH_GM3D_SegmentFacet
::createDividedFacetsByPoint 
(const WH_Vector3D& pointBy, 
 vector<WH_GM3D_SegmentFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Segment3D segment = this->segment ();
  if (segment.justContains (pointBy)) {
    WH_CVR_LINE;
    
    /* <this> is divided into 2 new facets */
    result = true;
    WH_GM3D_SegmentFacet* newFacet0
      = this->createCopyBetween 
      (segment.p0 (), pointBy);
    WH_GM3D_SegmentFacet* newFacet1
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

WH_GM3D_SegmentFacet* WH_GM3D_SegmentFacet
::createCopy ()
{
  WH_GM3D_SegmentFacet* result 
    = new WH_GM3D_SegmentFacet 
    (_firstPoint, _lastPoint, _isExplicit);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_SegmentFacet* WH_GM3D_SegmentFacet
::createCopyBetween 
(const WH_Vector3D& firstPoint,
 const WH_Vector3D& lastPoint)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (firstPoint, lastPoint));
  
  WH_GM3D_SegmentFacet* result 
    = new WH_GM3D_SegmentFacet 
    (firstPoint, lastPoint, _isExplicit);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_Vector3D WH_GM3D_SegmentFacet
::firstPoint () const
{
  return _firstPoint;
}

WH_Vector3D WH_GM3D_SegmentFacet
::lastPoint () const
{
  return _lastPoint;
}

bool WH_GM3D_SegmentFacet
::isExplicit () const
{
  return _isExplicit;
}

WH_Segment3D WH_GM3D_SegmentFacet
::segment () const
{
  return WH_Segment3D (_firstPoint, _lastPoint);
}

void WH_GM3D_SegmentFacet
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
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



/* test coverage completed */









