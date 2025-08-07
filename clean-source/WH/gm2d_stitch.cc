/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d_stitch.cc : stitch operation module */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d_stitch.h"



/* class WH_GM2D_Stitcher */

WH_GM2D_Stitcher
::WH_GM2D_Stitcher 
(WH_GM2D_FacetBody* body0,
 WH_GM2D_FacetBody* body1,
 WH_GM2D_FacetBody* resultBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(!body0->isRegular ());
  WH_ASSERT(body0->bodyType () 
	    != WH_GM2D_FacetBody::EMPTY);
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(!body1->isRegular ());
  WH_ASSERT(body1->bodyType () 
	    != WH_GM2D_FacetBody::EMPTY);
  WH_ASSERT(resultBody != WH_NULL);
  WH_ASSERT(!resultBody->isRegular ());
  WH_ASSERT(resultBody->bodyType () 
	    == WH_GM2D_FacetBody::EMPTY);
  WH_ASSERT(body0 != body1);
  WH_ASSERT(resultBody != body0);
  WH_ASSERT(resultBody != body1);
  
  WH_CVR_LINE;
  
  _body0 = body0;
  _body1 = body1;
  _resultBody = resultBody;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM2D_Stitcher
::~WH_GM2D_Stitcher () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_Stitcher
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body0 () != WH_NULL);
  WH_ASSERT(!this->body0 ()->isRegular ());
  WH_ASSERT(this->body0 ()->bodyType () 
	    != WH_GM2D_FacetBody::EMPTY);
  WH_ASSERT(this->body1 () != WH_NULL);
  WH_ASSERT(!this->body1 ()->isRegular ());
  WH_ASSERT(this->body1 ()->bodyType () 
	    != WH_GM2D_FacetBody::EMPTY);
  WH_ASSERT(this->resultBody () != WH_NULL);
  WH_ASSERT(!this->resultBody ()->isRegular ());
  WH_ASSERT(this->body0 () != this->body1 ());
  WH_ASSERT(this->resultBody () != this->body0 ());
  WH_ASSERT(this->resultBody () != this->body1 ());

  return true;
}

bool WH_GM2D_Stitcher
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_GM2D_Stitcher
::divideBodyByBody 
(WH_GM2D_FacetBody* bodyFrom, 
 WH_GM2D_FacetBody* bodyBy,
 vector<WH_GM2D_SegmentFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyFrom != WH_NULL);
  WH_ASSERT(bodyBy != WH_NULL);
  WH_ASSERT(bodyFrom != bodyBy);

  WH_CVR_LINE;

  facet_s_OUT.clear ();

  vector<WH_Segment2D> wireSegmentBy_s;
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = bodyBy->segmentFacet_s ().begin ();
       i_facet != bodyBy->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    if (facet_i->edgeType () == WH_GM2D_SegmentFacet::WIRE) {
      WH_CVR_LINE;
      wireSegmentBy_s.push_back (facet_i->segment ());
    }
  }  
  
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = bodyFrom->segmentFacet_s ().begin ();
       i_facet != bodyFrom->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    
    vector<WH_GM2D_SegmentFacet*> dividedFacet_s;
    dividedFacet_s.push_back (facet_i->createCopy ());

    /* divide facets by vertex points */
    WH_GM2D_SegmentFacet::divideFacetsByPoints 
      (bodyBy->vertexPoint_s (), 
       dividedFacet_s);

    /* divide wire facets by wire facets */
    if (facet_i->edgeType () == WH_GM2D_SegmentFacet::WIRE) {
      WH_CVR_LINE;
      WH_GM2D_SegmentFacet::divideFacetsBySegments 
	(wireSegmentBy_s, 
	 dividedFacet_s);
    }

    WH_T_Add (dividedFacet_s, facet_s_OUT);
  }
}

