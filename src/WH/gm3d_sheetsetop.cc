/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_sheetsetop.cc : sheet Boolean set operation module */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_sheetsetop.h"
#include "connector2d.h"
#include "gm2d_setop.h"



/* class WH_GM3D_SheetSetOperator */

WH_GM3D_SheetSetOperator
::WH_GM3D_SheetSetOperator 
(OperationType operationType,
 WH_GM3D_FacetBody* body0,
 WH_GM3D_FacetBody* body1,
 WH_GM3D_FacetBody* resultBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(!body0->isRegular ());
  WH_ASSERT(body0->bodyType () == WH_GM3D_FacetBody::SHEET_SET);
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(!body1->isRegular ());
  WH_ASSERT(body1->bodyType () == WH_GM3D_FacetBody::SHEET_SET);
  WH_ASSERT(resultBody != WH_NULL);
  WH_ASSERT(!resultBody->isRegular ());
  WH_ASSERT(resultBody->bodyType () == WH_GM3D_FacetBody::EMPTY);
  WH_ASSERT(body0 != body1);
  WH_ASSERT(resultBody != body0);
  WH_ASSERT(resultBody != body1);
  
  WH_CVR_LINE;
  
  _operationType = operationType;
  _body0 = body0;
  _body1 = body1;
  _resultBody = resultBody;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM3D_SheetSetOperator
::~WH_GM3D_SheetSetOperator () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_SheetSetOperator
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body0 () != WH_NULL);
  WH_ASSERT(!this->body0 ()->isRegular ());
  WH_ASSERT(this->body0 ()->bodyType () 
	    == WH_GM3D_FacetBody::SHEET_SET);
  WH_ASSERT(this->body1 () != WH_NULL);
  WH_ASSERT(!this->body1 ()->isRegular ());
  WH_ASSERT(this->body1 ()->bodyType () 
	    == WH_GM3D_FacetBody::SHEET_SET);
  WH_ASSERT(this->resultBody () != WH_NULL);
  WH_ASSERT(!this->resultBody ()->isRegular ());
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY
	    || this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::SHEET_SET);
  WH_ASSERT(this->body0 () != this->body1 ());
  WH_ASSERT(this->resultBody () != this->body0 ());
  WH_ASSERT(this->resultBody () != this->body1 ());

  return true;
}

bool WH_GM3D_SheetSetOperator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

WH_GM2D_FacetBody* WH_GM3D_SheetSetOperator
::createParameterFacetBody 
(WH_GM3D_FacetBody* body,
 const WH_Plane3D& basePlane)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  /* Sheets of <body> must be separated each other. */

  WH_CVR_LINE;

  WH_GM2D_FacetBody* result = new WH_GM2D_FacetBody (true);
  WH_ASSERT(result != WH_NULL);

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = body->polygonFacet_s ().begin ();
       i_facet != body->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    WH_ASSERT(WH_eq (facet_i->plane (), basePlane)
	      || WH_isReverse (facet_i->plane (), basePlane));

    {
      WH_Polygon3D poly = facet_i->outerLoopPolygon ();

      /* facets of outer loop are oriented toward outside */

      int nVertexs = poly.nVertexs ();
      vector<WH_Vector2D> param_s;
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	param_s.push_back 
	  (basePlane.parameterAt (poly.vertex (iVertex)));
      }
      WH_Polygon2D paramPoly (param_s);
      bool polyIsClockWise = paramPoly.isClockWise ();

      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	WH_Vector2D param0 = paramPoly.vertex (iVertex);
	WH_Vector2D param1 = paramPoly.vertex ((iVertex + 1) % nVertexs);
	WH_GM2D_SegmentFacet* paramFacet = WH_NULL;
	if (polyIsClockWise) {
	  WH_CVR_LINE;
	  paramFacet = new WH_GM2D_SegmentFacet 
	    (param0, param1, false, true);
	} else {
	  WH_CVR_LINE;
	  paramFacet = new WH_GM2D_SegmentFacet 
	    (param0, param1, true, false);
	}
	WH_ASSERT(paramFacet != WH_NULL);
	result->addSegmentFacet (paramFacet);
      }
    }
    
    for (vector<WH_Polygon3D>::const_iterator 
	   i_poly = facet_i->innerLoopPolygon_s ().begin ();
	 i_poly != facet_i->innerLoopPolygon_s ().end ();
	 i_poly++) {
      WH_Polygon3D poly_i = (*i_poly);
      
      /* facets of inner loops are oriented toward inside */

      int nVertexs = poly_i.nVertexs ();
      vector<WH_Vector2D> param_s;
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	param_s.push_back 
	  (basePlane.parameterAt (poly_i.vertex (iVertex)));
      }
      WH_Polygon2D paramPoly (param_s);
      bool polyIsClockWise = paramPoly.isClockWise ();

      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	WH_Vector2D param0 = paramPoly.vertex (iVertex);
	WH_Vector2D param1 = paramPoly.vertex ((iVertex + 1) % nVertexs);
	WH_GM2D_SegmentFacet* paramFacet = WH_NULL;
	if (polyIsClockWise) {
	  WH_CVR_LINE;
	  paramFacet = new WH_GM2D_SegmentFacet 
	    (param0, param1, true, false);
	} else {
	  WH_CVR_LINE;
	  paramFacet= new WH_GM2D_SegmentFacet 
	    (param0, param1, false, true);
	}
	WH_ASSERT(paramFacet != WH_NULL);
	result->addSegmentFacet (paramFacet);
      }
    }
  }  

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

