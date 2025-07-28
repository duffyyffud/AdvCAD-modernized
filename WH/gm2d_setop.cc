/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d_setop.cc : Boolean set operation module */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d_setop.h"



/* class WH_GM2D_SetOperator */

WH_GM2D_SetOperator
::WH_GM2D_SetOperator 
(OperationType operationType,
 WH_GM2D_FacetBody* body0,
 WH_GM2D_FacetBody* body1,
 WH_GM2D_FacetBody* resultBody) 
{
  /* PRE-CONDITION */
  if (body0 == WH_NULL) {
    throw invalid_argument("WH_GM2D_SetOperator: body0 cannot be null");
  }
  if (!body0->isRegular()) {
    throw invalid_argument("WH_GM2D_SetOperator: body0 must be regular");
  }
  if (body0->bodyType() != WH_GM2D_FacetBody::VOLUME) {
    throw invalid_argument("WH_GM2D_SetOperator: body0 must be VOLUME type");
  }
  if (body1 == WH_NULL) {
    throw invalid_argument("WH_GM2D_SetOperator: body1 cannot be null");
  }
  if (!body1->isRegular()) {
    throw invalid_argument("WH_GM2D_SetOperator: body1 must be regular");
  }
  if (body1->bodyType() != WH_GM2D_FacetBody::VOLUME) {
    throw invalid_argument("WH_GM2D_SetOperator: body1 must be VOLUME type");
  }
  if (resultBody == WH_NULL) {
    throw invalid_argument("WH_GM2D_SetOperator: resultBody cannot be null");
  }
  if (!resultBody->isRegular()) {
    throw invalid_argument("WH_GM2D_SetOperator: resultBody must be regular");
  }
  if (resultBody->bodyType() != WH_GM2D_FacetBody::EMPTY) {
    throw invalid_argument("WH_GM2D_SetOperator: resultBody must be EMPTY type");
  }
  if (body0 == body1) {
    throw invalid_argument("WH_GM2D_SetOperator: body0 and body1 must be distinct");
  }
  if (resultBody == body0) {
    throw invalid_argument("WH_GM2D_SetOperator: resultBody must be distinct from body0");
  }
  if (resultBody == body1) {
    throw invalid_argument("WH_GM2D_SetOperator: resultBody must be distinct from body1");
  }
  
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

WH_GM2D_SetOperator
::~WH_GM2D_SetOperator () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_SetOperator
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body0 () != WH_NULL);
  WH_ASSERT(this->body0 ()->isRegular ());
  WH_ASSERT(this->body0 ()->bodyType () 
	    == WH_GM2D_FacetBody::VOLUME);
  WH_ASSERT(this->body1 () != WH_NULL);
  WH_ASSERT(this->body1 ()->isRegular ());
  WH_ASSERT(this->body1 ()->bodyType () 
	    == WH_GM2D_FacetBody::VOLUME);
  WH_ASSERT(this->resultBody () != WH_NULL);
  WH_ASSERT(this->resultBody ()->isRegular ());
  WH_ASSERT(this->resultBody ()->bodyType () 
	    == WH_GM2D_FacetBody::EMPTY
	    || this->resultBody ()->bodyType () 
	    == WH_GM2D_FacetBody::VOLUME);
  WH_ASSERT(this->body0 () != this->body1 ());
  WH_ASSERT(this->resultBody () != this->body0 ());
  WH_ASSERT(this->resultBody () != this->body1 ());

  return true;
}

