/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of sorter.cc */



/* class WH_Sorter */

WH_INLINE int WH_Sorter
::nItems () const
{ 
  return _nEntrys; 
}

WH_INLINE void* WH_Sorter
::itemAt (int index) const 
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < _nEntrys);

  return _entrys[index].item;
}

WH_INLINE double WH_Sorter
::valueAt (int index) const 
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < _nEntrys);

  return _entrys[index].value;
}

