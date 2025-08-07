/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* mg3d_base.cc : 3-D tetrahedron mesh generation */

#include "mg3d_base.h"



/* class WH_MG3D_Node */

WH_MG3D_Node
::WH_MG3D_Node 
(const WH_Vector3D& position,
 bool isFirstOrder) 
  : _position (position)
{
  _id = WH_NO_INDEX;
  _isFirstOrder = isFirstOrder;
  _topologyType = UNDEFINED_TOPOLOGY;
  _vertex = WH_NULL;
  _edge = WH_NULL;
  _face = WH_NULL;
  _volume = WH_NULL;
  _isJustOnTopologicalEntity = false;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_Node
::~WH_MG3D_Node () 
{
}

bool WH_MG3D_Node
::checkInvariant () const
{
  switch (this->topologyType ()) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT(this->vertex () == WH_NULL);
    WH_ASSERT(this->edge () == WH_NULL);
    WH_ASSERT(this->face () == WH_NULL);
    WH_ASSERT(this->volume () == WH_NULL);
    break;
  case ON_VERTEX:
    WH_ASSERT(this->vertex () != WH_NULL);
    WH_ASSERT(this->edge () == WH_NULL);
    WH_ASSERT(this->face () == WH_NULL);
    WH_ASSERT(this->volume () == WH_NULL);
    break;
  case ON_EDGE:
    WH_ASSERT(this->vertex () == WH_NULL);
    WH_ASSERT(this->edge () != WH_NULL);
    WH_ASSERT(this->face () == WH_NULL);
    WH_ASSERT(this->volume () == WH_NULL);
    break;
  case ON_FACE:
    WH_ASSERT(this->vertex () == WH_NULL);
    WH_ASSERT(this->edge () == WH_NULL);
    WH_ASSERT(this->face () != WH_NULL);
    WH_ASSERT(this->volume () == WH_NULL);
    break;
  case INSIDE_VOLUME:
    WH_ASSERT(this->vertex () == WH_NULL);
    WH_ASSERT(this->edge () == WH_NULL);
    WH_ASSERT(this->face () == WH_NULL);
    WH_ASSERT(this->volume () != WH_NULL);
    break;
  case OUTSIDE_VOLUME:
    WH_ASSERT(this->vertex () == WH_NULL);
    WH_ASSERT(this->edge () == WH_NULL);
    WH_ASSERT(this->face () == WH_NULL);
    WH_ASSERT(this->volume () == WH_NULL);
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }

  if (!this->isFirstOrder ()) {
    WH_ASSERT(this->topologyType () != ON_VERTEX);
    WH_ASSERT(this->topologyType () != OUTSIDE_VOLUME);
  }

  return true;
}

