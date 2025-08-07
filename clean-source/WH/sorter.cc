/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* sorter.cc : sorting algorithm class */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "sorter.h"
#include "scalar.h"



/* class WH_Sorter */

WH_Sorter
  ::WH_Sorter ()
{
  WH_CVR_LINE;
  _entrys = WH_NULL;
  _nEntrys = 0;
}

WH_Sorter
  ::WH_Sorter (WH_Sorter&& other) noexcept
{
  WH_CVR_LINE;
  _entrys = other._entrys;
  _nEntrys = other._nEntrys;
  
  // Reset moved-from object
  other._entrys = WH_NULL;
  other._nEntrys = 0;
}

WH_Sorter
  ::~WH_Sorter ()
{
  WH_CVR_LINE;
  delete[] _entrys;
}

WH_Sorter& WH_Sorter
  ::operator= (WH_Sorter&& other) noexcept
{
  WH_CVR_LINE;
  if (this != &other) {
    delete[] _entrys;
    
    _entrys = other._entrys;
    _nEntrys = other._nEntrys;
    
    // Reset moved-from object
    other._entrys = WH_NULL;
    other._nEntrys = 0;
  }
  return *this;
}

void WH_Sorter
  ::initialize (int nItems)
{
  WH_CVR_LINE;
  delete[] _entrys;
  _nEntrys = nItems;
  _entrys = new SortEntry[_nEntrys];
}

void WH_Sorter
  ::setItemAt (int index, void* item, double value)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= index);
  WH_ASSERT(index < _nEntrys);

  WH_CVR_LINE;

  _entrys[index].value = value;
  _entrys[index].item = item;
}

int WH_Sorter
::compareInAscendantOrder 
(const void* entry0, const void* entry1)
{
  WH_CVR_LINE;
  SortEntry* e0 = (SortEntry*)entry0;
  SortEntry* e1 = (SortEntry*)entry1;
  if (WH_lt (e1->value, e0->value)) return 1;
  if (WH_lt (e0->value, e1->value)) return -1;
  return 0;
}

int WH_Sorter
::compareInDescendantOrder 
(const void* entry0, const void* entry1)
{
  WH_CVR_LINE;
  SortEntry* e0 = (SortEntry*)entry0;
  SortEntry* e1 = (SortEntry*)entry1;
  if (WH_lt (e0->value, e1->value)) return 1;
  if (WH_lt (e1->value, e0->value)) return -1;
  return 0;
}

void WH_Sorter
::sortInAscendantOrder ()
{
  WH_CVR_LINE;
  qsort (_entrys, 
	 (unsigned int)_nEntrys, 
	 (unsigned int)sizeof (SortEntry), 
	 compareInAscendantOrder);
}

void WH_Sorter
::sortInDescendantOrder ()
{
  WH_CVR_LINE;
  qsort (_entrys, 
	 (unsigned int)_nEntrys, 
	 (unsigned int)sizeof (SortEntry), 
	 compareInDescendantOrder);
}



#ifndef WH_INLINE_ENABLED
#include "sorter_inline.cc"
#endif



/* test coverage completed */
