/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* polygon2d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "polygon2d.h"



/* class WH_Polygon2D_A */

WH_Polygon2D_A
::WH_Polygon2D_A ()
{
  WH_CVR_LINE;
}

WH_Polygon2D_A
::~WH_Polygon2D_A ()
{
  WH_CVR_LINE;
}

bool WH_Polygon2D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_Polygon2D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(2 < this->nVertexs ());

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex0 = this->vertex (iVertex);
    WH_Vector2D vertex1 
      = this->vertex (this->nextVertexIndex (iVertex));
    WH_ASSERT(WH_ne (vertex0, vertex1));
  }

  WH_ASSERT(this->nVertexs () == this->nEdges ());

  WH_ASSERT(this->prevVertexIndex 
	    (this->nextVertexIndex (0)) 
	    == 0);
  WH_ASSERT(this->prevVertexIndex 
	    (this->nextVertexIndex (this->nVertexs () - 1)) 
	    == this->nVertexs () - 1);
  WH_ASSERT(this->nextVertexIndex 
	    (this->prevVertexIndex (0)) 
	    == 0);
  WH_ASSERT(this->nextVertexIndex 
	    (this->prevVertexIndex (this->nVertexs () - 1)) 
	    == this->nVertexs () - 1);
  
  WH_ASSERT(this->prevEdgeIndex 
	    (this->nextEdgeIndex (0)) 
	    == 0);
  WH_ASSERT(this->prevEdgeIndex 
	    (this->nextEdgeIndex (this->nEdges () - 1)) 
	    == this->nEdges () - 1);
  WH_ASSERT(this->nextEdgeIndex 
	    (this->prevEdgeIndex (0)) 
	    == 0);
  WH_ASSERT(this->nextEdgeIndex 
	    (this->prevEdgeIndex (this->nEdges () - 1)) 
	    == this->nEdges () - 1);
  
  {
    int iVertex0;
    int iVertex1;
    this->getVertexIndexsOfEdge (0, 
				 iVertex0, iVertex1);
    WH_ASSERT(iVertex0 == 0);
    WH_ASSERT(iVertex1 == 1);
    this->getVertexIndexsOfEdge (this->nEdges () - 1, 
				 iVertex0, iVertex1);
    WH_ASSERT(iVertex0 == this->nEdges () - 1);
    WH_ASSERT(iVertex1 == 0);
  }
  
  WH_Vector2D minRange = this->minRange ();
  WH_Vector2D maxRange = this->maxRange ();
  WH_ASSERT(WH_le (minRange, maxRange));

  if (this->isRegular ()) {
    WH_CVR_LINE;
    WH_ASSERT(WH_lt (minRange, maxRange));

    WH_Vector2D centerOfGravity = this->centerOfGravity ();
    WH_ASSERT(WH_between (centerOfGravity, minRange, maxRange));

    WH_ASSERT(WH_lt (0, this->perimeter ()));
    WH_ASSERT(WH_lt2 (0, this->area ()));

    for (int iVertex = 0; 
	 iVertex < this->nVertexs (); 
	 iVertex++) {
      WH_Vector2D vertex = this->vertex (iVertex);
      ContainmentType flag = this
	->checkContainmentAt (vertex);
      WH_ASSERT(flag == ON);
    }

    for (int iEdge = 0; 
	 iEdge < this->nEdges (); 
	 iEdge++) {
      WH_Segment2D edge = this->edge (iEdge);
      ContainmentType flag = this
	->checkContainmentAt (edge.midPoint ());
      WH_ASSERT(flag == ON);
    }

    if (this->isConvex ()) {
      WH_CVR_LINE;
      ContainmentType flag = this
	->checkContainmentAt (centerOfGravity);
      WH_ASSERT(flag == IN);
    }
  }

  return true;
}

int WH_Polygon2D_A
::nEdges () const
{
  return this->nVertexs ();
}

WH_Segment2D WH_Polygon2D_A
::edge (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

  WH_CVR_LINE;

  int iVertex0;
  int iVertex1;
  this->getVertexIndexsOfEdge (iEdge, 
			       iVertex0, iVertex1);
  WH_Vector2D v0 = this->vertex (iVertex0);
  WH_Vector2D v1 = this->vertex (iVertex1);
  WH_ASSERT(WH_ne (v0, v1));
  return WH_Segment2D (v0, v1);
}

