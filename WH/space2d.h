/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for space2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SCALAR
#include <WH/scalar.h>
#define WH_INCLUDED_WH_SCALAR
#endif

class WH_Vector2D;
class WH_Matrix2D;
class WH_Line2D;

/* value-based class */
/* no inheritance */
struct WH_Vector2D {
  double x, y;

  static WH_Vector2D zero ();
  static WH_Vector2D xUnit ();
  static WH_Vector2D yUnit ();
  static WH_Vector2D hugeValue ();

  WH_Vector2D ();
  WH_Vector2D (double x, double y); 
  friend ostream& operator<< (ostream& out, const WH_Vector2D& p);

  WH_Vector2D operator- () const; 
  const WH_Vector2D& operator+= (const WH_Vector2D& p); 
  const WH_Vector2D& operator-= (const WH_Vector2D& p); 
  const WH_Vector2D& operator*= (double ratio); 
  const WH_Vector2D& operator/= (double ratio); 
  double& operator[] (int index); 
  double operator[] (int index) const; 
  double length () const; 
  WH_Vector2D normalize () const;
  WH_Vector2D translate (const WH_Vector2D& offset) const;
  WH_Vector2D rotate (const WH_Vector2D& center, double angle) const;
  WH_Vector2D scale (const WH_Vector2D& center, double ratio) const;
  double slope () const;
};

/* non-inline free functions using WH_Vector2D */

bool WH_between 
(const WH_Vector2D& p, 
 const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_justBetween 
(const WH_Vector2D& p, 
 const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_between2 
(const WH_Vector2D& p, 
 const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_justBetween2 
(const WH_Vector2D& p, 
 const WH_Vector2D& p0, const WH_Vector2D& p1);
double WH_angleOfVectors (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_getScaleIfParallel (const WH_Vector2D& p0, const WH_Vector2D& p1,
			    double& ration_OUT);
bool WH_valuesOverlap 
(const WH_Vector2D& value0_0, const WH_Vector2D& value0_1, 
 const WH_Vector2D& value1_0, const WH_Vector2D& value1_1);
WH_Vector2D WH_circumcenterAmong 
(const WH_Vector2D& p0, 
 const WH_Vector2D& p1, 
 const WH_Vector2D& p2);
bool WH_contains 
(const vector<WH_Vector2D>& point_s, 
 const WH_Vector2D& point);
bool WH_hasDuplication 
(const vector<WH_Vector2D>& point_s);

/* inline free functions using WH_Vector2D */
#ifndef WH_INLINE_ENABLED  

WH_Vector2D WH_abs (const WH_Vector2D& p); 
WH_Vector2D operator+ (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D operator- (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D operator* (const WH_Vector2D& p0, double ratio);
WH_Vector2D operator/ (const WH_Vector2D& p0, double ratio);
double WH_scalarProduct (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D WH_multiply (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D WH_divide (const WH_Vector2D& p0, const WH_Vector2D& p1);

bool WH_eq (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_ne (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool operator== (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool operator!= (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_lt (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_gt (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_le (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_ge (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_eq2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_ne2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_lt2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_gt2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_le2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_ge2 (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D WH_min (const WH_Vector2D& p0, const WH_Vector2D& p1);
WH_Vector2D WH_max (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_isReverse (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_isParallel (const WH_Vector2D& p0, const WH_Vector2D& p1);
bool WH_isPerpendicular (const WH_Vector2D& p0, const WH_Vector2D& p1);

double WH_distance (const WH_Vector2D& p0, const WH_Vector2D& p1);
double WH_squareSum (const WH_Vector2D& p0, const WH_Vector2D& p1);
void WH_swap (WH_Vector2D& p0, WH_Vector2D& p1);
WH_Vector2D WH_interpolate 
(const WH_Vector2D& p0, const WH_Vector2D& p1, double ratio);
void WH_sortPair 
(WH_Vector2D& value0_IO, WH_Vector2D& value1_IO); 
bool WH_minMaxPairsOverlap 
(const WH_Vector2D& min0, const WH_Vector2D& max0, 
 const WH_Vector2D& min1, const WH_Vector2D& max1);
double WH_signedTriangleAreaAmong 
(const WH_Vector2D& p0, 
 const WH_Vector2D& p1, 
 const WH_Vector2D& p2);

#endif

/* value-based class */
/* no inheritance */
struct WH_Matrix2D {
  double v[3][3];

  WH_Matrix2D ();
  WH_Matrix2D 
    (double v00, double v01, double v02,
     double v10, double v11, double v12,
     double v20, double v21, double v22);
  friend ostream& operator<< (ostream& out, const WH_Matrix2D& m);

  static WH_Matrix2D zero ();
  static WH_Matrix2D identity ();
  static WH_Matrix2D translation (const WH_Vector2D& offset);
  static WH_Matrix2D scaling (const WH_Vector2D& ratio);
  static WH_Matrix2D rotation (double angle);
};

/* non-inline free functions using WH_Matrix2D */

WH_Matrix2D operator* (const WH_Matrix2D& m0, const WH_Matrix2D& m1);

/* inline free functions using WH_Matrix2D */
#ifndef WH_INLINE_ENABLED  

WH_Vector2D operator* (const WH_Vector2D& p, const WH_Matrix2D& m);

#endif

/* value-based class */
/* no inheritance */
class WH_Line2D {
 public:
  static WH_Line2D xAxis ();
  static WH_Line2D yAxis ();

  WH_Line2D ();
  WH_Line2D (double a, double b, double c);
  WH_Line2D (const WH_Vector2D& p0, const WH_Vector2D& p1);
  bool checkInvariant () const;
  bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Line2D& line);

  double a () const;
  double b () const;
  double c () const;
  WH_Vector2D normal () const;
  WH_Vector2D direction () const;
  double valueAt (const WH_Vector2D& position) const;
  bool contains (const WH_Vector2D& position) const;
  double distanceFrom (const WH_Vector2D& position) const;
  WH_Vector2D projectedPoint (const WH_Vector2D& position) const;
  bool isParallelToXAxis () const;
  bool isParallelToYAxis () const;
  WH_Vector2D origin () const;
  double parameterAt (const WH_Vector2D& position) const;
  WH_Vector2D positionAt (double parameter) const;
  enum WithLineIntersectionType {
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  WithLineIntersectionType checkIntersectionWith 
    (const WH_Line2D& line,
     WH_Vector2D& point_OUT) const;

 private:
  double _a, _b, _c;
  /* ax + by + c = 0 */
};

/* non-inline free functions using WH_Line2D */

double WH_angleOfLines (const WH_Line2D& l0, const WH_Line2D& l1);

/* inline free functions using WH_Line2D */
#ifndef WH_INLINE_ENABLED  

bool WH_eq (const WH_Line2D& l0, const WH_Line2D& l1);
bool WH_ne (const WH_Line2D& l0, const WH_Line2D& l1);
bool WH_isReverse (const WH_Line2D& l0, const WH_Line2D& l1);

#endif

#ifdef WH_INLINE_ENABLED
#include <WH/space2d_inline.cc>
#endif
