/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of field3d.cc */



/* class WH_Field3D_A */

WH_INLINE WH_Vector3D WH_Field3D_A
::minRange () const
{ 
  return _minRange; 
}

WH_INLINE WH_Vector3D WH_Field3D_A
::maxRange () const 
{ 
  return _maxRange; 
}

WH_INLINE int WH_Field3D_A
::xGrids () const 
{ 
  return _xGrids; 
}

WH_INLINE int WH_Field3D_A
::yGrids () const 
{ 
  return _yGrids; 
}

WH_INLINE int WH_Field3D_A
::zGrids () const 
{ 
  return _zGrids; 
}

WH_INLINE int WH_Field3D_A
::xCells () const 
{ 
  return _xCells;
}

WH_INLINE int WH_Field3D_A
::yCells () const 
{ 
  return _yCells; 
}

WH_INLINE int WH_Field3D_A
::zCells () const 
{ 
  return _zCells; 
}

WH_INLINE void WH_Field3D_A
::getGridsIn 
(int cx, int cy, int cz, 
 int gxs_OUT[8], int gys_OUT[8], int gzs_OUT[8]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy, cz));

  gxs_OUT[0] = cx;      gys_OUT[0] = cy;      gzs_OUT[0] = cz;     
  gxs_OUT[1] = cx + 1;  gys_OUT[1] = cy;      gzs_OUT[1] = cz;
  gxs_OUT[2] = cx + 1;  gys_OUT[2] = cy + 1;  gzs_OUT[2] = cz;
  gxs_OUT[3] = cx;      gys_OUT[3] = cy + 1;  gzs_OUT[3] = cz;
  gxs_OUT[4] = cx;      gys_OUT[4] = cy;      gzs_OUT[4] = cz + 1;     
  gxs_OUT[5] = cx + 1;  gys_OUT[5] = cy;      gzs_OUT[5] = cz + 1;
  gxs_OUT[6] = cx + 1;  gys_OUT[6] = cy + 1;  gzs_OUT[6] = cz + 1;
  gxs_OUT[7] = cx;      gys_OUT[7] = cy + 1;  gzs_OUT[7] = cz + 1;
}

WH_INLINE bool WH_Field3D_A
::isOutOfRangeAt (int gx, int gy, int gz) const
{
  return gx < 0 || _xGrids <= gx 
    || gy < 0 || _yGrids <= gy
      || gz < 0 || _zGrids <= gz;
}

WH_INLINE bool WH_Field3D_A
::isOutOfRangeIn (int cx, int cy, int cz) const
{
  return cx < 0 || _xCells <= cx 
    || cy < 0 || _yCells <= cy
      || cz < 0 || _zCells <= cz;
}

WH_INLINE int WH_Field3D_A
::gridIndexAt (int gx, int gy, int gz) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx, gy, gz));

  return gx * _yGrids * _zGrids + gy * _zGrids + gz; 
}

WH_INLINE int WH_Field3D_A
::cellIndexIn (int cx, int cy, int cz) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy, cz));

  return cx * _yCells * _zCells + cy * _zCells + cz; 
}

WH_INLINE void WH_Field3D_A
::getGridIndexsIn 
(int cx, int cy, int cz, 
 int gridIndexs_OUT[8]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy, cz));

  int base = cx * _yGrids * _zGrids + cy * _zGrids + cz;
  gridIndexs_OUT[0] = base;
  gridIndexs_OUT[1] = base + _yGrids * _zGrids;
  gridIndexs_OUT[2] = base + _yGrids * _zGrids + _zGrids;
  gridIndexs_OUT[3] = base + _zGrids;
  gridIndexs_OUT[4] = gridIndexs_OUT[0] + 1;
  gridIndexs_OUT[5] = gridIndexs_OUT[1] + 1;
  gridIndexs_OUT[6] = gridIndexs_OUT[2] + 1;
  gridIndexs_OUT[7] = gridIndexs_OUT[3] + 1;
}



/* class WH_UssField3D */

WH_INLINE WH_Vector3D WH_UssField3D
::cellSize (int cx, int cy, int cz) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy, cz));

  return _cellSize;
}

WH_INLINE WH_Vector3D WH_UssField3D
::positionAt (int gx, int gy, int gz) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx, gy, gz));

  return _minRange + WH_multiply (_cellSize, WH_Vector3D (gx, gy, gz));
}



/* class WH_NussField3D */

WH_INLINE double WH_NussField3D
::xCellSize (int cx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cx);
  WH_ASSERT(cx < this->xCells ());

  return _xCellSizes[cx];
}

WH_INLINE double WH_NussField3D
::yCellSize (int cy) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cy);
  WH_ASSERT(cy < this->yCells ());

  return _yCellSizes[cy];
}

WH_INLINE double WH_NussField3D
::zCellSize (int cz) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cz);
  WH_ASSERT(cz < this->zCells ());

  return _zCellSizes[cz];
}

WH_INLINE WH_Vector3D WH_NussField3D
::cellSize (int cx, int cy, int cz) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx, cy, cz));

  return WH_Vector3D (_xCellSizes[cx], 
		      _yCellSizes[cy], 
		      _zCellSizes[cz]);
}