bool WH_MG3D_Node
::assureInvariant () const
{
  this->checkInvariant ();

  if (this->isJustOnTopologicalEntity ()) {
    switch (this->topologyType ()) {
    case UNDEFINED_TOPOLOGY:
    case OUTSIDE_VOLUME:
      WH_ASSERT_NO_REACH;
      break;
    case ON_VERTEX:
      WH_ASSERT(WH_eq (this->vertex ()->point (), this->position ()));
      break;
    case ON_EDGE:
      WH_ASSERT(this->edge ()->curve ()->contains (this->position ()));
      break;
    case ON_FACE:
      WH_ASSERT(this->face ()->surface ()->contains (this->position ()));
      break;
    case INSIDE_VOLUME:
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  } else {
    switch (this->topologyType ()) {
    case UNDEFINED_TOPOLOGY:
    case ON_EDGE:
    case ON_FACE:
    case OUTSIDE_VOLUME:
      break;
    case ON_VERTEX:
    case INSIDE_VOLUME:
      WH_ASSERT_NO_REACH;
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }
  }

  return true;
}

WH_TPL3D_Face_A* WH_MG3D_Node
::commonFace 
(WH_MG3D_Node* node0, 
 WH_MG3D_Node* node1, 
 WH_MG3D_Node* node2)
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node2 != WH_NULL);

  /* WARNING : can these nodes have more than one common face ? */

  WH_TPL3D_Face_A* result = WH_NULL;

  if (node0->topologyType () != ON_VERTEX
      && node0->topologyType () != ON_EDGE
      && node0->topologyType () != ON_FACE) {
    return result;
  }
  if (node1->topologyType () != ON_VERTEX
      && node1->topologyType () != ON_EDGE
      && node1->topologyType () != ON_FACE) {
    return result;
  }
  if (node2->topologyType () != ON_VERTEX
      && node2->topologyType () != ON_EDGE
      && node2->topologyType () != ON_FACE) {
    return result;
  }

  if (node0->topologyType () == ON_FACE) {
    WH_TPL3D_Face_A* face = node0->face ();
    if (node1->hasFace (face) && node2->hasFace (face)) {
      result = face;
    }
  } else if (node1->topologyType () == ON_FACE) {
    WH_TPL3D_Face_A* face = node1->face ();
    if (node2->hasFace (face) && node0->hasFace (face)) {
      result = face;
    }
  } else if (node2->topologyType () == ON_FACE) {
    WH_TPL3D_Face_A* face = node2->face ();
    if (node0->hasFace (face) && node1->hasFace (face)) {
      result = face;
    }
  } else {
    vector<WH_TPL3D_Face_A*> face_s;
    node0->getFaces 
      (face_s);
    for (vector<WH_TPL3D_Face_A*>::const_iterator 
	   i_face = face_s.begin ();
	 i_face != face_s.end ();
	 i_face++) {
      WH_TPL3D_Face_A* face_i = (*i_face);
      if (node1->hasFace (face_i) && node2->hasFace (face_i)) {
	result = face_i;
	break;
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

WH_TPL3D_Face_A* WH_MG3D_Node
::commonFace 
(WH_MG3D_Node* node0, 
 WH_MG3D_Node* node1)
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  /* there shouldn`t be any common edge between these nodes */

  /* WARNING : can these nodes have more than one common face ? */

  WH_TPL3D_Face_A* result = WH_NULL;

  if (node0->topologyType () != ON_VERTEX
      && node0->topologyType () != ON_EDGE
      && node0->topologyType () != ON_FACE) {
    return result;
  }
  if (node1->topologyType () != ON_VERTEX
      && node1->topologyType () != ON_EDGE
      && node1->topologyType () != ON_FACE) {
    return result;
  }

  if (node0->topologyType () == ON_FACE) {
    WH_TPL3D_Face_A* face = node0->face ();
    if (node1->hasFace (face)) {
      result = face;
    }
  } else if (node1->topologyType () == ON_FACE) {
    WH_TPL3D_Face_A* face = node1->face ();
    if (node0->hasFace (face)) {
      result = face;
    }
  } else {
    vector<WH_TPL3D_Face_A*> face_s;
    node0->getFaces 
      (face_s);
    for (vector<WH_TPL3D_Face_A*>::const_iterator 
	   i_face = face_s.begin ();
	 i_face != face_s.end ();
	 i_face++) {
      WH_TPL3D_Face_A* face_i = (*i_face);
      if (node1->hasFace (face_i)) {
	result = face_i;
	break;
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

WH_TPL3D_Edge_A* WH_MG3D_Node
::commonEdge 
(WH_MG3D_Node* node0, 
 WH_MG3D_Node* node1)
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);

  /* WARNING : can these nodes have more than one common edge ? */

  WH_TPL3D_Edge_A* result = WH_NULL;

  if (node0->topologyType () != ON_VERTEX
      && node0->topologyType () != ON_EDGE) {
    return result;
  }
  if (node1->topologyType () != ON_VERTEX
      && node1->topologyType () != ON_EDGE) {
    return result;
  }

  if (node0->topologyType () == ON_EDGE) {
    WH_TPL3D_Edge_A* edge = node0->edge ();
    if (node1->hasEdge (edge)) {
      result = edge;
    }
  } else if (node1->topologyType () == ON_EDGE) {
    WH_TPL3D_Edge_A* edge = node1->edge ();
    if (node0->hasEdge (edge)) {
      result = edge;
    }
  } else {
    vector<WH_TPL3D_Edge_A*> edge_s;
    node0->getEdges 
      (edge_s);
    for (vector<WH_TPL3D_Edge_A*>::const_iterator 
	   i_edge = edge_s.begin ();
	 i_edge != edge_s.end ();
	 i_edge++) {
      WH_TPL3D_Edge_A* edge_i = (*i_edge);
      if (node1->hasEdge (edge_i)) {
	result = edge_i;
	break;
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

void WH_MG3D_Node
::putOnVertex 
(WH_TPL3D_Vertex_A* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->topologyType () == UNDEFINED_TOPOLOGY);
  WH_ASSERT(vertex != WH_NULL);
  WH_ASSERT(WH_eq (vertex->point (), this->position ()));
  
  _topologyType = ON_VERTEX;
  _vertex = vertex;
  _isJustOnTopologicalEntity = true;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->topologyType () == ON_VERTEX);
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Node
::putOnEdge 
(WH_TPL3D_Edge_A* edge,
 bool isJustOnTopologicalEntity)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->topologyType () == UNDEFINED_TOPOLOGY);
  WH_ASSERT(edge != WH_NULL);
#ifndef NDEBUG
  if (isJustOnTopologicalEntity) {
    WH_ASSERT(edge->curve ()->contains (this->position ()));
  }
#endif
  
  _topologyType = ON_EDGE;
  _edge = edge;
  _isJustOnTopologicalEntity = isJustOnTopologicalEntity;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->topologyType () == ON_EDGE);
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Node
::putOnFace 
(WH_TPL3D_Face_A* face,
 bool isJustOnTopologicalEntity)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->topologyType () == UNDEFINED_TOPOLOGY);
  WH_ASSERT(face != WH_NULL);
#ifndef NDEBUG
  if (isJustOnTopologicalEntity) {
    WH_ASSERT(face->surface ()->contains (this->position ()));
  }
#endif
  
  _topologyType = ON_FACE;
  _face = face;
  _isJustOnTopologicalEntity = isJustOnTopologicalEntity;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->topologyType () == ON_FACE);
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Node
::putInsideVolume 
(WH_TPL3D_Volume_A* volume)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->topologyType () == UNDEFINED_TOPOLOGY);
  WH_ASSERT(volume != WH_NULL);
  
  _topologyType = INSIDE_VOLUME;
  _volume = volume;
  _isJustOnTopologicalEntity = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->topologyType () == INSIDE_VOLUME);
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Node
::putOutsideVolume ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->topologyType () == UNDEFINED_TOPOLOGY);
  
  _topologyType = OUTSIDE_VOLUME;
  _isJustOnTopologicalEntity = false;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->topologyType () == OUTSIDE_VOLUME);
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Node
::setPosition (const WH_Vector3D& position)
{
  _position = position;
}

