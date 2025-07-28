/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* polygon3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "polygon3d.h"



/* class WH_Polygon3D_A */

WH_Polygon3D_A
::WH_Polygon3D_A ()
{
  WH_CVR_LINE;
}

WH_Polygon3D_A
::~WH_Polygon3D_A ()
{
  WH_CVR_LINE;
}

bool WH_Polygon3D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_Polygon3D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(2 < this->nVertexs ());

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex0 = this->vertex (iVertex);
    WH_Vector3D vertex1 
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
  
  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
  WH_ASSERT(WH_le (minRange, maxRange));

  if (!this->hasAnyVertexOffPlane ()) {
    WH_CVR_LINE;

    WH_Plane3D plane = this->plane ();
    for (int iVertex = 0; 
	 iVertex < this->nVertexs (); 
	 iVertex++) {
      WH_Vector3D vertex = this->vertex (iVertex);
      WH_ASSERT(plane.contains (vertex));
      WH_Vector2D vertex2D = this->vertex2D (iVertex);
      WH_ASSERT(WH_eq (vertex2D, plane.parameterAt (vertex)));
    }

    WH_Polygon2D polygon2D = this->polygon2D ();
    for (int iVertex = 0; 
	 iVertex < this->nVertexs (); 
	 iVertex++) {
      WH_Vector3D vertex = this->vertex (iVertex);
      WH_Vector2D vertex2D = polygon2D.vertex (iVertex);
      WH_ASSERT(WH_eq (plane.parameterAt (vertex), vertex2D));
      WH_ASSERT(WH_eq (plane.positionAt (vertex2D), vertex));
    }

    if (this->isRegular ()) {
      WH_CVR_LINE;
      
      WH_Vector3D centerOfGravity = this->centerOfGravity ();
      WH_ASSERT(WH_between (centerOfGravity, minRange, maxRange));
      
      WH_ASSERT(WH_lt (0, this->perimeter ()));
      WH_ASSERT(WH_lt2 (0, this->area ()));
      
      for (int iVertex = 0; 
	   iVertex < this->nVertexs (); 
	   iVertex++) {
	WH_Vector3D vertex = this->vertex (iVertex);
	ContainmentType flag = this
	  ->checkContainmentAt (vertex);
	WH_ASSERT(flag == ON);
      }
      
      for (int iEdge = 0; 
	   iEdge < this->nEdges (); 
	   iEdge++) {
	WH_Segment3D edge = this->edge (iEdge);
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
  }

  return true;
}

int WH_Polygon3D_A
::nEdges () const
{
  return this->nVertexs ();
}

WH_Segment3D WH_Polygon3D_A
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
  WH_Vector3D v0 = this->vertex (iVertex0);
  WH_Vector3D v1 = this->vertex (iVertex1);
  WH_ASSERT(WH_ne (v0, v1));
  return WH_Segment3D (v0, v1);
}

int WH_Polygon3D_A
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

int WH_Polygon3D_A
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

int WH_Polygon3D_A
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

int WH_Polygon3D_A
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

void WH_Polygon3D_A
::getVertexIndexsOfEdge 
(int iEdge,
 int& iVertex0_OUT, int& iVertex1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

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

bool WH_Polygon3D_A
::hasAnyVertexOffPlane () const
{
  WH_CVR_LINE;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    int i1_vertex = this->nextVertexIndex (iVertex);
    int i2_vertex = this->nextVertexIndex (i1_vertex);
    WH_Vector3D vertex0 = this->vertex (iVertex);
    WH_Vector3D vertex1 = this->vertex (i1_vertex);
    WH_Vector3D vertex2 = this->vertex (i2_vertex);
    WH_Vector3D vp = WH_vectorProduct 
      (vertex0 - vertex1, vertex0 - vertex2);
    if (WH_ne2 (vp, WH_Vector3D::zero ())) {
      WH_CVR_LINE;
      /* plane is defined */
      WH_Plane3D plane (vertex0, vertex1, vertex2);

      for (int jVertex = 0; 
	   jVertex < this->nVertexs (); 
	   jVertex++) {
	WH_Vector3D vertex_j = this->vertex (jVertex);
	if (!plane.contains (vertex_j)) {
	  WH_CVR_LINE;
	  /* there is a vertex off <plane> */
	  return true;
	}
      }

      /* all the vertices are on <plane> */
      return false;
    }
  }

  /* plane is not defined */
  return true;
}

