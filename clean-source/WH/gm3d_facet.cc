/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_facet.cc : facet module of 3-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_facet.h"
#include "connector2d.h"



/* class WH_GM3D_RegularFacetGroup */

WH_GM3D_RegularFacetGroup
::WH_GM3D_RegularFacetGroup 
(WH_GM3D_FacetBody* body,
 const WH_Plane3D& plane) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  WH_ASSERT(body->isRegular ());
  
  WH_CVR_LINE;
  
  _body = body;
  _plane = plane;
}

WH_GM3D_RegularFacetGroup
::~WH_GM3D_RegularFacetGroup () 
{
  WH_CVR_LINE;

  WH_T_Delete (_triangleFacet_s);
}

bool WH_GM3D_RegularFacetGroup
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);
  WH_ASSERT(this->body ()->isRegular ());

  return true;
}

bool WH_GM3D_RegularFacetGroup
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    WH_ASSERT(WH_eq (this->plane (), facet_i->plane ())
	      || WH_isReverse (this->plane (), facet_i->plane ()));
  }

  return true;
}

void WH_GM3D_RegularFacetGroup
::divideIncompatibleTriangleFacets ()
{
  WH_CVR_LINE;

  /* if there are any triangle facets which are incompatible along
     edges, divide them */
  
  /* collect all the vertex points of triangles into
     <allVertexPoint_s> */
  vector<WH_Vector3D> allVertexPoint_s;
  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    
    WH_Triangle3D tri = facet_i->triangle ();
    for (int iVertex = 0; iVertex < 3; iVertex++) {
      WH_Vector3D vertexPoint = tri.vertex (iVertex);
      if (!WH_contains (allVertexPoint_s, vertexPoint)) {
	WH_CVR_LINE;
	allVertexPoint_s.push_back (vertexPoint);
      }
    }
  }
  
  /* some of <_triangleFacet_s> are deleted and new triangle
     facets are added.*/
  WH_GM3D_TriangleFacet::divideFacetsByPoints
    (allVertexPoint_s, 
     _triangleFacet_s);
}

WH_Polygon2D WH_GM3D_RegularFacetGroup
::parameterPolygonOfBoundaryCluster 
(const WH_CNCT2D_SegmentCluster* boundaryCluster,
 const WH_Plane3D& planeOfNewFacet) const
{
  /* PRE-CONDITION */
  WH_ASSERT(boundaryCluster != WH_NULL);
  WH_ASSERT(WH_eq (this->plane (), planeOfNewFacet)
	    || WH_isReverse (this->plane (), planeOfNewFacet));

  WH_CVR_LINE;

  vector<WH_Segment2D> paramSegOnGroup_s;
  vector<int> endId_s;
  boundaryCluster->getRegularizedSegments 
    (paramSegOnGroup_s, endId_s);
  vector<WH_Vector2D> paramOnGroup_s;
  boundaryCluster->getVertexsFromSegments (paramSegOnGroup_s, 
					   paramOnGroup_s);

  int nVertexs = (int)paramOnGroup_s.size ();
  vector<WH_Vector3D> vertex_s (nVertexs);
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    vertex_s[iVertex] 
      = this->plane ().positionAt (paramOnGroup_s[iVertex]);
  }

  vector<WH_Vector2D> paramOnNewFacet_s (nVertexs);
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    paramOnNewFacet_s[iVertex] 
      = planeOfNewFacet.parameterAt (vertex_s[iVertex]);
  }

  WH_Polygon2D result (paramOnNewFacet_s);
  
  return result;
}

void WH_GM3D_RegularFacetGroup
::generatePolygonFacetOfCluster 
(const WH_CNCT2D_TriangleCluster* cluster)
{
  /* PRE-CONDITION */
  WH_ASSERT(cluster != WH_NULL);

  WH_CVR_LINE;

  /* use the plane and the orientation of the first triangle facet of
     <cluster> as the base of the new polygon facet */
  WH_ASSERT(0 < cluster->node_s ().size ());
  int firstTriFacetId = cluster->node_s ()[0]->id ();
  WH_ASSERT(0 <= firstTriFacetId);
  WH_ASSERT(firstTriFacetId < (int)this->triangleFacet_s ().size ());
  WH_GM3D_TriangleFacet* firstTriFacet 
    = this->triangleFacet_s ()[firstTriFacetId];
  
  WH_CNCT2D_SegmentCluster* outerLoopCluster 
    = cluster->outerLoopBoundaryCluster ();

  WH_Polygon2D outerLoopParamPoly
    = this->parameterPolygonOfBoundaryCluster 
    (outerLoopCluster, firstTriFacet->plane ());

  WH_GM3D_PolygonFacet* newFacet = this->body ()->createPolygonFacet 
    (firstTriFacet->plane (),
     outerLoopParamPoly,
     firstTriFacet->frontSideIsInsideVolume (),
     firstTriFacet->backSideIsInsideVolume ());
  
  /* set faceId to <newFacet> */
  this->body ()->addPolygonFacet (newFacet);

  /* add inner loops */
  for (vector<WH_CNCT2D_SegmentCluster*>::const_iterator
	 i_cluster = outerLoopCluster->innerLoopCluster_s ().begin ();
       i_cluster != outerLoopCluster->innerLoopCluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_SegmentCluster* cluster_i = (*i_cluster);
    WH_Polygon2D paramPoly
      = this->parameterPolygonOfBoundaryCluster 
      (cluster_i, firstTriFacet->plane ());
    newFacet->addInnerLoop (paramPoly);
  }
}

