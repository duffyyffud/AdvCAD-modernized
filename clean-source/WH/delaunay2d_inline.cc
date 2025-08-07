/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of delaunay2d.cc */



/* class WH_DLN2D_Point */

WH_INLINE void WH_DLN2D_Point
::setId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id || id < 0);
  /* point is dummy if <id> is negative */

  _id = id;
}

WH_INLINE int WH_DLN2D_Point
::id () const
{ 
  return _id; 
}

WH_INLINE WH_Vector2D WH_DLN2D_Point
::position () const
{ 
  return _position; 
}

WH_INLINE bool WH_DLN2D_Point
::isDummy () const
{ 
  return _id < 0; 
}



/* class WH_DLN2D_Segment */

WH_INLINE void WH_DLN2D_Segment
::setFront (WH_DLN2D_Triangle* front)
{
  /* PRE-CONDITION */
  WH_ASSERT(front != WH_NULL || front == WH_NULL);

  _front = front; 
}

WH_INLINE void WH_DLN2D_Segment
::setRear (WH_DLN2D_Triangle* rear)
{
  /* PRE-CONDITION */
  WH_ASSERT(rear != WH_NULL);

  _rear = rear; 
}

WH_INLINE WH_DLN2D_Point* WH_DLN2D_Segment
::point0 () const
{
  return _point0;
}

WH_INLINE WH_DLN2D_Point* WH_DLN2D_Segment
::point1 () const
{
  return _point1;
}

WH_INLINE WH_DLN2D_Segment* WH_DLN2D_Segment
::edge0 () const
{
  return _edge0;
}

WH_INLINE WH_DLN2D_Segment* WH_DLN2D_Segment
::edge1 () const
{
  return _edge1;
}

WH_INLINE WH_DLN2D_Triangle* WH_DLN2D_Segment
::front () const
{
  return _front;
}

WH_INLINE WH_DLN2D_Triangle* WH_DLN2D_Segment
::rear () const
{
  return _rear;
}

WH_INLINE bool WH_DLN2D_Segment
::hasPoint (WH_DLN2D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);

  return point == _point0 || point == _point1;
}



/* WH_DLN2D_Triangle class */

WH_INLINE void WH_DLN2D_Triangle
::setIterator 
(list<WH_DLN2D_Triangle*>::iterator iterator)
{
  _iterator = iterator;
}

WH_INLINE void WH_DLN2D_Triangle
::setNeighborAt 
(int edgeNumber, WH_DLN2D_Triangle* tri) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);
  WH_ASSERT(tri != WH_NULL);
#ifndef NDEBUG
  WH_ASSERT(tri->isNeighborOf (this));
  WH_ASSERT(edgeNumber == this->edgeNumberOfNeighbor (tri));
#endif

  _neighbors[edgeNumber] = tri; 
}

WH_INLINE void WH_DLN2D_Triangle
::clearNeighborAt (int edgeNumber) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);

  _neighbors[edgeNumber] = WH_NULL; 
}

WH_INLINE void WH_DLN2D_Triangle
::setMark () 
{ 
  _markFlag = true; 
}

WH_INLINE void WH_DLN2D_Triangle
::clearMark () 
{ 
  _markFlag = false; 
}

WH_INLINE WH_DLN2D_Point* WH_DLN2D_Triangle
::point (int vertexNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= vertexNumber);
  WH_ASSERT(vertexNumber < 3);

  return _points[vertexNumber]; 
}

WH_INLINE bool WH_DLN2D_Triangle
::hasPoint (WH_DLN2D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);

  if (_points[0] == point) return true;
  if (_points[1] == point) return true;
  if (_points[2] == point) return true;
  return false;
}

WH_INLINE list<WH_DLN2D_Triangle*>::iterator WH_DLN2D_Triangle
::iterator () const
{
  return _iterator;
}

WH_INLINE WH_DLN2D_Triangle* WH_DLN2D_Triangle
::neighborAt (int edgeNumber) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= edgeNumber);
  WH_ASSERT(edgeNumber < 3);

  return _neighbors[edgeNumber]; 
}

WH_INLINE bool WH_DLN2D_Triangle
::includesWithinCircle (const WH_DLN2D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);
  
  double sum = WH_squareSum (point->position (), _centerOfCircle);
  double sum2 = _radiusOfCircle * _radiusOfCircle;
  
  /* NEED TO REDEFINE : numerical error */
#if 0
  return WH_le2 (sum, sum2);
#else
  return WH_le (sum, sum2);
#endif
}



/* class WH_DLN2D_Triangulator */

WH_INLINE const vector<WH_DLN2D_Point*>& WH_DLN2D_Triangulator
::point_s () const
{
  return _point_s;
}

WH_INLINE const list<WH_DLN2D_Triangle*>& WH_DLN2D_Triangulator
::triangle_s () const
{
  return _triangle_s;
}