WH_Plane3D WH_Polygon3D_A
::plane () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif
  
  WH_CVR_LINE;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    int i1_vertex = this->nextVertexIndex (iVertex);
    int i2_vertex = this->nextVertexIndex (i1_vertex);
    WH_Vector3D vertex0 = this->vertex (iVertex);
    WH_Vector3D vertex1 = this->vertex (i1_vertex);
    WH_Vector3D vertex2 = this->vertex (i2_vertex);
    WH_Vector3D vp = WH_vectorProduct 
      (vertex0 - vertex1, vertex0 - vertex2);
    if (WH_ne2 (vp, WH_Vector3D::zero ())) {
      WH_CVR_LINE;
      /* plane is defined */
      return WH_Plane3D (vertex0, vertex1, vertex2);
    }
  }

  /* plane is not defined */
  WH_ASSERT_NO_REACH;
  return WH_Plane3D ();
}

WH_Vector2D WH_Polygon3D_A
::vertex2D (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  return this->plane ().parameterAt (this->vertex (iVertex));
}

WH_Segment2D WH_Polygon3D_A
::edge2D (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  WH_CVR_LINE;

  int iVertex0;
  int iVertex1;
  this->getVertexIndexsOfEdge (iEdge, 
			       iVertex0, iVertex1);
  WH_Vector2D v0 = this->vertex2D (iVertex0);
  WH_Vector2D v1 = this->vertex2D (iVertex1);
  WH_ASSERT(WH_ne (v0, v1));
  return WH_Segment2D (v0, v1);
}

WH_Polygon2D WH_Polygon3D_A
::polygon2D () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  WH_CVR_LINE;

  WH_Vector2D* vector2Ds 
    = new WH_Vector2D[this->nVertexs ()];
  WH_ASSERT(vector2Ds != WH_NULL);

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    vector2Ds[iVertex] = this->vertex2D (iVertex);
  }
  WH_Polygon2D result (this->nVertexs (), vector2Ds);

  delete[] vector2Ds;
  vector2Ds = WH_NULL;

  return result;
}

bool WH_Polygon3D_A
::hasAnyDuplicatedVertex () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex_i = this->vertex (iVertex);
    
    for (int jVertex = iVertex + 1; 
	 jVertex < this->nVertexs (); 
	 jVertex++) {
      WH_Vector3D vertex_j = this->vertex (jVertex);

      if (WH_eq (vertex_i, vertex_j) ) {
	WH_CVR_LINE;
	result = true;
	break;
      }
    }
  }

  return result;
}

bool WH_Polygon3D_A
::hasAnyCrossingEdge () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment3D edge_i = this->edge (iEdge);
    int iEdgeNext = this->nextEdgeIndex (iEdge);
    int iEdgePrev = this->prevEdgeIndex (iEdge);
    
    for (int jEdge = iEdge + 1; 
	 jEdge < this->nEdges (); 
	 jEdge++) {
      WH_Segment3D edge_j = this->edge (jEdge);

      WH_Vector3D point;
      WH_Segment3D::WithSegmentIntersectionType intersectionFlag = edge_i
	.checkIntersectionWith (edge_j,
				point);
      if (intersectionFlag == WH_Segment3D::POINT_WITH_SEGMENT) {
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
      } else if (intersectionFlag == WH_Segment3D::COINCIDENT_WITH_SEGMENT) {
	WH_CVR_LINE;
	WH_Segment3D::WithSegmentOverlapType overlapFlag = edge_i
	  .checkOverlapWith (edge_j);
	if (overlapFlag == WH_Segment3D::OVERLAP_WITH_SEGMENT) {
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
	} else if (overlapFlag != WH_Segment3D::NO_OVERLAP_WITH_SEGMENT) {
	  WH_CVR_LINE;
	  result = true;
	  break;
	}
      }
    }
  }

  return result;
}

