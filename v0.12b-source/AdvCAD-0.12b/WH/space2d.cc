/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* space2d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "space2d.h"



/* class WH_Vector2D */

ostream& operator<< (ostream& out, const WH_Vector2D& p) 
{
  out << "( " << WH_format (p.x) << " " << WH_format (p.y) << " ) ";
  return out;
}

bool WH_between (const WH_Vector2D& p, 
		 const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  if (p0.x <= p1.x) {
    WH_CVR_LINE;
    if (WH_lt (x, p0.x) || WH_lt (p1.x, x)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt (x, p1.x) || WH_lt (p0.x, x)) return false;
  }
  if (p0.y <= p1.y) {
    WH_CVR_LINE;
    if (WH_lt (y, p0.y) || WH_lt (p1.y, y)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt (y, p1.y) || WH_lt (p0.y, y)) return false;
  }
  return true;
}

bool WH_justBetween (const WH_Vector2D& p, 
		     const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  if (p0.x <= p1.x) {
    WH_CVR_LINE;
    if (WH_le (x, p0.x) || WH_le (p1.x, x)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le (x, p1.x) || WH_le (p0.x, x)) return false;
  }
  if (p0.y <= p1.y) {
    WH_CVR_LINE;
    if (WH_le (y, p0.y) || WH_le (p1.y, y)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le (y, p1.y) || WH_le (p0.y, y)) return false;
  }
  return true;
}

bool WH_between2 (const WH_Vector2D& p, 
		  const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  if (p0.x <= p1.x) {
    WH_CVR_LINE;
    if (WH_lt2 (x, p0.x) || WH_lt2 (p1.x, x)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt2 (x, p1.x) || WH_lt2 (p0.x, x)) return false;
  }
  if (p0.y <= p1.y) {
    WH_CVR_LINE;
    if (WH_lt2 (y, p0.y) || WH_lt2 (p1.y, y)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt2 (y, p1.y) || WH_lt2 (p0.y, y)) return false;
  }
  return true;
}

bool WH_justBetween2 (const WH_Vector2D& p, 
		      const WH_Vector2D& p0, const WH_Vector2D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  if (p0.x <= p1.x) {
    WH_CVR_LINE;
    if (WH_le2 (x, p0.x) || WH_le2 (p1.x, x)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le2 (x, p1.x) || WH_le2 (p0.x, x)) return false;
  }
  if (p0.y <= p1.y) {
    WH_CVR_LINE;
    if (WH_le2 (y, p0.y) || WH_le2 (p1.y, y)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le2 (y, p1.y) || WH_le2 (p0.y, y)) return false;
  }
  return true;
}

WH_Vector2D WH_Vector2D 
::translate (const WH_Vector2D& offset) const
{
  return *this + offset;
}

WH_Vector2D WH_Vector2D
::rotate (const WH_Vector2D& center, double angle) const
{
  WH_CVR_LINE;

  WH_Vector2D v = *this - center;
  double s = sin (angle);
  double c = cos (angle);
  v = WH_Vector2D (c * v.x - s * v.y, s * v.x + c * v.y);
  return center + v;
}

WH_Vector2D WH_Vector2D
::scale (const WH_Vector2D& center, double ratio) const
{
  return center + (*this - center) * ratio;
}

