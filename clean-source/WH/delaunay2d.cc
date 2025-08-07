/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* delaunay2d.cc : triangle generator using Delaunay method */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "delaunay2d.h"
#include "triangle2d.h"
#include <iostream>

using namespace std;



/* class WH_DLN2D_Point */

WH_DLN2D_Point
::WH_DLN2D_Point (const WH_Vector2D& position) 
{
  WH_CVR_LINE;

  _id = WH_NO_INDEX;
  _position = position;
}

WH_DLN2D_Point
::~WH_DLN2D_Point () 
{
  WH_CVR_LINE;
}

bool WH_DLN2D_Point
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(0 <= this->id () || this->id () < 0);

  return true;
}

bool WH_DLN2D_Point
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isDummy ()) {
    WH_CVR_LINE;
    WH_ASSERT(this->id () < 0);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(0 <= this->id ());
  }

  return true;
}



/* class WH_DLN2D_Segment */

WH_DLN2D_Segment
::WH_DLN2D_Segment 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);

  WH_CVR_LINE;

  _point0 = point0;
  _point1 = point1;
  _edge0 = WH_NULL;
  _edge1 = WH_NULL;
  _front = WH_NULL;
  _rear = WH_NULL;
}

bool WH_DLN2D_Segment
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->point0 () != WH_NULL);
  WH_ASSERT(this->point1 () != WH_NULL);

  return true;
}

bool WH_DLN2D_Segment
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_DLN2D_Segment
::addEdge (WH_DLN2D_Segment* segment)
{
  /* PRE-CONDITION */
  WH_ASSERT(segment != WH_NULL);

  WH_CVR_LINE;

  if (_edge0 == segment || _edge1 == segment) return;
  if (_edge0 == WH_NULL && segment->hasPoint (_point0)) {
    WH_CVR_LINE;
    _edge0 = segment;
  } else if (_edge1 == WH_NULL && segment->hasPoint (_point1)) {
    WH_CVR_LINE;
    _edge1 = segment;
  } else {
    WH_ASSERT_NO_REACH;
  }
}



/* WH_DLN2D_Triangle class */

WH_DLN2D_Triangle
::WH_DLN2D_Triangle 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1, 
 WH_DLN2D_Point* point2)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);

  WH_CVR_LINE;

  _points[0] = point0;  
  _points[1] = point1;  
  _points[2] = point2;
  
  // Check for nearly collinear points before circumcircle calculation
  WH_Vector2D p0 = _points[0]->position();
  WH_Vector2D p1 = _points[1]->position();
  WH_Vector2D p2 = _points[2]->position();
  
  // Compute triangle area using cross product
  double area = (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
  
  // Compute adaptive tolerance based on edge lengths
  double edge1 = (p1 - p0).length();
  double edge2 = (p2 - p1).length();
  double edge3 = (p0 - p2).length();
  double minEdge = WH_min(edge1, WH_min(edge2, edge3));
  double tolerance = minEdge * minEdge * 1e-10; // Area tolerance
  
  if (abs(area) < tolerance) {
    cerr << "WARNING: Nearly collinear triangle detected (area=" << area 
         << ", tolerance=" << tolerance << ")" << endl;
    cerr << "Points: (" << p0.x << "," << p0.y << ") (" 
         << p1.x << "," << p1.y << ") (" << p2.x << "," << p2.y << ")" << endl;
  }
  _centerOfCircle 
    = WH_circumcenterAmong (_points[0]->position (), 
			    _points[1]->position (), 
			    _points[2]->position ());
  _radiusOfCircle 
    = WH_distance (_centerOfCircle, _points[0]->position ());
    
  // Handle degenerate case: ensure positive radius
  if (!WH_lt(0, _radiusOfCircle)) {
    cerr << "WARNING: Degenerate triangle - computed radius=" << _radiusOfCircle << endl;
    cerr << "   Triangle edges: " << edge1 << ", " << edge2 << ", " << edge3 << endl;
    // Use larger fallback radius to ensure it's always positive
    _radiusOfCircle = WH_max(minEdge * 0.1, 1e-10); 
    cerr << "   Using fallback radius: " << _radiusOfCircle << endl;
  }

  for (int e = 0; e < 3; e++) _neighbors[e] = WH_NULL;
  _markFlag = false;
}

