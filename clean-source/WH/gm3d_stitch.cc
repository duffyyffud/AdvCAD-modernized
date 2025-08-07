/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_stitch.cc : stitch operation module */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_stitch.h"



/* class WH_GM3D_Stitcher */

WH_GM3D_Stitcher
::WH_GM3D_Stitcher 
(WH_GM3D_FacetBody* body0,
 WH_GM3D_FacetBody* body1,
 WH_GM3D_FacetBody* resultBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(!body0->isRegular ());
  WH_ASSERT(body0->bodyType () 
	    != WH_GM3D_FacetBody::EMPTY);
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(!body1->isRegular ());
  WH_ASSERT(body1->bodyType () 
	    != WH_GM3D_FacetBody::EMPTY);
  WH_ASSERT(resultBody != WH_NULL);
  WH_ASSERT(!resultBody->isRegular ());
  WH_ASSERT(resultBody->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);
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

WH_GM3D_Stitcher
::~WH_GM3D_Stitcher () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_Stitcher
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body0 () != WH_NULL);
  WH_ASSERT(!this->body0 ()->isRegular ());
  WH_ASSERT(this->body0 ()->bodyType () 
	    != WH_GM3D_FacetBody::EMPTY);
  WH_ASSERT(this->body1 () != WH_NULL);
  WH_ASSERT(!this->body1 ()->isRegular ());
  WH_ASSERT(this->body1 ()->bodyType () 
	    != WH_GM3D_FacetBody::EMPTY);
  WH_ASSERT(this->resultBody () != WH_NULL);
  WH_ASSERT(!this->resultBody ()->isRegular ());
  WH_ASSERT(this->body0 () != this->body1 ());
  WH_ASSERT(this->resultBody () != this->body0 ());
  WH_ASSERT(this->resultBody () != this->body1 ());

  return true;
}

bool WH_GM3D_Stitcher
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_GM3D_Stitcher
::divideBodyByBody 
(WH_GM3D_FacetBody* bodyFrom, 
 WH_GM3D_FacetBody* bodyBy,
 vector<WH_GM3D_SegmentFacet*>& segmentFacet_s_OUT,
 vector<WH_GM3D_TriangleFacet*>& triangleFacet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyFrom != WH_NULL);
  WH_ASSERT(bodyBy != WH_NULL);
  WH_ASSERT(bodyFrom != bodyBy);

  WH_CVR_LINE;

  segmentFacet_s_OUT.clear ();
  triangleFacet_s_OUT.clear ();

  vector<WH_Segment3D> segmentBy_s;
  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = bodyBy->segmentFacet_s ().begin ();
       i_facet != bodyBy->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    segmentBy_s.push_back (facet_i->segment ());
  }  
  
  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = bodyFrom->segmentFacet_s ().begin ();
       i_facet != bodyFrom->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    
    vector<WH_GM3D_SegmentFacet*> dividedFacet_s;
    dividedFacet_s.push_back (facet_i->createCopy ());

    /* divide segment facets by org. vertex points */
    WH_GM3D_SegmentFacet::divideFacetsByPoints 
      (bodyBy->vertexPoint_s (), 
       dividedFacet_s);

    /* divide segment facets by org. segment facets */
    WH_GM3D_SegmentFacet::divideFacetsBySegments 
      (segmentBy_s, 
       dividedFacet_s);

    WH_T_Add (dividedFacet_s, segmentFacet_s_OUT);
  }

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_pfacet = bodyFrom->polygonFacet_s ().begin ();
       i_pfacet != bodyFrom->polygonFacet_s ().end ();
       i_pfacet++) {
    WH_GM3D_PolygonFacet* pfacet_i = (*i_pfacet);
    
    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = pfacet_i->triangleFacet_s ().begin ();
	 i_facet != pfacet_i->triangleFacet_s ().end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);

      vector<WH_GM3D_TriangleFacet*> dividedFacet_s;
      dividedFacet_s.push_back (facet_i->createCopy ());
      
      /* divide triangle facets by org. segment facets */
      WH_GM3D_TriangleFacet::divideFacetsBySegments 
	(segmentBy_s, 
	 dividedFacet_s);
      
      WH_T_Add (dividedFacet_s, triangleFacet_s_OUT);
    }
  }
}