void WH_GM2D_Stitcher
::collectExplicitVertexPoints 
(vector<WH_Vector2D>& point_s_OUT)
{
  WH_CVR_LINE;

  /* collect explicit vertex points at intersection between two wire
     edges into <point_s_OUT> */
  
  point_s_OUT.clear ();

  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = _body0->segmentFacet_s ().begin ();
       i_facet != _body0->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);

    if (facet_i->edgeType () == WH_GM2D_SegmentFacet::WIRE) {
      WH_CVR_LINE;
      WH_Segment2D seg_i = facet_i->segment ();
	
      for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	     j_facet = _body1->segmentFacet_s ().begin ();
	   j_facet != _body1->segmentFacet_s ().end ();
	   j_facet++) {
	WH_GM2D_SegmentFacet* facet_j = (*j_facet);

	if (facet_j->edgeType () == WH_GM2D_SegmentFacet::WIRE) {
	  WH_CVR_LINE;
	  WH_Segment2D seg_j = facet_j->segment ();

	  WH_Vector2D intersectionPoint;
	  WH_Segment2D::WithSegmentIntersectionType intersectionFlag 
	    = seg_i.checkIntersectionWith (seg_j,
					   intersectionPoint);
	  if (intersectionFlag == WH_Segment2D::POINT_WITH_SEGMENT) {
	    WH_CVR_LINE;
	    if (seg_i.justContains (intersectionPoint)
		&& seg_j.justContains (intersectionPoint)) {
	      WH_CVR_LINE;
	      point_s_OUT.push_back (intersectionPoint);
	    }
	  }
	}
      }  
    }
  }  
  
}

void WH_GM2D_Stitcher
::clearDuplicatedFacets 
(WH_GM2D_FacetBody* bodyFrom,
 const vector<WH_GM2D_SegmentFacet*>& facetFrom_s,
 const vector<WH_GM2D_SegmentFacet*>& facetBy_s,
 vector<WH_GM2D_SegmentFacet*>& remainingFacet_s_IO,
 vector<WH_GM2D_SegmentFacet*>& changingFacet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyFrom != WH_NULL);

  WH_CVR_LINE;

  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = facetFrom_s.begin ();
       i_facet != facetFrom_s.end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    WH_Segment2D segment_i = facet_i->segment ();
    
    WH_GM2D_SegmentFacet* duplicatedFacetBy = WH_NULL;
    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   j_facet = facetBy_s.begin ();
	 j_facet != facetBy_s.end ();
	 j_facet++) {
      WH_GM2D_SegmentFacet* facet_j = (*j_facet);
      WH_Segment2D segment_j = facet_j->segment ();

      if (WH_eq (segment_j, segment_i) 
	  || WH_isReverse (segment_j, segment_i)) {
	WH_CVR_LINE;
	duplicatedFacetBy = facet_j;
	break;
      }
    }

    if (duplicatedFacetBy != WH_NULL) {
      WH_CVR_LINE;
      WH_ASSERT(facet_i->edgeType () 
		!= WH_GM2D_SegmentFacet::INNER_BOUNDARY);
      WH_ASSERT(duplicatedFacetBy->edgeType () 
		!= WH_GM2D_SegmentFacet::INNER_BOUNDARY);
      if (facet_i->edgeType () 
	  == WH_GM2D_SegmentFacet::WIRE
	  && duplicatedFacetBy->edgeType () 
	  == WH_GM2D_SegmentFacet::WIRE) {
	WH_CVR_LINE;
	if (bodyFrom == _body0) {
	  WH_CVR_LINE;
	  /* <facet_i> of <body0> will remain. */
	  /* <duplicatedFacetBy> of <body1> will be deleted. */

	  remainingFacet_s_IO.push_back (facet_i);
	} else {
	  WH_CVR_LINE;
	  /* <facet_i> of <body1> will be deleted. */
	  /* <duplicatedFacetBy> of <body0> will remain. */

	  /* nothing */
	}
      } else if (facet_i->edgeType () 
		 == WH_GM2D_SegmentFacet::OUTER_BOUNDARY
		 && duplicatedFacetBy->edgeType () 
		 == WH_GM2D_SegmentFacet::OUTER_BOUNDARY) {
	WH_CVR_LINE;
	if (bodyFrom == _body0) {
	  WH_CVR_LINE;
	  /* <facet_i> of <body0> will be changed as inner boundary. */
	  /* <duplicatedFacetBy> of <body1> will be deleted. */

	  changingFacet_s_IO.push_back (facet_i);
	  remainingFacet_s_IO.push_back (facet_i);
	} else {
	  WH_CVR_LINE;
	  /* <facet_i> of <body1> will be deleted. */
	  /* <duplicatedFacetBy> of <body0> will be changed as inner
             boundary. */

	  /* nothing */
	}
      } else if (facet_i->edgeType () 
		 == WH_GM2D_SegmentFacet::WIRE
		 && duplicatedFacetBy->edgeType () 
		 == WH_GM2D_SegmentFacet::OUTER_BOUNDARY) {
	WH_CVR_LINE;
	/* The wire <facet_i> will be deleted. */
	/* The outer boundary <duplicatedFacetBy> will remain. */

	/* nothing */
      } else if (facet_i->edgeType () 
		 == WH_GM2D_SegmentFacet::OUTER_BOUNDARY
		 && duplicatedFacetBy->edgeType () 
		 == WH_GM2D_SegmentFacet::WIRE) {
	WH_CVR_LINE;
	/* The outer boundary <facet_i> will remain. */
	/* The wire <duplicatedFacetBy> will be deleted. */

	remainingFacet_s_IO.push_back (facet_i);
      } else {
	WH_ASSERT_NO_REACH;
      }
    } else {
      WH_CVR_LINE;
      remainingFacet_s_IO.push_back (facet_i);
    }
  }
}

