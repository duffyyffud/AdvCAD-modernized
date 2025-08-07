/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* delaunay3d.cc : tetrahedron generator using Delaunay method */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "delaunay3d.h"
#include "triangle3d.h"
#include "tetrahedron3d.h"
#include "hashtable.h"



/* class WH_DLN3D_Point */

WH_DLN3D_Point
::WH_DLN3D_Point (const WH_Vector3D& position) 
{
  WH_CVR_LINE;

  _id = WH_NO_INDEX;
  _position = position;
}

WH_DLN3D_Point
::~WH_DLN3D_Point () 
{
  WH_CVR_LINE;
}

bool WH_DLN3D_Point
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(0 <= this->id () || this->id () < 0);

  return true;
}

bool WH_DLN3D_Point
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

void WH_DLN3D_Point
::setId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id || id < 0);
  /* point is dummy if <id> is negative */

  _id = id;
}



/* class WH_DLN3D_Triangle */

WH_DLN3D_Triangle
::WH_DLN3D_Triangle 
(WH_DLN3D_Point* point0, 
 WH_DLN3D_Point* point1, 
 WH_DLN3D_Point* point2,
 WH_DLN3D_Tetrahedron* tetra, 
 int faceNumber)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);

  WH_CVR_LINE;

  _tetrahedron = tetra;
  _faceNumber = faceNumber;
  _points[0] = point0;  
  _points[1] = point1;  
  _points[2] = point2;
  for (int e = 0; e < 3; e++) _edges[e] = WH_NULL;
  _front = WH_NULL;
  _frontFaceNumber = WH_NO_INDEX;
  _rear = WH_NULL;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_DLN3D_Triangle
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(this->point (0) != WH_NULL);
  WH_ASSERT(this->point (1) != WH_NULL);
  WH_ASSERT(this->point (2) != WH_NULL);
  WH_ASSERT(this->tetrahedron () != WH_NULL);
  WH_ASSERT(0 <= this->faceNumber ());
  WH_ASSERT(this->faceNumber () < 4);
  if (this->front () != WH_NULL) {
    WH_CVR_LINE;
    WH_ASSERT(0 <= this->frontFaceNumber ());
    WH_ASSERT(this->frontFaceNumber () < 4);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(this->frontFaceNumber () == WH_NO_INDEX);
  }

  return true;
}

bool WH_DLN3D_Triangle
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}



/* class WH_DLN3D_Tetrahedron */

WH_DLN3D_Tetrahedron
::WH_DLN3D_Tetrahedron 
(WH_DLN3D_Point* point0, 
 WH_DLN3D_Point* point1, 
 WH_DLN3D_Point* point2, 
 WH_DLN3D_Point* point3)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);
  WH_ASSERT(point3 != WH_NULL);

  WH_CVR_LINE;

  _points[0] = point0;  
  _points[1] = point1;  
  _points[2] = point2;  
  _points[3] = point3;
  _centerOfSphere 
    = WH_circumcenterAmong (_points[0]->position (), 
			    _points[1]->position (), 
			    _points[2]->position (), 
			    _points[3]->position ());
  _radiusOfSphere 
    = WH_distance (_centerOfSphere, _points[0]->position ());

  for (int f = 0; f < 4; f++) _neighbors[f] = WH_NULL;
  _markFlag = false;
}

WH_DLN3D_Tetrahedron
::~WH_DLN3D_Tetrahedron () 
{
  WH_CVR_LINE;
}

bool WH_DLN3D_Tetrahedron
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (0, _radiusOfSphere));

  WH_ASSERT(this->point (0) != WH_NULL);
  WH_ASSERT(this->point (1) != WH_NULL);
  WH_ASSERT(this->point (2) != WH_NULL);
  WH_ASSERT(this->point (3) != WH_NULL);

  return true;
}

bool WH_DLN3D_Tetrahedron
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}

