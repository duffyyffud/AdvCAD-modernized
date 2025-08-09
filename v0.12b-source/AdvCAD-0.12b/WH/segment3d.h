/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for segment3d.cc */

#ifndef WH_HEADER_WH_SEGMENT3D
#define WH_HEADER_WH_SEGMENT3D

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif

#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif



class WH_Segment3D;
class WH_SegmentIntersector3D;



/* value-based class */
/* no inheritance */
class WH_Segment3D {
 public:
  WH_Segment3D ();
  WH_Segment3D (const WH_Vector3D& p0, const WH_Vector3D& p1);
  bool checkInvariant () const;
  bool assureInvariant () const;
  friend ostream& operator<< (ostream& out, const WH_Segment3D& segment);

  WH_Vector3D p0 () const;
  WH_Vector3D p1 () const;
  WH_Line3D line () const;
  WH_Vector3D direction () const;
  double length () const;
  WH_Vector3D midPoint () const;
  WH_Vector3D minRange () const;
  WH_Vector3D maxRange () const;
  bool contains (const WH_Vector3D& position) const;
  bool justContains (const WH_Vector3D& position) const;
  double distanceFrom (const WH_Vector3D& position) const;
  bool isParallelToXAxis () const;
  bool isParallelToYAxis () const;
  bool isParallelToZAxis () const;
  double triangleAreaWith (const WH_Vector3D& position) const;
  bool hasVertexAt (const WH_Vector3D& position) const;
  enum WithLineIntersectionType {
    NO_INTERSECTION_WITH_LINE, 
    POINT_WITH_LINE, 
    PARALLEL_WITH_LINE, 
    COINCIDENT_WITH_LINE
  };
  WithLineIntersectionType checkIntersectionWith 
    (const WH_Line3D& line,
     WH_Vector3D& point_OUT) const;
  enum WithPlaneIntersectionType {
    NO_INTERSECTION_WITH_PLANE, 
    POINT_WITH_PLANE, 
    PARALLEL_WITH_PLANE, 
    COINCIDENT_WITH_PLANE
  };
  WithPlaneIntersectionType checkIntersectionWith 
    (const WH_Plane3D& plane,
     WH_Vector3D& point_OUT) const;
  enum WithSegmentIntersectionType {
    NO_INTERSECTION_WITH_SEGMENT, 
    POINT_WITH_SEGMENT, 
    PARALLEL_WITH_SEGMENT, 
    COINCIDENT_WITH_SEGMENT
  };
  WithSegmentIntersectionType checkIntersectionWith 
    (const WH_Segment3D& segment,
     WH_Vector3D& point_OUT) const;
  enum WithSegmentOverlapType {
    NO_OVERLAP_WITH_SEGMENT, 
    OVERLAP_WITH_SEGMENT, 
    CONTAINS_WITH_SEGMENT, 
    CONTAINED_WITH_SEGMENT, 
    SAME_WITH_SEGMENT
  };
  WithSegmentOverlapType checkOverlapWith 
    (const WH_Segment3D& segment) const;
  bool hasConnectionWith 
    (const WH_Segment3D& segment) const;

 private:
  WH_Vector3D _p0;
  WH_Vector3D _p1;
  WH_Line3D _line;

};

/* non-inline free functions using WH_Segment3D */

bool WH_contains 
(const vector<WH_Segment3D>& segment_s, 
 const WH_Segment3D& segment);
bool WH_hasDuplication 
(const vector<WH_Segment3D>& segment_s);

/* inline free functions using WH_Segment3D */
#ifndef WH_INLINE_ENABLED  

bool WH_eq (const WH_Segment3D& s0, const WH_Segment3D& s1);
bool WH_ne (const WH_Segment3D& s0, const WH_Segment3D& s1);
bool WH_isReverse (const WH_Segment3D& s0, const WH_Segment3D& s1);

#endif



/* value-based class */
/* heavy weight */
class WH_SegmentIntersector3D {
public:
  WH_SegmentIntersector3D 
  (const WH_Segment3D& segment0,
   const WH_Segment3D& segment1);
  virtual ~WH_SegmentIntersector3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  bool isPerformed () const;

  WH_Segment3D segment0 () const;
  WH_Segment3D segment1 () const;

  WH_Segment3D::WithSegmentIntersectionType intersectionType () const;
  WH_Segment3D::WithSegmentOverlapType overlapType () const;

  bool hasIntersectionPoint () const;
  WH_Vector3D intersectionPoint () const;

  bool hasIntersectionSegment () const;
  WH_Segment3D intersectionSegment () const;

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
  
  WH_Segment3D _segment0;
  WH_Segment3D _segment1;
  
  WH_Segment3D::WithSegmentIntersectionType _intersectionType;
  WH_Segment3D::WithSegmentOverlapType _overlapType;

  bool _hasIntersectionPoint;
  WH_Vector3D _intersectionPoint;

  bool _hasIntersectionSegment;
  WH_Segment3D _intersectionSegment;

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
#include <WH/segment3d_inline.cc>
#endif

#endif /* WH_HEADER_WH_SEGMENT3D */