int WH_Polygon2D_A
::nextVertexIndex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  WH_CVR_LINE;

  int result = iVertex + 1;
  if (result == this->nVertexs ()) {
    WH_CVR_LINE;
    result = 0;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexs ());
#endif

  return result;
}

int WH_Polygon2D_A
::prevVertexIndex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  WH_CVR_LINE;

  int result = iVertex - 1;
  if (result == -1) {
    WH_CVR_LINE;
    result = this->nVertexs () - 1;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexs ());
#endif

  return result;
}

int WH_Polygon2D_A
::nextEdgeIndex (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());
  
  WH_CVR_LINE;

  int result = iEdge + 1;
  if (result == this->nEdges ()) {
    WH_CVR_LINE;
    result = 0;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdges ());
#endif

  return result;
}

int WH_Polygon2D_A
::prevEdgeIndex (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());
  
  WH_CVR_LINE;

  int result = iEdge - 1;
  if (result == -1) {
    WH_CVR_LINE;
    result = this->nEdges () - 1;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdges ());
#endif

  return result;
}

void WH_Polygon2D_A
::getVertexIndexsOfEdge 
(int iEdge,
 int& iVertex0_OUT, int& iVertex1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

  WH_CVR_LINE;

  iVertex0_OUT = iEdge;
  iVertex1_OUT = this->nextVertexIndex (iVertex0_OUT);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= iVertex0_OUT);
  WH_ASSERT(iVertex0_OUT < this->nVertexs ());
  WH_ASSERT(0 <= iVertex1_OUT);
  WH_ASSERT(iVertex1_OUT < this->nVertexs ());
#endif
}

bool WH_Polygon2D_A
::hasAnyDuplicatedVertex () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex_i = this->vertex (iVertex);
    
    for (int jVertex = iVertex + 1; 
	 jVertex < this->nVertexs (); 
	 jVertex++) {
      WH_Vector2D vertex_j = this->vertex (jVertex);

      if (WH_eq (vertex_i, vertex_j) ) {
	WH_CVR_LINE;
	result = true;
	break;
      }
    }
  }

  return result;
}

bool WH_Polygon2D_A
::hasAnyCrossingEdge () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge_i = this->edge (iEdge);
    int iEdgeNext = this->nextEdgeIndex (iEdge);
    int iEdgePrev = this->prevEdgeIndex (iEdge);
    
    for (int jEdge = iEdge + 1; 
	 jEdge < this->nEdges (); 
	 jEdge++) {
      WH_Segment2D edge_j = this->edge (jEdge);

      WH_Vector2D point;
      WH_Segment2D::WithSegmentIntersectionType intersectionFlag 
	= edge_i.checkIntersectionWith (edge_j,
					point);
      if (intersectionFlag == WH_Segment2D::POINT_WITH_SEGMENT) {
	WH_CVR_LINE;
	if (jEdge == iEdgeNext
	    || jEdge == iEdgePrev) {
	  WH_CVR_LINE;
	  WH_ASSERT(edge_i.hasConnectionWith (edge_j));
	} else {
	  WH_CVR_LINE;
	  result = true;
	  break;
	}
      } else if (intersectionFlag == WH_Segment2D::COINCIDENT_WITH_SEGMENT) {
	WH_CVR_LINE;
	WH_Segment2D::WithSegmentOverlapType overlapFlag 
	  = edge_i.checkOverlapWith (edge_j);
	if (overlapFlag == WH_Segment2D::OVERLAP_WITH_SEGMENT) {
	  WH_CVR_LINE;
	  if (jEdge == iEdgeNext
	      || jEdge == iEdgePrev) {
	    WH_CVR_LINE;
	    WH_ASSERT(edge_i.hasConnectionWith (edge_j));
	  } else {
	    WH_CVR_LINE;
	    result = true;
	    break;
	  }
	} else if (overlapFlag != WH_Segment2D::NO_OVERLAP_WITH_SEGMENT) {
	  WH_CVR_LINE;
	  result = true;
	  break;
	}
      }
    }
  }

  return result;
}

