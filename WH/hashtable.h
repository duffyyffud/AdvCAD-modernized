/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for hashtable.cc */

#ifndef WH_HEADER_WH_HASHTABLE
#define WH_HEADER_WH_HASHTABLE

#ifndef WH_INCLUDED_WH_COMMON
#include <WH/common.h>
#define WH_INCLUDED_WH_COMMON
#endif

#ifndef WH_INCLUDED_WH_SCALAR
#include <WH/scalar.h>
#define WH_INCLUDED_WH_SCALAR
#endif



template <class Type> class WH_HashBucket;



/* value-based class */
/* heavy weight */
template <class Type>
class WH_HashBucket {
 public:
  WH_HashBucket (int nLists) {
    /* PRE-CONDITION */
    WH_ASSERT(0 < nLists);

    _nLists = nLists;
    _lists = new list<Type*>[nLists];
    WH_ASSERT(_lists != WH_NULL);
  }
  virtual ~WH_HashBucket () {
    delete[] _lists;
  }
  virtual bool checkInvariant () const {  
    return true;  
  }
  virtual bool assureInvariant () const {  
    return true;  
  }

  /* base */
  int nLists () const {  return _nLists;  }

  list<Type*>& listAt (int hashValue) {
    if (hashValue < 0) hashValue = -hashValue;
    return _lists[hashValue % _nLists]; 
  }

  const list<Type*>& listAt (int hashValue) const {
    if (hashValue < 0) hashValue = -hashValue;
    return _lists[hashValue % _nLists]; 
  }

  /* derived */

 protected:
  int _nLists;

  list<Type*>* _lists;   /* OWN : _lists[_nLists] */
  
  /* base */
  
  /* derived */
  
};



#endif /* WH_HEADER_WH_HASHTABLE */