void WH_GM3D_RegularFacetGroup
::addTriangleFacet 
(WH_GM3D_TriangleFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(facet != WH_NULL);
  WH_ASSERT(WH_eq (facet->plane (), this->plane ())
	    || WH_isReverse (facet->plane (), this->plane ()));
  
  _triangleFacet_s.push_back (facet);
}

void WH_GM3D_RegularFacetGroup
::generatePolygonFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->triangleFacet_s ().size ());

  WH_CVR_LINE;

  this->divideIncompatibleTriangleFacets ();

  /* group triangle facets connected each other using triangle
     connector */
  
  WH_Plane3D plane = this->plane ();

  WH_CNCT2D_TriangleConnector connector;
  
  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    
    WH_Triangle3D tri = facet_i->triangle ();
    WH_Triangle2D paramTri 
      (plane.parameterAt (tri.vertex (0)),
       plane.parameterAt (tri.vertex (1)),
       plane.parameterAt (tri.vertex (2)));
    
    WH_CNCT2D_TriangleNode* node 
      = new WH_CNCT2D_TriangleNode (paramTri);
    connector.addNode (node);
  }
  
  connector.connect ();
  connector.identifyClusters ();
  connector.extractBoundary ();
  
  /* for each cluster, generate a polygon facet */
  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = connector.cluster_s ().begin ();
       i_cluster != connector.cluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_TriangleCluster* cluster_i 
      = (WH_CNCT2D_TriangleCluster*)(*i_cluster);
    this->generatePolygonFacetOfCluster (cluster_i);
  }
}

WH_GM3D_FacetBody* WH_GM3D_RegularFacetGroup
::body () const
{
  return _body;
}

WH_Plane3D WH_GM3D_RegularFacetGroup
::plane () const
{
  return _plane;
}

const vector<WH_GM3D_TriangleFacet*>& WH_GM3D_RegularFacetGroup
::triangleFacet_s () const
{
  return _triangleFacet_s;
}



/* class WH_GM3D_NonRegularFacetGroup */

WH_GM3D_NonRegularFacetGroup
::WH_GM3D_NonRegularFacetGroup 
(WH_GM3D_FacetBody* body,
 int faceId) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  WH_ASSERT(!body->isRegular ());
  WH_ASSERT(0 <= faceId);
  
  WH_CVR_LINE;
  
  _body = body;
  _faceId = faceId;
}

WH_GM3D_NonRegularFacetGroup
::~WH_GM3D_NonRegularFacetGroup () 
{
  WH_CVR_LINE;

  WH_T_Delete (_triangleFacet_s);
}

bool WH_GM3D_NonRegularFacetGroup
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->body () != WH_NULL);
  WH_ASSERT(!this->body ()->isRegular ());
  WH_ASSERT(0 <= this->faceId ());

  return true;
}

bool WH_GM3D_NonRegularFacetGroup
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    WH_ASSERT(this->faceId () == facet_i->faceId ());
  }

  return true;
}

WH_Polygon2D WH_GM3D_NonRegularFacetGroup
::parameterPolygonOfBoundaryCluster 
(const WH_CNCT2D_SegmentCluster* boundaryCluster) const
{
  /* PRE-CONDITION */
  WH_ASSERT(boundaryCluster != WH_NULL);

  WH_CVR_LINE;

  vector<WH_Segment2D> paramSeg_s;
  vector<int> endId_s;
  boundaryCluster->getRegularizedSegments 
    (paramSeg_s, endId_s);
  vector<WH_Vector2D> param_s;
  boundaryCluster->getVertexsFromSegments (paramSeg_s, 
					   param_s);
  WH_Polygon2D result (param_s);
  
  return result;
}

