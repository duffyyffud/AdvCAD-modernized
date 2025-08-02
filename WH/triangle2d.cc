/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* triangle2d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "triangle2d.h"



/* class WH_Triangle2D_A */

const int WH_Triangle2D_A::nTriangleVertexs = 3;
const int WH_Triangle2D_A::nTriangleEdges = 3;
const int WH_Triangle2D_A::edgeVertexMap[3][2] = {
  { 1, 2 }, 
  { 2, 0 }, 
  { 0, 1 }
};

WH_Triangle2D_A
::WH_Triangle2D_A ()
{
  WH_CVR_LINE;
}

WH_Triangle2D_A
::~WH_Triangle2D_A ()
{
  WH_CVR_LINE;
}

bool WH_Triangle2D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon2D_A::checkInvariant ();

  return true;
}

bool WH_Triangle2D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polygon2D_A::assureInvariant ();

  WH_ASSERT(this->nVertexs () == 3);
  WH_ASSERT(this->nEdges () == 3);

  WH_ASSERT(this->isConvex ());

  return true;
}

double WH_Triangle2D_A
::signedArea () const
{
  WH_CVR_LINE;

  WH_Vector2D v0 = this->vertex (0);
  WH_Vector2D v1 = this->vertex (1);
  WH_Vector2D v2 = this->vertex (2);
  return WH_Segment2D (v0, v1).signedTriangleAreaWith (v2);
}

bool WH_Triangle2D_A
::isClockWise () const
{
  return WH_lt2 (this->signedArea (), 0);
}

void WH_Triangle2D_A
::getCircumcenter 
(WH_Vector2D& center_OUT, double& radius_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WH_Vector2D k = this->vertex (0);
  WH_Vector2D l = this->vertex (1);
  WH_Vector2D m = this->vertex (2);
  double xlk = l.x - k.x;
  double ylk = l.y - k.y;
  double xmk = m.x - k.x;
  double ymk = m.y - k.y;
  double det = xlk * ymk - xmk * ylk;
  if (WH_eq2 (det, 0)) {
    cerr << "WARNING: Degenerate triangle with zero determinant - using fallback circumcenter" << endl;
    // For degenerate triangle, use midpoint as circumcenter with large radius
    center_OUT = (k + l + m) / 3.0;  // Centroid as fallback
    radius_OUT = WH_distance(k, l) + WH_distance(l, m) + WH_distance(m, k);  // Large radius
    return;
  }
  double rlk = xlk * xlk + ylk * ylk;
  double rmk = xmk * xmk + ymk * ymk;
  WH_Vector2D offset 
    = WH_Vector2D (rlk * ymk - rmk * ylk,
		   xlk * rmk - xmk * rlk) / (det * 2);
  center_OUT = offset + k;
  radius_OUT = offset.length ();
}

void WH_Triangle2D_A
::getInternalCenter 
(WH_Vector2D& center_OUT, double& radius_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WH_Vector2D v0 = this->vertex (0);
  WH_Vector2D v1 = this->vertex (1);
  WH_Vector2D v2 = this->vertex (2);
  double r0 = this->edge (0).length ();
  double r1 = this->edge (1).length ();
  double r2 = this->edge (2).length ();
  double t = r0 + r1 + r2;
  double s = t / 2;
  center_OUT = (v0 * r1 + v1 * r2 + v2 * r0) / t;
  double sr = (s - r0) * (s - r1) * (s - r2) / s;
  WH_ASSERT(WH_lt2 (0, sr));
  radius_OUT = sqrt (sr);
}

int WH_Triangle2D_A
::nVertexs () const
{
  return 3;
}

int WH_Triangle2D_A
::nEdges () const
{
  return 3;
}

WH_Segment2D WH_Triangle2D_A
::edge (int iEdge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

  WH_CVR_LINE;

  if (iEdge == 2) {
    WH_CVR_LINE;
    return WH_Segment2D (this->vertex (2), 
			 this->vertex (0));
    
  } else {
    WH_CVR_LINE;
    return WH_Segment2D (this->vertex (iEdge), 
			 this->vertex (iEdge + 1));
  }
}

bool WH_Triangle2D_A
::isConvex () const
{
  return true;
}

double WH_Triangle2D_A
::area () const
{
  return fabs (this->signedArea ());
}

WH_Vector2D WH_Triangle2D_A
::centerOfGravity () const
{
  WH_CVR_LINE;

  WH_Vector2D v0 = this->vertex (0);
  WH_Vector2D v1 = this->vertex (1);
  WH_Vector2D v2 = this->vertex (2);
  return (v0 + v1 + v2) / 3;
}

WH_Polygon2D_A::ContainmentType WH_Triangle2D_A
::checkContainmentAt (const WH_Vector2D& position) const
{
  WH_CVR_LINE;

  WH_Polygon2D_A::ContainmentType result = OUT;

  WH_Vector2D minRange = this->minRange ();
  WH_Vector2D maxRange = this->maxRange ();
  if (WH_between (position, minRange, maxRange)) {
    WH_CVR_LINE;
    WH_Segment2D e0 = this->edge (0);
    WH_Segment2D e1 = this->edge (1);
    WH_Segment2D e2 = this->edge (2);
    if (e0.contains (position)
	|| e1.contains (position)
	|| e2.contains (position)) {
      WH_CVR_LINE;
      result = ON;
    } else {
      double sign0 = e0.signedTriangleAreaWith (position);
      double sign1 = e1.signedTriangleAreaWith (position);
      double sign2 = e2.signedTriangleAreaWith (position);
      if ((WH_lt2 (0, sign0) && WH_lt2 (0, sign1) && WH_lt2 (0, sign2))
	  || (WH_lt2 (sign0, 0) && WH_lt2 (sign1, 0) && WH_lt2 (sign2, 0))) {
	WH_CVR_LINE;
	result = IN;
      }
    }
  }

  return result;
}



/* class WH_Triangle2D */

WH_Triangle2D
::WH_Triangle2D ()
{
  WH_CVR_LINE;

  _vertexs[0] = WH_Vector2D (0, 0);
  _vertexs[1] = WH_Vector2D (1, 0);
  _vertexs[2] = WH_Vector2D (0, 1);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Triangle2D
::WH_Triangle2D 
(const WH_Vector2D& v0, 
 const WH_Vector2D& v1,
 const WH_Vector2D& v2)
{
  WH_CVR_LINE;

  _vertexs[0] = v0;
  _vertexs[1] = v1;
  _vertexs[2] = v2;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Triangle2D
::~WH_Triangle2D ()
{
  WH_CVR_LINE;
}

bool WH_Triangle2D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle2D_A::checkInvariant ();

  return true;
}

bool WH_Triangle2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle2D_A::assureInvariant ();

  return true;
}

ostream& operator<< (ostream& out, const WH_Triangle2D& triangle)
{
  WH_CVR_LINE;

  out << "WH_Triangle2D ";
  out << triangle.nVertexs () << " ";
  for (int iVertex = 0; 
       iVertex < triangle.nVertexs (); 
       iVertex++) {
    WH_Vector2D vertex = triangle.vertex (iVertex);
    out << vertex;
  }
  return out;
}

WH_Vector2D WH_Triangle2D
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  return _vertexs[iVertex];
}



/* test coverage completed */
