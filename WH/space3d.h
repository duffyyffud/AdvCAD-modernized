/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for space3d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif

class WH_Vector3D;
class WH_Matrix3D;
class WH_Line3D;
class WH_Plane3D;

/* value-based class */
/* no inheritance */
struct WH_Vector3D {
  double x, y, z;

  static WH_Vector3D zero ();
  static WH_Vector3D xUnit ();
  static WH_Vector3D yUnit ();
  static WH_Vector3D zUnit ();
  static WH_Vector3D hugeValue ();

  WH_Vector3D ();
  WH_Vector3D (double x, double y, double z); 
  friend ostream& operator<< (ostream& out, const WH_Vector3D& p);

  WH_Vector3D operator- () const; 
  const WH_Vector3D& operator+= (const WH_Vector3D& p); 
  const WH_Vector3D& operator-= (const WH_Vector3D& p); 
  const WH_Vector3D& operator*= (double ratio); 
  const WH_Vector3D& operator/= (double ratio); 
  double& operator[] (int index); 
  double operator[] (int index) const; 
  double length () const; 
  WH_Vector3D normalize () const;
  WH_Vector3D translate (const WH_Vector3D& offset) const;
  WH_Vector3D rotateByXAxis 
    (const WH_Vector3D& center, double angle) const;
  WH_Vector3D rotateByYAxis 
    (const WH_Vector3D& center, double angle) const;
  WH_Vector3D rotateByZAxis 
    (const WH_Vector3D& center, double angle) const;
  WH_Vector3D scale 
    (const WH_Vector3D& center, double ratio) const;
  WH_Vector3D scaleAlongXAxis 
    (const WH_Vector3D& center, double ratio) const;
  WH_Vector3D scaleAlongYAxis 
    (const WH_Vector3D& center, double ratio) const;
  WH_Vector3D scaleAlongZAxis 
    (const WH_Vector3D& center, double ratio) const;
};

/* non-inline free functions using WH_Vector3D */

