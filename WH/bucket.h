/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for bucket.cc */

#pragma once
#ifndef WH_INCLUDED_WH_FIELD
#include <WH/field.h>
#define WH_INCLUDED_WH_FIELD
#endif

class WH_Bucket_A;
template <class Type> class WH_Bucket;

/* value-based class */
/* heavy weight */
/* for base class of template version */
class WH_Bucket_A {
 public:
  WH_Bucket_A 
    (double minRange, double maxRange,
     int xCells);
  virtual ~WH_Bucket_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  double minRange () const;
  double maxRange () const;
  double cellSize () const;
  int xCells () const;

  /* derived */

 protected:
  WH_UssField _field;
  vector< list<void*> > _list_s;   /* OWN */
  int _indexOutOfRange;
  
  /* base */
  
  /* no implementation */
  WH_Bucket_A (const WH_Bucket_A& bucket);
  const WH_Bucket_A& operator= (const WH_Bucket_A& bucket);
  
  list<void*>& listAt (int index);
  list<void*>& listIn (int cx);
  list<void*>& listOutOfRange ();

  int nLists () const;
  int indexOutOfRange () const;
  int indexIn (int cx) const;
  const list<void*>& listAt (int index) const;
  const list<void*>& listIn (int cx) const;
  const list<void*>& listOutOfRange () const;

  void allocateIndexsWithin 
    (double minRange, double maxRange, 
     vector<int>& index_s_OUT) const;
  void allocateIndexsOn 
    (double position, 
     vector<int>& index_s_OUT) const;
  
  /* public interface in derived template class */
  void addItemFirstWithin_A 
    (double minRange, double maxRange, 
     void* item);
  void addItemFirstOn_A 
    (double position, 
     void* item);
  void addItemLastWithin_A 
    (double minRange, double maxRange, 
     void* item);
  void addItemLastOn_A 
    (double position, 
     void* item);
  void removeItemFromFirstWithin_A 
    (double minRange, double maxRange, 
     void* item);
  void removeItemFromFirstOn_A 
    (double position, 
     void* item);
  void removeItemFromLastWithin_A 
    (double minRange, double maxRange, 
     void* item);
  void removeItemFromLastOn_A 
    (double position, 
     void* item);
  void getItemsWithin_A 
    (double minRange, double maxRange, 
     vector<void*>& allTheItem_s_OUT) const;
  void getItemsOn_A 
    (double position, 
     vector<void*>& allTheItem_s_OUT) const;
  
  /* derived */
  
};

#ifdef WH_INLINE_ENABLED
#include <WH/bucket_inline.cc>
#endif

/* value-based class */
/* heavy weight */
template <class Type>
class WH_Bucket : public WH_Bucket_A {
 public:
  WH_Bucket 
    (double minRange, double maxRange,
     int xCells) 
    : WH_Bucket_A (minRange, maxRange, xCells) {}
  virtual ~WH_Bucket () {}
  
  /* base */
  void addItemFirstWithin 
    (double minRange, double maxRange, 
     Type* item) {
    this->addItemFirstWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void addItemFirstOn 
    (double position, 
     Type* item) {
    this->addItemFirstOn_A 
      (position, (void*)item);
  }

  void addItemLastWithin 
    (double minRange, double maxRange, 
     Type* item) {
    this->addItemLastWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void addItemLastOn 
    (double position, 
     Type* item) {
    this->addItemLastOn_A 
      (position, (void*)item);
  }

  void removeItemFromFirstWithin 
    (double minRange, double maxRange, 
     Type* item) {
    this->removeItemFromFirstWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void removeItemFromFirstOn 
    (double position, 
     Type* item) {
    this->removeItemFromFirstOn_A 
      (position, (void*)item);
  }

  void removeItemFromLastWithin 
    (double minRange, double maxRange, 
     Type* item) {
    this->removeItemFromLastWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void removeItemFromLastOn 
    (double position, 
     Type* item) {
    this->removeItemFromLastOn_A 
      (position, (void*)item);
  }

  void getItemsWithin 
    (double minRange, double maxRange, 
     vector<Type*>& allTheItem_s_OUT) const {
    this->getItemsWithin_A 
      (minRange, maxRange, 
       (vector<void*>&)allTheItem_s_OUT);
  }
  /* returns all items in <allTheItem_s_OUT> without duplication */

  void getItemsOn 
    (double position, 
     vector<Type*>& allTheItem_s_OUT) const {
    this->getItemsOn_A 
      (position, 
       (vector<void*>&)allTheItem_s_OUT);
  }
  /* returns all items in <allTheItem_s_OUT> without duplication */
  
  /* derived */

 protected:
  
  /* base */
  
  /* no implementation */
  WH_Bucket (const WH_Bucket& bucket);
  const WH_Bucket& operator= (const WH_Bucket& bucket);
  
  /* derived */
  
};