void WH_DLN3D_Tetrahedron
::clearNeighbor () 
{ 
  WH_CVR_LINE;

  for (int f1 = 0; f1 < 4; f1++) {
    WH_DLN3D_Tetrahedron* neighbor = this->neighborAt (f1);
    if (neighbor != WH_NULL) {
      WH_CVR_LINE;
      for (int f2 = 0; f2 < 4; f2++) {
	if (neighbor->neighborAt (f2) == this) {
	  WH_CVR_LINE;
	  neighbor->clearNeighborAt (f2);
	}
      }
    }
  }
}

void WH_DLN3D_Tetrahedron
::setNeighbor 
(WH_DLN3D_Tetrahedron* tetra) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL || tetra == WH_NULL);
  WH_ASSERT(tetra != this);
#ifndef NDEBUG
  if (tetra != WH_NULL) {
    WH_ASSERT(tetra->isNeighborOf (this));
    WH_ASSERT(this->isNeighborOf (tetra));
  }
#endif

  WH_CVR_LINE;

  /* assuming the tetrahedron is initializing itself, if <tetra> is
     null, do nothing. */

  if (tetra != WH_NULL) {
    this->setNeighborAt (this->faceNumberOfNeighbor (tetra), tetra);
    tetra->setNeighborAt (tetra->faceNumberOfNeighbor (this), this);
  }
}

void WH_DLN3D_Tetrahedron
::reorderPoints ()
{
  WH_CVR_LINE;

  WH_Plane3D plane (_points[0]->position (), 
		    _points[1]->position (), 
		    _points[2]->position ());
  double value = plane.valueAt (_points[3]->position ());
  if (value < 0.0) {
    WH_CVR_LINE;
    WH_DLN3D_Point* point = _points[0];  
    _points[0] = _points[1];  
    _points[1] = point;
    WH_DLN3D_Tetrahedron* neighbor = _neighbors[0];
    _neighbors[0] = _neighbors[1];  
    _neighbors[1] = neighbor;
  }
}

bool WH_DLN3D_Tetrahedron
::isDummy () const
{
  WH_CVR_LINE;

  for (int v = 0; v < 4; v++) {
    if (_points[v]->isDummy ()) return true;
  }
  return false;
}

bool WH_DLN3D_Tetrahedron
::isNeighborOf 
(const WH_DLN3D_Tetrahedron* tetra) const
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(tetra != this);
  
  WH_CVR_LINE;

  int count = 0;
  for (int v = 0; v < 4; v++) {
    if (tetra->hasPoint (this->point (v))) count++;
  }
  WH_ASSERT(count != 4);

  return (count == 3);
}

int WH_DLN3D_Tetrahedron
::faceNumberOfNeighbor 
(const WH_DLN3D_Tetrahedron* tetra) const
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(tetra != this);
  WH_ASSERT(this->isNeighborOf (tetra));
  WH_ASSERT(tetra->isNeighborOf (this));
  
  WH_CVR_LINE;

  int result = WH_NO_INDEX;
  
  for (int v = 0; v < 4; v++) {
    if (!tetra->hasPoint (this->point (v))) {
      result = v;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result < 4);
  for (int v = 0; v < 4; v++) {
    if (v == result) continue;
    WH_ASSERT(tetra->hasPoint (this->point (v)));
  }
#endif

  return result;
}

int WH_DLN3D_Tetrahedron
::faceNumberOf (const WH_DLN3D_Triangle* tri) const
{
  /* PRE-CONDITION */
  WH_ASSERT(tri != WH_NULL);

  WH_CVR_LINE;

  for (int v = 0; v < 4; v++) {
    if (!tri->hasPoint (_points[v])) return v;
  }
  WH_ASSERT_NO_REACH;
  return WH_NO_INDEX;
}

