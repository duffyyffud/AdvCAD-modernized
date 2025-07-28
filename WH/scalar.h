/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for scalar.cc */

#pragma once
#ifndef WH_INCLUDED_WH_COMMON
#include <WH/common.h>
#define WH_INCLUDED_WH_COMMON
#endif

struct WH;

/* inline / non-inline free functions */

void WH_swap (char& value0_IO, char& value1_IO); 
void WH_swap (int& value0_IO, int& value1_IO); 
void WH_swap (double& value0_IO, double& value1_IO); 
void WH_swap (void*& value0_IO, void*& value1_IO); 

bool WH_eq (double value0, double value1); 
bool WH_ne (double value0, double value1);
bool WH_ge (double value0, double value1);
bool WH_le (double value0, double value1);
bool WH_gt (double value0, double value1);
bool WH_lt (double value0, double value1);
bool WH_between (double value, double value0, double value1);
bool WH_justBetween (double value, double value0, double value1);

bool WH_eq2 (double value0, double value1); 
bool WH_ne2 (double value0, double value1);
bool WH_ge2 (double value0, double value1);
bool WH_le2 (double value0, double value1);
bool WH_gt2 (double value0, double value1);
bool WH_lt2 (double value0, double value1);
bool WH_between2 (double value, double value0, double value1);
bool WH_justBetween2 (double value, double value0, double value1);

bool WH_eq3 (double value0, double value1); 
bool WH_ne3 (double value0, double value1);
bool WH_ge3 (double value0, double value1);
bool WH_le3 (double value0, double value1);
bool WH_gt3 (double value0, double value1);
bool WH_lt3 (double value0, double value1);
bool WH_between3 (double value, double value0, double value1);
bool WH_justBetween3 (double value, double value0, double value1);

constexpr double WH_min (double value0, double value1); 
constexpr double WH_max (double value0, double value1); 
constexpr double WH_interpolate (double value0, double value1, double ratio); 
double WH_rangedValue (double value, double value0, double value1); 
double WH_format (double value);
double WH_format2 (double value);
double WH_format3 (double value);

void WH_sortPair 
(double& value0_IO, double& value1_IO); 
bool WH_minMaxPairsOverlap 
(double min0, double max0, 
 double min1, double max1);
bool WH_valuesOverlap 
(double value0_0, double value0_1, 
 double value1_0, double value1_1);

/* utility function set class of WH library */
struct WH {
  static const double HUGE_VALUE;
  
  static double eps;

  static void sortValuesInAscendantOrder 
    (int nValues, double values_IO[]); 
  static void sortIndexsInAscendantOrder
    (int nValues, double values_IO[], int indexs_OUT[]); 
  static void sortPointersInAscendantOrder
    (int nValues, double values_IO[], void* pointers_IO[]); 
  static void sortValuesInDescendantOrder 
    (int nValues, double values_IO[]); 
  static void sortIndexsInDescendantOrder
    (int nValues, double values_IO[], int indexs_OUT[]); 
  static void sortPointersInDescendantOrder
    (int nValues, double values_IO[], void* pointers_IO[]); 

  enum OverlapType {
    HAS_NO_OVERLAP_MIN0_MAX0_MIN1_MAX1, 
    TOUCHES_MIN0_MAX0_AS_MIN1_MAX1, 
    INTERSECTS_MIN0_MIN1_MAX0_MAX1, 
    CONTAINS_MIN0_MIN1_MAX1_MAX0, 
    CONTAINS_MIN0_AS_MIN1_MAX1_MAX0, 
    CONTAINS_MIN0_MIN1_MAX1_AS_MAX0, 
    MATCHES_MIN0_AS_MIN1_MAX0_AS_MAX1, 
    IS_CONTAINED_MIN1_MIN0_MAX0_MAX1, 
    IS_CONTAINED_MIN1_AS_MIN0_MAX0_MAX1, 
    IS_CONTAINED_MIN1_MIN0_MAX0_AS_MAX1, 
    INTERSECTS_MIN1_MIN0_MAX1_MAX0, 
    TOUCHES_MIN1_MAX1_AS_MIN0_MAX0, 
    HAS_NO_OVERLAP_MIN1_MAX1_MIN0_MAX0
  };
  static OverlapType checkOverlapOfMinMaxPairs 
    (double min0, double max0, 
     double min1, double max1);

  static double 
    solutionOfFirstOrderEquation 
      (double a, double b);
  enum SecondOrderEquationSolutionType {
    REAL, DUPLICATED_REAL, IMAGINARY
  };
  static SecondOrderEquationSolutionType 
    solveSecondOrderEquation 
      (double a, double b, double c,
       double& x0_OUT, double& x1_OUT);
  
  static double 
    determinant2D 
      (double a00, double a01,
       double a10, double a11);
  static bool 
    solveLinearEquation2D 
      (double a00, double a01,
       double a10, double a11, 
       double b0, double b1,
       double& x_OUT, double& y_OUT);

  static double 
    determinant3D 
      (double a00, double a01, double a02,
       double a10, double a11, double a12,
       double a20, double a21, double a22);
  static bool 
    solveLinearEquation3D 
      (double a00, double a01, double a02,
       double a10, double a11, double a12,
       double a20, double a21, double a22, 
       double b0, double b1, double b2,
       double& x_OUT, double& y_OUT, double& z_OUT);

  static WH_HugeInt alignedSize 
  (WH_HugeInt dataSize, int alignmentSize); 

};

#ifdef WH_INLINE_ENABLED
#include <WH/scalar_inline.cc>
#endif
