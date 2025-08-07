/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_convert.cc : converter module between B-rep and facet */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_convert.h"



/* class WH_GM3D_ConverterFromBrepToFacet */

WH_GM3D_ConverterFromBrepToFacet
::WH_GM3D_ConverterFromBrepToFacet 
(WH_GM3D_Body* brepBody,
 WH_GM3D_FacetBody* facetBody) 
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

WH_GM3D_ConverterFromBrepToFacet
::~WH_GM3D_ConverterFromBrepToFacet () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_ConverterFromBrepToFacet
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

bool WH_GM3D_ConverterFromBrepToFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  return true;
}

void WH_GM3D_ConverterFromBrepToFacet
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->brepBody ()->isConsistent ());
  WH_ASSERT(this->facetBody ()->bodyType () 
	    == WH_GM3D_FacetBody::EMPTY);

  WH_CVR_LINE;

  /* generate polygon facets from B-rep faces */
  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = _brepBody->face_s ().begin ();
       i_face != _brepBody->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    
    WH_GM3D_Loop* outerLoop = face_i->outerLoop ();
    WH_Polygon2D parameterPolygon = outerLoop->parameterPolygon ();
    WH_GM3D_PolygonFacet* facet 
      = this->facetBody ()->createPolygonFacet 
      (face_i->plane (), 
       parameterPolygon,
       face_i->frontSide ()->isInsideVolume (),  
       face_i->backSide ()->isInsideVolume ());

    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = face_i->innerLoop_s ().begin ();
	 i_loop != face_i->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);
      facet->addInnerLoop (loop_i->parameterPolygon ());
    }

    for (vector<WH_GM3D_OffLoopEdgeUse*>::const_iterator 
	   i_edgeUse = face_i->offLoopEdgeUse_s ().begin ();
	 i_edgeUse != face_i->offLoopEdgeUse_s ().end ();
	 i_edgeUse++) {
      WH_GM3D_OffLoopEdgeUse* edgeUse_i = (*i_edgeUse);
      facet->addOffLoopEdgeSegment (edgeUse_i->parameterSegment ());
    }

    for (vector<WH_GM3D_OffLoopVertexUse*>::const_iterator 
	   i_vertexUse = face_i->offLoopVertexUse_s ().begin ();
	 i_vertexUse != face_i->offLoopVertexUse_s ().end ();
	 i_vertexUse++) {
      WH_GM3D_OffLoopVertexUse* vertexUse_i = (*i_vertexUse);
      facet->addOffLoopVertexPoint (vertexUse_i->parameter ());
    }

    _facetBody->addPolygonFacet (facet);
  }

  _facetBody->generateTriangleFacets ();

  if (!this->facetBody ()->isRegular ()) {
    WH_CVR_LINE;
    
    /* generate segment facets from B-rep edges */
    for (vector<WH_GM3D_Edge*>::const_iterator 
	   i_edge = _brepBody->edge_s ().begin ();
	 i_edge != _brepBody->edge_s ().end ();
	 i_edge++) {
      WH_GM3D_Edge* edge_i = (*i_edge);
      
      bool isExplicit = false;
      if (0 < edge_i->offLoopEdgeUse_s ().size ()) {
	WH_CVR_LINE;
	isExplicit = true;
      }
      WH_GM3D_SegmentFacet* facet 
	= this->facetBody ()->createSegmentFacet 
	(edge_i->firstVertexUse ()->vertex ()->point (), 
	 edge_i->lastVertexUse ()->vertex ()->point (),
	 isExplicit);
      
      _facetBody->addSegmentFacet (facet);
    }
    
    /* generate vertex points from B-rep vertexs */
    for (vector<WH_GM3D_Vertex*>::const_iterator 
	   i_vertex = _brepBody->vertex_s ().begin ();
	 i_vertex != _brepBody->vertex_s ().end ();
	 i_vertex++) {
      WH_GM3D_Vertex* vertex_i = (*i_vertex);
      _facetBody->addVertexPoint (vertex_i->point ());
    }
  }
}

WH_GM3D_Body* WH_GM3D_ConverterFromBrepToFacet
::brepBody () const
{
  return _brepBody;
}

WH_GM3D_FacetBody* WH_GM3D_ConverterFromBrepToFacet
::facetBody () const
{
  return _facetBody;
}



/* class WH_GM3D_ConverterFromFacetToBrep */

WH_GM3D_ConverterFromFacetToBrep
::WH_GM3D_ConverterFromFacetToBrep 
(WH_GM3D_Body* brepBody,
 WH_GM3D_FacetBody* facetBody) 
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

WH_GM3D_ConverterFromFacetToBrep
::~WH_GM3D_ConverterFromFacetToBrep () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_ConverterFromFacetToBrep
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->brepBody () != WH_NULL);
  WH_ASSERT(this->facetBody () != WH_NULL);
  WH_ASSERT(this->brepBody ()->isRegular () 
	    == this->facetBody ()->isRegular ());

  return true;
}

bool WH_GM3D_ConverterFromFacetToBrep
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  return true;
}