bool WH_GM2D_SetOperator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_GM2D_SetOperator
::divideBodyByBody 
(const WH_GM2D_FacetBody* bodyFrom, 
 const WH_GM2D_FacetBody* bodyBy,
 vector<WH_GM2D_SegmentFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  if (bodyFrom == WH_NULL) {
    throw invalid_argument("divideBodyByBody: bodyFrom cannot be null");
  }
  if (bodyBy == WH_NULL) {
    throw invalid_argument("divideBodyByBody: bodyBy cannot be null");
  }
  if (bodyFrom == bodyBy) {
    throw invalid_argument("divideBodyByBody: bodyFrom and bodyBy must be distinct");
  }

  WH_CVR_LINE;

  facet_s_OUT.clear ();

  vector<WH_Segment2D> segmentBy_s;
  for (const auto* facet_i : bodyBy->segmentFacet_s()) {
    segmentBy_s.push_back(facet_i->segment());
  }  

  for (const auto* facet_i : bodyFrom->segmentFacet_s()) {
    vector<WH_GM2D_SegmentFacet*> dividedFacet_s;
    dividedFacet_s.push_back(facet_i->createCopy());
    WH_GM2D_SegmentFacet::divideFacetsBySegments(segmentBy_s, dividedFacet_s);
    WH_T_Add(dividedFacet_s, facet_s_OUT);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(2 < facet_s_OUT.size ());
#endif
}

void WH_GM2D_SetOperator
::classifyFacets 
(const vector<WH_GM2D_SegmentFacet*>& facet_s, 
 const WH_GM2D_FacetBody* bodyBy,
 vector<WH_GM2D_SegmentFacet*>& inFacet_s_OUT,
 vector<WH_GM2D_SegmentFacet*>& onFacet_s_OUT,
 vector<WH_GM2D_SegmentFacet*>& outFacet_s_OUT)
{
  /* PRE-CONDITION */
  if (facet_s.empty()) {
    throw invalid_argument("classifyFacets: facet_s cannot be empty");
  }
  if (bodyBy == WH_NULL) {
    throw invalid_argument("classifyFacets: bodyBy cannot be null");
  }

  WH_CVR_LINE;

  inFacet_s_OUT.clear ();
  onFacet_s_OUT.clear ();
  outFacet_s_OUT.clear ();

  bodyBy->setUpInOutCheck ();

  for (auto* facet_i : facet_s) {
    WH_Vector2D midPoint = facet_i->segment().midPoint();
    WH_InOutChecker2D::ContainmentType flag = bodyBy->checkContainmentAt(midPoint);
    switch (flag) {
    case WH_InOutChecker2D::IN:
      WH_CVR_LINE;
      inFacet_s_OUT.push_back(facet_i);
      break;
    case WH_InOutChecker2D::OUT:
      WH_CVR_LINE;
      outFacet_s_OUT.push_back(facet_i);
      break;
    case WH_InOutChecker2D::ON:
      WH_CVR_LINE;
      onFacet_s_OUT.push_back(facet_i);
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

void WH_GM2D_SetOperator
::classifyOnFacets 
(const vector<WH_GM2D_SegmentFacet*>& facet_s, 
 const WH_GM2D_FacetBody* bodyBy,
 vector<WH_GM2D_SegmentFacet*>& onInFacet_s_OUT,
 vector<WH_GM2D_SegmentFacet*>& onOutFacet_s_OUT)
{
  /* PRE-CONDITION */
  if (bodyBy == WH_NULL) {
    throw invalid_argument("classifyOnFacets: bodyBy cannot be null");
  }

  WH_CVR_LINE;

  onInFacet_s_OUT.clear ();
  onOutFacet_s_OUT.clear ();

  for (auto* facet_i : facet_s) {
    WH_Segment2D segment_i = facet_i->segment();

    WH_GM2D_SegmentFacet* containingFacet = WH_NULL;
    for (const auto* facet_j : bodyBy->segmentFacet_s()) {
      WH_Segment2D segment_j = facet_j->segment();

      if (segment_j.contains(segment_i.p0()) && segment_j.contains(segment_i.p1())) {
	WH_CVR_LINE;
	containingFacet = facet_j;
	break;
      }
    }
    WH_ASSERT(containingFacet != WH_NULL);
    
    WH_Vector2D normal;
    facet_i->getNormalToOutsideVolume(normal);
    WH_Vector2D normalOfContainingFacet;
    containingFacet->getNormalToOutsideVolume(normalOfContainingFacet);
    if (WH_eq(normal, normalOfContainingFacet)) {
      WH_CVR_LINE;
      /* ON-OUT */
      onOutFacet_s_OUT.push_back(facet_i);
    } else {
      WH_CVR_LINE;
      WH_ASSERT(WH_eq(normal, -normalOfContainingFacet));
      /* ON-IN */
      onInFacet_s_OUT.push_back(facet_i);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s.size () 
	    == onInFacet_s_OUT.size () 
	    + onOutFacet_s_OUT.size ());
#endif
}

void WH_GM2D_SetOperator
::collectFacets 
(const vector<WH_GM2D_SegmentFacet*>& facet_s)
{
  WH_CVR_LINE;

  for (const auto* facet_i : facet_s) {
    _resultBody->addSegmentFacet(facet_i->createCopy());
  }
}

void WH_GM2D_SetOperator
::perform ()
{
  /* PRE-CONDITION */
  if (this->resultBody()->bodyType() != WH_GM2D_FacetBody::EMPTY) {
    throw invalid_argument("perform: resultBody must be EMPTY type before operation");
  }

  WH_CVR_LINE;
  
  if (_operationType == SUBTRACTION) {
    WH_CVR_LINE;
    /* <body0> - <body1> == <body0> * (- <body1>) */
    /* negate body1 */
    _body1->reverseNormal ();
  }

  /* divide facets of each body by the other body */

  vector<WH_GM2D_SegmentFacet*> facetOfBody0_s;
  this->divideBodyByBody (_body0, _body1,
			  facetOfBody0_s);

  vector<WH_GM2D_SegmentFacet*> facetOfBody1_s;
  this->divideBodyByBody (_body1, _body0,
			  facetOfBody1_s);

  /* classify facets to IN, OUT, ON-IN and ON-OUT */

  vector<WH_GM2D_SegmentFacet*> inFacetOfBody0_s;
  vector<WH_GM2D_SegmentFacet*> onFacetOfBody0_s;
  vector<WH_GM2D_SegmentFacet*> outFacetOfBody0_s;
  this->classifyFacets 
    (facetOfBody0_s, _body1,
     inFacetOfBody0_s, onFacetOfBody0_s, outFacetOfBody0_s);
  
  vector<WH_GM2D_SegmentFacet*> inFacetOfBody1_s;
  vector<WH_GM2D_SegmentFacet*> onFacetOfBody1_s;
  vector<WH_GM2D_SegmentFacet*> outFacetOfBody1_s;
  this->classifyFacets 
    (facetOfBody1_s, _body0,
     inFacetOfBody1_s, onFacetOfBody1_s, outFacetOfBody1_s);

  vector<WH_GM2D_SegmentFacet*> onInFacetOfBody1_s;
  vector<WH_GM2D_SegmentFacet*> onOutFacetOfBody1_s;
  this->classifyOnFacets 
    (onFacetOfBody1_s, _body0,
     onInFacetOfBody1_s, onOutFacetOfBody1_s);
  
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

  if (_resultBody->bodyType () == WH_GM2D_FacetBody::VOLUME) {
    _resultBody->regularizeFacets ();
  }

  WH_T_Delete (facetOfBody0_s);
  WH_T_Delete (facetOfBody1_s);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif  
}

WH_GM2D_SetOperator::OperationType WH_GM2D_SetOperator
::operationType () const
{
  return _operationType;
}

WH_GM2D_FacetBody* WH_GM2D_SetOperator
::body0 () const
{
  return _body0;
}

WH_GM2D_FacetBody* WH_GM2D_SetOperator
::body1 () const
{
  return _body1;
}

WH_GM2D_FacetBody* WH_GM2D_SetOperator
::resultBody () const
{
  return _resultBody;
}



/* test coverage completed */



