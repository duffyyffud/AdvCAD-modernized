/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_facet_poly.cc : facet module of 3-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_facet.h"
#include "tessellate2d.h"



/* class WH_GM3D_PolygonFacet */

WH_GM3D_PolygonFacet
::WH_GM3D_PolygonFacet 
(const WH_Plane3D& plane,
 const WH_Polygon2D& outerLoopParameterPolygon, 
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume) 
  : _plane (plane), 
    _outerLoopParameterPolygon (outerLoopParameterPolygon) 
{
  WH_CVR_LINE;
  
  vector<WH_Vector3D> vertex_s;
  int nVertexs = outerLoopParameterPolygon.nVertexs ();
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    vertex_s.push_back 
      (plane.positionAt (outerLoopParameterPolygon.vertex (iVertex)));
  }
  _outerLoopPolygon = WH_Polygon3D (vertex_s); 

  _frontSideIsInsideVolume = frontSideIsInsideVolume;
  _backSideIsInsideVolume = backSideIsInsideVolume;

  _faceId = WH_NO_INDEX;

  _minRange = _outerLoopPolygon.minRange ();
  _maxRange = _outerLoopPolygon.maxRange ();

  _parameterMinRange = _outerLoopParameterPolygon.minRange ();
  _parameterMaxRange = _outerLoopParameterPolygon.maxRange ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM3D_PolygonFacet
::~WH_GM3D_PolygonFacet () 
{
  WH_CVR_LINE;

  WH_T_Delete (_triangleFacet_s);
}

bool WH_GM3D_PolygonFacet
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->innerLoopPolygon_s ().size () 
	    == this->innerLoopParameterPolygon_s ().size ());
  WH_ASSERT(this->offLoopEdgeSegment_s ().size () 
	    == this->offLoopParameterEdgeSegment_s ().size ());
  WH_ASSERT(this->offLoopVertexPoint_s ().size () 
	    == this->offLoopParameterVertexPoint_s ().size ());

  return true;
}