WH_DLN2D_Triangle
::~WH_DLN2D_Triangle () 
{
  WH_CVR_LINE;
}

bool WH_DLN2D_Triangle
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (0, _radiusOfCircle));

  WH_ASSERT(this->point (0) != WH_NULL);
  WH_ASSERT(this->point (1) != WH_NULL);
  WH_ASSERT(this->point (2) != WH_NULL);

  return true;
}

bool WH_DLN2D_Triangle
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_DLN2D_Triangle
::clearNeighbor () 
{ 
  WH_CVR_LINE;

  for (int e1 = 0; e1 < 3; e1++) {
    WH_DLN2D_Triangle* neighbor = this->neighborAt (e1);
    if (neighbor != WH_NULL) {
      WH_CVR_LINE;
      for (int e2 = 0; e2 < 3; e2++) {
	if (neighbor->neighborAt (e2) == this) {
	  WH_CVR_LINE;
	  neighbor->clearNeighborAt (e2);
	}
      }
    }
  }
}

void WH_DLN2D_Triangle
::setNeighbor 
(WH_DLN2D_Triangle* tri) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL || tri == WH_NULL);
  WH_ASSERT(tri != this);
#ifndef NDEBUG
  if (tri != WH_NULL) {
    WH_ASSERT(tri->isNeighborOf (this));
    WH_ASSERT(this->isNeighborOf (tri));
  }
#endif

  WH_CVR_LINE;

  /* assuming the triangle is initializing itself, if <tri> is
     null, do nothing. */

  if (tri != WH_NULL) {
    this->setNeighborAt (this->edgeNumberOfNeighbor (tri), tri);
    tri->setNeighborAt (tri->edgeNumberOfNeighbor (this), this);
  }
}

void WH_DLN2D_Triangle
::reorderPoints ()
{
  WH_CVR_LINE;

  WH_Line2D line (_points[0]->position (), 
		  _points[1]->position ());
  double value = line.valueAt (_points[2]->position ());
  if (value < 0.0) {
    WH_CVR_LINE;
    WH_DLN2D_Point* point = _points[0];  
    _points[0] = _points[1];  
    _points[1] = point;
    WH_DLN2D_Triangle* neighbor = _neighbors[0];
    _neighbors[0] = _neighbors[1];  
    _neighbors[1] = neighbor;
  }
}

bool WH_DLN2D_Triangle
::isDummy () const
{
  WH_CVR_LINE;

  for (int v = 0; v < 3; v++) {
    if (_points[v]->isDummy ()) return true;
  }
  return false;
}

bool WH_DLN2D_Triangle
::hasMark () const
{ 
  return _markFlag; 
}

bool WH_DLN2D_Triangle
::isNeighborOf 
(const WH_DLN2D_Triangle* tri) const
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);
  WH_ASSERT(tri != this);
  
  WH_CVR_LINE;

  int count = 0;
  for (int v = 0; v < 3; v++) {
    if (tri->hasPoint (this->point (v))) count++;
  }
  WH_ASSERT(count != 3);

  return (count == 2);
}

int WH_DLN2D_Triangle
::edgeNumberOfNeighbor 
(const WH_DLN2D_Triangle* tri) const
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);
  WH_ASSERT(tri != this);
  WH_ASSERT(this->isNeighborOf (tri));
  WH_ASSERT(tri->isNeighborOf (this));
  
  WH_CVR_LINE;

  int result = WH_NO_INDEX;
  
  for (int v = 0; v < 3; v++) {
    if (!tri->hasPoint (this->point (v))) {
      result = v;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < 3);
  for (int v = 0; v < 3; v++) {
    if (v == result) continue;
    WH_ASSERT(tri->hasPoint (this->point (v)));
  }
#endif

  return result;
}

