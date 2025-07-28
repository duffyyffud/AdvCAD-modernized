/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of space3d.cc */



/* class WH_Vector3D */

WH_INLINE constexpr WH_Vector3D
::WH_Vector3D () : x(0.0), y(0.0), z(0.0)
{
}

WH_INLINE constexpr WH_Vector3D
::WH_Vector3D (double x, double y, double z) : x(x), y(y), z(z)
{ 
}

WH_INLINE constexpr WH_Vector3D WH_Vector3D
::operator- () const 
{ 
  return WH_Vector3D (-x, -y, -z); 
}

WH_INLINE const WH_Vector3D& WH_Vector3D
::operator+= (const WH_Vector3D& p) 
{ 
  x += p.x;  y += p.y;  z += p.z;  return *this; 
}

WH_INLINE const WH_Vector3D& WH_Vector3D
::operator-= (const WH_Vector3D& p) 
{ 
  x -= p.x;  y -= p.y;  z -= p.z;  return *this; 
}

WH_INLINE const WH_Vector3D& WH_Vector3D
::operator*= (double ratio) 
{ 
  x *= ratio;  y *= ratio;  z *= ratio;  return *this; 
}

WH_INLINE const WH_Vector3D& WH_Vector3D
::operator/= (double ratio) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (ratio, 0.0));

  x /= ratio;  y /= ratio;  z /= ratio;  return *this; 
}

WH_INLINE double& WH_Vector3D
::operator[] (int index) 
{ 
  int mod = index % 3;
  return (mod == 0) ? x : (mod == 1 ? y : z); 
}

WH_INLINE double WH_Vector3D
::operator[] (int index) const 
{ 
  int mod = index % 3;
  return (mod == 0) ? x : (mod == 1 ? y : z); 
}
  
WH_INLINE double WH_Vector3D
::length () const 
{ 
  return sqrt (x * x + y * y + z * z); 
}

WH_INLINE WH_Vector3D WH_Vector3D
::normalize () const 
{
  double len = this->length ();
  WH_ASSERT(WH_ne (len, 0.0));
  return WH_Vector3D (x / len, y / len, z / len);
}

WH_INLINE WH_Vector3D abs (const WH_Vector3D& p)
{ 
  return WH_Vector3D (p.x > 0 ? p.x : -p.x, 
		      p.y > 0 ? p.y : -p.y,
		      p.z > 0 ? p.z : -p.z); 
}

WH_INLINE WH_Vector3D operator+ 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.x + p1.x, p0.y + p1.y, p0.z + p1.z);
}

WH_INLINE WH_Vector3D operator- 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.x - p1.x, p0.y - p1.y, p0.z - p1.z);
}

WH_INLINE WH_Vector3D operator* 
(const WH_Vector3D& p0, double ratio) 
{
  return WH_Vector3D (p0.x * ratio, p0.y * ratio, p0.z * ratio);
}

WH_INLINE WH_Vector3D operator/ 
(const WH_Vector3D& p0, double ratio) 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (ratio, 0.0));

  return WH_Vector3D (p0.x / ratio, p0.y / ratio, p0.z / ratio);
}

WH_INLINE double WH_scalarProduct 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return p0.x * p1.x + p0.y * p1.y + p0.z * p1.z;
}

WH_INLINE WH_Vector3D WH_vectorProduct 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.y * p1.z - p0.z * p1.y,
		      p0.z * p1.x - p0.x * p1.z,
		      p0.x * p1.y - p0.y * p1.x);
}

WH_INLINE WH_Vector3D WH_multiply 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.x * p1.x, p0.y * p1.y, p0.z * p1.z);
}

WH_INLINE WH_Vector3D WH_divide 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p1.x, 0.0));
  WH_ASSERT(WH_ne (p1.y, 0.0));
  WH_ASSERT(WH_ne (p1.z, 0.0));

  return WH_Vector3D (p0.x / p1.x, p0.y / p1.y, p0.z / p1.z);
}

