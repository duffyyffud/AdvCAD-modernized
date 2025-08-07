/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* tetrahedron3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "tetrahedron3d.h"



/* class WH_Tetrahedron3D_A */

const int WH_Tetrahedron3D_A::nTetrahedronVertexs = 4;
const int WH_Tetrahedron3D_A::nTetrahedronEdges = 6;
const int WH_Tetrahedron3D_A::nTetrahedronFaces = 4;

const int WH_Tetrahedron3D_A::edgeVertexMap[6][2] = {
  { 0, 1 }, 
  { 1, 2 }, 
  { 2, 0 }, 
  { 0, 3 }, 
  { 1, 3 }, 
  { 2, 3 }
};
const int WH_Tetrahedron3D_A::faceVertexMap[4][3] = {
  { 3, 2, 1 }, 
  { 3, 0, 2 }, 
  { 3, 1, 0 }, 
  { 0, 1, 2 }
};
const int WH_Tetrahedron3D_A::faceEdgeMap[4][3] = {
  { 5, 1, 4 }, 
  { 3, 2, 5 },
  { 4, 0, 3 }, 
  { 0, 1, 2 }
};

WH_Tetrahedron3D_A
::WH_Tetrahedron3D_A ()
{
  WH_CVR_LINE;
}

WH_Tetrahedron3D_A
::~WH_Tetrahedron3D_A ()
{
  WH_CVR_LINE;
}

bool WH_Tetrahedron3D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polyhedron3D_A::checkInvariant ();

  return true;
}

bool WH_Tetrahedron3D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Polyhedron3D_A::assureInvariant ();

  WH_ASSERT(this->nVertexs () == 4);
  WH_ASSERT(this->nEdges () == 6);
  WH_ASSERT(this->nFaces () == 4);

  if (this->isRegular ()) {
    WH_CVR_LINE;
    WH_ASSERT(this->checkContainmentAt (this->centerOfGravity ())
	      == IN);
  }

  return true;
}

int WH_Tetrahedron3D_A
::nVertexs () const
{
  return 4;
}

int WH_Tetrahedron3D_A
::nEdges () const
{
  return 6;
}

void WH_Tetrahedron3D_A
::getVertexIndexsOfEdge 
(int iEdge,
 int& iVertex0_OUT, int& iVertex1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());
  
  iVertex0_OUT = edgeVertexMap[iEdge][0];
  iVertex1_OUT = edgeVertexMap[iEdge][1];

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= iVertex0_OUT);
  WH_ASSERT(iVertex0_OUT < this->nVertexs ());
  WH_ASSERT(0 <= iVertex1_OUT);
  WH_ASSERT(iVertex1_OUT < this->nVertexs ());
#endif
}

int WH_Tetrahedron3D_A
::nFaces () const
{
  return 4;
}

int WH_Tetrahedron3D_A
::nVertexsOfFace (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  return 3;
}

int WH_Tetrahedron3D_A
::nEdgesOfFace (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  return 3;
}

int WH_Tetrahedron3D_A
::vertexIndexOfFace 
(int iFace, int iVertexOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iVertexOfFace);
  WH_ASSERT(iVertexOfFace < this->nVertexsOfFace (iFace));

  int result = faceVertexMap[iFace][iVertexOfFace];

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexs ());
#endif

  return result;
}

int WH_Tetrahedron3D_A
::edgeIndexOfFace 
(int iFace, int iEdgeOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iEdgeOfFace);
  WH_ASSERT(iEdgeOfFace < this->nEdgesOfFace (iFace));

  int result = faceEdgeMap[iFace][iEdgeOfFace];
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdges ());
#endif

  return result;
}

WH_Polygon3D WH_Tetrahedron3D_A
::face (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  WH_CVR_LINE;

  WH_Vector3D vertexs[3];
  vertexs[0] = this->vertexOfFace (iFace, 0);
  vertexs[1] = this->vertexOfFace (iFace, 1);
  vertexs[2] = this->vertexOfFace (iFace, 2);
  return WH_Polygon3D (3, vertexs);
}

WH_Triangle3D WH_Tetrahedron3D_A
::faceTriangle (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  return WH_Triangle3D 
    (this->vertexOfFace (iFace, 0),
     this->vertexOfFace (iFace, 1),
     this->vertexOfFace (iFace, 2));
}

double WH_Tetrahedron3D_A
::surfaceArea () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;

  double result = 0.0;

  for (int iFace = 0; 
       iFace < this->nFaces (); 
       iFace++) {
    result += this->faceTriangle (iFace).area ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt2 (0, result));
#endif
  
  return result;
}