void WH_GM3D_Stitcher
::collectExplicitVertexPoints 
(vector<WH_Vector3D>& point_s_OUT)
{
  WH_CVR_LINE;

  /* collect explicit vertex points at intersection between two
     segment facets into <point_s_OUT> */
  
  point_s_OUT.clear ();

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = _body0->segmentFacet_s ().begin ();
       i_facet != _body0->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    WH_Segment3D seg_i = facet_i->segment ();
    
    for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	   j_facet = _body1->segmentFacet_s ().begin ();
	 j_facet != _body1->segmentFacet_s ().end ();
	 j_facet++) {
      WH_GM3D_SegmentFacet* facet_j = (*j_facet);
      WH_Segment3D seg_j = facet_j->segment ();

      WH_Vector3D intersectionPoint;
      WH_Segment3D::WithSegmentIntersectionType intersectionFlag 
	= seg_i.checkIntersectionWith (seg_j,
				       intersectionPoint);
      if (intersectionFlag == WH_Segment3D::POINT_WITH_SEGMENT) {
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

void WH_GM3D_Stitcher
::changeSegmentFacetsAsExplicit 
(WH_GM3D_FacetBody* bodyBy,
 vector<WH_GM3D_SegmentFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyBy != WH_NULL);

  WH_CVR_LINE;

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = facet_s_IO.begin ();
       i_facet != facet_s_IO.end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    if (facet_i->isExplicit ()) continue;
    
    WH_Segment3D seg = facet_i->segment ();
    WH_Vector3D midPoint = seg.midPoint ();
    
    WH_GM3D_PolygonFacet* containingFacetBy = WH_NULL;
    for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	   j_facet = bodyBy->polygonFacet_s ().begin ();
	 j_facet != bodyBy->polygonFacet_s ().end ();
	 j_facet++) {
      WH_GM3D_PolygonFacet* facet_j = (*j_facet);
      
      if (facet_j->justContains (midPoint)) {
	WH_CVR_LINE;
	WH_ASSERT(facet_j->contains (seg.p0 ()));
	WH_ASSERT(facet_j->contains (seg.p1 ()));
	containingFacetBy = facet_j;
	break;
      }
    }
    
    if (containingFacetBy != WH_NULL) {
      WH_CVR_LINE;
      facet_i->changeAsExplicit ();
    }
  }
}

void WH_GM3D_Stitcher
::clearDuplicatedSegmentFacets 
(WH_GM3D_FacetBody* bodyBy,
 const vector<WH_GM3D_SegmentFacet*>& facet_s,
 vector<WH_GM3D_SegmentFacet*>& remainingFacet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyBy != WH_NULL);
  WH_ASSERT(bodyBy == this->body0 ());

  WH_CVR_LINE;

  remainingFacet_s_OUT.clear ();

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    
    WH_Segment3D seg = facet_i->segment ();
    WH_Vector3D midPoint = seg.midPoint ();
    
    WH_GM3D_SegmentFacet* containingFacetBy = WH_NULL;
    for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	   j_facet = bodyBy->segmentFacet_s ().begin ();
	 j_facet != bodyBy->segmentFacet_s ().end ();
	 j_facet++) {
      WH_GM3D_SegmentFacet* facet_j = (*j_facet);

      WH_Segment3D segBy = facet_j->segment ();
      if (segBy.justContains (midPoint)) {
	WH_CVR_LINE;
	WH_ASSERT(segBy.contains (seg.p0 ()));
	WH_ASSERT(segBy.contains (seg.p1 ()));
	containingFacetBy = facet_j;
	break;
      }
    }
    
    if (containingFacetBy == WH_NULL) {
      WH_CVR_LINE;
      remainingFacet_s_OUT.push_back (facet_i);
    }
  }
}