WH_INLINE bool WH_eq 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_eq (p0.x, p1.x) 
    && WH_eq (p0.y, p1.y) 
      && WH_eq (p0.z, p1.z);
}

WH_INLINE bool WH_ne 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_ne (p0.x, p1.x) 
    || WH_ne (p0.y, p1.y) 
      || WH_ne (p0.z, p1.z);
}

WH_INLINE bool operator== 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  return WH_eq (p0, p1);
}

WH_INLINE bool operator!= 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  return WH_ne (p0, p1);
}

WH_INLINE bool WH_lt
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_lt (p0.x, p1.x) 
    && WH_lt (p0.y, p1.y)
      && WH_lt (p0.z, p1.z);
}

WH_INLINE bool WH_gt
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_gt (p0.x, p1.x) 
    && WH_gt (p0.y, p1.y)
      && WH_gt (p0.z, p1.z);
}

WH_INLINE bool WH_le
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_le (p0.x, p1.x) 
    && WH_le (p0.y, p1.y)
      && WH_le (p0.z, p1.z);
}

WH_INLINE bool WH_ge
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_ge (p0.x, p1.x) 
    && WH_ge (p0.y, p1.y)
      && WH_ge (p0.z, p1.z);
}

WH_INLINE bool WH_eq2 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_eq2 (p0.x, p1.x) 
    && WH_eq2 (p0.y, p1.y) 
      && WH_eq2 (p0.z, p1.z);
}

WH_INLINE bool WH_ne2 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_ne2 (p0.x, p1.x) 
    || WH_ne2 (p0.y, p1.y) 
      || WH_ne2 (p0.z, p1.z);
}

WH_INLINE bool WH_lt2
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_lt2 (p0.x, p1.x) 
    && WH_lt2 (p0.y, p1.y)
      && WH_lt2 (p0.z, p1.z);
}

WH_INLINE bool WH_gt2
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_gt2 (p0.x, p1.x) 
    && WH_gt2 (p0.y, p1.y)
      && WH_gt2 (p0.z, p1.z);
}

WH_INLINE bool WH_le2
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_le2 (p0.x, p1.x) 
    && WH_le2 (p0.y, p1.y)
      && WH_le2 (p0.z, p1.z);
}

WH_INLINE bool WH_ge2
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_ge2 (p0.x, p1.x) 
    && WH_ge2 (p0.y, p1.y)
      && WH_ge2 (p0.z, p1.z);
}

WH_INLINE WH_Vector3D WH_min 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.x < p1.x ? p0.x : p1.x,
		      p0.y < p1.y ? p0.y : p1.y,
		      p0.z < p1.z ? p0.z : p1.z);
}

WH_INLINE WH_Vector3D WH_max 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_Vector3D (p0.x > p1.x ? p0.x : p1.x,
		      p0.y > p1.y ? p0.y : p1.y,
		      p0.z > p1.z ? p0.z : p1.z);
}

WH_INLINE bool WH_isReverse 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return WH_eq (p0.x, -p1.x) 
    && WH_eq (p0.y, -p1.y)
      && WH_eq (p0.z, -p1.z);
}

WH_INLINE bool WH_isParallel 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  return WH_eq (WH_vectorProduct (p0, p1), WH_Vector3D::zero ());
}

WH_INLINE bool WH_isPerpendicular 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  return WH_eq (WH_scalarProduct (p0, p1), 0);
}

WH_INLINE double WH_distance 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  return (p0 - p1).length ();
}

WH_INLINE double WH_squareSum 
(const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  double x = p0.x - p1.x;
  double y = p0.y - p1.y;
  double z = p0.z - p1.z;
  return x * x + y * y + z * z;
}

WH_INLINE void WH_swap 
(WH_Vector3D& p0, WH_Vector3D& p1) 
{
  WH_Vector3D dummy = p0;  
  p0 = p1; 
  p1 = dummy;
}

WH_INLINE WH_Vector3D WH_interpolate 
(const WH_Vector3D& p0, const WH_Vector3D& p1, double ratio) 
{
  return p0 + (p1 - p0) * ratio;
}