void WH_GM3D_NonRegularFacetGroup
::generatePolygonFacetOfCluster 
(const WH_CNCT2D_TriangleCluster* cluster)
{
  /* PRE-CONDITION */
  WH_ASSERT(cluster != WH_NULL);

  WH_CVR_LINE;

  /* use the plane and the orientation of the first triangle facet of
     <cluster> as the base of the new polygon facet */
  WH_ASSERT(0 < cluster->node_s ().size ());
  int firstTriFacetId = cluster->node_s ()[0]->id ();
  WH_ASSERT(0 <= firstTriFacetId);
  WH_ASSERT(firstTriFacetId < (int)this->triangleFacet_s ().size ());
  WH_GM3D_TriangleFacet* firstTriFacet 
    = this->triangleFacet_s ()[firstTriFacetId];
  WH_Plane3D plane = firstTriFacet->plane ();
  WH_ASSERT(0 < this->triangleFacet_s ().size ());
  WH_ASSERT(WH_eq (plane, this->triangleFacet_s ()[0]->plane ()));

  /* use the plane and the orientation of the first triangle facet of
     the group as the base of the new polygon facet */
  
  WH_CNCT2D_SegmentCluster* outerLoopCluster 
    = cluster->outerLoopBoundaryCluster ();

  WH_Polygon2D outerLoopParamPoly
    = this->parameterPolygonOfBoundaryCluster 
    (outerLoopCluster);

  WH_GM3D_PolygonFacet* newFacet = this->body ()->createPolygonFacet 
    (plane,
     outerLoopParamPoly,
     firstTriFacet->frontSideIsInsideVolume (),
     firstTriFacet->backSideIsInsideVolume ());
  
  /* set faceId to <newFacet> */
  this->body ()->addPolygonFacet (newFacet);

  /* add inner loops */
  for (vector<WH_CNCT2D_SegmentCluster*>::const_iterator
	 i_cluster = outerLoopCluster->innerLoopCluster_s ().begin ();
       i_cluster != outerLoopCluster->innerLoopCluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_SegmentCluster* cluster_i = (*i_cluster);
    WH_Polygon2D paramPoly
      = this->parameterPolygonOfBoundaryCluster 
      (cluster_i);
    newFacet->addInnerLoop (paramPoly);
  }
}

void WH_GM3D_NonRegularFacetGroup
::addTriangleFacet 
(WH_GM3D_TriangleFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(facet != WH_NULL);
  WH_ASSERT(facet->faceId () == this->faceId ());
  
  _triangleFacet_s.push_back (facet);
}

void WH_GM3D_NonRegularFacetGroup
::generatePolygonFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->triangleFacet_s ().size ());

  WH_CVR_LINE;

  /* group triangle facets connected each other using triangle
     connector */
  
  /* use the plane and the orientation of the first triangle facet of
     the group as the base of the new polygon facet */
  WH_ASSERT(0 < this->triangleFacet_s ().size ());
  WH_GM3D_TriangleFacet* firstTriFacet 
    = this->triangleFacet_s ()[0];
  WH_Plane3D plane = firstTriFacet->plane ();
  
  WH_CNCT2D_NonRegularTriangleConnector connector;
  
  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);
    
    WH_Triangle3D tri = facet_i->triangle ();
    WH_Triangle2D paramTri 
      (plane.parameterAt (tri.vertex (0)),
       plane.parameterAt (tri.vertex (1)),
       plane.parameterAt (tri.vertex (2)));
    
    WH_CNCT2D_TriangleNode* node 
      = new WH_CNCT2D_TriangleNode (paramTri);
    connector.addNode (node);
  }
  
  /* add all the explicit segment facets on <plane> as internal
     boundary of <connector> */
  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = this->body ()->segmentFacet_s ().begin ();
       i_facet != this->body ()->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    
    if (facet_i->isExplicit ()) {
      WH_CVR_LINE;
      WH_Segment3D seg = facet_i->segment ();
      if (plane.contains (seg.p0 ())
	  && plane.contains (seg.p1 ())) {
	WH_CVR_LINE;
	WH_Segment2D paramSeg 
	  (plane.parameterAt (seg.p0 ()),
	   plane.parameterAt (seg.p1 ()));
	connector.addInternalBoundarySegment (paramSeg);
      }
    }
  }

  connector.connect ();
  connector.identifyClusters ();
  connector.extractBoundary ();
  
  /* for each cluster, generate a polygon facet */
  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = connector.cluster_s ().begin ();
       i_cluster != connector.cluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_TriangleCluster* cluster_i 
      = (WH_CNCT2D_TriangleCluster*)(*i_cluster);
    this->generatePolygonFacetOfCluster (cluster_i);
  }
}

WH_GM3D_FacetBody* WH_GM3D_NonRegularFacetGroup
::body () const
{
  return _body;
}

int WH_GM3D_NonRegularFacetGroup
::faceId () const
{
  return _faceId;
}

const vector<WH_GM3D_TriangleFacet*>& WH_GM3D_NonRegularFacetGroup
::triangleFacet_s () const
{
  return _triangleFacet_s;
}



/* class WH_GM3D_FacetBody */

int WH_GM3D_FacetBody::_faceCount = 0;

WH_GM3D_FacetBody
::WH_GM3D_FacetBody (bool isRegular) 
{
  WH_CVR_LINE;

  _isRegular = isRegular;
  _normalIsReversed = false;
  _inOutChecker = WH_NULL;
}