void WH_MG3D_Node
::setId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id);

  _id = id;
}

void WH_MG3D_Node
::projectOnTopologicalEntity ()
{
  switch (_topologyType) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT_NO_REACH;
    break;
  case ON_EDGE:
    {
      WH_ASSERT(_edge != WH_NULL);
      
      /* NEED TO REDEFINE */
      /* if the projected point exceed the edge ??? */
      double parameter 
	= _edge->curve ()->parameterAt (_position);
      _position = _edge->curve ()->positionAt (parameter);
    }
    break;
  case ON_FACE:
    {
      WH_ASSERT(_face != WH_NULL);
      
      /* NEED TO REDEFINE */
      /* if the projected point exceed the face ??? */
      WH_Vector2D parameter 
	= _face->surface ()->parameterAt (_position);
      _position = _face->surface ()->positionAt (parameter);
    }
    break;
  case ON_VERTEX:
  case INSIDE_VOLUME:
  case OUTSIDE_VOLUME:
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
}

WH_Vector3D WH_MG3D_Node
::position () const
{
  return _position;
}
  
int WH_MG3D_Node
::id () const
{
  return _id;
}

bool WH_MG3D_Node
::isFirstOrder () const
{
  return _isFirstOrder;
}

WH_MG3D_Node::TopologyType WH_MG3D_Node
::topologyType () const
{
  return _topologyType;
}

WH_TPL3D_Vertex_A* WH_MG3D_Node
::vertex () const
{
  return _vertex;
}

WH_TPL3D_Edge_A* WH_MG3D_Node
::edge () const
{
  return _edge;
}

WH_TPL3D_Face_A* WH_MG3D_Node
::face () const
{
  return _face;
}

WH_TPL3D_Volume_A* WH_MG3D_Node
::volume () const
{
  return _volume;
}

bool WH_MG3D_Node
::isJustOnTopologicalEntity () const
{
  return _isJustOnTopologicalEntity;
}

bool WH_MG3D_Node
::hasFace (WH_TPL3D_Face_A* face) const
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);

  bool result = false;

  switch (_topologyType) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT_NO_REACH;
    break;
  case ON_VERTEX:
    {
      WH_ASSERT(_vertex != WH_NULL);
      
      const vector<WH_TPL3D_Face_A*>& connectedFace_s
	= _vertex->connectedFace_s ();
      vector<WH_TPL3D_Face_A*>::const_iterator i_faceFound 
	= find (connectedFace_s.begin (), connectedFace_s.end (),
		face);
      result = (i_faceFound != connectedFace_s.end ());
    }
    break;
  case ON_EDGE:
    {
      WH_ASSERT(_edge != WH_NULL);
      
      const vector<WH_TPL3D_Face_A*>& connectedFace_s
	= _edge->connectedFace_s ();
      vector<WH_TPL3D_Face_A*>::const_iterator i_faceFound 
	= find (connectedFace_s.begin (), connectedFace_s.end (),
		face);
      result = (i_faceFound != connectedFace_s.end ());
    }
    break;
  case ON_FACE:
    {
      WH_ASSERT(_face != WH_NULL);
      
      result = (_face == face);
    }
    break;
  case INSIDE_VOLUME:
  case OUTSIDE_VOLUME:
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }

  return result;
}

void WH_MG3D_Node
::getFaces
(vector<WH_TPL3D_Face_A*>& face_s_OUT) const
{
  face_s_OUT.clear ();

  switch (_topologyType) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT_NO_REACH;
    break;
  case ON_VERTEX:
    {
      WH_ASSERT(_vertex != WH_NULL);
      
      const vector<WH_TPL3D_Face_A*>& connectedFace_s
	= _vertex->connectedFace_s ();
      copy (connectedFace_s.begin (), connectedFace_s.end (),
	    back_inserter (face_s_OUT));
    }
    break;
  case ON_EDGE:
    {
      WH_ASSERT(_edge != WH_NULL);
      
      const vector<WH_TPL3D_Face_A*>& connectedFace_s
	= _edge->connectedFace_s ();
      copy (connectedFace_s.begin (), connectedFace_s.end (),
	    back_inserter (face_s_OUT));
    }
    break;
  case ON_FACE:
    {
      WH_ASSERT(_face != WH_NULL);
      
      face_s_OUT.push_back (_face);
    }
    break;
  case INSIDE_VOLUME:
  case OUTSIDE_VOLUME:
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
}