double WH_Vector2D 
::slope () const 
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (x, 0) || WH_ne (y, 0));

  WH_CVR_LINE;

  double result = M_PI * 2;

  if (WH_eq (x, 0)) {
    WH_CVR_LINE;
    result = (y > 0) ? M_PI / 2 : -M_PI / 2;
  } else if (WH_eq (y, 0)) {
    WH_CVR_LINE;
    result = (x > 0) ? 0 : M_PI;
  } else {
    WH_CVR_LINE;
    if (0 < x) {
      WH_CVR_LINE;
      if (0 < y) {
	WH_CVR_LINE;
	result = atan (y / x);
      } else {
	WH_CVR_LINE;
	result = -atan (-y / x);
      }
    } else {
      WH_CVR_LINE;
      if (0 < y) {
	WH_CVR_LINE;
	result = M_PI - atan (y / -x);
      } else {
	WH_CVR_LINE;
	result = -M_PI + atan (-y / -x);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(-M_PI < result);
  WH_ASSERT(WH_le (result, M_PI));
#endif

  return result;
}

double WH_angleOfVectors 
(const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  WH_CVR_LINE;

  double slope0 = p0.slope ();
  double slope1 = p1.slope ();
  double result = slope1 - slope0;
  if (WH_lt (M_PI, result)) {
    WH_CVR_LINE;
    result -= 2 * M_PI;
  } else if (WH_le (result, -M_PI)) {
    WH_CVR_LINE;
    result += 2 * M_PI;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(-M_PI < result);
  WH_ASSERT(WH_le (result, M_PI));
#endif

  return result;
}

bool WH_getScaleIfParallel 
(const WH_Vector2D& p0, const WH_Vector2D& p1,
 double& ratio_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector2D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector2D::zero ()));

  WH_CVR_LINE;

  bool result = false;
  if (WH_isParallel (p0, p1)) {
    WH_CVR_LINE;
    result = true;
    if (WH_ne (p1.x, 0)) {
      WH_CVR_LINE;
      ratio_OUT = p0.x / p1.x;
    } else {
      WH_CVR_LINE;
      WH_ASSERT(WH_ne (p1.y, 0));
      ratio_OUT = p0.y / p1.y;
    }
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(WH_ne (ratio_OUT, 0));
    WH_ASSERT(WH_eq (p0, p1 * ratio_OUT));
  }
#endif  

  return result;
}

bool WH_valuesOverlap 
(const WH_Vector2D& value0_0, const WH_Vector2D& value0_1, 
 const WH_Vector2D& value1_0, const WH_Vector2D& value1_1)
{
  WH_CVR_LINE;

  WH_Vector2D newValue0_0 = value0_0;
  WH_Vector2D newValue0_1 = value0_1;
  WH_sortPair (newValue0_0, newValue0_1);
  WH_Vector2D newValue1_0 = value1_0;
  WH_Vector2D newValue1_1 = value1_1;
  WH_sortPair (newValue1_0, newValue1_1);
  return WH_minMaxPairsOverlap 
    (newValue0_0, newValue0_1, newValue1_0, newValue1_1);
}

WH_Vector2D WH_circumcenterAmong 
(const WH_Vector2D& p0, 
 const WH_Vector2D& p1, 
 const WH_Vector2D& p2)
{
  WH_CVR_LINE;
  
  double x0 = p0.x;
  double y0 = p0.y;
  double x1 = p1.x;
  double y1 = p1.y;
  double x2 = p2.x;
  double y2 = p2.y;
  double a = ((x0 * x0 - x1 * x1 + y0 * y0 - y1 * y1) * (y0 - y2)
	      - (x0 * x0 - x2 * x2 + y0 * y0 - y2 * y2) * (y0 - y1))
    / ((x0 - x1) * (y0 - y2) - (x0 - x2) * (y0 - y1));
  a = a / 2;
  double b = ((x0 * x0 - x1 * x1 + y0 * y0 - y1 * y1) * (x0 - x2)
	      - (x0 * x0 - x2 * x2 + y0 * y0 - y2 * y2) * (x0 - x1))
    / ((y0 - y1) * (x0 - x2) - (y0 - y2) * (x0 - x1));
  b = b / 2;
  return WH_Vector2D (a, b);
}

bool WH_contains 
(const vector<WH_Vector2D>& point_s, 
 const WH_Vector2D& point)
{
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    if (WH_eq (point_i, point)) {
      return true;
    }
  }
  return false;
}

bool WH_hasDuplication 
(const vector<WH_Vector2D>& point_s)
{
  for (vector<WH_Vector2D>::const_iterator 
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector2D point_i = (*i_point);
    for (vector<WH_Vector2D>::const_iterator 
	   j_point = i_point + 1;
	 j_point != point_s.end ();
	 j_point++) {
      WH_Vector2D point_j = (*j_point);
      if (WH_eq (point_j, point_i)) {
	return true;
      }
    }
  }
  return false;
}