bool WH_GM3D_PolygonFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  WH_T_AssureInvariant (this->triangleFacet_s ());



  WH_Vector3D outerMinRange;
  WH_Vector3D outerMaxRange;
  this->getRange
    (outerMinRange, outerMaxRange);
  WH_ASSERT(WH_ne (outerMinRange, outerMaxRange));
  WH_ASSERT(WH_le (outerMinRange, outerMaxRange));

  WH_Vector2D outerParamMinRange;
  WH_Vector2D outerParamMaxRange;
  this->getParameterRange 
    (outerParamMinRange, outerParamMaxRange);
  WH_ASSERT(WH_lt (outerParamMinRange, outerParamMaxRange));

  WH_Polygon3D outerPoly = this->outerLoopPolygon ();
  WH_ASSERT(outerPoly.isRegular ());
  WH_ASSERT(WH_eq (outerPoly.plane (), this->plane ()) 
	    || WH_isReverse (outerPoly.plane (), this->plane ()));
  WH_ASSERT(WH_eq (outerPoly.minRange (), outerMinRange));
  WH_ASSERT(WH_eq (outerPoly.maxRange (), outerMaxRange));
  
  WH_Polygon2D outerParamPoly 
    = this->outerLoopParameterPolygon ();
  WH_ASSERT(outerParamPoly.isRegular ());
  WH_ASSERT(WH_eq (outerParamPoly.minRange (), outerParamMinRange));
  WH_ASSERT(WH_eq (outerParamPoly.maxRange (), outerParamMaxRange));
  
  for (vector<WH_Polygon3D>::const_iterator 
	 i_poly = this->innerLoopPolygon_s ().begin ();
       i_poly != this->innerLoopPolygon_s ().end ();
       i_poly++) {
    WH_Polygon3D poly_i = (*i_poly);
    WH_ASSERT(poly_i.isRegular ());
    WH_ASSERT(WH_eq (poly_i.plane (), this->plane ()) 
	      || WH_isReverse (poly_i.plane (), this->plane ()));
    WH_ASSERT(WH_between (poly_i.minRange (), 
			  outerMinRange, outerMaxRange));
    WH_ASSERT(WH_between (poly_i.maxRange (), 
			  outerMinRange, outerMaxRange));
  }

  for (vector<WH_Polygon2D>::const_iterator 
	 i_poly = this->innerLoopParameterPolygon_s ().begin ();
       i_poly != this->innerLoopParameterPolygon_s ().end ();
       i_poly++) {
    WH_Polygon2D poly_i = (*i_poly);
    WH_ASSERT(poly_i.isRegular ());
    WH_ASSERT(WH_justBetween (poly_i.minRange (), 
			      outerParamMinRange, outerParamMaxRange));
    WH_ASSERT(WH_justBetween (poly_i.maxRange (), 
			      outerParamMinRange, outerParamMaxRange));

    for (int iVertex = 0; iVertex < poly_i.nVertexs (); iVertex++) {
      WH_Vector2D point = poly_i.vertex (iVertex);
      WH_ASSERT(outerParamPoly.checkContainmentAt (point)
		== WH_Polygon2D::IN);
    }
  }

  WH_ASSERT(!WH_hasDuplication (this->offLoopEdgeSegment_s ()));
  for (vector<WH_Segment3D>::const_iterator 
	 i_seg = this->offLoopEdgeSegment_s ().begin ();
       i_seg != this->offLoopEdgeSegment_s ().end ();
       i_seg++) {
    WH_Segment3D seg_i = (*i_seg);
    WH_ASSERT(this->contains (seg_i.p0 ()));
    WH_ASSERT(this->contains (seg_i.p1 ()));
    WH_ASSERT(this->justContains (seg_i.midPoint ()));
  }

  WH_ASSERT(!WH_hasDuplication (this->offLoopParameterEdgeSegment_s ()));
  for (vector<WH_Segment2D>::const_iterator 
	 i_seg = this->offLoopParameterEdgeSegment_s ().begin ();
       i_seg != this->offLoopParameterEdgeSegment_s ().end ();
       i_seg++) {
    WH_Segment2D seg_i = (*i_seg);
    WH_ASSERT(this->checkContainmentAt (seg_i.midPoint ()) == WH_Polygon2D::IN);
    WH_ASSERT(this->checkContainmentAt (seg_i.p0 ()) == WH_Polygon2D::IN
	      || this->checkContainmentAt (seg_i.p0 ()) == WH_Polygon2D::ON);
    WH_ASSERT(this->checkContainmentAt (seg_i.p1 ()) == WH_Polygon2D::IN
	      || this->checkContainmentAt (seg_i.p1 ()) == WH_Polygon2D::ON);
  }

  WH_ASSERT(!WH_hasDuplication (this->offLoopVertexPoint_s ()));
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = this->offLoopVertexPoint_s ().begin ();
       i_point != this->offLoopVertexPoint_s ().end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    WH_ASSERT(this->justContains (point_i));
  }

  WH_ASSERT(!WH_hasDuplication (this->offLoopParameterVertexPoint_s ()));
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = this->offLoopParameterVertexPoint_s ().begin ();
       i_point != this->offLoopParameterVertexPoint_s ().end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    WH_ASSERT(this->checkContainmentAt (point_i) == WH_Polygon2D::IN);
  }

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);

    WH_ASSERT(facet_i->faceId () == this->faceId ());
    WH_ASSERT(WH_eq (facet_i->plane (), this->plane ()));
    WH_ASSERT(facet_i->frontSideIsInsideVolume ()
	      == this->frontSideIsInsideVolume ());
    WH_ASSERT(facet_i->backSideIsInsideVolume ()
	      == this->backSideIsInsideVolume ());

    WH_Triangle3D tri = facet_i->triangle ();
    WH_ASSERT(WH_between (tri.minRange (), 
			  outerMinRange, outerMaxRange));
    WH_ASSERT(WH_between (tri.maxRange (), 
			  outerMinRange, outerMaxRange));

    WH_Triangle2D paramTri = facet_i->parameterTriangle ();
    WH_ASSERT(WH_between (paramTri.minRange (), 
			  outerParamMinRange, outerParamMaxRange));
    WH_ASSERT(WH_between (paramTri.maxRange (), 
			  outerParamMinRange, outerParamMaxRange));

    for (int iVertex = 0; iVertex < 3; iVertex++) {
      WH_Vector2D param = paramTri.vertex (iVertex);
      WH_ASSERT(this->checkContainmentAt (param) == WH_Polygon2D::ON
		|| this->checkContainmentAt (param) == WH_Polygon2D::IN);
    }
    
    WH_Vector2D paramCenter = paramTri.centerOfGravity ();
    WH_ASSERT(this->checkContainmentAt (paramCenter) == WH_Polygon2D::IN);
  }

  return true;
}

