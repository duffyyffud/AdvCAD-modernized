/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_setop.cc : volume Boolean set operation module */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_setop.h"



/* class WH_GM3D_SetOperator */

WH_GM3D_SetOperator
::WH_GM3D_SetOperator 
(OperationType operationType,
 WH_GM3D_FacetBody* body0,
 WH_GM3D_FacetBody* body1,
 WH_GM3D_FacetBody* resultBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body0 != WH_NULL);
  WH_ASSERT(body0->isRegular ());
  WH_ASSERT(body0->bodyType () 
	    == WH_GM3D_FacetBody::VOLUME);
  WH_ASSERT(body1 != WH_NULL);
  WH_ASSERT(body1->isRegular ());
  WH_ASSERT(body1->bodyType () 
	    == WH_GM3D_FacetBody::VOLUME);
  WH_ASSERT(resultBody != WH_NULL);
  WH_ASSERT(resultBody->isRegular ());
  WH_ASSERT(resultBody->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);
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

WH_GM3D_SetOperator
::~WH_GM3D_SetOperator () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_SetOperator
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body0 () != WH_NULL);
  WH_ASSERT(this->body0 ()->isRegular ());
  WH_ASSERT(this->body0 ()->bodyType () 
	    == WH_GM3D_FacetBody::VOLUME);
  WH_ASSERT(this->body1 () != WH_NULL);
  WH_ASSERT(this->body1 ()->isRegular ());
  WH_ASSERT(this->body1 ()->bodyType () 
	    == WH_GM3D_FacetBody::VOLUME);
  WH_ASSERT(this->resultBody () != WH_NULL);
  WH_ASSERT(this->resultBody ()->isRegular ());
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY
	    || this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::VOLUME);
  WH_ASSERT(this->body0 () != this->body1 ());
  WH_ASSERT(this->resultBody () != this->body0 ());
  WH_ASSERT(this->resultBody () != this->body1 ());

  return true;
}

bool WH_GM3D_SetOperator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_GM3D_SetOperator
::divideBodyByBody 
(WH_GM3D_FacetBody* bodyFrom, 
 WH_GM3D_FacetBody* bodyBy,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyFrom != WH_NULL);
  WH_ASSERT(bodyBy != WH_NULL);
  WH_ASSERT(bodyFrom != bodyBy);

  WH_CVR_LINE;

  facet_s_OUT.clear ();

  vector<WH_Triangle3D> triangleBy_s;
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_pfacet = bodyBy->polygonFacet_s ().begin ();
       i_pfacet != bodyBy->polygonFacet_s ().end ();
       i_pfacet++) {
    WH_GM3D_PolygonFacet* pfacet_i = (*i_pfacet);

    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = pfacet_i->triangleFacet_s ().begin ();
	 i_facet != pfacet_i->triangleFacet_s ().end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);
      triangleBy_s.push_back (facet_i->triangle ());
    }  
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
      WH_GM3D_TriangleFacet::divideFacetsByTriangles 
	(triangleBy_s, 
	 dividedFacet_s);
      WH_T_Add (dividedFacet_s, facet_s_OUT);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(2 < facet_s_OUT.size ());
#endif
}

void WH_GM3D_SetOperator
::classifyFacets 
(const vector<WH_GM3D_TriangleFacet*>& facet_s, 
 WH_GM3D_FacetBody* bodyBy,
 vector<WH_GM3D_TriangleFacet*>& inFacet_s_OUT,
 vector<WH_GM3D_TriangleFacet*>& onFacet_s_OUT,
 vector<WH_GM3D_TriangleFacet*>& outFacet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s.size ());
  WH_ASSERT(bodyBy != WH_NULL);

  WH_CVR_LINE;

  inFacet_s_OUT.clear ();
  onFacet_s_OUT.clear ();
  outFacet_s_OUT.clear ();

  bodyBy->setUpInOutCheck ();

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);

    WH_Vector3D center = facet_i->triangle ().centerOfGravity ();
    WH_InOutChecker3D::ContainmentType flag 
      = bodyBy->checkContainmentAt (center);
    switch (flag) {
    case WH_InOutChecker3D::IN:
      WH_CVR_LINE;
      inFacet_s_OUT.push_back (facet_i);
      break;
    case WH_InOutChecker3D::OUT:
      WH_CVR_LINE;
      outFacet_s_OUT.push_back (facet_i);
      break;
    case WH_InOutChecker3D::ON:
      WH_CVR_LINE;
      onFacet_s_OUT.push_back (facet_i);
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s.size () 
	    == inFacet_s_OUT.size () 
	    + onFacet_s_OUT.size () 
	    + outFacet_s_OUT.size ());
#endif
}