bool WH_MG3D_Node
::hasEdge (WH_TPL3D_Edge_A* edge) const
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  bool result = false;

  switch (_topologyType) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT_NO_REACH;
    break;
  case ON_VERTEX:
    {
      WH_ASSERT(_vertex != WH_NULL);
      
      const vector<WH_TPL3D_Edge_A*>& connectedEdge_s
	= _vertex->connectedEdge_s ();
      vector<WH_TPL3D_Edge_A*>::const_iterator i_edgeFound 
	= find (connectedEdge_s.begin (), connectedEdge_s.end (),
		edge);
      result = (i_edgeFound != connectedEdge_s.end ());
    }
    break;
  case ON_EDGE:
    {
      WH_ASSERT(_edge != WH_NULL);
      
      result = (_edge == edge);
    }
    break;
  case ON_FACE:
  case INSIDE_VOLUME:
  case OUTSIDE_VOLUME:
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }

  return result;
}

void WH_MG3D_Node
::getEdges
(vector<WH_TPL3D_Edge_A*>& edge_s_OUT) const
{
  edge_s_OUT.clear ();

  switch (_topologyType) {
  case UNDEFINED_TOPOLOGY:
    WH_ASSERT_NO_REACH;
    break;
  case ON_VERTEX:
    {
      WH_ASSERT(_vertex != WH_NULL);
      
      const vector<WH_TPL3D_Edge_A*>& connectedEdge_s
	= _vertex->connectedEdge_s ();
      copy (connectedEdge_s.begin (), connectedEdge_s.end (),
	    back_inserter (edge_s_OUT));
    }
    break;
  case ON_EDGE:
    {
      WH_ASSERT(_edge != WH_NULL);
      
      edge_s_OUT.push_back (_edge);
    }
    break;
  case ON_FACE:
  case INSIDE_VOLUME:
  case OUTSIDE_VOLUME:
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }
}
  


/* class WH_MG3D_OriginalBoundaryEdgeSegment */

WH_MG3D_OriginalBoundaryEdgeSegment
::WH_MG3D_OriginalBoundaryEdgeSegment 
(WH_MG3D_Node* node0,
 WH_MG3D_Node* node1,
 WH_TPL3D_Edge_A* edge) 
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node0 != node1);
  WH_ASSERT(edge != WH_NULL);

  _node0 = node0;
  _node1 = node1;
  _edge = edge;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_OriginalBoundaryEdgeSegment
::~WH_MG3D_OriginalBoundaryEdgeSegment () 
{
}

bool WH_MG3D_OriginalBoundaryEdgeSegment
::checkInvariant () const
{
  WH_ASSERT(this->node0 () != WH_NULL);
  WH_ASSERT(this->node1 () != WH_NULL);
  WH_ASSERT(this->node0 () != this->node1 ());
  WH_ASSERT(this->edge () != WH_NULL);

  return true;
}

bool WH_MG3D_OriginalBoundaryEdgeSegment
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(WH_le (this->minRange (), this->maxRange ()));

  return true;
}

WH_MG3D_Node* WH_MG3D_OriginalBoundaryEdgeSegment
::node0 () const
{
  return _node0;
}

WH_MG3D_Node* WH_MG3D_OriginalBoundaryEdgeSegment
::node1 () const
{
  return _node1;
}

WH_TPL3D_Edge_A* WH_MG3D_OriginalBoundaryEdgeSegment
::edge () const
{
  return _edge;
}

WH_Vector3D WH_MG3D_OriginalBoundaryEdgeSegment
::minRange () const
{
  return WH_min (_node0->position (), _node1->position ());
}

WH_Vector3D WH_MG3D_OriginalBoundaryEdgeSegment
::maxRange () const
{
  return WH_max (_node0->position (), _node1->position ());
}

WH_Segment3D WH_MG3D_OriginalBoundaryEdgeSegment
::shape () const
{
  return WH_Segment3D (_node0->position (), _node1->position ());
}



/* class WH_MG3D_OriginalBoundaryFaceTriangle */

WH_MG3D_OriginalBoundaryFaceTriangle
::WH_MG3D_OriginalBoundaryFaceTriangle 
(WH_MG3D_Node* node0,
 WH_MG3D_Node* node1,
 WH_MG3D_Node* node2,
 WH_TPL3D_Face_A* face) 
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node2 != WH_NULL);
  WH_ASSERT(node0 != node1);
  WH_ASSERT(node0 != node2);
  WH_ASSERT(node1 != node2);
  WH_ASSERT(face != WH_NULL);

  _node0 = node0;
  _node1 = node1;
  _node2 = node2;
  _face = face;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_OriginalBoundaryFaceTriangle
::~WH_MG3D_OriginalBoundaryFaceTriangle () 
{
}

bool WH_MG3D_OriginalBoundaryFaceTriangle
::checkInvariant () const
{
  WH_ASSERT(this->node0 () != WH_NULL);
  WH_ASSERT(this->node1 () != WH_NULL);
  WH_ASSERT(this->node2 () != WH_NULL);
  WH_ASSERT(this->node0 () != this->node1 ());
  WH_ASSERT(this->node0 () != this->node2 ());
  WH_ASSERT(this->node1 () != this->node2 ());
  WH_ASSERT(this->face () != WH_NULL);

  return true;
}

bool WH_MG3D_OriginalBoundaryFaceTriangle
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(WH_le (this->minRange (), this->maxRange ()));

  return true;
}

WH_MG3D_Node* WH_MG3D_OriginalBoundaryFaceTriangle
::node0 () const
{
  return _node0;
}

