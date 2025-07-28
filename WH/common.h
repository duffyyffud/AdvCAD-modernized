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
#include <memory>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

// Debug output control - define WH_DEBUG_ENABLED to enable debug messages
#ifdef WH_DEBUG_ENABLED
#define WH_DEBUG(msg) cerr << "DEBUG: " << msg << endl
#else
#define WH_DEBUG(msg) do {} while(0)
#endif



/* Modern Exception Classes */

class WH_Exception : public std::exception {
public:
    explicit WH_Exception(const std::string& message) : message_(message) {}
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }
private:
    std::string message_;
};

class WH_NullPointerException : public WH_Exception {
public:
    explicit WH_NullPointerException(const std::string& message) 
        : WH_Exception("Null Pointer: " + message) {}
};

class WH_InvalidArgumentException : public WH_Exception {
public:
    explicit WH_InvalidArgumentException(const std::string& message) 
        : WH_Exception("Invalid Argument: " + message) {}
};

class WH_IndexOutOfRangeException : public WH_Exception {
public:
    explicit WH_IndexOutOfRangeException(const std::string& message) 
        : WH_Exception("Index Out of Range: " + message) {}
};

class WH_GeometryException : public WH_Exception {
public:
    explicit WH_GeometryException(const std::string& message) 
        : WH_Exception("Geometry Error: " + message) {}
};

/* Modern Error Macros */
#define WH_THROW_IF_NULL(ptr, msg) \
    if (ptr == WH_NULL) throw WH_NullPointerException(msg)

#define WH_THROW_IF_OUT_OF_RANGE(index, size, msg) \
    if (index < 0 || index >= size) throw WH_IndexOutOfRangeException(msg)



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






