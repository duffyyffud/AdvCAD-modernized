/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of scalar.cc */



/* free functions */

WH_INLINE void WH_swap (char& value0_IO, char& value1_IO)
{
  char dummy = value0_IO;
  value0_IO = value1_IO;
  value1_IO = dummy;
}

WH_INLINE void WH_swap (int& value0_IO, int& value1_IO)
{
  int dummy = value0_IO;
  value0_IO = value1_IO;
  value1_IO = dummy;
}

WH_INLINE void WH_swap (double& value0_IO, double& value1_IO)
{
  double dummy = value0_IO;
  value0_IO = value1_IO;
  value1_IO = dummy;
}

WH_INLINE void WH_swap (void*& value0_IO, void*& value1_IO)
{
  void* dummy = value0_IO;
  value0_IO = value1_IO;
  value1_IO = dummy;
}

WH_INLINE bool WH_eq (double value0, double value1) 
{
  return value0 - WH::eps < value1 && value1 < value0 + WH::eps;
}

WH_INLINE bool WH_ne (double value0, double value1)
{
  return value1 < value0 - WH::eps || value0 + WH::eps < value1;
}

WH_INLINE bool WH_ge (double value0, double value1)
{
  return value0 > value1 - WH::eps;
}

WH_INLINE bool WH_le (double value0, double value1)
{
  return value0 < value1 + WH::eps;
}

WH_INLINE bool WH_gt (double value0, double value1)
{
  return value0 > value1 + WH::eps;
}

WH_INLINE bool WH_lt (double value0, double value1)
{
  return value0 < value1 - WH::eps;
}

WH_INLINE bool WH_between (double value, double value0, double value1)
{
  return WH_le (value0, value) && WH_le (value, value1)
    || WH_le (value1, value) && WH_le (value, value0);
}

WH_INLINE bool WH_justBetween (double value, double value0, double value1)
{
  return WH_lt (value0, value) && WH_lt (value, value1)
    || WH_lt (value1, value) && WH_lt (value, value0);
}

WH_INLINE bool WH_eq2 (double value0, double value1) 
{
  return value0 - WH::eps * WH::eps < value1 
    && value1 < value0 + WH::eps * WH::eps;
}

WH_INLINE bool WH_ne2 (double value0, double value1)
{
  return value1 < value0 - WH::eps * WH::eps 
    || value0 + WH::eps * WH::eps < value1;
}

WH_INLINE bool WH_ge2 (double value0, double value1)
{
  return value0 > value1 - WH::eps * WH::eps;
}

WH_INLINE bool WH_le2 (double value0, double value1)
{
  return value0 < value1 + WH::eps * WH::eps;
}

WH_INLINE bool WH_gt2 (double value0, double value1)
{
  return value0 > value1 + WH::eps * WH::eps;
}

WH_INLINE bool WH_lt2 (double value0, double value1)
{
  return value0 < value1 - WH::eps * WH::eps;
}

WH_INLINE bool WH_between2 (double value, double value0, double value1)
{
  return WH_le2 (value0, value) && WH_le2 (value, value1)
    || WH_le2 (value1, value) && WH_le2 (value, value0);
}

WH_INLINE bool WH_justBetween2 (double value, double value0, double value1)
{
  return WH_lt2 (value0, value) && WH_lt2 (value, value1)
    || WH_lt2 (value1, value) && WH_lt2 (value, value0);
}

WH_INLINE bool WH_eq3 (double value0, double value1) 
{
  return value0 - WH::eps * WH::eps * WH::eps < value1 
    && value1 < value0 + WH::eps * WH::eps * WH::eps;
}

WH_INLINE bool WH_ne3 (double value0, double value1)
{
  return value1 < value0 - WH::eps * WH::eps * WH::eps 
    || value0 + WH::eps * WH::eps * WH::eps < value1;
}

WH_INLINE bool WH_ge3 (double value0, double value1)
{
  return value0 > value1 - WH::eps * WH::eps * WH::eps;
}

WH_INLINE bool WH_le3 (double value0, double value1)
{
  return value0 < value1 + WH::eps * WH::eps * WH::eps;
}

WH_INLINE bool WH_gt3 (double value0, double value1)
{
  return value0 > value1 + WH::eps * WH::eps * WH::eps;
}

WH_INLINE bool WH_lt3 (double value0, double value1)
{
  return value0 < value1 - WH::eps * WH::eps * WH::eps;
}

WH_INLINE bool WH_between3 (double value, double value0, double value1)
{
  return WH_le3 (value0, value) && WH_le3 (value, value1)
    || WH_le3 (value1, value) && WH_le3 (value, value0);
}

WH_INLINE bool WH_justBetween3 (double value, double value0, double value1)
{
  return WH_lt3 (value0, value) && WH_lt3 (value, value1)
    || WH_lt3 (value1, value) && WH_lt3 (value, value0);
}

WH_INLINE constexpr double WH_min (double value0, double value1) 
{
  return (value0 < value1) ? value0 : value1;
}

WH_INLINE constexpr double WH_max (double value0, double value1) 
{
  return (value0 < value1) ? value1 : value0;
}

WH_INLINE constexpr double WH_interpolate (double value0, double value1, double ratio) 
{
  return value0 + (value1 - value0) * ratio;
}

WH_INLINE void WH_sortPair (double& value0_IO, double& value1_IO)
{
  if (value1_IO < value0_IO) {
    double dummy = value0_IO;
    value0_IO = value1_IO;
    value1_IO = dummy;
  }
}

WH_INLINE bool WH_minMaxPairsOverlap 
(double min0, double max0, 
 double min1, double max1)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (min0, max0));
  WH_ASSERT(WH_le (min1, max1));
  
  if (WH_lt (max1, min0) || WH_lt (max0, min1)) {
    return false;
  } else {
    return true;
  }
}



/* class WH */

WH_INLINE double WH
::solutionOfFirstOrderEquation 
(double a, double b)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_ne (a, 0));
  
  /* a x + b = 0 */
  
  return -b / a;
}

WH_INLINE double WH
::determinant2D 
(double a00, double a01,
 double a10, double a11)
{
  /* | a00   a01 | */
  /* | a10   a11 | */

  return a00 * a11 - a01 * a10;
}

WH_INLINE double WH
::determinant3D 
(double a00, double a01, double a02,
 double a10, double a11, double a12,
 double a20, double a21, double a22)
{
  /* | a00   a01   a02 | */
  /* | a10   a11   a12 | */
  /* | a20   a21   a22 | */

  return a00 * (a11 * a22 - a21 * a12) 
    - a01 * (a10 * a22 - a20 * a12)
      + a02 * (a10 * a21 - a20 * a11);
}

WH_INLINE WH_HugeInt WH
::alignedSize (WH_HugeInt dataSize, int alignmentSize)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < dataSize);
  WH_ASSERT(0 < alignmentSize);
  
  WH_HugeInt result = dataSize;
  if (0 < dataSize % alignmentSize) {
    result = alignmentSize * (dataSize / alignmentSize + 1);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(dataSize <= result);
  WH_ASSERT(result < dataSize + alignmentSize);
  WH_ASSERT(result % alignmentSize == 0);
#endif

  return result;
}