bool WH_Polygon2D_A
::hasAnySpecialAngle () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);
    WH_Segment2D edgeNext = this->edge (this->nextEdgeIndex (iEdge));

    /* check angle of 0 degree */
    /* angle of 180 degree (flat angle) is considered as regular  */
    if (WH_eq (WH_angleOfLines (edge.line (), edgeNext.line ()), M_PI)) {
      WH_CVR_LINE;
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polygon2D_A
::isRegular () const
{
  return !this->hasAnyDuplicatedVertex ()
    && !this->hasAnyCrossingEdge ()
      && !this->hasAnySpecialAngle ();
}

bool WH_Polygon2D_A
::isConvex () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  bool result = false;

  bool signIsChanged = false;
  bool zeroSignIsFound = false;

  bool thisIsTheFirstIteration = true;
  double prevSign = 0.0;
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);

    /* obtain sign of angle between the edge and its next edge */
    int iVertex0;
    int iVertex1;
    this->getVertexIndexsOfEdge (iEdge, 
				 iVertex0, iVertex1);
    WH_Vector2D nextVertex 
      = this->vertex (this->nextVertexIndex (iVertex1));
    double sign = edge.signedTriangleAreaWith (nextVertex);
    /* only sign of <sign>, +/0/-, matters */
    
    if (WH_eq2 (sign, 0)) {
      WH_CVR_LINE;
      /* angle is zero */
      zeroSignIsFound = true;
      break;
    }

    WH_ASSERT(WH_ne2 (sign, 0));
    if (thisIsTheFirstIteration) {
      WH_CVR_LINE;
      thisIsTheFirstIteration = false;
      prevSign = sign;
    } else {
      WH_CVR_LINE;
      WH_ASSERT(WH_ne2 (prevSign, 0));
      if ((WH_lt2 (sign, 0) && WH_lt2 (0, prevSign))
	  || (WH_lt2 (0, sign) && WH_lt2 (prevSign, 0))) {
	WH_CVR_LINE;
	/* <sign> is different from <prevSign> */
	signIsChanged = true;
	break;
      }
    }
  }

  if (zeroSignIsFound || signIsChanged) {
    WH_CVR_LINE;
    result = false;
  } else {
    WH_CVR_LINE;
    result = true;
    WH_ASSERT(WH_ne2 (prevSign, 0));
    /* clockWiseFlag_OUT = WH_lt2 (prevSign, 0); */
  }

  return result;
}

bool WH_Polygon2D_A
::isClockWise () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  bool result = false;

  double angleSum = 0.0;
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);
    double slope = edge.line ().direction ().slope ();
    WH_Segment2D prevEdge;
    if (0 < iEdge) {
      WH_CVR_LINE;
      prevEdge = this->edge (iEdge - 1);
    } else {
      WH_CVR_LINE;
      prevEdge = this->edge (this->nEdges () - 1);
    }
    double prevSlope = prevEdge.line ().direction ().slope ();
    
    double angle = slope - prevSlope;
    WH_ASSERT(WH_ne (angle, M_PI));
    WH_ASSERT(WH_ne (angle, -M_PI));
    if (WH_lt (angle, -M_PI)) {
      WH_CVR_LINE;
      angle += M_PI * 2;
    } else if (WH_lt (M_PI, angle)) {
      WH_CVR_LINE;
      angle -= M_PI * 2;
    }
    WH_ASSERT(WH_lt (-M_PI, angle));
    WH_ASSERT(WH_lt (angle, M_PI));
    angleSum += angle;
  }
  WH_ASSERT(WH_eq (angleSum, M_PI * 2) 
	    || WH_eq (angleSum, -M_PI * 2));
  if (WH_eq (angleSum, -M_PI * 2)) {
    WH_CVR_LINE;
    result = true;
  }
  
  return result;
}

double WH_Polygon2D_A
::perimeter () const
{
  WH_CVR_LINE;

  double result = 0.0;
  
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);
    result += edge.length ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt (0, result));
#endif

  return result;
}

