/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_facet_tri.cc : facet module of 3-D geometry modeler */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_facet.h"



/* class WH_GM3D_TriangleFacet */

void WH_GM3D_TriangleFacet
::divideFacetsByTriangles 
(const vector<WH_Triangle3D>& triangleBy_s,
 vector<WH_GM3D_TriangleFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Triangle3D>::const_iterator 
	 i_tri = triangleBy_s.begin ();
       i_tri != triangleBy_s.end ();
       i_tri++) {
    WH_Triangle3D tri_i = (*i_tri);
    
    vector<WH_GM3D_TriangleFacet*> remainingFacet_s;
    vector<WH_GM3D_TriangleFacet*> deletedFacet_s;
    vector<WH_GM3D_TriangleFacet*> newFacet_s;
    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);

      vector<WH_GM3D_TriangleFacet*> facet_s;
      if (facet_i->createDividedFacetsByTriangle (tri_i,
						  facet_s)) {
	WH_CVR_LINE;
	deletedFacet_s.push_back (facet_i);
	WH_T_Add (facet_s, newFacet_s);
      } else {
	WH_CVR_LINE;
	remainingFacet_s.push_back (facet_i);
      }
    }
    WH_ASSERT(deletedFacet_s.size () + remainingFacet_s.size ()
	      == facet_s_IO.size ());
    
    if (0 < newFacet_s.size ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < deletedFacet_s.size ());
      WH_ASSERT(2 <= newFacet_s.size ());
      WH_T_Delete (deletedFacet_s);
      deletedFacet_s.clear ();
      WH_T_Move (remainingFacet_s, facet_s_IO);
      WH_T_Add (newFacet_s, facet_s_IO);
    }
  }

  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < facet_s_IO.size ());
#endif
}

void WH_GM3D_TriangleFacet
::divideFacetsBySegments 
(const vector<WH_Segment3D>& segmentBy_s,
 vector<WH_GM3D_TriangleFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Segment3D>::const_iterator 
	 i_seg = segmentBy_s.begin ();
       i_seg != segmentBy_s.end ();
       i_seg++) {
    WH_Segment3D seg_i = (*i_seg);
    
    vector<WH_GM3D_TriangleFacet*> remainingFacet_s;
    vector<WH_GM3D_TriangleFacet*> deletedFacet_s;
    vector<WH_GM3D_TriangleFacet*> newFacet_s;
    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);

      vector<WH_GM3D_TriangleFacet*> facet_s;
      if (facet_i->createDividedFacetsBySegment (seg_i,
						 facet_s)) {
	WH_CVR_LINE;
	deletedFacet_s.push_back (facet_i);
	WH_T_Add (facet_s, newFacet_s);
      } else {
	WH_CVR_LINE;
	remainingFacet_s.push_back (facet_i);
      }
    }
    WH_ASSERT(deletedFacet_s.size () + remainingFacet_s.size ()
	      == facet_s_IO.size ());
    
    if (0 < newFacet_s.size ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < deletedFacet_s.size ());
      WH_ASSERT(2 <= newFacet_s.size ());
      WH_T_Delete (deletedFacet_s);
      deletedFacet_s.clear ();
      WH_T_Move (remainingFacet_s, facet_s_IO);
      WH_T_Add (newFacet_s, facet_s_IO);
    }
  }

  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < facet_s_IO.size ());
#endif
}

void WH_GM3D_TriangleFacet
::divideFacetsByPoints 
(const vector<WH_Vector3D>& pointBy_s,
 vector<WH_GM3D_TriangleFacet*>& facet_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < facet_s_IO.size ());

  WH_CVR_LINE;

  for (vector<WH_Vector3D>::const_iterator 
	 i_point = pointBy_s.begin ();
       i_point != pointBy_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    
    vector<WH_GM3D_TriangleFacet*> remainingFacet_s;
    vector<WH_GM3D_TriangleFacet*> deletedFacet_s;
    vector<WH_GM3D_TriangleFacet*> newFacet_s;
    for (vector<WH_GM3D_TriangleFacet*>::const_iterator 
	   i_facet = facet_s_IO.begin ();
	 i_facet != facet_s_IO.end ();
	 i_facet++) {
      WH_GM3D_TriangleFacet* facet_i = (*i_facet);

      vector<WH_GM3D_TriangleFacet*> facet_s;
      if (facet_i->createDividedFacetsByPoint (point_i,
					       facet_s)) {
	WH_CVR_LINE;
	deletedFacet_s.push_back (facet_i);
	WH_T_Add (facet_s, newFacet_s);
      } else {
	WH_CVR_LINE;
	remainingFacet_s.push_back (facet_i);
      }
    }
    WH_ASSERT(deletedFacet_s.size () + remainingFacet_s.size ()
	      == facet_s_IO.size ());
    
    if (0 < newFacet_s.size ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < deletedFacet_s.size ());
      WH_ASSERT(2 <= newFacet_s.size ());
      WH_T_Delete (deletedFacet_s);
      deletedFacet_s.clear ();
      WH_T_Move (remainingFacet_s, facet_s_IO);
      WH_T_Add (newFacet_s, facet_s_IO);
    }
  }

  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < facet_s_IO.size ());
