/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* mg3d_delaunay3d.cc : Delaunay triangulation over a volume */

#include "mg3d_delaunay3d.h"
#include "bucket3d.h"
#include "inout3d.h"
#include "triangle3d.h"
#include "tetrahedron3d.h"



/* class WH_DLN3D_Point_MG3D */

WH_DLN3D_Point_MG3D
::WH_DLN3D_Point_MG3D 
(const WH_Vector3D& position,
 WH_MG3D_Node* node)
  : WH_DLN3D_Point (position) 
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  
  _node = node;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_DLN3D_Point_MG3D
::~WH_DLN3D_Point_MG3D ()
{
}

bool WH_DLN3D_Point_MG3D
::checkInvariant () const
{
  this->WH_DLN3D_Point::checkInvariant ();

  WH_ASSERT (this->node () != WH_NULL);
  
  return true;
}

bool WH_DLN3D_Point_MG3D
::assureInvariant () const
{
  this->WH_DLN3D_Point::assureInvariant ();
  
  return true;
}

WH_MG3D_Node* WH_DLN3D_Point_MG3D
::node () const
{
  return _node;
}



/* class WH_DLN3D_Tetrahedron_MG3D */

WH_DLN3D_Tetrahedron_MG3D
::WH_DLN3D_Tetrahedron_MG3D 
(WH_DLN3D_Point_MG3D* point0, 
 WH_DLN3D_Point_MG3D* point1, 
 WH_DLN3D_Point_MG3D* point2, 
 WH_DLN3D_Point_MG3D* point3)
  : WH_DLN3D_Tetrahedron (point0, point1, point2, point3) 
{
  _inOutType = UNDEFINED;
  _tetrahedron = WH_NULL;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_DLN3D_Tetrahedron_MG3D
::~WH_DLN3D_Tetrahedron_MG3D ()
{
}

bool WH_DLN3D_Tetrahedron_MG3D
::checkInvariant () const
{
  this->WH_DLN3D_Tetrahedron::checkInvariant ();
  
  WH_ASSERT(this->tetrahedron () == WH_NULL
	    || this->tetrahedron () != WH_NULL);

  return true;
}

bool WH_DLN3D_Tetrahedron_MG3D
::assureInvariant () const
{
  this->WH_DLN3D_Tetrahedron::assureInvariant ();
  
  if (this->tetrahedron () != WH_NULL) {
    WH_ASSERT(this->inOutType () == INNER);
  }

  return true;
}

WH_DLN3D_Point_MG3D* WH_DLN3D_Tetrahedron_MG3D
::pointMg (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < 4);
  WH_ASSERT(!this->point (index)->isDummy ());

  WH_DLN3D_Point_MG3D* result 
    = dynamic_cast<WH_DLN3D_Point_MG3D*>(_points[index]);
  WH_ASSERT(result != WH_NULL);

  return result;
}

bool WH_DLN3D_Tetrahedron_MG3D
::hasAnyInsideVolumeNode () const
{
  bool result = false;

  for (int v = 0; v < 4; v++) {
    if (this->point (v)->isDummy ()) continue;

    WH_DLN3D_Point_MG3D* pointMg = this->pointMg (v);
    WH_MG3D_Node* node = pointMg->node ();
    if (node != WH_NULL) {
      if (node->topologyType ()== WH_MG3D_Node::INSIDE_VOLUME) {
	result = true;
	break;
      }
    }
  }

  return result;
}

bool WH_DLN3D_Tetrahedron_MG3D
::hasAnyOutsideVolumeNode () const
{
  bool result = false;

  for (int v = 0; v < 4; v++) {
    if (this->point (v)->isDummy ()) {
      result = true;
      break;
    }

    WH_DLN3D_Point_MG3D* pointMg = this->pointMg (v);
    WH_MG3D_Node* node = pointMg->node ();
    if (node != WH_NULL) {
      if (node->topologyType ()== WH_MG3D_Node::OUTSIDE_VOLUME) {
	result = true;
	break;
      }
    }
  }

  return result;
}

WH_DLN3D_Tetrahedron_MG3D::InOutType WH_DLN3D_Tetrahedron_MG3D
::inOutType () const
{
  return _inOutType;
}

void WH_DLN3D_Tetrahedron_MG3D
::setInOutType (InOutType inOutType)
{
  _inOutType = inOutType;
}

void WH_DLN3D_Tetrahedron_MG3D
::setTetrahedron (WH_MG3D_Tetrahedron* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);

  _tetrahedron = tetra;
}

WH_MG3D_Tetrahedron* WH_DLN3D_Tetrahedron_MG3D
::tetrahedron () const
{
  return _tetrahedron;
}



/* class WH_DLN3D_FaceTriangle_MG3D */