WH_GM3D_FacetBody
::~WH_GM3D_FacetBody () 
{
  WH_CVR_LINE;

  delete _inOutChecker;
  WH_T_Delete (_segmentFacet_s);
  WH_T_Delete (_polygonFacet_s);
  WH_T_Delete (_triangleFacet_s);
}

bool WH_GM3D_FacetBody
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_GM3D_FacetBody
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_T_AssureInvariant (_segmentFacet_s);
  WH_T_AssureInvariant (_polygonFacet_s);
  WH_T_AssureInvariant (_triangleFacet_s);
  


  int nVertexs = (int)this->vertexPoint_s ().size ();
  
  int nEdges = (int)this->segmentFacet_s ().size ();
  
  /* count number of faces of each type */
  int nFaces = (int)this->polygonFacet_s ().size ();
  int sheetFaceCount = 0;
  int outerBoundaryFaceCount = 0;
  int innerBoundaryFaceCount = 0;
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    switch (facet_i->faceType ()) {
    case WH_GM3D_PolygonFacet::SHEET:
      sheetFaceCount++;
      break;
    case WH_GM3D_PolygonFacet::INNER_BOUNDARY:
      innerBoundaryFaceCount++;
      break;
    case WH_GM3D_PolygonFacet::OUTER_BOUNDARY:
      outerBoundaryFaceCount++;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }
  
  switch (this->bodyType ()) {
  case EMPTY:
    WH_CVR_LINE;
    
    WH_ASSERT(nVertexs == 0);

    WH_ASSERT(nEdges == 0);

    WH_ASSERT(nFaces == 0);
    
    break;
  case DOT_SET:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);

    WH_ASSERT(nEdges == 0);

    WH_ASSERT(nFaces == 0);
    
    break;
  case WIRE_SET:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);

    WH_ASSERT(0 < nEdges);

    WH_ASSERT(nFaces == 0);
    
    break;
  case SHEET_SET:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);

    WH_ASSERT(0 < nEdges);

    WH_ASSERT(0 < nFaces);
    WH_ASSERT(0 < sheetFaceCount);
    WH_ASSERT(outerBoundaryFaceCount == 0);
    WH_ASSERT(innerBoundaryFaceCount == 0);
    WH_ASSERT(nFaces == sheetFaceCount);
    
    break;
  case VOLUME:
    WH_CVR_LINE;
    
    if (this->isRegular ()) {
      WH_ASSERT(nVertexs == 0);
    } else {
      WH_ASSERT(0 < nVertexs);
    }

    if (this->isRegular ()) {
      WH_ASSERT(nEdges == 0);
    } else {
      WH_ASSERT(0 < nEdges);
    }

    WH_ASSERT(0 < nFaces);
    WH_ASSERT(sheetFaceCount == 0);
    WH_ASSERT(0 < outerBoundaryFaceCount);
    WH_ASSERT(innerBoundaryFaceCount == 0);
    WH_ASSERT(nFaces == outerBoundaryFaceCount);
    
    break;
  case OTHER:
    WH_CVR_LINE;
    
    WH_ASSERT(0 < nVertexs);
    
    WH_ASSERT(0 < nEdges);
    
    WH_ASSERT(0 < nFaces);
    WH_ASSERT(0 <= sheetFaceCount);
    WH_ASSERT(0 <= outerBoundaryFaceCount);
    WH_ASSERT(0 <= innerBoundaryFaceCount);
    
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
  
  if (this->isRegular ()) {
    WH_CVR_LINE;
    BodyType bodyType = this->bodyType ();
    WH_ASSERT(bodyType == EMPTY
	      || bodyType == VOLUME);
    if (bodyType == VOLUME) {
      WH_CVR_LINE;
      for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	     i_facet = this->polygonFacet_s ().begin ();
	   i_facet != this->polygonFacet_s ().end ();
	   i_facet++) {
	WH_GM3D_PolygonFacet* facet_i = (*i_facet);
	WH_ASSERT(facet_i->isRegular ());
      }
    }
  }

  WH_ASSERT(!WH_hasDuplication (this->vertexPoint_s ()));
  
  WH_ASSERT(!WH_T_HasDuplication (this->segmentFacet_s ()));

  /* check intersection of segment facets */
  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    WH_Segment3D segment_i = facet_i->segment ();
    
    for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	   j_facet = i_facet + 1;
	 j_facet != this->segmentFacet_s ().end ();
	 j_facet++) {
      WH_GM3D_SegmentFacet* facet_j = (*j_facet);
      WH_Segment3D segment_j = facet_j->segment ();
      
      WH_SegmentIntersector3D intersector (segment_j, segment_i);  
      intersector.perform ();
      WH_ASSERT(!intersector.intersects ()
		|| intersector.intersectsAtVertexOnly ());
    }
  }
  
  WH_ASSERT(!WH_T_HasDuplication (this->triangleFacet_s ()));

  /* NEED TO REDEFINE */
  /* check intersection of triangle facets */

  WH_ASSERT(!WH_T_HasDuplication (this->polygonFacet_s ()));

  /* NEED TO REDEFINE */
  /* check intersection of polygon facets */

  return true;
}

