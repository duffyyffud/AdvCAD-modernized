/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* space3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "space3d.h"



/* class WH_Vector3D */

ostream& operator<< (ostream& out, const WH_Vector3D& p) 
{
  out << "( " << WH_format (p.x) 
    << " " << WH_format (p.y) 
      << " " << WH_format (p.z) << " ) ";
  return out;
}

bool WH_between (const WH_Vector3D& p, 
		 const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  double z = p.z;
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
  if (p0.z <= p1.z) {
    WH_CVR_LINE;
    if (WH_lt (z, p0.z) || WH_lt (p1.z, z)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt (z, p1.z) || WH_lt (p0.z, z)) return false;
  }
  return true;
}

bool WH_justBetween (const WH_Vector3D& p, 
		     const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  double z = p.z;
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
  if (p0.z <= p1.z) {
    WH_CVR_LINE;
    if (WH_le (z, p0.z) || WH_le (p1.z, z)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le (z, p1.z) || WH_le (p0.z, z)) return false;
  }
  return true;
}

bool WH_between2 (const WH_Vector3D& p, 
		  const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  double z = p.z;
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
  if (p0.z <= p1.z) {
    WH_CVR_LINE;
    if (WH_lt2 (z, p0.z) || WH_lt2 (p1.z, z)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_lt2 (z, p1.z) || WH_lt2 (p0.z, z)) return false;
  }
  return true;
}

bool WH_justBetween2 (const WH_Vector3D& p, 
		      const WH_Vector3D& p0, const WH_Vector3D& p1) 
{
  WH_CVR_LINE;

  double x = p.x;
  double y = p.y;
  double z = p.z;
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
  if (p0.z <= p1.z) {
    WH_CVR_LINE;
    if (WH_le2 (z, p0.z) || WH_le2 (p1.z, z)) return false;
  } else {
    WH_CVR_LINE;
    if (WH_le2 (z, p1.z) || WH_le2 (p0.z, z)) return false;
  }
  return true;
}

double WH_sinOfVectors 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  double result = 0.0;
  
  WH_Vector3D vp = WH_vectorProduct (p0.normalize (), p1.normalize ());
  if (WH_eq2 (vp, WH_Vector3D::zero ())) {
    WH_CVR_LINE;
    result = 0.0;
  } else {
    WH_CVR_LINE;
    WH_Vector3D vpNormalized = vp.normalize ();
    if (WH_ne2 (vpNormalized.x, 0)) {
      WH_CVR_LINE;
      result = vp.x / vpNormalized.x;
    } else if (WH_ne2 (vpNormalized.y, 0)) {
      WH_CVR_LINE;
      result = vp.y / vpNormalized.y;
    } else {
      WH_CVR_LINE;
      WH_ASSERT(WH_ne2 (vpNormalized.z, 0));
      result = vp.z / vpNormalized.z;
    }
    WH_ASSERT(WH_ne (result, 0));
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_between (result, -1, 1));
#endif

  return result;
}

double WH_cosOfVectors 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  double result = WH_scalarProduct (p0.normalize (), p1.normalize ());

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_between2 (result, -1, 1));
#endif

  return result;
}

