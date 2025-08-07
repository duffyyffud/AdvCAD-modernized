/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* bucket3d.cc : bucket class definition in 3D space */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "bucket3d.h"



/* class WH_Bucket3D_A */

WH_Bucket3D_A
::WH_Bucket3D_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange,
 int xCells, int yCells, int zCells)
: _field (minRange, maxRange, xCells, yCells, zCells)
{
  WH_CVR_LINE;

  _list_s.resize (xCells * yCells * zCells + 1);
  _indexOutOfRange = xCells * yCells * zCells;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->assureInvariant ());
#endif
}

WH_Bucket3D_A
::~WH_Bucket3D_A ()
{
  WH_CVR_LINE;
}

bool WH_Bucket3D_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
  WH_ASSERT(0 < this->xCells ());
  WH_ASSERT(0 < this->yCells ());
  WH_ASSERT(0 < this->zCells ());
  WH_ASSERT(this->nLists () 
	    == this->xCells () * this->yCells () * this->zCells () + 1);
  WH_ASSERT(this->indexOutOfRange () 
	    == this->xCells () * this->yCells () * this->zCells ());
  
  return true;
}

bool WH_Bucket3D_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();
  
  _field.assureInvariant ();

  WH_ASSERT(WH_lt (WH_Vector3D (0, 0, 0), this->cellSize ()));

  vector<int> index_s;
  
  this->allocateIndexsOn 
    (this->minRange (),
     index_s);
  WH_ASSERT(index_s.size () == 2);
  WH_ASSERT(index_s[0] == this->indexIn (0, 0, 0));
  WH_ASSERT(index_s[1] == this->indexOutOfRange ());
  
  this->allocateIndexsOn 
    (this->maxRange (),
     index_s);
  WH_ASSERT(index_s.size () == 2);
  WH_ASSERT(index_s[0] == this->indexIn (this->xCells () - 1, 
				       this->yCells () - 1, 
				       this->zCells () - 1));
  WH_ASSERT(index_s[1] == this->indexOutOfRange ());
  
  this->allocateIndexsOn 
    (this->minRange () - this->cellSize (),
     index_s);
  WH_ASSERT(index_s.size () == 1);
  WH_ASSERT(index_s[0] == this->indexOutOfRange ());
  
  this->allocateIndexsOn 
    (this->maxRange () + this->cellSize (),
     index_s);
  WH_ASSERT(index_s.size () == 1);
  WH_ASSERT(index_s[0] == this->indexOutOfRange ());
  
  this->allocateIndexsOn 
    (this->minRange () + this->cellSize () * 0.5,
     index_s);
  WH_ASSERT(index_s.size () == 1);
  WH_ASSERT(index_s[0] == this->indexIn (0, 0, 0));
  
  this->allocateIndexsOn 
    (this->maxRange () - this->cellSize () * 0.5,
     index_s);
  WH_ASSERT(index_s.size () == 1);
  WH_ASSERT(index_s[0] == this->indexIn (this->xCells () - 1, 
				       this->yCells () - 1, 
				       this->zCells () - 1));
  
  if (1 < this->xCells () 
      && 1 < this->yCells () 
      && 1 < this->zCells ()) {
    WH_CVR_LINE;

    this->allocateIndexsOn 
      (this->minRange () + this->cellSize (),
       index_s);
    WH_ASSERT(index_s.size () == 8);
    WH_ASSERT(index_s[0] == this->indexIn (0, 0, 0));
    WH_ASSERT(index_s[1] == this->indexIn (0, 0, 1));
    WH_ASSERT(index_s[2] == this->indexIn (0, 1, 0));
    WH_ASSERT(index_s[3] == this->indexIn (0, 1, 1));
    WH_ASSERT(index_s[4] == this->indexIn (1, 0, 0));
    WH_ASSERT(index_s[5] == this->indexIn (1, 0, 1));
    WH_ASSERT(index_s[6] == this->indexIn (1, 1, 0));
    WH_ASSERT(index_s[7] == this->indexIn (1, 1, 1));

    this->allocateIndexsOn 
      (this->maxRange () - this->cellSize (),
       index_s);
    WH_ASSERT(index_s.size () == 8);
    WH_ASSERT(index_s[0] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 2, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[1] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 2, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[2] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 1, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[3] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 1, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[4] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 2, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[5] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 2, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[6] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 1, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[7] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 1, 
					   this->zCells () - 1));
    
    this->allocateIndexsWithin 
      (this->minRange () + this->cellSize () * 0.5,
       this->minRange () + this->cellSize () * 1.5,
       index_s);
    WH_ASSERT(index_s.size () == 8);
    WH_ASSERT(index_s[0] == this->indexIn (0, 0, 0));
    WH_ASSERT(index_s[1] == this->indexIn (0, 0, 1));
    WH_ASSERT(index_s[2] == this->indexIn (0, 1, 0));
    WH_ASSERT(index_s[3] == this->indexIn (0, 1, 1));
    WH_ASSERT(index_s[4] == this->indexIn (1, 0, 0));
    WH_ASSERT(index_s[5] == this->indexIn (1, 0, 1));
    WH_ASSERT(index_s[6] == this->indexIn (1, 1, 0));
    WH_ASSERT(index_s[7] == this->indexIn (1, 1, 1));

    this->allocateIndexsWithin 
      (this->maxRange () - this->cellSize () * 1.5,
       this->maxRange () - this->cellSize () * 0.5,
       index_s);
    WH_ASSERT(index_s.size () == 8);
    WH_ASSERT(index_s[0] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 2, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[1] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 2, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[2] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 1, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[3] == this->indexIn (this->xCells () - 2, 
					   this->yCells () - 1, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[4] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 2, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[5] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 2, 
					   this->zCells () - 1));
    WH_ASSERT(index_s[6] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 1, 
					   this->zCells () - 2));
    WH_ASSERT(index_s[7] == this->indexIn (this->xCells () - 1, 
					   this->yCells () - 1, 
					   this->zCells () - 1));
  }

  return true;
}