bool WH_GM3D_PolygonFacet
::insertVertexPointOnEdgeOfLoop 
(const WH_Vector2D& parameter,
 WH_Polygon3D& poly_IO,
 WH_Polygon2D& paramPoly_IO)
{ 
  bool result = false;

  int nVertexs = poly_IO.nVertexs ();
  int nEdges = poly_IO.nEdges ();

  for (int iEdge = 0; iEdge < nEdges; iEdge++) {
    WH_Segment2D seg = paramPoly_IO.edge (iEdge);
    if (seg.justContains (parameter)) {
      WH_CVR_LINE;
      
      vector<WH_Vector2D> param_s;
      vector<WH_Vector3D> point_s;
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	WH_Vector2D param = paramPoly_IO.vertex (iVertex);
	WH_Vector3D point = poly_IO.vertex (iVertex);
	param_s.push_back (param);
	point_s.push_back (point);
	if (iVertex == iEdge) {
	  WH_CVR_LINE;
	  param_s.push_back (parameter);
	  point_s.push_back (_plane.positionAt (parameter));
	}
      }
      paramPoly_IO = WH_Polygon2D (param_s);
      poly_IO = WH_Polygon3D (point_s);
      
      result = true;
      break;
    }
  }
  
  return result;
}

void WH_GM3D_PolygonFacet
::insertVertexPointOnLoop 
(const WH_Vector2D& parameter)
{
  WH_CVR_LINE;

  bool found = false;

  if (_outerLoopParameterPolygon.hasVertexAt (parameter)) {
    WH_CVR_LINE;
    found = true;
  }
  
  if (!found) {
    WH_CVR_LINE;
    int nLoops = (int)innerLoopPolygon_s ().size ();
    for (int iLoop = 0; iLoop < nLoops; iLoop++) {
      if (_innerLoopParameterPolygon_s[iLoop].hasVertexAt (parameter)) {
	WH_CVR_LINE;
	found = true;
	break;
      }
    }
  }
  
  if (!found) {
    WH_CVR_LINE;
    if (this->insertVertexPointOnEdgeOfLoop 
	(parameter,
	 _outerLoopPolygon, 
	 _outerLoopParameterPolygon)) {
      WH_CVR_LINE;
      found = true;
    }
  }
      
  if (!found) {
    WH_CVR_LINE;
    int nLoops = (int)innerLoopPolygon_s ().size ();
    for (int iLoop = 0; iLoop < nLoops; iLoop++) {
      if (this->insertVertexPointOnEdgeOfLoop 
	  (parameter,
	   _innerLoopPolygon_s[iLoop], 
	   _innerLoopParameterPolygon_s[iLoop])) {
	WH_CVR_LINE;
	found = true;
	break;
      }
    }
  }
  
  WH_ASSERT(found);
}

void WH_GM3D_PolygonFacet
::insertVertexPointOffLoop 
(const WH_Vector2D& parameter)
{
  WH_CVR_LINE;

  bool found = false;

  for (vector<WH_Segment2D>::const_iterator 
	 i_seg = this->offLoopParameterEdgeSegment_s ().begin ();
       i_seg != this->offLoopParameterEdgeSegment_s ().end ();
       i_seg++) {
    WH_Segment2D seg_i = (*i_seg);
    WH_ASSERT(!seg_i.justContains (parameter));
    if (seg_i.hasVertexAt (parameter)) {
      WH_CVR_LINE;
      found = true;
      break;
    }
  }
  
  if (!found) {
    WH_CVR_LINE;
    found = WH_contains (this->offLoopParameterVertexPoint_s (), 
			 parameter);
  }

  if (!found) {
    WH_CVR_LINE;
    this->addOffLoopVertexPoint (parameter);
    found = true;
  }

  WH_ASSERT(found);
}

