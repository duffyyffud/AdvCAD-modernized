/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of bucket2d.cc */



/* class WH_Bucket2D_A */

WH_INLINE WH_Vector2D WH_Bucket2D_A
::minRange () const
{
  return _field.minRange ();
}

WH_INLINE WH_Vector2D WH_Bucket2D_A
::maxRange () const
{
  return _field.maxRange ();
}

WH_INLINE WH_Vector2D WH_Bucket2D_A
::cellSize () const
{
  return _field.cellSize (0, 0);
}

WH_INLINE int WH_Bucket2D_A
::xCells () const
{
  return _field.xCells ();
}

WH_INLINE int WH_Bucket2D_A
::yCells () const
{
  return _field.yCells ();
}

WH_INLINE int WH_Bucket2D_A
::nLists () const
{
  return (int)_list_s.size ();
}

WH_INLINE int WH_Bucket2D_A
::indexOutOfRange () const
{
  return _indexOutOfRange;
}

WH_INLINE int WH_Bucket2D_A
::indexIn (int cx, int cy) const
{
  int result;

  if (_field.isOutOfRangeIn (cx, cy)) {
    result = _indexOutOfRange;
  } else {
    result = _field.cellIndexIn (cx, cy);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result <= _indexOutOfRange);
#endif

  return result;
}

WH_INLINE const list<void*>& WH_Bucket2D_A
::listAt (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index <= _indexOutOfRange);

  return _list_s[index]; 
}

WH_INLINE list<void*>& WH_Bucket2D_A
::listAt (int index)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index <= _indexOutOfRange);

  return _list_s[index]; 
}

WH_INLINE const list<void*>& WH_Bucket2D_A
::listIn (int cx, int cy) const
{
  return _list_s[this->indexIn (cx, cy)]; 
}

WH_INLINE list<void*>& WH_Bucket2D_A
::listIn (int cx, int cy)
{
  return _list_s[this->indexIn (cx, cy)]; 
}

WH_INLINE const list<void*>& WH_Bucket2D_A
::listOutOfRange () const
{ 
  return _list_s[_indexOutOfRange]; 
}

WH_INLINE list<void*>& WH_Bucket2D_A
::listOutOfRange ()
{ 
  return _list_s[_indexOutOfRange]; 
}

WH_INLINE void WH_Bucket2D_A
::allocateIndexsOn 
(const WH_Vector2D& position, 
 vector<int>& index_s_OUT) const
{
  this->allocateIndexsWithin 
    (position, position, 
     index_s_OUT);
}
  
WH_INLINE void WH_Bucket2D_A
::getItemsOn_A 
(const WH_Vector2D& position, 
 vector<void*>& allTheItem_s_OUT) const
{
  this->getItemsWithin_A 
    (position, position, 
     allTheItem_s_OUT);
}
  
WH_INLINE void WH_Bucket2D_A
::addItemFirstOn_A
(const WH_Vector2D& position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->addItemFirstWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket2D_A
::addItemLastOn_A 
(const WH_Vector2D& position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->addItemLastWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket2D_A
::removeItemFromFirstOn_A 
(const WH_Vector2D& position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->removeItemFromFirstWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket2D_A
::removeItemFromLastOn_A 
(const WH_Vector2D& position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->removeItemFromLastWithin_A (position, position, item);
}