bool WH_between 
(const WH_Vector3D& p, 
 const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_justBetween 
(const WH_Vector3D& p, 
 const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_between2 
(const WH_Vector3D& p, 
 const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_justBetween2 
(const WH_Vector3D& p, 
 const WH_Vector3D& p0, const WH_Vector3D& p1);
double WH_sinOfVectors (const WH_Vector3D& p0, const WH_Vector3D& p1);
double WH_cosOfVectors (const WH_Vector3D& p0, const WH_Vector3D& p1);
double WH_angleOfVectors (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_getScaleIfParallel (const WH_Vector3D& p0, const WH_Vector3D& p1,
			    double& ration_OUT);
bool WH_valuesOverlap 
(const WH_Vector3D& value0_0, const WH_Vector3D& value0_1, 
 const WH_Vector3D& value1_0, const WH_Vector3D& value1_1);
WH_Vector3D WH_circumcenterAmong 
(const WH_Vector3D& a, 
 const WH_Vector3D& b, 
 const WH_Vector3D& c, 
 const WH_Vector3D& d);
bool WH_contains 
(const vector<WH_Vector3D>& point_s, 
 const WH_Vector3D& point);
bool WH_hasDuplication 
(const vector<WH_Vector3D>& point_s);

/* inline free functions using WH_Vector3D */
#ifndef WH_INLINE_ENABLED  

WH_Vector3D WH_abs (const WH_Vector3D& p); 
WH_Vector3D operator+ (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D operator- (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D operator* (const WH_Vector3D& p0, double ratio);
WH_Vector3D operator/ (const WH_Vector3D& p0, double ratio);
double WH_scalarProduct (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D WH_vectorProduct (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D WH_multiply (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D WH_divide (const WH_Vector3D& p0, const WH_Vector3D& p1);

bool WH_eq (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_ne (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool operator== (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool operator!= (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_lt (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_gt (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_le (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_ge (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_eq2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_ne2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_lt2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_gt2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_le2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_ge2 (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D WH_min (const WH_Vector3D& p0, const WH_Vector3D& p1);
WH_Vector3D WH_max (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_isReverse (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_isParallel (const WH_Vector3D& p0, const WH_Vector3D& p1);
bool WH_isPerpendicular (const WH_Vector3D& p0, const WH_Vector3D& p1);

double WH_distance (const WH_Vector3D& p0, const WH_Vector3D& p1);
double WH_squareSum (const WH_Vector3D& p0, const WH_Vector3D& p1);
void WH_swap (WH_Vector3D& p0, WH_Vector3D& p1);
WH_Vector3D WH_interpolate 
(const WH_Vector3D& p0, const WH_Vector3D& p1, double ratio);
void WH_sortPair 
(WH_Vector3D& value0_IO, WH_Vector3D& value1_IO); 
bool WH_minMaxPairsOverlap 
(const WH_Vector3D& min0, const WH_Vector3D& max0, 
 const WH_Vector3D& min1, const WH_Vector3D& max1);

#endif
/* value-based class */
/* no inheritance */
struct WH_Matrix3D {
  double v[4][4];

  WH_Matrix3D ();
  WH_Matrix3D 
    (double v00, double v01, double v02, double v03,
     double v10, double v11, double v12, double v13,
     double v20, double v21, double v22, double v23,
     double v30, double v31, double v32, double v33);
  friend ostream& operator<< (ostream& out, const WH_Matrix3D& m);

  static WH_Matrix3D zero ();
  static WH_Matrix3D identity ();
  static WH_Matrix3D translation (const WH_Vector3D& offset);
  static WH_Matrix3D scaling (const WH_Vector3D& ratio);
  static WH_Matrix3D xAxisRotation (double angle);
  static WH_Matrix3D yAxisRotation (double angle);
  static WH_Matrix3D zAxisRotation (double angle);
  static WH_Matrix3D generalAxisRotation 
    (const WH_Vector3D& axisDir, double angle);
};

/* non-inline free functions using WH_Matrix3D */

WH_Matrix3D operator* (const WH_Matrix3D& m0, const WH_Matrix3D& m1);

/* inline free functions using WH_Matrix3D */
#ifndef WH_INLINE_ENABLED  

WH_Vector3D operator* (const WH_Vector3D& p, const WH_Matrix3D& m);

#endif

/* value-based class */
/* no inheritance */
class WH_Line3D {
 public:
  static WH_Line3D xAxis ();
  static WH_Line3D yAxis ();
  static WH_Line3D zAxis ();

  WH_Line3D ();
  WH_Line3D (const WH_Vector3D& p0, const WH_Vector3D& p1);
  bool checkInvariant () const;
  bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Line3D& line);

  double a () const;
  double b () const;
  double c () const;
  double p () const;
  double q () const;
  double r () const;
  WH_Vector3D direction () const;
  bool contains (const WH_Vector3D& position) const;
  double distanceFrom (const WH_Vector3D& position) const;
  double distanceFrom (const WH_Line3D& line) const;
  WH_Vector3D projectedPoint (const WH_Vector3D& position) const;
  bool isParallelToXAxis () const;
  bool isParallelToYAxis () const;
  bool isParallelToZAxis () const;
  WH_Vector3D origin () const;
  WH_Vector3D uAxisDir () const;
  WH_Vector3D vAxisDir () const;
  double parameterAt (const WH_Vector3D& position) const;
  WH_Vector3D positionAt (double parameter) const;
  
  enum WithLineIntersectionType {
    NO_INTERSECTION_WITH_LINE, 
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  WithLineIntersectionType checkIntersectionWith 
    (const WH_Line3D& line,
     WH_Vector3D& point_OUT) const;
  enum WithPlaneIntersectionType {
    POINT_WITH_PLANE, 
    PARALLEL_WITH_PLANE, 
    COINCIDENT_WITH_PLANE
  };
  WithPlaneIntersectionType checkIntersectionWith 
    (const WH_Plane3D& plane,
     WH_Vector3D& point_OUT) const;

 private:
  double _a, _b, _c;
  double _p, _q, _r;
  /* (x - p) / a = (y - q) / b = (z - r) / c */

};

/* non-inline free functions using WH_Line3D */

double WH_angleOfLines (const WH_Line3D& l0, const WH_Line3D& l1);

/* inline free functions using WH_Line3D */
#ifndef WH_INLINE_ENABLED  

bool WH_eq (const WH_Line3D& l0, const WH_Line3D& l1);
bool WH_ne (const WH_Line3D& l0, const WH_Line3D& l1);
bool WH_isReverse (const WH_Line3D& l0, const WH_Line3D& l1);

#endif

/* value-based class */
/* no inheritance */
class WH_Plane3D {
 public:
  static WH_Plane3D xyPlane ();
  static WH_Plane3D yzPlane ();
  static WH_Plane3D zxPlane ();
  
  WH_Plane3D ();
  WH_Plane3D (double a, double b, double c, double d);
  WH_Plane3D (const WH_Vector3D& planeNormal, const WH_Vector3D& anyPoint);
  WH_Plane3D (const WH_Vector3D& p0, 
	      const WH_Vector3D& p1, 
	      const WH_Vector3D& p2);
  bool checkInvariant () const;
  bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Plane3D& plane);

  double a () const;
  double b () const;
  double c () const;
  double d () const;
  WH_Vector3D normal () const;
  double valueAt (const WH_Vector3D& position) const;
  bool contains (const WH_Vector3D& position) const;
  double distanceFrom (const WH_Vector3D& position) const;
  WH_Vector3D projectedPoint (const WH_Vector3D& position) const;
  bool isPerpendicularToXAxis () const;
  bool isPerpendicularToYAxis () const;
  bool isPerpendicularToZAxis () const;
  WH_Vector3D origin () const;
  WH_Vector3D uAxisDir () const;
  WH_Vector3D vAxisDir () const;
  WH_Vector2D parameterAt (const WH_Vector3D& position) const;
  WH_Vector3D positionAt (const WH_Vector2D& parameter) const;
  WH_Line2D line2D (const WH_Line3D& line) const;
  enum WithLineIntersectionType {
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  WithLineIntersectionType checkIntersectionWith 
    (const WH_Line3D& line,
     WH_Vector3D& point_OUT) const;
  enum WithPlaneIntersectionType {
    LINE_WITH_PLANE, 
    PARALLEL_WITH_PLANE, 
    COINCIDENT_WITH_PLANE
  };
  WithPlaneIntersectionType checkIntersectionWith 
    (const WH_Plane3D& plane,
     WH_Line3D& line_OUT) const;

 private:
  double _a, _b, _c, _d;
  /* ax + by + cz + d = 0 */

  WH_Vector3D _uAxisDir;
  WH_Vector3D _vAxisDir;

  /* initialization only */
  void calculateUVAxis ();
  
};

/* non-inline free functions using WH_Plane3D */

double WH_angleOfPlaneAndLine 
(const WH_Plane3D& plane, const WH_Line3D& line);
double WH_angleOfPlanes 
(const WH_Plane3D& pl0, const WH_Plane3D& pl1);

/* inline free functions using WH_Plane3D */
#ifndef WH_INLINE_ENABLED  

bool WH_eq (const WH_Plane3D& pl0, const WH_Plane3D& pl1);
bool WH_ne (const WH_Plane3D& pl0, const WH_Plane3D& pl1);
bool WH_isReverse (const WH_Plane3D& pl0, const WH_Plane3D& pl1);

#endif

#ifdef WH_INLINE_ENABLED
#include <WH/space3d_inline.cc>
#endif
