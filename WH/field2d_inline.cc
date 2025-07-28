/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of field2d.cc */



/* class WH_Field2D_A */

WH_INLINE WH_Vector2D WH_Field2D_A
::minRange () const
{ 
  return _minRange; 
}

WH_INLINE WH_Vector2D WH_Field2D_A
::maxRange () const 
{ 
  return _maxRange; 
}

WH_INLINE int WH_Field2D_A
::xGrids () const 
{ 
  return _xGrids; 
}

WH_INLINE int WH_Field2D_A
::yGrids () const 
{ 
  return _yGrids; 
}

WH_INLINE int WH_Field2D_A
::xCells () const 
{ 
  return _xCells;
}

WH_INLINE int WH_Field2D_A
::yCells () const 
{ 
  return _yCells; 
}

WH_INLINE void WH_Field2D_A
::getGridsIn 
(int cx, int cy, 
 int gxs_OUT[4], int gys_OUT[4]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  gxs_OUT[0] = cx;             gys_OUT[0] = cy;
  gxs_OUT[1] = cx + 1;         gys_OUT[1] = cy;
  gxs_OUT[2] = cx + 1;         gys_OUT[2] = cy + 1;
  gxs_OUT[3] = cx;             gys_OUT[3] = cy + 1;
}

WH_INLINE bool WH_Field2D_A
::isOutOfRangeAt (int gx, int gy) const
{
  return gx < 0 || _xGrids <= gx || gy < 0 || _yGrids <= gy;
}

WH_INLINE bool WH_Field2D_A
::isOutOfRangeIn (int cx, int cy) const
{
  return cx < 0 || _xCells <= cx || cy < 0 || _yCells <= cy;
}

WH_INLINE int WH_Field2D_A
::gridIndexAt (int gx, int gy) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx, gy));

  return gx * _yGrids + gy; 
}

WH_INLINE int WH_Field2D_A
::cellIndexIn (int cx, int cy) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  return cx * _yCells + cy; 
}

WH_INLINE void WH_Field2D_A
::getGridIndexsIn 
(int cx, int cy, 
 int gridIndexs_OUT[4]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  int base = cx * _yGrids + cy;
  gridIndexs_OUT[0] = base;
  gridIndexs_OUT[1] = base + _yGrids;
  gridIndexs_OUT[2] = base + _yGrids + 1;
  gridIndexs_OUT[3] = base + 1;
}



/* class WH_UssField2D */

WH_INLINE WH_Vector2D WH_UssField2D
::cellSize (int cx, int cy) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  return _cellSize;
}

WH_INLINE WH_Vector2D WH_UssField2D
::positionAt (int gx, int gy) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx, gy));

  return _minRange + WH_multiply (_cellSize, WH_Vector2D (gx, gy));
}



/* class WH_NussField2D */

WH_INLINE double WH_NussField2D
::xCellSize (int cx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cx);
  WH_ASSERT(cx < this->xCells ());

  return _xCellSizes[cx];
}

WH_INLINE double WH_NussField2D
::yCellSize (int cy) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cy);
  WH_ASSERT(cy < this->yCells ());

  return _yCellSizes[cy];
}

WH_INLINE WH_Vector2D WH_NussField2D
::cellSize (int cx, int cy) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy));

  return WH_Vector2D (_xCellSizes[cx], _yCellSizes[cy]);
}




