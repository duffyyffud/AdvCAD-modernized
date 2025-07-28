/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for sorter.cc */

#pragma once
#ifndef WH_INCLUDED_WH_COMMON
#include <WH/common.h>
#define WH_INCLUDED_WH_COMMON
#endif

class WH_Sorter;

/* value-based class */
/* heavy weight */
/* no inheritance */
class WH_Sorter {
 public:
  WH_Sorter ();
  ~WH_Sorter ();

  void initialize (int nItems);
  void setItemAt (int index, void* item, double value);
  void sortInAscendantOrder ();
  void sortInDescendantOrder ();

  int nItems () const; 
  void* itemAt (int index) const;
  double valueAt (int index) const;

 private:
  struct SortEntry { 
    double value; 
    void* item; 
    void* dummy; 
  };

  SortEntry* _entrys;
  int _nEntrys;

  static int compareInAscendantOrder 
    (const void* entry0, const void* entry1);
  static int compareInDescendantOrder 
    (const void* entry0, const void* entry1);
  
  /* no implementation */
  WH_Sorter (const WH_Sorter& sorter);
  const WH_Sorter& operator= (const WH_Sorter& sorter);
};
#ifdef WH_INLINE_ENABLED
#include <WH/sorter_inline.cc>
#endif
