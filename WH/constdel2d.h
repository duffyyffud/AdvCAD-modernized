/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for constdel2d.cc */

#pragma once


#ifndef WH_INCLUDED_WH_DELAUNAY2D
#include <WH/delaunay2d.h>
#define WH_INCLUDED_WH_DELAUNAY2D
#endif

template <class Type> class WH_HashBucket;



class WH_CDLN2D_Triangle;
class WH_CDLN2D_BoundarySegment;
class WH_CDLN2D_Triangulator;

class WH_CDLN2D_BoundarySegment {
 public:
  WH_CDLN2D_BoundarySegment 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1,
     int frontDomainId,
     int rearDomainId);
  virtual ~WH_CDLN2D_BoundarySegment ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* 
             front
     point0 -------- point1
             back     
   */

  /* base */
  void setMark ();

  WH_DLN2D_Point* point0 () const;
  WH_DLN2D_Point* point1 () const;

  bool hasPoint (WH_DLN2D_Point* point) const;

  int frontDomainId () const;
  int rearDomainId () const;

  WH_Vector2D frontNormal () const;

  bool hasMark () const;

  /* derived */

 protected:
  WH_DLN2D_Point* _point0;
  WH_DLN2D_Point* _point1;

  int _frontDomainId;
  int _rearDomainId;

  bool _hasMark;

  /* base */

  /* derived */

};

class WH_CDLN2D_Triangle : public WH_DLN2D_Triangle {
 public:
  WH_CDLN2D_Triangle 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1, 
     WH_DLN2D_Point* point2);
  virtual ~WH_CDLN2D_Triangle ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  void setId (int id);

  void setDomainId (int id);

  int id () const;

  int domainId () const;

  /* derived */

 protected:
  int _id;

  int _domainId;

  /* base */

  /* derived */

};

class WH_CDLN2D_Triangulator : public WH_DLN2D_Triangulator {
 public:
  WH_CDLN2D_Triangulator ();
  virtual ~WH_CDLN2D_Triangulator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addBoundarySegment 
    (WH_CDLN2D_BoundarySegment* seg  /* ADOPT */);

  const vector<WH_CDLN2D_BoundarySegment*>& boundarySegment_s () const;

  /* derived */
  virtual void perform ();

  /* derived */
  
 protected:
  vector<WH_CDLN2D_BoundarySegment*> _boundarySegment_s;  /* own */

  WH_HashBucket<WH_CDLN2D_BoundarySegment>* _boundarySegmentBucket;
  
  /* base */
  virtual WH_CDLN2D_BoundarySegment* findBoundarySegment 
    (WH_DLN2D_Point* point0,
     WH_DLN2D_Point* point1) const;
  
  virtual void setUpBoundarySegmentBucket ();

  virtual void fitBoundary ();
  
  virtual void identifyDomain ();

  /* derived */

  /* factory method */
  virtual WH_DLN2D_Triangle* createTriangle 
    (WH_DLN2D_Point* point0, 
     WH_DLN2D_Point* point1,
     WH_DLN2D_Point* point2);

};



#ifdef WH_INLINE_ENABLED
#include <WH/constdel2d_inline.cc>
#endif