/* class WH_Matrix2D */

WH_Matrix2D
::WH_Matrix2D ()
{
  WH_CVR_LINE;

  v[0][0] = 0;  v[0][1] = 0;  v[0][2] = 0;
  v[1][0] = 0;  v[1][1] = 0;  v[1][2] = 0;
  v[2][0] = 0;  v[2][1] = 0;  v[2][2] = 0;
}

WH_Matrix2D
::WH_Matrix2D 
(double v00, double v01, double v02,
 double v10, double v11, double v12,
 double v20, double v21, double v22)
{
  WH_CVR_LINE;

  v[0][0] = v00;  
  v[0][1] = v01;  
  v[0][2] = v02;

  v[1][0] = v10;  
  v[1][1] = v11;  
  v[1][2] = v12;

  v[2][0] = v20;  
  v[2][1] = v21;  
  v[2][2] = v22;
}

ostream& operator<< (ostream& out, const WH_Matrix2D& m)
{
  WH_CVR_LINE;

  out << "WH_Matrix2D ";
  for (int i = 0; i < 3; i++) {
    out << "[ ";
    for (int j = 0; j < 3; j++) {
      out << WH_format (m.v[i][j]) << " ";
    }
    out << "] ";
  }
  return out;
}

WH_Matrix2D WH_Matrix2D
::zero ()
{
  return WH_Matrix2D ();
}
 
WH_Matrix2D WH_Matrix2D
::identity ()
{
  return WH_Matrix2D (1, 0, 0, 
		      0, 1, 0, 
		      0, 0, 1);
}

WH_Matrix2D WH_Matrix2D
::translation (const WH_Vector2D& offset)
{
  WH_CVR_LINE;

  WH_Matrix2D m = WH_Matrix2D::identity ();
  m.v[2][0] = offset.x;
  m.v[2][1] = offset.y;
  return m;
}

WH_Matrix2D WH_Matrix2D
::scaling (const WH_Vector2D& ratio)
{
  WH_CVR_LINE;

  WH_Matrix2D m = WH_Matrix2D::identity ();
  m.v[0][0] = ratio.x;
  m.v[1][1] = ratio.y;
  return m;
}

WH_Matrix2D WH_Matrix2D
::rotation (double angle)
{
  WH_CVR_LINE;

  WH_Matrix2D m = WH_Matrix2D::identity ();
  double s = sin (angle);  double c = cos (angle);
  m.v[0][0] = c;   m.v[0][1] = s;
  m.v[1][0] = -s;  m.v[1][1] = c;
  return m;
}

WH_Matrix2D operator* (const WH_Matrix2D& m0, const WH_Matrix2D& m1)
{
  WH_CVR_LINE;
  WH_Matrix2D result;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      result.v[i][j] = 0.0;
      for (int k = 0; k < 3; k++) {
	result.v[i][j] += m0.v[i][k] * m1.v[k][j];
      }
    }
  }
  return result;
}



/* class WH_Line2D */

WH_Line2D
::WH_Line2D ()
{
  WH_CVR_LINE;

  _a = 0;
  _b = 1;
  _c = 0;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Line2D
::WH_Line2D (double a, double b, double c)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (a, 0) || WH_ne (b, 0));

  WH_CVR_LINE;

  double len = WH_Vector2D (a, b).length ();
  WH_ASSERT(WH_ne (len, 0));
  _a = a / len;  
  _b = b / len;  
  _c = c / len;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Line2D
::WH_Line2D (const WH_Vector2D& p0, const WH_Vector2D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, p1));

  WH_CVR_LINE;

  double dx = p1.x - p0.x;
  double dy = p1.y - p0.y;
  double a = -dy;
  double b = dx;
  double c = dy * p0.x - p0.y * dx;
  double len = WH_Vector2D (a, b).length ();
  WH_ASSERT(WH_ne (len, 0));
  _a = a / len;  
  _b = b / len;  
  _c = c / len;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_Line2D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_eq (WH_Vector2D (_a, _b).length (), 1));

  return true;
}