double WH_Polygon2D_A
::area () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  double result = 0.0;

  WH_Vector2D origin = this->vertex (0);
  for (int iVertex = 1; 
       iVertex < this->nVertexs () - 1; 
       iVertex++) {
    WH_Vector2D vertex0 = this->vertex (iVertex);
    WH_Vector2D vertex1 
      = this->vertex (this->nextVertexIndex (iVertex));
    double signedArea = WH_Segment2D (vertex0, vertex1)
      .signedTriangleAreaWith (origin);
    result += signedArea;
  }
  result = fabs (result);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt2 (0, result));
#endif
  
  return result;
}

WH_Vector2D WH_Polygon2D_A
::centerOfGravity () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  WH_Vector2D result (0, 0);
  double areaSum = 0.0;
  WH_Vector2D origin = this->vertex (0);
  for (int iVertex = 1; 
       iVertex < this->nVertexs () - 1; 
       iVertex++) {
    WH_Vector2D vertex0 = this->vertex (iVertex);
    WH_Vector2D vertex1 
      = this->vertex (this->nextVertexIndex (iVertex));
    double signedArea = WH_Segment2D (vertex0, vertex1)
      .signedTriangleAreaWith (origin);
    areaSum += signedArea;
    result += (vertex0 + vertex1) * signedArea;
  }
  WH_ASSERT(WH_ne2 (0, areaSum));
  result = (result / areaSum + origin) / 3;

  return result;
}

WH_Vector2D WH_Polygon2D_A
::minRange () const
{
  WH_CVR_LINE;

  WH_Vector2D result = WH_Vector2D::hugeValue ();
  
  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex = this->vertex (iVertex);
    result = WH_min (result, vertex);
  }

  return result;
}

WH_Vector2D WH_Polygon2D_A
::maxRange () const
{
  WH_CVR_LINE;

  WH_Vector2D result = -WH_Vector2D::hugeValue ();
  
  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex = this->vertex (iVertex);
    result = WH_max (result, vertex);
  }

  return result;
}

bool WH_Polygon2D_A
::hasVertexAt (const WH_Vector2D& position) const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex = this->vertex (iVertex);
    if (WH_eq (vertex, position)) {
      WH_CVR_LINE;
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polygon2D_A
::hasVertexAtEveryPointIn
(const vector<WH_Vector2D>& point_s) const
{
  WH_CVR_LINE;

  bool result = true;

  for (vector<WH_Vector2D>::const_iterator
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    if (!this->hasVertexAt (point_i)) {
      WH_CVR_LINE;
      result = false;
      break;
    }
  }

  return result;
}

bool WH_Polygon2D_A
::containsOnBoundary (const WH_Vector2D& position) const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);
    if (edge.contains (position)) {
      result = true;
      break;
    }
  }  

  return result;
}

bool WH_Polygon2D_A
::containsInside (const WH_Vector2D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  bool result = false;

  /* calculate sum of angles from <position> to all edges */

  double sumOfAngle = 0.0;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);

    WH_ASSERT(!edge.contains (position));
    /* <position> is not on the edge */
    
    double angle = WH_angleOfVectors (edge.p0 () - position, 
				      edge.p1 () - position);
    WH_ASSERT(WH_lt (-M_PI, angle));
    WH_ASSERT(WH_le (angle, M_PI));
    
    WH_ASSERT(WH_ne (angle, M_PI));
    /* <position> is not on the edge */
    
    sumOfAngle += angle;
  }

  if (WH_eq (sumOfAngle, 2 * M_PI)
      || WH_eq (sumOfAngle, -2 * M_PI)) {
    WH_CVR_LINE;
    /* inside */
    result = true;
  } else {
    WH_CVR_LINE;
    WH_ASSERT(WH_eq (sumOfAngle, 0));
    /* outside */
    result = false;
  }

  return result;
}

WH_Polygon2D_A::ContainmentType WH_Polygon2D_A
::checkContainmentAt (const WH_Vector2D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  ContainmentType result = OUT;

  WH_Vector2D minRange = this->minRange ();
  WH_Vector2D maxRange = this->maxRange ();
  if (WH_between (position, minRange, maxRange)) {
    WH_CVR_LINE;
    if (this->containsOnBoundary (position)) {
      WH_CVR_LINE;
      result = ON;
    } else if (this->containsInside (position)) {
      WH_CVR_LINE;
      result = IN;
    }
  }
  
  return result;
}

