/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d.cc : the facade module of 3-D geometry modeling library */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d.h"
#include "gm3d_facet.h"
#include "gm3d_convert.h"
#include "gm3d_setop.h"
#include "gm3d_sheetsetop.h"
#include "gm3d_stitch.h"



/* module procedures */

static WH_GM3D_Body* CreateBrepFromFacet 
(WH_GM3D_FacetBody* facetBody)
{
  /* PRE-CONDITION */
  WH_ASSERT(facetBody != WH_NULL);
  
  WH_CVR_LINE;

  WH_GM3D_Body* result 
    = new WH_GM3D_Body (facetBody->isRegular ());
  WH_ASSERT(result != WH_NULL);
  
  WH_GM3D_ConverterFromFacetToBrep converter (result, facetBody);
  converter.perform ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

static WH_GM3D_FacetBody* CreateFacetFromBrep 
(WH_GM3D_Body* brepBody,
 bool facetBodyIsRegular)
{
  /* PRE-CONDITION */
  WH_ASSERT(brepBody != WH_NULL);
  WH_ASSERT(brepBody->isRegular ()
	    || !facetBodyIsRegular);
  
  WH_CVR_LINE;

  WH_GM3D_FacetBody* result 
    = new WH_GM3D_FacetBody (facetBodyIsRegular);
  WH_ASSERT(result != WH_NULL);
  
  WH_GM3D_ConverterFromBrepToFacet converter (brepBody, result);
  converter.perform ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

static WH_GM3D_Body* CreateVolumeOfTwo 
(WH_GM3D_Body* body0, WH_GM3D_Body* body1,
 WH_GM3D_SetOperator::OperationType operatorType)
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(body0->isRegular ());
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(body1->isRegular ());

  WH_CVR_LINE;

  WH_GM3D_FacetBody* facetBody0 = CreateFacetFromBrep (body0, true);
  WH_GM3D_FacetBody* facetBody1 = CreateFacetFromBrep (body1, true);

  WH_GM3D_FacetBody* resultFacetBody 
    = new WH_GM3D_FacetBody (true);
  WH_ASSERT(resultFacetBody != WH_NULL);

  WH_GM3D_SetOperator setOperator 
    (operatorType, facetBody0, facetBody1, resultFacetBody);
  setOperator.perform ();

  WH_GM3D_Body* result = CreateBrepFromFacet (resultFacetBody);

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

static WH_GM3D_Body* CreateSheetOfTwo 
(WH_GM3D_Body* body0, WH_GM3D_Body* body1,
 WH_GM3D_SheetSetOperator::OperationType operatorType)
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(!body0->isRegular ());
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(!body1->isRegular ());

  WH_CVR_LINE;

  WH_GM3D_FacetBody* facetBody0 = CreateFacetFromBrep (body0, false);
  WH_GM3D_FacetBody* facetBody1 = CreateFacetFromBrep (body1, false);

  WH_GM3D_FacetBody* resultFacetBody 
    = new WH_GM3D_FacetBody (false);
  WH_ASSERT(resultFacetBody != WH_NULL);

  WH_GM3D_SheetSetOperator setOperator 
    (operatorType, facetBody0, facetBody1, resultFacetBody);
  setOperator.perform ();

  WH_GM3D_Body* result = CreateBrepFromFacet (resultFacetBody);

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

static WH_GM3D_Body* CreateBodyByStitching 
(WH_GM3D_Body* body0, WH_GM3D_Body* body1)
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(body1 != WH_NULL);

  WH_CVR_LINE;

  WH_GM3D_FacetBody* facetBody0 = CreateFacetFromBrep (body0, false);
  WH_GM3D_FacetBody* facetBody1 = CreateFacetFromBrep (body1, false);

  WH_GM3D_FacetBody* resultFacetBody 
    = new WH_GM3D_FacetBody (false);
  WH_ASSERT(resultFacetBody != WH_NULL);

  WH_GM3D_Stitcher stitcher 
    (facetBody0, facetBody1, resultFacetBody);
  stitcher.perform ();

  WH_GM3D_Body* result = CreateBrepFromFacet (resultFacetBody);

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



/* class WH_GM3D */

WH_GM3D_Body* WH_GM3D
::createDotSet 
(const vector<WH_Vector3D>& point_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < point_s.size ());

  WH_CVR_LINE;

  auto body = make_unique<WH_GM3D_FacetBody>(false);
  WH_ASSERT(body != nullptr);
  
  for (int iPoint = 0; iPoint < (int)point_s.size (); iPoint++) {
    WH_Vector3D point = point_s[iPoint];
    if (!WH_T_Exists (body->vertexPoint_s (), point)) {
      WH_CVR_LINE;
      body->addVertexPoint (point);
    }
  }
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM3D_Body* result = CreateBrepFromFacet (body.get());
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::createWireSet 
(const vector<WH_Segment3D>& seg_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < seg_s.size ());

  WH_CVR_LINE;

  WH_GM3D_FacetBody* body = new WH_GM3D_FacetBody (false);
  WH_ASSERT(body != WH_NULL);
  
  for (int iSeg = 0; iSeg < (int)seg_s.size (); iSeg++) {
    WH_Segment3D seg = seg_s[iSeg];
    
    if (iSeg == 0) {
      WH_CVR_LINE;
      WH_ASSERT(body->bodyType () == WH_GM3D_FacetBody::EMPTY);

      /* set <seg> into empty <body> */

      body->addVertexPoint (seg.p0 ()); 
      body->addVertexPoint (seg.p1 ()); 
      WH_GM3D_SegmentFacet* facet 
	= new WH_GM3D_SegmentFacet (seg.p0 (), seg.p1 (), false); 
      WH_ASSERT(facet != WH_NULL);
      body->addSegmentFacet (facet);
    } else {
      WH_CVR_LINE;
      WH_ASSERT(body->bodyType () == WH_GM3D_FacetBody::WIRE_SET);

      /* stitch <seg> into wire set <body> */

      WH_GM3D_FacetBody* addedBody = new WH_GM3D_FacetBody (false);
      WH_ASSERT(addedBody != WH_NULL);
      
      addedBody->addVertexPoint (seg.p0 ()); 
      addedBody->addVertexPoint (seg.p1 ()); 
      WH_GM3D_SegmentFacet* facet 
	= new WH_GM3D_SegmentFacet (seg.p0 (), seg.p1 (), false);
      WH_ASSERT(facet != WH_NULL);
      addedBody->addSegmentFacet (facet);
      
      WH_GM3D_FacetBody* resultBody = new WH_GM3D_FacetBody (false);
      WH_ASSERT(resultBody != WH_NULL);
      
      WH_GM3D_Stitcher* stitcher 
	= new WH_GM3D_Stitcher 
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

  WH_GM3D_Body* result = CreateBrepFromFacet (body);

  delete body;
  body = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::createSheet 
(const WH_Polygon3D& polygon)
{
  /* PRE-CONDITION */
  WH_ASSERT(polygon.isRegular ());

  WH_CVR_LINE;

  auto body = make_unique<WH_GM3D_FacetBody>(false);
  WH_ASSERT(body != nullptr);
  
  for (int iVertex = 0; iVertex < polygon.nVertexs (); iVertex++) {
    WH_Vector3D point = polygon.vertex (iVertex);
    body->addVertexPoint (point);
  }

  for (int iEdge = 0; iEdge < polygon.nEdges (); iEdge++) {
    WH_Segment3D seg = polygon.edge (iEdge);
    WH_GM3D_SegmentFacet* facet 
      = new WH_GM3D_SegmentFacet (seg.p0 (), seg.p1 (), false);
    WH_ASSERT(facet != WH_NULL);
    body->addSegmentFacet (facet);
  }

  WH_GM3D_PolygonFacet* facet 
    = new WH_GM3D_PolygonFacet 
    (polygon.plane (), polygon.polygon2D (), false, false);
  WH_ASSERT(facet != WH_NULL);
  body->addPolygonFacet (facet);

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM3D_Body* result = CreateBrepFromFacet (body.get());

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::createVolume
(const vector<WH_Vector3D>& outsideNormal_s,
 const vector<WH_Polygon3D>& polygon_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(outsideNormal_s.size () == polygon_s.size ());

  WH_CVR_LINE;

  auto body = make_unique<WH_GM3D_FacetBody>(true);
  WH_ASSERT(body != nullptr);

  int nPolys = (int)polygon_s.size ();
  for (int iPoly = 0; iPoly < nPolys; iPoly++) {
    WH_Polygon3D poly = polygon_s[iPoly];
    WH_Vector3D outsideNormal = outsideNormal_s[iPoly].normalize ();

    WH_Plane3D plane (poly.vertex (0), 
		      poly.vertex (1), 
		      poly.vertex (2));
    if (WH_ne (plane.normal (), outsideNormal)) {
      WH_CVR_LINE;
      plane = WH_Plane3D (poly.vertex (0), 
			  poly.vertex (2), 
			  poly.vertex (1));
    }
    WH_ASSERT(WH_eq (plane.normal (), outsideNormal));
    
    vector<WH_Vector2D> param_s;
    for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
      param_s.push_back (plane.parameterAt (poly.vertex (iVertex)));
    }
    WH_Polygon2D paramPoly (param_s);

    WH_GM3D_PolygonFacet* facet 
      = new WH_GM3D_PolygonFacet 
      (plane, paramPoly, false, true);
    WH_ASSERT(facet != WH_NULL);
    body->addPolygonFacet (facet);
  }  

  body->generateTriangleFacets ();

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM3D_Body* result = CreateBrepFromFacet (body.get());

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::createBox 
(const WH_Vector3D& origin,
 const WH_Vector3D& extent)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (WH_Vector3D::zero (), extent));

  WH_CVR_LINE;

  double x = extent.x;
  double y = extent.y;
  double z = extent.z;

  vector<WH_Vector3D> outsideNormal_s;
  vector<WH_Polygon3D> polygon_s;

  {
    outsideNormal_s.push_back (WH_Vector3D (0, 0, -1));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (0, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (x, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (x, y, 0) + origin);
    point_s.push_back (WH_Vector3D (0, y, 0) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  {
    outsideNormal_s.push_back (WH_Vector3D (0, 0, 1));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (0, 0, z) + origin);
    point_s.push_back (WH_Vector3D (x, 0, z) + origin);
    point_s.push_back (WH_Vector3D (x, y, z) + origin);
    point_s.push_back (WH_Vector3D (0, y, z) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  {
    outsideNormal_s.push_back (WH_Vector3D (-1, 0, 0));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (0, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (0, y, 0) + origin);
    point_s.push_back (WH_Vector3D (0, y, z) + origin);
    point_s.push_back (WH_Vector3D (0, 0, z) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  {
    outsideNormal_s.push_back (WH_Vector3D (1, 0, 0));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (x, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (x, y, 0) + origin);
    point_s.push_back (WH_Vector3D (x, y, z) + origin);
    point_s.push_back (WH_Vector3D (x, 0, z) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  {
    outsideNormal_s.push_back (WH_Vector3D (0, -1, 0));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (0, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (x, 0, 0) + origin);
    point_s.push_back (WH_Vector3D (x, 0, z) + origin);
    point_s.push_back (WH_Vector3D (0, 0, z) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  {
    outsideNormal_s.push_back (WH_Vector3D (0, 1, 0));
    vector<WH_Vector3D> point_s;
    point_s.push_back (WH_Vector3D (0, y, 0) + origin);
    point_s.push_back (WH_Vector3D (x, y, 0) + origin);
    point_s.push_back (WH_Vector3D (x, y, z) + origin);
    point_s.push_back (WH_Vector3D (0, y, z) + origin);
    polygon_s.push_back (WH_Polygon3D (point_s));
  }

  WH_GM3D_Body* result = WH_GM3D::createVolume 
    (outsideNormal_s, polygon_s);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::extrude 
(WH_GM3D_Body* profileBody,
 const WH_Vector3D& offset)
{
  /* PRE-CONDITION */
  WH_ASSERT(profileBody != WH_NULL);
  WH_ASSERT(profileBody->bodyType () == WH_GM3D_Body::SHEET_SET);
  WH_ASSERT(profileBody->face_s ().size () == 1);
  WH_ASSERT(WH_ne (offset, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  WH_GM3D_FacetBody* facetProfileBody 
    = CreateFacetFromBrep (profileBody, false);
  WH_ASSERT(facetProfileBody->polygonFacet_s ().size () == 1);

  WH_GM3D_PolygonFacet* profileFacet 
    = facetProfileBody->polygonFacet_s ()[0];
  WH_ASSERT(!profileFacet->frontSideIsInsideVolume ());
  WH_ASSERT(!profileFacet->backSideIsInsideVolume ());
  WH_ASSERT(WH_isParallel (profileFacet->plane ().normal (), offset));

  WH_GM3D_FacetBody* body = new WH_GM3D_FacetBody (true);
  WH_ASSERT(body != WH_NULL);

  /* bottom face */
  {
    WH_Plane3D plane = profileFacet->plane ();

    bool frontSideIsInsideVolume = false;
    bool backSideIsInsideVolume = true;
    if (WH_lt (0, WH_scalarProduct (plane.normal (), offset))) {
      WH_CVR_LINE;
      frontSideIsInsideVolume = true;
      backSideIsInsideVolume = false;
    }

    WH_GM3D_PolygonFacet* facet 
      = new WH_GM3D_PolygonFacet 
      (plane, profileFacet->outerLoopParameterPolygon (), 
       frontSideIsInsideVolume, backSideIsInsideVolume);
    WH_ASSERT(facet != WH_NULL);
    
    for (int iPoly = 0; 
	 iPoly < (int)profileFacet->innerLoopParameterPolygon_s ().size ();
	 iPoly++) {
      WH_Polygon2D poly = profileFacet->innerLoopParameterPolygon_s ()[iPoly];
      facet->addInnerLoop (poly);
    }

    body->addPolygonFacet (facet);
  }

  /* top face */
  {
    WH_Polygon3D outerProfilePoly = profileFacet->outerLoopPolygon ();
    vector<WH_Vector3D> vertex_s;
    for (int iVertex = 0; iVertex < outerProfilePoly.nVertexs (); iVertex++) {
      vertex_s.push_back (outerProfilePoly.vertex (iVertex) + offset);
    }
    WH_Polygon3D outerPoly (vertex_s);
    WH_Plane3D plane = outerPoly.plane ();

    vector<WH_Vector2D> param_s;
    for (int iVertex = 0; iVertex < outerPoly.nVertexs (); iVertex++) {
      param_s.push_back (plane.parameterAt (outerPoly.vertex (iVertex)));
    }
    WH_Polygon2D outerParamPoly (param_s);

    bool frontSideIsInsideVolume = true;
    bool backSideIsInsideVolume = false;
    if (WH_lt (0, WH_scalarProduct (plane.normal (), offset))) {
      WH_CVR_LINE;
      frontSideIsInsideVolume = false;
      backSideIsInsideVolume = true;
    }

    WH_GM3D_PolygonFacet* facet 
      = new WH_GM3D_PolygonFacet 
      (plane, outerParamPoly, 
       frontSideIsInsideVolume, backSideIsInsideVolume);
    WH_ASSERT(facet != WH_NULL);

    for (int iPoly = 0; 
	 iPoly < (int)profileFacet->innerLoopPolygon_s ().size ();
	 iPoly++) {
      WH_Polygon3D profilePoly = profileFacet->innerLoopPolygon_s ()[iPoly];
      vector<WH_Vector3D> vertex_s;
      for (int iVertex = 0; iVertex < profilePoly.nVertexs (); iVertex++) {
	vertex_s.push_back (profilePoly.vertex (iVertex) + offset);
      }
      WH_Polygon3D poly (vertex_s);
      vector<WH_Vector2D> param_s;
      for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	param_s.push_back (plane.parameterAt (poly.vertex (iVertex)));
      }
      WH_Polygon2D paramPoly (param_s);
      facet->addInnerLoop (paramPoly);
    }

    body->addPolygonFacet (facet);
  }

  /* side faces */
  {
    WH_Plane3D basePlane = profileFacet->outerLoopPolygon ().plane ();

    {
      WH_Polygon3D profilePoly = profileFacet->outerLoopPolygon ();
      WH_Polygon2D paramProfilePoly 
	= profileFacet->outerLoopParameterPolygon ();

      bool frontSideIsInsideVolume = false;
      bool backSideIsInsideVolume = true;
      if (paramProfilePoly.isClockWise ()) {
	WH_CVR_LINE;
	if (WH_lt (0, WH_scalarProduct (basePlane.normal (), offset))) {
	  WH_CVR_LINE;
	  frontSideIsInsideVolume = true;
	  backSideIsInsideVolume = false;
	}
      } else {
	WH_CVR_LINE;
	if (WH_lt (WH_scalarProduct (basePlane.normal (), offset), 0)) {
	  WH_CVR_LINE;
	  frontSideIsInsideVolume = true;
	  backSideIsInsideVolume = false;
	}
      }

      int nVertexs = profilePoly.nVertexs ();
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	WH_Vector3D vertex0 = profilePoly.vertex (iVertex);
	WH_Vector3D vertex1 = profilePoly.vertex ((iVertex + 1) % nVertexs);
	vector<WH_Vector3D> vertex_s;
	vertex_s.push_back (vertex0);
	vertex_s.push_back (vertex1);
	vertex_s.push_back (vertex1 + offset);
	vertex_s.push_back (vertex0 + offset);
	WH_Polygon3D poly (vertex_s);
	WH_Plane3D plane = poly.plane ();
	vector<WH_Vector2D> param_s;
	for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	  param_s.push_back (plane.parameterAt (poly.vertex (iVertex)));
	}
	WH_Polygon2D paramPoly (param_s);
	
	WH_GM3D_PolygonFacet* facet 
	  = new WH_GM3D_PolygonFacet 
	  (plane, paramPoly, 
	   frontSideIsInsideVolume, backSideIsInsideVolume);
	WH_ASSERT(facet != WH_NULL);
	
	body->addPolygonFacet (facet);
      }
    }
  
    for (int iPoly = 0; 
	 iPoly < (int)profileFacet->innerLoopPolygon_s ().size ();
	 iPoly++) {
      WH_Polygon3D profilePoly = profileFacet->innerLoopPolygon_s ()[iPoly];
      WH_Polygon2D paramProfilePoly 
	= profileFacet->innerLoopParameterPolygon_s ()[iPoly];

      bool frontSideIsInsideVolume = true;
      bool backSideIsInsideVolume = false;
      if (paramProfilePoly.isClockWise ()) {
	WH_CVR_LINE;
	if (WH_lt (0, WH_scalarProduct (basePlane.normal (), offset))) {
	  WH_CVR_LINE;
	  frontSideIsInsideVolume = false;
	  backSideIsInsideVolume = true;
	}
      } else {
	WH_CVR_LINE;
	if (WH_lt (WH_scalarProduct (basePlane.normal (), offset), 0)) {
	  WH_CVR_LINE;
	  frontSideIsInsideVolume = false;
	  backSideIsInsideVolume = true;
	}
      }

      int nVertexs = profilePoly.nVertexs ();
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	WH_Vector3D vertex0 = profilePoly.vertex (iVertex);
	WH_Vector3D vertex1 = profilePoly.vertex ((iVertex + 1) % nVertexs);
	vector<WH_Vector3D> vertex_s;
	vertex_s.push_back (vertex0);
	vertex_s.push_back (vertex1);
	vertex_s.push_back (vertex1 + offset);
	vertex_s.push_back (vertex0 + offset);
	WH_Polygon3D poly (vertex_s);
	WH_Plane3D plane = poly.plane ();
	vector<WH_Vector2D> param_s;
	for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	  param_s.push_back (plane.parameterAt (poly.vertex (iVertex)));
	}
	WH_Polygon2D paramPoly (param_s);
	
	WH_GM3D_PolygonFacet* facet 
	  = new WH_GM3D_PolygonFacet 
	  (plane, paramPoly, 
	   frontSideIsInsideVolume, backSideIsInsideVolume);
	WH_ASSERT(facet != WH_NULL);
	
	body->addPolygonFacet (facet);
      }
    }
  }

  body->generateTriangleFacets ();

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM3D_Body* result = CreateBrepFromFacet (body);

  delete body;
  body = WH_NULL;

  delete facetProfileBody;
  facetProfileBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_GM3D_Body* WH_GM3D
::revolve 
(WH_GM3D_Body* profileBody,
 const WH_Line3D& axis, int nDivisions)
{
  /* PRE-CONDITION */
  WH_ASSERT(profileBody != WH_NULL);
  WH_ASSERT(profileBody->bodyType () == WH_GM3D_Body::SHEET_SET);
  WH_ASSERT(profileBody->face_s ().size () == 1);
  WH_ASSERT(4 <= nDivisions);

  WH_CVR_LINE;

  WH_GM3D_FacetBody* facetProfileBody 
    = CreateFacetFromBrep (profileBody, false);
  WH_ASSERT(facetProfileBody->polygonFacet_s ().size () == 1);

  WH_GM3D_PolygonFacet* profileFacet 
    = facetProfileBody->polygonFacet_s ()[0];
  WH_ASSERT(!profileFacet->frontSideIsInsideVolume ());
  WH_ASSERT(!profileFacet->backSideIsInsideVolume ());
  WH_ASSERT(profileFacet->innerLoopParameterPolygon_s ().size () == 0);

  /* plane of <profileFacet> contains <axis> */

  WH_GM3D_FacetBody* body = new WH_GM3D_FacetBody (true);
  WH_ASSERT(body != WH_NULL);

  WH_Polygon3D profilePoly = profileFacet->outerLoopPolygon ();

  /* find any vertex point off <axis> into <anyPointOffAxis> */
  bool anyPointOffAxisIsFound = false;
  WH_Vector3D anyPointOffAxis;
  for (int iVertex = 0; iVertex < profilePoly.nVertexs (); iVertex++) {
    WH_Vector3D vertex = profilePoly.vertex (iVertex);
    if (!axis.contains (vertex)) {
      WH_CVR_LINE;
      anyPointOffAxisIsFound = true;
      anyPointOffAxis = vertex;
      break;
    }
  }
  WH_ASSERT(anyPointOffAxisIsFound);
  WH_ASSERT(!axis.contains (anyPointOffAxis));

  /* <axis.direction ()> is Z axis direction */
  /* find Y axis direction and compare it with normal of <profilePoly> */
  WH_Vector3D projectedAnyPoint = axis.projectedPoint (anyPointOffAxis);
  WH_Vector3D xAxisDir = anyPointOffAxis - projectedAnyPoint;
  WH_Vector3D zAxisDir = axis.direction ();
  WH_Vector3D yAxisDir = WH_vectorProduct (zAxisDir, xAxisDir);
  bool yAxisIsSameDirectionAsProfilePoly 
    = WH_lt (0, WH_scalarProduct (yAxisDir, profilePoly.plane ().normal ()));

  bool directionIsReverse = false;
  WH_Polygon2D profileParamPoly = profileFacet->outerLoopParameterPolygon ();
  if (profileParamPoly.isClockWise ()) {
    WH_CVR_LINE;
    if (yAxisIsSameDirectionAsProfilePoly) {
      WH_CVR_LINE;
      directionIsReverse = false;
    } else {
      WH_CVR_LINE;
      directionIsReverse = true;
    }
  } else {
    WH_CVR_LINE;
    if (yAxisIsSameDirectionAsProfilePoly) {
      WH_CVR_LINE;
      directionIsReverse = true;
    } else {
      WH_CVR_LINE;
      directionIsReverse = false;
    }
  }

  int nVertexs = profilePoly.nVertexs ();
  for (int iVertex = 0; iVertex < profilePoly.nVertexs (); iVertex++) {
    WH_Vector3D vertex0 = profilePoly.vertex (iVertex);
    WH_Vector3D vertex1 = profilePoly.vertex ((iVertex + 1) % nVertexs);
    WH_Vector3D projectedVertex0 = axis.projectedPoint (vertex0);
    WH_Vector3D projectedVertex1 = axis.projectedPoint (vertex1);

    if (axis.contains (vertex0) && axis.contains (vertex1)) {
      WH_CVR_LINE;
      /* edge along axis of rotation */
      /* nothing */

    } else if (WH_isPerpendicular (axis.direction (), vertex1 - vertex0)) {
      WH_CVR_LINE;

      WH_ASSERT(WH_eq (projectedVertex0, projectedVertex1));
      WH_ASSERT(WH_ne (vertex0, projectedVertex0)
		|| WH_ne (vertex1, projectedVertex1));

      WH_Vector3D innerPoint = vertex0;
      WH_Vector3D outerPoint = vertex1;
      bool frontSideIsInsideVolume = true;
      bool backSideIsInsideVolume = false;
      WH_Vector3D projectedPoint = projectedVertex0;
      if (WH_lt (WH_distance (vertex1, projectedVertex1),
		 WH_distance (vertex0, projectedVertex0))) {
	WH_CVR_LINE;
	WH_swap (innerPoint, outerPoint);
	frontSideIsInsideVolume = !frontSideIsInsideVolume;
	backSideIsInsideVolume = !backSideIsInsideVolume;
      }
      if (directionIsReverse) {
	WH_CVR_LINE;
	frontSideIsInsideVolume = !frontSideIsInsideVolume;
	backSideIsInsideVolume = !backSideIsInsideVolume;
      }
      
      vector<WH_Vector3D> outerVertex_s;
      for (int iDiv = 0; iDiv < nDivisions; iDiv++) {
	double angle = (M_PI * 2 / nDivisions) * iDiv; 
	WH_Matrix3D m = WH_Matrix3D::generalAxisRotation
	  (axis.direction (), angle);
	WH_Vector3D v = projectedPoint 
	  + (outerPoint - projectedPoint) * m;
	outerVertex_s.push_back (v);
      }
      WH_Polygon3D outerPoly (outerVertex_s);
      
      WH_Plane3D plane = outerPoly.plane ();
      
      vector<WH_Vector2D> outerParam_s;
      for (int iVertex = 0; iVertex < outerPoly.nVertexs (); iVertex++) {
	WH_Vector2D param = plane.parameterAt (outerPoly.vertex (iVertex));
	outerParam_s.push_back (param);
      }
      WH_Polygon2D outerParamPoly (outerParam_s);
      
      WH_GM3D_PolygonFacet* facet 
	= new WH_GM3D_PolygonFacet 
	(plane, outerParamPoly, 
	 frontSideIsInsideVolume, backSideIsInsideVolume);
      WH_ASSERT(facet != WH_NULL);
      
      if (WH_ne (innerPoint, projectedPoint)) {
	vector<WH_Vector3D> innerVertex_s;
	for (int iDiv = 0; iDiv < nDivisions; iDiv++) {
	  double angle = (M_PI * 2 / nDivisions) * iDiv; 
	  WH_Matrix3D m = WH_Matrix3D::generalAxisRotation
	    (axis.direction (), angle);
	  WH_Vector3D v = projectedPoint 
	    + (innerPoint - projectedPoint) * m;
	  innerVertex_s.push_back (v);
	}
	WH_Polygon3D innerPoly (innerVertex_s);
	
	vector<WH_Vector2D> innerParam_s;
	for (int iVertex = 0; iVertex < innerPoly.nVertexs (); iVertex++) {
	  WH_Vector2D param = plane.parameterAt (innerPoly.vertex (iVertex));
	  innerParam_s.push_back (param);
	}
	WH_Polygon2D innerParamPoly (innerParam_s);
	
	facet->addInnerLoop (innerParamPoly);
      }
      
      body->addPolygonFacet (facet);

    } else {
      WH_CVR_LINE;

      bool frontSideIsInsideVolume = false;
      bool backSideIsInsideVolume = true;
      if (directionIsReverse) {
	WH_CVR_LINE;
	frontSideIsInsideVolume = !frontSideIsInsideVolume;
	backSideIsInsideVolume = !backSideIsInsideVolume;
      }

      for (int iDiv = 0; iDiv < nDivisions; iDiv++) {
	double angle0 = (M_PI * 2 / nDivisions) * iDiv; 
	double angle1 = (M_PI * 2 / nDivisions) * (iDiv + 1); 
	WH_Matrix3D m0 = WH_Matrix3D::generalAxisRotation
	  (axis.direction (), angle0);
	WH_Matrix3D m1 = WH_Matrix3D::generalAxisRotation
	  (axis.direction (), angle1);

	WH_Vector3D v0_0 = projectedVertex0 +
	  (vertex0 - projectedVertex0) * m0;
	WH_Vector3D v0_1 = projectedVertex0 +
	  (vertex0 - projectedVertex0) * m1;
	WH_Vector3D v1_0 = projectedVertex1 +
	  (vertex1 - projectedVertex1) * m0;
	WH_Vector3D v1_1 = projectedVertex1 +
	  (vertex1 - projectedVertex1) * m1;
	WH_ASSERT(WH_ne (v0_0, v0_1) || WH_ne (v1_0, v1_1));

	vector<WH_Vector3D> vertex_s;
	vertex_s.push_back (v0_0);
	if (WH_ne (v0_0, v0_1)) {
	  WH_CVR_LINE;
	  vertex_s.push_back (v0_1);
	}
	if (WH_ne (v1_0, v1_1)) {
	  WH_CVR_LINE;
	  vertex_s.push_back (v1_1);
	}
	vertex_s.push_back (v1_0);
	WH_Polygon3D poly (vertex_s);
	WH_Plane3D plane = poly.plane ();
	vector<WH_Vector2D> param_s;
	for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	  param_s.push_back (plane.parameterAt (poly.vertex (iVertex)));
	}
	WH_Polygon2D paramPoly (param_s);
	
	WH_GM3D_PolygonFacet* facet 
	  = new WH_GM3D_PolygonFacet 
	  (plane, paramPoly, 
	   frontSideIsInsideVolume, backSideIsInsideVolume);
	WH_ASSERT(facet != WH_NULL);
	
	body->addPolygonFacet (facet);
      }
    }
  }

  body->generateTriangleFacets ();

#ifndef WH_PRE_ONLY
  WH_ASSERT(body->assureInvariant ());
#endif

  WH_GM3D_Body* result = CreateBrepFromFacet (body);

  delete body;
  body = WH_NULL;

  delete facetProfileBody;
  facetProfileBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_GM3D
::add 
(WH_GM3D_Body* blankBody,
 WH_GM3D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody != WH_NULL);
  WH_ASSERT((blankBody->isRegular () && toolBody->isRegular ())
	    || (!blankBody->isRegular () && !toolBody->isRegular ()));
  
  WH_CVR_LINE;

  
  WH_GM3D_Body* resultBody = WH_NULL;
  if (blankBody->isRegular () && toolBody->isRegular ()) {
    WH_CVR_LINE;
    resultBody = CreateVolumeOfTwo 
      (blankBody, toolBody, WH_GM3D_SetOperator::UNION);
    blankBody->clear (true);
    blankBody->copyFrom (resultBody);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(!blankBody->isRegular () && !toolBody->isRegular ());
    resultBody = CreateSheetOfTwo 
      (blankBody, toolBody, WH_GM3D_SheetSetOperator::UNION);
    blankBody->clear (false);
    blankBody->copyFrom (resultBody);
  }
  
  delete toolBody;  /* DELETE */
  toolBody = WH_NULL;
  
  delete resultBody;
  resultBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody == WH_NULL);
#endif
}

void WH_GM3D
::subtract 
(WH_GM3D_Body* blankBody,
 WH_GM3D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody != WH_NULL);
  WH_ASSERT((blankBody->isRegular () && toolBody->isRegular ())
	    || (!blankBody->isRegular () && !toolBody->isRegular ()));

  WH_CVR_LINE;

  WH_GM3D_Body* resultBody = WH_NULL;
  if (blankBody->isRegular () && toolBody->isRegular ()) {
    WH_CVR_LINE;
    resultBody = CreateVolumeOfTwo 
      (blankBody, toolBody, WH_GM3D_SetOperator::SUBTRACTION);
    blankBody->clear (true);
    blankBody->copyFrom (resultBody);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(!blankBody->isRegular () && !toolBody->isRegular ());
    resultBody = CreateSheetOfTwo 
      (blankBody, toolBody, WH_GM3D_SheetSetOperator::SUBTRACTION);
    blankBody->clear (false);
    blankBody->copyFrom (resultBody);
  }

  delete toolBody;  /* DELETE */
  toolBody = WH_NULL;

  delete resultBody;
  resultBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody == WH_NULL);
#endif
}

void WH_GM3D
::stitch 
(WH_GM3D_Body* blankBody,
 WH_GM3D_Body* toolBody  /* DELETE */)
{
  /* PRE-CONDITION */
  WH_ASSERT(blankBody != WH_NULL);
  WH_ASSERT(toolBody != WH_NULL);

  WH_CVR_LINE;

  WH_GM3D_Body* resultBody = CreateBodyByStitching 
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






