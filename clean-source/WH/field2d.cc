/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* field2d.cc : field (grid) on 2D */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "field2d.h"



/* WH_Field2D_A class */

const int WH_Field2D_A::edgeVertexMap[4][2] = {
  { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }
};

WH_Field2D_A
::WH_Field2D_A 
(const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
 int xCells, int yCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (minRange, maxRange));
  WH_ASSERT(0 < xCells);
  WH_ASSERT(0 < yCells);

  WH_CVR_LINE;

  _minRange = minRange;
  _maxRange = maxRange;
  _xCells = xCells;
  _yCells = yCells;
  _xGrids = _xCells + 1;
  _yGrids = _yCells + 1;
}

WH_Field2D_A
::~WH_Field2D_A ()
{
  WH_CVR_LINE;
}

bool WH_Field2D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
  WH_ASSERT(0 < this->xCells ());
  WH_ASSERT(0 < this->yCells ());
  WH_ASSERT(this->xGrids () == this->xCells () + 1);
  WH_ASSERT(this->yGrids () == this->yCells () + 1);

  return true;
}

bool WH_Field2D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(this->isOutOfRangeAt (-1, -1));
  WH_ASSERT(this->isOutOfRangeAt (this->xGrids (), this->yGrids ()));
  WH_ASSERT(this->isOutOfRangeIn (-1, -1));
  WH_ASSERT(this->isOutOfRangeIn (this->xCells (), this->yCells ()));

  WH_ASSERT(this->gridIndexAt (0, 0) == 0);
  WH_ASSERT(this->gridIndexAt (this->xGrids () - 1, 
			       this->yGrids () - 1) 
	    == this->xGrids () * this->yGrids () - 1);
  WH_ASSERT(this->cellIndexIn (0, 0) == 0);
  WH_ASSERT(this->cellIndexIn (this->xCells () - 1, 
			       this->yCells () - 1) 
	    == this->xCells () * this->yCells () - 1);
  
  WH_ASSERT(WH_le (WH_Vector2D (0, 0), this->cellSize (0, 0)));
  WH_ASSERT(WH_le (WH_Vector2D (0, 0), 
		   this->cellSize (this->xCells () - 1, 
				   this->yCells () - 1)));
  WH_ASSERT(WH_eq (this->positionAt (0, 0), 
		   this->minRange ()));
  WH_ASSERT(WH_eq (this->positionAt (this->xGrids () - 1, 
				     this->yGrids () - 1), 
		this->maxRange ()));
  
  int gxs[4], gys[4];
  int gridIndexs[4];
  WH_Vector2D points[4];

  this->getGridsIn (0, 0,
		    gxs, gys);
  this->getGridIndexsIn (0, 0, 
			 gridIndexs);
  WH_ASSERT(gridIndexs[0] == this->gridIndexAt (gxs[0], gys[0]));
  WH_ASSERT(gridIndexs[1] == this->gridIndexAt (gxs[1], gys[1]));
  WH_ASSERT(gridIndexs[2] == this->gridIndexAt (gxs[2], gys[2]));
  WH_ASSERT(gridIndexs[3] == this->gridIndexAt (gxs[3], gys[3]));
  this->getPositionsIn (0, 0,
			points);
  WH_ASSERT(WH_eq (points[0], this->positionAt (gxs[0], gys[0])));
  WH_ASSERT(WH_eq (points[1], this->positionAt (gxs[1], gys[1])));
  WH_ASSERT(WH_eq (points[2], this->positionAt (gxs[2], gys[2])));
  WH_ASSERT(WH_eq (points[3], this->positionAt (gxs[3], gys[3])));
  
  this->getGridsIn (this->xCells () - 1, this->yCells () - 1,
		    gxs, gys);
  this->getGridIndexsIn (this->xCells () - 1, this->yCells () - 1,
			 gridIndexs);
  WH_ASSERT(gridIndexs[0] == this->gridIndexAt (gxs[0], gys[0]));
  WH_ASSERT(gridIndexs[1] == this->gridIndexAt (gxs[1], gys[1]));
  WH_ASSERT(gridIndexs[2] == this->gridIndexAt (gxs[2], gys[2]));
  WH_ASSERT(gridIndexs[3] == this->gridIndexAt (gxs[3], gys[3]));
  this->getPositionsIn (this->xCells () - 1, this->yCells () - 1,
			points);
  WH_ASSERT(WH_eq (points[0], this->positionAt (gxs[0], gys[0])));
  WH_ASSERT(WH_eq (points[1], this->positionAt (gxs[1], gys[1])));
  WH_ASSERT(WH_eq (points[2], this->positionAt (gxs[2], gys[2])));
  WH_ASSERT(WH_eq (points[3], this->positionAt (gxs[3], gys[3])));

  return true;
}