WH_DLN3D_FaceTriangle_MG3D
::WH_DLN3D_FaceTriangle_MG3D 
(WH_DLN3D_Point_MG3D* point0,
 WH_DLN3D_Point_MG3D* point1,
 WH_DLN3D_Point_MG3D* point2,
 WH_DLN3D_Tetrahedron_MG3D* innerTetrahedron,
 WH_DLN3D_Tetrahedron_MG3D* outerTetrahedron) 
{
  /* PRE-CONDITION */
  WH_ASSERT(point0 != WH_NULL);
  WH_ASSERT(point1 != WH_NULL);
  WH_ASSERT(point2 != WH_NULL);
  WH_ASSERT(point0 != point1);
  WH_ASSERT(point0 != point2);
  WH_ASSERT(point1 != point2);
  WH_ASSERT(innerTetrahedron != WH_NULL);
  WH_ASSERT(innerTetrahedron->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::INNER);
  WH_ASSERT(outerTetrahedron != WH_NULL);
  WH_ASSERT(outerTetrahedron->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::OUTER);

  _point0 = point0;
  _point1 = point1;
  _point2 = point2;
  _innerTetrahedron = innerTetrahedron;
  _outerTetrahedron = outerTetrahedron;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_DLN3D_FaceTriangle_MG3D
::~WH_DLN3D_FaceTriangle_MG3D () 
{
}

bool WH_DLN3D_FaceTriangle_MG3D
::checkInvariant () const
{
  WH_ASSERT(this->point0 () != WH_NULL);
  WH_ASSERT(this->point1 () != WH_NULL);
  WH_ASSERT(this->point2 () != WH_NULL);
  WH_ASSERT(this->point0 () != this->point1 ());
  WH_ASSERT(this->point0 () != this->point2 ());
  WH_ASSERT(this->point1 () != this->point2 ());
  WH_ASSERT(this->innerTetrahedron () != WH_NULL);
  WH_ASSERT(this->innerTetrahedron ()->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::INNER
	    || this->innerTetrahedron ()->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
  WH_ASSERT(this->outerTetrahedron () != WH_NULL);
  WH_ASSERT(this->outerTetrahedron ()->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::OUTER
	    || this->outerTetrahedron ()->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);

  return true;
}

bool WH_DLN3D_FaceTriangle_MG3D
::assureInvariant () const
{
  this->checkInvariant ();

  return true;
}

WH_DLN3D_Point_MG3D* WH_DLN3D_FaceTriangle_MG3D
::point0 () const
{
  return _point0;
}

WH_DLN3D_Point_MG3D* WH_DLN3D_FaceTriangle_MG3D
::point1 () const
{
  return _point1;
}

WH_DLN3D_Point_MG3D* WH_DLN3D_FaceTriangle_MG3D
::point2 () const
{
  return _point2;
}

WH_DLN3D_Tetrahedron_MG3D* WH_DLN3D_FaceTriangle_MG3D
::innerTetrahedron () const
{
  return _innerTetrahedron;
}
 
WH_DLN3D_Tetrahedron_MG3D* WH_DLN3D_FaceTriangle_MG3D
::outerTetrahedron () const
{
  return _outerTetrahedron;
}

WH_TPL3D_Face_A* WH_DLN3D_FaceTriangle_MG3D
::commonFace () const
{
  WH_TPL3D_Face_A* result = WH_NULL;

  WH_MG3D_Node* node0 = _point0->node ();
  WH_MG3D_Node* node1 = _point1->node ();
  WH_MG3D_Node* node2 = _point2->node ();
  if (node0 != WH_NULL
      && node1 != WH_NULL
      && node2 != WH_NULL) {
    result = WH_MG3D_Node::commonFace 
      (node0, node1, node2);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif
    
  return result;
}



/* class WH_DLN3D_Triangulator_MG3D */

WH_DLN3D_Triangulator_MG3D
::WH_DLN3D_Triangulator_MG3D 
(WH_MG3D_MeshGenerator* meshGenerator,
 WH_TPL3D_Volume_A* volume)
{
  /* PRE-CONDITION */
  WH_ASSERT(meshGenerator != WH_NULL);
  WH_ASSERT(volume != WH_NULL);

  _meshGenerator = meshGenerator;
  _volume = volume;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_DLN3D_Triangulator_MG3D
::~WH_DLN3D_Triangulator_MG3D ()
{
  WH_T_Delete (_faceTriangle_s);
}
  
bool WH_DLN3D_Triangulator_MG3D
::checkInvariant () const
{
  this->WH_DLN3D_Triangulator::checkInvariant ();
  
  WH_ASSERT(this->meshGenerator () != WH_NULL);
  WH_ASSERT(this->volume () != WH_NULL);

  return true;
}

bool WH_DLN3D_Triangulator_MG3D
::assureInvariant () const
{
  this->WH_DLN3D_Triangulator::assureInvariant ();
  
  WH_T_AssureInvariant (_faceTriangle_s);

  return true;
}

WH_MG3D_MeshGenerator* WH_DLN3D_Triangulator_MG3D
::meshGenerator () const
{
  return _meshGenerator;
}

WH_TPL3D_Volume_A* WH_DLN3D_Triangulator_MG3D
::volume () const
{
  return _volume;
}

const vector<WH_DLN3D_FaceTriangle_MG3D*>& WH_DLN3D_Triangulator_MG3D
::faceTriangle_s () const
{
  return _faceTriangle_s;
}

void WH_DLN3D_Triangulator_MG3D
::classifyInOutOfTetrahedronsRoughly ()
{
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*> (tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);
    
    if (tetraMg_i->inOutType () 
	!= WH_DLN3D_Tetrahedron_MG3D::UNDEFINED) continue;

    if (tetraMg_i->isDummy ()
	|| tetraMg_i->hasAnyOutsideVolumeNode ()) {
      if (tetraMg_i->hasAnyInsideVolumeNode ()) {
	tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
      } else {
	tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::OUTER);
      }
    } else {
      /* check in/out at center of gravity */

      WH_Vector3D p0 = tetraMg_i->point (0)->position ();
      WH_Vector3D p1 = tetraMg_i->point (1)->position ();
      WH_Vector3D p2 = tetraMg_i->point (2)->position ();
      WH_Vector3D p3 = tetraMg_i->point (3)->position ();
      WH_Vector3D center = (p0 + p1 + p2 + p3) / 4;
      
      WH_InOutChecker3D::ContainmentType flag 
	= _meshGenerator->inOutChecker ()->checkContainmentAt (center);
      switch (flag) {
      case WH_InOutChecker3D::IN:
	tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::INNER);
	break;
      case WH_InOutChecker3D::OUT:
      case WH_InOutChecker3D::ON:
	tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::OUTER);
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
  }
}

void WH_DLN3D_Triangulator_MG3D
::collectBoundaryFaceTriangles ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_faceTriangle_s.size () == 0);
  
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);
    
    if (tetraMg_i->inOutType () 
	== WH_DLN3D_Tetrahedron_MG3D::INNER) {
      
      for (int f = 0; f < 4; f++) {
	WH_DLN3D_Tetrahedron* neighbor 
	  = tetraMg_i->neighborAt (f);
	WH_DLN3D_Tetrahedron_MG3D* neighborMg 
	  = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(neighbor);
	WH_ASSERT(neighborMg != WH_NULL);

	if (neighborMg->inOutType () 
	    == WH_DLN3D_Tetrahedron_MG3D::OUTER) {
	  
	  WH_DLN3D_Point_MG3D* point0 
	    = tetraMg_i->pointMg (WH_Tetrahedron3D_A::faceVertexMap[f][0]);
	  WH_DLN3D_Point_MG3D* point1 
	    = tetraMg_i->pointMg (WH_Tetrahedron3D_A::faceVertexMap[f][1]);
	  WH_DLN3D_Point_MG3D* point2 
	    = tetraMg_i->pointMg (WH_Tetrahedron3D_A::faceVertexMap[f][2]);

	  WH_ASSERT(neighborMg->hasPoint (point0));
	  WH_ASSERT(neighborMg->hasPoint (point1));
	  WH_ASSERT(neighborMg->hasPoint (point2));
	  
	  WH_DLN3D_FaceTriangle_MG3D* tri 
	    = new WH_DLN3D_FaceTriangle_MG3D 
	    (point0, point1, point2, tetraMg_i, neighborMg);
	  WH_ASSERT(tri != WH_NULL);

	  _faceTriangle_s.push_back (tri);
	}
      }
    }
  }
}