bool WH_Line2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_Vector2D normal = this->normal ();
  WH_Vector2D direction = this->direction ();
  WH_ASSERT(WH_eq (normal.length (), 1));
  WH_ASSERT(WH_eq (direction.length (), 1));
  WH_ASSERT(WH_isPerpendicular (normal, direction));
  WH_Vector2D origin = this->origin ();
  WH_ASSERT(this->contains (origin));
  WH_ASSERT(WH_eq (this->valueAt (origin), 0));
  WH_ASSERT(WH_eq (this->distanceFrom (origin), 0));
  WH_ASSERT(WH_eq (this->projectedPoint (origin), origin));
  WH_ASSERT(WH_eq (this->parameterAt (origin), 0));
  WH_ASSERT(WH_eq (this->positionAt (0), origin));
  WH_ASSERT(WH_eq (WH_angleOfLines (*this, *this), 0));

  WH_Vector2D point;
  WH_ASSERT(this->checkIntersectionWith 
	  (*this, 
	   point) == COINCIDENT_WITH_LINE);
  
  return true;
}

ostream& operator<< (ostream& out, const WH_Line2D& line)
{
  return out << "WH_Line2D " 
    << WH_format (line._a) << " x + " 
      << WH_format (line._b) << " y + " 
	<< WH_format (line._c) << " = 0 ";
}

WH_Vector2D WH_Line2D
::projectedPoint (const WH_Vector2D& position) const
{
  return position - this->normal () * this->valueAt (position);
}
  
bool WH_Line2D
::isParallelToXAxis () const
{
  return WH_eq (_a, 0) && WH_eq (fabs (_b), 1);
}

bool WH_Line2D
::isParallelToYAxis () const
{
  return WH_eq (fabs (_a), 1) && WH_eq (_b, 0);
}

double WH_angleOfLines (const WH_Line2D& l0, const WH_Line2D& l1)
{
  WH_CVR_LINE;

  double value = l0.a () * l1.a () + l0.b () * l1.b ();
  return acos (WH_rangedValue (value, -1.0, 1.0));

  /* NEED TO REDEFINE */
  /* -1.0 <= <value> <= 1.0   or   0.0 <= <value> <= 1.0   ??? */
}

WH_Line2D::WithLineIntersectionType WH_Line2D
::checkIntersectionWith 
(const WH_Line2D& line,
 WH_Vector2D& point_OUT) const
{
  WH_CVR_LINE;

  WithLineIntersectionType result = POINT_WITH_LINE;
  
  if (WH_isParallel (this->direction (), line.direction ())) {
    WH_CVR_LINE;
    /* parallel */
    
    if (WH_eq (*this, line) || WH_isReverse (*this, line)) {
      WH_CVR_LINE;
      result = COINCIDENT_WITH_LINE;
    } else {
      WH_CVR_LINE;
      result = PARALLEL_WITH_LINE;
    }
  } else {
    WH_CVR_LINE;
    /* not parallel */

    double x;
    double y;
    bool flag = WH::solveLinearEquation2D 
      (this->a (), this->b (), 
       line.a (), line.b (), 
       this->c (), line.c (),
       x, y);
    WH_ASSERT(flag);
    point_OUT = WH_Vector2D (x, y);
    result = POINT_WITH_LINE;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case POINT_WITH_LINE:
    WH_ASSERT(this->contains (point_OUT));
    WH_ASSERT(line.contains (point_OUT));
    break;
  case PARALLEL_WITH_LINE: 
    WH_ASSERT(WH_isParallel (this->direction (), line.direction ()));
    WH_ASSERT(WH_isParallel (this->normal (), line.normal ()));
    break;
  case COINCIDENT_WITH_LINE:
    WH_ASSERT(WH_isParallel (this->direction (), line.direction ()));
    WH_ASSERT(WH_isParallel (this->normal (), line.normal ()));
    WH_ASSERT(WH_eq (*this, line) || WH_isReverse (*this, line));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}



#ifndef WH_INLINE_ENABLED
#include "space2d_inline.cc"
#endif



/* test coverage completed */
