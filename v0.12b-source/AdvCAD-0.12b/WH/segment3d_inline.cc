/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of segment3d.cc */



/* class WH_Segment3D */

WH_INLINE WH_Vector3D WH_Segment3D
::p0 () const
{ 
  return _p0; 
}

WH_INLINE WH_Vector3D WH_Segment3D
::p1 () const
{ 
  return _p1; 
}

WH_INLINE WH_Line3D WH_Segment3D
::line () const
{ 
  return _line; 
}

WH_INLINE WH_Vector3D WH_Segment3D
::direction () const 
{ 
  return _line.direction (); 
}

WH_INLINE double WH_Segment3D
::length () const
{
  return WH_distance (_p0, _p1);
}

WH_INLINE WH_Vector3D WH_Segment3D
::midPoint () const
{
  return (_p0 + _p1) / 2;
}

WH_INLINE WH_Vector3D WH_Segment3D
::minRange () const
{
  return WH_min (_p0, _p1);
}

WH_INLINE WH_Vector3D WH_Segment3D
::maxRange () const
{
  return WH_max (_p0, _p1);
}

WH_INLINE bool WH_Segment3D
::isParallelToXAxis () const
{
  return _line.isParallelToXAxis ();
}

WH_INLINE bool WH_Segment3D
::isParallelToYAxis () const
{
  return _line.isParallelToYAxis ();
}

WH_INLINE bool WH_Segment3D
::isParallelToZAxis () const
{
  return _line.isParallelToZAxis ();
}

WH_INLINE bool WH_Segment3D
::hasVertexAt (const WH_Vector3D& position) const
{
  return WH_eq (_p0, position) || WH_eq (_p1, position);
}

WH_INLINE bool WH_eq 
(const WH_Segment3D& s0, const WH_Segment3D& s1) 
{
  return WH_eq (s0.p0 (), s1.p0 ()) && WH_eq (s0.p1 (), s1.p1 ());
}

WH_INLINE bool WH_ne 
(const WH_Segment3D& s0, const WH_Segment3D& s1) 
{
  return !WH_eq (s0, s1);
}

WH_INLINE bool WH_isReverse 
(const WH_Segment3D& s0, const WH_Segment3D& s1) 
{
  return WH_eq (s0.p0 (), s1.p1 ()) && WH_eq (s0.p1 (), s1.p0 ());
}



