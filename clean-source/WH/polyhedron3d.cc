/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* polyhedron3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "polyhedron3d.h"



/* class WH_Polyhedron3D_A */

WH_Polyhedron3D_A
::WH_Polyhedron3D_A ()
{
  WH_CVR_LINE;
}

WH_Polyhedron3D_A
::~WH_Polyhedron3D_A ()
{
  WH_CVR_LINE;
}

bool WH_Polyhedron3D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_Polyhedron3D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(3 < this->nVertexs ());
  WH_ASSERT(5 < this->nEdges ());
  WH_ASSERT(3 < this->nFaces ());

  for (int iEdge = 0;
       iEdge < this->nEdges ();
       iEdge++) {

    int iVertex0;
    int iVertex1;
    this->getVertexIndexsOfEdge (iEdge,
				 iVertex0, iVertex1);
    WH_ASSERT(0 <= iVertex0);
    WH_ASSERT(iVertex0 < this->nVertexs ());
    WH_ASSERT(0 <= iVertex1);
    WH_ASSERT(iVertex1 < this->nVertexs ());
    
    WH_Vector3D vertex0 = this->vertex (iVertex0);
    WH_Vector3D vertex1 = this->vertex (iVertex1);
    WH_ASSERT(WH_ne (vertex0, vertex1));
  }

  for (int iFace = 0;
       iFace < this->nFaces ();
       iFace++) {

    WH_ASSERT(2 < this->nVertexsOfFace (iFace));
    for (int iVertexOfFace = 0;
	 iVertexOfFace < this->nVertexsOfFace (iFace);
	 iVertexOfFace++) {

      int iVertex 
	= this->vertexIndexOfFace (iFace, iVertexOfFace);
      WH_ASSERT(0 <= iVertex);
      WH_ASSERT(iVertex < this->nVertexs ());
      WH_Vector3D vertex = this->vertex (iVertex);

      int iNextVertexOfFace 
	= this->nextVertexIndexOfFace (iFace, iVertexOfFace);
      int iNextVertex 
	= this->vertexIndexOfFace (iFace, iNextVertexOfFace);
      WH_ASSERT(0 <= iNextVertex);
      WH_ASSERT(iNextVertex < this->nVertexs ());
      WH_Vector3D nextVertex = this->vertex (iNextVertex);
      WH_ASSERT(WH_ne (vertex, nextVertex));
      
      int iPrevVertexOfFace 
	= this->prevVertexIndexOfFace (iFace, iVertexOfFace);
      int iPrevVertex 
	= this->vertexIndexOfFace (iFace, iPrevVertexOfFace);
      WH_ASSERT(0 <= iPrevVertex);
      WH_ASSERT(iPrevVertex < this->nVertexs ());
      WH_Vector3D prevVertex = this->vertex (iPrevVertex);
      WH_ASSERT(WH_ne (vertex, prevVertex));
    }

    WH_ASSERT(2 < this->nEdgesOfFace (iFace));
    for (int iEdgeOfFace = 0;
	 iEdgeOfFace < this->nEdgesOfFace (iFace);
	 iEdgeOfFace++) {
      int iEdge 
	= this->edgeIndexOfFace (iFace, iEdgeOfFace);
      WH_ASSERT(0 <= iEdge);
      WH_ASSERT(iEdge < this->nEdges ());

      int iNextEdgeOfFace 
	= this->nextEdgeIndexOfFace (iFace, iEdgeOfFace);
      int iNextEdge 
	= this->edgeIndexOfFace (iFace, iNextEdgeOfFace);
      WH_ASSERT(0 <= iNextEdge);
      WH_ASSERT(iNextEdge < this->nEdges ());
      
      int iPrevEdgeOfFace 
	= this->prevEdgeIndexOfFace (iFace, iEdgeOfFace);
      int iPrevEdge 
	= this->edgeIndexOfFace (iFace, iPrevEdgeOfFace);
      WH_ASSERT(0 <= iPrevEdge);
      WH_ASSERT(iPrevEdge < this->nEdges ());
    }
  }
  
  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
  WH_ASSERT(WH_le (minRange, maxRange));

  if (this->isRegular ()) {
    WH_CVR_LINE;
    
    WH_ASSERT(WH_lt (minRange, maxRange));

    WH_Vector3D centerOfGravity = this->centerOfGravity ();
    WH_ASSERT(WH_between (centerOfGravity, minRange, maxRange));
    
    WH_ASSERT(WH_lt2 (0, this->surfaceArea ()));
    WH_ASSERT(WH_lt3 (0, this->volume ()));
    
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

    for (int iFace = 0; 
	 iFace < this->nFaces (); 
	 iFace++) {
      WH_Polygon3D face = this->face (iFace);
      ContainmentType flag = this
	->checkContainmentAt (face.centerOfGravity ());
      WH_ASSERT(flag == ON);
    }
  }

  return true;
}