void WH_DLN3D_Triangulator_MG3D
::reclassifyInOutOfTetrahedrons ()
{
  for (vector<WH_DLN3D_FaceTriangle_MG3D*>::const_iterator 
	 i_tri = _faceTriangle_s.begin ();
       i_tri != _faceTriangle_s.end ();
       i_tri++) {
    WH_DLN3D_FaceTriangle_MG3D* tri_i = (*i_tri);

    WH_TPL3D_Face_A* commonFace = tri_i->commonFace ();
    if (commonFace == WH_NULL) {
      /* check in/out of sample points of <tri> */
      WH_Vector3D p0 = tri_i->point0 ()->position ();
      WH_Vector3D p1 = tri_i->point1 ()->position ();
      WH_Vector3D p2 = tri_i->point2 ()->position ();
      WH_Vector3D center = (p0 + p1 + p2) / 3;
      
      WH_InOutChecker3D::ContainmentType flag 
	= _meshGenerator->inOutChecker ()->checkContainmentAt (center);
      switch (flag) {
      case WH_InOutChecker3D::IN:
	tri_i->outerTetrahedron ()
	  ->setInOutType (WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
	break;
      case WH_InOutChecker3D::OUT:
	tri_i->innerTetrahedron ()
	  ->setInOutType (WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
	break;
      case WH_InOutChecker3D::ON:
	tri_i->innerTetrahedron ()
	  ->setInOutType (WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
	tri_i->outerTetrahedron ()
	  ->setInOutType (WH_DLN3D_Tetrahedron_MG3D::BOUNDARY);
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
  }
}

void WH_DLN3D_Triangulator_MG3D
::createDividedTetrahedronsOnEdge 
(WH_DLN3D_Tetrahedron* tetra,
 int edgeNumber,
 WH_DLN3D_Point* newPoint,
 vector<WH_DLN3D_Tetrahedron*>& newTetra_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 6);
  WH_ASSERT(newPoint != WH_NULL);

  WH_DLN3D_Point* p0 = tetra->point (0);
  WH_DLN3D_Point* p1 = tetra->point (1);
  WH_DLN3D_Point* p2 = tetra->point (2);
  WH_DLN3D_Point* p3 = tetra->point (3);

  vector<WH_DLN3D_Tetrahedron*> tetra_s;
  switch (edgeNumber) {
  case 0:
    tetra_s.push_back (this->createTetrahedron (newPoint, p1, p2, p3));
    tetra_s.push_back (this->createTetrahedron (p0, newPoint, p2, p3));
    break;
  case 1:
    tetra_s.push_back (this->createTetrahedron (p0, p1, newPoint, p3));
    tetra_s.push_back (this->createTetrahedron (newPoint, p2, p0, p3));
    break;
  case 2:
    tetra_s.push_back (this->createTetrahedron (p0, p1, newPoint, p3));
    tetra_s.push_back (this->createTetrahedron (newPoint, p1, p2, p3));
    break;
  case 3:
    tetra_s.push_back (this->createTetrahedron (p0, p1, p2, newPoint));
    tetra_s.push_back (this->createTetrahedron (newPoint, p1, p2, p3));
    break;
  case 4:
    tetra_s.push_back (this->createTetrahedron (p0, p1, p2, newPoint));
    tetra_s.push_back (this->createTetrahedron (p0, newPoint, p2, p3));
    break;
  case 5:
    tetra_s.push_back (this->createTetrahedron (p0, p1, p2, newPoint));
    tetra_s.push_back (this->createTetrahedron (p0, p1, newPoint, p3));
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
  WH_ASSERT(tetra_s.size () == 2);

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = tetra_s.begin ();
       i_tetra != tetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (int iFace = 0; iFace < 4; iFace++) {
      WH_DLN3D_Tetrahedron* neighbor = tetra->neighborAt (iFace);
      if (neighbor != WH_NULL && neighbor->isNeighborOf (tetra_i)) {
	tetra_i->setNeighbor (neighbor);
      }
    }
    newTetra_s_IO.push_back (tetra_i);
  }
}

void WH_DLN3D_Triangulator_MG3D
::divideTetrahedronsIntersectingOnEdge 
(WH_DLN3D_Point* newPoint,
 WH_DLN3D_Point* edgePoint0,
 WH_DLN3D_Point* edgePoint1)
{
  /* PRE-CONDITION */
  WH_ASSERT(newPoint != WH_NULL);
  WH_ASSERT(edgePoint0 != WH_NULL);
  WH_ASSERT(edgePoint1 != WH_NULL);
#ifndef NDEBUG
  {
    WH_Segment3D segment (edgePoint0->position (), edgePoint1->position ());
    WH_ASSERT (segment.justContains (newPoint->position ()));
  }
#endif
  
  vector<WH_DLN3D_Tetrahedron*> oldTetra_s;
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    if (tetra_i->hasPoint (edgePoint0)
	&& tetra_i->hasPoint (edgePoint1)) {
      oldTetra_s.push_back (tetra_i);
    }
  }
  WH_ASSERT(3 <= oldTetra_s.size ());
  
  vector<WH_DLN3D_Tetrahedron*> newTetra_s;
  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = oldTetra_s.begin ();
       i_tetra != oldTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (int iEdge = 0; iEdge < 6; iEdge++) {
      WH_DLN3D_Point* point0 
	= tetra_i->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][0]);
      WH_DLN3D_Point* point1 
	= tetra_i->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][1]);
      if ((point0 == edgePoint0 && point1 == edgePoint1)
	  || (point1 == edgePoint0 && point0 == edgePoint1)) {
	this->createDividedTetrahedronsOnEdge 
	  (tetra_i, iEdge, newPoint,
	   newTetra_s);
	break;
      }
    }
  }
  WH_ASSERT(newTetra_s.size () == oldTetra_s.size () * 2);

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = oldTetra_s.begin ();
       i_tetra != oldTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);

    this->removeTetrahedron (tetra_i);
  }
  /* <oldTetra_s> are no longer valid */

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = newTetra_s.begin ();
       i_tetra != newTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	   j_tetra = newTetra_s.begin ();
	 j_tetra != newTetra_s.end ();
	 j_tetra++) {
      WH_DLN3D_Tetrahedron* tetra_j = (*j_tetra);
      
      if (tetra_j == tetra_i) continue;

      if (tetra_j->isNeighborOf (tetra_i)) {
	tetra_i->setNeighbor (tetra_j);
      }
    }  

    this->addTetrahedron (tetra_i);
  }  
}

