/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm2d_convert.cc : converter module between B-rep and facet */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm2d_convert.h"



/* class WH_GM2D_ConverterFromBrepToFacet */

WH_GM2D_ConverterFromBrepToFacet
::WH_GM2D_ConverterFromBrepToFacet 
(WH_GM2D_Body* brepBody,
 WH_GM2D_FacetBody* facetBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(brepBody != WH_NULL);
  WH_ASSERT(facetBody != WH_NULL);
  WH_ASSERT((brepBody->isRegular () && facetBody->isRegular ())
	    || (brepBody->isRegular () && !facetBody->isRegular ())
	    || (!brepBody->isRegular () && !facetBody->isRegular ()));
  
  WH_CVR_LINE;
  
  _brepBody = brepBody;
  _facetBody = facetBody;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM2D_ConverterFromBrepToFacet
::~WH_GM2D_ConverterFromBrepToFacet () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_ConverterFromBrepToFacet
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->brepBody () != WH_NULL);
  WH_ASSERT(this->facetBody () != WH_NULL);
  WH_ASSERT((this->brepBody ()->isRegular () 
	     && this->facetBody ()->isRegular ())
	    || (this->brepBody ()->isRegular () 
		&& !this->facetBody ()->isRegular ())
	    || (!this->brepBody ()->isRegular () 
		&& !this->facetBody ()->isRegular ()));

  return true;
}

bool WH_GM2D_ConverterFromBrepToFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  return true;
}

void WH_GM2D_ConverterFromBrepToFacet
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->brepBody ()->isConsistent ());
  WH_ASSERT(this->facetBody ()->bodyType () 
	    == WH_GM2D_FacetBody::EMPTY);

  WH_CVR_LINE;

  /* generate segment facets from B-rep edges */
  for (vector<WH_GM2D_Edge*>::const_iterator 
	 i_edge = this->brepBody ()->edge_s ().begin ();
       i_edge != this->brepBody ()->edge_s ().end ();
       i_edge++) {
    WH_GM2D_Edge* edge_i = (*i_edge);
    
    WH_GM2D_SegmentFacet* facet 
      = this->facetBody ()->createSegmentFacet 
      (edge_i->firstVertexUse ()->vertex ()->point (), 
       edge_i->lastVertexUse ()->vertex ()->point (),
       edge_i->frontSide ()->isInsideVolume (),  
       edge_i->backSide ()->isInsideVolume ());
    this->facetBody ()->addSegmentFacet (facet);
  }
  
  if (!this->facetBody ()->isRegular ()) {
    WH_CVR_LINE;
    
    /* generate vertex points from B-rep vertexs */
    for (vector<WH_GM2D_Vertex*>::const_iterator 
	   i_vertex = this->brepBody ()->vertex_s ().begin ();
	 i_vertex != this->brepBody ()->vertex_s ().end ();
	 i_vertex++) {
      WH_GM2D_Vertex* vertex_i = (*i_vertex);
      this->facetBody ()->addVertexPoint (vertex_i->point ());
    }
  }
}

WH_GM2D_Body* WH_GM2D_ConverterFromBrepToFacet
::brepBody () const
{
  return _brepBody;
}

WH_GM2D_FacetBody* WH_GM2D_ConverterFromBrepToFacet
::facetBody () const
{
  return _facetBody;
}



/* class WH_GM2D_ConverterFromFacetToBrep */

WH_GM2D_ConverterFromFacetToBrep
::WH_GM2D_ConverterFromFacetToBrep 
(WH_GM2D_Body* brepBody,
 WH_GM2D_FacetBody* facetBody) 
{
  /* PRE-CONDITION */
  WH_ASSERT(brepBody != WH_NULL);
  WH_ASSERT(facetBody != WH_NULL);
  WH_ASSERT(brepBody->isRegular () == facetBody->isRegular ());
  
  WH_CVR_LINE;
  
  _brepBody = brepBody;
  _facetBody = facetBody;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM2D_ConverterFromFacetToBrep
::~WH_GM2D_ConverterFromFacetToBrep () 
{
  WH_CVR_LINE;
}

bool WH_GM2D_ConverterFromFacetToBrep
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->brepBody () != WH_NULL);
  WH_ASSERT(this->facetBody () != WH_NULL);
  WH_ASSERT(this->brepBody ()->isRegular () 
	    == this->facetBody ()->isRegular ());

  return true;
}

bool WH_GM2D_ConverterFromFacetToBrep
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  return true;
}

WH_GM2D_Vertex* WH_GM2D_ConverterFromFacetToBrep
::registerVertex 
(const WH_Vector2D& vertexPoint)
{
  WH_CVR_LINE;

  WH_GM2D_Vertex* result 
    = this->brepBody ()->findVertex (vertexPoint);

  if (result == WH_NULL) {
    WH_CVR_LINE;
    result = this->brepBody ()->createVertex (vertexPoint);
    this->brepBody ()->addVertex (result);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_GM2D_ConverterFromFacetToBrep
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->brepBody ()->isConsistent ());

  WH_CVR_LINE;

  for (vector<WH_GM2D_SegmentFacet*>::const_iterator 
	 i_facet = this->facetBody ()->segmentFacet_s ().begin ();
       i_facet != this->facetBody ()->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM2D_SegmentFacet* facet_i = (*i_facet);
    
    WH_GM2D_Edge* edge = this->brepBody ()->createEdge ();
    WH_ASSERT(edge != WH_NULL);
    edge->setVertexs 
      (this->registerVertex (facet_i->firstPoint ()), 
       this->registerVertex (facet_i->lastPoint ()));
    edge->setSides 
      (facet_i->frontSideIsInsideVolume (),
       facet_i->backSideIsInsideVolume ());
    this->brepBody ()->addEdge (edge);
  }  

  for (vector<WH_Vector2D>::const_iterator 
	 i_point = this->facetBody ()->vertexPoint_s ().begin ();
       i_point != this->facetBody ()->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    this->registerVertex (point_i);
  }

  this->brepBody ()->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->brepBody ()->isConsistent ());
#endif
}

WH_GM2D_Body* WH_GM2D_ConverterFromFacetToBrep
::brepBody () const
{
  return _brepBody;
}

WH_GM2D_FacetBody* WH_GM2D_ConverterFromFacetToBrep
::facetBody () const
{
  return _facetBody;
}



/* test coverage completed */