void WH_GM3D_SheetSetOperator
::makeResultBody 
(WH_GM2D_FacetBody* body,
 const WH_Plane3D& basePlane)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);

  WH_CVR_LINE;

  if (body->bodyType () == WH_GM2D_FacetBody::VOLUME) {
    WH_CVR_LINE;

    WH_CNCT2D_SegmentConnector connector;

    for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	   i_facet = body->segmentFacet_s ().begin ();
	 i_facet != body->segmentFacet_s ().end ();
	 i_facet++) {
      WH_GM2D_SegmentFacet* facet_i = (*i_facet);
      WH_CNCT2D_SegmentNode* node 
	= new WH_CNCT2D_SegmentNode (facet_i->segment ());
      connector.addNode (node);
    }

    connector.connect ();
    connector.identifyClusters ();
    connector.sortClusters ();
    connector.identifyLoops ();

    vector<WH_GM2D_SegmentFacet*> newFacet_s;
    for (vector<WH_CNCT_Cluster*>::const_iterator
	   i_cluster = connector.cluster_s ().begin ();
	 i_cluster != connector.cluster_s ().end ();
	 i_cluster++) {
      WH_CNCT2D_SegmentCluster* cluster_i 
	= (WH_CNCT2D_SegmentCluster*)(*i_cluster);

      if (!cluster_i->isOuterLoop ()) continue;
      
      WH_Polygon2D outerParamPolygon;
      {
	vector<WH_Segment2D> paramSeg_s;
	cluster_i->getSegments 
	  (paramSeg_s);
	vector<WH_Vector2D> param_s;
	cluster_i->getVertexsFromSegments (paramSeg_s, 
					   param_s);
	outerParamPolygon = WH_Polygon2D (param_s);

	int nVertexs = (int)param_s.size ();
	vector<WH_Vector3D> point_s (nVertexs);
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  point_s[iVertex] = basePlane.positionAt (param_s[iVertex]);
	}  
	
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  WH_Vector3D point = point_s[iVertex];
	  this->resultBody ()->addVertexPoint (point);
	}  
	
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  WH_Vector3D point0 = point_s[iVertex];
	  WH_Vector3D point1 = point_s[(iVertex + 1) % nVertexs];
	  WH_GM3D_SegmentFacet* facet 
	    = new WH_GM3D_SegmentFacet (point0, point1, false);
	  WH_ASSERT(facet != WH_NULL);
	  this->resultBody ()->addSegmentFacet (facet);
	}
      }

      WH_GM3D_PolygonFacet* facet 
	= this->resultBody ()->createPolygonFacet 
	(basePlane, outerParamPolygon, false, false);
      
      for (vector<WH_CNCT2D_SegmentCluster*>::const_iterator
	     j_cluster = cluster_i->innerLoopCluster_s ().begin ();
	   j_cluster != cluster_i->innerLoopCluster_s ().end ();
	   j_cluster++) {
	WH_CNCT2D_SegmentCluster* cluster_j = (*j_cluster);

	vector<WH_Segment2D> paramSeg_s;
	cluster_j->getSegments 
	  (paramSeg_s);
	vector<WH_Vector2D> param_s;
	cluster_j->getVertexsFromSegments (paramSeg_s, 
					 param_s);
	WH_Polygon2D paramPolygon (param_s);
	facet->addInnerLoop (paramPolygon);

	int nVertexs = (int)param_s.size ();
	vector<WH_Vector3D> point_s (nVertexs);
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  point_s[iVertex] = basePlane.positionAt (param_s[iVertex]);
	}  
	
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  WH_Vector3D point = point_s[iVertex];
	  this->resultBody ()->addVertexPoint (point);
	}  
	
	for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	  WH_Vector3D point0 = point_s[iVertex];
	  WH_Vector3D point1 = point_s[(iVertex + 1) % nVertexs];
	  WH_GM3D_SegmentFacet* facet 
	    = new WH_GM3D_SegmentFacet (point0, point1, false);
	  WH_ASSERT(facet != WH_NULL);
	  this->resultBody ()->addSegmentFacet (facet);
	}
      }

      this->resultBody ()->addPolygonFacet (facet);
    }
  }
}