int WH_DLN2D_Triangle
::edgeNumberOf (const WH_DLN2D_Segment* seg) const
{
  /* PRE-CONDITION */
  WH_ASSERT(seg != WH_NULL);

  WH_CVR_LINE;

  for (int v = 0; v < 3; v++) {
    if (!seg->hasPoint (_points[v])) return v;
  }
  WH_ASSERT_NO_REACH;
  return WH_NO_INDEX;
}



/* class WH_DLN2D_Triangulator */

WH_DLN2D_Triangulator
::WH_DLN2D_Triangulator ()
{
  WH_CVR_LINE;

  _currentPoint = WH_NULL;
}

WH_DLN2D_Triangulator
::~WH_DLN2D_Triangulator ()
{
  WH_CVR_LINE;

  WH_T_Delete (_triangle_s);
  WH_T_Delete (_cornerDummyPoint_s);
  WH_T_Delete (_point_s);
}
  
bool WH_DLN2D_Triangulator
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_DLN2D_Triangulator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = _cornerDummyPoint_s.begin ();
       i_point != _cornerDummyPoint_s.end ();
       i_point++) {
    WH_DLN2D_Point* point_i = (*i_point);
    WH_ASSERT(point_i->isDummy ());
  }

  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++) {
    WH_DLN2D_Point* point_i = (*i_point);
    WH_ASSERT(!point_i->isDummy ());
  }

  WH_T_AssureInvariant (_cornerDummyPoint_s);
  WH_T_AssureInvariant (_point_s);
  WH_T_AssureInvariant (_triangle_s);

  return true;
}

void WH_DLN2D_Triangulator
::addPoint (WH_DLN2D_Point* point)
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  WH_ASSERT(point->id () == WH_NO_INDEX);
  
  point->setId (_point_s.size ());
  _point_s.push_back (point);

  /* POST-CONDITION */
  WH_ASSERT(point->id () == (int)this->point_s ().size () - 1);
}

void WH_DLN2D_Triangulator
::getRange 
(WH_Vector2D& minRange_OUT, 
 WH_Vector2D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->point_s ().size ());
  
  WH_CVR_LINE;
  
  minRange_OUT = WH_Vector2D::hugeValue ();
  maxRange_OUT = -WH_Vector2D::hugeValue ();
  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = this->point_s ().begin ();
       i_point != this->point_s ().end ();
       i_point++) {
    WH_DLN2D_Point* point_i = (*i_point);
    WH_Vector2D position = point_i->position ();
    minRange_OUT = WH_min (minRange_OUT, position);
    maxRange_OUT = WH_max (maxRange_OUT, position);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
#endif  
}

WH_DLN2D_Point* WH_DLN2D_Triangulator
::createDummyPoint 
(int id, const WH_Vector2D& position)
{
  /* PRE-CONDITION */
  WH_ASSERT(id < 0);

  WH_DLN2D_Point* result 
    = new WH_DLN2D_Point (position);
  result->setId (id);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_DLN2D_Triangle* WH_DLN2D_Triangulator
::createTriangle 
(WH_DLN2D_Point* point0, 
 WH_DLN2D_Point* point1,
 WH_DLN2D_Point* point2)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);

  WH_DLN2D_Triangle* result
    = new WH_DLN2D_Triangle 
    (point0, point1, point2);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_DLN2D_Triangulator
::addTriangle (WH_DLN2D_Triangle* tri)
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);
  
  WH_CVR_LINE;

  _triangle_s.push_front (tri);

  list<WH_DLN2D_Triangle*>::iterator 
    i_tri = _triangle_s.begin ();
  tri->setIterator (i_tri);
}

void WH_DLN2D_Triangulator
::removeTriangle (WH_DLN2D_Triangle* tri)
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);

  WH_CVR_LINE;

  list<WH_DLN2D_Triangle*>::iterator 
    i_tri = tri->iterator ();
  _triangle_s.erase (i_tri);

  delete tri;
  tri = WH_NULL;
}