void WH_DLN3D_Triangulator_MG3D
::createDividedTetrahedronsOnFace 
(WH_DLN3D_Tetrahedron* tetra,
 int faceNumber,
 WH_DLN3D_Point* newPoint,
 vector<WH_DLN3D_Tetrahedron*>& newTetra_s_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);
  WH_ASSERT(newPoint != WH_NULL);

  WH_DLN3D_Point* p0 = tetra->point (0);
  WH_DLN3D_Point* p1 = tetra->point (1);
  WH_DLN3D_Point* p2 = tetra->point (2);
  WH_DLN3D_Point* p3 = tetra->point (3);

  vector<WH_DLN3D_Tetrahedron*> tetra_s;
  switch (faceNumber) {
  case 0:
    tetra_s.push_back (this->createTetrahedron (p0, p1, p2, newPoint));
    tetra_s.push_back (this->createTetrahedron (p0, p2, p3, newPoint));
    tetra_s.push_back (this->createTetrahedron (p0, p3, p1, newPoint));
    break;
  case 1:
    tetra_s.push_back (this->createTetrahedron (p1, p2, p0, newPoint));
    tetra_s.push_back (this->createTetrahedron (p1, p3, p2, newPoint));
    tetra_s.push_back (this->createTetrahedron (p1, p0, p3, newPoint));
    break;
  case 2:
    tetra_s.push_back (this->createTetrahedron (p2, p0, p1, newPoint));
    tetra_s.push_back (this->createTetrahedron (p2, p1, p3, newPoint));
    tetra_s.push_back (this->createTetrahedron (p2, p3, p0, newPoint));
    break;
  case 3:
    tetra_s.push_back (this->createTetrahedron (newPoint, p0, p1, p3));
    tetra_s.push_back (this->createTetrahedron (newPoint, p1, p2, p3));
    tetra_s.push_back (this->createTetrahedron (newPoint, p2, p0, p3));
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
  WH_ASSERT(tetra_s.size () == 3);

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = tetra_s.begin ();
       i_tetra != tetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (int iFace = 0; iFace < 4; iFace++) {
      WH_DLN3D_Tetrahedron* neighbor = tetra->neighborAt (iFace);
      if (neighbor != WH_NULL && neighbor->isNeighborOf (tetra_i)) {
	tetra_i->setNeighbor (neighbor);
      }
    }
    newTetra_s_IO.push_back (tetra_i);
  }
}