#endif
}

WH_GM3D_TriangleFacet
::WH_GM3D_TriangleFacet 
(const WH_Plane3D& plane,
 const WH_Triangle2D& parameterTriangle,
 bool frontSideIsInsideVolume,
 bool backSideIsInsideVolume,
 int faceId) 
  : _plane (plane), 
    _triangle (plane.positionAt (parameterTriangle.vertex (0)),
	       plane.positionAt (parameterTriangle.vertex (1)),
	       plane.positionAt (parameterTriangle.vertex (2))),
    _parameterTriangle (parameterTriangle)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceId);

  WH_CVR_LINE;
  
  _frontSideIsInsideVolume = frontSideIsInsideVolume;
  _backSideIsInsideVolume = backSideIsInsideVolume;

  _faceId = faceId;
  
  _minRange = _triangle.minRange ();
  _maxRange = _triangle.maxRange ();

  _parameterMinRange = _parameterTriangle.minRange ();
  _parameterMaxRange = _parameterTriangle.maxRange ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_GM3D_TriangleFacet
::~WH_GM3D_TriangleFacet () 
{
  WH_CVR_LINE;
}

bool WH_GM3D_TriangleFacet
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(0 <= this->faceId ());

  return true;
}

bool WH_GM3D_TriangleFacet
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  


  WH_Vector3D minRange;
  WH_Vector3D maxRange;
  this->getRange
    (minRange, maxRange);
  WH_ASSERT(WH_ne (minRange, maxRange));
  WH_ASSERT(WH_le (minRange, maxRange));

  WH_Vector2D paramMinRange;
  WH_Vector2D paramMaxRange;
  this->getParameterRange 
    (paramMinRange, paramMaxRange);
  WH_ASSERT(WH_lt (paramMinRange, paramMaxRange));

  WH_Triangle3D tri = this->triangle ();
  WH_ASSERT(WH_eq (tri.minRange (), minRange));
  WH_ASSERT(WH_eq (tri.maxRange (), maxRange));
  WH_ASSERT(WH_eq (tri.plane (), this->plane ()) 
	    || WH_isReverse (tri.plane (), this->plane ()));

  WH_Triangle2D paramTri = this->parameterTriangle ();
  WH_ASSERT(WH_eq (paramTri.minRange (), paramMinRange));
  WH_ASSERT(WH_eq (paramTri.maxRange (), paramMaxRange));

  return true;
}

void WH_GM3D_TriangleFacet
::reverseNormal ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->faceType () == OUTER_BOUNDARY);

  if (_frontSideIsInsideVolume && !_backSideIsInsideVolume) {
    _frontSideIsInsideVolume = false;
    _backSideIsInsideVolume = true;
  } else if (!_frontSideIsInsideVolume && _backSideIsInsideVolume) {
    _frontSideIsInsideVolume = true;
    _backSideIsInsideVolume = false;
  } else {
    WH_ASSERT_NO_REACH;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->faceType () == OUTER_BOUNDARY);
#endif
}

void WH_GM3D_TriangleFacet
::changeFromOuterToInner ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->faceType () == OUTER_BOUNDARY);

  _frontSideIsInsideVolume = true;
  _backSideIsInsideVolume = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->faceType () == INNER_BOUNDARY);
#endif
}