void WH_DLN2D_Triangulator
::createDummyPoints ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(1 < _point_s.size ());

  WH_CVR_LINE;

  WH_Vector2D minRange;
  WH_Vector2D maxRange;
  this->getRange 
    (minRange, maxRange);
  WH_Vector2D center = (minRange + maxRange) / 2;
  double size = WH_distance (minRange, maxRange) / 2 * 1.5;

  WH_Vector2D corners[4];
  corners[0] = center + WH_Vector2D (-size, -size);
  corners[1] = center + WH_Vector2D ( size, -size);
  corners[2] = center + WH_Vector2D ( size,  size);
  corners[3] = center + WH_Vector2D (-size,  size);

  int count = 0;

  /* create 4 dummy points that consists of a rectangle surrounding the */
  /* domain */
  for (int p = 0; p < 4; p++) {
    WH_DLN2D_Point* point 
      = this->createDummyPoint (-1 - count, corners[count]);
    _cornerDummyPoint_s.push_back (point);
    count++;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_cornerDummyPoint_s.size () == 4);
#endif
}

void WH_DLN2D_Triangulator
::createInitialTriangles ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 4);
  
  WH_CVR_LINE;

  /* create 2 initial triangle that consists of a rectangle surrounding the */
  /* domain */
  WH_DLN2D_Triangle* initialTriangles[2];
  initialTriangles[0] 
    = this->createTriangle (_cornerDummyPoint_s[0], 
			    _cornerDummyPoint_s[1], 
			    _cornerDummyPoint_s[3]);
  initialTriangles[1] 
    = this->createTriangle (_cornerDummyPoint_s[2], 
			    _cornerDummyPoint_s[3], 
			    _cornerDummyPoint_s[1]);
  initialTriangles[0]
    ->setNeighborAt (0, initialTriangles[1]);
  initialTriangles[1]
    ->setNeighborAt (0, initialTriangles[0]);
  this->addTriangle (initialTriangles[0]);
  this->addTriangle (initialTriangles[1]);
}

void WH_DLN2D_Triangulator
::prepare ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(1 < _point_s.size ());
  
  WH_CVR_LINE;

  this->createDummyPoints ();
  this->createInitialTriangles ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < _cornerDummyPoint_s.size ());
  WH_ASSERT(1 < _point_s.size ());
#endif
}

WH_DLN2D_Triangle* WH_DLN2D_Triangulator
::pickUpFirstTriangle () const
{
  WH_CVR_LINE;

  for (list<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_DLN2D_Triangle* tri_i = (*i_tri);
    if (tri_i->includesWithinCircle (_currentPoint)) {
      WH_CVR_LINE;
      return tri_i;
    }
  }
  cerr <<  " WH_DLN2D_Triangulator : first triangle is not found "
       << endl;
  WH_ASSERT_NO_REACH;
  return WH_NULL;
}

void WH_DLN2D_Triangulator
::searchNeighbor (WH_DLN2D_Triangle* tri, int edgeNumber)
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);

  WH_CVR_LINE;

  WH_DLN2D_Triangle* neighbor = tri->neighborAt (edgeNumber);
  if (neighbor != WH_NULL && neighbor->hasMark ()) return;
  if (neighbor == WH_NULL 
      || !neighbor->includesWithinCircle (_currentPoint)) {
    WH_CVR_LINE;
    WH_DLN2D_Point* point0 
      = tri->point 
      (WH_Triangle2D_A::edgeVertexMap[edgeNumber][0]);
    WH_DLN2D_Point* point1 
      = tri->point 
      (WH_Triangle2D_A::edgeVertexMap[edgeNumber][1]);
    WH_DLN2D_Segment* seg 
      = new WH_DLN2D_Segment (point0, point1);
    WH_ASSERT(seg != WH_NULL);
    _surroundingSegment_s.push_back (seg);
    seg->setFront (neighbor);
  } else {
    WH_CVR_LINE;
    this->markTriangle (neighbor);
  }
}

void WH_DLN2D_Triangulator
::markTriangle (WH_DLN2D_Triangle* tri)
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);

  WH_CVR_LINE;

  tri->setMark ();
  _deletedTriangle_s.push_back (tri);
  for (int e = 0; e < 3; e++) {
    this->searchNeighbor (tri, e);
  }
}