void WH_DLN3D_Triangulator_MG3D
::divideTetrahedronsIntersectingOnFace 
(WH_DLN3D_Point* newPoint,
 WH_DLN3D_Point* facePoint0,
 WH_DLN3D_Point* facePoint1,
 WH_DLN3D_Point* facePoint2)
{
  /* PRE-CONDITION */
  WH_ASSERT(newPoint != WH_NULL);
  WH_ASSERT(facePoint0 != WH_NULL);
  WH_ASSERT(facePoint1 != WH_NULL);
  WH_ASSERT(facePoint2 != WH_NULL);
#ifndef NDEBUG
  {
    WH_Triangle3D tri 
      (facePoint0->position (), facePoint1->position (), facePoint2->position ());
    WH_ASSERT (tri.checkContainmentAt (newPoint->position ()) 
	       == WH_Triangle3D_A::IN);
  }
#endif

  vector<WH_DLN3D_Tetrahedron*> oldTetra_s;
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    if (tetra_i->hasPoint (facePoint0)
	&& tetra_i->hasPoint (facePoint1)
	&& tetra_i->hasPoint (facePoint2)) {
      oldTetra_s.push_back (tetra_i);
    }
  }
  WH_ASSERT(oldTetra_s.size () == 2);
  
  vector<WH_DLN3D_Tetrahedron*> newTetra_s;
  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = oldTetra_s.begin ();
       i_tetra != oldTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (int iVertex = 0; iVertex < 4; iVertex++) {
      if (tetra_i->point (iVertex) != facePoint0
	  && tetra_i->point (iVertex) != facePoint1
	  && tetra_i->point (iVertex) != facePoint2) {
	int iFace = iVertex;
	this->createDividedTetrahedronsOnFace 
	  (tetra_i, iFace, newPoint,
	   newTetra_s);
	break;
      }
    }
  }
  WH_ASSERT(newTetra_s.size () == oldTetra_s.size () * 3);

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = oldTetra_s.begin ();
       i_tetra != oldTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);

    this->removeTetrahedron (tetra_i);
  }
  /* <oldTetra_s> are no longer valid */

  for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = newTetra_s.begin ();
       i_tetra != newTetra_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    
    for (vector<WH_DLN3D_Tetrahedron*>::const_iterator 
	   j_tetra = newTetra_s.begin ();
	 j_tetra != newTetra_s.end ();
	 j_tetra++) {
      WH_DLN3D_Tetrahedron* tetra_j = (*j_tetra);
      
      if (tetra_j == tetra_i) continue;

      if (tetra_j->isNeighborOf (tetra_i)) {
	tetra_i->setNeighbor (tetra_j);
      }
    }  

    this->addTetrahedron (tetra_i);
  }  
}

