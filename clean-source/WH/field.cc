/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* field.cc : field (grid) on 1D */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "field.h"



/* WH_Field_A class */

WH_Field_A
::WH_Field_A 
(double minRange, double maxRange,
 int xCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (minRange, maxRange));
  WH_ASSERT(0 < xCells);

  WH_CVR_LINE;

  _minRange = minRange;
  _maxRange = maxRange;
  _xCells = xCells;
  _xGrids = _xCells + 1;
}

WH_Field_A
::~WH_Field_A ()
{
  WH_CVR_LINE;
}

bool WH_Field_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
  WH_ASSERT(0 < this->xCells ());
  WH_ASSERT(this->xGrids () == this->xCells () + 1);

  return true;
}

bool WH_Field_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(this->isOutOfRangeAt (-1));
  WH_ASSERT(this->isOutOfRangeAt (this->xGrids ()));
  WH_ASSERT(this->isOutOfRangeIn (-1));
  WH_ASSERT(this->isOutOfRangeIn (this->xCells ()));

  WH_ASSERT(this->gridIndexAt (0) == 0);
  WH_ASSERT(this->gridIndexAt (this->xGrids () - 1) 
	  == this->xGrids () - 1);
  WH_ASSERT(this->cellIndexIn (0) == 0);
  WH_ASSERT(this->cellIndexIn (this->xCells () - 1) 
	  == this->xCells () - 1);

  WH_ASSERT(WH_le (0, this->cellSize (0)));
  WH_ASSERT(WH_le (0, 
		  this->cellSize (this->xCells () - 1)));
  WH_ASSERT(WH_eq (this->positionAt (0), 
		  this->minRange ()));
  WH_ASSERT(WH_eq (this->positionAt (this->xGrids () - 1), 
		  this->maxRange ()));

  int gxs[2];
  int gridIndexs[2];
  double points[2];

  this->getGridsIn (0,
		    gxs);
  this->getGridIndexsIn (0, 
			 gridIndexs);
  WH_ASSERT(gridIndexs[0] == this->gridIndexAt (gxs[0]));
  WH_ASSERT(gridIndexs[1] == this->gridIndexAt (gxs[1]));
  this->getPositionsIn (0,
			points);
  WH_ASSERT(WH_eq (points[0], this->positionAt (gxs[0])));
  WH_ASSERT(WH_eq (points[1], this->positionAt (gxs[1])));
  
  this->getGridsIn (this->xCells () - 1,
		    gxs);
  this->getGridIndexsIn (this->xCells () - 1,
			 gridIndexs);
  WH_ASSERT(gridIndexs[0] == this->gridIndexAt (gxs[0]));
  WH_ASSERT(gridIndexs[1] == this->gridIndexAt (gxs[1]));
  this->getPositionsIn (this->xCells () - 1,
			points);
  WH_ASSERT(WH_eq (points[0], this->positionAt (gxs[0])));
  WH_ASSERT(WH_eq (points[1], this->positionAt (gxs[1])));

  return true;
}

void WH_Field_A
::getPositionsIn 
(int cx, 
 double positions_OUT[2]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  WH_CVR_LINE;

  positions_OUT[0] = this->positionAt (cx + 0);
  positions_OUT[1] = this->positionAt (cx + 1);
}

bool WH_Field_A
::getCellOn 
(double position,
 int& cx_OUT, 
 double& localCoord_OUT) const
{
  bool result = false;

  /* nothing : PURE VIRTUAL */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position, this->minRange ())) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else if (WH_lt (this->maxRange (), position)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT, 0, 1));
  }
#endif
  
  return result;
}



/* WH_UssField class */

WH_UssField
::WH_UssField 
(double minRange, double maxRange,
 int xCells)
: WH_Field_A (minRange, maxRange, xCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(0 < xCells);

  WH_CVR_LINE;

  double div = _xCells;
  _cellSize = (_maxRange - _minRange) / div;
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_UssField
::~WH_UssField ()
{
  WH_CVR_LINE;
}

bool WH_UssField
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field_A::checkInvariant ();

  return true;
}

bool WH_UssField
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field_A::assureInvariant ();

  return true;
}

bool WH_UssField
::getCellOn 
(double position,
 int& cx_OUT, 
 double& localCoord_OUT) const
{
  WH_CVR_LINE;

  double div = (position - _minRange) / _cellSize;
  cx_OUT = (int)floor (div + WH::eps); 
  localCoord_OUT = div - cx_OUT;

  if (WH_eq (position, _maxRange)) {
    WH_CVR_LINE;
    cx_OUT = _xCells - 1;
    localCoord_OUT = 1.0;
  } else if (_xCells <= cx_OUT) {
    WH_CVR_LINE;
    cx_OUT = _xCells;
    localCoord_OUT = 0.0;
  } else if (cx_OUT < 0) {
    WH_CVR_LINE;
    cx_OUT = -1;
    localCoord_OUT = 0.0;
  }

  bool result = !this->isOutOfRangeIn (cx_OUT);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position, this->minRange ())) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else if (WH_lt (this->maxRange (), position)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT, 0, 1));
  }