bool WH_DLN3D_Tetrahedron
::isSliver ()
{
  WH_CVR_LINE;

  bool result = false;

  WH_Vector3D v0 = this->point (0)->position ();
  WH_Vector3D v1 = this->point (1)->position ();
  WH_Vector3D v2 = this->point (2)->position ();
  WH_Vector3D v3 = this->point (3)->position ();

  WH_Segment3D v01 (v0, v1);
  WH_Segment3D v02 (v0, v2);
  WH_Segment3D v03 (v0, v3);
  WH_Segment3D v12 (v1, v2);
  WH_Segment3D v13 (v1, v3);
  WH_Segment3D v23 (v2, v3);

  double minEdgeLength = v01.length ();
  minEdgeLength = WH_min (minEdgeLength, v02.length ());
  minEdgeLength = WH_min (minEdgeLength, v03.length ());
  minEdgeLength = WH_min (minEdgeLength, v12.length ());
  minEdgeLength = WH_min (minEdgeLength, v13.length ());
  minEdgeLength = WH_min (minEdgeLength, v23.length ());
  WH_ASSERT(WH_lt (0, minEdgeLength));
  
  double distance01_23 = v01.line ().distanceFrom (v23.line ());
  double distance02_13 = v02.line ().distanceFrom (v13.line ());
  double distance03_12 = v03.line ().distanceFrom (v12.line ());

  double minEdgeDistance = distance01_23;
  minEdgeDistance = WH_min (minEdgeDistance, distance02_13);
  minEdgeDistance = WH_min (minEdgeDistance, distance03_12);
  WH_ASSERT(WH_lt (0, minEdgeDistance));
  
  /* MAGIC NUMBER */
  if (WH_lt (minEdgeDistance / minEdgeLength, 0.2)) {
    result = true;
  }

  return result;
}



/* class WH_DLN3D_Triangulator */

static const int TetraInCubeMap[5][4] = {
  { 1, 3, 4, 0 }, 
  { 3, 6, 1, 2 }, 
  { 4, 1, 6, 5 }, 
  { 3, 4, 6, 7 }, 
  { 4, 1, 3, 6 }
};

static const int NeighborMap[5][4] = {
  { -1, -1, -1, 4 }, 
  { -1, -1, -1, 4 }, 
  { -1, -1, -1, 4 }, 
  { -1, -1, -1, 4 }, 
  { 1, 3, 2, 0 }
};

WH_DLN3D_Triangulator
::WH_DLN3D_Triangulator ()
{
  WH_CVR_LINE;

  _currentPoint = WH_NULL;
}

WH_DLN3D_Triangulator
::~WH_DLN3D_Triangulator ()
{
  WH_CVR_LINE;

  WH_T_Delete (_tetrahedron_s);
  WH_T_Delete (_cornerDummyPoint_s);
  WH_T_Delete (_otherDummyPoint_s);
  WH_T_Delete (_point_s);
}
  
bool WH_DLN3D_Triangulator
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_DLN3D_Triangulator
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _cornerDummyPoint_s.begin ();
       i_point != _cornerDummyPoint_s.end ();
       i_point++) {
    WH_DLN3D_Point* point_i = (*i_point);
    WH_ASSERT(point_i->isDummy ());
  }
  
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _otherDummyPoint_s.begin ();
       i_point != _otherDummyPoint_s.end ();
       i_point++) {
    WH_DLN3D_Point* point_i = (*i_point);
    WH_ASSERT(point_i->isDummy ());
  }

  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++) {
    WH_DLN3D_Point* point_i = (*i_point);
    WH_ASSERT(!point_i->isDummy ());
  }

  WH_T_AssureInvariant (_cornerDummyPoint_s);
  WH_T_AssureInvariant (_otherDummyPoint_s);
  WH_T_AssureInvariant (_point_s);
  WH_T_AssureInvariant (_tetrahedron_s);

  return true;
}

