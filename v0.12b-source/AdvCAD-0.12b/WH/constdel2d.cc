/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* constdel2d.cc : constrainted version of Delaunay triangulator */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "constdel2d.h"
#include "segment2d.h"
#include "triangle2d.h"
#include "hashtable.h"



/* class WH_CDLN2D_BoundarySegment */

WH_CDLN2D_BoundarySegment
::WH_CDLN2D_BoundarySegment 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1,
 int frontDomainId,
 int rearDomainId)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point0 != point1);
  WH_ASSERT(point0->id () != point1->id ());
  WH_ASSERT(0 <= frontDomainId);
  WH_ASSERT(0 <= rearDomainId);
  WH_ASSERT(frontDomainId != rearDomainId
	    || frontDomainId == rearDomainId);

  WH_CVR_LINE;

  _point0 = point0;
  _point1 = point1;
  _frontDomainId = frontDomainId;
  _rearDomainId = rearDomainId;
  _hasMark = false;
}

WH_CDLN2D_BoundarySegment
::~WH_CDLN2D_BoundarySegment ()
{
  WH_CVR_LINE;

}

bool WH_CDLN2D_BoundarySegment
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->point0 () != WH_NULL);
  WH_ASSERT(this->point1 () != WH_NULL);
  WH_ASSERT(this->point0 () != this->point1 ());
  WH_ASSERT(0 <= this->frontDomainId ());
  WH_ASSERT(0 <= this->rearDomainId ());

  return true;
}

bool WH_CDLN2D_BoundarySegment
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}



/* class WH_CDLN2D_Triangle */

WH_CDLN2D_Triangle
::WH_CDLN2D_Triangle 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1, 
 WH_DLN2D_Point* point2)
  : WH_DLN2D_Triangle (point0, point1, point2)
{
  WH_CVR_LINE;

  _id = WH_NO_INDEX;
  _domainId = WH_NO_INDEX;
}

WH_CDLN2D_Triangle
::~WH_CDLN2D_Triangle ()
{
  WH_CVR_LINE;
}
  
bool WH_CDLN2D_Triangle
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_DLN2D_Triangle::checkInvariant ();

  return true;
}

bool WH_CDLN2D_Triangle
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_DLN2D_Triangle::assureInvariant ();

  return true;
}



/* class WH_CDLN2D_Triangulator */

WH_CDLN2D_Triangulator
::WH_CDLN2D_Triangulator ()
{
  WH_CVR_LINE;

  _boundarySegmentBucket = WH_NULL;
}

WH_CDLN2D_Triangulator
::~WH_CDLN2D_Triangulator ()
{
  WH_CVR_LINE;

  delete _boundarySegmentBucket;
  WH_T_Delete (_boundarySegment_s);
}
  
bool WH_CDLN2D_Triangulator
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_DLN2D_Triangulator::checkInvariant ();

  return true;
}

bool WH_CDLN2D_Triangulator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_DLN2D_Triangulator::assureInvariant ();

  WH_T_AssureInvariant (_boundarySegment_s);

  return true;
}

void WH_CDLN2D_Triangulator
::addBoundarySegment (WH_CDLN2D_BoundarySegment* seg  /* ADOPT */)
{
  /* PRE-CONDITION */
  WH_ASSERT(seg != WH_NULL);

  _boundarySegment_s.push_back (seg);
}

inline static int SegmentHashValue 
(WH_DLN2D_Point* point0, WH_DLN2D_Point* point1)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point0 != point1);
  WH_ASSERT(point0->id () != point1->id ());

  return min (point0->id (), point1->id ());
}

void WH_CDLN2D_Triangulator
::setUpBoundarySegmentBucket ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_boundarySegmentBucket == WH_NULL);

  _boundarySegmentBucket 
    = new WH_HashBucket<WH_CDLN2D_BoundarySegment> 
    (_boundarySegment_s.size ());
  WH_ASSERT(_boundarySegmentBucket != WH_NULL);

  for (vector<WH_CDLN2D_BoundarySegment*>::const_iterator 
	 i_seg = _boundarySegment_s.begin ();
       i_seg != _boundarySegment_s.end ();
       i_seg++) {
    WH_CDLN2D_BoundarySegment* seg_i = (*i_seg);
    int hashValue = SegmentHashValue (seg_i->point0 (), seg_i->point1 ());
    _boundarySegmentBucket->listAt (hashValue).push_back (seg_i);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_boundarySegmentBucket != WH_NULL);