bool WH_GM3D_TriangleFacet
::createDividedFacetsByTriangle 
(const WH_Triangle3D& triangleBy, 
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();
  
  WH_Vector3D minRange;
  WH_Vector3D maxRange;
  this->getRange 
    (minRange, maxRange);
  if (!WH_minMaxPairsOverlap 
      (minRange, maxRange,
       triangleBy.minRange (), triangleBy.maxRange ())) {
    WH_CVR_LINE;
    
    /* POST-CONDITION */
#ifndef WH_PRE_ONLY
    if (result) {
      WH_ASSERT(2 <= facet_s_OUT.size ());
    } else {
      WH_ASSERT(facet_s_OUT.size () == 0);
    }
#endif

    return result;
  }
      
  WH_Line3D intersectionLine;
  WH_Plane3D::WithPlaneIntersectionType intersectionFlag 
    = this->plane ().checkIntersectionWith (triangleBy.plane (),
					    intersectionLine);
  if (intersectionFlag 
      == WH_Plane3D::LINE_WITH_PLANE) {
    WH_CVR_LINE;

    WH_Triangle3D tri = this->triangle ();

    WH_Vector3D point0;
    WH_Vector3D point1;
    WH_Vector3D pointBy0;
    WH_Vector3D pointBy1;
    if (tri.hasIntersectionWith 
	(intersectionLine,
	 point0, point1)
	&& triangleBy.hasIntersectionWith 
	(intersectionLine,
	 pointBy0, pointBy1)) {
      WH_CVR_LINE;

      if (WH_eq (point0, point1)) {
	WH_CVR_LINE;
	WH_Vector3D point = point0;
	if (WH_eq (pointBy0, pointBy1)) {
	  WH_CVR_LINE;
	  WH_Vector3D pointBy = pointBy0;

	  if (WH_eq (point, pointBy)) {
	    WH_CVR_LINE;
	    WH_Vector3D intersectionPoint = point;
	    result = this->createDividedFacetsByPoint 
	      (intersectionPoint,
	       facet_s_OUT);
	  }

	} else {
	  WH_CVR_LINE;
	  WH_Segment3D segBy (pointBy0, pointBy1);

	  if (segBy.contains (point)) {
	    WH_CVR_LINE;
	    WH_Vector3D intersectionPoint = point;
	    result = this->createDividedFacetsByPoint 
	      (intersectionPoint,
	       facet_s_OUT);
	  }

	}
      } else {
	WH_CVR_LINE;
	WH_Segment3D seg (point0, point1);
	if (WH_eq (pointBy0, pointBy1)) {
	  WH_CVR_LINE;
	  WH_Vector3D pointBy = pointBy0;

	  if (seg.contains (pointBy)) {
	    WH_CVR_LINE;
	    WH_Vector3D intersectionPoint = pointBy;
	    result = this->createDividedFacetsByPoint 
	      (intersectionPoint,
	       facet_s_OUT);
	  }

	} else {
	  WH_CVR_LINE;
	  WH_Segment3D segBy (pointBy0, pointBy1);
	  
	  WH_SegmentIntersector3D intersector (seg, segBy);
	  intersector.perform ();
	  if (intersector.hasIntersectionPoint ()) {
	    WH_CVR_LINE;
	    result = this->createDividedFacetsByPoint 
	      (intersector.intersectionPoint (),
	       facet_s_OUT);
	  } else if (intersector.hasIntersectionSegment ()) {
	    WH_CVR_LINE;
	    result = this->createDividedFacetsBySegment 
	      (intersector.intersectionSegment (),
	       facet_s_OUT);
	  }

	}
      }
    }

  } else if (intersectionFlag 
	     == WH_Plane3D::COINCIDENT_WITH_PLANE) {
    WH_CVR_LINE;

    vector<WH_Segment3D> segmentBy_s;
    segmentBy_s.push_back (triangleBy.edge (0));
    segmentBy_s.push_back (triangleBy.edge (1));
    segmentBy_s.push_back (triangleBy.edge (2));

    vector<WH_GM3D_TriangleFacet*> facet_s;
    facet_s.push_back (this->createCopy ());

    WH_GM3D_TriangleFacet::divideFacetsBySegments
      (segmentBy_s,
       facet_s);

    if (1 < facet_s.size ()) {
      WH_CVR_LINE;
      result = true;
      WH_T_Move (facet_s, facet_s_OUT);
    } else {
      WH_CVR_LINE;
      WH_ASSERT(facet_s.size () == 1);
      WH_T_Delete (facet_s);
      facet_s.clear ();
    }	   

  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(2 <= facet_s_OUT.size ());
  } else {
    WH_ASSERT(facet_s_OUT.size () == 0);
  }
#endif

  return result;
}

