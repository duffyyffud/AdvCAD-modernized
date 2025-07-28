/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for segment2d.cc */

#ifndef WH_HEADER_WH_SEGMENT2D
#define WH_HEADER_WH_SEGMENT2D

#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif



class WH_Segment2D;
class WH_SegmentIntersector2D;



/* value-based class */
/* no inheritance */
class WH_Segment2D {
 public:
  WH_Segment2D ();
  WH_Segment2D (const WH_Vector2D& p0, const WH_Vector2D& p1);
  bool checkInvariant () const;
  bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Segment2D& segment);

  WH_Vector2D p0 () const;
  WH_Vector2D p1 () const;
  WH_Line2D line () const;
  WH_Vector2D normal () const;
  WH_Vector2D direction () const;
  double length () const;
  WH_Vector2D midPoint () const;
  WH_Vector2D minRange () const;
  WH_Vector2D maxRange () const;
  bool contains (const WH_Vector2D& position) const;
  bool justContains (const WH_Vector2D& position) const;
  double distanceFrom (const WH_Vector2D& position) const;
  bool isParallelToXAxis () const;
  bool isParallelToYAxis () const;
  double signedTriangleAreaWith (const WH_Vector2D& position) const;
  bool isClockWiseWith (const WH_Vector2D& position) const;
  bool hasVertexAt (const WH_Vector2D& position) const;
  enum WithLineIntersectionType {
    NO_INTERSECTION_WITH_LINE, 
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  WithLineIntersectionType checkIntersectionWith 
    (const WH_Line2D& line,
     WH_Vector2D& point_OUT) const;
  enum WithSegmentIntersectionType {
    NO_INTERSECTION_WITH_SEGMENT, 
    POINT_WITH_SEGMENT, 
    PARALLEL_WITH_SEGMENT, 
    COINCIDENT_WITH_SEGMENT
  };
  WithSegmentIntersectionType checkIntersectionWith 
    (const WH_Segment2D& segment,
     WH_Vector2D& point_OUT) const;
  enum WithSegmentOverlapType {
    NO_OVERLAP_WITH_SEGMENT, 
    OVERLAP_WITH_SEGMENT, 
    CONTAINS_WITH_SEGMENT, 
    CONTAINED_WITH_SEGMENT, 
    SAME_WITH_SEGMENT
  };
  WithSegmentOverlapType checkOverlapWith 
    (const WH_Segment2D& segment) const;
  bool hasConnectionWith 
    (const WH_Segment2D& segment) const;

 private:
  WH_Vector2D _p0;
  WH_Vector2D _p1;
  WH_Line2D _line;

};

/* non-inline free functions using WH_Segment2D */

bool WH_contains 
(const vector<WH_Segment2D>& segment_s, 
 const WH_Segment2D& segment);
bool WH_hasDuplication 
(const vector<WH_Segment2D>& segment_s);

/* inline free functions using WH_Segment2D */
#ifndef WH_INLINE_ENABLED  

bool WH_eq (const WH_Segment2D& s0, const WH_Segment2D& s1);
bool WH_ne (const WH_Segment2D& s0, const WH_Segment2D& s1);
bool WH_isReverse (const WH_Segment2D& s0, const WH_Segment2D& s1);

#endif



/* value-based class */
/* heavy weight */
class WH_SegmentIntersector2D {
public:
  WH_SegmentIntersector2D 
  (const WH_Segment2D& segment0,
   const WH_Segment2D& segment1);
  virtual ~WH_SegmentIntersector2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  bool isPerformed () const;

  WH_Segment2D segment0 () const;
  WH_Segment2D segment1 () const;

  WH_Segment2D::WithSegmentIntersectionType intersectionType () const;
  WH_Segment2D::WithSegmentOverlapType overlapType () const;

  bool hasIntersectionPoint () const;
  WH_Vector2D intersectionPoint () const;

  bool hasIntersectionSegment () const;
  WH_Segment2D intersectionSegment () const;

  bool intersectsAtVertex0OfSegment0 () const;
  bool intersectsAtVertex1OfSegment0 () const;
  bool intersectsJustBetweenSegment0 () const;

  bool intersectsAtVertex0OfSegment1 () const;
  bool intersectsAtVertex1OfSegment1 () const;
  bool intersectsJustBetweenSegment1 () const;

  virtual bool intersects () const;
  
  virtual bool intersectsAtVertexOnly () const;
  
  /* derived */
  
protected:
  bool _isPerformed;
  
  WH_Segment2D _segment0;
  WH_Segment2D _segment1;
  
  WH_Segment2D::WithSegmentIntersectionType _intersectionType;
  WH_Segment2D::WithSegmentOverlapType _overlapType;

  bool _hasIntersectionPoint;
  WH_Vector2D _intersectionPoint;

  bool _hasIntersectionSegment;
  WH_Segment2D _intersectionSegment;

  bool _intersectsAtVertex0OfSegment0;
  bool _intersectsAtVertex1OfSegment0;
  bool _intersectsJustBetweenSegment0;

  bool _intersectsAtVertex0OfSegment1;
  bool _intersectsAtVertex1OfSegment1;
  bool _intersectsJustBetweenSegment1;

  /* base */
  virtual void classifyIntersectionPoint ();

  virtual void classifyIntersectionSegment ();

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/segment2d_inline.cc>
#endif

#endif /* WH_HEADER_WH_SEGMENT2D */
