/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* triangle3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "triangle3d.h"



/* class WH_Triangle3D_A */

const int WH_Triangle3D_A::nTriangleVertexs = 3;
const int WH_Triangle3D_A::nTriangleEdges = 3;
const int WH_Triangle3D_A::edgeVertexMap[3][2] = {
  { 1, 2 }, 
  { 2, 0 }, 
  { 0, 1 }
};

WH_Triangle3D_A
::WH_Triangle3D_A ()
{
  WH_CVR_LINE;
}

WH_Triangle3D_A
::~WH_Triangle3D_A ()
{
  WH_CVR_LINE;
}

bool WH_Triangle3D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon3D_A::checkInvariant ();

  return true;
}

bool WH_Triangle3D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon3D_A::assureInvariant ();

  WH_ASSERT(this->nVertexs () == 3);
  WH_ASSERT(this->nEdges () == 3);

  WH_ASSERT(this->isConvex ());

  WH_ASSERT(WH_eq (*this, *this));
  if (this->isRegular ()) {
    WH_ASSERT(WH_isSame (*this, *this));
  }

  return true;
}

void WH_Triangle3D_A
::getCircumcenter 
(WH_Vector3D& center_OUT, double& radius_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WH_Plane3D plane = this->plane ();
  WH_Vector2D k = this->vertex2D (0);
  WH_Vector2D l = this->vertex2D (1);
  WH_Vector2D m = this->vertex2D (2);
  double xlk = l.x - k.x;
  double ylk = l.y - k.y;
  double xmk = m.x - k.x;
  double ymk = m.y - k.y;
  double det = xlk * ymk - xmk * ylk;
  WH_ASSERT(WH_ne2 (det, 0));
  double rlk = xlk * xlk + ylk * ylk;
  double rmk = xmk * xmk + ymk * ymk;
  WH_Vector2D offset2D 
    = WH_Vector2D (rlk * ymk - rmk * ylk,
		   xlk * rmk - xmk * rlk) / (det * 2);
  WH_Vector2D center2D = offset2D + k;
  center_OUT = plane.positionAt (center2D);
  radius_OUT = offset2D.length ();
}

void WH_Triangle3D_A
::getInternalCenter 
(WH_Vector3D& center_OUT, double& radius_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WH_Plane3D plane = this->plane ();
  WH_Vector2D v0 = this->vertex2D (0);
  WH_Vector2D v1 = this->vertex2D (1);
  WH_Vector2D v2 = this->vertex2D (2);
  double r0 = this->edge (0).length ();
  double r1 = this->edge (1).length ();
  double r2 = this->edge (2).length ();
  double t = r0 + r1 + r2;
  double s = t / 2;
  WH_Vector2D center2D = (v0 * r1 + v1 * r2 + v2 * r0) / t;
  center_OUT = plane.positionAt (center2D);
  double sr = (s - r0) * (s - r1) * (s - r2) / s;
  WH_ASSERT(WH_lt2 (0, sr));
  radius_OUT = sqrt (sr);
}

int WH_Triangle3D_A
::nVertexs () const
{
  return 3;
}

int WH_Triangle3D_A
::nEdges () const
{
  return 3;
}

WH_Segment3D WH_Triangle3D_A
::edge (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

  WH_CVR_LINE;

  if (iEdge == 2) {
    WH_CVR_LINE;
    return WH_Segment3D (this->vertex (2), 
			 this->vertex (0));
    
  } else {
    WH_CVR_LINE;
    return WH_Segment3D (this->vertex (iEdge), 
			 this->vertex (iEdge + 1));
  }
}

bool WH_Triangle3D_A
::isConvex () const
{
  return true;
}

double WH_Triangle3D_A
::area () const
{
  WH_CVR_LINE;

  WH_Vector3D v0 = this->vertex (0);
  WH_Vector3D v1 = this->vertex (1);
  WH_Vector3D v2 = this->vertex (2);
  return WH_Segment3D (v0, v1).triangleAreaWith (v2);
}