void WH_GM3D_TriangleFacet
::checkIntersectionOfDF
(WH_Triangle2D::EdgeIntersectionData edgeData[3],
 vector<int>& vertexId_s_OUT,
 vector<int>& edgeId_s_OUT,
 vector<WH_Vector2D>& edgePoint_s_OUT)
{
  WH_CVR_LINE;

  /* find one or two vertexs or edges which intersects with the
     segment */
  
  vertexId_s_OUT.clear ();
  edgeId_s_OUT.clear ();
  edgePoint_s_OUT.clear ();

  for (int iEdge = 0; iEdge < 3; iEdge++) {
    if (edgeData[iEdge].intersectionType 
	== WH_Triangle2D::EdgeIntersectionData::HAS_INTERSECTION) {
      switch (edgeData[iEdge].positionType) {
      case WH_Triangle2D::EdgeIntersectionData::AT_VERTEX0:
	vertexId_s_OUT.push_back (iEdge);
	break;
      case WH_Triangle2D::EdgeIntersectionData::AT_VERTEX1:
	break;
      case WH_Triangle2D::EdgeIntersectionData::ON_EDGE:
	edgeId_s_OUT.push_back (iEdge);
	edgePoint_s_OUT.push_back (edgeData[iEdge].intersectionPoint);
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
  }
}

void WH_GM3D_TriangleFacet
::createDFOfCase_E
(int edgeId, 
 const WH_Vector2D& edgePoint,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId);
  WH_ASSERT(edgeId < 3);
  
  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();

#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.edge (edgeId).justContains (edgePoint));
#endif
  
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((0 + edgeId) % 3), 
      edgePoint,
      paramTri.vertex ((2 + edgeId) % 3)));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (edgePoint,
      paramTri.vertex ((1 + edgeId) % 3), 
      paramTri.vertex ((2 + edgeId) % 3)));

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 2);
#endif
}

void WH_GM3D_TriangleFacet
::createDFOfCase_I
(const WH_Vector2D& insidePoint,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.checkContainmentAt (insidePoint)
	    == WH_Triangle2D::IN);
#endif

  for (int iEdge = 0; iEdge < 3; iEdge++) {
    facet_s_OUT.push_back 
      (this->createCopyAmong 
       (paramTri.vertex ((0 + iEdge) % 3), 
	paramTri.vertex ((1 + iEdge) % 3),
	insidePoint));
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 3);
#endif
}

void WH_GM3D_TriangleFacet
::createDFOfCase_EE_Coinsident
(int edgeId, 
 const WH_Vector2D& edgePoint0,
 const WH_Vector2D& edgePoint1,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId);
  WH_ASSERT(edgeId < 3);
  
  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.edge (edgeId).justContains (edgePoint0));
  WH_ASSERT(paramTri.edge (edgeId).justContains (edgePoint1));
#endif

  double dist0 = WH_distance 
    (paramTri.vertex (edgeId), edgePoint0);
  double dist1 = WH_distance 
    (paramTri.vertex (edgeId), edgePoint1);
  WH_ASSERT(WH_ne (dist1, dist0));

  WH_Vector2D point0 = edgePoint0;
  WH_Vector2D point1 = edgePoint1;
  if (WH_lt (dist1, dist0)) {
    WH_swap (point0, point1);
  }
  
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((0 + edgeId) % 3), 
      point0,
      paramTri.vertex ((2 + edgeId) % 3)));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (point0,
      point1,
      paramTri.vertex ((2 + edgeId) % 3)));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (point1,
      paramTri.vertex ((1 + edgeId) % 3), 
      paramTri.vertex ((2 + edgeId) % 3)));

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 3);
#endif
}

void WH_GM3D_TriangleFacet
::createDFOfCase_EE_NoCoinsident
(int edgeId0, 
 const WH_Vector2D& edgePoint0,
 int edgeId1, 
 const WH_Vector2D& edgePoint1,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId0);
  WH_ASSERT(edgeId0 < 3);
  WH_ASSERT(0 <= edgeId1);
  WH_ASSERT(edgeId1 < 3);
  WH_ASSERT(edgeId0 < edgeId1);
  
  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.edge (edgeId0).justContains (edgePoint0));
  WH_ASSERT(paramTri.edge (edgeId1).justContains (edgePoint1));
#endif

  WH_Vector2D point0 = edgePoint0;
  WH_Vector2D point1 = edgePoint1;
  int edgeId = edgeId0;
  if (edgeId0 == 0 && edgeId1 == 2) {
    WH_swap (point0, point1);
    edgeId = edgeId1;
  }

  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((0 + edgeId) % 3),
      point0, 
      paramTri.vertex ((2 + edgeId) % 3)));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (point0, 
      paramTri.vertex ((1 + edgeId) % 3),
      point1));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (point1, 
      paramTri.vertex ((2 + edgeId) % 3),
      point0));

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 3);
#endif
}