void WH_Bucket3D_A
::allocateIndexsWithin 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 vector<int>& index_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));

  WH_CVR_LINE;

  index_s_OUT.erase (index_s_OUT.begin (),
		     index_s_OUT.end ());

  WH_Vector3D div0 = WH_divide (minRange - _field.minRange (), 
				_field.cellSize (0, 0, 0));
  int cx0 = (int)floor (div0.x + WH::eps);
  if (WH_eq (div0.x, cx0)) cx0--;
  int cy0 = (int)floor (div0.y + WH::eps);
  if (WH_eq (div0.y, cy0)) cy0--;
  int cz0 = (int)floor (div0.z + WH::eps);
  if (WH_eq (div0.z, cz0)) cz0--;

  WH_Vector3D div1 = WH_divide (maxRange - _field.minRange (), 
				_field.cellSize (0, 0, 0)) ;
  int cx1 = (int)floor (div1.x + WH::eps);
  int cy1 = (int)floor (div1.y + WH::eps);
  int cz1 = (int)floor (div1.z + WH::eps);

  WH_ASSERT(cx0 <= cx1);
  WH_ASSERT(cy0 <= cy1);
  WH_ASSERT(cz0 <= cz1);

  int maxIndexs 
    = (cx1 - cx0 + 1) * (cy1 - cy0 + 1) * (cz1 - cz0 + 1) + 1;
  index_s_OUT.reserve (maxIndexs);
  
  bool outOfRangeExists = false;
  for (int cx = cx0; cx <= cx1; cx++) {
    for (int cy = cy0; cy <= cy1; cy++) {
      for (int cz = cz0; cz <= cz1; cz++) {
	int index = this->indexIn (cx, cy, cz);
	if (index == _indexOutOfRange) {
	  WH_CVR_LINE;
	  outOfRangeExists = true;
	} else {
	  WH_CVR_LINE;
	  index_s_OUT.push_back (index);
	}
      }
    }
  }
  if (outOfRangeExists) {
    WH_CVR_LINE;
    index_s_OUT.push_back (_indexOutOfRange);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

void WH_Bucket3D_A
::getItemsWithin_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 vector<void*>& allTheItem_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));

  WH_CVR_LINE;

  allTheItem_s_OUT.erase (allTheItem_s_OUT.begin (), 
			  allTheItem_s_OUT.end ());

  WH_Vector3D div0 = WH_divide (minRange - _field.minRange (), 
				_field.cellSize (0, 0, 0));
  int cx0 = (int)floor (div0.x + WH::eps);
  if (WH_eq (div0.x, cx0)) cx0--;
  int cy0 = (int)floor (div0.y + WH::eps);
  if (WH_eq (div0.y, cy0)) cy0--;
  int cz0 = (int)floor (div0.z + WH::eps);
  if (WH_eq (div0.z, cz0)) cz0--;

  WH_Vector3D div1 = WH_divide (maxRange - _field.minRange (), 
				_field.cellSize (0, 0, 0)) ;
  int cx1 = (int)floor (div1.x + WH::eps);
  int cy1 = (int)floor (div1.y + WH::eps);
  int cz1 = (int)floor (div1.z + WH::eps);

  WH_ASSERT(cx0 <= cx1);
  WH_ASSERT(cy0 <= cy1);
  WH_ASSERT(cz0 <= cz1);

  if (cx0 == cx1 && cy0 == cy1 && cz0 == cz1) {
    WH_CVR_LINE;

    /* single cell */
    /* duplication check is not necessary */

    const list<void*>& item_s = this->listIn (cx0, cy0, cz0);
    for (list<void*>::const_iterator 
	   i_item = item_s.begin ();
	 i_item != item_s.end ();
	 i_item++) {
      void* item_i = (*i_item);
      allTheItem_s_OUT.push_back (item_i);
    }
  } else {
    WH_CVR_LINE;

    /* multiple cells */
    /* duplication check is necessary */

    bool outOfRangeExists = false;
    for (int cx = cx0; cx <= cx1; cx++) {
      for (int cy = cy0; cy <= cy1; cy++) {
	for (int cz = cz0; cz <= cz1; cz++) {
	  int index = this->indexIn (cx, cy, cz);
	  if (index == _indexOutOfRange) {
	    WH_CVR_LINE;
	    outOfRangeExists = true;
	  } else {
	    WH_CVR_LINE;
	    const list<void*>& item_s = this->listAt (index);
	    for (list<void*>::const_iterator 
		   i_item = item_s.begin ();
		 i_item != item_s.end ();
		 i_item++) {
	      void* item_i = (*i_item);
	      vector<void*>::reverse_iterator i_item1 
		= find (allTheItem_s_OUT.rbegin (), 
			allTheItem_s_OUT.rend (), 
			item_i);
	      if (i_item1 == allTheItem_s_OUT.rend ()) {
		WH_CVR_LINE;
		allTheItem_s_OUT.push_back (item_i);
	      }
	    }
	  }
	}
      }
    }
    if (outOfRangeExists) {
      WH_CVR_LINE;
      const list<void*>& item_s = this->listOutOfRange ();
      for (list<void*>::const_iterator 
	     i_item = item_s.begin ();
	   i_item != item_s.end ();
	   i_item++) {
	void* item_i = (*i_item);
	vector<void*>::reverse_iterator i_item1 
	  = find (allTheItem_s_OUT.rbegin (), 
		  allTheItem_s_OUT.rend (), 
		  item_i);
	if (i_item1 == allTheItem_s_OUT.rend ()) {
	  WH_CVR_LINE;
	  allTheItem_s_OUT.push_back (item_i);
	}
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
#endif
}

void WH_Bucket3D_A
::addItemFirstWithin_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 void* item)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(item != WH_NULL);

  WH_CVR_LINE;

  vector<int> index_s;
  this->allocateIndexsWithin 
    (minRange, maxRange, 
     index_s);
  for (int i = 0; i < (int)index_s.size (); i++) {
    this->listAt (index_s[i]).push_front (item);
  }
}