WH_Vector3D WH_Triangle3D_A
::centerOfGravity () const
{
  WH_CVR_LINE;

  WH_Vector3D v0 = this->vertex (0);
  WH_Vector3D v1 = this->vertex (1);
  WH_Vector3D v2 = this->vertex (2);
  return (v0 + v1 + v2) / 3;
}

WH_Polygon3D_A::ContainmentType WH_Triangle3D_A
::checkContainmentAt (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  WH_Polygon3D_A::ContainmentType result = OUT;

  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
  if (WH_between (position, minRange, maxRange)) {
    WH_CVR_LINE;

    WH_Plane3D plane = this->plane ();
    if (plane.contains (position)) {
      WH_CVR_LINE;
      /* <position> is on <plane> of the polygon */
      WH_Vector2D position2D = plane.parameterAt (position);
      
      WH_Segment2D e0 = this->edge2D (0);
      WH_Segment2D e1 = this->edge2D (1);
      WH_Segment2D e2 = this->edge2D (2);
      double sign0 = e0.signedTriangleAreaWith (position2D);
      double sign1 = e1.signedTriangleAreaWith (position2D);
      double sign2 = e2.signedTriangleAreaWith (position2D);
      if ((WH_lt2 (0, sign0) && WH_lt2 (0, sign1) && WH_lt2 (0, sign2))
	  || (WH_lt2 (sign0, 0) && WH_lt2 (sign1, 0) && WH_lt2 (sign2, 0))) {
	WH_CVR_LINE;
	result = IN;
      } else if (e0.contains (position2D)
		 || e1.contains (position2D)
		 || e2.contains (position2D)) {
	WH_CVR_LINE;
	result = ON;
      }
    }
  }

  return result;
}

double WH_Triangle3D_A
::signedTetrahedronVolumeWith (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  WH_Vector3D v0 = this->vertex (0);
  WH_Vector3D v1 = this->vertex (1);
  WH_Vector3D v2 = this->vertex (2);
  WH_Vector3D v3 = position;
  
  double xk = v0.x;  double yk = v0.y;  double zk = v0.z;
  double xl = v1.x;  double yl = v1.y;  double zl = v1.z;
  double xm = v2.x;  double ym = v2.y;  double zm = v2.z;
  double xn = v3.x;  double yn = v3.y;  double zn = v3.z;

  double xlk = xl - xk;
  double ylk = yl - yk;
  double zlk = zl - zk;

  double xmk = xm - xk;
  double ymk = ym - yk;
  double zmk = zm - zk;
  
  double xnk = xn - xk;
  double ynk = yn - yk;
  double znk = zn - zk;
  
  double dmn = ymk * znk - zmk * ynk;
  double dln = ylk * znk - zlk * ynk;
  double dlm = ylk * zmk - zlk * ymk;
  
  return (xlk * dmn - xmk * dln + xnk * dlm) / 6;
}

bool WH_Triangle3D_A
::isClockWiseWith (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->plane ().contains (position));

  double sign = this->signedTetrahedronVolumeWith (position);
  WH_ASSERT(WH_ne3 (sign, 0));
  
  return WH_lt3 (sign, 0);
}

bool WH_eq (const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1)
{
  WH_CVR_LINE;

  WH_Vector3D a0 = tri0.vertex (0);
  WH_Vector3D a1 = tri0.vertex (1);
  WH_Vector3D a2 = tri0.vertex (2);

  WH_Vector3D b0 = tri1.vertex (0);
  WH_Vector3D b1 = tri1.vertex (1);
  WH_Vector3D b2 = tri1.vertex (2);

  return (WH_eq (a0, b0) && WH_eq (a1, b1) && WH_eq (a2, b2));
}

bool WH_ne (const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1)
{
  return !WH_eq (tri0, tri1);
}

int WH_nMatchedVertexs 
(const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1,
 bool& reverseFlag_OUT,
 int& vertexIdOfTriangle0_OUT, int& vertexIdOfTriangle1_OUT,
 int& edgeIdOfTriangle0_OUT, int& edgeIdOfTriangle1_OUT)
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(tri0.isRegular ());
  WH_ASSERT(tri1.isRegular ());
