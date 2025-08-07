/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* poly_tpl3d.cc */
/* 3-D geometry model topology framework */
/* polygonal representation */

#include "poly_tpl3d.h"



/* class WH_TPL3D_PolyVertex */

WH_TPL3D_PolyVertex
::WH_TPL3D_PolyVertex 
(WH_TPL3D_PolyBody* body,
 const WH_Vector3D& point) 
  : _point (point)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);

  _body = body;
  body->addVertex (this);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyVertex
::~WH_TPL3D_PolyVertex () 
{
}

bool WH_TPL3D_PolyVertex
::checkInvariant () const
{
  this->WH_TPL3D_Vertex_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyVertex
::assureInvariant () const
{
  this->WH_TPL3D_Vertex_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyVertex
::addConnectedFace 
(WH_TPL3D_PolyFace* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);

  vector<WH_TPL3D_Face_A*>::iterator i_faceFound 
    = find (_connectedFace_s.begin (), _connectedFace_s.end (), 
	    face);
  if (i_faceFound == _connectedFace_s.end ()) {
    _connectedFace_s.push_back (face);
  }
}

void WH_TPL3D_PolyVertex
::addConnectedEdge 
(WH_TPL3D_PolyEdge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);

  vector<WH_TPL3D_Edge_A*>::iterator i_edgeFound 
    = find (_connectedEdge_s.begin (), _connectedEdge_s.end (), 
	    edge);
  if (i_edgeFound == _connectedEdge_s.end ()) {
    _connectedEdge_s.push_back (edge);
  }
}

WH_Vector3D WH_TPL3D_PolyVertex
::point () const
{
  return _point;
}



/* class WH_TPL3D_PolyCurve */

WH_TPL3D_PolyCurve
::WH_TPL3D_PolyCurve 
(const WH_Line3D& line) 
  : _line (line)
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyCurve
::~WH_TPL3D_PolyCurve () 
{
}