double WH_angleOfVectors 
(const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  double c = WH_rangedValue 
    (WH_scalarProduct (p0.normalize (), p1.normalize ()), 
     -1, 1);
  double result = acos (c);
  if (WH_lt (M_PI, result)) {
    WH_CVR_LINE;                               /* NOT COVERED */
    result -= 2 * M_PI;
  } else if (WH_le (result, -M_PI)) {
    WH_CVR_LINE;                               /* NOT COVERED */
    result += 2 * M_PI;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(-M_PI < result);
  WH_ASSERT(WH_le (result, M_PI));
#endif

  return result;
}

WH_Vector3D WH_Vector3D 
::translate (const WH_Vector3D& offset) const
{
  return *this + offset;
}

WH_Vector3D WH_Vector3D
::rotateByXAxis (const WH_Vector3D& center, double angle) const
{
  WH_CVR_LINE;

  WH_Vector3D v = *this - center;
  double s = sin (angle);
  double c = cos (angle);
  v = WH_Vector3D (v.x, c * v.y - s * v.z, s * v.y + c * v.z);
  return center + v;
}

WH_Vector3D WH_Vector3D
::rotateByYAxis (const WH_Vector3D& center, double angle) const
{
  WH_CVR_LINE;

  WH_Vector3D v = *this - center;
  double s = sin (angle);
  double c = cos (angle);
  v = WH_Vector3D (c * v.x + s * v.z, v.y, - s * v.x + c * v.z);
  return center + v;
}

WH_Vector3D WH_Vector3D
::rotateByZAxis (const WH_Vector3D& center, double angle) const
{
  WH_CVR_LINE;

  WH_Vector3D v = *this - center;
  double s = sin (angle);
  double c = cos (angle);
  v = WH_Vector3D (c * v.x - s * v.y, s * v.x + c * v.y, v.z);
  return center + v;
}

WH_Vector3D WH_Vector3D
::scale (const WH_Vector3D& center, double ratio) const
{
  return center + (*this - center) * ratio;
}

WH_Vector3D WH_Vector3D
::scaleAlongXAxis (const WH_Vector3D& center, double ratio) const
{
  return WH_Vector3D (center.x + (x - center.x) * ratio, y, z);
}

WH_Vector3D WH_Vector3D
::scaleAlongYAxis (const WH_Vector3D& center, double ratio) const
{
  return WH_Vector3D (x, center.y + (y - center.y) * ratio, z);
}

WH_Vector3D WH_Vector3D
::scaleAlongZAxis (const WH_Vector3D& center, double ratio) const
{
  return WH_Vector3D (x, y, center.z + (z - center.z) * ratio);
}

bool WH_getScaleIfParallel 
(const WH_Vector3D& p0, const WH_Vector3D& p1,
 double& ratio_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, WH_Vector3D::zero ()));
  WH_ASSERT(WH_ne (p1, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  bool result = false;
  if (WH_isParallel (p0, p1)) {
    WH_CVR_LINE;
    result = true;
    if (WH_ne (p1.x, 0)) {
      WH_CVR_LINE;
      ratio_OUT = p0.x / p1.x;
    } else if (WH_ne (p1.y, 0)) {
      WH_CVR_LINE;
      ratio_OUT = p0.y / p1.y;
    } else {
      WH_CVR_LINE;
      WH_ASSERT(WH_ne (p1.z, 0));
      ratio_OUT = p0.z / p1.z;
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
(const WH_Vector3D& value0_0, const WH_Vector3D& value0_1, 
 const WH_Vector3D& value1_0, const WH_Vector3D& value1_1)
{
  WH_CVR_LINE;

  WH_Vector3D newValue0_0 = value0_0;
  WH_Vector3D newValue0_1 = value0_1;
  WH_sortPair (newValue0_0, newValue0_1);
  WH_Vector3D newValue1_0 = value1_0;
  WH_Vector3D newValue1_1 = value1_1;
  WH_sortPair (newValue1_0, newValue1_1);
  return WH_minMaxPairsOverlap 
    (newValue0_0, newValue0_1, newValue1_0, newValue1_1);
}

WH_Vector3D WH_circumcenterAmong 
(const WH_Vector3D& a, 
 const WH_Vector3D& b, 
 const WH_Vector3D& c, 
 const WH_Vector3D& d)
{
  WH_CVR_LINE;

  double ax = a.x;
  double ay = a.y;
  double az = a.z;
  double bx = b.x;
  double by = b.y;
  double bz = b.z;
  double cx = c.x;
  double cy = c.y;
  double cz = c.z;
  double dx = d.x;
  double dy = d.y;
  double dz = d.z;

  /* normalized normal vector of the plane determined by pointA, */
  /* pointB, pointC.    */
  double vx   = ((by - ay)*(cz - az)) - ((bz - az)*(cy - ay));
  double vy   = ((bz - az)*(cx - ax)) - ((bx - ax)*(cz - az));
  double vz   = ((bx - ax)*(cy - ay)) - ((by - ay)*(cx - ax));
  double dist = sqrt( (vx*vx) + (vy*vy) + (vz*vz) );
  vx   = vx / dist;
  vy   = vy / dist;
  vz   = vz / dist;
  
  /* parameters for caluculating the center of sphere */
  double a1 = (ax - bx)*2;
  double a2 = (ax - cx)*2;
  double b1 = (ay - by)*2;
  double b2 = (ay - cy)*2;
  double c1 = (az - bz)*2;
  double c2 = (az - cz)*2;
  double d1 = (ax*ax) + (ay*ay) + (az*az) - (bx*bx) - (by*by) - (bz*bz);
  double d2 = (ax*ax) + (ay*ay) + (az*az) - (cx*cx) - (cy*cy) - (cz*cz);
  
  /* calculation of circumcenter */
  double a3    = (ax - dx) * 2;
  double b3    = (ay - dy) * 2;
  double c3    = (az - dz) * 2;
  double d3    =    (ax*ax) + (ay*ay) + (az*az)
    - (dx*dx) - (dy*dy) - (dz*dz) ;
  double detA  =   (a1*b2*c3) + (a2*b3*c1) + (a3*b1*c2)
    - (a3*b2*c1) - (a2*b1*c3) - (a1*b3*c2) ;
  double detX  =   (d1*b2*c3) + (d2*b3*c1) + (d3*b1*c2)
    - (d3*b2*c1) - (d2*b1*c3) - (d1*b3*c2) ;
  double detY  =   (a1*d2*c3) + (a2*d3*c1) + (a3*d1*c2)
    - (a3*d2*c1) - (a2*d1*c3) - (a1*d3*c2) ;
  double detZ  =   (a1*b2*d3) + (a2*b3*d1) + (a3*b1*d2)
    - (a3*b2*d1) - (a2*b1*d3) - (a1*b3*d2) ;
  double ansX = detX / detA;
  double ansY = detY / detA;
  double ansZ = detZ / detA;

  return WH_Vector3D (ansX, ansY, ansZ);
}

bool WH_contains 
(const vector<WH_Vector3D>& point_s, 
 const WH_Vector3D& point)
{
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    if (WH_eq (point_i, point)) {
      return true;
    }
  }
  return false;
}

bool WH_hasDuplication 
(const vector<WH_Vector3D>& point_s)
{
  for (vector<WH_Vector3D>::const_iterator 
	 i_point = point_s.begin ();
       i_point != point_s.end ();
       i_point++) {
    WH_Vector3D point_i = (*i_point);
    for (vector<WH_Vector3D>::const_iterator 
	   j_point = i_point + 1;
	 j_point != point_s.end ();
	 j_point++) {
      WH_Vector3D point_j = (*j_point);
      if (WH_eq (point_j, point_i)) {
	return true;
      }
    }
  }
  return false;
}



/* WH_Matrix3D class */

WH_Matrix3D
::WH_Matrix3D ()
{
  WH_CVR_LINE;

  v[0][0] = 0;  v[0][1] = 0;  v[0][2] = 0;  v[0][3] = 0;
  v[1][0] = 0;  v[1][1] = 0;  v[1][2] = 0;  v[1][3] = 0;
  v[2][0] = 0;  v[2][1] = 0;  v[2][2] = 0;  v[2][3] = 0;
  v[3][0] = 0;  v[3][1] = 0;  v[3][2] = 0;  v[3][3] = 0;
}

WH_Matrix3D
::WH_Matrix3D 
(double v00, double v01, double v02, double v03,
 double v10, double v11, double v12, double v13,
 double v20, double v21, double v22, double v23,
 double v30, double v31, double v32, double v33)
{
  WH_CVR_LINE;

  v[0][0] = v00;  
  v[0][1] = v01;  
  v[0][2] = v02;
  v[0][3] = v03;

  v[1][0] = v10;  
  v[1][1] = v11;  
  v[1][2] = v12;
  v[1][3] = v13;

  v[2][0] = v20;  
  v[2][1] = v21;  
  v[2][2] = v22;
  v[2][3] = v23;

  v[3][0] = v30;  
  v[3][1] = v31;  
  v[3][2] = v32;
  v[3][3] = v33;
}

ostream& operator<< (ostream& out, const WH_Matrix3D& m)
{
  WH_CVR_LINE;

  out << "WH_Matrix3D ";
  for (int i = 0; i < 4; i++) {
    out << "[ ";
    for (int j = 0; j < 4; j++) {
      out << WH_format (m.v[i][j]) << " ";
    }
    out << "] ";
  }
  return out;
}

WH_Matrix3D WH_Matrix3D
::zero ()
{
  return WH_Matrix3D ();
}

WH_Matrix3D WH_Matrix3D
::identity ()
{
  return WH_Matrix3D (1, 0, 0, 0, 
		      0, 1, 0, 0, 
		      0, 0, 1, 0, 
		      0, 0, 0, 1);
}
  
WH_Matrix3D WH_Matrix3D
::translation (const WH_Vector3D& offset)
{
  WH_CVR_LINE;

  WH_Matrix3D m = WH_Matrix3D::identity ();
  m.v[3][0] = offset.x;
  m.v[3][1] = offset.y;
  m.v[3][2] = offset.z;
  return m;
}

WH_Matrix3D WH_Matrix3D
::scaling (const WH_Vector3D& ratio)
{
  WH_CVR_LINE;

  WH_Matrix3D m = WH_Matrix3D::identity ();
  m.v[0][0] = ratio.x;
  m.v[1][1] = ratio.y;
  m.v[2][2] = ratio.z;
  return m;
}

WH_Matrix3D WH_Matrix3D
::xAxisRotation (double angle)
{
  WH_CVR_LINE;

  WH_Matrix3D m = WH_Matrix3D::identity ();
  double s = sin (angle);  
  double c = cos (angle);
  m.v[1][1] = c;   m.v[1][2] = s;
  m.v[2][1] = -s;  m.v[2][2] = c;
  return m;
}

WH_Matrix3D WH_Matrix3D
::yAxisRotation (double angle)
{
  WH_CVR_LINE;

  WH_Matrix3D m = WH_Matrix3D::identity ();
  double s = sin (angle);  
  double c = cos (angle);
  m.v[0][0] = c;   m.v[0][2] = -s;
  m.v[2][0] = s;   m.v[2][2] = c;
  return m;
}

WH_Matrix3D WH_Matrix3D
::zAxisRotation (double angle)
{
  WH_CVR_LINE;

  WH_Matrix3D m = WH_Matrix3D::identity ();
  double s = sin (angle);  
  double c = cos (angle);
  m.v[0][0] = c;   m.v[0][1] = s;
  m.v[1][0] = -s;  m.v[1][1] = c;
  return m;
}

WH_Matrix3D WH_Matrix3D
::generalAxisRotation (const WH_Vector3D& axisDir, double angle)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (axisDir, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  WH_Vector3D dir = axisDir.normalize ();
  WH_Matrix3D m = WH_Matrix3D::identity ();
  double s = sin (angle);  
  double c = cos (angle);
  m.v[0][0] = c + (1 - c) * dir.x * dir.x;   
  m.v[0][1] = dir.z * s + (1 - c) * dir.x * dir.y;
  m.v[0][2] = -dir.y * s + (1 - c) * dir.x * dir.z;

  m.v[1][0] = -dir.z * s + (1 - c) * dir.x * dir.y;  
  m.v[1][1] = c + (1 - c) * dir.y * dir.y;
  m.v[1][2] = dir.x * s + (1 - c) * dir.y * dir.z;

  m.v[2][0] = dir.y * s + (1 - c) * dir.x * dir.z;  
  m.v[2][1] = -dir.x * s + (1 - c) * dir.y * dir.z;
  m.v[2][2] = c + (1 - c) * dir.z * dir.z;
  return m;
}

WH_Matrix3D operator* (const WH_Matrix3D& m0, const WH_Matrix3D& m1)
{
  WH_CVR_LINE;

  WH_Matrix3D result;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      result.v[i][j] = 0.0;
      for (int k = 0; k < 4; k++) {
	result.v[i][j] += m0.v[i][k] * m1.v[k][j];
      }
    }
  }
  return result;
}



/* class WH_Line3D */

WH_Line3D
::WH_Line3D ()
{
  WH_CVR_LINE;

  _a = 1;
  _b = 0;   
  _c = 0;
  _p = _q = _r = 0;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}
  
WH_Line3D
::WH_Line3D (const WH_Vector3D& p0, const WH_Vector3D& p1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, p1));

  WH_CVR_LINE;

  WH_Vector3D dir = (p1 - p0).normalize ();
  _a = dir.x;  _b = dir.y;  _c = dir.z;

  double parameter = WH_scalarProduct (WH_Vector3D (0, 0, 0) - p0, dir);
  WH_Vector3D origin = p0 + dir * parameter;
  _p = origin.x;  
  _q = origin.y;  
  _r = origin.z;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->contains (p0));
  WH_ASSERT(this->contains (p1));