void WH_GM3D_Stitcher
::clearDuplicatedTriangleFacets 
(WH_GM3D_FacetBody* bodyBy,
 const vector<WH_GM3D_TriangleFacet*>& facet_s,
 vector<WH_GM3D_TriangleFacet*>& remainingFacet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyBy != WH_NULL);

  WH_CVR_LINE;

  bool facetIsPreserved = (bodyBy == _body1);

  remainingFacet_s_OUT.clear ();

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    
    WH_Triangle3D tri = facet_i->triangle ();
    WH_Vector3D center = tri.centerOfGravity ();
    
    WH_GM3D_PolygonFacet* containingFacetBy = WH_NULL;
    for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	   j_facet = bodyBy->polygonFacet_s ().begin ();
	 j_facet != bodyBy->polygonFacet_s ().end ();
	 j_facet++) {
      WH_GM3D_PolygonFacet* facet_j = (*j_facet);
      
      if (facet_j->justContains (center)) {
	WH_CVR_LINE;
	WH_ASSERT(facet_j->contains (tri.vertex (0)));
	WH_ASSERT(facet_j->contains (tri.vertex (1)));
	WH_ASSERT(facet_j->contains (tri.vertex (2)));
	containingFacetBy = facet_j;
	break;
      }
    }
    
    if (containingFacetBy != WH_NULL) {
      WH_CVR_LINE;
      WH_ASSERT(facet_i->faceType () 
		!= WH_GM3D_TriangleFacet::INNER_BOUNDARY);
      WH_ASSERT(containingFacetBy->faceType () 
		!= WH_GM3D_PolygonFacet::INNER_BOUNDARY);
      if (facet_i->faceType () 
	  == WH_GM3D_TriangleFacet::SHEET
	  && containingFacetBy->faceType () 
	  == WH_GM3D_PolygonFacet::SHEET) {
	WH_CVR_LINE;
	if (facetIsPreserved) {
	  WH_CVR_LINE;
	  remainingFacet_s_OUT.push_back (facet_i);
	} else {
	  /* nothing */
	  /* <facet_i> will be deleted */
	}
      } else if (facet_i->faceType () 
		 == WH_GM3D_TriangleFacet::OUTER_BOUNDARY
		 && containingFacetBy->faceType () 
		 == WH_GM3D_PolygonFacet::OUTER_BOUNDARY) {
	WH_CVR_LINE;
	if (facetIsPreserved) {
	  WH_CVR_LINE;
	  facet_i->changeFromOuterToInner ();
	  remainingFacet_s_OUT.push_back (facet_i);
	} else {
	  /* nothing */
	  /* <facet_i> will be deleted */
	}
      } else if (facet_i->faceType () 
		 == WH_GM3D_TriangleFacet::OUTER_BOUNDARY
		 && containingFacetBy->faceType () 
		 == WH_GM3D_PolygonFacet::SHEET) {
	WH_CVR_LINE;
	remainingFacet_s_OUT.push_back (facet_i);
      } else {
	WH_ASSERT(facet_i->faceType () 
		  == WH_GM3D_TriangleFacet::SHEET
		  && containingFacetBy->faceType () 
		  == WH_GM3D_PolygonFacet::OUTER_BOUNDARY);
	/* nothing */
	/* <facet_i> will be deleted */
      }
    } else {
      WH_CVR_LINE;
      remainingFacet_s_OUT.push_back (facet_i);
    }
  }
}

void WH_GM3D_Stitcher
::collectTriangleFacets 
(const vector<WH_GM3D_TriangleFacet*>& facet_s)
{
  WH_CVR_LINE;

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    _resultBody->addTriangleFacet (facet_i->createCopy ());
  }
}

