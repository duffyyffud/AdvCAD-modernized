/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of constdel2d.cc */



/* class WH_CDLN2D_BoundarySegment */

WH_INLINE void WH_CDLN2D_BoundarySegment
::setMark ()
{
  _hasMark = true;
}

WH_INLINE WH_DLN2D_Point* WH_CDLN2D_BoundarySegment
::point0 () const
{
  return _point0;
}

WH_INLINE WH_DLN2D_Point* WH_CDLN2D_BoundarySegment
::point1 () const
{
  return _point1;
}

WH_INLINE bool WH_CDLN2D_BoundarySegment
::hasPoint (WH_DLN2D_Point* point) const
{
  /* PRE-CONDITION */
  WH_ASSERT(point != WH_NULL);

  return point == _point0 || point == _point1;
}

WH_INLINE WH_Vector2D WH_CDLN2D_BoundarySegment
::frontNormal () const
{
  WH_Vector2D dir = _point1->position () - _point0->position ();
  return dir.rotate (WH_Vector2D::zero (), M_PI / 2).normalize ();
}

WH_INLINE int WH_CDLN2D_BoundarySegment
::frontDomainId () const
{
  return _frontDomainId;
}

WH_INLINE int WH_CDLN2D_BoundarySegment
::rearDomainId () const
{
  return _rearDomainId;
}

WH_INLINE bool WH_CDLN2D_BoundarySegment
::hasMark () const
{
  return _hasMark;
}



/* class WH_CDLN2D_Triangle */

WH_INLINE void WH_CDLN2D_Triangle
::setId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id);

  _id = id;
}

WH_INLINE void WH_CDLN2D_Triangle
::setDomainId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id);

  _domainId = id;
}

WH_INLINE int WH_CDLN2D_Triangle
::id () const
{
  return _id;
}

WH_INLINE int WH_CDLN2D_Triangle
::domainId () const
{
  return _domainId;
}



/* class WH_CDLN2D_Triangulator */

WH_INLINE const vector<WH_CDLN2D_BoundarySegment*>& WH_CDLN2D_Triangulator
::boundarySegment_s () const
{
  return _boundarySegment_s;
}