WH_DLN3D_Point_MG3D* WH_DLN3D_Triangulator_MG3D
::createPointAtIntersectionWithEdge 
(WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg, 
 const WH_Vector3D& intersectionPoint)
{
  /* PRE-CONDITION */
  WH_ASSERT(obeSeg != WH_NULL);
  
  WH_MG3D_Node* newNode 
    = new WH_MG3D_Node (intersectionPoint);
  WH_ASSERT(newNode != WH_NULL);
  newNode->putOnEdge (obeSeg->edge (), false);
  WH_ASSERT(!_meshGenerator->hasNodeNear (newNode->position (), 0.0));
  _meshGenerator->addNode (newNode);
  
  WH_DLN3D_Point_MG3D* result = new WH_DLN3D_Point_MG3D 
    (newNode->position (), newNode);
  WH_ASSERT(result != WH_NULL);
  this->addPoint (result);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_DLN3D_Point_MG3D* WH_DLN3D_Triangulator_MG3D
::createPointAtIntersectionWithFace 
(WH_MG3D_OriginalBoundaryFaceTriangle* obfTri, 
 const WH_Vector3D& intersectionPoint)
{
  /* PRE-CONDITION */
  WH_ASSERT(obfTri != WH_NULL);
  
  WH_MG3D_Node* newNode 
    = new WH_MG3D_Node (intersectionPoint);
  WH_ASSERT(newNode != WH_NULL);
  newNode->putOnFace (obfTri->face (), false);
  WH_ASSERT(!_meshGenerator->hasNodeNear (newNode->position (), 0.0));
  _meshGenerator->addNode (newNode);
  
  WH_DLN3D_Point_MG3D* result = new WH_DLN3D_Point_MG3D 
    (newNode->position (), newNode);
  WH_ASSERT(result != WH_NULL);
  this->addPoint (result);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

bool WH_DLN3D_Triangulator_MG3D
::checkIntersection 
(WH_DLN3D_Tetrahedron_MG3D* tetra)
{
  /* PRE-CONDITION */
  WH_ASSERT(tetra != WH_NULL);

  WH_Tetrahedron3D shape 
    (tetra->point (0)->position (),
     tetra->point (1)->position (),
     tetra->point (2)->position (),
     tetra->point (3)->position ());

  vector<WH_MG3D_OriginalBoundaryEdgeSegment*> obeSeg_s;
  _meshGenerator->obeSegBucket ()
    ->getItemsWithin (shape.minRange (), shape.maxRange (),
		      obeSeg_s);

  for (int iFace = 0; iFace < 4; iFace++) {
    WH_Triangle3D tri = shape.faceTriangle (iFace);
    
    for (vector<WH_MG3D_OriginalBoundaryEdgeSegment*>::const_iterator 
	   i_obeSeg = obeSeg_s.begin ();
	 i_obeSeg != obeSeg_s.end ();
	 i_obeSeg++) {
      WH_MG3D_OriginalBoundaryEdgeSegment* obeSeg_i = (*i_obeSeg);
      
      WH_Segment3D seg = obeSeg_i->shape ();

      WH_Polygon3D_A::EdgeIntersectionData edgeData[3];
      WH_Vector3D intersectionPoint;
      WH_Triangle3D::WithSegmentIntersectionType flag = 
	tri.checkIntersectionWith
	(seg,
	 intersectionPoint, edgeData);
      
      switch (flag) {
      case WH_Polygon3D_A::NO_INTERSECTION_WITH_SEGMENT:
	break;
      case WH_Polygon3D_A::POINT_WITH_SEGMENT:
	if (!tri.hasVertexAt (intersectionPoint)) {
	  for (int e = 0; e < 3; e++) {
	    if (tri.edge (e).justContains (intersectionPoint)) {
	      int iEdge = WH_Tetrahedron3D_A::faceEdgeMap[iFace][e];

	      /* the boundary edge intersects at <intersectionPoint>
                 on edge <iEdge> */

	      WH_DLN3D_Point_MG3D* newPoint = 
		this->createPointAtIntersectionWithEdge 
		(obeSeg_i, intersectionPoint);
	      WH_DLN3D_Point* point0 
		= tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][0]);
	      WH_DLN3D_Point* point1 
		= tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][1]);
	      this->divideTetrahedronsIntersectingOnEdge 
		(newPoint, point0, point1);

	      return true;
	    }  
	  }

	  /* the boundary edge intersects at <intersectionPoint>
	     inside face <iFace> */
	  WH_DLN3D_Point_MG3D* newPoint = 
	    this->createPointAtIntersectionWithEdge 
	    (obeSeg_i, intersectionPoint);
	  WH_DLN3D_Point* point0 
	    = tetra->point (WH_Tetrahedron3D_A::faceVertexMap[iFace][0]);
	  WH_DLN3D_Point* point1 
	    = tetra->point (WH_Tetrahedron3D_A::faceVertexMap[iFace][1]);
	  WH_DLN3D_Point* point2 
	    = tetra->point (WH_Tetrahedron3D_A::faceVertexMap[iFace][2]);
	  this->divideTetrahedronsIntersectingOnFace 
	    (newPoint, point0, point1, point2);
	  
	  return true;
	}
	break;
      case WH_Polygon3D_A::PARALLEL_WITH_SEGMENT:
	break;
      case WH_Polygon3D_A::COINCIDENT_WITH_SEGMENT:
	for (int e = 0; e < 3; e++) {
	  if (edgeData[e].intersectionType
	      == WH_Polygon3D_A::EdgeIntersectionData::HAS_INTERSECTION
	      && edgeData[e].positionType
	      == WH_Polygon3D_A::EdgeIntersectionData::ON_EDGE) {
	    intersectionPoint = edgeData[e].intersectionPoint;
	    int iEdge = WH_Tetrahedron3D_A::faceEdgeMap[iFace][e];
	    
	    /* the boundary edge intersects at <intersectionPoint>
	       on edge <iEdge> */
	    WH_DLN3D_Point_MG3D* newPoint = 
	      this->createPointAtIntersectionWithEdge 
	      (obeSeg_i, intersectionPoint);
	    WH_DLN3D_Point* point0 
	      = tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][0]);
	    WH_DLN3D_Point* point1 
	      = tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][1]);
	    this->divideTetrahedronsIntersectingOnEdge 
	      (newPoint, point0, point1);

	    return true;
	  }
	}
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
  }

  vector<WH_MG3D_OriginalBoundaryFaceTriangle*> obfTri_s;
  _meshGenerator->obfTriBucket ()
    ->getItemsWithin (shape.minRange (), shape.maxRange (),
		      obfTri_s);

  for (int iEdge = 0; iEdge < 6; iEdge++) {
    WH_Segment3D segment = shape.edge (iEdge);
    
    for (vector<WH_MG3D_OriginalBoundaryFaceTriangle*>::const_iterator 
	   i_obfTri = obfTri_s.begin ();
	 i_obfTri != obfTri_s.end ();
	 i_obfTri++) {
      WH_MG3D_OriginalBoundaryFaceTriangle* obfTri_i = (*i_obfTri);
      
      WH_Triangle3D tri = obfTri_i->shape ();
  
      WH_Polygon3D_A::EdgeIntersectionData edgeData[3];
      WH_Vector3D intersectionPoint;
      WH_Polygon3D_A::WithSegmentIntersectionType flag = 
	tri.checkIntersectionWith
	(segment,
	 intersectionPoint, edgeData);
      
      if (flag == WH_Polygon3D_A::POINT_WITH_SEGMENT) {
	if (!segment.hasVertexAt (intersectionPoint)) {
	  
	  /* the boundary face intersects at <intersectionPoint> 
	     on edge <iEdge> */
	  WH_DLN3D_Point_MG3D* newPoint = 
	    this->createPointAtIntersectionWithFace 
	    (obfTri_i, intersectionPoint);
	  WH_DLN3D_Point* point0 
	    = tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][0]);
	  WH_DLN3D_Point* point1 
	    = tetra->point (WH_Tetrahedron3D_A::edgeVertexMap[iEdge][1]);
	  this->divideTetrahedronsIntersectingOnEdge 
	    (newPoint, point0, point1);
	  
	  return true;
	}
      }
    }
  }

  return false;
}