void WH_GM3D_TriangleFacet
::createDFOfCase_EI
(int edgeId, 
 const WH_Vector2D& edgePoint,
 const WH_Vector2D& insidePoint,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId);
  WH_ASSERT(edgeId < 3);
  
  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.edge (edgeId).justContains (edgePoint));
  WH_ASSERT(paramTri.checkContainmentAt (insidePoint)
	    == WH_Triangle2D::IN);
#endif

  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((0 + edgeId) % 3), 
      edgePoint,
      insidePoint));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (edgePoint,
      paramTri.vertex ((1 + edgeId) % 3), 
      insidePoint));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((1 + edgeId) % 3), 
      paramTri.vertex ((2 + edgeId) % 3), 
      insidePoint));
  facet_s_OUT.push_back 
    (this->createCopyAmong 
     (paramTri.vertex ((2 + edgeId) % 3), 
      paramTri.vertex ((0 + edgeId) % 3), 
      insidePoint));

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 4);
#endif
}

void WH_GM3D_TriangleFacet
::createDFOfCase_II
(const WH_Vector2D& insidePoint0,
 const WH_Vector2D& insidePoint1,
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (insidePoint0, insidePoint1));

  WH_CVR_LINE;
  
  facet_s_OUT.clear ();

  WH_Triangle2D paramTri = this->parameterTriangle ();
  
#ifndef WH_PRE_ONLY
  WH_ASSERT(paramTri.checkContainmentAt (insidePoint0)
	    == WH_Triangle2D::IN);
  WH_ASSERT(paramTri.checkContainmentAt (insidePoint1)
	    == WH_Triangle2D::IN);
#endif

  bool segmentContainsInsidePoint1 = false;
  for (int iEdge = 0; iEdge < 3; iEdge++) {
    WH_Segment2D seg (paramTri.vertex (iEdge), 
		      insidePoint0);
    if (seg.justContains (insidePoint1)) {
      facet_s_OUT.push_back 
	(this->createCopyAmong 
	 (insidePoint1, 
	  paramTri.vertex ((0 + iEdge) % 3), 
	  paramTri.vertex ((1 + iEdge) % 3)));
      facet_s_OUT.push_back 
	(this->createCopyAmong 
	 (insidePoint0, 
	  insidePoint1, 
	  paramTri.vertex ((1 + iEdge) % 3)));
      facet_s_OUT.push_back 
	(this->createCopyAmong 
	 (paramTri.vertex ((1 + iEdge) % 3), 
	  paramTri.vertex ((2 + iEdge) % 3), 
	  insidePoint0));
      facet_s_OUT.push_back 
	(this->createCopyAmong 
	 (paramTri.vertex ((0 + iEdge) % 3), 
	  insidePoint1, 
	  paramTri.vertex ((2 + iEdge) % 3)));
      facet_s_OUT.push_back 
	(this->createCopyAmong 
	 (insidePoint1, 
	  insidePoint0, 
	  paramTri.vertex ((2 + iEdge) % 3)));
      segmentContainsInsidePoint1 = true;
      break;
    }
  }

  if (!segmentContainsInsidePoint1) {
    bool triangleContainsInsidePoint1 = false;
    for (int iEdge = 0; iEdge < 3; iEdge++) {
      WH_Triangle2D tri (paramTri.vertex ((0 + iEdge) % 3), 
			 paramTri.vertex ((1 + iEdge) % 3), 
			 insidePoint0);
      if (tri.checkContainmentAt (insidePoint1) 
	  == WH_Triangle2D::IN) {
	facet_s_OUT.push_back 
	  (this->createCopyAmong 
	   (insidePoint0, 
	    paramTri.vertex ((2 + iEdge) % 3), 
	    paramTri.vertex ((0 + iEdge) % 3)));
	facet_s_OUT.push_back 
	  (this->createCopyAmong 
	   (insidePoint1, 
	    insidePoint0, 
	    paramTri.vertex ((0 + iEdge) % 3)));
	facet_s_OUT.push_back 
	  (this->createCopyAmong 
	   (paramTri.vertex ((0 + iEdge) % 3), 
	    paramTri.vertex ((1 + iEdge) % 3), 
	    insidePoint1));
	facet_s_OUT.push_back 
	  (this->createCopyAmong 
	   (paramTri.vertex ((2 + iEdge) % 3), 
	    insidePoint0, 
	    paramTri.vertex ((1 + iEdge) % 3)));
	facet_s_OUT.push_back 
	  (this->createCopyAmong 
	   (insidePoint0, 
	    insidePoint1, 
	    paramTri.vertex ((1 + iEdge) % 3)));
	triangleContainsInsidePoint1 = true;
	break;
      }
    }
    WH_ASSERT(triangleContainsInsidePoint1);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(facet_s_OUT.size () == 5);
#endif
}