#endif
}
  
bool WH_Line3D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_eq (WH_Vector3D (_a, _b, _c).length (), 1));

  return true;
}

bool WH_Line3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_Vector3D direction = this->direction ();
  WH_ASSERT(WH_eq (direction.length (), 1));
  WH_ASSERT(WH_eq (direction, 
		   WH_Vector3D (this->a (), this->b (), this->c ())));

  WH_Vector3D origin = this->origin ();
  WH_ASSERT(WH_eq (origin, 
		   WH_Vector3D (this->p (), this->q (), this->r ())));
  WH_ASSERT(this->contains (origin));
  WH_ASSERT(WH_eq (this->distanceFrom (origin), 0));
  WH_ASSERT(WH_eq (this->projectedPoint (origin), origin));
  WH_ASSERT(WH_eq (this->parameterAt (origin), 0));
  WH_ASSERT(WH_eq (this->positionAt (0), origin));

  WH_Vector3D uAxisDir = this->uAxisDir ();
  WH_Vector3D vAxisDir = this->vAxisDir ();
  WH_ASSERT(WH_eq (uAxisDir.length (), 1));
  WH_ASSERT(WH_eq (vAxisDir.length (), 1));
  WH_ASSERT(WH_isPerpendicular (uAxisDir, direction));
  WH_ASSERT(WH_isPerpendicular (vAxisDir, direction));
  WH_ASSERT(WH_isPerpendicular (uAxisDir, vAxisDir));

  WH_Vector3D uPoint = origin + uAxisDir;
  WH_Vector3D vPoint = origin + vAxisDir;
  WH_ASSERT(!this->contains (uPoint));
  WH_ASSERT(!this->contains (vPoint));
  WH_ASSERT(WH_eq (this->distanceFrom (uPoint), 1));
  WH_ASSERT(WH_eq (this->distanceFrom (vPoint), 1));
  WH_ASSERT(WH_eq (this->projectedPoint (uPoint), origin));
  WH_ASSERT(WH_eq (this->projectedPoint (vPoint), origin));

  WH_ASSERT(WH_eq (WH_angleOfLines (*this, *this), 0));

  WH_Vector3D point;
  WH_ASSERT(this->checkIntersectionWith (*this, 
					 point) 
	    == COINCIDENT_WITH_LINE);
  
  return true;
}