void WH_DLN3D_Triangulator_MG3D
::divideBoundaryTetrahedrons ()
{
  for (;;) {
    bool hasIntersection = false;

    for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	   i_tetra = _tetrahedron_s.begin ();
	 i_tetra != _tetrahedron_s.end ();
	 i_tetra++) {
      WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
      WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
	= dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
      WH_ASSERT(tetraMg_i != WH_NULL);
      
      if (tetraMg_i->inOutType () == WH_DLN3D_Tetrahedron_MG3D::BOUNDARY
	  || tetraMg_i->inOutType () == WH_DLN3D_Tetrahedron_MG3D::UNDEFINED) {
	if (this->checkIntersection (tetraMg_i)) {
	  hasIntersection = true;
	  break;
	}
      }
    }

    if (!hasIntersection) break;
  }

  /* <_faceTriangle_s> are no longer valid */
  WH_T_Delete (_faceTriangle_s);
  _faceTriangle_s.clear (); 

  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);
    
    /* check in/out at center of gravity */
    
    WH_Vector3D p0 = tetraMg_i->point (0)->position ();
    WH_Vector3D p1 = tetraMg_i->point (1)->position ();
    WH_Vector3D p2 = tetraMg_i->point (2)->position ();
    WH_Vector3D p3 = tetraMg_i->point (3)->position ();
    WH_Vector3D center = (p0 + p1 + p2 + p3) / 4;
    
    WH_InOutChecker3D::ContainmentType flag 
      = _meshGenerator->inOutChecker ()->checkContainmentAt (center);
    switch (flag) {
    case WH_InOutChecker3D::IN:
      {
	/* NEED TO REDEFINE : ???? */

	WH_Vector3D q0 = center * 0.1 + p0 * 0.9;
	WH_Vector3D q1 = center * 0.1 + p1 * 0.9;
	WH_Vector3D q2 = center * 0.1 + p2 * 0.9;
	WH_Vector3D q3 = center * 0.1 + p3 * 0.9;
	if (_meshGenerator->inOutChecker ()
	    ->checkContainmentAt (q0) == WH_InOutChecker3D::IN
	    && _meshGenerator->inOutChecker ()
	    ->checkContainmentAt (q1) == WH_InOutChecker3D::IN
	    && _meshGenerator->inOutChecker ()
	    ->checkContainmentAt (q2) == WH_InOutChecker3D::IN
	    && _meshGenerator->inOutChecker ()
	    ->checkContainmentAt (q3) == WH_InOutChecker3D::IN) {
	  tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::INNER);
	} else {
	  tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::OUTER);
	}
      }
      break;
    case WH_InOutChecker3D::OUT:
    case WH_InOutChecker3D::ON:
      tetraMg_i->setInOutType (WH_DLN3D_Tetrahedron_MG3D::OUTER);
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }
}