void WH_GM2D_Stitcher
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM2D_FacetBody::EMPTY);

  WH_CVR_LINE;
  
  /* divide facets of each body by the other body */

  vector<WH_GM2D_SegmentFacet*> facetOfBody0_s;  /* own */
  this->divideBodyByBody (_body0, _body1,
			  facetOfBody0_s);

  vector<WH_GM2D_SegmentFacet*> facetOfBody1_s;  /* own */
  this->divideBodyByBody (_body1, _body0,
			  facetOfBody1_s);

  /* collect explicit vertex points */
  vector<WH_Vector2D> explicitVertexPoint_s;
  this->collectExplicitVertexPoints 
    (explicitVertexPoint_s);

  /* find duplicated facets */
  vector<WH_GM2D_SegmentFacet*> remainingFacet_s;
  vector<WH_GM2D_SegmentFacet*> changingFacet_s;
  this->clearDuplicatedFacets 
    (_body0, facetOfBody0_s, facetOfBody1_s,
     remainingFacet_s, changingFacet_s);
  this->clearDuplicatedFacets 
    (_body1, facetOfBody1_s, facetOfBody0_s,
     remainingFacet_s, changingFacet_s);
  
  /* change duplicated outer boundary facets to inner boundary */
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = changingFacet_s.begin ();
       i_facet != changingFacet_s.end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    facet_i->changeFromOuterToInner ();
  }

  /* collect remaining facets */
  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = remainingFacet_s.begin ();
       i_facet != remainingFacet_s.end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    _resultBody->addSegmentFacet (facet_i->createCopy ());
  }

  /* collect vertex points */
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = _body0->vertexPoint_s ().begin ();
       i_point != _body0->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    _resultBody->addVertexPoint (point_i);
  }
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = _body1->vertexPoint_s ().begin ();
       i_point != _body1->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    if (!WH_T_Exists (_resultBody->vertexPoint_s (), point_i)) {
      _resultBody->addVertexPoint (point_i);
    }
  }
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = explicitVertexPoint_s.begin ();
       i_point != explicitVertexPoint_s.end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    if (!WH_T_Exists (_resultBody->vertexPoint_s (), point_i)) {
      _resultBody->addVertexPoint (point_i);
    }
  }

  WH_T_Delete (facetOfBody0_s);
  WH_T_Delete (facetOfBody1_s);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif  
}

WH_GM2D_FacetBody* WH_GM2D_Stitcher
::body0 () const
{
  return _body0;
}

WH_GM2D_FacetBody* WH_GM2D_Stitcher
::body1 () const
{
  return _body1;
}

WH_GM2D_FacetBody* WH_GM2D_Stitcher
::resultBody () const
{
  return _resultBody;
}



/* test coverage completed */