bool WH_Polygon3D_A
::hasAnySpecialAngle () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment2D edge2D = this->edge2D (iEdge);
    WH_Segment2D edgeNext2D = this->edge2D (this->nextEdgeIndex (iEdge));

    /* check angle of 0 degree */
    /* angle of 180 degree (flat angle) is considered as regular  */
    if (WH_eq (WH_angleOfLines (edge2D.line (), edgeNext2D.line ()), M_PI)) {
      WH_CVR_LINE;
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polygon3D_A
::isRegular () const
{
  return !this->hasAnyVertexOffPlane ()
    && !this->hasAnyDuplicatedVertex ()
    && !this->hasAnyCrossingEdge ()
    && !this->hasAnySpecialAngle ();
}

bool WH_Polygon3D_A
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

    /* obtain sign of angle between the edge and its next edge */
    int iVertex0;
    int iVertex1;
    this->getVertexIndexsOfEdge (iEdge, 
				 iVertex0, iVertex1);
    WH_Vector2D vertex2D0 = this->vertex2D (iVertex0); 
    WH_Vector2D vertex2D1 = this->vertex2D (iVertex1); 
    WH_Vector2D nextVertex2D 
      = this->vertex2D (this->nextVertexIndex (iVertex1));
    double sign = WH_Segment2D (vertex2D0, vertex2D1)
      .signedTriangleAreaWith (nextVertex2D);
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
  }

  return result;
}

double WH_Polygon3D_A
::perimeter () const
{
  WH_CVR_LINE;

  double result = 0.0;
  
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment3D edge = this->edge (iEdge);
    result += edge.length ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt (0, result));
#endif

  return result;
}

double WH_Polygon3D_A
::area () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  double result = 0.0;

  WH_Vector2D origin2D = this->vertex2D (0);
  for (int iVertex = 1; 
       iVertex < this->nVertexs () - 1; 
       iVertex++) {
    WH_Vector2D vertex2D0 = this->vertex2D (iVertex);
    WH_Vector2D vertex2D1 
      = this->vertex2D (this->nextVertexIndex (iVertex));
    double signedArea = WH_Segment2D (vertex2D0, vertex2D1)
      .signedTriangleAreaWith (origin2D);
    result += signedArea;
  }
  result = fabs (result);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt2 (0, result));
#endif
  
  return result;
}

WH_Vector3D WH_Polygon3D_A
::centerOfGravity () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  WH_Vector2D center2D (0, 0);
  double areaSum = 0.0;
  WH_Vector2D origin2D = this->vertex2D (0);
  for (int iVertex = 1; 
       iVertex < this->nVertexs () - 1; 
       iVertex++) {
    WH_Vector2D vertex2D0 = this->vertex2D (iVertex);
    WH_Vector2D vertex2D1 
      = this->vertex2D (this->nextVertexIndex (iVertex));
    double signedArea = WH_Segment2D (vertex2D0, vertex2D1)
      .signedTriangleAreaWith (origin2D);
    areaSum += signedArea;
    center2D += (vertex2D0 + vertex2D1) * signedArea;
  }
  WH_ASSERT(WH_ne2 (0, areaSum));
  center2D = (center2D / areaSum + origin2D) / 3;

  WH_Plane3D plane = this->plane ();
  WH_Vector3D result = plane.positionAt (center2D);

  return result;
}

WH_Vector3D WH_Polygon3D_A
::minRange () const
{
  WH_CVR_LINE;

  WH_Vector3D result = WH_Vector3D::hugeValue ();
  
  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = this->vertex (iVertex);
    result = WH_min (result, vertex);
  }

  return result;
}