WH_GM3D_TriangleFacet* WH_GM3D_PolygonFacet
::createTriangleFacet 
(const WH_Vector2D& parameter0,
 const WH_Vector2D& parameter1,
 const WH_Vector2D& parameter2)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (parameter0, parameter1));
  WH_ASSERT(WH_ne (parameter0, parameter2));
  WH_ASSERT(WH_ne (parameter1, parameter2));
  WH_ASSERT(0 <= this->faceId ());

  WH_Triangle2D paramTri (parameter0, parameter1, parameter2);
  WH_GM3D_TriangleFacet* result 
    = new WH_GM3D_TriangleFacet 
    (this->plane (), 
     paramTri, 
     this->frontSideIsInsideVolume (), 
     this->backSideIsInsideVolume (),
     this->faceId ());
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_GM3D_PolygonFacet
::setFaceId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->faceId () == WH_NO_INDEX);
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
  WH_ASSERT(0 <= id);

  _faceId = id;
}

void WH_GM3D_PolygonFacet
::addInnerLoop 
(const WH_Polygon2D& parameterPolygon)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#ifndef WH_PRE_ONLY
  {
    WH_Vector2D minRange;
    WH_Vector2D maxRange;
    this->getParameterRange 
      (minRange, maxRange);
    WH_ASSERT(WH_justBetween (parameterPolygon.minRange (), 
			      minRange, maxRange));
    WH_ASSERT(WH_justBetween (parameterPolygon.maxRange (), 
			      minRange, maxRange));
  }
#endif

  WH_CVR_LINE;

  _innerLoopParameterPolygon_s.push_back (parameterPolygon);

  WH_Plane3D plane = this->plane ();
  vector<WH_Vector3D> vertex_s;
  int nVertexs = parameterPolygon.nVertexs ();
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    vertex_s.push_back 
      (plane.positionAt (parameterPolygon.vertex (iVertex)));
  }
  WH_Polygon3D polygon (vertex_s); 

  _innerLoopPolygon_s.push_back (polygon);
}
  
void WH_GM3D_PolygonFacet
::addOffLoopEdgeSegment 
(const WH_Segment2D& parameterSegment)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#ifndef WH_PRE_ONLY
  {
    WH_Vector2D minRange;
    WH_Vector2D maxRange;
    this->getParameterRange 
      (minRange, maxRange);
    WH_ASSERT(WH_between (parameterSegment.p0 (), minRange, maxRange));
    WH_ASSERT(WH_between (parameterSegment.p1 (), minRange, maxRange));
    WH_ASSERT(!WH_contains (this->offLoopParameterEdgeSegment_s (), 
			    parameterSegment));
  }
#endif

  WH_CVR_LINE;

  _offLoopParameterEdgeSegment_s.push_back (parameterSegment);

  WH_Plane3D plane = this->plane ();
  WH_Segment3D seg (plane.positionAt (parameterSegment.p0 ()),
		    plane.positionAt (parameterSegment.p1 ()));
  _offLoopEdgeSegment_s.push_back (seg);
}

void WH_GM3D_PolygonFacet
::addOffLoopVertexPoint 
(const WH_Vector2D& parameter)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#ifndef WH_PRE_ONLY
  {
    WH_Vector2D minRange;
    WH_Vector2D maxRange;
    this->getParameterRange 
      (minRange, maxRange);
    WH_ASSERT(WH_justBetween (parameter, minRange, maxRange));
    WH_ASSERT(!WH_contains (this->offLoopParameterVertexPoint_s (), 
			    parameter));
    for (vector<WH_Segment2D>::const_iterator 
	   i_seg = this->offLoopParameterEdgeSegment_s ().begin ();
	 i_seg != this->offLoopParameterEdgeSegment_s ().end ();
	 i_seg++) {
      WH_Segment2D seg_i = (*i_seg);
      WH_ASSERT(!seg_i.contains (parameter));
    }
  }
#endif

  WH_CVR_LINE;

  _offLoopParameterVertexPoint_s.push_back (parameter);

  WH_Plane3D plane = this->plane ();
  WH_Vector3D point = plane.positionAt (parameter);
  _offLoopVertexPoint_s.push_back (point);
}