WH_MG3D_Node* WH_MG3D_OriginalBoundaryFaceTriangle
::node1 () const
{
  return _node1;
}

WH_MG3D_Node* WH_MG3D_OriginalBoundaryFaceTriangle
::node2 () const
{
  return _node2;
}

WH_TPL3D_Face_A* WH_MG3D_OriginalBoundaryFaceTriangle
::face () const
{
  return _face;
}

WH_Vector3D WH_MG3D_OriginalBoundaryFaceTriangle
::minRange () const
{
  WH_Vector3D result = _node0->position ();
  result = WH_min (result, _node1->position ());
  result = WH_min (result, _node2->position ());
  return result;
}

WH_Vector3D WH_MG3D_OriginalBoundaryFaceTriangle
::maxRange () const
{
  WH_Vector3D result = _node0->position ();
  result = WH_max (result, _node1->position ());
  result = WH_max (result, _node2->position ());
  return result;
}

WH_Triangle3D WH_MG3D_OriginalBoundaryFaceTriangle
::shape () const
{
  return WH_Triangle3D (_node0->position (), 
			_node1->position (), 
			_node2->position ());
}



/* class WH_MG3D_FinalBoundaryFaceTriangle */

const int WH_MG3D_FinalBoundaryFaceTriangle
::nFirstOrderNodes = 3;
const int WH_MG3D_FinalBoundaryFaceTriangle
::nSecondOrderNodes = 3;

WH_MG3D_FinalBoundaryFaceTriangle
::WH_MG3D_FinalBoundaryFaceTriangle 
(WH_MG3D_Node* node0,
 WH_MG3D_Node* node1,
 WH_MG3D_Node* node2,
 WH_MG3D_Tetrahedron* tetrahedron,
 int tetrahedronFaceId,
 WH_TPL3D_Face_A* face) 
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node2 != WH_NULL);
  WH_ASSERT(node0 != node1);
  WH_ASSERT(node0 != node2);
  WH_ASSERT(node1 != node2);
  WH_ASSERT(node0->isFirstOrder ());
  WH_ASSERT(node1->isFirstOrder ());
  WH_ASSERT(node2->isFirstOrder ());
  WH_ASSERT(tetrahedron != WH_NULL);
  WH_ASSERT(0 <= tetrahedronFaceId);
  WH_ASSERT(tetrahedronFaceId < WH_Tetrahedron3D_A::nTetrahedronFaces);
  WH_ASSERT(face != WH_NULL || face == WH_NULL);

  _firstOrderNodes[0] = node0;
  _firstOrderNodes[1] = node1;
  _firstOrderNodes[2] = node2;

  _tetrahedron = tetrahedron;
  _tetrahedronFaceId = tetrahedronFaceId;

  _face = face;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_FinalBoundaryFaceTriangle
::~WH_MG3D_FinalBoundaryFaceTriangle () 
{
}

bool WH_MG3D_FinalBoundaryFaceTriangle
::checkInvariant () const
{
  WH_ASSERT(nFirstOrderNodes == WH_Triangle3D_A::nTriangleVertexs);
  WH_ASSERT(nSecondOrderNodes == WH_Triangle3D_A::nTriangleEdges);

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node = this->firstOrderNode (iNode);
    WH_ASSERT(node != WH_NULL);
  }

  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(node != WH_NULL);
    }
  } else {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(node == WH_NULL);
    }
  }

  WH_ASSERT(this->tetrahedron () != WH_NULL);
  WH_ASSERT(0 <= this->tetrahedronFaceId ());
  WH_ASSERT(this->tetrahedronFaceId () 
	    < WH_Tetrahedron3D_A::nTetrahedronFaces);
  
  WH_ASSERT(this->face () != WH_NULL 
	  || this->face () == WH_NULL);

  return true;
}

bool WH_MG3D_FinalBoundaryFaceTriangle
::assureInvariant () const
{
  this->checkInvariant ();

  int faceId = this->tetrahedronFaceId ();
  WH_MG3D_Tetrahedron* tetra = this->tetrahedron (); 
  WH_ASSERT(tetra->neighbor (faceId) == WH_NULL);

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node_i = this->firstOrderNode (iNode);
    for (int jNode = iNode + 1; jNode < nFirstOrderNodes; jNode++) {
      WH_MG3D_Node* node_j = this->firstOrderNode (jNode);
      WH_ASSERT(node_j != node_i);
    }
  }

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node = this->firstOrderNode (iNode);
    WH_ASSERT(node->isFirstOrder ());
    WH_ASSERT(this->hasFirstOrderNode (node));
    WH_ASSERT(tetra->hasFirstOrderNode (node));
    WH_ASSERT(tetra->firstOrderNodeOnFace 
	      (faceId, iNode) == node);
  }
  
  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node_i = this->secondOrderNode (iNode);
      for (int jNode = iNode + 1; jNode < nSecondOrderNodes; jNode++) {
	WH_MG3D_Node* node_j = this->secondOrderNode (jNode);
	WH_ASSERT(node_j != node_i);
      }
    }

    WH_ASSERT(tetra->hasAnySecondOrderNode ()); 
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(!node->isFirstOrder ());
      WH_ASSERT(this->hasSecondOrderNode (node));
      WH_ASSERT(tetra->hasSecondOrderNode (node));
      WH_ASSERT(tetra->secondOrderNodeOnFace 
		(faceId, iNode) == node);
    }
  }

  if (this->face () != WH_NULL) {
    WH_TPL3D_Face_A* face = this->face ();
    WH_ASSERT(face != WH_NULL);
    
    for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->firstOrderNode (iNode);
      WH_ASSERT(node->topologyType () == WH_MG3D_Node::ON_VERTEX
	      || node->topologyType () == WH_MG3D_Node::ON_EDGE
	      || node->topologyType () == WH_MG3D_Node::ON_FACE);
      WH_ASSERT(node->hasFace (face));
    }
    
    if (this->hasAnySecondOrderNode ()) {
      for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
	WH_MG3D_Node* node = this->secondOrderNode (iNode);
	WH_ASSERT(node->topologyType () == WH_MG3D_Node::ON_EDGE
		  || node->topologyType () == WH_MG3D_Node::ON_FACE);
	WH_ASSERT(node->hasFace (face));
      }
    }
  }

  return true;
}