ostream& operator<< (ostream& out, const WH_Line3D& line)
{
  WH_CVR_LINE;

  return out << "WH_Line3D " 
    << "(x - " << WH_format (line._p) 
      << " ) / " << WH_format (line._a) 
	<< " = (y - " << WH_format (line._q) 
	  << " ) / " << WH_format (line._b) 
	    << " = (z - " << WH_format (line._r) 
	      << " ) / " << WH_format (line._c) 
		<< " ";
}

bool WH_Line3D
::contains (const WH_Vector3D& position) const
{
  double t = this->parameterAt (position);
  return WH_eq (position, this->positionAt (t));
}

double WH_Line3D
::distanceFrom (const WH_Vector3D& position) const
{
  double t = this->parameterAt (position);
  return WH_distance (position, this->positionAt (t));
}

WH_Vector3D WH_Line3D
::projectedPoint (const WH_Vector3D& position) const
{
  double t = this->parameterAt (position);
  return this->positionAt (t);
}

bool WH_Line3D
::isParallelToXAxis () const
{
  return WH_eq (fabs (_a), 1)
    && WH_eq (_b, 0)
      && WH_eq (_c, 0);
}
  
bool WH_Line3D
::isParallelToYAxis () const
{
  return WH_eq (_a, 0) 
    && WH_eq (fabs (_b), 1)
      && WH_eq (_c, 0);
}