void WH_Bucket3D_A
::addItemLastWithin_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 void* item)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(item != WH_NULL);

  WH_CVR_LINE;

  vector<int> index_s;
  this->allocateIndexsWithin 
    (minRange, maxRange, 
     index_s);
  for (int i = 0; i < (int)index_s.size (); i++) {
    this->listAt (index_s[i]).push_back (item);
  }
}

void WH_Bucket3D_A
::removeItemFromFirstWithin_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 void* item)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(item != WH_NULL);

  WH_CVR_LINE;

  vector<int> index_s;
  this->allocateIndexsWithin 
    (minRange, maxRange, 
     index_s);
  for (int i = 0; i < (int)index_s.size (); i++) {
    list<void*>& item_s = this->listAt (index_s[i]);
    list<void*>::iterator i_item1 
      = find (item_s.begin (), item_s.end (), 
	      item);
    WH_ASSERT(i_item1 != item_s.end ());
    item_s.erase (i_item1);
  }
}

void WH_Bucket3D_A
::removeItemFromLastWithin_A 
(const WH_Vector3D& minRange, const WH_Vector3D& maxRange, 
 void* item)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(item != WH_NULL);

  WH_CVR_LINE;

  vector<int> index_s;
  this->allocateIndexsWithin 
    (minRange, maxRange, 
     index_s);
  for (int i = 0; i < (int)index_s.size (); i++) {
    list<void*>& item_s = this->listAt (index_s[i]);
    WH_ASSERT(0 < item_s.size ());
    list<void*>::iterator i_item1 = item_s.end ();
    WH_ASSERT(i_item1 != item_s.begin ());
    bool found = false;
    for (;;) {
      i_item1--;
      if ((*i_item1) == item) {
	WH_CVR_LINE;
	found = true;
	break;
      }
      WH_ASSERT(i_item1 != item_s.begin ());
    }
    WH_ASSERT(found);
    WH_ASSERT(i_item1 != item_s.end ());
    item_s.erase (i_item1);
  }
}



#ifndef WH_INLINE_ENABLED
#include "bucket3d_inline.cc"
#endif



/* test coverage completed */