WH_MG3D_Node* WH_MG3D_FinalBoundaryFaceTriangle
::firstOrderNode (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nFirstOrderNodes);

  return _firstOrderNodes[index];
}

bool WH_MG3D_FinalBoundaryFaceTriangle
::hasFirstOrderNode (WH_MG3D_Node* node) const
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  
  bool result = false;

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    if (_firstOrderNodes[iNode] == node) {
      result = true;
      break;
    }
  }

  return result;
}

bool WH_MG3D_FinalBoundaryFaceTriangle
::hasAnySecondOrderNode () const
{
  WH_MG3D_Node* anyNode = _tetrahedron
    ->secondOrderNodeOnFace (_tetrahedronFaceId, 0);
  return (anyNode != WH_NULL);
}

WH_MG3D_Node* WH_MG3D_FinalBoundaryFaceTriangle
::secondOrderNode (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nSecondOrderNodes);

  return _tetrahedron
    ->secondOrderNodeOnFace (_tetrahedronFaceId, index);
}

bool WH_MG3D_FinalBoundaryFaceTriangle
::hasSecondOrderNode (WH_MG3D_Node* node) const
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  
  bool result = false;

  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      if (this->secondOrderNode (iNode) == node) {
	result = true;
	break;
      }
    }
  }
  
  return result;
}

WH_MG3D_Tetrahedron* WH_MG3D_FinalBoundaryFaceTriangle
::tetrahedron () const
{
  return _tetrahedron;
}

int WH_MG3D_FinalBoundaryFaceTriangle
::tetrahedronFaceId () const
{
  return _tetrahedronFaceId;
}

WH_TPL3D_Face_A* WH_MG3D_FinalBoundaryFaceTriangle
::face () const
{
  return _face;
} 



/* class WH_MG3D_Tetrahedron */

const int WH_MG3D_Tetrahedron
::nFirstOrderNodes = 4;
const int WH_MG3D_Tetrahedron
::nSecondOrderNodes = 6;

WH_MG3D_Tetrahedron
::WH_MG3D_Tetrahedron 
(WH_MG3D_Node* node0,
 WH_MG3D_Node* node1,
 WH_MG3D_Node* node2,
 WH_MG3D_Node* node3,
 WH_TPL3D_Volume_A* volume) 
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node2 != WH_NULL);
  WH_ASSERT(node3 != WH_NULL);
  WH_ASSERT(node0 != node1);
  WH_ASSERT(node0 != node2);
  WH_ASSERT(node0 != node3);
  WH_ASSERT(node1 != node2);
  WH_ASSERT(node1 != node3);
  WH_ASSERT(node2 != node3);
  WH_ASSERT(node0->isFirstOrder ());
  WH_ASSERT(node1->isFirstOrder ());
  WH_ASSERT(node2->isFirstOrder ());
  WH_ASSERT(node3->isFirstOrder ());
  WH_ASSERT(volume != WH_NULL);

  _firstOrderNodes[0] = node0;
  _firstOrderNodes[1] = node1;
  _firstOrderNodes[2] = node2;
  _firstOrderNodes[3] = node3;
  _secondOrderNodes[0] = WH_NULL;
  _secondOrderNodes[1] = WH_NULL;
  _secondOrderNodes[2] = WH_NULL;
  _secondOrderNodes[3] = WH_NULL;
  _secondOrderNodes[4] = WH_NULL;
  _secondOrderNodes[5] = WH_NULL;

  _neighbors[0] = WH_NULL;
  _neighbors[1] = WH_NULL;
  _neighbors[2] = WH_NULL;
  _neighbors[3] = WH_NULL;

  _volume = volume;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_MG3D_Tetrahedron
::~WH_MG3D_Tetrahedron () 
{
}