void WH_GM3D_FacetBody
::addVertexPoint (const WH_Vector3D& point)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isRegular ());
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_contains (this->vertexPoint_s (), point));
#endif

  _vertexPoint_s.push_back (point);
}

void WH_GM3D_FacetBody
::addSegmentFacet (WH_GM3D_SegmentFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isRegular ());
  WH_ASSERT(facet != WH_NULL);
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->segmentFacet_s (), facet));
#endif
  
  _segmentFacet_s.push_back (facet);
}

void WH_GM3D_FacetBody
::addPolygonFacet (WH_GM3D_PolygonFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(facet != WH_NULL);
#ifndef NDEBUG
  if (this->isRegular ()) {
    WH_ASSERT(facet->faceType () 
	      == WH_GM3D_PolygonFacet::OUTER_BOUNDARY);
  }
#endif
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->polygonFacet_s (), facet));
#endif
  
  _polygonFacet_s.push_back (facet);
  facet->setFaceId (_faceCount);
  _faceCount++;
}

void WH_GM3D_FacetBody
::generateTriangleFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->triangleFacet_s ().size () == 0);

  WH_CVR_LINE;

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = _polygonFacet_s.begin ();
       i_facet != _polygonFacet_s.end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    facet_i->generateTriangleFacets ();
  }
}

void WH_GM3D_FacetBody
::addTriangleFacet (WH_GM3D_TriangleFacet* facet)
{
  /* PRE-CONDITION */
  WH_ASSERT(facet != WH_NULL);
#ifndef NDEBUG
  if (this->isRegular ()) {
    WH_ASSERT(facet->faceType () 
	      == WH_GM3D_TriangleFacet::OUTER_BOUNDARY);
  }
#endif
#ifndef WH_PRE_ONLY
  WH_ASSERT(!WH_T_Exists (this->triangleFacet_s (), facet));
#endif
  
  _triangleFacet_s.push_back (facet);
}

void WH_GM3D_FacetBody
::generateRegularPolygonFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isRegular ());
  WH_ASSERT(this->polygonFacet_s ().size () == 0);

  WH_CVR_LINE;

  /* group triangle facets on the same plane into <group_s> */
  vector<WH_GM3D_RegularFacetGroup*> group_s;
  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);

    WH_Plane3D plane = facet_i->plane ();

    WH_GM3D_RegularFacetGroup* groupOfFacet = WH_NULL;
    for (vector<WH_GM3D_RegularFacetGroup*>::const_iterator 
	   i_group = group_s.begin ();
	 i_group != group_s.end ();
	 i_group++) {
      WH_GM3D_RegularFacetGroup* group_i = (*i_group);

      if (WH_eq (group_i->plane (), plane)
	  || WH_isReverse (group_i->plane (), plane)) {
	WH_CVR_LINE;
	groupOfFacet = group_i;
	break;
      }
    }
    
    if (groupOfFacet == WH_NULL) {
      WH_CVR_LINE;
      groupOfFacet 
	= new WH_GM3D_RegularFacetGroup (this, plane);
      WH_ASSERT(groupOfFacet != WH_NULL);
      group_s.push_back (groupOfFacet);
    }

    WH_ASSERT(groupOfFacet != WH_NULL);
    
    groupOfFacet->addTriangleFacet (facet_i);
  }

  /* move ownership of triangle facets from <_triangleFacet_s> to
     <group_s> */
  _triangleFacet_s.clear ();

  for (vector<WH_GM3D_RegularFacetGroup*>::const_iterator 
	 i_group = group_s.begin ();
       i_group != group_s.end ();
       i_group++) {
    WH_GM3D_RegularFacetGroup* group_i = (*i_group);
    group_i->generatePolygonFacets ();
  }

#ifndef WH_PRE_ONLY
  WH_T_AssureInvariant (group_s);
#endif

  WH_T_Delete (group_s);
  group_s.clear ();

  /* collect all the vertex points of polygon facets into
     <allVertexPoint_s> */
  vector<WH_Vector3D> allVertexPoint_s;
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    {
      WH_Polygon3D poly = facet_i->outerLoopPolygon ();
      for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	WH_Vector3D vertexPoint = poly.vertex (iVertex);
	if (!WH_contains (allVertexPoint_s, vertexPoint)) {
	  WH_CVR_LINE;
	  allVertexPoint_s.push_back (vertexPoint);
	}
      }
    }
    for (vector<WH_Polygon3D>::const_iterator 
	   i_poly = facet_i->innerLoopPolygon_s ().begin ();
	 i_poly != facet_i->innerLoopPolygon_s ().end ();
	 i_poly++) {
      WH_Polygon3D poly = (*i_poly);
      for (int iVertex = 0; iVertex < poly.nVertexs (); iVertex++) {
	WH_Vector3D vertexPoint = poly.vertex (iVertex);
	if (!WH_contains (allVertexPoint_s, vertexPoint)) {
	  WH_CVR_LINE;
	  allVertexPoint_s.push_back (vertexPoint);
	}
      }
    }
  }

  /* insert <allVertexPoint_s> into polygon facets as 180 degree
     corner angle vertexs */
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);

    for (vector<WH_Vector3D>::const_iterator 
	   i_point = allVertexPoint_s.begin ();
	 i_point != allVertexPoint_s.end ();
	 i_point++) {
      WH_Vector3D point_i = (*i_point);
      facet_i->insertVertexPoint (point_i);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#endif
}