void WH_Field2D_A
::getPositionsIn 
(int cx, int cy, 
 WH_Vector2D positions_OUT[4]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  WH_CVR_LINE;

  positions_OUT[0] = this->positionAt (cx + 0, cy + 0);
  positions_OUT[1] = this->positionAt (cx + 1, cy + 0);
  positions_OUT[2] = this->positionAt (cx + 1, cy + 1);
  positions_OUT[3] = this->positionAt (cx + 0, cy + 1);
}

bool WH_Field2D_A
::getCellOn 
(const WH_Vector2D& position,
 int& cx_OUT, int& cy_OUT, 
 WH_Vector2D& localCoord_OUT) const
{
  bool result = false;

  /* nothing : PURE VIRTUAL */

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position.x, this->minRange ().x)) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else if (WH_lt (this->maxRange ().x, position.x)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT.x, 0, 1));
  }
  if (WH_lt (position.y, this->minRange ().y)) {
    WH_ASSERT(cy_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else if (WH_lt (this->maxRange ().y, position.y)) {
    WH_ASSERT(cy_OUT == this->yCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else {
    WH_ASSERT(0 <= cy_OUT);
    WH_ASSERT(cy_OUT < this->yCells ());
    WH_ASSERT(WH_between (localCoord_OUT.y, 0, 1));
  }
#endif
  
  return result;
}



/* WH_UssField2D class */

WH_UssField2D
::WH_UssField2D 
(const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
 int xCells, int yCells)
: WH_Field2D_A (minRange, maxRange, xCells, yCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(0 < xCells);
  WH_ASSERT(0 < yCells);

  WH_CVR_LINE;

  WH_Vector2D div (_xCells, _yCells);
  _cellSize = WH_divide (_maxRange - _minRange, div);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_UssField2D
::~WH_UssField2D ()
{
  WH_CVR_LINE;
}

bool WH_UssField2D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field2D_A::checkInvariant ();

  return true;
}

bool WH_UssField2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field2D_A::assureInvariant ();

  return true;
}

bool WH_UssField2D
::getCellOn 
(const WH_Vector2D& position,
 int& cx_OUT, int& cy_OUT, 
 WH_Vector2D& localCoord_OUT) const
{
  WH_CVR_LINE;

  WH_Vector2D div = WH_divide (position - _minRange, _cellSize);
  cx_OUT = (int)floor (div.x + WH::eps); 
  cy_OUT = (int)floor (div.y + WH::eps); 
  localCoord_OUT = div - WH_Vector2D (cx_OUT, cy_OUT);

  if (WH_eq (position.x, _maxRange.x)) {
    WH_CVR_LINE;
    cx_OUT = _xCells - 1;
    localCoord_OUT.x = 1.0;
  } else if (_xCells <= cx_OUT) {
    WH_CVR_LINE;
    cx_OUT = _xCells;
    localCoord_OUT.x = 0.0;
  } else if (cx_OUT < 0) {
    WH_CVR_LINE;
    cx_OUT = -1;
    localCoord_OUT.x = 0.0;
  }

  if (WH_eq (position.y, _maxRange.y)) {
    WH_CVR_LINE;
    cy_OUT = _yCells - 1;
    localCoord_OUT.y = 1.0;
  } else if (_yCells <= cy_OUT) {
    WH_CVR_LINE;
    cy_OUT = _yCells;
    localCoord_OUT.y = 0.0;
  } else if (cy_OUT < 0) {
    WH_CVR_LINE;
    cy_OUT = -1;
    localCoord_OUT.y = 0.0;
  }

  bool result = !this->isOutOfRangeIn (cx_OUT, cy_OUT);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position.x, this->minRange ().x)) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else if (WH_lt (this->maxRange ().x, position.x)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT.x, 0, 1));
  }
  if (WH_lt (position.y, this->minRange ().y)) {
    WH_ASSERT(cy_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else if (WH_lt (this->maxRange ().y, position.y)) {
    WH_ASSERT(cy_OUT == this->yCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else {
    WH_ASSERT(0 <= cy_OUT);
    WH_ASSERT(cy_OUT < this->yCells ());
    WH_ASSERT(WH_between (localCoord_OUT.y, 0, 1));
  }
#endif
  
  return result;
}



/* WH_NussField2D class */

WH_NussField2D
::WH_NussField2D 
(const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
 int xCells, int yCells,
 double xCellSizes[], double yCellSizes[])
: WH_Field2D_A (minRange, maxRange, xCells, yCells)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(0 < xCells);
  WH_ASSERT(0 < yCells);
  WH_ASSERT(xCellSizes != WH_NULL);
  WH_ASSERT(yCellSizes != WH_NULL);
#ifndef NDEBUG
  {
    for (int cx = 0; cx < _xCells; cx++) {
      WH_ASSERT(WH_lt (0, xCellSizes[cx]));
    }
    for (int cy = 0; cy < _yCells; cy++) {
      WH_ASSERT(WH_lt (0, yCellSizes[cy]));
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
  
  _yCellSizes = new double[_yCells];
  WH_ASSERT(_yCellSizes != WH_NULL);
  for (int cy = 0; cy < _yCells; cy++) {
    _yCellSizes[cy] = yCellSizes[cy];
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_NussField2D
::WH_NussField2D 
(const WH_NussField2D& field)
: WH_Field2D_A (field._minRange, field._maxRange, 
		field._xCells, field._yCells)
{
  WH_CVR_LINE;

  _xCellSizes = new double[_xCells];
  WH_ASSERT(_xCellSizes != WH_NULL);
  for (int cx = 0; cx < _xCells; cx++) {
    _xCellSizes[cx] = field._xCellSizes[cx];
  }
  
  _yCellSizes = new double[_yCells];
  WH_ASSERT(_yCellSizes != WH_NULL);
  for (int cy = 0; cy < _yCells; cy++) {
    _yCellSizes[cy] = field._yCellSizes[cy];
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_NussField2D
::~WH_NussField2D ()
{
  WH_CVR_LINE;

  delete[] _xCellSizes;
  delete[] _yCellSizes;
}

const WH_NussField2D& WH_NussField2D
::operator= 
(const WH_NussField2D& field)
{
  WH_CVR_LINE;

  if (this == &field) return *this;

  delete[] _xCellSizes;
  delete[] _yCellSizes;

  this->WH_Field2D_A::operator= (field);

  _xCellSizes = new double[_xCells];
  WH_ASSERT(_xCellSizes != WH_NULL);
  for (int cx = 0; cx < _xCells; cx++) {
    _xCellSizes[cx] = field._xCellSizes[cx];
  }
  
  _yCellSizes = new double[_yCells];
  WH_ASSERT(_yCellSizes != WH_NULL);
  for (int cy = 0; cy < _yCells; cy++) {
    _yCellSizes[cy] = field._yCellSizes[cy];
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif

  return *this;
}

bool WH_NussField2D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field2D_A::checkInvariant ();

  return true;
}

bool WH_NussField2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Field2D_A::assureInvariant ();

  WH_Vector2D sum (0, 0);
  for (int cx = 0; cx < this->xCells (); cx++) {
    WH_ASSERT(WH_lt (0, this->xCellSize (cx)));
    sum.x += this->xCellSize (cx);
  }
  for (int cy = 0; cy < this->yCells (); cy++) {
    WH_ASSERT(WH_lt (0, this->yCellSize (cy)));
    sum.y += this->yCellSize (cy);
  }
  WH_ASSERT(WH_eq (sum, this->maxRange () - this->minRange ()));
  
  return true;
}

WH_Vector2D WH_NussField2D
::positionAt (int gx, int gy) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx, gy));

  WH_CVR_LINE;

  double xOffset = 0;
  for (int cx = 0; cx < gx; cx++) {
    xOffset += _xCellSizes[cx];
  }
  double yOffset = 0;
  for (int cy = 0; cy < gy; cy++) {
    yOffset += _yCellSizes[cy];
  }

  WH_Vector2D result = _minRange + WH_Vector2D (xOffset, yOffset);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(WH_between (result, this->minRange (), this->maxRange ()));
#endif

  return result;
}

bool WH_NussField2D
::getCellOn 
(const WH_Vector2D& position,
 int& cx_OUT, int& cy_OUT, 
 WH_Vector2D& localCoord_OUT) const
{
  WH_CVR_LINE;

  WH_Vector2D offset = position - _minRange;

  if (WH_lt (offset.x, 0)) {
    WH_CVR_LINE;
    cx_OUT = -1;
    localCoord_OUT.x = 0.0;
  } else {
    WH_CVR_LINE;
    int cx;
    for (cx = 0; cx < _xCells; cx++) {
      if (WH_lt (offset.x, _xCellSizes[cx])) break;
      offset.x -= _xCellSizes[cx];
    }
    if (cx == _xCells) {
      WH_CVR_LINE;
      if (WH_eq (offset.x, 0)) {
	WH_CVR_LINE;
	cx_OUT = _xCells - 1;
	localCoord_OUT.x = 1.0;
      } else {
	WH_CVR_LINE;
	cx_OUT = _xCells;
	localCoord_OUT.x = 0.0;
      }
    } else {
      WH_CVR_LINE;
      cx_OUT = cx;
      WH_ASSERT(WH_lt (0, _xCellSizes[cx]));
      localCoord_OUT.x = offset.x / _xCellSizes[cx];
    }
  }

  if (WH_lt (offset.y, 0)) {
    WH_CVR_LINE;
    cy_OUT = -1;
    localCoord_OUT.y = 0.0;
  } else {
    WH_CVR_LINE;
    int cy;
    for (cy = 0; cy < _yCells; cy++) {
      WH_CVR_LINE;
      if (WH_lt (offset.y, _yCellSizes[cy])) break;
      offset.y -= _yCellSizes[cy];
    }
    if (cy == _yCells) {
      WH_CVR_LINE;
      if (WH_eq (offset.y, 0)) {
	WH_CVR_LINE;
	cy_OUT = _yCells - 1;
	localCoord_OUT.y = 1.0;
      } else {
	WH_CVR_LINE;
	cy_OUT = _yCells;
	localCoord_OUT.y = 0.0;
      }
    } else {
      WH_CVR_LINE;
      cy_OUT = cy;
      WH_ASSERT(WH_lt (0, _yCellSizes[cy]));
      localCoord_OUT.y = offset.y / _yCellSizes[cy];
    }
  }

  bool result = !this->isOutOfRangeIn (cx_OUT, cy_OUT);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (WH_lt (position.x, this->minRange ().x)) {
    WH_ASSERT(cx_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else if (WH_lt (this->maxRange ().x, position.x)) {
    WH_ASSERT(cx_OUT == this->xCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.x, 0));
  } else {
    WH_ASSERT(0 <= cx_OUT);
    WH_ASSERT(cx_OUT < this->xCells ());
    WH_ASSERT(WH_between (localCoord_OUT.x, 0, 1));
  }
  if (WH_lt (position.y, this->minRange ().y)) {
    WH_ASSERT(cy_OUT == -1);
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else if (WH_lt (this->maxRange ().y, position.y)) {
    WH_ASSERT(cy_OUT == this->yCells ());
    WH_ASSERT(WH_eq (localCoord_OUT.y, 0));
  } else {
    WH_ASSERT(0 <= cy_OUT);
    WH_ASSERT(cy_OUT < this->yCells ());
    WH_ASSERT(WH_between (localCoord_OUT.y, 0, 1));
  }
#endif
  
  return result;
}



#ifndef WH_INLINE_ENABLED
#include "field2d_inline.cc"
#endif



/* test coverage completed */