WH_Vector3D WH_Polyhedron3D_A
::vertex (int iVertex) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iVertex);
  WH_ASSERT(iVertex < this->nVertexs ());

  /* nothing : PURE VIRTUAL */
  WH_Vector3D result (0, 0, 0);
  
  /* POST-CONDITION */
  
  return result;
}

void WH_Polyhedron3D_A
::getVertexIndexsOfEdge 
(int iEdge,
 int& iVertex0_OUT, int& iVertex1_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iEdge);
  WH_ASSERT(iEdge < this->nEdges ());

  /* nothing : PURE VIRTUAL */
  iVertex0_OUT = WH_NO_INDEX;
  iVertex1_OUT = WH_NO_INDEX;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= iVertex0_OUT);
  WH_ASSERT(iVertex0_OUT < this->nVertexs ());
  WH_ASSERT(0 <= iVertex1_OUT);
  WH_ASSERT(iVertex1_OUT < this->nVertexs ());
#endif
}

int WH_Polyhedron3D_A
::nVertexsOfFace (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  /* nothing : PURE VIRTUAL */
  int result = 0;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(2 < result);
#endif

  return result;
}

int WH_Polyhedron3D_A
::vertexIndexOfFace 
(int iFace, int iVertexOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iVertexOfFace);
  WH_ASSERT(iVertexOfFace < this->nVertexsOfFace (iFace));

  /* nothing : PURE VIRTUAL */
  int result = WH_NO_INDEX;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexs ());
#endif

  return result;
}

int WH_Polyhedron3D_A
::edgeIndexOfFace 
(int iFace, int iEdgeOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iEdgeOfFace);
  WH_ASSERT(iEdgeOfFace < this->nEdgesOfFace (iFace));

  /* nothing : PURE VIRTUAL */
  int result = WH_NO_INDEX;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdges ());
#endif

  return result;
}

WH_Segment3D WH_Polyhedron3D_A
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

int WH_Polyhedron3D_A
::nEdgesOfFace (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  return this->nVertexsOfFace (iFace);
}

int WH_Polyhedron3D_A
::nextVertexIndexOfFace 
(int iFace, int iVertexOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iVertexOfFace);
  WH_ASSERT(iVertexOfFace < this->nVertexsOfFace (iFace));

  WH_CVR_LINE;

  int result = iVertexOfFace + 1;
  if (result == this->nVertexsOfFace (iFace)) {
    WH_CVR_LINE;
    result = 0;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexsOfFace (iFace));
#endif

  return result;
}

int WH_Polyhedron3D_A
::prevVertexIndexOfFace 
(int iFace, int iVertexOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iVertexOfFace);
  WH_ASSERT(iVertexOfFace < this->nVertexsOfFace (iFace));

  WH_CVR_LINE;

  int result = iVertexOfFace - 1;
  if (result == -1) {
    WH_CVR_LINE;
    result = this->nVertexsOfFace (iFace) - 1;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nVertexsOfFace (iFace));