WH_Vector3D WH_Polygon3D_A
::maxRange () const
{
  WH_CVR_LINE;

  WH_Vector3D result = -WH_Vector3D::hugeValue ();
  
  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = this->vertex (iVertex);
    result = WH_max (result, vertex);
  }

  return result;
}

bool WH_Polygon3D_A
::hasVertexAt (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iVertex = 0; 
       iVertex < this->nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = this->vertex (iVertex);
    if (WH_eq (vertex, position)) {
      WH_CVR_LINE;
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polygon3D_A
::hasVertexAtEveryPointIn
(const vector<WH_Vector3D>& point_s) const
{
  WH_CVR_LINE;

  bool result = true;

  for (vector<WH_Vector3D>::const_iterator
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    if (!this->hasVertexAt (point_i)) {
      WH_CVR_LINE;
      result = false;
      break;
    }
  }

  return result;
}

bool WH_Polygon3D_A
::containsOnBoundary (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment3D edge = this->edge (iEdge);
    if (edge.contains (position)) {
      WH_CVR_LINE;
      result = true;
      break;
    }
  }  

  return result;
}

bool WH_Polygon3D_A
::containsInside (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  bool result = false;

  WH_Plane3D plane = this->plane ();
  if (plane.contains (position)) {
    WH_CVR_LINE;
    /* <position> is on <plane> of the polygon */
    WH_Vector2D position2D = plane.parameterAt (position);

    /* calculate sum of angles from <position> to all edges */
    
    double sumOfAngle = 0.0;
    
    for (int iEdge = 0; 
	 iEdge < this->nEdges (); 
	 iEdge++) {
      WH_Segment2D edge2D = this->edge2D (iEdge);
      
      WH_ASSERT(!edge2D.contains (position2D));
      /* <position2D> is not on the edge */
      
      double angle = WH_angleOfVectors (edge2D.p0 () - position2D, 
					edge2D.p1 () - position2D);
      WH_ASSERT(WH_lt (-M_PI, angle));
      WH_ASSERT(WH_le (angle, M_PI));
      
      WH_ASSERT(WH_ne (angle, M_PI));
      /* <position2D> is not on the edge */
      
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
  }

  return result;
}

WH_Polygon3D_A::ContainmentType WH_Polygon3D_A
::checkContainmentAt (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  ContainmentType result = OUT;
  
  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
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

void WH_Polygon3D_A
::moveEdgeData 
(WH_Polygon2D_A::EdgeIntersectionData edgeData2D[],
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edgeData2D != WH_NULL);
  WH_ASSERT(edgeData_OUT != WH_NULL);

  WH_CVR_LINE;

  WH_Plane3D plane = this->plane ();
  
  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Polygon2D_A::EdgeIntersectionData* src = &edgeData2D[iEdge];
    EdgeIntersectionData* dest = &edgeData_OUT[iEdge];
    
    switch (src->intersectionType) {
    case WH_Polygon2D_A::EdgeIntersectionData::HAS_NO_INTERSECTION:
      WH_CVR_LINE;
      dest->intersectionType 
	= EdgeIntersectionData::HAS_NO_INTERSECTION;
      break;
    case WH_Polygon2D_A::EdgeIntersectionData::HAS_INTERSECTION:
      WH_CVR_LINE;
      {
	dest->intersectionType 
	  = EdgeIntersectionData::HAS_INTERSECTION;
	dest->intersectionPoint 
	  = plane.positionAt (src->intersectionPoint);
	switch (src->positionType) {
	case WH_Polygon2D_A::EdgeIntersectionData::AT_VERTEX0:
	  WH_CVR_LINE;
	  dest->positionType = EdgeIntersectionData::AT_VERTEX0;
	  break;
	case WH_Polygon2D_A::EdgeIntersectionData::AT_VERTEX1:
	  WH_CVR_LINE;
	  dest->positionType = EdgeIntersectionData::AT_VERTEX1;
	  break;
	case WH_Polygon2D_A::EdgeIntersectionData::ON_EDGE:
	  WH_CVR_LINE;
	  dest->positionType = EdgeIntersectionData::ON_EDGE;
	  break;
	default:  
	  WH_ASSERT_NO_REACH;  
	  break;
	}
      }
      break;
    case WH_Polygon2D_A::EdgeIntersectionData::IS_COINCIDENT:
      WH_CVR_LINE;
      dest->intersectionType 
	= EdgeIntersectionData::IS_COINCIDENT;
      break;
    default:  
      WH_ASSERT_NO_REACH;  
      break;
    }
  }
}

WH_Polygon3D_A::WithLineIntersectionType WH_Polygon3D_A
::checkIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point_OUT,
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  WithLineIntersectionType result = NO_INTERSECTION_WITH_LINE;

  WH_Vector3D pointWithLine;
  WH_Plane3D::WithLineIntersectionType flagWithLine
    = this->plane ().checkIntersectionWith (line,
					    pointWithLine);

  switch (flagWithLine) {
  case WH_Plane3D::POINT_WITH_LINE:
    WH_CVR_LINE;
    {
      ContainmentType containmentFlag 
	= this->checkContainmentAt (pointWithLine);
      if (containmentFlag == IN
	  || containmentFlag == ON) {
	WH_CVR_LINE;
	result = POINT_WITH_LINE;
	point_OUT = pointWithLine;
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_LINE;
      }
    }
    break;
  case WH_Plane3D::PARALLEL_WITH_LINE:
    WH_CVR_LINE;
    result = PARALLEL_WITH_LINE;
    break;
  case WH_Plane3D::COINCIDENT_WITH_LINE:
    WH_CVR_LINE;
    {
      WH_Line2D line2D = this->plane ().line2D (line);
      WH_Polygon2D polygon2D = this->polygon2D ();
      
      WH_Polygon2D::EdgeIntersectionData* edgeData2D 
	= new WH_Polygon2D::EdgeIntersectionData[this->nEdges ()];
      WH_ASSERT(edgeData2D != WH_NULL);
      
      if (polygon2D.intersectsWith (line2D,
				    edgeData2D)) {
	WH_CVR_LINE;
	result = COINCIDENT_WITH_LINE;
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_LINE;
      }
      this->moveEdgeData (edgeData2D, 
			  edgeData_OUT);
      
      delete[] edgeData2D;
      edgeData2D = WH_NULL;
    }
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }

  return result;
}