#endif

  WH_CVR_LINE;
  /* NEED TO REDEFINE */ 
  /* This function is not test-covered fully yet. */

  WH_Vector3D a0 = tri0.vertex (0);
  WH_Vector3D a1 = tri0.vertex (1);
  WH_Vector3D a2 = tri0.vertex (2);

  WH_Vector3D b0 = tri1.vertex (0);
  WH_Vector3D b1 = tri1.vertex (1);
  WH_Vector3D b2 = tri1.vertex (2);
  
  if (WH_eq (a0, b0)) {
    
    if (WH_eq (a1, b1)) {
      reverseFlag_OUT = false;
      if (WH_eq (a2, b2)) {
	/* (0, 1, 2) - (0, 1, 2) */
	return 3;
      } else {
	/* (0, 1) - (0, 1) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a1, b2)) {
      reverseFlag_OUT = true;
      if (WH_eq (a2, b1)) {
	/* (0, 1, 2) - (0, 2, 1) */
	return 3;
      } else {
	/* (0, 1) - (0, 2) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else if (WH_eq (a2, b1)) {
      reverseFlag_OUT = true;
      if (WH_eq (a1, b2)) {
	/* (0, 2, 1) - (0, 1, 2) */
	return 3;
      } else {
	/* (0, 2) - (0, 1) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a2, b2)) {
      reverseFlag_OUT = false;
      if (WH_eq (a1, b1)) {
	/* (0, 2, 1) - (0, 2, 1) */
	return 3;
      } else {
	/* (0, 2) - (0, 2) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else {
      /* (0) - (0) */
      vertexIdOfTriangle0_OUT = 0;
      vertexIdOfTriangle1_OUT = 0;
      return 1;
    }
    
  } else if (WH_eq (a0, b1)) {
    
    if (WH_eq (a1, b0)) {
      reverseFlag_OUT = true;
      if (WH_eq (a2, b2)) {
	/* (0, 1, 2) - (1, 0, 2) */
	return 3;
      } else {
	/* (0, 1) - (1, 0) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a1, b2)) {
      reverseFlag_OUT = false;
      if (WH_eq (a2, b0)) {
	/* (0, 1, 2) - (1, 2, 0) */
	return 3;
      } else {
	/* (0, 1) - (1, 2) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else if (WH_eq (a2, b0)) {
      reverseFlag_OUT = false;
      if (WH_eq (a1, b2)) {
	/* (0, 2, 1) - (1, 0, 2) */
	return 3;
      } else {
	/* (0, 2) - (1, 0) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a2, b2)) {
      reverseFlag_OUT = true;
      if (WH_eq (a1, b0)) {
	/* (0, 2, 1) - (1, 2, 0) */
	return 3;
      } else {
	/* (0, 2) - (1, 2) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else {
      /* (0) - (1) */
      vertexIdOfTriangle0_OUT = 0;
      vertexIdOfTriangle1_OUT = 1;
      return 1;
    }

  } else if (WH_eq (a0, b2)) {

    if (WH_eq (a1, b0)) {
      reverseFlag_OUT = false;
      if (WH_eq (a2, b1)) {
	/* (0, 1, 2) - (2, 0, 1) */
	return 3;
      } else {
	/* (0, 1) - (2, 0) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else if (WH_eq (a1, b1)) {
      reverseFlag_OUT = true;
      if (WH_eq (a2, b0)) {
	/* (0, 1, 2) - (2, 1, 0) */
	return 3;
      } else {
	/* (0, 1) - (2, 1) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else if (WH_eq (a2, b0)) {
      reverseFlag_OUT = true;
      if (WH_eq (a1, b1)) {
	/* (0, 2, 1) - (2, 0, 1) */
	return 3;
      } else {
	/* (0, 2) - (2, 0) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else if (WH_eq (a2, b1)) {
      reverseFlag_OUT = false;
      if (WH_eq (a1, b0)) {
	/* (0, 2, 1) - (2, 1, 0) */
	return 3;
      } else {
	/* (0, 2) - (2, 1) */
	edgeIdOfTriangle0_OUT = 2;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else {
      /* (0) - (2) */
      vertexIdOfTriangle0_OUT = 0;
      vertexIdOfTriangle1_OUT = 2;
      return 1;
    }

  } else if (WH_eq (a1, b0)) {

    if (WH_eq (a0, b1)) {
      reverseFlag_OUT = true;
      if (WH_eq (a2, b2)) {
	/* (1, 0, 2) - (0, 1, 2) */
	return 3;
      } else {
	/* (1, 0) - (0, 1) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a0, b2)) {
      reverseFlag_OUT = false;
      if (WH_eq (a2, b1)) {
	/* (1, 0, 2) - (0, 2, 1) */
	return 3;
      } else {
	/* (1, 0) - (0, 2) */
	edgeIdOfTriangle0_OUT = 0;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else if (WH_eq (a2, b1)) {
      reverseFlag_OUT = false;
      if (WH_eq (a0, b2)) {
	/* (1, 2, 0) - (0, 1, 2) */
	return 3;
      } else {
	/* (1, 2) - (0, 1) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a2, b2)) {
      reverseFlag_OUT = true;
      if (WH_eq (a0, b1)) {
	/* (1, 2, 0) - (0, 2, 1) */
	return 3;
      } else {
	/* (1, 2) - (0, 2) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else {
      /* (1) - (0) */
      vertexIdOfTriangle0_OUT = 1;
      vertexIdOfTriangle1_OUT = 0;
      return 1;
    }

  } else if (WH_eq (a1, b1)) {

    WH_ASSERT(WH_ne (a0, b0));
    WH_ASSERT(WH_ne (a0, b2));

    if (WH_eq (a2, b0)) {
      reverseFlag_OUT = true;
      if (WH_eq (a0, b2)) {
	/* (1, 2, 0) - (1, 0, 2) */
	return 3;
      } else {
	/* (1, 2) - (1, 0) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 0;
	return 2;
      }
    } else if (WH_eq (a2, b2)) {
      reverseFlag_OUT = false;
      if (WH_eq (a0, b0)) {
	/* (1, 2, 0) - (1, 2, 0) */
	return 3;
      } else {
	/* (1, 2) - (1, 2) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else {
      /* (1) - (1) */
      vertexIdOfTriangle0_OUT = 1;
      vertexIdOfTriangle1_OUT = 1;
      return 1;
    }

  } else if (WH_eq (a1, b2)) {

    WH_ASSERT(WH_ne (a0, b0));
    WH_ASSERT(WH_ne (a0, b1));

    if (WH_eq (a2, b0)) {
      reverseFlag_OUT = false;
      if (WH_eq (a0, b1)) {
	/* (1, 2, 0) - (2, 0, 1) */
	return 3;
      } else {
	/* (1, 2) - (2, 0) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 2;
	return 2;
      }
    } else if (WH_eq (a2, b1)) {
      reverseFlag_OUT = true;
      if (WH_eq (a0, b0)) {
	/* (1, 2, 0) - (2, 1, 0) */
	return 3;
      } else {
	/* (1, 2) - (2, 1) */
	edgeIdOfTriangle0_OUT = 1;
	edgeIdOfTriangle1_OUT = 1;
	return 2;
      }
    } else {
      /* (1) - (2) */
      vertexIdOfTriangle0_OUT = 1;
      vertexIdOfTriangle1_OUT = 2;
      return 1;
    }

  } else if (WH_eq (a2, b0)) {

    WH_ASSERT(WH_ne (a0, b1));
    WH_ASSERT(WH_ne (a0, b2));
    WH_ASSERT(WH_ne (a1, b1));
    WH_ASSERT(WH_ne (a1, b2));

    /* (2) - (0) */
    vertexIdOfTriangle0_OUT = 2;
    vertexIdOfTriangle1_OUT = 0;
    return 1;

  } else if (WH_eq (a2, b1)) {

    WH_ASSERT(WH_ne (a0, b1));
    WH_ASSERT(WH_ne (a0, b2));
    WH_ASSERT(WH_ne (a1, b1));
    WH_ASSERT(WH_ne (a1, b2));

    /* (2) - (1) */
    vertexIdOfTriangle0_OUT = 2;
    vertexIdOfTriangle1_OUT = 1;
    return 1;

  } else if (WH_eq (a2, b2)) {

    WH_ASSERT(WH_ne (a0, b1));
    WH_ASSERT(WH_ne (a0, b2));
    WH_ASSERT(WH_ne (a1, b1));
    WH_ASSERT(WH_ne (a1, b2));

    /* (2) - (2) */
    vertexIdOfTriangle0_OUT = 2;
    vertexIdOfTriangle1_OUT = 2;
    return 1;

  }

  return 0;
}

bool WH_Triangle3D_A
::hasConnectionAtVertexWith 
(const WH_Triangle3D_A& triangle) const
{
  WH_CVR_LINE;

  bool reverseFlag;
  int vertexId0, vertexId1;
  int edgeId0, edgeId1;
  return (WH_nMatchedVertexs 
	  (*this, triangle,
	   reverseFlag, vertexId0, vertexId1, edgeId0, edgeId1) == 1);
}

bool WH_Triangle3D_A
::hasConnectionAtEdgeWith 
(const WH_Triangle3D_A& triangle) const
{
  WH_CVR_LINE;

  bool reverseFlag;
  int vertexId0, vertexId1;
  int edgeId0, edgeId1;
  return (WH_nMatchedVertexs 
	  (*this, triangle,
	   reverseFlag, vertexId0, vertexId1, edgeId0, edgeId1) == 2);
}

bool WH_isSame 
(const WH_Triangle3D_A& tri0, const WH_Triangle3D_A& tri1)
{
  WH_CVR_LINE;

  bool reverseFlag;
  int vertexId0, vertexId1;
  int edgeId0, edgeId1;
  return (WH_nMatchedVertexs 
	  (tri0, tri1,
	   reverseFlag, vertexId0, vertexId1, edgeId0, edgeId1) == 3);
}

bool WH_Triangle3D_A
::hasIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point0_OUT, WH_Vector3D& point1_OUT) const
{
  WH_CVR_LINE;

  bool result = false;

  WH_Vector3D point;
  EdgeIntersectionData edgeData[3];
  WithLineIntersectionType intersectionFlag 
    = this->WH_Polygon3D_A::checkIntersectionWith 
    (line,
     point, edgeData);
  switch (intersectionFlag) {
  case NO_INTERSECTION_WITH_LINE:
    WH_CVR_LINE;
    /* nothing */
    break;
  case POINT_WITH_LINE:
    WH_CVR_LINE;
    /* intersection at a point (inside/boundary) with <line> which */
    /* is not parallel to the plane of the triangle */
    result = true;
    point0_OUT = point;
    point1_OUT = point;
    break;
  case PARALLEL_WITH_LINE:
    WH_CVR_LINE;
    /* nothing */
    break;
  case COINCIDENT_WITH_LINE:
    WH_CVR_LINE;
    {
      /* check intersection at edge */
      bool edgeIsFound = false;
      for (int iEdge = 0; iEdge < 3; iEdge++) {
	if (edgeData[iEdge].intersectionType 
	    == EdgeIntersectionData::IS_COINCIDENT) {
	  WH_CVR_LINE;
	  /* intersection at an edge of the triangle */
	  result = true;
	  edgeIsFound = true;
	  int vertexId0;
	  int vertexId1;
	  this->getVertexIndexsOfEdge (iEdge, 
				       vertexId0, vertexId1);
	  point0_OUT = this->vertex (vertexId0);
	  point1_OUT = this->vertex (vertexId1);
	  break;
	}
      }

      if (!edgeIsFound) {
	WH_CVR_LINE;
	/* check intersection at vertex or other conditions */

	bool point0IsFound = false;
	bool point1IsFound = false;
	for (int iEdge = 0; iEdge < 3; iEdge++) {
	  if (edgeData[iEdge].intersectionType 
	      == EdgeIntersectionData::HAS_INTERSECTION) {
	    WH_CVR_LINE;
	    WH_Vector3D point = edgeData[iEdge].intersectionPoint;
	    if (point0IsFound) {
	      WH_CVR_LINE;
	      if (WH_ne (point0_OUT, point)) {
		WH_CVR_LINE;
		point1_OUT = point;
		point1IsFound = true;
		break;
	      }
	    } else {
	      WH_CVR_LINE;
	      point0IsFound = true;
	      point0_OUT = point;
	    }
	  }
	}
	
	if (point1IsFound) {
	  WH_CVR_LINE;
	  WH_ASSERT(point0IsFound);
	  /* <line> goes across with the triangle */
	  result = true;
	} else if (point0IsFound) {
	  WH_CVR_LINE;
	  /* intersection at a vertex of the triangle */
	  result = true;
	  point1_OUT = point0_OUT;
	}
      }
    }
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(WH_ne (point0_OUT, point1_OUT)
	    || WH_eq (point0_OUT, point1_OUT));
    WH_ASSERT(line.contains (point0_OUT));
    WH_ASSERT(line.contains (point1_OUT));
    ContainmentType flag = this->checkContainmentAt (point0_OUT);
    WH_ASSERT(flag == IN || flag == ON);
    flag = this->checkContainmentAt (point1_OUT);
    WH_ASSERT(flag == IN || flag == ON);
  }
#endif

  return result;
}

static bool TriangleAndSegmentIntersectAtPoint 
(const WH_Triangle3D_A& triangle, const WH_Segment3D& segment,
 WH_Vector3D& point_OUT)
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(triangle.isRegular ());
#endif

  WH_CVR_LINE;

  bool result = false;

  WH_Segment3D::WithPlaneIntersectionType flag 
    = segment.checkIntersectionWith (triangle.plane (),
				     point_OUT);
  if (flag == WH_Segment3D::POINT_WITH_PLANE) {
    WH_CVR_LINE;
    WH_Polygon3D_A::ContainmentType containmentFlag 
      = triangle.checkContainmentAt (point_OUT);
    if (containmentFlag == WH_Polygon3D_A::IN
	|| containmentFlag == WH_Polygon3D_A::ON) {
      WH_CVR_LINE;
      result = true;
    }
  }

  return result;
}

WH_Triangle3D_A::WithTriangleIntersectionType WH_Triangle3D_A
::checkIntersectionWithTriangle 
(const WH_Triangle3D_A& triangle,
 bool& intersectsAtVertexOnly_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
  WH_ASSERT(triangle.isRegular ());
#endif

  WH_CVR_LINE;

  WithTriangleIntersectionType result = NO_INTERSECTION_WITH_TRIANGLE;
  intersectsAtVertexOnly_OUT = false;
  
  WH_Plane3D thisPlane = this->plane ();
  WH_Plane3D trianglePlane = triangle.plane ();
  
  WH_Line3D intersectionLine;
  WH_Plane3D::WithPlaneIntersectionType planePlaneFlag 
    = thisPlane.checkIntersectionWith (trianglePlane,
				       intersectionLine);
  
  switch (planePlaneFlag) {
  case WH_Plane3D::LINE_WITH_PLANE:
    WH_CVR_LINE;
    {
      bool edgeIntersectsWithTriangle = false;
      bool intersectionPointOnEdgeIsFound = false;

      /* intersection between <this> edge and <triangle> */
      for (int iEdge = 0; iEdge < 3; iEdge++) {
	WH_Segment3D thisEdge = this->edge (iEdge);
	WH_Vector3D point;
	if (TriangleAndSegmentIntersectAtPoint 
	    (triangle, thisEdge,
	     point)) {
	  edgeIntersectsWithTriangle = true;
	  if ((WH_ne (point, triangle.vertex (0))
	       && WH_ne (point, triangle.vertex (1))
	       && WH_ne (point, triangle.vertex (2)))
	      || (WH_ne (point, thisEdge.p0 ())
		  && WH_ne (point, thisEdge.p1 ()))) {
	    WH_CVR_LINE;
	    intersectionPointOnEdgeIsFound = true;
	  }
	}
      }

      /* intersection between edge of <triangle> and <this> */
      for (int iEdge = 0; iEdge < 3; iEdge++) {
	WH_Segment3D triangleEdge = triangle.edge (iEdge);
	WH_Vector3D point;
	if (TriangleAndSegmentIntersectAtPoint 
	    (*this, triangleEdge,
	     point)) {
	  WH_CVR_LINE;
	  edgeIntersectsWithTriangle = true;
	  if ((WH_ne (point, this->vertex (0))
	       && WH_ne (point, this->vertex (1))
	       && WH_ne (point, this->vertex (2)))
	      || (WH_ne (point, triangleEdge.p0 ())
		  && WH_ne (point, triangleEdge.p1 ()))) {
	    WH_CVR_LINE;
	    intersectionPointOnEdgeIsFound = true;
	  }
	}
      }
      
      if (edgeIntersectsWithTriangle) {
	WH_CVR_LINE;
	result = INTERSECTION_WITH_TRIANGLE;
	if (intersectionPointOnEdgeIsFound) {
	  WH_CVR_LINE;
	  intersectsAtVertexOnly_OUT = false;
	} else {
	  WH_CVR_LINE;
	  intersectsAtVertexOnly_OUT = true;
	}
      } else {
	WH_CVR_LINE;
	result = NO_INTERSECTION_WITH_TRIANGLE;
	intersectsAtVertexOnly_OUT = false;
      }
    }
    break;
  case WH_Plane3D::PARALLEL_WITH_PLANE:
    WH_CVR_LINE;
    result = PARALLEL_WITH_TRIANGLE;
    intersectsAtVertexOnly_OUT = false;
    break;
  case WH_Plane3D::COINCIDENT_WITH_PLANE:
    WH_CVR_LINE;
    {
      /* intersection between <this> edge and edge of <triangle> */
      bool edgeIntersectsWithEdge = false;
      bool intersectionPointOnEdgeIsFound = false;
      for (int iEdge = 0; iEdge < 3; iEdge++) {
	WH_Segment3D thisEdge = this->edge (iEdge);
	for (int jEdge = 0; jEdge < 3; jEdge++) {
	  WH_Segment3D triangleEdge = triangle.edge (jEdge);

	  if (thisEdge.hasConnectionWith (triangleEdge)) {
	    WH_CVR_LINE;
	    edgeIntersectsWithEdge = true;	    
	  } else {
	    WH_CVR_LINE;
	    WH_Vector3D point;
	    if (thisEdge.checkIntersectionWith (triangleEdge, 
						point) 
		== WH_Segment3D::POINT_WITH_SEGMENT) {
	      WH_CVR_LINE;
	      edgeIntersectsWithEdge = true;	    
	      intersectionPointOnEdgeIsFound = true;
	    }
	  }
	}
      }

      /* check containment of vertex each other */
      int thisContainmentCount = 0;
      int triangleContainmentCount = 0;
      for (int iVertex = 0; iVertex < 3; iVertex++) {
	ContainmentType containmentFlag 
	  = this->checkContainmentAt (triangle.vertex (iVertex));
	if (containmentFlag == IN
	    || containmentFlag == ON) {
	  WH_CVR_LINE;
	  thisContainmentCount++;
	}
	containmentFlag 
	  = triangle.checkContainmentAt (this->vertex (iVertex));
	if (containmentFlag == IN
	    || containmentFlag == ON) {
	  WH_CVR_LINE;
	  triangleContainmentCount++;
	}
      }

      if (edgeIntersectsWithEdge) {
	WH_CVR_LINE;
	if (thisContainmentCount == 3
	    && triangleContainmentCount == 3) {
	  WH_CVR_LINE;
	  /* <this> is same as <triangle> */
	  WH_ASSERT(WH_isSame (*this, triangle));
	  result = OVERLAP_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = true;
	} else if (thisContainmentCount == 3) {
	  WH_CVR_LINE;
	  /* <this> contains <triangle> */
	  result = OVERLAP_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = false;
	} else if (triangleContainmentCount == 3) {
	  WH_CVR_LINE;
	  /* <triangle> just contains <this> */
	  result = OVERLAP_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = false;
	} else {
	  WH_CVR_LINE;
	  result = OVERLAP_WITH_TRIANGLE;
	  if (intersectionPointOnEdgeIsFound) {
	    WH_CVR_LINE;
	    intersectsAtVertexOnly_OUT = false;
	  } else {
	    WH_CVR_LINE;
	    intersectsAtVertexOnly_OUT = true;
	  }
	}
      } else {
	WH_CVR_LINE;
	if (thisContainmentCount == 3
	    && triangleContainmentCount == 3) {
	  WH_ASSERT_NO_REACH;
	} else if (thisContainmentCount == 3) {
	  WH_CVR_LINE;
	  /* <this> just contains <triangle> */
	  WH_ASSERT(triangleContainmentCount == 0);
	  result = OVERLAP_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = false;
	} else if (triangleContainmentCount == 3) {
	  WH_CVR_LINE;
	  /* <triangle> just contains <this> */
	  WH_ASSERT(thisContainmentCount == 0);
	  result = OVERLAP_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = false;
	} else {
	  WH_CVR_LINE;
	  result = NO_INTERSECTION_WITH_TRIANGLE;
	  intersectsAtVertexOnly_OUT = false;
	}
      }
    }
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }

    /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_INTERSECTION_WITH_TRIANGLE:
    break;
  case PARALLEL_WITH_TRIANGLE:
    break;
  case INTERSECTION_WITH_TRIANGLE:
    break;
  case OVERLAP_WITH_TRIANGLE:
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}



/* class WH_Triangle3D */

WH_Triangle3D
::WH_Triangle3D ()
{
  WH_CVR_LINE;

  _vertexs[0] = WH_Vector3D (0, 0, 0);
  _vertexs[1] = WH_Vector3D (1, 0, 0);
  _vertexs[2] = WH_Vector3D (0, 1, 0);

  _planeExists = true;
  _plane = this->WH_Triangle3D_A::plane ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Triangle3D
::WH_Triangle3D 
(const WH_Vector3D& v0, 
 const WH_Vector3D& v1,
 const WH_Vector3D& v2)
{
  WH_CVR_LINE;

  _vertexs[0] = v0;
  _vertexs[1] = v1;
  _vertexs[2] = v2;

  if (this->WH_Triangle3D_A::hasAnyVertexOffPlane ()) {
    WH_CVR_LINE;
    _planeExists = false;
  } else {
    WH_CVR_LINE;
    _planeExists = true;
    _plane = this->WH_Triangle3D_A::plane ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Triangle3D
::~WH_Triangle3D ()
{
  WH_CVR_LINE;
}

bool WH_Triangle3D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle3D_A::checkInvariant ();

  return true;
}

bool WH_Triangle3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle3D_A::assureInvariant ();

  return true;
}

ostream& operator<< (ostream& out, const WH_Triangle3D& triangle)
{
  WH_CVR_LINE;

  out << "WH_Triangle3D ";
  out << triangle.nVertexs () << " ";
  for (int iVertex = 0; 
       iVertex < triangle.nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = triangle.vertex (iVertex);
    out << vertex;
  }
  return out;
}

WH_Vector3D WH_Triangle3D
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  return _vertexs[iVertex];
}

bool WH_Triangle3D
::hasAnyVertexOffPlane () const
{
  return !_planeExists;
}

WH_Plane3D WH_Triangle3D
::plane () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(!this->hasAnyVertexOffPlane ());
#endif

  return _plane;
}



/* not yet covered 
int nMatchedVertexs ();
*/
