/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of space2d.cc */



/* class WH_Vector2D */

WH_INLINE constexpr WH_Vector2D
::WH_Vector2D () : x(0.0), y(0.0)
{
}

WH_INLINE constexpr WH_Vector2D
::WH_Vector2D (double x, double y) : x(x), y(y)
{ 
}

WH_INLINE constexpr WH_Vector2D WH_Vector2D
::operator- () const 
{ 
  return WH_Vector2D (-x, -y); 
}

WH_INLINE const WH_Vector2D& WH_Vector2D
::operator+= (const WH_Vector2D& p) 
{ 
  x += p.x;  y += p.y;  return *this; 
}

WH_INLINE const WH_Vector2D& WH_Vector2D
::operator-= (const WH_Vector2D& p) 
{ 
  x -= p.x;  y -= p.y;  return *this; 
}

WH_INLINE const WH_Vector2D& WH_Vector2D
::operator*= (double ratio) 
{ 
  x *= ratio;  y *= ratio;  return *this; 
}

WH_INLINE const WH_Vector2D& WH_Vector2D
::operator/= (double ratio) 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (ratio, 0.0));

  x /= ratio;  y /= ratio;  return *this; 
}

WH_INLINE double& WH_Vector2D
::operator[] (int index) 
{ 
  return index % 2 == 0 ? x : y; 
}

WH_INLINE double WH_Vector2D
::operator[] (int index) const 
{ 
  return index % 2 == 0 ? x : y; 
}
  
WH_INLINE double WH_Vector2D
::length () const 
{ 
  return sqrt (x * x + y * y); 
}

WH_INLINE WH_Vector2D WH_Vector2D
::normalize () const 
{
  double len = this->length ();
  WH_ASSERT(WH_ne (len, 0.0));
  return WH_Vector2D (x / len, y / len);
}

WH_INLINE WH_Vector2D WH_abs (const WH_Vector2D& p)
{ 
  return WH_Vector2D (p.x > 0 ? p.x : -p.x, p.y > 0 ? p.y : -p.y); 
}

WH_INLINE WH_Vector2D operator+ 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_Vector2D (p0.x + p1.x, p0.y + p1.y);
}

WH_INLINE WH_Vector2D operator- 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_Vector2D (p0.x - p1.x, p0.y - p1.y);
}

WH_INLINE WH_Vector2D operator* 
(const WH_Vector2D& p0, double ratio) 
{
  return WH_Vector2D (p0.x * ratio, p0.y * ratio);
}

WH_INLINE WH_Vector2D operator/ 
(const WH_Vector2D& p0, double ratio) 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (ratio, 0.0));

  return WH_Vector2D (p0.x / ratio, p0.y / ratio);
}

WH_INLINE double WH_scalarProduct 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return p0.x * p1.x + p0.y * p1.y;
}

WH_INLINE WH_Vector2D WH_multiply 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_Vector2D (p0.x * p1.x, p0.y * p1.y);
}

WH_INLINE WH_Vector2D WH_divide 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p1.x, 0.0));
  WH_ASSERT(WH_ne (p1.y, 0.0));

  return WH_Vector2D (p0.x / p1.x, p0.y / p1.y);
}

WH_INLINE bool WH_eq 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_eq (p0.x, p1.x) && WH_eq (p0.y, p1.y);
}

WH_INLINE bool WH_ne 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_ne (p0.x, p1.x) || WH_ne (p0.y, p1.y);
}

WH_INLINE bool operator== 
(const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  return WH_eq (p0, p1);
}

WH_INLINE bool operator!= 
(const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  return WH_ne (p0, p1);
}

WH_INLINE bool WH_lt
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_lt (p0.x, p1.x) && WH_lt (p0.y, p1.y);
}

WH_INLINE bool WH_gt
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_gt (p0.x, p1.x) && WH_gt (p0.y, p1.y);
}

WH_INLINE bool WH_le
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_le (p0.x, p1.x) && WH_le (p0.y, p1.y);
}

WH_INLINE bool WH_ge
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_ge (p0.x, p1.x) && WH_ge (p0.y, p1.y);
}

WH_INLINE bool WH_eq2 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_eq2 (p0.x, p1.x) && WH_eq2 (p0.y, p1.y);
}

WH_INLINE bool WH_ne2 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_ne2 (p0.x, p1.x) || WH_ne2 (p0.y, p1.y);
}

WH_INLINE bool WH_lt2
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_lt2 (p0.x, p1.x) && WH_lt2 (p0.y, p1.y);
}

WH_INLINE bool WH_gt2
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_gt2 (p0.x, p1.x) && WH_gt2 (p0.y, p1.y);
}

WH_INLINE bool WH_le2
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_le2 (p0.x, p1.x) && WH_le2 (p0.y, p1.y);
}

WH_INLINE bool WH_ge2
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_ge2 (p0.x, p1.x) && WH_ge2 (p0.y, p1.y);
}

WH_INLINE WH_Vector2D WH_min 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_Vector2D (p0.x < p1.x ? p0.x : p1.x,
		      p0.y < p1.y ? p0.y : p1.y);
}

WH_INLINE WH_Vector2D WH_max 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_Vector2D (p0.x > p1.x ? p0.x : p1.x,
		      p0.y > p1.y ? p0.y : p1.y);
}

WH_INLINE bool WH_isReverse 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return WH_eq (p0.x, -p1.x) && WH_eq (p0.y, -p1.y);
}

WH_INLINE bool WH_isParallel 
(const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector2D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector2D::zero ()));

  double c = WH_scalarProduct (p0, p1) / (p0.length () * p1.length ());
  return WH_eq (fabs (c), 1);
}