WH_Polygon3D_A::WithPlaneIntersectionType WH_Polygon3D_A
::checkIntersectionWith 
(const WH_Plane3D& plane,
 WH_Line3D& line_OUT,
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  WithPlaneIntersectionType result = NO_INTERSECTION_WITH_PLANE;

  WH_Line3D lineWithPlane;
  WH_Plane3D::WithPlaneIntersectionType flagWithPlane
    = this->plane ().checkIntersectionWith (plane,
					    lineWithPlane);

  switch (flagWithPlane) {
  case WH_Plane3D::LINE_WITH_PLANE:
    WH_CVR_LINE;
    {
      WH_Line2D line2D = this->plane ().line2D (lineWithPlane);
      WH_Polygon2D polygon2D = this->polygon2D ();
      
      WH_Polygon2D::EdgeIntersectionData* edgeData2D 
	= new WH_Polygon2D::EdgeIntersectionData[this->nEdges ()];
      WH_ASSERT(edgeData2D != WH_NULL);
      
      if (polygon2D.intersectsWith (line2D,
				    edgeData2D)) {
	WH_CVR_LINE;
	result = LINE_WITH_PLANE;
	line_OUT = lineWithPlane;
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_PLANE;
      }
      this->moveEdgeData (edgeData2D, 
			  edgeData_OUT);
      
      delete[] edgeData2D;
      edgeData2D = WH_NULL;
    }
    break;
  case WH_Plane3D::PARALLEL_WITH_PLANE:
    WH_CVR_LINE;
    result = PARALLEL_WITH_PLANE;
    break;
  case WH_Plane3D::COINCIDENT_WITH_PLANE:
    WH_CVR_LINE;
    result = COINCIDENT_WITH_PLANE;
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }

  return result;
}

