/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of bucket.cc */



/* class WH_Bucket_A */

WH_INLINE double WH_Bucket_A
::minRange () const
{
  return _field.minRange ();
}

WH_INLINE double WH_Bucket_A
::maxRange () const
{
  return _field.maxRange ();
}

WH_INLINE double WH_Bucket_A
::cellSize () const
{
  return _field.cellSize (0);
}

WH_INLINE int WH_Bucket_A
::xCells () const
{
  return _field.xCells ();
}

WH_INLINE int WH_Bucket_A
::nLists () const
{
  return (int)_list_s.size ();
}

WH_INLINE int WH_Bucket_A
::indexOutOfRange () const
{
  return _indexOutOfRange;
}

WH_INLINE int WH_Bucket_A
::indexIn (int cx) const
{
  int result;

  if (_field.isOutOfRangeIn (cx)) {
    result = _indexOutOfRange;
  } else {
    result = _field.cellIndexIn (cx);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 <= result);
  WH_ASSERT(result <= _indexOutOfRange);
#endif

  return result;
}

WH_INLINE const list<void*>& WH_Bucket_A
::listAt (int index) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index <= _indexOutOfRange);

  return _list_s[index]; 
}

WH_INLINE list<void*>& WH_Bucket_A
::listAt (int index)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index <= _indexOutOfRange);

  return _list_s[index]; 
}

WH_INLINE const list<void*>& WH_Bucket_A
::listIn (int cx) const
{
  return _list_s[this->indexIn (cx)]; 
}

WH_INLINE list<void*>& WH_Bucket_A
::listIn (int cx)
{
  return _list_s[this->indexIn (cx)]; 
}

WH_INLINE const list<void*>& WH_Bucket_A
::listOutOfRange () const
{ 
  return _list_s[_indexOutOfRange]; 
}

WH_INLINE list<void*>& WH_Bucket_A
::listOutOfRange ()
{ 
  return _list_s[_indexOutOfRange]; 
}

WH_INLINE void WH_Bucket_A
::allocateIndexsOn 
(double position, 
 vector<int>& index_s_OUT) const
{
  this->allocateIndexsWithin 
    (position, position, 
     index_s_OUT);
}
  
WH_INLINE void WH_Bucket_A
::getItemsOn_A 
(double position, 
 vector<void*>& allTheItem_s_OUT) const
{
  this->getItemsWithin_A 
    (position, position, 
     allTheItem_s_OUT);
}
  
WH_INLINE void WH_Bucket_A
::addItemFirstOn_A 
(double position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->addItemFirstWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket_A
::addItemLastOn_A 
(double position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->addItemLastWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket_A
::removeItemFromFirstOn_A 
(double position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->removeItemFromFirstWithin_A (position, position, item);
}

WH_INLINE void WH_Bucket_A
::removeItemFromLastOn_A 
(double position, 
 void* item) 
{
  /* PRE-CONDITION */
  WH_ASSERT(item != WH_NULL);

  this->removeItemFromLastWithin_A (position, position, item);
}