bool WH_Line3D
::isParallelToZAxis () const
{
  return WH_eq (_a, 0)
    && WH_eq (_b, 0)
      && WH_eq (fabs (_c), 1);
}

WH_Vector3D WH_Line3D
::uAxisDir () const
{
  WH_CVR_LINE;

  WH_Vector3D dir = this->direction ();
  if (WH_eq (dir, WH_Vector3D::xUnit ())
      || WH_eq (dir, -WH_Vector3D::xUnit ())) {
    WH_CVR_LINE;
    return WH_Vector3D::yUnit ();
  } else {
    WH_CVR_LINE;
    return WH_vectorProduct (dir, WH_Vector3D::xUnit ()).normalize ();
  }
}

WH_Vector3D WH_Line3D
::vAxisDir () const
{
  return WH_vectorProduct (this->direction (), 
			   this->uAxisDir ()).normalize ();
}

double WH_Line3D
::distanceFrom (const WH_Line3D& line) const
{
  WH_CVR_LINE;

  double result = 0.0;

  double a0 = _a;
  double b0 = _b;
  double c0 = _c;
  double p0 = _p;
  double q0 = _q;
  double r0 = _r;

  double a1 = line.a ();
  double b1 = line.b ();
  double c1 = line.c ();
  double p1 = line.p ();
  double q1 = line.q ();
  double r1 = line.r ();

  double A00 = p0 * p0 + q0 * q0 + r0 * r0;
  double A01 = - p0 * p1 - q0 * q1 - r0 * r1;
  double B0 = p0 * (a0 - a1) + q0 * (b0 - b1) + r0 * (c0 - c1);

  double A10 = - p0 * p1 - q0 * q1 - r0 * r1;
  double A11 = p1 * p1 + q1 * q1 + r1 * r1;
  double B1 = p1 * (a1 - a0) + q1 * (b1 - b0) + r1 * (c1 - c0);

  double t, s;
  bool flag = WH::solveLinearEquation2D (A00, A01, A10, A11, B0, B1,
					 t, s);
  if (flag) {
    WH_CVR_LINE;
    result = WH_distance (this->positionAt (t), line.positionAt (s));
  } else {
    WH_CVR_LINE;
    /* parallel or coinsident */

    WH_Vector3D anyPoint = this->origin ();
    result = WH_distance (anyPoint, line.projectedPoint (anyPoint));
  }

  return result;
}

WH_Line3D::WithLineIntersectionType WH_Line3D
::checkIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithLineIntersectionType result = NO_INTERSECTION_WITH_LINE;

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
    
    double x1 = this->p ();
    double y1 = this->q ();
    double z1 = this->r ();
    double f1 = this->a ();
    double g1 = this->b ();
    double h1 = this->c ();
    
    double x2 = line.p ();
    double y2 = line.q ();
    double z2 = line.r ();
    double f2 = line.a ();
    double g2 = line.b ();
    double h2 = line.c ();

    double fg = f1 * g2 - f2 * g1;
    double gh = g1 * h2 - g2 * h1;
    double hf = h1 * f2 - h2 * f1;
    double denominator = fg * fg + gh * gh + hf * hf;

    double distance 
      = fabs ((x2 - x1) * gh + (y2 - y1) * hf + (z2 - z1) * fg) 
      / sqrt (denominator);
    if (WH_eq (distance, 0)) {
      WH_CVR_LINE;
      /* intersects at point */
      
      /* 
	 f1 * s - f2 * t + (x1 - x2) = 0 
	 g1 * s - g2 * t + (y1 - y2) = 0 
	 h1 * s - h2 * t + (z1 - z2) = 0 
      */
      double s, t;
      bool flag = WH::solveLinearEquation2D 
	(f1, -f2,
	 g1, -g2,
	 x1 - x2, y1 - y2,
	 s, t);
      if (!flag) {
	WH_CVR_LINE;
	flag = WH::solveLinearEquation2D 
	  (g1, -g2,
	   h1, -h2,
	   y1 - y2, z1 - z2, 
	   s, t);
	if (!flag) {
	  WH_CVR_LINE;
	  flag = WH::solveLinearEquation2D 
	    (h1, -h2,
	     f1, -f2,
	     z1 - z2, x1 - x2, 
	     s, t);
	  WH_ASSERT(flag);
	}
      }
      WH_Vector3D p0 = this->positionAt (s);
      WH_Vector3D p1 = line.positionAt (t);
      if (WH_eq (p0, p1)) {
	WH_CVR_LINE;                               /* NOT COVERED */
	point_OUT = p0;
	result = POINT_WITH_LINE;
      } else {
	WH_CVR_LINE;                               /* NOT COVERED */
	result = NO_INTERSECTION_WITH_LINE;
      }
    } else {
      WH_CVR_LINE;
      /* no intersction */
      
      result = NO_INTERSECTION_WITH_LINE;
    }
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_INTERSECTION_WITH_LINE:
    break;
  case POINT_WITH_LINE:
    WH_ASSERT(this->contains (point_OUT));
    WH_ASSERT(line.contains (point_OUT));
    break;
  case PARALLEL_WITH_LINE:
    WH_ASSERT(WH_isParallel (this->direction (), line.direction ()));
    break;
  case COINCIDENT_WITH_LINE:
    WH_ASSERT(WH_isParallel (this->direction (), line.direction ()));
    WH_ASSERT(WH_eq (*this, line) || WH_isReverse (*this, line));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

