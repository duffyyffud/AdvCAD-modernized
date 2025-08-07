/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of delaunay3d.cc */



/* class WH_DLN3D_Point */

WH_INLINE int WH_DLN3D_Point
::id () const
{ 
  return _id; 
}

WH_INLINE WH_Vector3D WH_DLN3D_Point
::position () const
{ 
  return _position; 
}

WH_INLINE bool WH_DLN3D_Point
::isDummy () const
{ 
  return _id < 0; 
}



/* class WH_DLN3D_Triangle */

WH_INLINE void WH_DLN3D_Triangle
::setEdgeAt (int edgeNumber, WH_DLN3D_Triangle* tri) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);
  WH_ASSERT(tri != WH_NULL);

  _edges[edgeNumber] = tri; 
}

WH_INLINE void WH_DLN3D_Triangle
::setFront (WH_DLN3D_Tetrahedron* front) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(front != WH_NULL || front == WH_NULL);

  _front = front; 
  if (_front != WH_NULL) {
    _frontFaceNumber = _front->faceNumberOf (this);
  }
}

WH_INLINE void WH_DLN3D_Triangle
::setRear (WH_DLN3D_Tetrahedron* rear) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(rear != WH_NULL);

  _rear = rear; 
}

WH_INLINE WH_DLN3D_Tetrahedron* WH_DLN3D_Triangle
::tetrahedron () const
{ 
  return _tetrahedron; 
}

WH_INLINE int WH_DLN3D_Triangle
::faceNumber () const
{ 
  return _faceNumber; 
}

WH_INLINE WH_DLN3D_Point* WH_DLN3D_Triangle
::point (int vertexNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= vertexNumber);
  WH_ASSERT(vertexNumber < 3);

  return _points[vertexNumber]; 
}

WH_INLINE bool WH_DLN3D_Triangle
::hasPoint (WH_DLN3D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);

  if (_points[0] == point) return true;
  if (_points[1] == point) return true;
  if (_points[2] == point) return true;
  return false;
}

WH_INLINE WH_DLN3D_Triangle* WH_DLN3D_Triangle
::edgeAt (int edgeNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);

  return _edges[edgeNumber]; 
}

WH_INLINE WH_DLN3D_Tetrahedron* WH_DLN3D_Triangle
::front () const
{ 
  return _front; 
}

WH_INLINE WH_DLN3D_Tetrahedron* WH_DLN3D_Triangle
::rear () const
{ 
  return _rear; 
}

WH_INLINE int WH_DLN3D_Triangle
::frontFaceNumber () const
{
  return _frontFaceNumber; 
}
  


/* class WH_DLN3D_Tetrahedron */

WH_INLINE void WH_DLN3D_Tetrahedron
::setIterator 
(list<WH_DLN3D_Tetrahedron*>::iterator iterator)
{
  _iterator = iterator;
}

WH_INLINE void WH_DLN3D_Tetrahedron
::setNeighborAt 
(int faceNumber, WH_DLN3D_Tetrahedron* tetra) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);
  WH_ASSERT(tetra != WH_NULL);
#ifndef NDEBUG
  WH_ASSERT(tetra->isNeighborOf (this));
  WH_ASSERT(faceNumber == this->faceNumberOfNeighbor (tetra));
#endif

  _neighbors[faceNumber] = tetra; 
}

WH_INLINE void WH_DLN3D_Tetrahedron
::clearNeighborAt (int faceNumber) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);

  _neighbors[faceNumber] = WH_NULL; 
}

WH_INLINE void WH_DLN3D_Tetrahedron
::setMark () 
{ 
  _markFlag = true; 
}

WH_INLINE void WH_DLN3D_Tetrahedron
::clearMark () 
{ 
  _markFlag = false; 
}

WH_INLINE WH_DLN3D_Point* WH_DLN3D_Tetrahedron
::point (int vertexNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= vertexNumber);
  WH_ASSERT(vertexNumber < 4);

  return _points[vertexNumber]; 
}

WH_INLINE bool WH_DLN3D_Tetrahedron
::hasPoint (WH_DLN3D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);

  if (_points[0] == point) return true;
  if (_points[1] == point) return true;
  if (_points[2] == point) return true;
  if (_points[3] == point) return true;
  return false;
}

WH_INLINE list<WH_DLN3D_Tetrahedron*>::iterator WH_DLN3D_Tetrahedron
::iterator () const
{
  return _iterator;
}

WH_INLINE WH_DLN3D_Tetrahedron* WH_DLN3D_Tetrahedron
::neighborAt (int faceNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= faceNumber);
  WH_ASSERT(faceNumber < 4);

  return _neighbors[faceNumber]; 
}

WH_INLINE bool WH_DLN3D_Tetrahedron
::hasMark () const
{ 
  return _markFlag; 
}

WH_INLINE bool WH_DLN3D_Tetrahedron
::includesWithinSphere (const WH_DLN3D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  
  double sum = WH_squareSum (point->position (), _centerOfSphere);
  double sum2 = _radiusOfSphere * _radiusOfSphere;
  
  /* NEED TO REDEFINE : numerical error */
#if 0
  return WH_le2 (sum, sum2);
#else
  return WH_le (sum, sum2);
#endif
}



/* class WH_DLN3D_Triangulator */

WH_INLINE const vector<WH_DLN3D_Point*>& WH_DLN3D_Triangulator
::point_s () const
{
  return _point_s;
}

WH_INLINE const list<WH_DLN3D_Tetrahedron*>& WH_DLN3D_Triangulator
::tetrahedron_s () const
{
  return _tetrahedron_s;
}