WH_INLINE bool WH_isPerpendicular 
(const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector2D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector2D::zero ()));

  return WH_eq (WH_scalarProduct (p0, p1), 0);
}

WH_INLINE double WH_distance 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  return (p0 - p1).length ();
}

WH_INLINE double WH_squareSum 
(const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  double x = p0.x - p1.x;
  double y = p0.y - p1.y;
  return x * x + y * y;
}

WH_INLINE void WH_swap 
(WH_Vector2D& p0, WH_Vector2D& p1) 
{
  WH_Vector2D dummy = p0;  
  p0 = p1; 
  p1 = dummy;
}

WH_INLINE WH_Vector2D WH_interpolate 
(const WH_Vector2D& p0, const WH_Vector2D& p1, double ratio) 
{
  return p0 + (p1 - p0) * ratio;
}

WH_INLINE void WH_sortPair 
(WH_Vector2D& value0_IO, WH_Vector2D& value1_IO)
{
  WH_sortPair (value0_IO.x, value1_IO.x);
  WH_sortPair (value0_IO.y, value1_IO.y);
}

WH_INLINE bool WH_minMaxPairsOverlap 
(const WH_Vector2D& min0, const WH_Vector2D& max0, 
 const WH_Vector2D& min1, const WH_Vector2D& max1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (min0, max0));
  WH_ASSERT(WH_le (min1, max1));

  return WH_minMaxPairsOverlap (min0.x, max0.x, min1.x, max1.x)
    && WH_minMaxPairsOverlap (min0.y, max0.y, min1.y, max1.y);
}

WH_INLINE double WH_signedTriangleAreaAmong 
(const WH_Vector2D& p0, 
 const WH_Vector2D& p1, 
 const WH_Vector2D& p2)
{
  /* p0 ... K */
  /* p1 ... L */
  /* p2 ... M */
  
  WH_Vector2D lk = p1 - p0;
  WH_Vector2D mk = p2 - p0;
  return (lk.x * mk.y - mk.x * lk.y) / 2;
}

WH_INLINE WH_Vector2D WH_Vector2D
::zero ()
{
  return WH_Vector2D (0, 0);
}

WH_INLINE WH_Vector2D WH_Vector2D
::xUnit ()
{
  return WH_Vector2D (1, 0);
}

WH_INLINE WH_Vector2D WH_Vector2D
::yUnit ()
{
  return WH_Vector2D (0, 1);
}

WH_INLINE WH_Vector2D WH_Vector2D
::hugeValue ()
{
  return WH_Vector2D (1.0e+20, 1.0e+20);
}



/* class WH_Matrix2D */

WH_INLINE WH_Vector2D operator* 
(const WH_Vector2D& p, const WH_Matrix2D& m) 
{
  return WH_Vector2D 
    (m.v[0][0] * p.x + m.v[1][0] * p.y + m.v[2][0],
     m.v[0][1] * p.x + m.v[1][1] * p.y + m.v[2][1]);
}



/* class WH_Line2D */

WH_INLINE double WH_Line2D
::a () const
{ 
  return _a; 
}

WH_INLINE double WH_Line2D
::b () const
{ 
  return _b; 
}

WH_INLINE double WH_Line2D
::c () const
{
  return _c; 
}

WH_INLINE WH_Vector2D WH_Line2D
::normal () const 
{ 
  return WH_Vector2D (_a, _b); 
}

WH_INLINE WH_Vector2D WH_Line2D
::direction () const 
{ 
  return WH_Vector2D (_b, -_a); 
}

WH_INLINE double WH_Line2D
::valueAt (const WH_Vector2D& position) const 
{
  return _a * position.x + _b * position.y + _c;
}

WH_INLINE bool WH_Line2D
::contains (const WH_Vector2D& position) const 
{
  return WH_eq (this->valueAt (position), 0);
}

WH_INLINE double WH_Line2D
::distanceFrom (const WH_Vector2D& position) const 
{
  return fabs (this->valueAt (position));
}

WH_INLINE WH_Vector2D WH_Line2D
::origin () const
{
  return WH_Vector2D (-_a * _c, -_b * _c);
}

WH_INLINE double WH_Line2D
::parameterAt 
(const WH_Vector2D& position) const
{
  return WH_scalarProduct (position - this->origin (), 
			   this->direction ());
}

WH_INLINE WH_Vector2D WH_Line2D
::positionAt (double parameter) const
{
  return this->origin () + this->direction () * parameter;
}

WH_INLINE bool WH_eq 
(const WH_Line2D& l0, const WH_Line2D& l1) 
{
  return WH_eq (l0.a (), l1.a ()) && WH_eq (l0.b (), l1.b ()) 
    && WH_eq (l0.c (), l1.c ());
}

WH_INLINE bool WH_ne 
(const WH_Line2D& l0, const WH_Line2D& l1) 
{
  return !WH_eq (l0, l1);
}

WH_INLINE bool WH_isReverse 
(const WH_Line2D& l0, const WH_Line2D& l1) 
{
  return WH_eq (l0.a (), -l1.a ()) && WH_eq (l0.b (), -l1.b ()) 
    && WH_eq (l0.c (), -l1.c ());
}

WH_INLINE WH_Line2D WH_Line2D
::xAxis ()
{
  return WH_Line2D (0, 1, 0);
}

WH_INLINE WH_Line2D WH_Line2D
::yAxis ()
{
  return WH_Line2D (1, 0, 0);
}