void WH_GM3D_PolygonFacet
::insertEdgeSegment 
(const WH_Segment3D& segment)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);

  WH_CVR_LINE;

  WH_Plane3D plane = this->plane ();
  if (plane.contains (segment.p0 ())
      && plane.contains (segment.p1 ())) {
    WH_CVR_LINE;
    WH_Segment2D paramSeg (plane.parameterAt (segment.p0 ()),
			   plane.parameterAt (segment.p1 ()));
    
    WH_Polygon2D::ContainmentType midFlag 
      = this->checkContainmentAt (paramSeg.midPoint ());
    if (midFlag == WH_Polygon2D::IN) {
      WH_CVR_LINE;

      WH_Polygon2D::ContainmentType p0Flag 
	= this->checkContainmentAt (paramSeg.p0 ());
      WH_ASSERT(p0Flag == WH_Polygon2D::IN
		|| p0Flag == WH_Polygon2D::ON);
      if (p0Flag == WH_Polygon2D::ON) {
	WH_CVR_LINE;
	this->insertVertexPointOnLoop (paramSeg.p0 ());
      }
      
      WH_Polygon2D::ContainmentType p1Flag 
	= this->checkContainmentAt (paramSeg.p1 ());
      WH_ASSERT(p1Flag == WH_Polygon2D::IN
		|| p1Flag == WH_Polygon2D::ON);
      if (p1Flag == WH_Polygon2D::ON) {
	WH_CVR_LINE;
	this->insertVertexPointOnLoop (paramSeg.p1 ());
      }
      
      this->addOffLoopEdgeSegment (paramSeg);
    }
  }
}

void WH_GM3D_PolygonFacet
::insertVertexPoint 
(const WH_Vector3D& point)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);

  WH_CVR_LINE;

  WH_Plane3D plane = this->plane ();
  if (plane.contains (point)) {
    WH_CVR_LINE;
    WH_Vector2D param = plane.parameterAt (point);
    
    WH_Polygon2D::ContainmentType flag 
      = this->checkContainmentAt (param);
    if (flag == WH_Polygon2D::ON) {
      WH_CVR_LINE;
      this->insertVertexPointOnLoop (param);
    } else if (flag == WH_Polygon2D::IN) {
      WH_CVR_LINE;
      this->insertVertexPointOffLoop (param);
    }
  }
}

void WH_GM3D_PolygonFacet
::generateTriangleFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
  WH_ASSERT(0 <= this->faceId ());

  WH_CVR_LINE;

  WH_TSLT_Tessellator2D tessellator;

  {
    WH_Polygon2D poly = this->outerLoopParameterPolygon ();

    /* outer loop is counter clock-wise */
    int nVertexs = poly.nVertexs ();
    tessellator.beginLoop ();
    if (poly.isClockWise ()) {
      WH_CVR_LINE;
      for (int iVertex = nVertexs - 1; 0 <= iVertex; iVertex--) {
	tessellator.addLoopVertex (poly.vertex (iVertex));
      }
    } else {
      WH_CVR_LINE;
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	tessellator.addLoopVertex (poly.vertex (iVertex));
      }
    }
    tessellator.endLoop ();
  }
  
  for (vector<WH_Polygon2D>::const_iterator 
	 i_poly = this->innerLoopParameterPolygon_s ().begin ();
       i_poly != this->innerLoopParameterPolygon_s ().end ();
       i_poly++) {
    WH_Polygon2D poly_i = (*i_poly);

    /* inner loop is clock-wise */
    int nVertexs = poly_i.nVertexs ();
    tessellator.beginLoop ();
    if (poly_i.isClockWise ()) {
      WH_CVR_LINE;
      for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
	tessellator.addLoopVertex (poly_i.vertex (iVertex));
      }
    } else {
      WH_CVR_LINE;
      for (int iVertex = nVertexs - 1; 0 <= iVertex; iVertex--) {
	tessellator.addLoopVertex (poly_i.vertex (iVertex));
      }
    }
    tessellator.endLoop ();
  }

  tessellator.perform ();

  for (vector<WH_TSLT_Tessellator2D::Triangle*>::const_iterator 
	 i_tri = tessellator.triangle_s ().begin ();
       i_tri != tessellator.triangle_s ().end ();
       i_tri++) {
    WH_TSLT_Tessellator2D::Triangle* tri_i = (*i_tri);
    
    WH_Vector2D point0 
      = tessellator.loop_s ()[tri_i->loopId0]->vertex_s[tri_i->vertexId0];
    WH_Vector2D point1 
      = tessellator.loop_s ()[tri_i->loopId1]->vertex_s[tri_i->vertexId1];
    WH_Vector2D point2 
      = tessellator.loop_s ()[tri_i->loopId2]->vertex_s[tri_i->vertexId2];
    
    WH_GM3D_TriangleFacet* facet 
      = this->createTriangleFacet 
      (point0, point1, point2);
    _triangleFacet_s.push_back (facet);
  }

  /* divide <_triangleFacet_s> further by off-loop edge segments */
  WH_GM3D_TriangleFacet::divideFacetsBySegments 
    (this->offLoopEdgeSegment_s (),
     _triangleFacet_s);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(0 < this->triangleFacet_s ().size ());