void WH_DLN3D_Triangulator
::addPoint (WH_DLN3D_Point* point)
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  WH_ASSERT(point->id () == WH_NO_INDEX);

  point->setId (_point_s.size ());
  _point_s.push_back (point);

  /* POST-CONDITION */
  WH_ASSERT(point->id () == (int)this->point_s ().size () - 1);
}

void WH_DLN3D_Triangulator
::getRange 
(WH_Vector3D& minRange_OUT, 
 WH_Vector3D& maxRange_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < this->point_s ().size ());
  
  WH_CVR_LINE;
  
  minRange_OUT = WH_Vector3D::hugeValue ();
  maxRange_OUT = -WH_Vector3D::hugeValue ();
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = this->point_s ().begin ();
       i_point != this->point_s ().end ();
       i_point++) {
    WH_DLN3D_Point* point_i = (*i_point);
    WH_Vector3D position = point_i->position ();
    minRange_OUT = WH_min (minRange_OUT, position);
    maxRange_OUT = WH_max (maxRange_OUT, position);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_le (minRange_OUT, maxRange_OUT));
#endif  
}

WH_DLN3D_Point* WH_DLN3D_Triangulator
::createDummyPoint 
(int id, const WH_Vector3D& position)
{
  /* PRE-CONDITION */
  WH_ASSERT(id < 0);

  WH_DLN3D_Point* result 
    = new WH_DLN3D_Point (position);
  result->setId (id);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_DLN3D_Tetrahedron* WH_DLN3D_Triangulator
::createTetrahedron 
(WH_DLN3D_Point* point0, 
 WH_DLN3D_Point* point1,
 WH_DLN3D_Point* point2,
 WH_DLN3D_Point* point3)
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);
  WH_ASSERT(point3 != WH_NULL);

  WH_DLN3D_Tetrahedron* result
    = new WH_DLN3D_Tetrahedron 
    (point0, point1, point2, point3);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_DLN3D_Triangulator
::addTetrahedron (WH_DLN3D_Tetrahedron* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  
  WH_CVR_LINE;

  _tetrahedron_s.push_front (tetra);

  list<WH_DLN3D_Tetrahedron*>::iterator 
    i_tetra = _tetrahedron_s.begin ();
  tetra->setIterator (i_tetra);
}

void WH_DLN3D_Triangulator
::removeTetrahedron (WH_DLN3D_Tetrahedron* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);

  WH_CVR_LINE;

  list<WH_DLN3D_Tetrahedron*>::iterator 
    i_tetra = tetra->iterator ();
  _tetrahedron_s.erase (i_tetra);

  delete tetra;
  tetra = WH_NULL;
}

