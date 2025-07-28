/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* common standard header files inclusion */

#pragma once



/* common header files */
#include <iostream>
#include <iomanip>
#include <fstream>

#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <utility>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;



/* common types */

typedef long WH_HugeInt;



/* common macros */

#define WH_NULL nullptr
#define WH_NO_INDEX ((int)-1)

#ifdef WH_INLINE_ENABLED
#define WH_INLINE inline 
#else
#define WH_INLINE
#endif



/* assertion macros */

void WH_Assert 
(const char* expr, 
 const char* file, int line);
void WH_AssertMsg 
(const char* expr, 
 const char* file, int line, 
 const char* msg);

#ifdef NDEBUG

#define WH_ASSERT(expr)  

#define WH_ASSERT_MSG(expr, msg)  

#define WH_ASSERT_NO_REACH  

#else

#define WH_ASSERT(expr) \
if (expr) ; \
else WH_Assert (#expr, __FILE__, __LINE__)

#define WH_ASSERT_MSG(expr, msg) \
if (expr) ; \
else WH_AssertMsg (#expr, __FILE__, __LINE__, msg)

#define WH_ASSERT_NO_REACH WH_AssertMsg ("false", __FILE__, __LINE__, "NO_REACH")

#endif



/* common templates */

template <class Collection>
void WH_T_Delete (const Collection& item_s)
{
  for (typename Collection::const_iterator 
	 i_item = item_s.begin ();
       i_item != item_s.end ();
       i_item++) {
    delete (*i_item);
  }
}

template <class Collection>
void WH_T_AssureInvariant (const Collection& item_s)
{
  for (typename Collection::const_iterator 
	 i_item = item_s.begin ();
       i_item != item_s.end ();
       i_item++) {
    (*i_item)->assureInvariant ();
  }
}

template <class Collection, class Item>
bool WH_T_Exists (const Collection& item_s, const Item& item)
{
  typename Collection::const_iterator i_item 
    = find (item_s.begin (), item_s.end (), 
	    item);
  return (i_item != item_s.end ());
}

template <class Collection, class Item>
void WH_T_AddIfNotExists (Collection& item_s, const Item& item)
{
  typename Collection::const_iterator i_item 
    = find (item_s.begin (), item_s.end (), 
	    item);
  if (i_item == item_s.end ()) {
    item_s.push_back (item);
  }
}

template <class Collection0, class Collection1>
void WH_T_Add 
(const Collection0& srcItem_s, Collection1& destItem_s)
{
  for (typename Collection0::const_iterator 
	 i_item = srcItem_s.begin ();
       i_item != srcItem_s.end ();
       i_item++) {
    destItem_s.push_back (*i_item);
  }
}

template <class Collection0, class Collection1>
void WH_T_Move 
(const Collection0& srcItem_s, Collection1& destItem_s)
{
  destItem_s.clear ();
  for (typename Collection0::const_iterator 
	 i_item = srcItem_s.begin ();
       i_item != srcItem_s.end ();
       i_item++) {
    destItem_s.push_back (*i_item);
  }
}

template <class Collection>
bool WH_T_HasDuplication (const Collection& item_s)
{
  for (typename Collection::const_iterator 
	 i_item = item_s.begin ();
       i_item != item_s.end ();
       i_item++) {
    typename Collection::const_iterator j_item = i_item;
    j_item++;
    for (;
	 j_item != item_s.end ();
	 j_item++) {
      if ((*i_item) == (*j_item)) {
	return true;
      }
    }
  }

  return false;
}



/* test coverage macros */

#ifndef WH_INCLUDED_WH_COVERAGE
#include <WH/coverage.h>
#define WH_INCLUDED_WH_COVERAGE
#endif