void WH_DLN3D_Triangulator_MG3D
::doPostProcess ()
{
  this->classifyInOutOfTetrahedronsRoughly ();
  this->collectBoundaryFaceTriangles ();
  this->reclassifyInOutOfTetrahedrons ();

  this->divideBoundaryTetrahedrons ();

#ifndef NDEBUG
  for (list<WH_DLN3D_Tetrahedron*>::const_iterator 
	 i_tetra = _tetrahedron_s.begin ();
       i_tetra != _tetrahedron_s.end ();
       i_tetra++) {
    WH_DLN3D_Tetrahedron* tetra_i = (*i_tetra);
    WH_DLN3D_Tetrahedron_MG3D* tetraMg_i 
      = dynamic_cast<WH_DLN3D_Tetrahedron_MG3D*>(tetra_i);
    WH_ASSERT(tetraMg_i != WH_NULL);
    
    WH_ASSERT (tetraMg_i->inOutType () == WH_DLN3D_Tetrahedron_MG3D::INNER
	       || tetraMg_i->inOutType () == WH_DLN3D_Tetrahedron_MG3D::OUTER);
  }
#endif

  this->reorderTetrahedron ();
}

WH_DLN3D_Tetrahedron* WH_DLN3D_Triangulator_MG3D
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
    = new WH_DLN3D_Tetrahedron_MG3D 
    ((WH_DLN3D_Point_MG3D*)point0, 
     (WH_DLN3D_Point_MG3D*)point1, 
     (WH_DLN3D_Point_MG3D*)point2, 
     (WH_DLN3D_Point_MG3D*)point3);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}