void WH_DLN3D_Triangulator
::createDummyPoints ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(_otherDummyPoint_s.size () == 0);
  WH_ASSERT(1 < _point_s.size ());

  WH_CVR_LINE;

  WH_Vector3D minRange;
  WH_Vector3D maxRange;
  this->getRange 
    (minRange, maxRange);
  WH_Vector3D center = (minRange + maxRange) / 2;
  double size = WH_distance (minRange, maxRange) / 2 * 1.5;

  WH_Vector3D corners[8];
  corners[0] = center + WH_Vector3D (-size, -size, -size);
  corners[1] = center + WH_Vector3D ( size, -size, -size);
  corners[2] = center + WH_Vector3D ( size,  size, -size);
  corners[3] = center + WH_Vector3D (-size,  size, -size);
  corners[4] = center + WH_Vector3D (-size, -size,  size);
  corners[5] = center + WH_Vector3D ( size, -size,  size);
  corners[6] = center + WH_Vector3D ( size,  size,  size);
  corners[7] = center + WH_Vector3D (-size,  size,  size);

  /* create dummy points that consists of a cube surrounding the */
  /* domain. */

  int count = 0;

  /* dummy points on 8 corners */
  for (int p = 0; p < 8; p++) {
    WH_DLN3D_Point* point 
      = this->createDummyPoint (-1 - count, corners[count]);
    _cornerDummyPoint_s.push_back (point);
    count++;
  }

  /* dummy points near the 6 faces */
  for (int op = 0; op < 6; op++) {
    for (int i = -2; i <= 2; i++) {
      for (int j = -2; j <= 2; j++) {
	WH_Vector3D offset;
	switch (op) {
	case 0:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (0.8, 0.35 * i, 0.35 * j); 
	  break;
	case 1:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (-0.8, 0.35 * i, 0.35 * j); 
	  break;
	case 2:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (0.35 * i, 0.8, 0.35 * j); 
	  break;
	case 3:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (0.35 * i, -0.8, 0.35 * j); 
	  break;
	case 4:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (0.35 * i, 0.35 * j, 0.8); 
	  break;
	case 5:
	  WH_CVR_LINE;
	  offset = WH_Vector3D (0.35 * i, 0.35 * j, -0.8); 
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
	WH_DLN3D_Point* point 
	  = this->createDummyPoint
	  (-1 - count, center + offset * size);
	_otherDummyPoint_s.push_back (point);
	count++;
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_cornerDummyPoint_s.size () == 8);
#endif
}

void WH_DLN3D_Triangulator
::createInitialTetrahedrons ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 8);
  
  WH_CVR_LINE;

  /* create 5 initial tetrahedra that consists of a cube surrounding 
     the domain, using the 8 first dummy points */
  WH_DLN3D_Tetrahedron* initialTetrahedrons[5];
  for (int e = 0; e < 5; e++) {
    WH_DLN3D_Point* point1 
      = _cornerDummyPoint_s[TetraInCubeMap[e][0]];
    WH_DLN3D_Point* point2 
      = _cornerDummyPoint_s[TetraInCubeMap[e][1]];
    WH_DLN3D_Point* point3 
      = _cornerDummyPoint_s[TetraInCubeMap[e][2]];
    WH_DLN3D_Point* point4 
      = _cornerDummyPoint_s[TetraInCubeMap[e][3]];
    initialTetrahedrons[e] 
      = this->createTetrahedron 
      (point1, point2, point3, point4);
    this->addTetrahedron (initialTetrahedrons[e]);
  }
  
  /* set neighboring relationship between initial tetrahedra */
  for (int e = 0; e < 5; e++) {
    for (int f = 0; f < 4; f++) {
      int neighborIndex = NeighborMap[e][f];
      if (0 <= neighborIndex) {
	WH_CVR_LINE;
	WH_ASSERT(0 <= neighborIndex);
	WH_ASSERT(neighborIndex < 5);
	initialTetrahedrons[e]
	  ->setNeighborAt 
	  (f, initialTetrahedrons[neighborIndex]);
      }
    }
  }
}

void WH_DLN3D_Triangulator
::prepare ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(_otherDummyPoint_s.size () == 0);
  WH_ASSERT(1 < _point_s.size ());
  
  WH_CVR_LINE;

  this->createDummyPoints ();
  this->createInitialTetrahedrons ();
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < _cornerDummyPoint_s.size ());
  WH_ASSERT(1 < _point_s.size ());
#endif
}

WH_DLN3D_Tetrahedron* WH_DLN3D_Triangulator
::pickUpFirstTetrahedron () const
{
  WH_CVR_LINE;

  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    if (tetra_i->includesWithinSphere (_currentPoint)) {
      WH_CVR_LINE;
      return tetra_i;
    }
  }
  cerr << " WH_DLN3D_Triangulator : first tetrahedron is not found "
       << endl;
  WH_ASSERT_NO_REACH;
  return WH_NULL;
}