bool WH_Polygon2D_A
::intersectsWith 
(const WH_Line2D& line,
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edgeData_OUT != WH_NULL);
  
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);

    int iVertex0;
    int iVertex1;
    this->getVertexIndexsOfEdge 
      (iEdge,
       iVertex0, iVertex1);
    
    WH_Vector2D point;
    WH_Segment2D::WithLineIntersectionType intersectioFlag 
      = edge.checkIntersectionWith (line, 
				    point);
    switch (intersectioFlag) {
    case WH_Segment2D::POINT_WITH_LINE:
      WH_CVR_LINE;
      edgeData_OUT[iEdge].intersectionType 
	= EdgeIntersectionData::HAS_INTERSECTION;
      edgeData_OUT[iEdge].intersectionPoint = point;
      if (WH_eq (point, edge.p0 ())) {
	WH_CVR_LINE;
	edgeData_OUT[iEdge].positionType 
	  = EdgeIntersectionData::AT_VERTEX0;
      } else if (WH_eq (point, edge.p1 ())) {
	WH_CVR_LINE;
	edgeData_OUT[iEdge].positionType 
	  = EdgeIntersectionData::AT_VERTEX1;
      } else {
	WH_CVR_LINE;
	edgeData_OUT[iEdge].positionType 
	  = EdgeIntersectionData::ON_EDGE;
      }
      result = true;
      break;

    case WH_Segment2D::COINCIDENT_WITH_LINE:
      WH_CVR_LINE;
      edgeData_OUT[iEdge].intersectionType 
	= EdgeIntersectionData::IS_COINCIDENT;
      result = true;
      break;

    case WH_Segment2D::NO_INTERSECTION_WITH_LINE:
    case WH_Segment2D::PARALLEL_WITH_LINE:
      WH_CVR_LINE;
      edgeData_OUT[iEdge].intersectionType 
	= EdgeIntersectionData::HAS_NO_INTERSECTION;
      break;

    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }  
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);

    switch (edgeData_OUT[iEdge].intersectionType) {
    case EdgeIntersectionData::HAS_INTERSECTION:
      {
	WH_Vector2D point = edgeData_OUT[iEdge].intersectionPoint;
	switch (edgeData_OUT[iEdge].positionType) {
	case EdgeIntersectionData::AT_VERTEX0:
	  WH_ASSERT(WH_eq (point, edge.p0 ()));
	  break;
	case EdgeIntersectionData::AT_VERTEX1:
	  WH_ASSERT(WH_eq (point, edge.p1 ()));
	  break;
	case EdgeIntersectionData::ON_EDGE:
	  WH_ASSERT(edge.justContains (point));
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
      }
      break;
    case EdgeIntersectionData::IS_COINCIDENT:
      {
	WH_ASSERT(WH_eq (edge.line (), line)
		  || WH_isReverse (edge.line (), line));
	int iEdge_prev = this->prevEdgeIndex (iEdge);
	if (edgeData_OUT[iEdge_prev].intersectionType
	    != EdgeIntersectionData::IS_COINCIDENT) {
	  WH_ASSERT(edgeData_OUT[iEdge_prev].intersectionType
		  == EdgeIntersectionData::HAS_INTERSECTION);
	  WH_ASSERT(edgeData_OUT[iEdge_prev].positionType
		  == EdgeIntersectionData::AT_VERTEX1);
	}
	int iEdge_next = this->nextEdgeIndex (iEdge);
	if (edgeData_OUT[iEdge_next].intersectionType
	    != EdgeIntersectionData::IS_COINCIDENT) {
	  WH_ASSERT(edgeData_OUT[iEdge_next].intersectionType
		  == EdgeIntersectionData::HAS_INTERSECTION);
	  WH_ASSERT(edgeData_OUT[iEdge_next].positionType
		  == EdgeIntersectionData::AT_VERTEX0);
	}
      }
      break;
    case EdgeIntersectionData::HAS_NO_INTERSECTION:
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }
#endif

  return result;
}