void WH_GM3D_FacetBody
::generateNonRegularPolygonFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isRegular ());
  WH_ASSERT(this->polygonFacet_s ().size () == 0);

  WH_CVR_LINE;

  /* group triangle facets on the same face id into <group_s> */
  vector<WH_GM3D_NonRegularFacetGroup*> group_s;
  for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	 i_facet = this->triangleFacet_s ().begin ();
       i_facet != this->triangleFacet_s ().end ();
       i_facet++) {
    WH_GM3D_TriangleFacet* facet_i = (*i_facet);

    WH_GM3D_NonRegularFacetGroup* groupOfFacet = WH_NULL;
    for (vector<WH_GM3D_NonRegularFacetGroup*>::const_iterator 
	   i_group = group_s.begin ();
	 i_group != group_s.end ();
	 i_group++) {
      WH_GM3D_NonRegularFacetGroup* group_i = (*i_group);
      if (group_i->faceId () == facet_i->faceId ()) {
	WH_CVR_LINE;
	groupOfFacet = group_i;
	break;
      }
    }
    
    if (groupOfFacet == WH_NULL) {
      WH_CVR_LINE;
      groupOfFacet 
	= new WH_GM3D_NonRegularFacetGroup (this, facet_i->faceId ());
      WH_ASSERT(groupOfFacet != WH_NULL);
      group_s.push_back (groupOfFacet);
    }
    WH_ASSERT(groupOfFacet != WH_NULL);
    
    groupOfFacet->addTriangleFacet (facet_i);
  }

  /* move ownership of triangle facets from <_triangleFacet_s> to
     <group_s> */
  _triangleFacet_s.clear ();

  for (vector<WH_GM3D_NonRegularFacetGroup*>::const_iterator 
	 i_group = group_s.begin ();
       i_group != group_s.end ();
       i_group++) {
    WH_GM3D_NonRegularFacetGroup* group_i = (*i_group);
    group_i->generatePolygonFacets ();
  }

#ifndef WH_PRE_ONLY
  WH_T_AssureInvariant (group_s);
#endif

  WH_T_Delete (group_s);
  group_s.clear ();

  /* insert off-loop edges, off-loop vertexs and 180 corner vertexs
     into polygon facets */

  vector<WH_Segment3D> explicitSeg_s;
  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    if (facet_i->isExplicit ()) {
      WH_CVR_LINE;
      explicitSeg_s.push_back (facet_i->segment ());
    }
  }

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    
    /* insert explicit edge segments as off-loop edge segments */
    for (vector<WH_Segment3D>::const_iterator 
	   i_seg = explicitSeg_s.begin ();
	 i_seg != explicitSeg_s.end ();
	 i_seg++) {
      WH_Segment3D seg_i = (*i_seg);
      facet_i->insertEdgeSegment (seg_i);
    }
    
    /* insert vertex points as 180 degreen corner vertexs or off-loop
       vertexs */
    for (vector<WH_Vector3D>::const_iterator 
	   i_point = this->vertexPoint_s ().begin ();
	 i_point != this->vertexPoint_s ().end ();
	 i_point++) {
      WH_Vector3D point_i = (*i_point);
      facet_i->insertVertexPoint (point_i);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#endif
}

void WH_GM3D_FacetBody
::generatePolygonFacets ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->polygonFacet_s ().size () == 0);

  WH_CVR_LINE;

  if (this->isRegular ()) {
    WH_CVR_LINE;
    this->generateRegularPolygonFacets ();
  } else {
    WH_CVR_LINE;
    this->generateNonRegularPolygonFacets ();
  }

  this->generateTriangleFacets ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
#endif
}

void WH_GM3D_FacetBody
::reverseNormal ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->bodyType () == VOLUME);
  
  WH_CVR_LINE;

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    facet_i->reverseNormal ();
  }

  if (_normalIsReversed) {
    WH_CVR_LINE;
    _normalIsReversed = false;
  } else {
    WH_CVR_LINE;
    _normalIsReversed = true;
  }
}