bool WH_TPL3D_PolyCurve
::checkInvariant () const
{
  this->WH_TPL3D_Curve_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyCurve
::assureInvariant () const
{
  this->WH_TPL3D_Curve_A::assureInvariant ();

  return true;
}

WH_Line3D WH_TPL3D_PolyCurve
::line () const
{
  return _line;
}

bool WH_TPL3D_PolyCurve
::isLine () const
{
  return true;
}

bool WH_TPL3D_PolyCurve
::isPeriodic () const
{
  return false;
}

WH_Vector3D WH_TPL3D_PolyCurve
::positionAt 
(double parameter) const
{
  return _line.positionAt (parameter);
}

WH_Vector3D WH_TPL3D_PolyCurve
::directionAt 
(double /* parameter */) const
{
  return _line.direction ();
}

double WH_TPL3D_PolyCurve
::lengthBetween 
(double parameter0, double parameter1) const
{
  return fabs (parameter1 - parameter0);
}

bool WH_TPL3D_PolyCurve
::contains 
(const WH_Vector3D& position) const
{
  return _line.contains (position);
}

double WH_TPL3D_PolyCurve
::parameterAt 
(const WH_Vector3D& position) const
{
  return _line.parameterAt (position);
}

bool WH_TPL3D_PolyCurve
::getLength 
(double& length_OUT) const
{
  return false;
}

bool WH_TPL3D_PolyCurve
::getMinParameter 
(double& parameter_OUT) const
{
  return false;
}

bool WH_TPL3D_PolyCurve
::getMaxParameter 
(double& parameter_OUT) const
{
  return false;
}



/* class WH_TPL3D_PolyEdge */

WH_TPL3D_PolyEdge
::WH_TPL3D_PolyEdge 
(WH_TPL3D_PolyBody* body,
 WH_TPL3D_PolyVertex* vertex0,
 WH_TPL3D_PolyVertex* vertex1) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);
  WH_ASSERT(vertex0 != vertex1);

  _body = body;
  body->addEdge (this);
  _vertex0 = vertex0;
  _vertex1 = vertex1;

  WH_Line3D line (vertex0->point (), vertex1->point ());
  _curve = new WH_TPL3D_PolyCurve (line);
  WH_ASSERT(_curve != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyEdge
::~WH_TPL3D_PolyEdge () 
{
}

bool WH_TPL3D_PolyEdge
::checkInvariant () const
{
  this->WH_TPL3D_Edge_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyEdge
::assureInvariant () const
{
  this->WH_TPL3D_Edge_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyEdge
::addConnectedFace 
(WH_TPL3D_PolyFace* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  
  vector<WH_TPL3D_Face_A*>::iterator i_faceFound 
    = find (_connectedFace_s.begin (), _connectedFace_s.end (), 
	    face);
  if (i_faceFound == _connectedFace_s.end ()) {
    _connectedFace_s.push_back (face);
  }
}

void WH_TPL3D_PolyEdge
::updateConnection ()
{
  WH_TPL3D_PolyVertex* polyVertex0 
    = dynamic_cast<WH_TPL3D_PolyVertex*>(_vertex0);
  WH_ASSERT(polyVertex0 != WH_NULL);
  polyVertex0->addConnectedEdge (this);
  
  WH_TPL3D_PolyVertex* polyVertex1 
    = dynamic_cast<WH_TPL3D_PolyVertex*>(_vertex1);
  WH_ASSERT(polyVertex1 != WH_NULL);
  polyVertex1->addConnectedEdge (this);
}



/* class WH_TPL3D_PolyLoopVertexUse */

WH_TPL3D_PolyLoopVertexUse
::WH_TPL3D_PolyLoopVertexUse 
(WH_TPL3D_PolyLoop* loop,
 WH_TPL3D_PolyVertex* vertex) 
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(vertex != WH_NULL);

  _loop = loop;
  _vertex = vertex;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyLoopVertexUse
::~WH_TPL3D_PolyLoopVertexUse () 
{
}

bool WH_TPL3D_PolyLoopVertexUse
::checkInvariant () const
{
  this->WH_TPL3D_LoopVertexUse_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyLoopVertexUse
::assureInvariant () const
{
  this->WH_TPL3D_LoopVertexUse_A::assureInvariant ();

  return true;
}



/* class WH_TPL3D_PolyCurve2D */

WH_TPL3D_PolyCurve2D
::WH_TPL3D_PolyCurve2D 
(const WH_Line2D& line) 
  : _line (line)
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyCurve2D
::~WH_TPL3D_PolyCurve2D () 
{
}

bool WH_TPL3D_PolyCurve2D
::checkInvariant () const
{
  this->WH_TPL3D_Curve2D_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyCurve2D
::assureInvariant () const
{
  this->WH_TPL3D_Curve2D_A::assureInvariant ();

  return true;
}

WH_Line2D WH_TPL3D_PolyCurve2D
::line () const
{
  return _line;
}

bool WH_TPL3D_PolyCurve2D
::isLine () const
{
  return true;
}

bool WH_TPL3D_PolyCurve2D
::isPeriodic () const
{
  return false;
}

WH_Vector2D WH_TPL3D_PolyCurve2D
::positionAt 
(double parameter) const
{
  return _line.positionAt (parameter);
}

WH_Vector2D WH_TPL3D_PolyCurve2D
::directionAt 
(double /* parameter */) const
{
  return _line.direction ();
}

double WH_TPL3D_PolyCurve2D
::lengthBetween 
(double parameter0, double parameter1) const
{
  return fabs (parameter1 - parameter0);
}

bool WH_TPL3D_PolyCurve2D
::contains 
(const WH_Vector2D& position) const
{
  return _line.contains (position);
}

double WH_TPL3D_PolyCurve2D
::parameterAt 
(const WH_Vector2D& position) const
{
  return _line.parameterAt (position);
}

bool WH_TPL3D_PolyCurve2D
::getLength 
(double& length_OUT) const
{
  return false;
}

bool WH_TPL3D_PolyCurve2D
::getMinParameter 
(double& parameter_OUT) const
{
  return false;
}

bool WH_TPL3D_PolyCurve2D
::getMaxParameter 
(double& parameter_OUT) const
{
  return false;
}



/* class WH_TPL3D_PolyLoopEdgeUse */

WH_TPL3D_PolyLoopEdgeUse
::WH_TPL3D_PolyLoopEdgeUse 
(WH_TPL3D_PolyLoop* loop,
 WH_TPL3D_PolyEdge* edge) 
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(edge != WH_NULL);

  _loop = loop;
  _edge = edge;

  WH_TPL3D_Surface_A* surface = _loop->face ()->surface ();
  WH_ASSERT(surface != WH_NULL);
  WH_Vector2D point0 
    = surface->parameterAt (_edge->vertex0 ()->point ());
  WH_Vector2D point1 
    = surface->parameterAt (_edge->vertex1 ()->point ());
  WH_Line2D line (point0, point1);
  _curve = new WH_TPL3D_PolyCurve2D (line);
  WH_ASSERT(_curve != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyLoopEdgeUse
::~WH_TPL3D_PolyLoopEdgeUse () 
{
}

bool WH_TPL3D_PolyLoopEdgeUse
::checkInvariant () const
{
  this->WH_TPL3D_LoopEdgeUse_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyLoopEdgeUse
::assureInvariant () const
{
  this->WH_TPL3D_LoopEdgeUse_A::assureInvariant ();

  return true;
}



/* class WH_TPL3D_PolyLoop */

WH_TPL3D_PolyLoop
::WH_TPL3D_PolyLoop 
(WH_TPL3D_PolyFace* face) 
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);

  _face = face;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyLoop
::~WH_TPL3D_PolyLoop () 
{
}

bool WH_TPL3D_PolyLoop
::checkInvariant () const
{
  this->WH_TPL3D_Loop_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyLoop
::assureInvariant () const
{
  this->WH_TPL3D_Loop_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyLoop
::addVertex 
(WH_TPL3D_PolyVertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);

  WH_TPL3D_PolyLoopVertexUse* vertexUse
    = new WH_TPL3D_PolyLoopVertexUse (this, vertex);
  WH_ASSERT(vertexUse != WH_NULL);

  _vertexUse_s.push_back (vertexUse);
}

void WH_TPL3D_PolyLoop
::makeEdgeUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_edgeUse_s.size () == 0);
  WH_ASSERT(0 < _vertexUse_s.size ());
  /* no single-edge loop */

  if (1 < _vertexUse_s.size ()) {
    /* if it is normal loop */
    
    WH_TPL3D_PolyBody* polyBody
      = dynamic_cast<WH_TPL3D_PolyBody*>(_face->body ());
    WH_ASSERT(polyBody != WH_NULL);

    for (int i_vertexUse = 0; 
	 i_vertexUse < (int)_vertexUse_s.size ();
	 i_vertexUse++) {
      WH_TPL3D_LoopVertexUse_A* vertexUse0 
	= _vertexUse_s[i_vertexUse];
      WH_TPL3D_LoopVertexUse_A* vertexUse1 = WH_NULL;
      if (i_vertexUse == (int)_vertexUse_s.size () - 1) {
	vertexUse1 = _vertexUse_s[0];
      } else {
	vertexUse1 = _vertexUse_s[i_vertexUse + 1];
      }

      WH_TPL3D_PolyVertex* polyVertex0 
	= dynamic_cast<WH_TPL3D_PolyVertex*>(vertexUse0->vertex ());
      WH_ASSERT(polyVertex0 != WH_NULL);
      
      WH_TPL3D_PolyVertex* polyVertex1 
	= dynamic_cast<WH_TPL3D_PolyVertex*>(vertexUse1->vertex ());
      WH_ASSERT(polyVertex1 != WH_NULL);
      
      WH_TPL3D_PolyEdge* edge 
	= polyBody->findEdge (polyVertex0, polyVertex1);
      if (edge == WH_NULL) {
	edge = new WH_TPL3D_PolyEdge 
	  (polyBody, polyVertex0, polyVertex1);
	WH_ASSERT(edge != WH_NULL);
      }
      WH_ASSERT(edge != WH_NULL);

      WH_TPL3D_PolyLoopEdgeUse* edgeUse 
	= new WH_TPL3D_PolyLoopEdgeUse (this, edge);
      WH_ASSERT(edgeUse != WH_NULL);
      _edgeUse_s.push_back (edgeUse);
    }
  }
}

void WH_TPL3D_PolyLoop
::updateConnection ()
{
  WH_TPL3D_PolyFace* polyFace 
    = dynamic_cast<WH_TPL3D_PolyFace*>(this->face ());
  WH_ASSERT(polyFace != WH_NULL);

  for (vector<WH_TPL3D_LoopVertexUse_A*>::const_iterator 
	 i_vertexUse = _vertexUse_s.begin ();
       i_vertexUse != _vertexUse_s.end ();
       i_vertexUse++) {
    WH_TPL3D_LoopVertexUse_A* vertexUse_i = (*i_vertexUse);
    WH_TPL3D_PolyVertex* polyVertex 
      = dynamic_cast<WH_TPL3D_PolyVertex*>(vertexUse_i->vertex ());
    WH_ASSERT(polyVertex != WH_NULL);
    polyVertex->addConnectedFace (polyFace);
  }
  for (vector<WH_TPL3D_LoopEdgeUse_A*>::const_iterator 
	 i_edgeUse = _edgeUse_s.begin ();
       i_edgeUse != _edgeUse_s.end ();
       i_edgeUse++) {
    WH_TPL3D_LoopEdgeUse_A* edgeUse_i = (*i_edgeUse);
    WH_TPL3D_PolyEdge* polyEdge 
      = dynamic_cast<WH_TPL3D_PolyEdge*>(edgeUse_i->edge ());
    WH_ASSERT(polyEdge != WH_NULL);
    polyEdge->addConnectedFace (polyFace);
  }
}



/* class WH_TPL3D_PolySurface */

WH_TPL3D_PolySurface
::WH_TPL3D_PolySurface 
(const WH_Plane3D& plane) 
  : _plane (plane)
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolySurface
::~WH_TPL3D_PolySurface () 
{
}

bool WH_TPL3D_PolySurface
::checkInvariant () const
{
  this->WH_TPL3D_Surface_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolySurface
::assureInvariant () const
{
  this->WH_TPL3D_Surface_A::assureInvariant ();

  return true;
}

WH_Plane3D WH_TPL3D_PolySurface
::plane () const
{
  return _plane;
}

bool WH_TPL3D_PolySurface
::isPlane () const
{
  return true;
}

bool WH_TPL3D_PolySurface
::isPeriodic () const
{
  return false;
}

WH_Vector3D WH_TPL3D_PolySurface
::positionAt 
(const WH_Vector2D& parameter) const
{
  return _plane.positionAt (parameter);
}

WH_Vector3D WH_TPL3D_PolySurface
::normalAt 
(const WH_Vector2D& parameter) const
{
  return _plane.normal ();
}

double WH_TPL3D_PolySurface
::uLengthBetween 
(double /* vParameter */, 
 double uParameter0, double uParameter1) const
{
  return fabs (uParameter1 - uParameter0);
}

double WH_TPL3D_PolySurface
::vLengthBetween 
(double /* uParameter */, 
 double vParameter0, double vParameter1) const
{
  return fabs (vParameter1 - vParameter0);
}

bool WH_TPL3D_PolySurface
::contains 
(const WH_Vector3D& position) const
{
  return _plane.contains (position);
}

WH_Vector2D WH_TPL3D_PolySurface
::parameterAt 
(const WH_Vector3D& position) const
{
  return _plane.parameterAt (position);
}



/* class WH_TPL3D_PolyFace */

WH_TPL3D_PolyFace
::WH_TPL3D_PolyFace 
(WH_TPL3D_PolyBody* body,
 FaceType faceType) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);
  
  _body = body;
  body->addFace (this);
  _faceType = faceType;
  _orientationIsSameAsSurfaceNormal = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyFace
::~WH_TPL3D_PolyFace () 
{
}

bool WH_TPL3D_PolyFace
::checkInvariant () const
{
  this->WH_TPL3D_Face_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyFace
::assureInvariant () const
{
  this->WH_TPL3D_Face_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyFace
::setOuterLoop (WH_TPL3D_PolyLoop* loop,
		const WH_Vector3D& outsideNormal)
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  WH_ASSERT(_outerLoop == WH_NULL);
  WH_ASSERT(_surface == WH_NULL);
  
  _outerLoop = loop;

  /* NEED TO REDEFINE */
  /* if the first 3 vertex points are concave, what happen ? */

  const vector<WH_TPL3D_LoopVertexUse_A*>& vertexUse_s 
    = loop->vertexUse_s ();
  WH_ASSERT(2 < vertexUse_s.size ());
  WH_Vector3D anyPoint = vertexUse_s[0]->vertex ()->point ();
  WH_Plane3D plane (outsideNormal, anyPoint);
  _surface = new WH_TPL3D_PolySurface (plane);
  WH_ASSERT(_surface != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_outerLoop != WH_NULL);
  WH_ASSERT(_surface != WH_NULL);
#endif
}

void WH_TPL3D_PolyFace
::addInnerLoop (WH_TPL3D_PolyLoop* loop)
{
  /* PRE-CONDITION */
  WH_ASSERT(loop != WH_NULL);
  
  _innerLoop_s.push_back (loop);
}

void WH_TPL3D_PolyFace
::makeEdgeUses ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_outerLoop != WH_NULL);
  WH_ASSERT(_surface != WH_NULL);

  WH_TPL3D_PolyLoop* polyLoop 
    = dynamic_cast<WH_TPL3D_PolyLoop*>(_outerLoop);
  WH_ASSERT(polyLoop != WH_NULL);
  polyLoop->makeEdgeUses ();

  for (vector<WH_TPL3D_Loop_A*>::const_iterator 
	 i_loop = _innerLoop_s.begin ();
       i_loop != _innerLoop_s.end ();
       i_loop++) {
    WH_TPL3D_Loop_A* loop_i = (*i_loop);
    WH_TPL3D_PolyLoop* polyLoop 
      = dynamic_cast<WH_TPL3D_PolyLoop*>(loop_i);
    WH_ASSERT(polyLoop != WH_NULL);
    polyLoop->makeEdgeUses ();
  }
}

void WH_TPL3D_PolyFace
::updateConnection ()
{
  WH_TPL3D_PolyLoop* polyLoop 
    = dynamic_cast<WH_TPL3D_PolyLoop*>(_outerLoop);
  WH_ASSERT(polyLoop != WH_NULL);
  polyLoop->updateConnection ();

  for (vector<WH_TPL3D_Loop_A*>::iterator i_loop
	 = _innerLoop_s.begin ();
       i_loop != _innerLoop_s.end ();
       i_loop++) {
    WH_TPL3D_Loop_A* loop_i = (*i_loop);
    WH_TPL3D_PolyLoop* polyLoop 
      = dynamic_cast<WH_TPL3D_PolyLoop*>(loop_i);
    WH_ASSERT(polyLoop != WH_NULL);
    polyLoop->updateConnection ();
  }
}

WH_TPL3D_PolyFace::FaceType WH_TPL3D_PolyFace
::faceType () const
{
  return _faceType;
}

bool WH_TPL3D_PolyFace
::orientationIsSameAsSurfaceNormal () const
{
  return _orientationIsSameAsSurfaceNormal;
}



/* class WH_TPL3D_PolyVolume */

WH_TPL3D_PolyVolume
::WH_TPL3D_PolyVolume 
(WH_TPL3D_PolyBody* body) 
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);

  _body = body;
  body->addVolume (this);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyVolume
::~WH_TPL3D_PolyVolume () 
{
}

bool WH_TPL3D_PolyVolume
::checkInvariant () const
{
  this->WH_TPL3D_Volume_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyVolume
::assureInvariant () const
{
  this->WH_TPL3D_Volume_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyVolume
::addFace (WH_TPL3D_PolyFace* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  
  _face_s.push_back (face);
}



/* class WH_TPL3D_PolyBody */

WH_TPL3D_PolyBody
::WH_TPL3D_PolyBody () 
{
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_TPL3D_PolyBody
::~WH_TPL3D_PolyBody () 
{
}

bool WH_TPL3D_PolyBody
::checkInvariant () const
{
  this->WH_TPL3D_Body_A::checkInvariant ();

  return true;
}

bool WH_TPL3D_PolyBody
::assureInvariant () const
{
  this->WH_TPL3D_Body_A::assureInvariant ();

  return true;
}

void WH_TPL3D_PolyBody
::addVertex (WH_TPL3D_PolyVertex* vertex)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex != WH_NULL);
  
  _vertex_s.push_back (vertex);
}

void WH_TPL3D_PolyBody
::addEdge (WH_TPL3D_PolyEdge* edge)
{
  /* PRE-CONDITION */
  WH_ASSERT(edge != WH_NULL);
  
  _edge_s.push_back (edge);
}

WH_TPL3D_PolyEdge* WH_TPL3D_PolyBody
::findEdge 
(WH_TPL3D_PolyVertex* vertex0, 
 WH_TPL3D_PolyVertex* vertex1)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);
  WH_ASSERT(vertex0 != vertex1);
  
  WH_TPL3D_PolyEdge* result = WH_NULL;

  for (vector<WH_TPL3D_Edge_A*>::const_iterator 
	 i_edge = _edge_s.begin ();
       i_edge != _edge_s.end ();
       i_edge++) {
    WH_TPL3D_Edge_A* edge_i = (*i_edge);

    if ((edge_i->vertex0 () == vertex0
	 && edge_i->vertex1 () == vertex1) 
	|| (edge_i->vertex0 () == vertex1
	    && edge_i->vertex1 () == vertex0)) {
      WH_TPL3D_PolyEdge* polyEdge 
	= dynamic_cast<WH_TPL3D_PolyEdge*>(edge_i);
      WH_ASSERT(polyEdge != WH_NULL);
      result = polyEdge;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL || result == WH_NULL);
#endif

  return result;
}

void WH_TPL3D_PolyBody
::addFace (WH_TPL3D_PolyFace* face)
{
  /* PRE-CONDITION */
  WH_ASSERT(face != WH_NULL);
  
  _face_s.push_back (face);
}

void WH_TPL3D_PolyBody
::addVolume (WH_TPL3D_PolyVolume* volume)
{
  /* PRE-CONDITION */
  WH_ASSERT(volume != WH_NULL);
  
  _volume_s.push_back (volume);
}

void WH_TPL3D_PolyBody
::makeEdgeUses ()
{
  for (vector<WH_TPL3D_Face_A*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_TPL3D_Face_A* face_i = (*i_face);
    WH_TPL3D_PolyFace* polyFace 
      = dynamic_cast<WH_TPL3D_PolyFace*>(face_i);
    WH_ASSERT(polyFace != WH_NULL);
    polyFace->makeEdgeUses ();
  }
}

void WH_TPL3D_PolyBody
::updateConnection ()
{
  for (vector<WH_TPL3D_Edge_A*>::const_iterator 
	 i_edge = _edge_s.begin ();
       i_edge != _edge_s.end ();
       i_edge++) {
    WH_TPL3D_Edge_A* edge_i = (*i_edge);
    WH_TPL3D_PolyEdge* polyEdge 
      = dynamic_cast<WH_TPL3D_PolyEdge*>(edge_i);
    WH_ASSERT(polyEdge != WH_NULL);
    polyEdge->updateConnection ();
  }
  for (vector<WH_TPL3D_Face_A*>::const_iterator 
	 i_face = _face_s.begin ();
       i_face != _face_s.end ();
       i_face++) {
    WH_TPL3D_Face_A* face_i = (*i_face);
    WH_TPL3D_PolyFace* polyFace 
      = dynamic_cast<WH_TPL3D_PolyFace*>(face_i);
    WH_ASSERT(polyFace != WH_NULL);
    polyFace->updateConnection ();
  }
}