void WH_DLN3D_Triangulator
::searchNeighbor 
(WH_DLN3D_Tetrahedron* tetra, int faceNumber)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);

  WH_CVR_LINE;

  WH_DLN3D_Tetrahedron* neighbor 
    = tetra->neighborAt (faceNumber);
  if (neighbor != WH_NULL && neighbor->hasMark ()) return;
  if (neighbor == WH_NULL 
      || !neighbor->includesWithinSphere (_currentPoint)) {
    WH_CVR_LINE;
    WH_DLN3D_Point* point0 
      = tetra->point 
      (WH_Tetrahedron3D_A::faceVertexMap[faceNumber][0]);
    WH_DLN3D_Point* point1 
      = tetra->point 
      (WH_Tetrahedron3D_A::faceVertexMap[faceNumber][1]);
    WH_DLN3D_Point* point2 
      = tetra->point 
      (WH_Tetrahedron3D_A::faceVertexMap[faceNumber][2]);
    WH_DLN3D_Triangle* tri 
      = new WH_DLN3D_Triangle
      (point0, point1, point2, tetra, faceNumber);
    WH_ASSERT(tri != WH_NULL);
    _surroundingTriangle_s.push_back (tri);
    tri->setFront (neighbor);
  } else {
    WH_CVR_LINE;
    this->markTetrahedron (neighbor);
  }
}

void WH_DLN3D_Triangulator
::markTetrahedron (WH_DLN3D_Tetrahedron* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);

  WH_CVR_LINE;

  tetra->setMark ();
  _deletedTetrahedron_s.push_back (tetra);
  for (int f = 0; f < 4; f++) {
    this->searchNeighbor (tetra, f);
  }
}

bool WH_DLN3D_Triangulator
::checkTriangle ()
{
  WH_CVR_LINE;

  /* check if there are surrounding triangles that has the */
  /* <currentPoint> just on the plane defined by them */
  for (vector<WH_DLN3D_Triangle*>::const_iterator 
	 i_tri = _surroundingTriangle_s.begin ();
       i_tri != _surroundingTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_Triangle* tri_i = (*i_tri);

    WH_Plane3D plane (tri_i->point (0)->position (),
		      tri_i->point (1)->position (),
		      tri_i->point (2)->position ());

    /* NEED TO REDEFINE : numerical error */
#if 0
    if (WH_eq (plane.valueAt (_currentPoint->position ()), 0)) {
      cerr << " checkTriangle CANCEL \n";
      return false;
    }
#else
    /* MAGIC NUMBER */
    if (fabs (plane.valueAt (_currentPoint->position ())) < 0.00001) {
      WH_CVR_LINE;
      cerr << " checkTriangle CANCEL \n";
      return false;
    }
#endif
  }
  return true;
}