bool WH_MG3D_Tetrahedron
::checkInvariant () const
{
  WH_ASSERT(nFirstOrderNodes == WH_Tetrahedron3D_A::nTetrahedronVertexs);
  WH_ASSERT(nSecondOrderNodes == WH_Tetrahedron3D_A::nTetrahedronEdges);

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node = this->firstOrderNode (iNode);
    WH_ASSERT(node != WH_NULL);
  }

  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(node != WH_NULL);
    }
  } else {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(node == WH_NULL);
    }
  }

  for (int iFace = 0; 
       iFace < WH_Tetrahedron3D_A::nTetrahedronFaces; 
       iFace++) {
    WH_MG3D_Tetrahedron* neighbor = this->neighbor (iFace);
    WH_ASSERT(neighbor != WH_NULL || neighbor == WH_NULL);
  }
  
  WH_ASSERT(this->volume () != WH_NULL);

  return true;
}

bool WH_MG3D_Tetrahedron
::assureInvariant () const
{
  this->checkInvariant ();

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node_i = this->firstOrderNode (iNode);
    for (int jNode = iNode + 1; jNode < nFirstOrderNodes; jNode++) {
      WH_MG3D_Node* node_j = this->firstOrderNode (jNode);
      WH_ASSERT(node_j != node_i);
    }
  }

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    WH_MG3D_Node* node = this->firstOrderNode (iNode);
    WH_ASSERT(node->isFirstOrder ());
    WH_ASSERT(this->hasFirstOrderNode (node));
    WH_ASSERT(node->topologyType () == WH_MG3D_Node::ON_VERTEX
	      || node->topologyType () == WH_MG3D_Node::ON_EDGE
	      || node->topologyType () == WH_MG3D_Node::ON_FACE
	      || node->topologyType () == WH_MG3D_Node::INSIDE_VOLUME);
  }
  
  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node_i = this->secondOrderNode (iNode);
      for (int jNode = iNode + 1; jNode < nSecondOrderNodes; jNode++) {
	WH_MG3D_Node* node_j = this->secondOrderNode (jNode);
	WH_ASSERT(node_j != node_i);
      }
    }

    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      WH_MG3D_Node* node = this->secondOrderNode (iNode);
      WH_ASSERT(!node->isFirstOrder ());
      WH_ASSERT(this->hasSecondOrderNode (node));
      WH_ASSERT(node->topologyType () == WH_MG3D_Node::ON_EDGE
		|| node->topologyType () == WH_MG3D_Node::ON_FACE
		|| node->topologyType () == WH_MG3D_Node::INSIDE_VOLUME);
    }
  }

  for (int iFace = 0; 
       iFace < WH_Tetrahedron3D_A::nTetrahedronFaces; 
       iFace++) {
    WH_MG3D_Tetrahedron* neighbor = this->neighbor (iFace);
    if (neighbor == WH_NULL) continue;

    for (int iVertex = 0; 
	 iVertex < WH_Triangle3D_A::nTriangleVertexs;
	 iVertex++) {
      WH_MG3D_Node* node 
	= this->firstOrderNodeOnFace (iFace, iVertex);
      WH_ASSERT(neighbor->hasFirstOrderNode (node));
    }
    
    bool sameNodeIsFound = false;
    for (int jFace = 0; 
	 jFace < WH_Tetrahedron3D_A::nTetrahedronFaces; 
	 jFace++) {
      WH_MG3D_Tetrahedron* neighborOfNeighbor 
	= neighbor->neighbor (jFace);
      
      if (neighborOfNeighbor == this) {
	for (int iVertex = 0; 
	     iVertex < WH_Triangle3D_A::nTriangleVertexs;
	     iVertex++) {
	  WH_MG3D_Node* node 
	    = neighbor->firstOrderNodeOnFace (jFace, iVertex);
	  WH_ASSERT(this->hasFirstOrderNode (node));
	}
	
	sameNodeIsFound = true;
	break;
      }
    }
    WH_ASSERT(sameNodeIsFound);
  }

  return true;
}

void WH_MG3D_Tetrahedron
::setNeighbors 
(WH_MG3D_Tetrahedron* neighbor0,
 WH_MG3D_Tetrahedron* neighbor1,
 WH_MG3D_Tetrahedron* neighbor2,
 WH_MG3D_Tetrahedron* neighbor3)
{
  /* PRE-CONDITION */
  WH_ASSERT(neighbor0 != WH_NULL
	    || neighbor0 == WH_NULL);
  WH_ASSERT(neighbor1 != WH_NULL
	    || neighbor1 == WH_NULL);
  WH_ASSERT(neighbor2 != WH_NULL
	    || neighbor2 == WH_NULL);
  WH_ASSERT(neighbor3 != WH_NULL
	    || neighbor3 == WH_NULL);
  
  _neighbors[0] = neighbor0;
  _neighbors[1] = neighbor1;
  _neighbors[2] = neighbor2;
  _neighbors[3] = neighbor3;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

void WH_MG3D_Tetrahedron
::setSecondOrderNodes 
(WH_MG3D_Node* node0,
 WH_MG3D_Node* node1,
 WH_MG3D_Node* node2,
 WH_MG3D_Node* node3,
 WH_MG3D_Node* node4,
 WH_MG3D_Node* node5)
{
  /* PRE-CONDITION */
  WH_ASSERT(node0 != WH_NULL);
  WH_ASSERT(node1 != WH_NULL);
  WH_ASSERT(node2 != WH_NULL);
  WH_ASSERT(node3 != WH_NULL);
  WH_ASSERT(node4 != WH_NULL);
  WH_ASSERT(node5 != WH_NULL);
  WH_ASSERT(node0 != node1);
  WH_ASSERT(node0 != node2);
  WH_ASSERT(node0 != node3);
  WH_ASSERT(node0 != node4);
  WH_ASSERT(node0 != node5);
  WH_ASSERT(node1 != node2);
  WH_ASSERT(node1 != node3);
  WH_ASSERT(node1 != node4);
  WH_ASSERT(node1 != node5);
  WH_ASSERT(node2 != node3);
  WH_ASSERT(node2 != node4);
  WH_ASSERT(node2 != node5);
  WH_ASSERT(node3 != node4);
  WH_ASSERT(node3 != node5);
  WH_ASSERT(node4 != node5);
  WH_ASSERT(!node0->isFirstOrder ());
  WH_ASSERT(!node1->isFirstOrder ());
  WH_ASSERT(!node2->isFirstOrder ());
  WH_ASSERT(!node3->isFirstOrder ());
  WH_ASSERT(!node4->isFirstOrder ());
  WH_ASSERT(!node5->isFirstOrder ());

  _secondOrderNodes[0] = node0;
  _secondOrderNodes[1] = node1;
  _secondOrderNodes[2] = node2;
  _secondOrderNodes[3] = node3;
  _secondOrderNodes[4] = node4;
  _secondOrderNodes[5] = node5;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->hasAnySecondOrderNode ());
#endif
}