void WH_GM3D_Stitcher
::collectSegmentFacets 
(const vector<WH_GM3D_SegmentFacet*>& facet_s)
{
  WH_CVR_LINE;

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    _resultBody->addSegmentFacet (facet_i->createCopy ());
  }
}

void WH_GM3D_Stitcher
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);

  WH_CVR_LINE;
  
  /* divide facets of each body by the other body */

  vector<WH_GM3D_SegmentFacet*> segmentFacetOfBody0_s;  /* own */
  vector<WH_GM3D_TriangleFacet*> triangleFacetOfBody0_s;  /* own */
  this->divideBodyByBody (_body0, _body1,
			  segmentFacetOfBody0_s,
			  triangleFacetOfBody0_s);

  vector<WH_GM3D_SegmentFacet*> segmentFacetOfBody1_s;  /* own */
  vector<WH_GM3D_TriangleFacet*> triangleFacetOfBody1_s;  /* own */
  this->divideBodyByBody (_body1, _body0,
			  segmentFacetOfBody1_s,
			  triangleFacetOfBody1_s);

  /* collect explicit vertex points at intersection between two
     segment facets */
  vector<WH_Vector3D> explicitVertexPoint_s;
  this->collectExplicitVertexPoints 
    (explicitVertexPoint_s);

  /* change segment facets along the boundary of the other body as
     explicit */
  this->changeSegmentFacetsAsExplicit 
    (_body1,
     segmentFacetOfBody0_s);
  this->changeSegmentFacetsAsExplicit 
    (_body0,
     segmentFacetOfBody1_s);

  /* clear duplicated facets */

  vector<WH_GM3D_SegmentFacet*> remainingSegmentFacetOfBody1_s;
  this->clearDuplicatedSegmentFacets 
    (_body0, segmentFacetOfBody1_s,
     remainingSegmentFacetOfBody1_s);

  vector<WH_GM3D_TriangleFacet*> remainingTriangleFacetOfBody0_s;
  this->clearDuplicatedTriangleFacets 
    (_body1, triangleFacetOfBody0_s,
     remainingTriangleFacetOfBody0_s);
  
  vector<WH_GM3D_TriangleFacet*> remainingTriangleFacetOfBody1_s;
  this->clearDuplicatedTriangleFacets 
    (_body0, triangleFacetOfBody1_s,
     remainingTriangleFacetOfBody1_s);

  /* collect remaining facets */
  this->collectTriangleFacets (remainingTriangleFacetOfBody0_s);
  this->collectTriangleFacets (remainingTriangleFacetOfBody1_s);
  this->collectSegmentFacets (segmentFacetOfBody0_s);
  this->collectSegmentFacets (remainingSegmentFacetOfBody1_s);

  /* collect vertex points */
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = _body0->vertexPoint_s ().begin ();
       i_point != _body0->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    _resultBody->addVertexPoint (point_i);
  }
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = _body1->vertexPoint_s ().begin ();
       i_point != _body1->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    if (!WH_T_Exists (_resultBody->vertexPoint_s (), point_i)) {
      _resultBody->addVertexPoint (point_i);
    }
  }
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = explicitVertexPoint_s.begin ();
       i_point != explicitVertexPoint_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    if (!WH_T_Exists (_resultBody->vertexPoint_s (), point_i)) {
      _resultBody->addVertexPoint (point_i);
    }
  }

  _resultBody->generatePolygonFacets ();

  WH_T_Delete (segmentFacetOfBody0_s);
  WH_T_Delete (segmentFacetOfBody1_s);
  WH_T_Delete (triangleFacetOfBody0_s);
  WH_T_Delete (triangleFacetOfBody1_s);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif  
}

WH_GM3D_FacetBody* WH_GM3D_Stitcher
::body0 () const
{
  return _body0;
}

WH_GM3D_FacetBody* WH_GM3D_Stitcher
::body1 () const
{
  return _body1;
}

WH_GM3D_FacetBody* WH_GM3D_Stitcher
::resultBody () const
{
  return _resultBody;
}



/* test coverage completed */




