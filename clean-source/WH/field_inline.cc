/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of field.cc */



/* class WH_Field_A */

WH_INLINE double WH_Field_A
::minRange () const
{ 
  return _minRange; 
}

WH_INLINE double WH_Field_A
::maxRange () const 
{ 
  return _maxRange; 
}

WH_INLINE int WH_Field_A
::xGrids () const 
{ 
  return _xGrids; 
}

WH_INLINE int WH_Field_A
::xCells () const 
{ 
  return _xCells;
}

WH_INLINE void WH_Field_A
::getGridsIn 
(int cx, 
 int gxs_OUT[2]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  gxs_OUT[0] = cx;    
  gxs_OUT[1] = cx + 1;
}

WH_INLINE bool WH_Field_A
::isOutOfRangeAt (int gx) const
{
  return gx < 0 || _xGrids <= gx;
}

WH_INLINE bool WH_Field_A
::isOutOfRangeIn (int cx) const
{
  return cx < 0 || _xCells <= cx;
}

WH_INLINE int WH_Field_A
::gridIndexAt (int gx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx));

  return gx; 
}

WH_INLINE int WH_Field_A
::cellIndexIn (int cx) const
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  return cx; 
}

WH_INLINE void WH_Field_A
::getGridIndexsIn 
(int cx, 
 int gridIndexs_OUT[2]) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  int base = cx;
  gridIndexs_OUT[0] = base;
  gridIndexs_OUT[1] = base + 1;
}



/* class WH_UssField */

WH_INLINE double WH_UssField
::cellSize (int cx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  return _cellSize;
}

WH_INLINE double WH_UssField
::positionAt (int gx) const
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeAt (gx));

  return _minRange + _cellSize * gx;
}



/* class WH_NussField */

WH_INLINE double WH_NussField
::xCellSize (int cx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(0 <= cx);
  WH_ASSERT(cx < this->xCells ());

  return _xCellSizes[cx];
}

WH_INLINE double WH_NussField
::cellSize (int cx) const 
{ 
  /* PRE-CONDITION */
  WH_ASSERT(!this->isOutOfRangeIn (cx));

  return _xCellSizes[cx];
}