#endif
}

WH_CDLN2D_BoundarySegment* WH_CDLN2D_Triangulator
::findBoundarySegment 
(WH_DLN2D_Point* point0,
 WH_DLN2D_Point* point1) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);

  WH_CVR_LINE;

  WH_CDLN2D_BoundarySegment* result = WH_NULL;

  int hashValue = SegmentHashValue (point0, point1);
  const list<WH_CDLN2D_BoundarySegment*>& seg_s
    = _boundarySegmentBucket->listAt (hashValue);
  
  for (list<WH_CDLN2D_BoundarySegment*>::const_iterator 
	 i_seg = seg_s.begin ();
       i_seg != seg_s.end ();
       i_seg++) {
    WH_CDLN2D_BoundarySegment* seg_i = (*i_seg);
    if (seg_i->hasPoint (point0) && seg_i->hasPoint (point1)) {
      result = seg_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

void WH_CDLN2D_Triangulator
::fitBoundary ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < _triangle_s.size ());

  WH_CVR_LINE;

  for (list<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_CDLN2D_Triangle* tri_i 
      = (WH_CDLN2D_Triangle*)(*i_tri);

    for (int e = 0; e < 3; e++) {
      WH_DLN2D_Point* point0 = tri_i->point 
	(WH_Triangle2D_A::edgeVertexMap[e][0]);
      WH_DLN2D_Point* point1 = tri_i->point 
	(WH_Triangle2D_A::edgeVertexMap[e][1]);
      WH_CDLN2D_BoundarySegment* seg
	= this->findBoundarySegment (point0, point1);
      if (seg != WH_NULL) {
	seg->setMark ();
	WH_Vector2D triangleCenter 
	  = (tri_i->point (0)->position () 
	     + tri_i->point (1)->position () 
	     + tri_i->point (2)->position ()) / 3;
	WH_Vector2D edgeMidPoint 
	  = (point0->position () + point1->position ()) / 2;
	WH_Vector2D dir = triangleCenter - edgeMidPoint;
	if (WH_lt (0, WH_scalarProduct (seg->frontNormal (), dir))) {
	  tri_i->setDomainId (seg->frontDomainId ());
	} else {
	  tri_i->setDomainId (seg->rearDomainId ());
	} 
      }
    }
  }

  for (vector<WH_CDLN2D_BoundarySegment*>::const_iterator 
	 i_seg = _boundarySegment_s.begin ();
       i_seg != _boundarySegment_s.end ();
       i_seg++) {
    WH_CDLN2D_BoundarySegment* seg_i = (*i_seg);
    if (!seg_i->hasMark ()) {
      /* NEED TO REDEFINE */

      WH_ASSERT_NO_REACH;
    }
  }
}

