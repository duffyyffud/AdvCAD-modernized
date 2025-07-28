/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* scalar.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "scalar.h"



/* class WH */

const double WH::HUGE_VALUE = 1.0e+20;

double WH::eps = 0.0000001;

void WH
::sortValuesInAscendantOrder 
(int nValues, double values_IO[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[j], values_IO[i])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i], values_IO[i + 1]));
  }
#endif  
}

void WH
::sortIndexsInAscendantOrder
(int nValues, double values_IO[], int indexs_OUT[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues; i++) {
    indexs_OUT[i] = i;
  }
  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[j], values_IO[i])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
	WH_swap (indexs_OUT[i], indexs_OUT[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i], values_IO[i + 1]));
  }
#endif  
}

void WH
::sortPointersInAscendantOrder
(int nValues, double values_IO[], void* pointers_IO[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[j], values_IO[i])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
	WH_swap (pointers_IO[i], pointers_IO[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i], values_IO[i + 1]));
  }
#endif  
}

void WH
::sortValuesInDescendantOrder 
(int nValues, double values_IO[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[i], values_IO[j])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i + 1], values_IO[i]));
  }
#endif  
}

void WH
::sortIndexsInDescendantOrder
(int nValues, double values_IO[], int indexs_OUT[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues; i++) {
    indexs_OUT[i] = i;
  }
  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[i], values_IO[j])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
	WH_swap (indexs_OUT[i], indexs_OUT[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i + 1], values_IO[i]));
  }
#endif  
}