bool WH_GM3D_TriangleFacet
::createDividedFacetsBySegment 
(const WH_Segment3D& segmentBy, 
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Vector3D minRange;
  WH_Vector3D maxRange;
  this->getRange 
    (minRange, maxRange);
  if (!WH_minMaxPairsOverlap 
      (minRange, maxRange,
       segmentBy.minRange (), segmentBy.maxRange ())) {
    WH_CVR_LINE;

    /* POST-CONDITION */
#ifndef WH_PRE_ONLY
    if (result) {
      WH_ASSERT(2 <= facet_s_OUT.size ());
    } else {
      WH_ASSERT(facet_s_OUT.size () == 0);
    }
#endif
    
    return result;
  }

  WH_Plane3D plane = this->plane ();

  WH_Vector3D intersectionPoint;
  WH_Segment3D::WithPlaneIntersectionType planeFlag 
    = segmentBy.checkIntersectionWith (plane,
				       intersectionPoint);
  if (planeFlag == WH_Segment3D::POINT_WITH_PLANE) {
    WH_CVR_LINE;
    /* <segmentBy> may intersect with this at <intersectionPoint>. */
    
    return this->createDividedFacetsByPoint (intersectionPoint, 
					     facet_s_OUT);

  } else if (planeFlag == WH_Segment3D::COINCIDENT_WITH_PLANE) {
    WH_CVR_LINE;
    /* <segmentBy> may overlap with this. */
    WH_Segment2D paramSegBy 
      (plane.parameterAt (segmentBy.p0 ()),
       plane.parameterAt (segmentBy.p1 ()));

    WH_Triangle2D paramTri = this->parameterTriangle ();
    
    WH_Triangle2D::EdgeIntersectionData edgeData[3];
    bool intersectionFlag 
      = paramTri.intersectsWith (paramSegBy,
				 edgeData);
    if (intersectionFlag) {
      WH_CVR_LINE;
      /* V-I, E-I, E-E, E-V, E-O */
      
      /* find one edge which coinsides with the segment */
      int edgeIdCoinsident = WH_NO_INDEX;
      for (int iEdge = 0; iEdge < 3; iEdge++) {
	if (edgeData[iEdge].intersectionType 
	    == WH_Triangle2D::EdgeIntersectionData::IS_COINCIDENT) {
	  WH_CVR_LINE;
	  edgeIdCoinsident = iEdge;
	  break;
	}
      }
      
      if (edgeIdCoinsident != WH_NO_INDEX) {
	WH_CVR_LINE;
	/* E-E or E-V (coinside cases) */

	WH_Segment2D edgeSeg = paramTri.edge (edgeIdCoinsident);
  
	bool EEFlag = false;
	WH_Vector2D EEEdgePoint0;
	WH_Vector2D EEEdgePoint1;
	
	bool EVFlag = false;
	WH_Vector2D EVEdgePoint;
	
	WH_Segment2D::WithSegmentOverlapType overlapFlag 
	  = edgeSeg.checkOverlapWith (paramSegBy);
	if (overlapFlag == WH_Segment2D::OVERLAP_WITH_SEGMENT
	    || overlapFlag == WH_Segment2D::CONTAINS_WITH_SEGMENT) {
	  WH_CVR_LINE;
	  if (edgeSeg.justContains (paramSegBy.p0 ())
	      && edgeSeg.justContains (paramSegBy.p1 ())) {
	    WH_CVR_LINE;
	    EEFlag = true;
	    EEEdgePoint0 = paramSegBy.p0 ();
	    EEEdgePoint1 = paramSegBy.p1 ();
	  } else if (edgeSeg.justContains (paramSegBy.p0 ())) {
	    WH_CVR_LINE;
	    EVFlag = true;
	    EVEdgePoint = paramSegBy.p0 ();
	  } else if (edgeSeg.justContains (paramSegBy.p1 ())) {
	    WH_CVR_LINE;
	    EVFlag = true;
	    EVEdgePoint = paramSegBy.p1 ();
	  }
	}
	if (EVFlag) {
	  WH_CVR_LINE;
	  /* E-V (coinside) */

	  this->createDFOfCase_E
	    (edgeIdCoinsident, EVEdgePoint,
	     facet_s_OUT);
	  result = true;

	} else if (EEFlag) {
	  WH_CVR_LINE;
	  /* E-E (coinside) */

	  this->createDFOfCase_EE_Coinsident
	    (edgeIdCoinsident, EEEdgePoint0, EEEdgePoint1,
	     facet_s_OUT);
	  result = true;

	}
	
	/* END E-E or E-V (coinside cases) */
      } else {
	WH_CVR_LINE;
	/* I-I, V-I, E-I, E-E, E-V, E-O (no coinside) */

	/* find one or two vertexs or edges which intersects with the
	   segment */
	vector<int> vertexId_s;
	vector<int> edgeId_s;
	vector<WH_Vector2D> edgePoint_s;
	this->checkIntersectionOfDF 
	  (edgeData,
	   vertexId_s, edgeId_s, edgePoint_s);

	WH_Triangle2D::ContainmentType p0Flag 
	  = paramTri.checkContainmentAt (paramSegBy.p0 ());
	WH_Triangle2D::ContainmentType p1Flag 
	  = paramTri.checkContainmentAt (paramSegBy.p1 ());

	if (p0Flag == WH_Triangle2D::IN
	    && p1Flag == WH_Triangle2D::IN) {
	  WH_CVR_LINE;
	  /* I-I */
	  WH_ASSERT(vertexId_s.size () == 0);
	  WH_ASSERT(edgeId_s.size () == 0);
	  
	  WH_Vector2D insidePoint0 = paramSegBy.p0 ();
	  WH_Vector2D insidePoint1 = paramSegBy.p1 ();
	  this->createDFOfCase_II
	    (insidePoint0, insidePoint1, 
	     facet_s_OUT);
	  result = true;
	  
	} else if (p0Flag == WH_Triangle2D::IN
		   || p1Flag == WH_Triangle2D::IN) {
	  WH_CVR_LINE;
	  /* V-I or E-I */
	  WH_ASSERT(vertexId_s.size () + edgeId_s.size () == 1);

	  WH_Vector2D insidePoint;
	  if (p0Flag == WH_Triangle2D::IN) {
	    WH_CVR_LINE;
	    insidePoint = paramSegBy.p0 ();
	  } else {
	    WH_CVR_LINE;
	    insidePoint = paramSegBy.p1 ();
	  }

	  if (vertexId_s.size () == 1) {
	    WH_CVR_LINE;
	    /* V-I */
	    WH_ASSERT(edgeId_s.size () == 0);

	    this->createDFOfCase_I
	      (insidePoint, 
	       facet_s_OUT);
	    result = true;
	    
	  } else {
	    WH_CVR_LINE;
	    /* E-I */
	    WH_ASSERT(vertexId_s.size () == 0);
	    WH_ASSERT(edgeId_s.size () == 1);

	    this->createDFOfCase_EI
	      (edgeId_s[0], edgePoint_s[0], insidePoint, 
	       facet_s_OUT);
	    result = true;

	  }

	  /* END V-I or E-I */
	} else if (edgeId_s.size () == 2) {
	  WH_CVR_LINE;
	  /* E-E (no coinside) */
	  WH_ASSERT(vertexId_s.size () == 0);
	  
	  this->createDFOfCase_EE_NoCoinsident
	    (edgeId_s[0], edgePoint_s[0], edgeId_s[1], edgePoint_s[1], 
	     facet_s_OUT);
	  result = true;
	  
	} else if (vertexId_s.size () == 1 && edgeId_s.size () == 1) {
	  WH_CVR_LINE;
	  /* E-V (no coinside) */
	  
	  this->createDFOfCase_E
	    (edgeId_s[0], edgePoint_s[0], 
	     facet_s_OUT);
	  result = true;

	} else if (vertexId_s.size () == 0 && edgeId_s.size () == 1) {
	  WH_CVR_LINE;
	  /* E-O */
	  
	  this->createDFOfCase_E
	    (edgeId_s[0], edgePoint_s[0], 
	     facet_s_OUT);
	  result = true;

	} else {
	  WH_CVR_LINE;
	  /* other cases : O-O, V-O, V-V */

	  /* nothing */
	}

	/* END I-I, V-I, E-I, E-E, E-V, E-O (no coinside) */
      }

      /* END I-I, V-I, E-I, E-E, E-V, E-O */
    } else {
      WH_CVR_LINE;
      WH_ASSERT(!intersectionFlag);
      /* no intersection */
      /* O-O */
      
      /* nothing */
    }

  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(2 <= facet_s_OUT.size ());
  } else {
    WH_ASSERT(facet_s_OUT.size () == 0);
  }
#endif

  return result;
}