void WH_GM3D_FacetBody
::setUpInOutCheck ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->bodyType () == VOLUME
	    || this->bodyType () == OTHER);
  WH_ASSERT(_inOutChecker == WH_NULL);
  WH_ASSERT(0 < this->polygonFacet_s ().size ());
  WH_ASSERT(this->triangleFacet_s ().size () == 0);
  
  WH_CVR_LINE;

  double minLength = WH::HUGE_VALUE;
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_pfacet = this->polygonFacet_s ().begin ();
       i_pfacet != this->polygonFacet_s ().end ();
       i_pfacet++) {
    WH_GM3D_PolygonFacet* pfacet_i = (*i_pfacet);
    
    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = pfacet_i->triangleFacet_s ().begin ();
	 i_facet != pfacet_i->triangleFacet_s ().end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);
      
      WH_Triangle3D tri = facet_i->triangle ();
      minLength = WH_min (minLength, tri.edge (0).length ());
      minLength = WH_min (minLength, tri.edge (1).length ());
      minLength = WH_min (minLength, tri.edge (2).length ());
    }
  }
  
  _inOutChecker = new WH_InOutChecker3D (minLength);
  WH_ASSERT(_inOutChecker != WH_NULL);
  
  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_pfacet = this->polygonFacet_s ().begin ();
       i_pfacet != this->polygonFacet_s ().end ();
       i_pfacet++) {
    WH_GM3D_PolygonFacet* pfacet_i = (*i_pfacet);
    
    if (pfacet_i->faceType () == WH_GM3D_PolygonFacet::OUTER_BOUNDARY) {
      WH_Vector3D normal;
      pfacet_i->getNormalToOutsideVolume 
	(normal);
      if (this->normalIsReversed ()) {
	WH_CVR_LINE;
	normal = -normal;
      }

      WH_ASSERT(0 < pfacet_i->triangleFacet_s ().size ());
      for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	     i_facet = pfacet_i->triangleFacet_s ().begin ();
	   i_facet != pfacet_i->triangleFacet_s ().end ();
	   i_facet++) {
	WH_GM3D_TriangleFacet* facet_i = (*i_facet);
	WH_Triangle3D tri = facet_i->triangle ();
	_inOutChecker->addFace 
	  (tri.vertex (0), tri.vertex (1), tri.vertex (2), 
	   normal);
      }
    }
  }
  _inOutChecker->setUp ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_inOutChecker != WH_NULL);
#endif
}

WH_GM3D_SegmentFacet* WH_GM3D_FacetBody
::createSegmentFacet 
(const WH_Vector3D& firstPoint,
 const WH_Vector3D& lastPoint,
 bool isExplicit)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (firstPoint, lastPoint));

  WH_GM3D_SegmentFacet* result 
    = new WH_GM3D_SegmentFacet 
    (firstPoint, lastPoint, isExplicit);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_TriangleFacet* WH_GM3D_FacetBody
::createTriangleFacet 
(const WH_Plane3D& plane,
 const WH_Triangle2D& parameterTriangle,
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume,
 int faceId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceId);

  WH_GM3D_TriangleFacet* result 
    = new WH_GM3D_TriangleFacet 
    (plane, parameterTriangle, 
     frontSideIsInsideVolume, backSideIsInsideVolume,
     faceId);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_PolygonFacet* WH_GM3D_FacetBody