void WH
::sortPointersInDescendantOrder
(int nValues, double values_IO[], void* pointers_IO[])
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= nValues);

  WH_CVR_LINE;

  for (int i = 0; i < nValues - 1; i++) {
    for (int j = i + 1; j < nValues; j++) {
      if (WH_lt (values_IO[i], values_IO[j])) {
	WH_CVR_LINE;
	WH_swap (values_IO[i], values_IO[j]);
	WH_swap (pointers_IO[i], pointers_IO[j]);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (int i = 0; i < nValues - 1; i++) {
    WH_ASSERT(WH_le (values_IO[i + 1], values_IO[i]));
  }
#endif  
}

WH::OverlapType WH
::checkOverlapOfMinMaxPairs 
(double min0, double max0, 
 double min1, double max1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (min0, max0));
  WH_ASSERT(WH_lt (min1, max1));

  WH_CVR_LINE;

  if (WH_lt (max0, min1)) {
    WH_CVR_LINE;
    return HAS_NO_OVERLAP_MIN0_MAX0_MIN1_MAX1;
  } else if (WH_lt (max1, min0)) {
    WH_CVR_LINE;
    return HAS_NO_OVERLAP_MIN1_MAX1_MIN0_MAX0;
  } else if (WH_eq (max0, min1)) {
    WH_CVR_LINE;
    return TOUCHES_MIN0_MAX0_AS_MIN1_MAX1;
  } else if (WH_eq (max1, min0)) {
    WH_CVR_LINE;
    return TOUCHES_MIN1_MAX1_AS_MIN0_MAX0;
  } else if (WH_eq (min0, min1) && WH_eq (max0, max1)) {
    WH_CVR_LINE;
    return MATCHES_MIN0_AS_MIN1_MAX0_AS_MAX1;
  } else if (WH_lt (min0, min1) && WH_lt (min1, max0) && WH_lt (max0, max1)) {
    WH_CVR_LINE;
    return INTERSECTS_MIN0_MIN1_MAX0_MAX1;
  } else if (WH_lt (min1, min0) && WH_lt (min0, max1) && WH_lt (max1, max0)) {
    WH_CVR_LINE;
    return INTERSECTS_MIN1_MIN0_MAX1_MAX0;
  } else if (WH_lt (min0, min1) && WH_lt (max1, max0)) {
    WH_CVR_LINE;
    return CONTAINS_MIN0_MIN1_MAX1_MAX0;
  } else if (WH_lt (min1, min0) && WH_lt (max0, max1)) {
    WH_CVR_LINE;
    return IS_CONTAINED_MIN1_MIN0_MAX0_MAX1;
  } else if (WH_eq (min0, min1) && WH_lt (min1, max1) && WH_lt (max1, max0)) {
    WH_CVR_LINE;
    return CONTAINS_MIN0_AS_MIN1_MAX1_MAX0;
  } else if (WH_lt (min0, min1) && WH_lt (min1, max1) && WH_eq (max1, max0)) {
    WH_CVR_LINE;
    return CONTAINS_MIN0_MIN1_MAX1_AS_MAX0;
  } else if (WH_eq (min1, min0) && WH_lt (min0, max0) && WH_lt (max0, max1)) {
    WH_CVR_LINE;
    return IS_CONTAINED_MIN1_AS_MIN0_MAX0_MAX1;
  } else if (WH_lt (min1, min0) && WH_lt (min0, max0) && WH_eq (max0, max1)) {
    WH_CVR_LINE;
    return IS_CONTAINED_MIN1_MIN0_MAX0_AS_MAX1;
  } else {
    WH_ASSERT_NO_REACH;
    return HAS_NO_OVERLAP_MIN0_MAX0_MIN1_MAX1;
  }
}

WH::SecondOrderEquationSolutionType WH
::solveSecondOrderEquation 
(double a, double b, double c,
 double& x0_OUT, double& x1_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (a, 0));
  
  WH_CVR_LINE;

  /* a x^2 + b x + c = 0 */
  
  SecondOrderEquationSolutionType result = IMAGINARY;

  double b2_4ac = b * b - 4 * a * c;
  if (WH_eq2 (b2_4ac, 0)) {
    WH_CVR_LINE;
    x0_OUT = x1_OUT = -b / (2 * a);

    /* satisfy post-condition */
    if (WH_eq (a * x0_OUT * x0_OUT + b * x0_OUT + c, 0)) {
      WH_CVR_LINE;
      result = DUPLICATED_REAL;
    }
  }

  if (result != DUPLICATED_REAL) {
    if (0 < b2_4ac) {
      WH_CVR_LINE;
      if (0 < a) {
	WH_CVR_LINE;
	x0_OUT = (-b - sqrt (b2_4ac)) / (2 * a);
	x1_OUT = (-b + sqrt (b2_4ac)) / (2 * a);
      } else {
	WH_CVR_LINE;
	x0_OUT = (-b + sqrt (b2_4ac)) / (2 * a);
	x1_OUT = (-b - sqrt (b2_4ac)) / (2 * a);
      }

      /* satisfy post-condition */
      if (WH_eq (a * x0_OUT * x0_OUT + b * x0_OUT + c, 0)
	  && WH_eq (a * x1_OUT * x1_OUT + b * x1_OUT + c, 0)) {
	WH_CVR_LINE;
	result = REAL;
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case REAL:
    WH_ASSERT(WH_eq (a * x0_OUT * x0_OUT + b * x0_OUT + c, 0));
    WH_ASSERT(WH_eq (a * x1_OUT * x1_OUT + b * x1_OUT + c, 0));
    WH_ASSERT(x0_OUT < x1_OUT);
    break;
  case DUPLICATED_REAL:
    WH_ASSERT(WH_eq (a * x0_OUT * x0_OUT + b * x0_OUT + c, 0));
    WH_ASSERT(WH_eq (a * x1_OUT * x1_OUT + b * x1_OUT + c, 0));
    WH_ASSERT(WH_eq (x0_OUT, x1_OUT));
    break;
  case IMAGINARY:
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif
  
  return result;
}

bool WH
::solveLinearEquation2D 
(double a00, double a01,
 double a10, double a11, 
 double b0, double b1,
 double& x_OUT, double& y_OUT)
{
  bool result = false;

  /* a00 x + a01 y + b0 = 0 */
  /* a10 x + a11 y + b1 = 0 */

  WH_CVR_LINE;

  double det = determinant2D 
    (a00, a01, 
     a10, a11);
  if (WH_ne2 (det, 0)) {
    WH_CVR_LINE;
    x_OUT = (a01 * b1 - a11 * b0) / det;
    y_OUT = (a10 * b0 - a00 * b1) / det;

    /* satisfy post-condition */
    if (WH_eq (a00 * x_OUT + a01 * y_OUT + b0, 0)
	&& WH_eq (a10 * x_OUT + a11 * y_OUT + b1, 0)) {
      WH_CVR_LINE;
      result = true;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(WH_eq (a00 * x_OUT + a01 * y_OUT + b0, 0));
    WH_ASSERT(WH_eq (a10 * x_OUT + a11 * y_OUT + b1, 0));
  }
#endif

  return result;
}

bool WH
::solveLinearEquation3D 
(double a00, double a01, double a02,
 double a10, double a11, double a12, 
 double a20, double a21, double a22, 
 double b0, double b1, double b2,
 double& x_OUT, double& y_OUT, double& z_OUT)
{
  bool result = false;

  WH_CVR_LINE;

  /* a00 x + a01 y + a02 z + b0 = 0 */
  /* a10 x + a11 y + a12 z + b1 = 0 */
  /* a20 x + a21 y + a22 z + b2 = 0 */

  double det = determinant3D 
    (a00, a01, a02, 
     a10, a11, a12,
     a20, a21, a22);
  if (WH_ne2 (det, 0)) {
    WH_CVR_LINE;
    double bc = a11 * a22 - a21 * a12;
    double ac = a10 * a22 - a20 * a12;
    double ab = a10 * a21 - a20 * a11;
    double dc = b1 * a22 - b2 * a12;
    double db = b1 * a21 - b2 * a11;
    double ad = a10 * b2 - a20 * b1;
    x_OUT = (a01 * dc - b0 * bc - a02 * db) / det;
    y_OUT = (b0 * ac - a00 * dc - a02 * ad) / det;
    z_OUT = (a01 * ad + a00 * db - b0 * ab) / det;

    /* satisfy post-condition */
    if (WH_eq (a00 * x_OUT + a01 * y_OUT + a02 * z_OUT + b0, 0)
	&& WH_eq (a10 * x_OUT + a11 * y_OUT + a12 * z_OUT + b1, 0)
	&& WH_eq (a20 * x_OUT + a21 * y_OUT + a22 * z_OUT + b2, 0)) {
      WH_CVR_LINE;
      result = true;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (result) {
    WH_ASSERT(WH_eq (a00 * x_OUT + a01 * y_OUT + a02 * z_OUT + b0, 0));
    WH_ASSERT(WH_eq (a10 * x_OUT + a11 * y_OUT + a12 * z_OUT + b1, 0));
    WH_ASSERT(WH_eq (a20 * x_OUT + a21 * y_OUT + a22 * z_OUT + b2, 0));
  }
#endif

  return result;
}



/* free functions */

double WH_rangedValue (double value, double value0, double value1) 
{
  WH_CVR_LINE;

  if (value0 < value1) {
    WH_CVR_LINE;
    if (value < value0) {
      WH_CVR_LINE;
      return value0;
    } else if (value1 < value) {
      WH_CVR_LINE;
      return value1;
    } else {
      WH_CVR_LINE;
      return value;
    }
  } else {
    WH_CVR_LINE;
    if (value < value1) {
      WH_CVR_LINE;
      return value1;
    } else if (value0 < value) {
      WH_CVR_LINE;
      return value0;
    } else {
      WH_CVR_LINE;
      return value;
    }
  }
}

double WH_format (double value) 
{
  return fabs (value) < WH::eps ? 0.0 : value;
}

double WH_format2 (double value) 
{
  return fabs (value) < WH::eps * WH::eps ? 0.0 : value;
}

double WH_format3 (double value) 
{
  return fabs (value) < WH::eps * WH::eps * WH::eps ? 0.0 : value;
}

bool WH_valuesOverlap 
(double value0_0, double value0_1, 
 double value1_0, double value1_1)
{
  WH_CVR_LINE;
  WH_sortPair (value0_0, value0_1);
  WH_sortPair (value1_0, value1_1);
  return WH_minMaxPairsOverlap (value0_0, value0_1, value1_0, value1_1);
}



#ifndef WH_INLINE_ENABLED
#include "scalar_inline.cc"
#endif



/* test coverage completed */