double WH_Tetrahedron3D_A
::signedVolume () const
{
  WH_CVR_LINE;

  WH_Vector3D v0 = this->vertex (0);
  WH_Vector3D v1 = this->vertex (1);
  WH_Vector3D v2 = this->vertex (2);
  WH_Vector3D v3 = this->vertex (3);
  
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

double WH_Tetrahedron3D_A
::volume () const
{
  return fabs (this->signedVolume ());
}

bool WH_Tetrahedron3D_A
::isClockWise () const
{
  return WH_lt3 (this->signedVolume (), 0);
}

WH_Vector3D WH_Tetrahedron3D_A
::centerOfGravity () const
{
  WH_CVR_LINE;

  WH_Vector3D v0 = this->vertex (0);
  WH_Vector3D v1 = this->vertex (1);
  WH_Vector3D v2 = this->vertex (2);
  WH_Vector3D v3 = this->vertex (3);
  return (v0 + v1 + v2 + v3) / 4;
}

WH_Polyhedron3D_A::ContainmentType WH_Tetrahedron3D_A
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
    int plusCount = 0;
    int minusCount = 0;
    int zeroCount = 0;
    for (int iFace = 0; 
	 iFace < this->nFaces (); 
	 iFace++) {
      WH_Plane3D plane (this->vertexOfFace (iFace, 0),
			this->vertexOfFace (iFace, 1),
			this->vertexOfFace (iFace, 2));
      double value = plane.valueAt (position);
      if (WH_lt (0, value)) {
	WH_CVR_LINE;
	plusCount++;
      } else if (WH_lt (value, 0)) {
	WH_CVR_LINE;
	minusCount++;
      } else {
	WH_CVR_LINE;
	zeroCount++;
      }
    }
    if (plusCount == 4 || minusCount == 4) {
      WH_CVR_LINE;
      result = IN;
    } else if (plusCount == 0 || minusCount == 0) {
      WH_CVR_LINE;
      result = ON;
    }
  }
  
  return result;
}

void WH_Tetrahedron3D_A
::getCircumcenter 
(WH_Vector3D& center_OUT, double& radius_OUT) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif

  WH_CVR_LINE;

  WH_Vector3D a = this->vertex (0);
  WH_Vector3D b = this->vertex (1);
  WH_Vector3D c = this->vertex (2);
  WH_Vector3D d = this->vertex (3);
  
  center_OUT = WH_circumcenterAmong (a, b, c, d);
  radius_OUT = WH_distance (center_OUT, this->vertex (0));

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_eq (WH_distance (center_OUT, this->vertex (0)), 
		   radius_OUT));
  WH_ASSERT(WH_eq (WH_distance (center_OUT, this->vertex (1)), 
		   radius_OUT));
  WH_ASSERT(WH_eq (WH_distance (center_OUT, this->vertex (2)), 
		   radius_OUT));
  WH_ASSERT(WH_eq (WH_distance (center_OUT, this->vertex (3)), 
		   radius_OUT));
#endif  
}



/* class WH_Tetrahedron3D */

WH_Tetrahedron3D
::WH_Tetrahedron3D ()
{
  WH_CVR_LINE;

  _vertexs[0] = WH_Vector3D (0, 0, 0);
  _vertexs[1] = WH_Vector3D (1, 0, 0);
  _vertexs[2] = WH_Vector3D (0, 1, 0);
  _vertexs[3] = WH_Vector3D (0, 0, 1);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Tetrahedron3D
::WH_Tetrahedron3D 
(const WH_Vector3D& v0, 
 const WH_Vector3D& v1,
 const WH_Vector3D& v2,
 const WH_Vector3D& v3)
{
  WH_CVR_LINE;

  _vertexs[0] = v0;
  _vertexs[1] = v1;
  _vertexs[2] = v2;
  _vertexs[3] = v3;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Tetrahedron3D
::~WH_Tetrahedron3D ()
{
  WH_CVR_LINE;
}

bool WH_Tetrahedron3D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Tetrahedron3D_A::checkInvariant ();

  return true;
}

bool WH_Tetrahedron3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Tetrahedron3D_A::assureInvariant ();

  return true;
}

ostream& operator<< 
(ostream& out, const WH_Tetrahedron3D& tetrahedron)
{
  WH_CVR_LINE;

  out << "WH_Tetrahedron3D ";
  out << tetrahedron.nVertexs () << " ";
  for (int iVertex = 0; 
       iVertex < tetrahedron.nVertexs (); 
       iVertex++) {
    WH_Vector3D vertex = tetrahedron.vertex (iVertex);
    out << vertex;
  }
  return out;
}

WH_Vector3D WH_Tetrahedron3D
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());
  
  return _vertexs[iVertex];
}



/* test coverage completed */


