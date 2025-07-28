/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for bucket2d.cc */

#ifndef WH_HEADER_WH_BUCKET2D
#define WH_HEADER_WH_BUCKET2D

#ifndef WH_INCLUDED_WH_FIELD2D
#include <WH/field2d.h>
#define WH_INCLUDED_WH_FIELD2D
#endif



class WH_Bucket2D_A;
template <class Type> class WH_Bucket2D;



/* value-based class */
/* heavy weight */
/* for base class of template version */
class WH_Bucket2D_A {
 public:
  WH_Bucket2D_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
     int xCells, int yCells);
  virtual ~WH_Bucket2D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  WH_Vector2D minRange () const;
  WH_Vector2D maxRange () const;
  WH_Vector2D cellSize () const;
  int xCells () const;
  int yCells () const;

  /* derived */

 protected:
  WH_UssField2D _field;
  vector< list<void*> > _list_s;   /* OWN */
  int _indexOutOfRange;
  
  /* base */

  /* no implementation */
  WH_Bucket2D_A (const WH_Bucket2D_A& bucket);
  const WH_Bucket2D_A& operator= (const WH_Bucket2D_A& bucket);

  list<void*>& listAt (int index);
  list<void*>& listIn (int cx, int cy);
  list<void*>& listOutOfRange ();

  int nLists () const;
  int indexOutOfRange () const;
  int indexIn (int cx, int cy) const;
  const list<void*>& listAt (int index) const;
  const list<void*>& listIn (int cx, int cy) const;
  const list<void*>& listOutOfRange () const;

  void allocateIndexsWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     vector<int>& index_s_OUT) const;
  void allocateIndexsOn 
    (const WH_Vector2D& position, 
     vector<int>& index_s_OUT) const;
  
  /* public interface in derived template class */
  void addItemFirstWithin_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     void* item);
  void addItemFirstOn_A 
    (const WH_Vector2D& position, 
     void* item);
  void addItemLastWithin_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     void* item);
  void addItemLastOn_A 
    (const WH_Vector2D& position, 
     void* item);
  void removeItemFromFirstWithin_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     void* item);
  void removeItemFromFirstOn_A 
    (const WH_Vector2D& position, 
     void* item);
  void removeItemFromLastWithin_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     void* item);
  void removeItemFromLastOn_A 
    (const WH_Vector2D& position, 
     void* item);
  void getItemsWithin_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     vector<void*>& allTheItem_s_OUT) const;
  void getItemsOn_A 
    (const WH_Vector2D& position, 
     vector<void*>& allTheItem_s_OUT) const;

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/bucket2d_inline.cc>
#endif



/* value-based class */
/* heavy weight */
template <class Type>
class WH_Bucket2D : public WH_Bucket2D_A {
 public:
  WH_Bucket2D 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
     int xCells, int yCells) 
    : WH_Bucket2D_A (minRange, maxRange, xCells, yCells) {}
  virtual ~WH_Bucket2D () {}

  /* base */
  void addItemFirstWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     Type* item) {
    this->addItemFirstWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void addItemFirstOn 
    (const WH_Vector2D& position, 
     Type* item) {
    this->addItemFirstOn_A 
      (position, (void*)item);
  }

  void addItemLastWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     Type* item) {
    this->addItemLastWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void addItemLastOn 
    (const WH_Vector2D& position, 
     Type* item) {
    this->addItemLastOn_A 
      (position, (void*)item);
  }

  void removeItemFromFirstWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     Type* item) {
    this->removeItemFromFirstWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void removeItemFromFirstOn 
    (const WH_Vector2D& position, 
     Type* item) {
    this->removeItemFromFirstOn_A 
      (position, (void*)item);
  }

  void removeItemFromLastWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     Type* item) {
    this->removeItemFromLastWithin_A 
      (minRange, maxRange, (void*)item);
  }

  void removeItemFromLastOn 
    (const WH_Vector2D& position, 
     Type* item) {
    this->removeItemFromLastOn_A 
      (position, (void*)item);
  }

  void getItemsWithin 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange, 
     vector<Type*>& allTheItem_s_OUT) const {
    this->getItemsWithin_A 
      (minRange, maxRange, 
       (vector<void*>&)allTheItem_s_OUT);
  }
  /* returns all items in <allTheItem_s_OUT> without duplication */

  void getItemsOn 
    (const WH_Vector2D& position, 
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
  WH_Bucket2D (const WH_Bucket2D& bucket);
  const WH_Bucket2D& operator= (const WH_Bucket2D& bucket);

  /* derived */

};



#endif /* WH_HEADER_WH_BUCKET2D */