#endif

  return result;
}

int WH_Polyhedron3D_A
::nextEdgeIndexOfFace 
(int iFace, int iEdgeOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iEdgeOfFace);
  WH_ASSERT(iEdgeOfFace < this->nEdgesOfFace (iFace));

  WH_CVR_LINE;

  int result = iEdgeOfFace + 1;
  if (result == this->nEdgesOfFace (iFace)) {
    WH_CVR_LINE;
    result = 0;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdgesOfFace (iFace));
#endif

  return result;
}

int WH_Polyhedron3D_A
::prevEdgeIndexOfFace 
(int iFace, int iEdgeOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iEdgeOfFace);
  WH_ASSERT(iEdgeOfFace < this->nEdgesOfFace (iFace));

  WH_CVR_LINE;

  int result = iEdgeOfFace - 1;
  if (result == -1) {
    WH_CVR_LINE;
    result = this->nEdgesOfFace (iFace) - 1;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < this->nEdgesOfFace (iFace));
#endif

  return result;
}

WH_Vector3D WH_Polyhedron3D_A
::vertexOfFace (int iFace, int iVertexOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iVertexOfFace);
  WH_ASSERT(iVertexOfFace < this->nVertexsOfFace (iFace));

  return this->vertex 
    (this->vertexIndexOfFace (iFace, iVertexOfFace));
}

WH_Segment3D WH_Polyhedron3D_A
::edgeOfFace (int iFace, int iEdgeOfFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());
  WH_ASSERT(0 <= iEdgeOfFace);
  WH_ASSERT(iEdgeOfFace < this->nEdgesOfFace (iFace));

  return this->edge 
    (this->edgeIndexOfFace (iFace, iEdgeOfFace));
}

WH_Polygon3D WH_Polyhedron3D_A
::face (int iFace) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= iFace);
  WH_ASSERT(iFace < this->nFaces ());

  WH_CVR_LINE;                                     /* NOT COVERED */

  WH_Vector3D* vertexs 
    = new WH_Vector3D[this->nVertexsOfFace (iFace)];
  WH_ASSERT(vertexs != WH_NULL);

  for (int iVertexOfFace = 0; 
       iVertexOfFace < this->nVertexsOfFace (iFace); 
       iVertexOfFace++) {
    vertexs[iVertexOfFace] 
      = this->vertexOfFace (iFace, iVertexOfFace);
  }
  WH_Polygon3D result (this->nVertexsOfFace (iFace), vertexs);

  delete[] vertexs;
  vertexs = WH_NULL;

  return result;
}

bool WH_Polyhedron3D_A
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
	WH_CVR_LINE;                               /* NOT COVERED */
	result = true;
	break;
      }
    }
  }

  return result;
}

bool WH_Polyhedron3D_A
::hasAnyCrossingEdge () const
{
  WH_CVR_LINE;

  bool result = false;

  for (int iEdge = 0; 
       iEdge < this->nEdges (); 
       iEdge++) {
    WH_Segment3D edge_i = this->edge (iEdge);
    
    for (int jEdge = iEdge + 1; 
	 jEdge < this->nEdges (); 
	 jEdge++) {
      WH_Segment3D edge_j = this->edge (jEdge);

      if (edge_i.hasConnectionWith (edge_j)) continue;

      WH_Vector3D point;
      WH_Segment3D::WithSegmentIntersectionType intersectionFlag 
	= edge_i.checkIntersectionWith (edge_j,
					point);
      if (intersectionFlag == WH_Segment3D::POINT_WITH_SEGMENT) {
	WH_CVR_LINE;
	result = true;
	break;
      } else if (intersectionFlag == WH_Segment3D::COINCIDENT_WITH_SEGMENT) {
	WH_CVR_LINE;                                /* NOT COVERED */
	WH_Segment3D::WithSegmentOverlapType overlapFlag 
	  = edge_i.checkOverlapWith (edge_j);
	if (overlapFlag != WH_Segment3D::NO_OVERLAP_WITH_SEGMENT) {
	  WH_CVR_LINE;                              /* NOT COVERED */
	  result = true;
	  break;
	}
      }
    }
  }

  return result;
}