WH_Line3D::WithPlaneIntersectionType WH_Line3D
::checkIntersectionWith 
(const WH_Plane3D& plane,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithPlaneIntersectionType result = POINT_WITH_PLANE;
  
  WH_Plane3D::WithLineIntersectionType flagWithLine 
    = plane.checkIntersectionWith (*this,
				   point_OUT);
  switch (flagWithLine) {
  case WH_Plane3D::POINT_WITH_LINE:
    WH_CVR_LINE;
    result = POINT_WITH_PLANE;
    break;
  case WH_Plane3D::PARALLEL_WITH_LINE:
    WH_CVR_LINE;
    result = PARALLEL_WITH_PLANE;
    break;
  case WH_Plane3D::COINCIDENT_WITH_LINE:
    WH_CVR_LINE;
    result = COINCIDENT_WITH_PLANE;
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case POINT_WITH_PLANE:
    this->contains (point_OUT);
    plane.contains (point_OUT);
    break;
  case PARALLEL_WITH_PLANE: 
    WH_ASSERT(WH_isPerpendicular (this->direction (), plane.normal ()));
    break;
  case COINCIDENT_WITH_PLANE:
    WH_ASSERT(WH_isPerpendicular (this->direction (), plane.normal ()));
    plane.contains (this->origin ());
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

double WH_angleOfLines (const WH_Line3D& l0, const WH_Line3D& l1)
{
  return WH_angleOfVectors (l0.direction (), l1.direction ());
}



/* class WH_Plane3D */

WH_Plane3D
::WH_Plane3D ()
{
  WH_CVR_LINE;

  _a = 0;
  _b = 0;
  _c = 1;
  _d = 0;

  this->calculateUVAxis ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Plane3D
::WH_Plane3D (double a, double b, double c, double d)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (a, 0) || WH_ne (b, 0) || WH_ne (c, 0));

  WH_CVR_LINE;

  double len = WH_Vector3D (a, b, c).length ();
  WH_ASSERT(WH_ne (len, 0));
  _a = a / len;  
  _b = b / len;  
  _c = c / len;
  _d = d / len;

  this->calculateUVAxis ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Plane3D
::WH_Plane3D 
(const WH_Vector3D& planeNormal, const WH_Vector3D& anyPoint)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (planeNormal, WH_Vector3D::zero ()));

  WH_CVR_LINE;

  WH_Vector3D normal = planeNormal.normalize ();
  _a = normal.x;  
  _b = normal.y;  
  _c = normal.z;
  _d = -(_a * anyPoint.x + _b * anyPoint.y + _c * anyPoint.z);

  this->calculateUVAxis ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->contains (anyPoint));
#endif
}

WH_Plane3D
::WH_Plane3D 
(const WH_Vector3D& p0, const WH_Vector3D& p1, const WH_Vector3D& p2)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (p0, p1));
  WH_ASSERT(WH_ne (p0, p2));
  WH_ASSERT(WH_ne (p1, p2));

  WH_CVR_LINE;

  WH_Vector3D normal 
    = WH_vectorProduct (p1 - p0, p2 - p0).normalize ();
  WH_ASSERT(WH_ne (normal, WH_Vector3D::zero ()));
  _a = normal.x;  
  _b = normal.y;  
  _c = normal.z;
  _d = -(_a * p0.x + _b * p0.y + _c * p0.z);

  this->calculateUVAxis ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->contains (p0));
  WH_ASSERT(this->contains (p1));
  WH_ASSERT(this->contains (p2));
#endif
}

void WH_Plane3D
::calculateUVAxis ()
{
  WH_CVR_LINE;

  WH_Vector3D normal = this->normal ();
  if (WH_eq (normal, WH_Vector3D::yUnit ())
      || WH_eq (normal, -WH_Vector3D::yUnit ())) {
    WH_CVR_LINE;
    _uAxisDir = WH_Vector3D::xUnit ();
  } else {
    WH_CVR_LINE;
    _uAxisDir = WH_vectorProduct (WH_Vector3D::yUnit (), normal).normalize ();
  }

  _vAxisDir = WH_vectorProduct (normal, _uAxisDir).normalize ();
}