#endif
}

void WH_GM3D_PolygonFacet
::reverseNormal ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->faceType () == OUTER_BOUNDARY);

  WH_CVR_LINE;

  if (_frontSideIsInsideVolume && !_backSideIsInsideVolume) {
    _frontSideIsInsideVolume = false;
    _backSideIsInsideVolume = true;
  } else if (!_frontSideIsInsideVolume && _backSideIsInsideVolume) {
    _frontSideIsInsideVolume = true;
    _backSideIsInsideVolume = false;
  } else {
    WH_ASSERT_NO_REACH;
  }

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    facet_i->reverseNormal ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->faceType () == OUTER_BOUNDARY);
#endif
}

WH_Plane3D WH_GM3D_PolygonFacet
::plane () const
{
  return _plane;
}

WH_Polygon3D WH_GM3D_PolygonFacet
::outerLoopPolygon () const
{
  return _outerLoopPolygon;
}

WH_Polygon2D WH_GM3D_PolygonFacet
::outerLoopParameterPolygon () const
{
  return _outerLoopParameterPolygon;
}

const vector<WH_Polygon3D>& WH_GM3D_PolygonFacet
::innerLoopPolygon_s () const
{
  return _innerLoopPolygon_s;
}

const vector<WH_Polygon2D>& WH_GM3D_PolygonFacet
::innerLoopParameterPolygon_s () const
{
  return _innerLoopParameterPolygon_s;
}

const vector<WH_Segment3D>& WH_GM3D_PolygonFacet
::offLoopEdgeSegment_s () const
{
  return _offLoopEdgeSegment_s;
}

const vector<WH_Segment2D>& WH_GM3D_PolygonFacet
::offLoopParameterEdgeSegment_s () const
{
  return _offLoopParameterEdgeSegment_s;
}

const vector<WH_Vector3D>& WH_GM3D_PolygonFacet
::offLoopVertexPoint_s () const
{
  return _offLoopVertexPoint_s;
}

const vector<WH_Vector2D>& WH_GM3D_PolygonFacet
::offLoopParameterVertexPoint_s () const
{
  return _offLoopParameterVertexPoint_s;
}

bool WH_GM3D_PolygonFacet
::frontSideIsInsideVolume () const
{
  return _frontSideIsInsideVolume;
}

bool WH_GM3D_PolygonFacet
::backSideIsInsideVolume () const
{
  return _backSideIsInsideVolume;
}

int WH_GM3D_PolygonFacet
::faceId () const
{
  return _faceId;
}

bool WH_GM3D_PolygonFacet
::isRegular () const
{
  return (this->offLoopEdgeSegment_s ().size () == 0
	  && this->offLoopVertexPoint_s ().size () == 0);
}

WH_GM3D_PolygonFacet::FaceType WH_GM3D_PolygonFacet
::faceType () const
{
  WH_CVR_LINE;

  if (_frontSideIsInsideVolume) {
    if (_backSideIsInsideVolume) {
      return INNER_BOUNDARY;
    } else {
      return OUTER_BOUNDARY;
    }
  } else {
    if (_backSideIsInsideVolume) {
      return OUTER_BOUNDARY;
    } else {
      return SHEET;
    }
  }
}