void WH_DLN3D_Triangulator
::connectEdgeOfTriangle ()
{
  /* PRE-CONDITION */
#ifndef NDEBUG
  int segmentNewCount = 0;
  int segmentDeleteCount = 0;
#endif

  WH_CVR_LINE;

  WH_HashBucket<Segment> segmentBucket 
    (_surroundingTriangle_s.size () * 3 / 2);

  /* for each triangle, pick up 3 edge segments. find duplication of
     the segment. make relationship between the segment and the
     triangle. */
  for (vector<WH_DLN3D_Triangle*>::const_iterator 
	 i_tri = _surroundingTriangle_s.begin ();
       i_tri != _surroundingTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_Triangle* tri_i = (*i_tri);

    for (int iEdge = 0; iEdge < 3; iEdge++) {
      WH_DLN3D_Point* point0 
	= tri_i->point (WH_Triangle3D_A::edgeVertexMap[iEdge][0]);
      WH_DLN3D_Point* point1 
	= tri_i->point (WH_Triangle3D_A::edgeVertexMap[iEdge][1]);
      int hashValue = point0->id () + point1->id ();
      list<Segment*>& seg_s 
	= segmentBucket.listAt (hashValue);
      /* <seg_s> is not const, so that it can be modified later */

      bool sameSegmentIsFound = false;
      for (list<Segment*>::iterator 
	     i_seg = seg_s.begin ();
	   i_seg != seg_s.end ();
	   i_seg++) {
	Segment* seg_i = (*i_seg);

	if ((seg_i->point0 == point0 && seg_i->point1 == point1)
	    || (seg_i->point0 == point1 && seg_i->point1 == point0)) {
	  WH_CVR_LINE;

	  tri_i->setEdgeAt (iEdge, seg_i->triangle);
	  seg_i->triangle->setEdgeAt (seg_i->edgeNumber, tri_i);
	  
	  /* invalidate <i_seg> and delete <seg_i> */
	  seg_s.erase (i_seg);
	  delete seg_i;
	  seg_i = WH_NULL;

#ifndef NDEBUG
	  segmentDeleteCount++;
#endif
	  
	  sameSegmentIsFound = true;
	  break;
	}
      }

      if (!sameSegmentIsFound) {
	WH_CVR_LINE;

	Segment* seg = new Segment (tri_i, iEdge, point0, point1);
	WH_ASSERT(seg != WH_NULL);
	seg_s.push_front (seg);

#ifndef NDEBUG
	segmentNewCount++;
#endif
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(segmentDeleteCount == segmentNewCount);
  for (vector<WH_DLN3D_Triangle*>::const_iterator 
	 i_tri = _surroundingTriangle_s.begin ();
       i_tri != _surroundingTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_Triangle* tri_i = (*i_tri);
    for (int e = 0; e < 3; e++) {
      WH_ASSERT(tri_i->edgeAt (e) != WH_NULL);
    }
  }
#endif
}

void WH_DLN3D_Triangulator
::deleteOldTetrahedron ()
{
  WH_CVR_LINE;

  /* delete collected tetrahedra that are within the volume defined 
     by surrounding triangles */
  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _deletedTetrahedron_s.begin ();
       i_tetra != _deletedTetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    this->removeTetrahedron (tetra_i);
  }  
  _deletedTetrahedron_s.clear ();
}

void WH_DLN3D_Triangulator
::makeNewTetrahedron ()
{
  WH_CVR_LINE;

  /* for each surrounding triangle, create a new tetrahedron by */
  /* connecting the <currentPoint> and 3 vertices of the surrounding */
  /* triangle */
  for (vector<WH_DLN3D_Triangle*>::const_iterator 
	 i_tri = _surroundingTriangle_s.begin ();
       i_tri != _surroundingTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_Triangle* tri_i = (*i_tri);
    WH_DLN3D_Tetrahedron* tetra = this->createTetrahedron 
      (tri_i->point (0), tri_i->point (1), tri_i->point (2), _currentPoint);
    this->addTetrahedron (tetra);
    tri_i->setRear (tetra);
  }

  /* set neighboring relationship of newly created tetrahedra */
  for (vector<WH_DLN3D_Triangle*>::const_iterator 
	 i_tri = _surroundingTriangle_s.begin ();
       i_tri != _surroundingTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_Triangle* tri_i = (*i_tri);
    WH_DLN3D_Tetrahedron* rearTetra = tri_i->rear ();
    for (int e = 0; e < 3; e++) {
      WH_DLN3D_Tetrahedron* edgeTetra 
	= tri_i->edgeAt (e)->rear ();
      rearTetra->setNeighborAt (e, edgeTetra);
    }
    WH_DLN3D_Tetrahedron* frontTetra = tri_i->front ();
    if (frontTetra != WH_NULL) {
      WH_CVR_LINE;
      rearTetra->setNeighborAt (3, frontTetra);
      int faceNumber = tri_i->frontFaceNumber ();
      frontTetra->setNeighborAt (faceNumber, rearTetra);
    }
  }

  /* delete surrounding triangles */
  WH_T_Delete (_surroundingTriangle_s);
  _surroundingTriangle_s.clear ();
}

bool WH_DLN3D_Triangulator
::insertPoint (WH_DLN3D_Point* point)
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  WH_ASSERT(_deletedTetrahedron_s.size () == 0);  
  WH_ASSERT(_surroundingTriangle_s.size () == 0);  
  
  WH_CVR_LINE;

  bool result = false;

  _currentPoint = point;

  this->markTetrahedron 
    (this->pickUpFirstTetrahedron ());
  if (this->checkTriangle ()) {
    WH_CVR_LINE;
    /* perform addition of the point */

    this->connectEdgeOfTriangle ();
    this->deleteOldTetrahedron ();
    this->makeNewTetrahedron ();
    
    result = true;
  } else {
    WH_CVR_LINE;
    /* cancel addition of the point */
    
    for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	   i_tetra = _deletedTetrahedron_s.begin ();
	 i_tetra != _deletedTetrahedron_s.end ();
	 i_tetra++) {
      WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
      tetra_i->clearMark ();
    }
    _deletedTetrahedron_s.clear ();
    
    WH_T_Delete (_surroundingTriangle_s);
    _surroundingTriangle_s.clear ();  
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_deletedTetrahedron_s.size () == 0);  
  WH_ASSERT(_surroundingTriangle_s.size () == 0);  
#endif

  return result;
}