#endif
  
  return result;
}



/* WH_NussField class */

WH_NussField
::WH_NussField 
(double minRange, double maxRange,
 int xCells,
 double xCellSizes[])
: WH_Field_A (minRange, maxRange, xCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(0 < xCells);
  WH_ASSERT(xCellSizes != WH_NULL);
#ifndef NDEBUG
  {
    for (int cx = 0; cx < _xCells; cx++) {
      WH_ASSERT(WH_lt (0, xCellSizes[cx]));
    }
  }
#endif
  
  WH_CVR_LINE;

  _xCellSizes = new double[_xCells];
  WH_ASSERT(_xCellSizes != WH_NULL);
  for (int cx = 0; cx < _xCells; cx++) {
    _xCellSizes[cx] = xCellSizes[cx];
    WH_ASSERT(WH_lt (0, xCellSizes[cx]));
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_NussField
::WH_NussField 
(const WH_NussField& field)
: WH_Field_A (field._minRange, field._maxRange, 
	      field._xCells)
{
  WH_CVR_LINE;

  _xCellSizes = new double[_xCells];
  WH_ASSERT(_xCellSizes != WH_NULL);
  for (int cx = 0; cx < _xCells; cx++) {
    _xCellSizes[cx] = field._xCellSizes[cx];
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_NussField
::~WH_NussField ()
{
  WH_CVR_LINE;

  delete[] _xCellSizes;
}

const WH_NussField& WH_NussField
::operator= 
(const WH_NussField& field)
{
  WH_CVR_LINE;

  if (this == &field) return *this;

  delete[] _xCellSizes;

  this->WH_Field_A::operator= (field);

  _xCellSizes = new double[_xCells];
  WH_ASSERT(_xCellSizes != WH_NULL);
  for (int cx = 0; cx < _xCells; cx++) {
    _xCellSizes[cx] = field._xCellSizes[cx];
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif

  return *this;
}

bool WH_NussField
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field_A::checkInvariant ();

  return true;
}

bool WH_NussField
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field_A::assureInvariant ();

  double sum = 0.0;
  for (int cx = 0; cx < this->xCells (); cx++) {
    WH_ASSERT(WH_lt (0, this->xCellSize (cx)));
    sum += this->xCellSize (cx);
  }
  WH_ASSERT(WH_eq (sum, this->maxRange () - this->minRange ()));
  
  return true;
}

double WH_NussField
::positionAt (int gx) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx));

  WH_CVR_LINE;

  double xOffset = 0.0;
  for (int cx = 0; cx < gx; cx++) {
    xOffset += _xCellSizes[cx];
  }

  double result = _minRange + xOffset;
  
  /* POST-CONDITION */
  WH_ASSERT(WH_between (result, this->minRange (), this->maxRange ()));

  return result;
}

bool WH_NussField
::getCellOn 
(double position,
 int& cx_OUT, 
 double& localCoord_OUT) const
{
  WH_CVR_LINE;

  double offset = position - _minRange;

  if (WH_lt (offset, 0)) {
    WH_CVR_LINE;
    cx_OUT = -1;
    localCoord_OUT = 0.0;
  } else {
    WH_CVR_LINE;
    int cx;
    for (cx = 0; cx < _xCells; cx++) {
      if (WH_lt (offset, _xCellSizes[cx])) break;
      offset -= _xCellSizes[cx];
    }
    if (cx == _xCells) {
      WH_CVR_LINE;
      if (WH_eq (offset, 0)) {
	WH_CVR_LINE;
	cx_OUT = _xCells - 1;
	localCoord_OUT = 1.0;
      } else {
	WH_CVR_LINE;
	cx_OUT = _xCells;
	localCoord_OUT = 0.0;
      }
    } else {
      WH_CVR_LINE;
      cx_OUT = cx;
      WH_ASSERT(WH_lt (0, _xCellSizes[cx]));
      localCoord_OUT = offset / _xCellSizes[cx];
    }
  }

  bool result = !this->isOutOfRangeIn (cx_OUT);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position, this->minRange ())) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else if (WH_lt (this->maxRange (), position)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT, 0, 1));
  }
#endif
  
  return result;
}



#ifndef WH_INLINE_ENABLED
#include "field_inline.cc"
#endif



/* test coverage completed */