bool WH_DLN2D_Triangulator
::checkSegment ()
{
  WH_CVR_LINE;

  /* check if there are surrounding segments that has the */
  /* <currentPoint> just on the line defined by them */
  for (vector<WH_DLN2D_Segment*>::const_iterator 
	 i_seg = _surroundingSegment_s.begin ();
       i_seg != _surroundingSegment_s.end ();
       i_seg++) {
    WH_DLN2D_Segment* seg_i = (*i_seg);
    
    WH_Line2D line (seg_i->point0 ()->position (),
		    seg_i->point1 ()->position ());
    if (WH_eq (line.valueAt (_currentPoint->position ()), 0)) {
      WH_CVR_LINE;
      return false;
    }
  }
  return true;
}

void WH_DLN2D_Triangulator
::connectSegment ()
{
  WH_CVR_LINE;

  /* find duplication of the segment. make relationship between segments. */ 
  for (vector<WH_DLN2D_Segment*>::const_iterator 
	 i_seg = _surroundingSegment_s.begin ();
       i_seg != _surroundingSegment_s.end ();
       i_seg++) {
    WH_DLN2D_Segment* seg_i = (*i_seg);

    for (vector<WH_DLN2D_Segment*>::const_iterator 
	   j_seg = _surroundingSegment_s.begin ();
	 j_seg != _surroundingSegment_s.end ();
	 j_seg++) {
      WH_DLN2D_Segment* seg_j = (*j_seg);

      if (seg_i == seg_j) continue;
      if (seg_j->hasPoint (seg_i->point0 ())
	  || seg_j->hasPoint (seg_i->point1 ())) {
	WH_CVR_LINE;
	seg_i->addEdge (seg_j);
	seg_j->addEdge (seg_i);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (vector<WH_DLN2D_Segment*>::const_iterator 
	 i_seg = _surroundingSegment_s.begin ();
       i_seg != _surroundingSegment_s.end ();
       i_seg++) {
    WH_DLN2D_Segment* seg_i = (*i_seg);
    WH_ASSERT (seg_i->edge0 () != WH_NULL 
	       && seg_i->edge1 () != WH_NULL);
  }
#endif
}

void WH_DLN2D_Triangulator
::deleteOldTriangle ()
{
  WH_CVR_LINE;

  /* delete collected triangles that are within the area defined by */
  /* surrounding edges */
  for (vector<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = _deletedTriangle_s.begin ();
       i_tri != _deletedTriangle_s.end ();
       i_tri++) {
    WH_DLN2D_Triangle* tri_i = (*i_tri);
    this->removeTriangle (tri_i);
  }  
  _deletedTriangle_s.clear ();
}

void WH_DLN2D_Triangulator
::makeNewTriangle ()
{
  WH_CVR_LINE;

  /* for each surrounding edge, create a new triangle by connecting */
  /* the <_currentPoint> and 2 vertices of the surrounding edge */
  for (vector<WH_DLN2D_Segment*>::const_iterator 
	 i_seg = _surroundingSegment_s.begin ();
       i_seg != _surroundingSegment_s.end ();
       i_seg++) {
    WH_DLN2D_Segment* seg_i = (*i_seg);
    WH_DLN2D_Triangle* tri = this->createTriangle 
      (seg_i->point0 (), seg_i->point1 (), _currentPoint);
    this->addTriangle (tri);
    seg_i->setRear (tri);
  }

  /* set neighboring relationship of newly created triangles */
  for (vector<WH_DLN2D_Segment*>::const_iterator 
	 i_seg = _surroundingSegment_s.begin ();
       i_seg != _surroundingSegment_s.end ();
       i_seg++) {
    WH_DLN2D_Segment* seg_i = (*i_seg);

    WH_DLN2D_Triangle* rearTri = seg_i->rear ();
    rearTri->setNeighborAt (1, seg_i->edge0 ()->rear ());
    rearTri->setNeighborAt (0, seg_i->edge1 ()->rear ());
    WH_DLN2D_Triangle* frontTri = seg_i->front ();
    if (frontTri != WH_NULL) {
      WH_CVR_LINE;
      rearTri->setNeighborAt (2, frontTri);
      int edgeNumber = frontTri->edgeNumberOf (seg_i);
      frontTri->setNeighborAt (edgeNumber, rearTri);
    }
  }

  /* delete surrounding segments */
  WH_T_Delete (_surroundingSegment_s);
  _surroundingSegment_s.clear ();
}

bool WH_DLN2D_Triangulator
::insertPoint (WH_DLN2D_Point* point)
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  WH_ASSERT(_deletedTriangle_s.size () == 0);  
  WH_ASSERT(_surroundingSegment_s.size () == 0);  
  
  WH_CVR_LINE;

  bool result = false;

  _currentPoint = point;

  this->markTriangle 
    (this->pickUpFirstTriangle ());
  if (this->checkSegment ()) {
    WH_CVR_LINE;
    /* perform addition of the point */

    this->connectSegment ();
    this->deleteOldTriangle ();
    this->makeNewTriangle ();

    result = true;
  } else {
    WH_CVR_LINE;
    /* cancel addition of the point */

    for (vector<WH_DLN2D_Triangle*>::const_iterator 
	   i_tri = _deletedTriangle_s.begin ();
	 i_tri != _deletedTriangle_s.end ();
	 i_tri++) {
      WH_DLN2D_Triangle* tri_i = (*i_tri);
      tri_i->clearMark ();
    }
    _deletedTriangle_s.clear ();
    
    WH_T_Delete (_surroundingSegment_s);
    _surroundingSegment_s.clear ();  
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_deletedTriangle_s.size () == 0);  
  WH_ASSERT(_surroundingSegment_s.size () == 0);  
#endif

  return result;
}

void WH_DLN2D_Triangulator
::makeTriangle ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < _cornerDummyPoint_s.size ());
  WH_ASSERT(2 < _point_s.size ());

  WH_CVR_LINE;

  vector<bool> checkMarks (_point_s.size ());
  for (int i_point = 0; 
       i_point < (int)checkMarks.size (); 
       i_point++) {
    checkMarks[i_point] = false;
  }
  
  /* add each one of 50 points */
  int markNumber = 0;
  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN2D_Point* point_i = (*i_point);
    if (!checkMarks[markNumber] && markNumber % 50 == 0) {
      WH_CVR_LINE;
      if (this->insertPoint (point_i)) {
	WH_CVR_LINE;
	checkMarks[markNumber] = true;
      }
    }
  }
  
  /* add each one of 10 points */
  markNumber = 0;
  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN2D_Point* point_i = (*i_point);
    if (!checkMarks[markNumber] && markNumber % 10 == 0) {
      WH_CVR_LINE;
      if (this->insertPoint (point_i)) {
	WH_CVR_LINE;
	checkMarks[markNumber] = true;
      }
    }
  }
  
  /* add rest of points */
  markNumber = 0;
  for (vector<WH_DLN2D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN2D_Point* point_i = (*i_point);
    if (!checkMarks[markNumber]) {
      WH_CVR_LINE;
      if (this->insertPoint (point_i)) {
	WH_CVR_LINE;
	checkMarks[markNumber] = true;
      }
    }
  }
}

void WH_DLN2D_Triangulator
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(2 < _point_s.size ());

  WH_CVR_LINE;

  this->prepare ();
  this->makeTriangle ();
}

void WH_DLN2D_Triangulator
::deleteTriangles (const vector<WH_DLN2D_Triangle*>& tri_s)
{
  WH_CVR_LINE;

  for (vector<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = tri_s.begin ();
       i_tri != tri_s.end ();
       i_tri++) {
    WH_DLN2D_Triangle* tri_i = (*i_tri);
    tri_i->clearNeighbor ();
    this->removeTriangle (tri_i);
  }

  /* now <tri_s> are invalid */
}

void WH_DLN2D_Triangulator
::reorderTriangle ()
{
  WH_CVR_LINE;

  /* for each triangle, correct order of its vertices */
  for (list<WH_DLN2D_Triangle*>::const_iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_DLN2D_Triangle* tri_i = (*i_tri);
    tri_i->reorderPoints ();
  }
}



#ifndef WH_INLINE_ENABLED
#include "delaunay2d_inline.cc"
#endif



/* not covered yet
 */