void WH_DLN3D_Triangulator
::makeTetrahedron ()
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < _cornerDummyPoint_s.size ());
  WH_ASSERT(1 < _point_s.size ());

  WH_CVR_LINE;

  /* add other dummy points near the 6 faces of a cube */
  /* surrounding the domain  */
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _otherDummyPoint_s.begin ();
       i_point != _otherDummyPoint_s.end ();
       i_point++) {
    WH_DLN3D_Point* point_i = (*i_point);
    this->insertPoint (point_i);
  }

  vector<bool> checkMarks (_point_s.size ());
  for (int i_point = 0; 
       i_point < (int)checkMarks.size (); 
       i_point++) {
    checkMarks[i_point] = false;
  }
  
  /* add each one of 50 points */
  int markNumber = 0;
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN3D_Point* point_i = (*i_point);
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
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN3D_Point* point_i = (*i_point);
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
  for (vector<WH_DLN3D_Point*>::const_iterator 
	 i_point = _point_s.begin ();
       i_point != _point_s.end ();
       i_point++, markNumber++) {
    WH_DLN3D_Point* point_i = (*i_point);
    if (!checkMarks[markNumber]) {
      WH_CVR_LINE;
      if (this->insertPoint (point_i)) {
	WH_CVR_LINE;
	checkMarks[markNumber] = true;
      }
    }
  }
}

void WH_DLN3D_Triangulator
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_cornerDummyPoint_s.size () == 0);
  WH_ASSERT(_otherDummyPoint_s.size () == 0);
  WH_ASSERT(1 < _point_s.size ());

  WH_CVR_LINE;

  this->prepare ();
  this->makeTetrahedron ();
}

void WH_DLN3D_Triangulator
::deleteTetrahedrons (const vector<WH_DLN3D_Tetrahedron*>& tetra_s)
{
  WH_CVR_LINE;

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = tetra_s.begin ();
       i_tetra != tetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    tetra_i->clearNeighbor ();
    this->removeTetrahedron (tetra_i);
  }

  /* now <tetra_s> are invalid */
}

void WH_DLN3D_Triangulator
::reorderTetrahedron ()
{
  WH_CVR_LINE;

  /* for each tetrahedron, correct order of its vertices */
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    tetra_i->reorderPoints ();
  }
}



#ifndef WH_INLINE_ENABLED
#include "delaunay3d_inline.cc"
#endif



/* not covered yet
delaunay3d.cc:129:  WH_CVR_LINE;
delaunay3d.cc:205:    WH_CVR_LINE;
delaunay3d.cc:219:  WH_CVR_LINE;
delaunay3d.cc:434:  WH_CVR_LINE;
delaunay3d.cc:989:      WH_CVR_LINE;
delaunay3d.cc:1158:    WH_CVR_LINE;
*/