const vector<WH_GM3D_TriangleFacet*>& WH_GM3D_PolygonFacet
::triangleFacet_s () const
{
  return _triangleFacet_s;
}

bool WH_GM3D_PolygonFacet
::getNormalToOutsideVolume 
(WH_Vector3D& normal_OUT) const
{
  WH_CVR_LINE;
  
  if (this->frontSideIsInsideVolume ()) {
    if (this->backSideIsInsideVolume ()) {
      return false;
    } else {
      normal_OUT = -this->plane ().normal ();
      return true;
    }
  } else {
    if (this->backSideIsInsideVolume ()) {
      normal_OUT = this->plane ().normal ();
      return true;
    } else {
      WH_CVR_LINE;
      return false;
    }
  }
}
  
void WH_GM3D_PolygonFacet
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  minRange_OUT = _minRange;
  maxRange_OUT = _maxRange;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}

void WH_GM3D_PolygonFacet
::getParameterRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
{
  minRange_OUT = _parameterMinRange;
  maxRange_OUT = _parameterMaxRange;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt (minRange_OUT, maxRange_OUT));
#endif
}

WH_Polygon2D::ContainmentType WH_GM3D_PolygonFacet
::checkContainmentAt 
(const WH_Vector2D& parameter) const
{
  WH_CVR_LINE;

  WH_Polygon2D::ContainmentType result = WH_Polygon2D::OUT;

  WH_Vector2D minRange;
  WH_Vector2D maxRange;
  this->getParameterRange 
    (minRange, maxRange);
  if (WH_between (parameter, minRange, maxRange)) {
    WH_CVR_LINE;
    
    WH_Polygon2D outerParamPoly 
      = this->outerLoopParameterPolygon ();
    WH_Polygon2D::ContainmentType outerFlag 
      = outerParamPoly.checkContainmentAt (parameter);
    switch (outerFlag) {
    case WH_Polygon2D::IN:
      WH_CVR_LINE;
      {
	result = WH_Polygon2D::IN;
	for (vector<WH_Polygon2D>::const_iterator 
	       i_poly = this->innerLoopParameterPolygon_s ().begin ();
	     i_poly != this->innerLoopParameterPolygon_s ().end ();
	     i_poly++) {
	  WH_Polygon2D poly_i = (*i_poly);

	  WH_Polygon2D::ContainmentType flag 
	    = poly_i.checkContainmentAt (parameter);
	  if (flag == WH_Polygon2D::IN) {
	    WH_CVR_LINE;
	    result = WH_Polygon2D::OUT;
	    break;
	  } else if (flag == WH_Polygon2D::ON) {
	    WH_CVR_LINE;
	    result = WH_Polygon2D::ON;
	    break;
	  }
	}
      }
      break;
    case WH_Polygon2D::OUT:
      WH_CVR_LINE;
      result = WH_Polygon2D::OUT;
      break;
    case WH_Polygon2D::ON:
      WH_CVR_LINE;
      result = WH_Polygon2D::ON;
      break;
    }
  }
  
  return result;
}
  
bool WH_GM3D_PolygonFacet
::contains 
(const WH_Vector3D& point) const
{
  WH_CVR_LINE;

  bool result = false;

  if (this->plane ().contains (point)) {
    WH_CVR_LINE;
    WH_Vector2D param = this->plane ().parameterAt (point);
    WH_Polygon2D::ContainmentType flag 
      = this->checkContainmentAt (param);
    if (flag == WH_Polygon2D::IN
	|| flag == WH_Polygon2D::ON) {
      WH_CVR_LINE;
      result = true;
    }
  }

  return result;
}
  
bool WH_GM3D_PolygonFacet
::justContains 
(const WH_Vector3D& point) const
{
  WH_CVR_LINE;

  bool result = false;
  
  if (this->plane ().contains (point)) {
    WH_CVR_LINE;
    WH_Vector2D param = this->plane ().parameterAt (point);
    WH_Polygon2D::ContainmentType flag 
      = this->checkContainmentAt (param);
    if (flag == WH_Polygon2D::IN) {
      WH_CVR_LINE;
      result = true;
    }
  }

  return result;
}



/* test coverage completed */