void WH_GM3D_SheetSetOperator
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->canPerform ());
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);

  WH_CVR_LINE;

  WH_ASSERT(0 < this->body0 ()->polygonFacet_s ().size ());
  WH_Plane3D basePlane 
    = this->body0 ()->polygonFacet_s ()[0]->plane ();

  WH_GM2D_FacetBody* paramBody0 
    = this->createParameterFacetBody 
    (this->body0 (), basePlane);

  WH_GM2D_FacetBody* paramBody1 
    = this->createParameterFacetBody 
    (this->body1 (), basePlane);

  WH_GM2D_FacetBody* resultParamBody = new WH_GM2D_FacetBody (true);
  WH_ASSERT(resultParamBody != WH_NULL);

  WH_GM2D_SetOperator::OperationType opType;
  switch (this->operationType ()) {
  case UNION:
    WH_CVR_LINE;
    opType = WH_GM2D_SetOperator::UNION;
    break;
  case INTERSECTION:
    WH_CVR_LINE;
    opType = WH_GM2D_SetOperator::INTERSECTION;
    break;
  case SUBTRACTION:
    WH_CVR_LINE;
    opType = WH_GM2D_SetOperator::SUBTRACTION;
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
  WH_GM2D_SetOperator setOperator 
    (opType, paramBody0, paramBody1, resultParamBody);
  setOperator.perform ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(resultParamBody->assureInvariant ());
#endif

  this->makeResultBody (resultParamBody, basePlane);

  delete paramBody0;
  paramBody0 = WH_NULL;
  
  delete paramBody1;
  paramBody1 = WH_NULL;

  delete resultParamBody;
  resultParamBody = WH_NULL;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif  
}

WH_GM3D_SheetSetOperator::OperationType WH_GM3D_SheetSetOperator
::operationType () const
{
  return _operationType;
}

WH_GM3D_FacetBody* WH_GM3D_SheetSetOperator
::body0 () const
{
  return _body0;
}

WH_GM3D_FacetBody* WH_GM3D_SheetSetOperator
::body1 () const
{
  return _body1;
}

WH_GM3D_FacetBody* WH_GM3D_SheetSetOperator
::resultBody () const
{
  return _resultBody;
}

bool WH_GM3D_SheetSetOperator
::canPerform () const
{
  WH_CVR_LINE;

  bool result = true;

  /* check if there is any non-regular polygon facet */
  /* check if there is any off-plane polygon facet */

  bool firstPlaneIsHere = false;
  WH_Plane3D firstPlane;

  WH_ASSERT(0 < this->body0 ()->polygonFacet_s ().size ());
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->body0 ()->polygonFacet_s ().begin ();
       i_facet != this->body0 ()->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    WH_ASSERT(facet_i->faceType () == WH_GM3D_PolygonFacet::SHEET);
    if (!facet_i->isRegular ()) {
      WH_CVR_LINE;                            /* NOT COVERED */
      result = false;
      break;
    } else {
      WH_CVR_LINE;
      if (firstPlaneIsHere) {
	WH_CVR_LINE;
	if (WH_ne (facet_i->plane (), firstPlane)
	    && !WH_isReverse (facet_i->plane (), firstPlane)) {
	  WH_CVR_LINE;                            /* NOT COVERED */
	  result = false;
	  break;
	}
      } else {
	WH_CVR_LINE;
	firstPlane = facet_i->plane ();
	firstPlaneIsHere = true;
      }
    }
  }  

  if (result) {
    WH_CVR_LINE;
    WH_ASSERT(0 < this->body1 ()->polygonFacet_s ().size ());
    for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	   i_facet = this->body1 ()->polygonFacet_s ().begin ();
	 i_facet != this->body1 ()->polygonFacet_s ().end ();
	 i_facet++) {
      WH_GM3D_PolygonFacet* facet_i = (*i_facet);
      
      WH_ASSERT(facet_i->faceType () == WH_GM3D_PolygonFacet::SHEET);
      if (!facet_i->isRegular ()) {
	WH_CVR_LINE;                            /* NOT COVERED */
	result = false;
	break;
      } else {
	WH_CVR_LINE;
	if (WH_ne (facet_i->plane (), firstPlane)
	    && !WH_isReverse (facet_i->plane (), firstPlane)) {
	  WH_CVR_LINE;                          /* NOT COVERED */
	  result = false;
	  break;
	}
      }
    }  
  }

  return result;
}



/* not yet covered 
bool WH_GM3D_SheetSetOperator
::canPerform () const
*/