bool WH_Plane3D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_Vector3D normal (_a, _b, _c);
  WH_ASSERT(WH_eq (normal.length (), 1));
  WH_ASSERT(WH_eq (_uAxisDir.length (), 1));
  WH_ASSERT(WH_eq (_vAxisDir.length (), 1));
  WH_ASSERT(WH_isPerpendicular (normal, _uAxisDir));
  WH_ASSERT(WH_isPerpendicular (normal, _vAxisDir));
  WH_ASSERT(WH_isPerpendicular (_uAxisDir, _vAxisDir));

  return true;
}

bool WH_Plane3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_Vector3D normal = this->normal ();
  WH_ASSERT(WH_eq (normal.length (), 1));
  WH_ASSERT(WH_eq (normal, 
		   WH_Vector3D (this->a (), this->b (), this->c ())));

  WH_Vector3D uAxisDir = this->uAxisDir ();
  WH_Vector3D vAxisDir = this->vAxisDir ();
  WH_ASSERT(WH_eq (uAxisDir.length (), 1));
  WH_ASSERT(WH_eq (vAxisDir.length (), 1));
  WH_ASSERT(WH_isPerpendicular (normal, uAxisDir));
  WH_ASSERT(WH_isPerpendicular (normal, vAxisDir));
  WH_ASSERT(WH_isPerpendicular (uAxisDir, vAxisDir));

  WH_Vector3D origin = this->origin ();
  WH_ASSERT(this->contains (origin));
  WH_ASSERT(WH_eq (this->valueAt (origin), 0));
  WH_ASSERT(WH_eq (this->distanceFrom (origin), 0));
  WH_ASSERT(WH_eq (this->projectedPoint (origin), origin));
  WH_ASSERT(WH_eq (this->parameterAt (origin), WH_Vector2D::zero ()));
  WH_ASSERT(WH_eq (this->positionAt (WH_Vector2D::zero ()), origin));
  WH_ASSERT(WH_eq (WH_angleOfPlanes (*this, *this), 0));

  WH_Vector3D uPoint = origin + uAxisDir;
  WH_ASSERT(this->contains (uPoint));
  WH_ASSERT(WH_eq (this->valueAt (uPoint), 0));
  WH_ASSERT(WH_eq (this->distanceFrom (uPoint), 0));
  WH_ASSERT(WH_eq (this->projectedPoint (uPoint), uPoint));
  WH_ASSERT(WH_eq (this->parameterAt (uPoint), WH_Vector2D::xUnit ()));
  WH_ASSERT(WH_eq (this->positionAt (WH_Vector2D::xUnit ()), uPoint));

  WH_Vector3D vPoint = origin + vAxisDir;
  WH_ASSERT(this->contains (vPoint));
  WH_ASSERT(WH_eq (this->valueAt (vPoint), 0));
  WH_ASSERT(WH_eq (this->distanceFrom (vPoint), 0));
  WH_ASSERT(WH_eq (this->projectedPoint (vPoint), vPoint));
  WH_ASSERT(WH_eq (this->parameterAt (vPoint), WH_Vector2D::yUnit ()));
  WH_ASSERT(WH_eq (this->positionAt (WH_Vector2D::yUnit ()), vPoint));

  WH_Vector3D wPoint = origin + normal;
  WH_ASSERT(!this->contains (wPoint));
  WH_ASSERT(WH_eq (this->valueAt (wPoint), 1));
  WH_ASSERT(WH_eq (this->distanceFrom (wPoint), 1));
  WH_ASSERT(WH_eq (this->projectedPoint (wPoint), origin));

  WH_Line3D line;
  WH_ASSERT(this->checkIntersectionWith (*this, 
					 line) 
	    == COINCIDENT_WITH_PLANE);
  
  return true;
}

ostream& operator<< (ostream& out, const WH_Plane3D& plane)
{
  WH_CVR_LINE;

  return out << "WH_Plane3D " 
    << WH_format (plane._a) << " x + " 
      << WH_format (plane._b) << " y + " 
	<< WH_format (plane._c) << " z + "
	  << WH_format (plane._d) << " = 0 ";
}

WH_Vector3D WH_Plane3D
::projectedPoint (const WH_Vector3D& position) const
{
  return position - this->normal () * this->valueAt (position);
}
  
bool WH_Plane3D
::isPerpendicularToXAxis () const
{
  return WH_eq (fabs (_a), 1) 
    && WH_eq (_b, 0) 
      && WH_eq (_c, 0);
}

bool WH_Plane3D
::isPerpendicularToYAxis () const
{
  return WH_eq (_a, 0) 
    && WH_eq (fabs (_b), 1) 
      && WH_eq (_c, 0);
}

bool WH_Plane3D
::isPerpendicularToZAxis () const
{
  return WH_eq (_a, 0) 
    && WH_eq (_b, 0)
      && WH_eq (fabs (_c), 1);
}

WH_Line2D WH_Plane3D
::line2D (const WH_Line3D& line) const
{
  WH_CVR_LINE;

  WH_Vector3D origin = line.origin ();
  WH_Vector3D otherPoint = origin + line.direction ();
  return WH_Line2D (this->parameterAt (origin), 
		    this->parameterAt (otherPoint));
}