WH_MG3D_Node* WH_MG3D_Tetrahedron
::firstOrderNode (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nFirstOrderNodes);

  return _firstOrderNodes[index];
}

bool WH_MG3D_Tetrahedron
::hasFirstOrderNode (WH_MG3D_Node* node) const
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  
  bool result = false;

  for (int iNode = 0; iNode < nFirstOrderNodes; iNode++) {
    if (_firstOrderNodes[iNode] == node) {
      result = true;
      break;
    }
  }

  return result;
}

WH_MG3D_Node* WH_MG3D_Tetrahedron
::firstOrderNodeOnEdge 
(int edgeId, int vertexOnEdgeId) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId);
  WH_ASSERT(edgeId < WH_Tetrahedron3D_A::nTetrahedronEdges);
  WH_ASSERT(0 <= vertexOnEdgeId);
  WH_ASSERT(vertexOnEdgeId < 2);

  int index = WH_Tetrahedron3D_A::edgeVertexMap[edgeId][vertexOnEdgeId];
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nFirstOrderNodes);
  return _firstOrderNodes[index];
}
  
WH_MG3D_Node* WH_MG3D_Tetrahedron
::firstOrderNodeOnFace 
(int faceId, int vertexOnFaceId) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceId);
  WH_ASSERT(faceId < WH_Tetrahedron3D_A::nTetrahedronFaces);
  WH_ASSERT(0 <= vertexOnFaceId);
  WH_ASSERT(vertexOnFaceId < WH_Triangle3D_A::nTriangleVertexs);

  int index = WH_Tetrahedron3D_A::faceVertexMap[faceId][vertexOnFaceId];
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nFirstOrderNodes);
  return _firstOrderNodes[index];
}

bool WH_MG3D_Tetrahedron
::hasAnySecondOrderNode () const
{
  return (_secondOrderNodes[0] != WH_NULL);
}

WH_MG3D_Node* WH_MG3D_Tetrahedron
::secondOrderNode (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nSecondOrderNodes);

  return _secondOrderNodes[index];
}

bool WH_MG3D_Tetrahedron
::hasSecondOrderNode (WH_MG3D_Node* node) const
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  
  bool result = false;

  if (this->hasAnySecondOrderNode ()) {
    for (int iNode = 0; iNode < nSecondOrderNodes; iNode++) {
      if (_secondOrderNodes[iNode] == node) {
	result = true;
	break;
      }
    }
  }
  
  return result;
}

WH_MG3D_Node* WH_MG3D_Tetrahedron
::secondOrderNodeOnEdge 
(int edgeId) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeId);
  WH_ASSERT(edgeId < WH_Tetrahedron3D_A::nTetrahedronEdges);
  
  int index = edgeId;
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nSecondOrderNodes);
  return _secondOrderNodes[index];
}

WH_MG3D_Node* WH_MG3D_Tetrahedron
::secondOrderNodeOnFace 
(int faceId, int edgeOnFaceId) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceId);
  WH_ASSERT(faceId < WH_Tetrahedron3D_A::nTetrahedronFaces);
  WH_ASSERT(0 <= edgeOnFaceId);
  WH_ASSERT(edgeOnFaceId < WH_Triangle3D_A::nTriangleEdges);

  int index = WH_Tetrahedron3D_A::faceEdgeMap[faceId][edgeOnFaceId];
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < nSecondOrderNodes);
  return _secondOrderNodes[index];
}

WH_MG3D_Tetrahedron* WH_MG3D_Tetrahedron
::neighbor (int faceId) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceId);
  WH_ASSERT(faceId < WH_Tetrahedron3D_A::nTetrahedronFaces);
  
  return _neighbors[faceId];
}

WH_TPL3D_Volume_A* WH_MG3D_Tetrahedron
::volume () const
{
  return _volume;
}