bool WH_Polygon2D_A
::intersectsWith 
(const WH_Segment2D& segment,
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edgeData_OUT != WH_NULL);
  
  WH_CVR_LINE;

  bool result = false;

  if (WH_minMaxPairsOverlap 
      (this->minRange (), this->maxRange (),
       segment.minRange (), segment.maxRange ())) {
    WH_CVR_LINE;
    /* pass box check */

    for (int iEdge = 0; 
	 iEdge < this->nEdges (); 
	 iEdge++) {
      WH_Segment2D edge = this->edge (iEdge);
      
      int iVertex0;
      int iVertex1;
      this->getVertexIndexsOfEdge 
	(iEdge,
	 iVertex0, iVertex1);
      
      WH_Vector2D point;
      WH_Segment2D::WithSegmentIntersectionType intersectioFlag 
	= edge.checkIntersectionWith (segment, 
				      point);
      switch (intersectioFlag) {
      case WH_Segment2D::POINT_WITH_SEGMENT:
	WH_CVR_LINE;
	edgeData_OUT[iEdge].intersectionType 
	  = EdgeIntersectionData::HAS_INTERSECTION;
	edgeData_OUT[iEdge].intersectionPoint = point;
	if (WH_eq (point, edge.p0 ())) {
	  WH_CVR_LINE;
	  edgeData_OUT[iEdge].positionType 
	    = EdgeIntersectionData::AT_VERTEX0;
	} else if (WH_eq (point, edge.p1 ())) {
	  WH_CVR_LINE;
	  edgeData_OUT[iEdge].positionType 
	    = EdgeIntersectionData::AT_VERTEX1;
	} else {
	  WH_CVR_LINE;
	  edgeData_OUT[iEdge].positionType 
	    = EdgeIntersectionData::ON_EDGE;
	}
	result = true;
	break;
	
      case WH_Segment2D::COINCIDENT_WITH_SEGMENT:
	WH_CVR_LINE;
	edgeData_OUT[iEdge].intersectionType 
	  = EdgeIntersectionData::IS_COINCIDENT;
	result = true;
	break;
	
      case WH_Segment2D::NO_INTERSECTION_WITH_SEGMENT:
      case WH_Segment2D::PARALLEL_WITH_SEGMENT:
	WH_CVR_LINE;
	edgeData_OUT[iEdge].intersectionType 
	  = EdgeIntersectionData::HAS_NO_INTERSECTION;
	break;
	
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }  

    if (!result) {
      WH_CVR_LINE;
      /* polygon may contain <segment> */
      
      ContainmentType flag 
	= this->checkContainmentAt (segment.p0 ());
      if (flag == IN) {
	WH_CVR_LINE;
	WH_ASSERT(this->checkContainmentAt (segment.p1 ()) == IN);
	result = true;
      }
    }

  } else {
    WH_CVR_LINE;
    /* no overlap in box check */

    for (int iEdge = 0; 
	 iEdge < this->nEdges (); 
	 iEdge++) {
      edgeData_OUT[iEdge].intersectionType 
	= EdgeIntersectionData::HAS_NO_INTERSECTION;
    }
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge = this->edge (iEdge);

    switch (edgeData_OUT[iEdge].intersectionType) {
    case EdgeIntersectionData::HAS_INTERSECTION:
      {
	WH_Vector2D point = edgeData_OUT[iEdge].intersectionPoint;
	switch (edgeData_OUT[iEdge].positionType) {
	case EdgeIntersectionData::AT_VERTEX0:
	  WH_ASSERT(WH_eq (point, edge.p0 ()));
	  break;
	case EdgeIntersectionData::AT_VERTEX1:
	  WH_ASSERT(WH_eq (point, edge.p1 ()));
	  break;
	case EdgeIntersectionData::ON_EDGE:
	  WH_ASSERT(edge.justContains (point));
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
      }
      break;
    case EdgeIntersectionData::IS_COINCIDENT:
      {
	WH_ASSERT(WH_eq (edge.line (), segment.line ())
		  || WH_isReverse (edge.line (), segment.line ()));
	if (segment.contains (edge.p0 ())) {
	  int iEdge_prev = this->prevEdgeIndex (iEdge);
	  if (edgeData_OUT[iEdge_prev].intersectionType
	      != EdgeIntersectionData::IS_COINCIDENT) {
	    WH_ASSERT(edgeData_OUT[iEdge_prev].intersectionType
		    == EdgeIntersectionData::HAS_INTERSECTION);
	    WH_ASSERT(edgeData_OUT[iEdge_prev].positionType
		    == EdgeIntersectionData::AT_VERTEX1);
	  }
	}
	if (segment.contains (edge.p1 ())) {
	  int iEdge_next = this->nextEdgeIndex (iEdge);
	  if (edgeData_OUT[iEdge_next].intersectionType
	      != EdgeIntersectionData::IS_COINCIDENT) {
	    WH_ASSERT(edgeData_OUT[iEdge_next].intersectionType
		    == EdgeIntersectionData::HAS_INTERSECTION);
	    WH_ASSERT(edgeData_OUT[iEdge_next].positionType
		    == EdgeIntersectionData::AT_VERTEX0);
	  }
	}
      }
      break;
    case EdgeIntersectionData::HAS_NO_INTERSECTION:
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }
#endif

  return result;
}



/* class WH_Polygon2D */

WH_Polygon2D
::WH_Polygon2D ()
{
  WH_CVR_LINE;

  _nVertexs = 3;
  _vertexs = new WH_Vector2D[_nVertexs];
  _vertexs[0] = WH_Vector2D (0, 0);
  _vertexs[1] = WH_Vector2D (1, 0);
  _vertexs[2] = WH_Vector2D (0, 1);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon2D
::WH_Polygon2D 
(int nVertexs, const WH_Vector2D vertexs[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 < nVertexs);
  WH_ASSERT(vertexs != WH_NULL);
  
  WH_CVR_LINE;

  _nVertexs = nVertexs;
  _vertexs = new WH_Vector2D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = vertexs[iVertex];
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon2D
::WH_Polygon2D 
(const vector<WH_Vector2D>& vertex_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(2 < vertex_s.size ());

  WH_CVR_LINE;
  
  _nVertexs = (int)vertex_s.size ();
  _vertexs = new WH_Vector2D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = vertex_s[iVertex];
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon2D
::WH_Polygon2D 
(const WH_Polygon2D& polygon)
{
  WH_CVR_LINE;

  _nVertexs = polygon._nVertexs;
  _vertexs = new WH_Vector2D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = polygon._vertexs[iVertex];
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon2D
::~WH_Polygon2D ()
{
  WH_CVR_LINE;

  delete[] _vertexs;
}

const WH_Polygon2D& WH_Polygon2D
::operator= (const WH_Polygon2D& polygon)
{
  WH_CVR_LINE;
  if (this != &polygon) {
    WH_CVR_LINE;
    delete[] _vertexs;
    _nVertexs = polygon._nVertexs;
    _vertexs = new WH_Vector2D[_nVertexs];
    WH_ASSERT(_vertexs != WH_NULL);
    for (int iVertex = 0; 
	 iVertex < _nVertexs; 
	 iVertex++) {
      _vertexs[iVertex] = polygon._vertexs[iVertex];
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif

  return *this;
}

bool WH_Polygon2D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon2D_A::checkInvariant ();

  /* implementation class invariant */
  WH_ASSERT(2 < _nVertexs);
  WH_ASSERT(_vertexs != WH_NULL);
  
  return true;
}

bool WH_Polygon2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon2D_A::assureInvariant ();

  return true;
}

ostream& operator<< (ostream& out, const WH_Polygon2D& polygon)
{
  WH_CVR_LINE;

  out << "WH_Polygon2D ";
  out << polygon.nVertexs () << " ";
  for (int iVertex = 0; 
       iVertex < polygon.nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex = polygon.vertex (iVertex);
    out << vertex;
  }
  return out;
}

int WH_Polygon2D
::nVertexs () const
{
  return _nVertexs;
}

WH_Vector2D WH_Polygon2D
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  return _vertexs[iVertex];
}



/* test coverage completed */