double WH_angleOfPlaneAndLine 
(const WH_Plane3D& plane, const WH_Line3D& line)
{
  return WH_angleOfVectors (plane.normal (), line.direction ());
}

double WH_angleOfPlanes 
(const WH_Plane3D& pl0, const WH_Plane3D& pl1)
{
  return WH_angleOfVectors (pl0.normal (), pl1.normal ());
}

WH_Plane3D::WithLineIntersectionType WH_Plane3D
::checkIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithLineIntersectionType result = POINT_WITH_LINE;
  
  if (WH_isPerpendicular (line.direction (), this->normal ())) {
    WH_CVR_LINE;
    /* parallel */

    double distance = this->distanceFrom (line.origin ());
    if (WH_eq (distance, 0)) {
      WH_CVR_LINE;
      /* coincident */
      
      result = COINCIDENT_WITH_LINE;
    } else {
      WH_CVR_LINE;
      /* parallel */
      
      result = PARALLEL_WITH_LINE;
    }
  } else {
    WH_CVR_LINE;
    /* intersects at point */

    double a = this->a ();
    double b = this->b ();
    double c = this->c ();
    double d = this->d ();

    double f = line.a ();
    double g = line.b ();
    double h = line.c ();

    double x0 = line.p ();
    double y0 = line.q ();
    double z0 = line.r ();

    double denominator = a * f + b * g + c * h;

    double t = -(a * x0 + b * y0 + c * z0 + d) / denominator;
    point_OUT = line.positionAt (t);
    result = POINT_WITH_LINE;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case POINT_WITH_LINE:
    this->contains (point_OUT);
    line.contains (point_OUT);
    break;
  case PARALLEL_WITH_LINE: 
    WH_ASSERT(WH_isPerpendicular (line.direction (), this->normal ()));
    break;
  case COINCIDENT_WITH_LINE:
    WH_ASSERT(WH_isPerpendicular (line.direction (), this->normal ()));
    this->contains (line.origin ());
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

WH_Plane3D::WithPlaneIntersectionType WH_Plane3D
::checkIntersectionWith 
(const WH_Plane3D& plane,
 WH_Line3D& line_OUT) const
{
  WH_CVR_LINE;

  WithPlaneIntersectionType result = LINE_WITH_PLANE;
  
  if (WH_isParallel (this->normal (), plane.normal ())) {
    WH_CVR_LINE;
    /* parallel */
    
    if (WH_eq (*this, plane) || WH_isReverse (*this, plane)) {
      WH_CVR_LINE;
      result = COINCIDENT_WITH_PLANE;
    } else {
      WH_CVR_LINE;
      result = PARALLEL_WITH_PLANE;
    }
  } else {
    WH_CVR_LINE;
    /* intersects along line */

    double a1 = this->a ();
    double b1 = this->b ();
    double c1 = this->c ();
    double d1 = this->d ();
    double a2 = plane.a ();
    double b2 = plane.b ();
    double c2 = plane.c ();
    double d2 = plane.d ();
  
    double f = b1 * c2 - b2 * c1;
    double g = c1 * a2 - c2 * a1;
    double h = a1 * b2 - a2 * b1;
    double det = f * f + g * g + h * h;

    double da = d1 * a2 - a1 * d2;
    double db = d1 * b2 - b1 * d2;
    double dc = d1 * c2 - c1 * d2;

    /* to avoid comparison using <eps> of order 1 */
    WH_Vector3D direction (f, g, h);
    double length = sqrt (f * f + g * g + h * h);
    direction.x /= length;
    direction.y /= length;
    direction.z /= length;

    WH_Vector3D origin ((g * dc - h * db) / det,
			(h * da - f * dc) / det,
			(f * db - g * da) / det);
    line_OUT = WH_Line3D (origin, origin + direction);
    result = LINE_WITH_PLANE;
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case LINE_WITH_PLANE:
    {
      WH_Vector3D point;
      WithLineIntersectionType flag 
	= this->checkIntersectionWith (line_OUT,
				       point);
      WH_ASSERT(flag == COINCIDENT_WITH_LINE);
      flag = plane.checkIntersectionWith (line_OUT,
					  point);
      WH_ASSERT(flag == COINCIDENT_WITH_LINE);
    }
    break;
  case PARALLEL_WITH_PLANE: 
    WH_ASSERT(WH_isParallel (this->normal (), plane.normal ()));
    break;
  case COINCIDENT_WITH_PLANE:
    WH_ASSERT(WH_isParallel (this->normal (), plane.normal ()));
    WH_ASSERT(WH_eq (*this, plane) || WH_isReverse (*this, plane));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}



#ifndef WH_INLINE_ENABLED
#include "space3d_inline.cc"
#endif



/* not covered yet
double angleOfVectors 
(const WH_Vector3D& p0, const WH_Vector3D& p1)

WH_Line3D::WithLineIntersectionType WH_Line3D
::checkIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point_OUT) const
*/