WH_Polygon3D_A::WithSegmentIntersectionType WH_Polygon3D_A
::checkIntersectionWith
(const WH_Segment3D& segment,
 WH_Vector3D& point_OUT,
 EdgeIntersectionData edgeData_OUT[]) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WithSegmentIntersectionType result = NO_INTERSECTION_WITH_SEGMENT;

  WH_Vector3D pointWithPlane;
  WH_Segment3D::WithPlaneIntersectionType flagWithPlane
    = segment.checkIntersectionWith 
    (this->plane (),
     pointWithPlane);

  switch (flagWithPlane) {
  case WH_Segment3D::NO_INTERSECTION_WITH_PLANE:
    WH_CVR_LINE;
    result = NO_INTERSECTION_WITH_SEGMENT;
    break;
  case WH_Segment3D::POINT_WITH_PLANE:
    WH_CVR_LINE;
    {
      ContainmentType containmentFlag 
	= this->checkContainmentAt (pointWithPlane);
      if (containmentFlag == IN
	  || containmentFlag == ON) {
	WH_CVR_LINE;
	result = POINT_WITH_SEGMENT;
	point_OUT = pointWithPlane;
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_SEGMENT;
      }
    }
    break;
  case WH_Segment3D::PARALLEL_WITH_PLANE:
    WH_CVR_LINE;
    result = PARALLEL_WITH_SEGMENT;
    break;
  case WH_Segment3D::COINCIDENT_WITH_PLANE:
    WH_CVR_LINE;
    {
      WH_Segment2D segment2D 
	(this->plane ().parameterAt (segment.p0 ()),
	 this->plane ().parameterAt (segment.p1 ()));
      WH_Polygon2D polygon2D = this->polygon2D ();
      
      WH_Polygon2D::EdgeIntersectionData* edgeData2D 
	= new WH_Polygon2D::EdgeIntersectionData[this->nEdges ()];
      WH_ASSERT(edgeData2D != WH_NULL);
      
      if (polygon2D.intersectsWith (segment2D,
				    edgeData2D)) {
	WH_CVR_LINE;
	result = COINCIDENT_WITH_SEGMENT;
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_SEGMENT;
      }
      this->moveEdgeData (edgeData2D, 
			  edgeData_OUT);
      
      delete[] edgeData2D;
      edgeData2D = WH_NULL;
    }
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }

  return result;
}



/* class WH_Polygon3D */