WH_GM3D_Vertex* WH_GM3D_ConverterFromFacetToBrep
::registerVertex 
(const WH_Vector3D& vertexPoint)
{
  WH_CVR_LINE;

  WH_GM3D_Vertex* result 
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

WH_GM3D_Edge* WH_GM3D_ConverterFromFacetToBrep
::registerEdge 
(WH_GM3D_Vertex* firstVertex,
 WH_GM3D_Vertex* lastVertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(firstVertex != WH_NULL);
  WH_ASSERT(lastVertex != WH_NULL);

  WH_CVR_LINE;

  WH_GM3D_Edge* result = WH_NULL;
  for (vector<WH_GM3D_Edge*>::const_iterator 
	 i_edge = this->brepBody ()->edge_s ().begin ();
       i_edge != this->brepBody ()->edge_s ().end ();
       i_edge++) {
    WH_GM3D_Edge* edge_i = (*i_edge);
    
    if ((edge_i->firstVertexUse ()->vertex () == firstVertex
	 && edge_i->lastVertexUse ()->vertex () == lastVertex)
	|| (edge_i->firstVertexUse ()->vertex () == lastVertex
	    && edge_i->lastVertexUse ()->vertex () == firstVertex)) {
      WH_CVR_LINE;
      result = edge_i;
      break;
    }
  }

  if (result == WH_NULL) {
    WH_CVR_LINE;
    result = this->brepBody ()->createEdge ();
    result->setVertexs (firstVertex, lastVertex);
    this->brepBody ()->addEdge (result);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_GM3D_ConverterFromFacetToBrep
::makeLoop 
(WH_GM3D_Loop* loop,
 const WH_Polygon3D& polygon)
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(!loop->isConsistent ());

  int nVertexs = polygon.nVertexs ();
  vector<WH_GM3D_Vertex*> vertex_s (nVertexs);
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    WH_Vector3D point = polygon.vertex (iVertex);
    vertex_s[iVertex] = this->registerVertex (point);
    loop->addVertex (vertex_s[iVertex]);
  }

  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    this->registerEdge (vertex_s[iVertex], 
			vertex_s[(iVertex + 1) % nVertexs]);
  }
}

void WH_GM3D_ConverterFromFacetToBrep
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->brepBody ()->isConsistent ());

  WH_CVR_LINE;

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->facetBody ()->polygonFacet_s ().begin ();
       i_facet != this->facetBody ()->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    WH_GM3D_Face* face 
      = this->brepBody ()->createFace (facet_i->plane ());
    face->setSides 
      (facet_i->frontSideIsInsideVolume (),
       facet_i->backSideIsInsideVolume ());
    
    {
      WH_GM3D_Loop* loop = this->brepBody ()->createLoop (face);
      this->makeLoop (loop, facet_i->outerLoopPolygon ());
      face->setOuterLoop (loop); 
    }
    
    for (vector<WH_Polygon3D>::const_iterator 
	   i_poly = facet_i->innerLoopPolygon_s ().begin ();
	 i_poly != facet_i->innerLoopPolygon_s ().end ();
	 i_poly++) {
      WH_Polygon3D poly_i = (*i_poly);
      WH_GM3D_Loop* loop = this->brepBody ()->createLoop (face);
      this->makeLoop (loop, poly_i);
      face->addInnerLoop (loop); 
    }

    for (vector<WH_Segment3D>::const_iterator 
	   i_seg = facet_i->offLoopEdgeSegment_s ().begin ();
	 i_seg != facet_i->offLoopEdgeSegment_s ().end ();
	 i_seg++) {
      WH_Segment3D seg_i = (*i_seg);
      face->addOffLoopEdge 
	(this->registerEdge 
	 (this->registerVertex (seg_i.p0 ()), 
	  this->registerVertex (seg_i.p1 ())));
    }
    
    for (vector<WH_Vector3D>::const_iterator 
	   i_point = facet_i->offLoopVertexPoint_s ().begin ();
	 i_point != facet_i->offLoopVertexPoint_s ().end ();
	 i_point++) {
      WH_Vector3D point_i = (*i_point);
      face->addOffLoopVertex (this->registerVertex (point_i));
    }

    this->brepBody ()->addFace (face);
  }  

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = this->facetBody ()->segmentFacet_s ().begin ();
       i_facet != this->facetBody ()->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    this->registerEdge 
      (this->registerVertex (facet_i->firstPoint ()), 
       this->registerVertex (facet_i->lastPoint ()));
  }  

  for (vector<WH_Vector3D>::const_iterator 
	 i_point = this->facetBody ()->vertexPoint_s ().begin ();
       i_point != this->facetBody ()->vertexPoint_s ().end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    this->registerVertex (point_i);
  }

  this->brepBody ()->fix ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->brepBody ()->isConsistent ());
#endif
}

WH_GM3D_Body* WH_GM3D_ConverterFromFacetToBrep
::brepBody () const
{
  return _brepBody;
}

WH_GM3D_FacetBody* WH_GM3D_ConverterFromFacetToBrep
::facetBody () const
{
  return _facetBody;
}



/* test coverage completed */



