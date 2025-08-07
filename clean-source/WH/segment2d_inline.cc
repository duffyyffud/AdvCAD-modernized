/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of segment2d.cc */



/* class WH_Segment2D */

WH_INLINE WH_Vector2D WH_Segment2D
::p0 () const
{ 
  return _p0; 
}

WH_INLINE WH_Vector2D WH_Segment2D
::p1 () const
{ 
  return _p1; 
}

WH_INLINE WH_Line2D WH_Segment2D
::line () const
{ 
  return _line; 
}

WH_INLINE WH_Vector2D WH_Segment2D
::normal () const 
{ 
  return _line.normal (); 
}

WH_INLINE WH_Vector2D WH_Segment2D
::direction () const 
{ 
  return _line.direction (); 
}

WH_INLINE double WH_Segment2D
::length () const
{
  return WH_distance (_p0, _p1);
}

WH_INLINE WH_Vector2D WH_Segment2D
::midPoint () const
{
  return (_p0 + _p1) / 2;
}

WH_INLINE WH_Vector2D WH_Segment2D
::minRange () const
{
  return WH_min (_p0, _p1);
}

WH_INLINE WH_Vector2D WH_Segment2D
::maxRange () const
{
  return WH_max (_p0, _p1);
}

WH_INLINE bool WH_Segment2D
::isParallelToXAxis () const
{
  return _line.isParallelToXAxis ();
}

WH_INLINE bool WH_Segment2D
::isParallelToYAxis () const
{
  return _line.isParallelToYAxis ();
}

WH_INLINE bool WH_Segment2D
::hasVertexAt (const WH_Vector2D& position) const
{
  return WH_eq (_p0, position) || WH_eq (_p1, position);
}

WH_INLINE bool WH_eq 
(const WH_Segment2D& s0, const WH_Segment2D& s1) 
{
  return WH_eq (s0.p0 (), s1.p0 ()) && WH_eq (s0.p1 (), s1.p1 ());
}

WH_INLINE bool WH_ne 
(const WH_Segment2D& s0, const WH_Segment2D& s1) 
{
  return !WH_eq (s0, s1);
}

WH_INLINE bool WH_isReverse 
(const WH_Segment2D& s0, const WH_Segment2D& s1) 
{
  return WH_eq (s0.p0 (), s1.p1 ()) && WH_eq (s0.p1 (), s1.p0 ());
}