void WH_GM3D_SetOperator
::classifyOnFacets 
(const vector<WH_GM3D_TriangleFacet*>& facet_s, 
 WH_GM3D_FacetBody* bodyBy,
 vector<WH_GM3D_TriangleFacet*>& onInFacet_s_OUT,
 vector<WH_GM3D_TriangleFacet*>& onOutFacet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(bodyBy != WH_NULL);

  WH_CVR_LINE;

  onInFacet_s_OUT.clear ();
  onOutFacet_s_OUT.clear ();

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = facet_s.begin ();
       i_facet != facet_s.end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);

    WH_Triangle3D triangle = facet_i->triangle ();
    WH_Vector3D center = triangle.centerOfGravity ();
    WH_Vector3D normal;
    facet_i->getNormalToOutsideVolume 
      (normal);
    
    WH_GM3D_TriangleFacet* containingFacet = WH_NULL;
    bool isOnOut = false;
    for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	   j_pfacet = bodyBy->polygonFacet_s ().begin ();
	 j_pfacet != bodyBy->polygonFacet_s ().end ();
	 j_pfacet++) {
      WH_GM3D_PolygonFacet* pfacet_j = (*j_pfacet);

      for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	     j_facet = pfacet_j->triangleFacet_s ().begin ();
	   j_facet != pfacet_j->triangleFacet_s ().end ();
	   j_facet++) {
	WH_GM3D_TriangleFacet* facet_j = (*j_facet);
	
	WH_Vector3D normalOfFacetBy;
	facet_j->getNormalToOutsideVolume 
	  (normalOfFacetBy);
	if (WH_ne (normal, normalOfFacetBy)
	    && WH_ne (normal, -normalOfFacetBy)) continue;
	
	if (facet_j->triangle ().checkContainmentAt (center)
	    == WH_Triangle3D::IN) {
	  WH_CVR_LINE;
	  containingFacet = facet_j;
	  if (WH_eq (normal, normalOfFacetBy)) {
	    WH_CVR_LINE;
	    isOnOut = true;
	  }
	  break;
	}
      }

      if (containingFacet != WH_NULL) {
	WH_CVR_LINE;
	break;
      }
    }
    WH_ASSERT(containingFacet != WH_NULL);

    if (isOnOut) {
      WH_CVR_LINE;
      onOutFacet_s_OUT.push_back (facet_i);
    } else {
      WH_CVR_LINE;
      onInFacet_s_OUT.push_back (facet_i);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s.size () 
	    == onInFacet_s_OUT.size () 
	    + onOutFacet_s_OUT.size ());
#endif
}

void WH_GM3D_SetOperator
::collectFacets 
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

void WH_GM3D_SetOperator
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);

  WH_CVR_LINE;
  
  if (_operationType == SUBTRACTION) {
    WH_CVR_LINE;
    /* <body0> - <body1> == <body0> * (- <body1>) */
    /* negate body1 */
    _body1->reverseNormal ();
  }

  /* divide facets of each body by the other body */

  vector<WH_GM3D_TriangleFacet*> facetOfBody0_s;
  this->divideBodyByBody (_body0, _body1,
			  facetOfBody0_s);

  vector<WH_GM3D_TriangleFacet*> facetOfBody1_s;
  this->divideBodyByBody (_body1, _body0,
			  facetOfBody1_s);

  /* classify facets to IN, OUT, ON-IN and ON-OUT */

  vector<WH_GM3D_TriangleFacet*> inFacetOfBody0_s;
  vector<WH_GM3D_TriangleFacet*> onFacetOfBody0_s;
  vector<WH_GM3D_TriangleFacet*> outFacetOfBody0_s;
  this->classifyFacets 
    (facetOfBody0_s, _body1,
     inFacetOfBody0_s, onFacetOfBody0_s, outFacetOfBody0_s);
  
  vector<WH_GM3D_TriangleFacet*> inFacetOfBody1_s;
  vector<WH_GM3D_TriangleFacet*> onFacetOfBody1_s;
  vector<WH_GM3D_TriangleFacet*> outFacetOfBody1_s;
  this->classifyFacets 
    (facetOfBody1_s, _body0,
     inFacetOfBody1_s, onFacetOfBody1_s, outFacetOfBody1_s);

  vector<WH_GM3D_TriangleFacet*> onInFacetOfBody1_s;
  vector<WH_GM3D_TriangleFacet*> onOutFacetOfBody1_s;
  this->classifyOnFacets 
    (onFacetOfBody1_s, _body0,
     onInFacetOfBody1_s, onOutFacetOfBody1_s);
  
  /* collect triangle facets and reconstruct polygon facets */

  if (_operationType == UNION) {
    WH_CVR_LINE;
    /* collect OUT facets of body0, OUT and ON-OUT facets of body1 */
    this->collectFacets (outFacetOfBody0_s);
    this->collectFacets (outFacetOfBody1_s);
    this->collectFacets (onOutFacetOfBody1_s);
  } else {
    WH_CVR_LINE;
    /* intersection and subtraction (body1 is negated) */
    /* collect IN facets of body0, IN and ON-OUT facets of body1 */
    this->collectFacets (inFacetOfBody0_s);
    this->collectFacets (inFacetOfBody1_s);
    this->collectFacets (onOutFacetOfBody1_s);
  }

  _resultBody->generatePolygonFacets ();

  WH_T_Delete (facetOfBody0_s);
  WH_T_Delete (facetOfBody1_s);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif  
}

WH_GM3D_SetOperator::OperationType WH_GM3D_SetOperator
::operationType () const
{
  return _operationType;
}

WH_GM3D_FacetBody* WH_GM3D_SetOperator
::body0 () const
{
  return _body0;
}

WH_GM3D_FacetBody* WH_GM3D_SetOperator
::body1 () const
{
  return _body1;
}

WH_GM3D_FacetBody* WH_GM3D_SetOperator
::resultBody () const
{
  return _resultBody;
}



/* test coverage completed */