WH_Polygon3D
::WH_Polygon3D ()
{
  WH_CVR_LINE;

  _nVertexs = 3;
  _vertexs = new WH_Vector3D[_nVertexs];
  _vertexs[0] = WH_Vector3D (0, 0, 0);
  _vertexs[1] = WH_Vector3D (1, 0, 0);
  _vertexs[2] = WH_Vector3D (0, 1, 0);

  _planeExists = true;
  _plane = this->WH_Polygon3D_A::plane ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon3D
::WH_Polygon3D 
(int nVertexs, const WH_Vector3D vertexs[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 < nVertexs);
  WH_ASSERT(vertexs != WH_NULL);
  
  WH_CVR_LINE;

  _nVertexs = nVertexs;
  _vertexs = new WH_Vector3D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = vertexs[iVertex];
  }

  if (this->WH_Polygon3D_A::hasAnyVertexOffPlane ()) {
    WH_CVR_LINE;
    _planeExists = false;
  } else {
    WH_CVR_LINE;
    _planeExists = true;
    _plane = this->WH_Polygon3D_A::plane ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon3D
::WH_Polygon3D 
(const vector<WH_Vector3D>& vertex_s)
{
  /* PRE-CONDITION */
  WH_ASSERT(2 < vertex_s.size ());

  WH_CVR_LINE;
  
  _nVertexs = (int)vertex_s.size ();
  _vertexs = new WH_Vector3D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = vertex_s[iVertex];
  }

  if (this->WH_Polygon3D_A::hasAnyVertexOffPlane ()) {
    WH_CVR_LINE;
    _planeExists = false;
  } else {
    WH_CVR_LINE;
    _planeExists = true;
    _plane = this->WH_Polygon3D_A::plane ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon3D
::WH_Polygon3D 
(const WH_Polygon3D& polygon)
{
  WH_CVR_LINE;

  _nVertexs = polygon._nVertexs;
  _vertexs = new WH_Vector3D[_nVertexs];
  WH_ASSERT(_vertexs != WH_NULL);
  for (int iVertex = 0; 
       iVertex < _nVertexs; 
       iVertex++) {
    _vertexs[iVertex] = polygon._vertexs[iVertex];
  }

  _planeExists = polygon._planeExists;
  _plane = polygon._plane;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Polygon3D
::WH_Polygon3D 
(WH_Polygon3D&& polygon) noexcept
{
  WH_CVR_LINE;

  _nVertexs = polygon._nVertexs;
  _vertexs = polygon._vertexs;
  _planeExists = polygon._planeExists;
  _plane = polygon._plane;

  // Reset moved-from object
  polygon._nVertexs = 0;
  polygon._vertexs = WH_NULL;
  polygon._planeExists = false;
}

WH_Polygon3D
::~WH_Polygon3D ()
{
  WH_CVR_LINE;

  delete[] _vertexs;
}

const WH_Polygon3D& WH_Polygon3D
::operator= (const WH_Polygon3D& polygon)
{
  WH_CVR_LINE;

  if (this != &polygon) {
    delete[] _vertexs;
    _nVertexs = polygon._nVertexs;
    _vertexs = new WH_Vector3D[_nVertexs];
    WH_ASSERT(_vertexs != WH_NULL);
    for (int iVertex = 0; 
	 iVertex < _nVertexs; 
	 iVertex++) {
      _vertexs[iVertex] = polygon._vertexs[iVertex];
    }

    _planeExists = polygon._planeExists;
    _plane = polygon._plane;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif

  return *this;
}

WH_Polygon3D& WH_Polygon3D
::operator= (WH_Polygon3D&& polygon) noexcept
{
  WH_CVR_LINE;

  if (this != &polygon) {
    delete[] _vertexs;
    
    _nVertexs = polygon._nVertexs;
    _vertexs = polygon._vertexs;
    _planeExists = polygon._planeExists;
    _plane = polygon._plane;

    // Reset moved-from object
    polygon._nVertexs = 0;
    polygon._vertexs = WH_NULL;
    polygon._planeExists = false;
  }

  return *this;
}

bool WH_Polygon3D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon3D_A::checkInvariant ();

  /* implementation class invariant */
  WH_ASSERT(2 < _nVertexs);
  WH_ASSERT(_vertexs != WH_NULL);
  
  return true;
}

bool WH_Polygon3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon3D_A::assureInvariant ();
 
  return true;
}

ostream& operator<< (ostream& out, const WH_Polygon3D& polygon)
{
  WH_CVR_LINE;

  out << "WH_Polygon3D ";
  out << polygon.nVertexs () << " ";
  for (int iVertex = 0; 
       iVertex < polygon.nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = polygon.vertex (iVertex);
    out << vertex;
  }
  return out;
}

int WH_Polygon3D
::nVertexs () const
{
  return _nVertexs;
}

WH_Vector3D WH_Polygon3D
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  return _vertexs[iVertex];
}

bool WH_Polygon3D
::hasAnyVertexOffPlane () const
{
  return !_planeExists;
}

WH_Plane3D WH_Polygon3D
::plane () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  return _plane;
}



/* test coverage completed */