void WH_CDLN2D_Triangulator
::identifyDomain ()
{
  WH_CVR_LINE;

  /* emulation of recursive call using arrays over triangles */

  vector<WH_CDLN2D_Triangle*> tri_s;
  for (list<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_CDLN2D_Triangle* tri_i 
      = (WH_CDLN2D_Triangle*)(*i_tri);
    tri_s.push_back (tri_i);
  }
  
  /* set triangle id */
  for (int iTri = 0; iTri < (int)tri_s.size (); iTri++) {
    WH_CDLN2D_Triangle* tri = tri_s[iTri];
    tri->setId (iTri);
  }

  /* data on call stack */

  vector<char> iEdge_s (tri_s.size ());
  /* current edge index (e = 0, e < 3, e++) for each triangle */

  vector<int> iPrevTri_s (tri_s.size ());
  /* triangle id of previous call */

  vector<bool> hasMark_s (tri_s.size ());
  /* set mark on triangle if it is processing */

  /* initialize stack data */
  for (int iTri = 0; iTri < (int)tri_s.size (); iTri++) {
    iEdge_s[iTri] = 0;
    iPrevTri_s[iTri] = WH_NO_INDEX;
    hasMark_s[iTri] = false;
  }
  
  for (;;) {
    /* find id of any triangle, with known in-out id, without mark */
    int iTriUndefined = WH_NO_INDEX;
    for (int iTri = 0; iTri < (int)tri_s.size (); iTri++) {
      if (tri_s[iTri]->domainId () != WH_NO_INDEX
	  && !hasMark_s[iTri]) {
	iTriUndefined = iTri;
	break;
      }
    }
    if (iTriUndefined == WH_NO_INDEX) break;

    int iCurTri = iTriUndefined;
    WH_ASSERT(0 <= iCurTri);
    WH_ASSERT(iCurTri < (int)tri_s.size ());
    WH_ASSERT(tri_s[iCurTri]->domainId () != WH_NO_INDEX);
    iPrevTri_s[iCurTri] = WH_NO_INDEX;
    iEdge_s[iCurTri] = 0;
    hasMark_s[iCurTri] = true;

    for (;;) {
      WH_ASSERT(hasMark_s[iCurTri]);
      int iEdge = iEdge_s[iCurTri];
      WH_ASSERT(0 <= iEdge);
      WH_ASSERT(iEdge <= 3);
      if (iEdge < 3) {
	/* check neighbor triangle <iEdge> */
	WH_CDLN2D_Triangle* neighbor = (WH_CDLN2D_Triangle*)
	  tri_s[iCurTri]->neighborAt (iEdge);
	if (neighbor == WH_NULL) {
	  /* proceed to the next edge */
	  iEdge_s[iCurTri]++;

	} else {
	  if (hasMark_s[neighbor->id ()]) {
	    /* proceed to the next edge */
	    iEdge_s[iCurTri]++;

	  } else {
	    iEdge_s[iCurTri]++;
	    if (neighbor->domainId () == WH_NO_INDEX) {
	      neighbor->setDomainId (tri_s[iCurTri]->domainId ());
	    }
	    
	    /* go to <neighbor> at edge <iEdge> */
	    int iPrevTri = iCurTri;
	    iCurTri = neighbor->id ();
	    WH_ASSERT(0 <= iCurTri);
	    WH_ASSERT(iCurTri < (int)tri_s.size ());
	    WH_ASSERT(tri_s[iCurTri]->domainId () != WH_NO_INDEX);
	    iPrevTri_s[iCurTri] = iPrevTri;
	    iEdge_s[iCurTri] = 0;
	    hasMark_s[iCurTri] = true;

	  }
	}
      } else {
	/* end of the current triangle call */
	WH_ASSERT(iEdge == 3);
	int iPrevTri = iPrevTri_s[iCurTri];
	if (iPrevTri == WH_NO_INDEX) {
	  /* end loop */
	  break;

	} else {
	  /* go back to the previous triangle call */
	  iCurTri = iPrevTri;
	  WH_ASSERT(0 <= iCurTri);
	  WH_ASSERT(iCurTri < (int)tri_s.size ());
	  WH_ASSERT(tri_s[iCurTri]->domainId () != WH_NO_INDEX);

	}
      }
    }
  }
}

void WH_CDLN2D_Triangulator
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(3 <= _point_s.size ());
  WH_ASSERT(3 <= _boundarySegment_s.size ());

  WH_CVR_LINE;

  this->WH_DLN2D_Triangulator::perform ();
  this->setUpBoundarySegmentBucket ();
  this->fitBoundary ();
  this->identifyDomain ();
}

WH_DLN2D_Triangle* WH_CDLN2D_Triangulator
::createTriangle 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1,
 WH_DLN2D_Point* point2)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);

  WH_CDLN2D_Triangle* result
    = new WH_CDLN2D_Triangle 
    (point0, point1, point2);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}



#ifndef WH_INLINE_ENABLED
#include "constdel2d_inline.cc"
#endif



/* not covered yet
 */