WH_INLINE void WH_sortPair 
(WH_Vector3D& value0_IO, WH_Vector3D& value1_IO)
{
  WH_sortPair (value0_IO.x, value1_IO.x);
  WH_sortPair (value0_IO.y, value1_IO.y);
  WH_sortPair (value0_IO.z, value1_IO.z);
}

WH_INLINE bool WH_minMaxPairsOverlap 
(const WH_Vector3D& min0, const WH_Vector3D& max0, 
 const WH_Vector3D& min1, const WH_Vector3D& max1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (min0, max0));
  WH_ASSERT(WH_le (min1, max1));

  return WH_minMaxPairsOverlap (min0.x, max0.x, min1.x, max1.x)
    && WH_minMaxPairsOverlap (min0.y, max0.y, min1.y, max1.y)
      && WH_minMaxPairsOverlap (min0.z, max0.z, min1.z, max1.z);
}

WH_INLINE WH_Vector3D WH_Vector3D
::zero ()
{
  return WH_Vector3D (0, 0, 0);
}

WH_INLINE WH_Vector3D WH_Vector3D
::xUnit ()
{
  return WH_Vector3D (1, 0, 0);
}

WH_INLINE WH_Vector3D WH_Vector3D
::yUnit ()
{
  return WH_Vector3D (0, 1, 0);
}

WH_INLINE WH_Vector3D WH_Vector3D
::zUnit ()
{
  return WH_Vector3D (0, 0, 1);
}

WH_INLINE WH_Vector3D WH_Vector3D
::hugeValue ()
{
  return WH_Vector3D (1.0e+20, 1.0e+20, 1.0e+20);
}



/* class WH_Matrix3D */

WH_INLINE WH_Vector3D operator* 
(const WH_Vector3D& p, const WH_Matrix3D& m)
{
  return WH_Vector3D 
    (m.v[0][0] * p.x + m.v[1][0] * p.y + m.v[2][0] * p.z + m.v[3][0],
     m.v[0][1] * p.x + m.v[1][1] * p.y + m.v[2][1] * p.z + m.v[3][1],
     m.v[0][2] * p.x + m.v[1][2] * p.y + m.v[2][2] * p.z + m.v[3][2]);
}
  


/* class WH_Line3D */

WH_INLINE double WH_Line3D
::a () const 
{ 
  return _a; 
}

WH_INLINE double WH_Line3D
::b () const 
{ 
  return _b; 
}

WH_INLINE double WH_Line3D
::c () const 
{ 
  return _c; 
}

WH_INLINE double WH_Line3D
::p () const 
{ 
  return _p; 
}

WH_INLINE double WH_Line3D
::q () const 
{ 
  return _q; 
}

WH_INLINE double WH_Line3D
::r () const 
{ 
  return _r; 
}

WH_INLINE WH_Vector3D WH_Line3D
::direction () const 
{ 
  return WH_Vector3D (_a, _b, _c); 
}

WH_INLINE WH_Vector3D WH_Line3D
::origin () const 
{ 
  return WH_Vector3D (_p, _q, _r); 
}

WH_INLINE double WH_Line3D
::parameterAt (const WH_Vector3D& position) const 
{
  return WH_scalarProduct ((position - this->origin ()),
			   this->direction ());
}

WH_INLINE WH_Vector3D WH_Line3D
::positionAt (double parameter) const 
{
  return this->origin () + this->direction () * parameter;
}

WH_INLINE bool WH_eq 
(const WH_Line3D& l0, const WH_Line3D& l1)
{
  return WH_eq (l0.direction (), l1.direction ())
    && WH_eq (l0.origin (), l1.origin ());
}

WH_INLINE bool WH_ne 
(const WH_Line3D& l0, const WH_Line3D& l1)
{
  return !WH_eq (l0, l1);
}

WH_INLINE bool WH_isReverse 
(const WH_Line3D& l0, const WH_Line3D& l1)
{
  return WH_eq (l0.direction (), -l1.direction ())
    && WH_eq (l0.origin (), l1.origin ());
}