bool WH_Polyhedron3D_A
::hasAnyIllegularFace () const
{
  WH_CVR_LINE;

  bool result = false;
  
  for (int iFace = 0; 
       iFace < this->nFaces (); 
       iFace++) {
    WH_Polygon3D face = this->face (iFace);
    if (!face.isRegular ()) {
      WH_CVR_LINE;                                /* NOT COVERED */
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polyhedron3D_A
::isRegular () const
{
  return !this->hasAnyDuplicatedVertex ()
    && !this->hasAnyCrossingEdge ()
    && !this->hasAnyIllegularFace ();
}

double WH_Polyhedron3D_A
::surfaceArea () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;                                     /* NOT COVERED */

  double result = 0.0;

  for (int iFace = 0; 
       iFace < this->nFaces (); 
       iFace++) {
    WH_Polygon3D face = this->face (iFace);
    result += face.area ();
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt2 (0, result));
#endif
  
  return result;
}

double WH_Polyhedron3D_A
::volume () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;                                     /* NOT COVERED */

  double result = 0.0;

  /* NEED TO REDEFINE */
  WH_ASSERT_NO_REACH;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_lt3 (0, result));
#endif
  
  return result;
}

WH_Vector3D WH_Polyhedron3D_A
::centerOfGravity () const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;                                     /* NOT COVERED */

  WH_Vector3D result (0, 0, 0);

  /* NEED TO REDEFINE */
  WH_ASSERT_NO_REACH;

  return result;
}

WH_Vector3D WH_Polyhedron3D_A
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

WH_Vector3D WH_Polyhedron3D_A
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

bool WH_Polyhedron3D_A
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

bool WH_Polyhedron3D_A
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

bool WH_Polyhedron3D_A
::containsOnBoundary (const WH_Vector3D& position) const
{
  WH_CVR_LINE;                                      /* NOT COVERED */

  bool result = false;

  for (int iFace = 0; 
       iFace < this->nFaces (); 
       iFace++) {
    WH_Polygon3D face = this->face (iFace);
    WH_Polygon3D::ContainmentType flag 
      = face.checkContainmentAt (position);
    if (flag == WH_Polygon3D::IN
	|| flag == WH_Polygon3D::ON) {
      WH_CVR_LINE;                                 /* NOT COVERED */
      result = true;
      break;
    }
  }

  return result;
}

bool WH_Polyhedron3D_A
::containsInside (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;                                    /* NOT COVERED */

  bool result = false;

  /* NEED TO REDEFINE */
  WH_ASSERT_NO_REACH;

  return result;
}

WH_Polyhedron3D_A::ContainmentType WH_Polyhedron3D_A
::checkContainmentAt (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isRegular ());
#endif
  
  WH_CVR_LINE;                                   /* NOT COVERED */

  ContainmentType result = OUT;
  
  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
  if (WH_between (position, minRange, maxRange)) {
    if (this->containsOnBoundary (position)) {
      WH_CVR_LINE;                              /* NOT COVERED */
      result = ON;
    } else if (this->containsInside (position)) {
      WH_CVR_LINE;                              /* NOT COVERED */
      result = IN;
    }
  }
  
  return result;
}



/* not covered yet
polyhedron3d.cc:474:	WH_CVR_LINE;
polyhedron3d.cc:512:	WH_CVR_LINE;
polyhedron3d.cc:516:	  WH_CVR_LINE;
polyhedron3d.cc:539:      WH_CVR_LINE;
face (int iFace) const
surfaceArea () const
volume () const
centerOfGravity () const
containsOnBoundary (const WH_Vector3D& position) const
containsInside (const WH_Vector3D& position) const
checkContainmentAt (const WH_Vector3D& position) const
*/