::createPolygonFacet 
(const WH_Plane3D& plane,
 const WH_Polygon2D& outerLoopParameterPolygon, 
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume)
{
  WH_GM3D_PolygonFacet* result 
    = new WH_GM3D_PolygonFacet 
    (plane, outerLoopParameterPolygon, 
     frontSideIsInsideVolume, backSideIsInsideVolume);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

bool WH_GM3D_FacetBody
::isRegular () const
{
  return _isRegular;
}

bool WH_GM3D_FacetBody
::normalIsReversed () const
{
  return _normalIsReversed;
}

const vector<WH_Vector3D>& WH_GM3D_FacetBody
::vertexPoint_s () const
{
  return _vertexPoint_s;
}
  
const vector<WH_GM3D_SegmentFacet*>& WH_GM3D_FacetBody
::segmentFacet_s () const
{
  return _segmentFacet_s;
}

const vector<WH_GM3D_PolygonFacet*>& WH_GM3D_FacetBody
::polygonFacet_s () const
{
  return _polygonFacet_s;
}

const vector<WH_GM3D_TriangleFacet*>& WH_GM3D_FacetBody
::triangleFacet_s () const
{
  return _triangleFacet_s;
}

WH_GM3D_FacetBody::BodyType WH_GM3D_FacetBody
::bodyType () const
{
  WH_CVR_LINE;

  BodyType result = OTHER;

  int nVertexs = (int)this->vertexPoint_s ().size ();
  int nEdges = (int)this->segmentFacet_s ().size ();
  int nFaces = (int)this->polygonFacet_s ().size ();

  if (nVertexs == 0 && nEdges == 0 && nFaces == 0) {
    WH_CVR_LINE;
    result = EMPTY;
  } else if (nEdges == 0 && nFaces == 0) {
    WH_CVR_LINE;
    result = DOT_SET;
  } else if (nFaces == 0) {
    WH_CVR_LINE;
    result = WIRE_SET;
  } else {
    WH_CVR_LINE;

    int sheetFaceCount = 0;
    int outerBoundaryFaceCount = 0;
    int innerBoundaryFaceCount = 0;
    for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	   i_facet = this->polygonFacet_s ().begin ();
	 i_facet != this->polygonFacet_s ().end ();
	 i_facet++) {
      WH_GM3D_PolygonFacet* facet_i = (*i_facet);
      
      switch (facet_i->faceType ()) {
      case WH_GM3D_PolygonFacet::SHEET:
	WH_CVR_LINE;
	sheetFaceCount++;
	break;
      case WH_GM3D_PolygonFacet::INNER_BOUNDARY:
	WH_CVR_LINE;
	innerBoundaryFaceCount++;
	break;
      case WH_GM3D_PolygonFacet::OUTER_BOUNDARY:
	WH_CVR_LINE;
	outerBoundaryFaceCount++;
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }

    if (0 < sheetFaceCount 
	&& outerBoundaryFaceCount == 0 
	&& innerBoundaryFaceCount == 0) {
      WH_CVR_LINE;
      result = SHEET_SET;
    } else if (sheetFaceCount == 0
	       && 0 < outerBoundaryFaceCount 
	       && innerBoundaryFaceCount == 0) {
      WH_CVR_LINE;
      result = VOLUME;
    } else {
      WH_CVR_LINE;
      result = OTHER;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case EMPTY:
    WH_ASSERT(nVertexs == 0);
    WH_ASSERT(nEdges == 0);
    WH_ASSERT(nFaces == 0);
    break;
  case DOT_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(nEdges == 0);
    WH_ASSERT(nFaces == 0);
    break;
  case WIRE_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(nFaces == 0);
    break;
  case SHEET_SET:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 < nFaces);
    break;
  case VOLUME:
    if (this->isRegular ()) {
      WH_ASSERT(nVertexs == 0);
      WH_ASSERT(nEdges == 0);
    } else {
      WH_ASSERT(0 < nVertexs);
      WH_ASSERT(0 < nEdges);
    }
    WH_ASSERT(0 < nFaces);
    break;
  case OTHER:
    WH_ASSERT(0 < nVertexs);
    WH_ASSERT(0 < nEdges);
    WH_ASSERT(0 < nFaces);
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
#endif  

  return result;
}

WH_InOutChecker3D::ContainmentType WH_GM3D_FacetBody
::checkContainmentAt 
(const WH_Vector3D& point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(_inOutChecker != WH_NULL);
  
  WH_CVR_LINE;

  WH_InOutChecker3D::ContainmentType result 
    = _inOutChecker->checkContainmentAt (point);
  
  if (this->normalIsReversed ()) {
    WH_CVR_LINE;
    
    switch (result) {
    case WH_InOutChecker3D::IN:
      WH_CVR_LINE;
      result = WH_InOutChecker3D::OUT;
      break;
    case WH_InOutChecker3D::OUT:
      WH_CVR_LINE;
      result = WH_InOutChecker3D::IN;
      break;
    case WH_InOutChecker3D::ON:
      WH_CVR_LINE;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }

  return result;
}

WH_GM3D_SegmentFacet* WH_GM3D_FacetBody
::findSegmentFacet 
(const WH_Vector3D& point0,
 const WH_Vector3D& point1) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (point0, point1));

  WH_CVR_LINE;

  WH_GM3D_SegmentFacet* result = WH_NULL;

  for (vector<WH_GM3D_SegmentFacet*>::const_iterator 
	 i_facet = this->segmentFacet_s ().begin ();
       i_facet != this->segmentFacet_s ().end ();
       i_facet++) {
    WH_GM3D_SegmentFacet* facet_i = (*i_facet);
    WH_Segment3D seg = facet_i->segment ();
    if (seg.hasVertexAt (point0) && seg.hasVertexAt (point1)) {
      WH_CVR_LINE;
      result = facet_i;
      break;
    }
  }  

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

WH_GM3D_PolygonFacet* WH_GM3D_FacetBody
::findPolygonFacet 
(const vector<WH_Vector3D>& point_s) const
{
  /* PRE-CONDITION */
  WH_ASSERT(3 <= point_s.size ());

  WH_CVR_LINE;

  WH_GM3D_PolygonFacet* result = WH_NULL;

  for (vector<WH_GM3D_PolygonFacet*>::const_iterator 
	 i_facet = this->polygonFacet_s ().begin ();
       i_facet != this->polygonFacet_s ().end ();
       i_facet++) {
    WH_GM3D_PolygonFacet* facet_i = (*i_facet);
    WH_Polygon3D poly = facet_i->outerLoopPolygon ();
    if (poly.hasVertexAtEveryPointIn (point_s)) {
      WH_CVR_LINE;
      result = facet_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}



/* test coverage completed */