WH_INLINE WH_Line3D WH_Line3D
::xAxis ()
{
  return WH_Line3D (WH_Vector3D (0, 0, 0), WH_Vector3D (1, 0, 0));
}

WH_INLINE WH_Line3D WH_Line3D
::yAxis ()
{
  return WH_Line3D (WH_Vector3D (0, 0, 0), WH_Vector3D (0, 1, 0));
}

WH_INLINE WH_Line3D WH_Line3D
::zAxis ()
{
  return WH_Line3D (WH_Vector3D (0, 0, 0), WH_Vector3D (0, 0, 1));
}



/* class WH_Plane3D */

WH_INLINE double WH_Plane3D
::a () const 
{ 
  return _a; 
}

WH_INLINE double WH_Plane3D
::b () const 
{ 
  return _b; 
}

WH_INLINE double WH_Plane3D
::c () const 
{ 
  return _c; 
}

WH_INLINE double WH_Plane3D
::d () const 
{ 
  return _d; 
}

WH_INLINE WH_Vector3D WH_Plane3D
::normal () const 
{ 
  return WH_Vector3D (_a, _b, _c); 
}

WH_INLINE double WH_Plane3D
::valueAt (const WH_Vector3D& position) const 
{
  return _a * position.x + _b * position.y + _c * position.z + _d;
}

WH_INLINE bool WH_Plane3D
::contains (const WH_Vector3D& position) const 
{
  return WH_eq (this->valueAt (position), 0);
}

WH_INLINE double WH_Plane3D
::distanceFrom (const WH_Vector3D& position) const 
{
  return fabs (this->valueAt (position));
}

WH_INLINE WH_Vector3D WH_Plane3D
::origin () const
{
  return WH_Vector3D (-_a * _d, -_b * _d, -_c * _d);
}

WH_INLINE WH_Vector3D WH_Plane3D
::uAxisDir () const
{
  return _uAxisDir;
}

WH_INLINE WH_Vector3D WH_Plane3D
::vAxisDir () const
{
  return _vAxisDir;
}

WH_INLINE WH_Vector2D WH_Plane3D
::parameterAt (const WH_Vector3D& position) const
{
  WH_Vector3D origin = this->origin ();
  return WH_Vector2D 
    (WH_scalarProduct (position - origin, this->uAxisDir ()),
     WH_scalarProduct (position - origin, this->vAxisDir ()));
}

WH_INLINE WH_Vector3D WH_Plane3D
::positionAt (const WH_Vector2D& parameter) const
{
  return this->origin () 
    + this->uAxisDir () * parameter.x
      + this->vAxisDir () * parameter.y;
}

WH_INLINE bool WH_eq 
(const WH_Plane3D& pl0, const WH_Plane3D& pl1) 
{
  return WH_eq (pl0.a (), pl1.a ()) 
    && WH_eq (pl0.b (), pl1.b ()) 
      && WH_eq (pl0.c (), pl1.c ()) 
	&& WH_eq (pl0.d (), pl1.d ());
}

WH_INLINE bool WH_ne 
(const WH_Plane3D& pl0, const WH_Plane3D& pl1) 
{
  return !WH_eq (pl0, pl1);
}

WH_INLINE bool WH_isReverse 
(const WH_Plane3D& pl0, const WH_Plane3D& pl1) 
{
  return WH_eq (pl0.a (), -pl1.a ()) 
    && WH_eq (pl0.b (), -pl1.b ()) 
      && WH_eq (pl0.c (), -pl1.c ())
	&& WH_eq (pl0.d (), -pl1.d ());
}

WH_INLINE WH_Plane3D WH_Plane3D
::xyPlane ()
{
  return WH_Plane3D (0, 0, 1, 0);
}

WH_INLINE WH_Plane3D WH_Plane3D
::yzPlane ()
{
  return WH_Plane3D (1, 0, 0, 0);
}

WH_INLINE WH_Plane3D WH_Plane3D
::zxPlane ()
{
  return WH_Plane3D (0, 1, 0, 0);
}