bool WH_GM3D_TriangleFacet
::createDividedFacetsByPoint 
(const WH_Vector3D& pointBy, 
 vector<WH_GM3D_TriangleFacet*>& facet_s_OUT)
{
  WH_CVR_LINE;

  bool result = false;

  facet_s_OUT.clear ();

  WH_Vector3D minRange;
  WH_Vector3D maxRange;
  this->getRange 
    (minRange, maxRange);
  if (!WH_between (pointBy, minRange, maxRange)) {
    WH_CVR_LINE;

    /* POST-CONDITION */
#ifndef WH_PRE_ONLY
    if (result) {
      WH_ASSERT(2 <= facet_s_OUT.size ());
    } else {
      WH_ASSERT(facet_s_OUT.size () == 0);
    }
#endif
    
    return result;
  }

  WH_Plane3D plane = this->plane ();
  if (plane.contains (pointBy)) {
    WH_CVR_LINE;
    WH_Vector2D paramBy = plane.parameterAt (pointBy);
    WH_Triangle2D paramTri = this->parameterTriangle ();
    if (!paramTri.hasVertexAt (paramBy)) {
      WH_CVR_LINE;
      WH_Triangle2D::ContainmentType flag 
	= paramTri.checkContainmentAt (paramBy);
      if (flag == WH_Triangle2D::IN) {
	WH_CVR_LINE;
	this->createDFOfCase_I (paramBy,
				facet_s_OUT);
	result = true;
      } else if (flag == WH_Triangle2D::ON) {
	WH_CVR_LINE;
	for (int iEdge = 0; iEdge < 3; iEdge++) {
	  if (paramTri.edge (iEdge).justContains (paramBy)) {
	    WH_CVR_LINE;
	    this->createDFOfCase_E (iEdge, paramBy,
				    facet_s_OUT);
	    result = true;
	    break;
	  }
	}
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(2 <= facet_s_OUT.size ());
  } else {
    WH_ASSERT(facet_s_OUT.size () == 0);
  }
#endif

  return result;
}

WH_GM3D_TriangleFacet* WH_GM3D_TriangleFacet
::createCopy ()
{
  WH_GM3D_TriangleFacet* result 
    = new WH_GM3D_TriangleFacet 
    (_plane, 
     _parameterTriangle, 
     _frontSideIsInsideVolume, _backSideIsInsideVolume,
     _faceId);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_GM3D_TriangleFacet* WH_GM3D_TriangleFacet
::createCopyAmong 
(const WH_Vector2D& parameter0,
 const WH_Vector2D& parameter1,
 const WH_Vector2D& parameter2)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (parameter0, parameter1));
  WH_ASSERT(WH_ne (parameter0, parameter2));
  WH_ASSERT(WH_ne (parameter1, parameter2));
  
  WH_GM3D_TriangleFacet* result 
    = new WH_GM3D_TriangleFacet 
    (_plane, 
     WH_Triangle2D (parameter0, parameter1, parameter2), 
     _frontSideIsInsideVolume, _backSideIsInsideVolume,
     _faceId);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_Plane3D WH_GM3D_TriangleFacet
::plane () const
{
  return _plane;
}
     
WH_Triangle3D WH_GM3D_TriangleFacet
::triangle () const
{
  return _triangle;
}

WH_Triangle2D WH_GM3D_TriangleFacet
::parameterTriangle () const
{
  return _parameterTriangle;
}

bool WH_GM3D_TriangleFacet
::frontSideIsInsideVolume () const
{
  return _frontSideIsInsideVolume;
}

bool WH_GM3D_TriangleFacet
::backSideIsInsideVolume () const
{
  return _backSideIsInsideVolume;
}

int WH_GM3D_TriangleFacet
::faceId () const
{
  return _faceId;
}

WH_GM3D_TriangleFacet::FaceType WH_GM3D_TriangleFacet
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

bool WH_GM3D_TriangleFacet
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
      return false;
    }
  }
}
  
void WH_GM3D_TriangleFacet
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

void WH_GM3D_TriangleFacet
::getParameterRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
{
  minRange_OUT = _parameterMinRange;
  maxRange_OUT = _parameterMaxRange;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
  WH_ASSERT(WH_ne (minRange_OUT, maxRange_OUT));
#endif
}



/* test coverage completed */







