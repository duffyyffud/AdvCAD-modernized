/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for inout2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif

template <class Type> class WH_Bucket2D;

class WH_InOutChecker2D;

/* value-based class */
/* heavy weight */
class WH_InOutChecker2D {
 public:
  WH_InOutChecker2D 
    (double edgeSize);
  virtual ~WH_InOutChecker2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addEdge 
    (const WH_Vector2D& point0,
     const WH_Vector2D& point1,
     const WH_Vector2D& normal);

  virtual void setUp ();
  
  double edgeSize () const;

  WH_Vector2D minRange () const;
  WH_Vector2D maxRange () const;

  const vector<WH_Segment2D*>& segment_s () const;

  enum ContainmentType {
    IN, OUT, ON
  };
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector2D& position) const;
  
  /* derived */
  
 protected:
  bool _isSetUp;

  double _edgeSize;

  WH_Vector2D _minRange;
  WH_Vector2D _maxRange;

  vector<WH_Segment2D*> _segment_s;  /* OWN */
  
  unique_ptr<WH_Bucket2D<WH_Segment2D>> _segmentBucket;  /* OWN */

  /* base */

  /* derived */

};
