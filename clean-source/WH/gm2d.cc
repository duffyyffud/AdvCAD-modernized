/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d.cc : the facade module of 2-D geometry modeling library */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d.h"
#include "gm2d_facet.h"
#include "gm2d_convert.h"
#include "gm2d_setop.h"
#include "gm2d_stitch.h"
#include "polygon2d.h"



/* module procedures */

static WH_GM2D_Body* CreateBrepFromFacet 
(WH_GM2D_FacetBody* facetBody)
{
  /* PRE-CONDITION */
  WH_ASSERT(facetBody != WH_NULL);
  
  WH_CVR_LINE;

  WH_GM2D_Body* result 
    = new WH_GM2D_Body (facetBody->isRegular ());
  WH_ASSERT(result != WH_NULL);
  
  WH_GM2D_ConverterFromFacetToBrep converter (result, facetBody);
  converter.perform ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

static WH_GM2D_FacetBody* CreateFacetFromBrep 
(WH_GM2D_Body* brepBody,
 bool facetBodyIsRegular)
{
  /* PRE-CONDITION */
  WH_ASSERT(brepBody != WH_NULL);
  WH_ASSERT(brepBody->isRegular ()
	    || !facetBodyIsRegular);
  
  WH_CVR_LINE;

  WH_GM2D_FacetBody* result 
    = new WH_GM2D_FacetBody (facetBodyIsRegular);
  WH_ASSERT(result != WH_NULL);
  
  WH_GM2D_ConverterFromBrepToFacet converter (brepBody, result);
  converter.perform ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

static WH_GM2D_Body* CreateVolumeOfTwo 
(WH_GM2D_Body* body0, WH_GM2D_Body* body1,
 WH_GM2D_SetOperator::OperationType operatorType)
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(body0->isRegular ());
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(body1->isRegular ());

  WH_CVR_LINE;

  WH_GM2D_FacetBody* facetBody0 = CreateFacetFromBrep (body0, true);
  WH_GM2D_FacetBody* facetBody1 = CreateFacetFromBrep (body1, true);

  WH_GM2D_FacetBody* resultFacetBody 
    = new WH_GM2D_FacetBody (true);
  WH_ASSERT(resultFacetBody != WH_NULL);

  WH_GM2D_SetOperator setOperator 
    (operatorType, facetBody0, facetBody1, resultFacetBody);
  setOperator.perform ();

  WH_GM2D_Body* result = CreateBrepFromFacet (resultFacetBody);

  delete facetBody0;
  facetBody0 = WH_NULL;

  delete facetBody1;
  facetBody1 = WH_NULL;

  delete resultFacetBody;
  resultFacetBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

static WH_GM2D_Body* CreateBodyByStitching 
(WH_GM2D_Body* body0, WH_GM2D_Body* body1)
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(body1 != WH_NULL);

  WH_CVR_LINE;

  WH_GM2D_FacetBody* facetBody0 = CreateFacetFromBrep (body0, false);
  WH_GM2D_FacetBody* facetBody1 = CreateFacetFromBrep (body1, false);

  WH_GM2D_FacetBody* resultFacetBody 
    = new WH_GM2D_FacetBody (false);
  WH_ASSERT(resultFacetBody != WH_NULL);

  WH_GM2D_Stitcher stitcher 
    (facetBody0, facetBody1, resultFacetBody);
  stitcher.perform ();

  WH_GM2D_Body* result = CreateBrepFromFacet (resultFacetBody);

  delete facetBody0;
  facetBody0 = WH_NULL;

  delete facetBody1;
  facetBody1 = WH_NULL;

  delete resultFacetBody;
  resultFacetBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}



/* class WH_GM2D */

WH_GM2D_Body* WH_GM2D
::createDotSet 
(const vector<WH_Vector2D>& point_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < point_s.size ());

  WH_CVR_LINE;

  WH_GM2D_FacetBody* body = new WH_GM2D_FacetBody (false);
  WH_ASSERT(body!= WH_NULL);
  
  for (int iPoint = 0; iPoint < (int)point_s.size (); iPoint++) {
    WH_Vector2D point = point_s[iPoint];
    if (!WH_T_Exists (body->vertexPoint_s (), point)) {
      WH_CVR_LINE;
      body->addVertexPoint (point);
    }
  }
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM2D_Body* result = CreateBrepFromFacet (body);
  
  delete body;
  body = WH_NULL;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM2D_Body* WH_GM2D
::createWireSet 
(const vector<WH_Segment2D>& seg_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < seg_s.size ());

  WH_CVR_LINE;

  WH_GM2D_FacetBody* body = new WH_GM2D_FacetBody (false);
  WH_ASSERT(body != WH_NULL);
  
  for (int iSeg = 0; iSeg < (int)seg_s.size (); iSeg++) {
    WH_Segment2D seg = seg_s[iSeg];
    
    if (iSeg == 0) {
      WH_CVR_LINE;
      WH_ASSERT(body->bodyType () == WH_GM2D_FacetBody::EMPTY);

      /* set <seg> into empty <body> */

      body->addVertexPoint (seg.p0 ()); 
      body->addVertexPoint (seg.p1 ()); 
      WH_GM2D_SegmentFacet* facet 
	= new WH_GM2D_SegmentFacet 
	(seg.p0 (), seg.p1 (), false, false);
      WH_ASSERT(facet != WH_NULL);
      body->addSegmentFacet (facet);
    } else {
      WH_CVR_LINE;
      WH_ASSERT(body->bodyType () == WH_GM2D_FacetBody::WIRE_SET);

      /* stitch <seg> into wire set <body> */

      WH_GM2D_FacetBody* addedBody = new WH_GM2D_FacetBody (false);
      WH_ASSERT(addedBody != WH_NULL);
      
      addedBody->addVertexPoint (seg.p0 ()); 
      addedBody->addVertexPoint (seg.p1 ()); 
      WH_GM2D_SegmentFacet* facet 
	= new WH_GM2D_SegmentFacet 
	(seg.p0 (), seg.p1 (), false, false);
      WH_ASSERT(facet != WH_NULL);
      addedBody->addSegmentFacet (facet);
      
      WH_GM2D_FacetBody* resultBody = new WH_GM2D_FacetBody (false);
      WH_ASSERT(resultBody != WH_NULL);
      
      WH_GM2D_Stitcher* stitcher 
	= new WH_GM2D_Stitcher 
	(body, addedBody, resultBody);
      WH_ASSERT(stitcher != WH_NULL);
      
      stitcher->perform ();
      
      delete stitcher;
      stitcher = WH_NULL;
      
      delete addedBody;
      addedBody = WH_NULL;

      delete body;
      body = WH_NULL;
      
      /* set <body> as <resultBody> */
      body = resultBody;
    }

    WH_ASSERT(body != WH_NULL);
  }

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM2D_Body* result = CreateBrepFromFacet (body);

  delete body;
  body = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM2D_Body* WH_GM2D
::createVolume 
(const WH_Polygon2D& polygon)
{
  /* PRE-CONDITION */
  WH_ASSERT(polygon.isRegular ());

  WH_CVR_LINE;

  WH_GM2D_FacetBody* body = new WH_GM2D_FacetBody (true);
  WH_ASSERT(body != WH_NULL);
  
  for (int iEdge = 0; iEdge < polygon.nEdges (); iEdge++) {
    WH_Segment2D seg = polygon.edge (iEdge);
    WH_GM2D_SegmentFacet* facet 
      = new WH_GM2D_SegmentFacet (seg.p0 (), seg.p1 (), true, false);
    WH_ASSERT(facet != WH_NULL);
    body->addSegmentFacet (facet);
  }

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM2D_Body* result = CreateBrepFromFacet (body);

  delete body;
  body = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_GM2D
::add 
(WH_GM2D_Body* blankBody,
 WH_GM2D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(blankBody->isRegular ());
  WH_ASSERT(toolBody != WH_NULL);
  WH_ASSERT(toolBody->isRegular ());
  
  WH_CVR_LINE;

  WH_GM2D_Body* resultBody = CreateVolumeOfTwo 
    (blankBody, toolBody, WH_GM2D_SetOperator::UNION);
  
  blankBody->clear (true);
  blankBody->copyFrom (resultBody);
  
  delete toolBody;  /* DELETE */
  toolBody = WH_NULL;
  
  delete resultBody;
  resultBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(blankBody->isRegular ());
  WH_ASSERT(toolBody == WH_NULL);
#endif
}

void WH_GM2D
::subtract 
(WH_GM2D_Body* blankBody,
 WH_GM2D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(blankBody->isRegular ());
  WH_ASSERT(toolBody != WH_NULL);
  WH_ASSERT(toolBody->isRegular ());

  WH_CVR_LINE;

  WH_GM2D_Body* resultBody = CreateVolumeOfTwo 
    (blankBody, toolBody, WH_GM2D_SetOperator::SUBTRACTION);

  blankBody->clear (true);
  blankBody->copyFrom (resultBody);

  delete toolBody;  /* DELETE */
  toolBody = WH_NULL;

  delete resultBody;
  resultBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(blankBody->isRegular ());
  WH_ASSERT(toolBody == WH_NULL);
#endif
}

void WH_GM2D
::stitch 
(WH_GM2D_Body* blankBody,
 WH_GM2D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody != WH_NULL);

  WH_CVR_LINE;

  WH_GM2D_Body* resultBody = CreateBodyByStitching 
    (blankBody, toolBody);

  blankBody->clear (false);
  blankBody->copyFrom (resultBody);

  delete toolBody;  /* DELETE */
  toolBody = WH_NULL;

  delete resultBody;
  resultBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(!blankBody->isRegular ());
  WH_ASSERT(toolBody == WH_NULL);
#endif
}



/* test coverage completed */



