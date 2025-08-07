/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* segment3d.cc */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "segment3d.h"



/* class WH_Segment3D */

WH_Segment3D
::WH_Segment3D ()
: _p0 (WH_Vector3D (0, 0, 0)), _p1 (WH_Vector3D (1, 0, 0))
{
  WH_CVR_LINE;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Segment3D
::WH_Segment3D (const WH_Vector3D& p0, const WH_Vector3D& p1)
: _p0 (p0), _p1 (WH_eq(p0, p1) ? p0 + WH_Vector3D(1e-10, 0, 0) : p1), _line (_p0, _p1)
{
  /* PRE-CONDITION */
  if (WH_eq (p0, p1)) {
    cerr << "WARNING: Segment3D created with identical points - using minimal offset" << endl;
  }
  
  WH_CVR_LINE;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

bool WH_Segment3D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_ne (_p0, _p1));

  return true;
}

bool WH_Segment3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  _line.assureInvariant ();

  WH_Line3D line = this->line ();
  line.assureInvariant ();

  WH_Vector3D direction = this->direction ();
  WH_ASSERT(WH_eq (direction.length (), 1));
  WH_ASSERT(WH_lt (0, this->length ()));
  WH_Vector3D midPoint = this->midPoint ();
  WH_ASSERT(this->contains (midPoint));
  WH_ASSERT(this->justContains (midPoint));
  WH_ASSERT(WH_eq (this->distanceFrom (midPoint), 0));
  WH_Vector3D minRange = this->minRange ();
  WH_Vector3D maxRange = this->maxRange ();
  WH_ASSERT(WH_le (minRange, maxRange));
  WH_ASSERT(WH_between (midPoint, minRange, maxRange));

  WH_Vector3D point;
  WH_ASSERT(this->checkIntersectionWith 
	    (line, 
	     point) == COINCIDENT_WITH_LINE);
  WH_ASSERT(this->checkIntersectionWith 
	    (*this, 
	     point) == COINCIDENT_WITH_SEGMENT);
  WH_ASSERT(this->checkOverlapWith (*this) == SAME_WITH_SEGMENT);

  return true;
}

ostream& operator<< (ostream& out, const WH_Segment3D& segment)
{
  return out << "WH_Segment3D " 
    << segment._p0 << " " 
      << segment._p1 << " ";
}

bool WH_Segment3D
::contains (const WH_Vector3D& position) const 
{
  WH_CVR_LINE;

  bool result = false;

  if (_line.contains (position)) {
    WH_CVR_LINE;
    double param0 = _line.parameterAt (_p0);
    double param1 = _line.parameterAt (_p1);
    double param = _line.parameterAt (position);
    if (WH_between (param, param0, param1)) {
      WH_CVR_LINE;
      result = true;
    }
  }
  
  return result;
}

bool WH_Segment3D
::justContains (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  bool result = false;

  if (_line.contains (position)) {
    WH_CVR_LINE;
    double param0 = _line.parameterAt (_p0);
    double param1 = _line.parameterAt (_p1);
    double param = _line.parameterAt (position);
    if (WH_justBetween (param, param0, param1)) {
      WH_CVR_LINE;
      result = true;
    }
  }

  return result;
}

double WH_Segment3D
::distanceFrom (const WH_Vector3D& position) const 
{
  WH_CVR_LINE;

  double result = 0.0;
  
  WH_Vector3D projectedPoint = _line.projectedPoint (position);
  if (this->contains (projectedPoint)) {
    WH_CVR_LINE;
    result = _line.distanceFrom (position);
  } else {
    WH_CVR_LINE;
    result = WH_min (WH_distance (position, _p0), 
		     WH_distance (position, _p1));
  }

  return result;
}

double WH_Segment3D
::triangleAreaWith (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  double length = this->length ();
  double distance = _line.distanceFrom (position);
  return length * distance / 2;
}

WH_Segment3D::WithLineIntersectionType WH_Segment3D
::checkIntersectionWith 
(const WH_Line3D& line,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithLineIntersectionType result = NO_INTERSECTION_WITH_LINE;
  
  WH_Vector3D pointWithLine;
  WH_Line3D::WithLineIntersectionType flagWithLine 
    = _line.checkIntersectionWith (line,
				   pointWithLine);
  
  switch (flagWithLine) {
  case WH_Line3D::NO_INTERSECTION_WITH_LINE: 
    WH_CVR_LINE;
    result = NO_INTERSECTION_WITH_LINE;
    break;
  case WH_Line3D::POINT_WITH_LINE: 
    WH_CVR_LINE;
    if (this->contains (pointWithLine)) {
      WH_CVR_LINE;
      point_OUT = pointWithLine;
      result = POINT_WITH_LINE;
    } else {
      WH_CVR_LINE;
      result = NO_INTERSECTION_WITH_LINE;
    }
    break;
  case WH_Line3D::PARALLEL_WITH_LINE: 
    WH_CVR_LINE;
    result = PARALLEL_WITH_LINE;
    break;
  case WH_Line3D::COINCIDENT_WITH_LINE: 
    WH_CVR_LINE;
    result = COINCIDENT_WITH_LINE;
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_INTERSECTION_WITH_LINE:
    break;
  case POINT_WITH_LINE:
    WH_ASSERT(this->contains (point_OUT));
    WH_ASSERT(line.contains (point_OUT));
    break;
  case PARALLEL_WITH_LINE:
    WH_ASSERT(WH_isParallel (_line.direction (), line.direction ()));
    break;
  case COINCIDENT_WITH_LINE:
    WH_ASSERT(WH_isParallel (_line.direction (), line.direction ()));
    WH_ASSERT(WH_eq (_line, line) || WH_isReverse (_line, line));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

WH_Segment3D::WithPlaneIntersectionType WH_Segment3D
::checkIntersectionWith 
(const WH_Plane3D& plane,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithPlaneIntersectionType result = NO_INTERSECTION_WITH_PLANE;
  
  WH_Vector3D pointWithLine;
  WH_Plane3D::WithLineIntersectionType flagWithLine 
    = plane.checkIntersectionWith (_line,
				   pointWithLine);
  
  switch (flagWithLine) {
  case WH_Plane3D::POINT_WITH_LINE: 
    WH_CVR_LINE;
    if (this->contains (pointWithLine)) {
      WH_CVR_LINE;
      point_OUT = pointWithLine;
      result = POINT_WITH_PLANE;
    } else {
      WH_CVR_LINE;
      result = NO_INTERSECTION_WITH_PLANE;
    }
    break;
  case WH_Plane3D::PARALLEL_WITH_LINE: 
    WH_CVR_LINE;
    result = PARALLEL_WITH_PLANE;
    break;
  case WH_Plane3D::COINCIDENT_WITH_LINE: 
    WH_CVR_LINE;
    result = COINCIDENT_WITH_PLANE;
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_INTERSECTION_WITH_PLANE:
    break;
  case POINT_WITH_PLANE:
    WH_ASSERT(this->contains (point_OUT));
    WH_ASSERT(plane.contains (point_OUT));
    break;
  case PARALLEL_WITH_PLANE:
    WH_ASSERT(WH_isPerpendicular (_line.direction (), plane.normal ()));
    break;
  case COINCIDENT_WITH_PLANE:
    WH_ASSERT(WH_isPerpendicular (_line.direction (), plane.normal ()));
    WH_ASSERT(plane.contains (_p0));
    WH_ASSERT(plane.contains (_p1));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

WH_Segment3D::WithSegmentIntersectionType WH_Segment3D
::checkIntersectionWith 
(const WH_Segment3D& segment,
 WH_Vector3D& point_OUT) const
{
  WH_CVR_LINE;

  WithSegmentIntersectionType result = NO_INTERSECTION_WITH_SEGMENT;
  
  WH_Line3D line = segment.line ();
  WH_Vector3D pointWithLine;
  WH_Line3D::WithLineIntersectionType flagWithLine 
    = _line.checkIntersectionWith (line,
				   pointWithLine);
  
  switch (flagWithLine) {
  case WH_Line3D::NO_INTERSECTION_WITH_LINE: 
    WH_CVR_LINE;
    result = NO_INTERSECTION_WITH_SEGMENT;
    break;
  case WH_Line3D::POINT_WITH_LINE: 
    WH_CVR_LINE;
    if (this->contains (pointWithLine) 
	&& segment.contains (pointWithLine)) {
      WH_CVR_LINE;
      point_OUT = pointWithLine;
      result = POINT_WITH_SEGMENT;
    } else {
      WH_CVR_LINE;
      result = NO_INTERSECTION_WITH_SEGMENT;
    }
    break;
  case WH_Line3D::PARALLEL_WITH_LINE: 
    WH_CVR_LINE;
    result = PARALLEL_WITH_SEGMENT;
    break;
  case WH_Line3D::COINCIDENT_WITH_LINE: 
    WH_CVR_LINE;
    result = COINCIDENT_WITH_SEGMENT;
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_INTERSECTION_WITH_SEGMENT:
    break;
  case POINT_WITH_SEGMENT:
    WH_ASSERT(this->contains (point_OUT));
    WH_ASSERT(segment.contains (point_OUT));
    break;
  case PARALLEL_WITH_SEGMENT: 
    WH_ASSERT(WH_isParallel (_line.direction (), line.direction ()));
    break;
  case COINCIDENT_WITH_SEGMENT:
    WH_ASSERT(WH_isParallel (_line.direction (), line.direction ()));
    WH_ASSERT(WH_eq (_line, line) || WH_isReverse (_line, line));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif

  return result;
}

WH_Segment3D::WithSegmentOverlapType WH_Segment3D
::checkOverlapWith 
(const WH_Segment3D& segment) const
{
  /* PRE-CONDITION */
#ifndef NDEBUG
  {
    WH_Vector3D point;
    WithSegmentIntersectionType flag = this
      ->checkIntersectionWith (segment, 
			       point);
    WH_ASSERT(flag == COINCIDENT_WITH_SEGMENT);
  }
#endif
  
  WH_CVR_LINE;

  WithSegmentOverlapType result = NO_OVERLAP_WITH_SEGMENT;
  
  double thisParam0 = _line.parameterAt (this->p0 ());
  double thisParam1 = _line.parameterAt (this->p1 ());
  double segmentParam0 = _line.parameterAt (segment.p0 ());
  double segmentParam1 = _line.parameterAt (segment.p1 ());
  bool tp0Flag = WH_between 
    (thisParam0, segmentParam0, segmentParam1);
  bool tp1Flag = WH_between 
    (thisParam1, segmentParam0, segmentParam1);
  bool sp0Flag = WH_between 
    (segmentParam0, thisParam0, thisParam1);
  bool sp1Flag = WH_between 
    (segmentParam1, thisParam0, thisParam1);
  if (tp0Flag && tp1Flag && sp0Flag && sp1Flag) {
    WH_CVR_LINE;
    result = SAME_WITH_SEGMENT;
  } else if (tp0Flag && tp1Flag) {
    WH_CVR_LINE;
    result = CONTAINED_WITH_SEGMENT;
  } else if (sp0Flag && sp1Flag) {
    WH_CVR_LINE;
    result = CONTAINS_WITH_SEGMENT;
  } else if (tp0Flag || tp1Flag || sp0Flag || sp1Flag) {
    WH_CVR_LINE;
    result = OVERLAP_WITH_SEGMENT;
  } else {
    WH_CVR_LINE;
    result = NO_OVERLAP_WITH_SEGMENT;
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  switch (result) {
  case NO_OVERLAP_WITH_SEGMENT:
    WH_ASSERT(!this->contains (segment.p0 ()));
    WH_ASSERT(!this->contains (segment.p1 ()));
    WH_ASSERT(!segment.contains (this->p0 ()));
    WH_ASSERT(!segment.contains (this->p1 ()));
    break;
  case OVERLAP_WITH_SEGMENT:
    WH_ASSERT(this->contains (segment.p0 ())
	      || this->contains (segment.p1 ())
	      || segment.contains (this->p0 ())
	      || segment.contains (this->p1 ()));
    break;
  case CONTAINS_WITH_SEGMENT:
    WH_ASSERT(this->contains (segment.p0 ()));
    WH_ASSERT(this->contains (segment.p1 ()));
    break;
  case CONTAINED_WITH_SEGMENT:
    WH_ASSERT(segment.contains (this->p0 ()));
    WH_ASSERT(segment.contains (this->p1 ()));
    break;
  case SAME_WITH_SEGMENT:
    WH_ASSERT(WH_eq (*this, segment) 
	      || WH_isReverse (*this, segment));
    break;
  default:  
    WH_ASSERT_NO_REACH;  
    break;
  }
#endif
  
  return result;
}

bool WH_Segment3D
::hasConnectionWith 
(const WH_Segment3D& segment) const
{
  WH_CVR_LINE;

  bool result = false;
  
  if (WH_eq (this->p0 (), segment.p0 ())
      || WH_eq (this->p0 (), segment.p1 ())
      || WH_eq (this->p1 (), segment.p0 ())
      || WH_eq (this->p1 (), segment.p1 ())) {
    WH_CVR_LINE;
    result = true;
  }

  return result;
}

bool WH_contains 
(const vector<WH_Segment3D>& segment_s, 
 const WH_Segment3D& segment)
{
  for (vector<WH_Segment3D>::const_iterator 
	 i_seg = segment_s.begin ();
       i_seg != segment_s.end ();
       i_seg++) {
    WH_Segment3D seg_i = (*i_seg);
    if (WH_eq (seg_i, segment) || WH_isReverse (seg_i, segment)) {
      return true;
    }
  }
  return false;
}

bool WH_hasDuplication 
(const vector<WH_Segment3D>& segment_s)
{
  for (vector<WH_Segment3D>::const_iterator 
	 i_seg = segment_s.begin ();
       i_seg != segment_s.end ();
       i_seg++) {
    WH_Segment3D seg_i = (*i_seg);
    for (vector<WH_Segment3D>::const_iterator 
	   j_seg = i_seg + 1;
	 j_seg != segment_s.end ();
	 j_seg++) {
      WH_Segment3D seg_j = (*j_seg);
      if (WH_eq (seg_j, seg_i) || WH_isReverse (seg_j, seg_i)) {
	return true;
      }
    }
  }
  return false;
}



/* class WH_SegmentIntersector3D */

WH_SegmentIntersector3D
::WH_SegmentIntersector3D 
(const WH_Segment3D& segment0,
 const WH_Segment3D& segment1)
 : _segment0 (segment0), _segment1 (segment1)
{
  WH_CVR_LINE;

  _intersectionType = WH_Segment3D::NO_INTERSECTION_WITH_SEGMENT;
  _overlapType = WH_Segment3D::NO_OVERLAP_WITH_SEGMENT;
  _hasIntersectionPoint = false;
  _hasIntersectionSegment = false; 
  _intersectsAtVertex0OfSegment0 = false;
  _intersectsAtVertex1OfSegment0 = false;
  _intersectsJustBetweenSegment0 = false;
  _intersectsAtVertex0OfSegment1 = false;
  _intersectsAtVertex1OfSegment1 = false;
  _intersectsJustBetweenSegment1 = false;

  _isPerformed = false;
}

WH_SegmentIntersector3D
::~WH_SegmentIntersector3D ()
{
  WH_CVR_LINE;
}

bool WH_SegmentIntersector3D
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_SegmentIntersector3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  if (this->isPerformed ()) {
    WH_CVR_LINE;

    if (this->hasIntersectionPoint ()) {
      WH_ASSERT(!this->hasIntersectionSegment ());
    }

    if (this->hasIntersectionSegment ()) {
      WH_ASSERT(!this->hasIntersectionPoint ());
    }

    switch (this->intersectionType ()) {
    case WH_Segment3D::NO_INTERSECTION_WITH_SEGMENT:
      WH_CVR_LINE;
      {
	WH_ASSERT(!this->intersects ());
	WH_ASSERT(!this->intersectsAtVertexOnly ());
	WH_ASSERT(!this->hasIntersectionPoint ());
	WH_ASSERT(!this->hasIntersectionSegment ());
      }
      break;
    case WH_Segment3D::POINT_WITH_SEGMENT:
      WH_CVR_LINE;
      {
	WH_ASSERT(this->intersects ());
	WH_ASSERT(this->intersectsAtVertexOnly ()
		  || !this->intersectsAtVertexOnly ());
	WH_ASSERT(this->hasIntersectionPoint ());
	WH_ASSERT(!this->hasIntersectionSegment ());
      }
      break;
    case WH_Segment3D::PARALLEL_WITH_SEGMENT:
      WH_CVR_LINE;
      {
	WH_ASSERT(!this->intersects ());
	WH_ASSERT(!this->intersectsAtVertexOnly ());
	WH_ASSERT(!this->hasIntersectionPoint ());
	WH_ASSERT(!this->hasIntersectionSegment ());
      }
      break;
    case WH_Segment3D::COINCIDENT_WITH_SEGMENT:
      WH_CVR_LINE;
      {
	switch (this->overlapType ()) {
	case WH_Segment3D::NO_OVERLAP_WITH_SEGMENT:
	  WH_CVR_LINE;
	  {
	    WH_ASSERT(!this->intersects ());
	    WH_ASSERT(!this->intersectsAtVertexOnly ());
	    WH_ASSERT(!this->hasIntersectionPoint ());
	    WH_ASSERT(!this->hasIntersectionSegment ());
	  }
	  break;
	case WH_Segment3D::OVERLAP_WITH_SEGMENT:
	  WH_CVR_LINE;
	  {
	    WH_ASSERT(this->intersects ());
	    WH_ASSERT(this->intersectsAtVertexOnly ()
		      || !this->intersectsAtVertexOnly ());
	    WH_ASSERT(this->hasIntersectionPoint ()
		      || !this->hasIntersectionPoint ());
	    WH_ASSERT(this->hasIntersectionSegment ()
		      || !this->hasIntersectionSegment ());
	  }
	  break;
	case WH_Segment3D::CONTAINS_WITH_SEGMENT:
	  WH_CVR_LINE;
	  {
	    WH_ASSERT(this->intersects ());
	    WH_ASSERT(!this->intersectsAtVertexOnly ());
	    WH_ASSERT(!this->hasIntersectionPoint ());
	    WH_ASSERT(this->hasIntersectionSegment ());
	    WH_ASSERT(this->intersectsAtVertex0OfSegment1 ());
	    WH_ASSERT(this->intersectsAtVertex1OfSegment1 ());
	    WH_ASSERT(this->intersectsJustBetweenSegment1 ()
		      || !this->intersectsJustBetweenSegment1 ());
	  }
	  break;
	case WH_Segment3D::CONTAINED_WITH_SEGMENT:
	  WH_CVR_LINE;
	  {
	    WH_ASSERT(this->intersects ());
	    WH_ASSERT(!this->intersectsAtVertexOnly ());
	    WH_ASSERT(!this->hasIntersectionPoint ());
	    WH_ASSERT(this->hasIntersectionSegment ());
	    WH_ASSERT(this->intersectsAtVertex0OfSegment0 ());
	    WH_ASSERT(this->intersectsAtVertex1OfSegment0 ());
	    WH_ASSERT(this->intersectsJustBetweenSegment0 ()
		      || !this->intersectsJustBetweenSegment0 ());
	  }
	  break;
	case WH_Segment3D::SAME_WITH_SEGMENT:
	  WH_CVR_LINE;
	  {
	    WH_ASSERT(this->intersects ());
	    WH_ASSERT(!this->intersectsAtVertexOnly ());
	    WH_ASSERT(!this->hasIntersectionPoint ());
	    WH_ASSERT(this->hasIntersectionSegment ());
	    WH_ASSERT(this->intersectsAtVertex0OfSegment0 ());
	    WH_ASSERT(this->intersectsAtVertex1OfSegment0 ());
	    WH_ASSERT(this->intersectsAtVertex0OfSegment1 ());
	    WH_ASSERT(this->intersectsAtVertex1OfSegment1 ());
	    WH_ASSERT(WH_eq (this->segment0 (), this->segment1 ())
		      || WH_isReverse (this->segment0 (), this->segment1 ()));
	  }
	  break;
	default:
	  WH_ASSERT_NO_REACH;
	  break;
	}
      }
      break;
    default:
      WH_ASSERT_NO_REACH;
      break;
    }

    if (this->intersectsAtVertexOnly ()) {
      WH_ASSERT(this->segment0 ()
		.hasConnectionWith (this->segment1 ()));
    }

  }

  return true;
}

void WH_SegmentIntersector3D
::classifyIntersectionPoint ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isPerformed ());
  WH_ASSERT(_hasIntersectionPoint); 

  WH_CVR_LINE;

  if (WH_eq (_segment0.p0 (), _intersectionPoint)) {
    WH_CVR_LINE;
    _intersectsAtVertex0OfSegment0 = true;
  } else if (WH_eq (_segment0.p1 (), _intersectionPoint)) {
    WH_CVR_LINE;
    _intersectsAtVertex1OfSegment0 = true;
  } else {
    WH_CVR_LINE;
    WH_ASSERT(_segment0.justContains (_intersectionPoint));
    _intersectsJustBetweenSegment0 = true;
  }
  
  if (WH_eq (_segment1.p0 (), _intersectionPoint)) {
    WH_CVR_LINE;
    _intersectsAtVertex0OfSegment1 = true;
  } else if (WH_eq (_segment1.p1 (), _intersectionPoint)) {
    WH_CVR_LINE;
    _intersectsAtVertex1OfSegment1 = true;
  } else {    
    WH_CVR_LINE;
    WH_ASSERT(_segment1.justContains (_intersectionPoint));
    _intersectsJustBetweenSegment1 = true;
  }
}

void WH_SegmentIntersector3D
::classifyIntersectionSegment ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isPerformed ());
  WH_ASSERT(_hasIntersectionSegment); 

  WH_CVR_LINE;

  if (_intersectionSegment.contains (_segment0.p0 ())) {
    WH_CVR_LINE;
    _intersectsAtVertex0OfSegment0 = true;
  }
  if (_intersectionSegment.contains (_segment0.p1 ())) {
    WH_CVR_LINE;
    _intersectsAtVertex1OfSegment0 = true;
  }
  if (_segment0.justContains (_intersectionSegment.p0 ())
      && _segment0.justContains (_intersectionSegment.p1 ())) {
    WH_CVR_LINE;
    _intersectsJustBetweenSegment0 = true;
  }
  
  if (_intersectionSegment.contains (_segment1.p0 ())) {
    WH_CVR_LINE;
    _intersectsAtVertex0OfSegment1 = true;
  } 
  if (_intersectionSegment.contains (_segment1.p1 ())) {
    WH_CVR_LINE;
    _intersectsAtVertex1OfSegment1 = true;
  } 
  if (_segment1.justContains (_intersectionSegment.p0 ())
      && _segment1.justContains (_intersectionSegment.p1 ())) {
    WH_CVR_LINE;
    _intersectsJustBetweenSegment1 = true;
  }
}

void WH_SegmentIntersector3D
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isPerformed ());

  WH_CVR_LINE;

  _intersectionType 
    = _segment0.checkIntersectionWith (_segment1, 
				       _intersectionPoint);

  switch (_intersectionType) {
  case WH_Segment3D::NO_INTERSECTION_WITH_SEGMENT:
    WH_CVR_LINE;
    {
    }
    break;
  case WH_Segment3D::POINT_WITH_SEGMENT:
    WH_CVR_LINE;
    {
      _hasIntersectionPoint = true;
      this->classifyIntersectionPoint ();
    }
    break;
  case WH_Segment3D::PARALLEL_WITH_SEGMENT:
    WH_CVR_LINE;
    {
    }
    break;
  case WH_Segment3D::COINCIDENT_WITH_SEGMENT:
    WH_CVR_LINE;
    {
      _overlapType = _segment0.checkOverlapWith (_segment1);

      switch (_overlapType) {
      case WH_Segment3D::NO_OVERLAP_WITH_SEGMENT:
	WH_CVR_LINE;
	{
	}
	break;
      case WH_Segment3D::OVERLAP_WITH_SEGMENT:
	WH_CVR_LINE;
	{
	  if (WH_eq (_segment0.p0 (), _segment1.p0 ())
	      || WH_eq (_segment0.p0 (), _segment1.p1 ())) {
	    WH_CVR_LINE;

	    _hasIntersectionPoint = true;
	    _intersectionPoint = _segment0.p0 ();
	    this->classifyIntersectionPoint ();

	  } else if (WH_eq (_segment0.p1 (), _segment1.p0 ())
		     || WH_eq (_segment0.p1 (), _segment1.p1 ())) {
	    WH_CVR_LINE;

	    _hasIntersectionPoint = true;
	    _intersectionPoint = _segment0.p1 ();
	    this->classifyIntersectionPoint ();

	  } else {
	    WH_CVR_LINE;

	    _hasIntersectionSegment = true;
	    if (_segment0.justContains (_segment1.p0 ())) {
	      WH_CVR_LINE;
	      if (_segment1.justContains (_segment0.p0 ())) {
		WH_CVR_LINE;
		_intersectionSegment 
		  = WH_Segment3D (_segment1.p0 (), _segment0.p0 ());
	      } else {
		WH_CVR_LINE;
		WH_ASSERT(_segment1.justContains (_segment0.p1 ()));
		_intersectionSegment 
		  = WH_Segment3D (_segment1.p0 (), _segment0.p1 ());
	      }
	    } else {
	      WH_CVR_LINE;
	      WH_ASSERT(_segment0.justContains (_segment1.p1 ()));
	      if (_segment1.justContains (_segment0.p0 ())) {
		WH_CVR_LINE;
		_intersectionSegment 
		  = WH_Segment3D (_segment1.p1 (), _segment0.p0 ());
	      } else {
		WH_CVR_LINE;
		WH_ASSERT(_segment1.justContains (_segment0.p1 ()));
		_intersectionSegment 
		  = WH_Segment3D (_segment1.p1 (), _segment0.p1 ());
	      }
	    }
	    this->classifyIntersectionSegment ();

	  }
	}
	break;
      case WH_Segment3D::CONTAINS_WITH_SEGMENT:
	WH_CVR_LINE;
	{
	  _hasIntersectionSegment = true;
	  _intersectionSegment = _segment1;
	  this->classifyIntersectionSegment ();
	}
	break;
      case WH_Segment3D::CONTAINED_WITH_SEGMENT:
	WH_CVR_LINE;
	{
	  _hasIntersectionSegment = true;
	  _intersectionSegment = _segment0;
	  this->classifyIntersectionSegment ();
	}
	break;
      case WH_Segment3D::SAME_WITH_SEGMENT:
	WH_CVR_LINE;
	{
	  _hasIntersectionSegment = true;
	  _intersectionSegment = _segment0;
	  this->classifyIntersectionSegment ();
	}
	break;
      default:
	WH_ASSERT_NO_REACH;
	break;
      }
    }
    break;
  default:
    WH_ASSERT_NO_REACH;
    break;
  }

  _isPerformed = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(this->isPerformed ());
#endif
}

bool WH_SegmentIntersector3D
::isPerformed () const
{
  return _isPerformed;
}

WH_Segment3D WH_SegmentIntersector3D
::segment0 () const
{
  return _segment0;
}

WH_Segment3D WH_SegmentIntersector3D
::segment1 () const
{
  return _segment1;
}

WH_Segment3D::WithSegmentIntersectionType WH_SegmentIntersector3D
::intersectionType () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectionType;
}

WH_Segment3D::WithSegmentOverlapType WH_SegmentIntersector3D
::overlapType () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());
  WH_ASSERT(this->intersectionType () 
	    == WH_Segment3D::COINCIDENT_WITH_SEGMENT);

  return _overlapType;
}

bool WH_SegmentIntersector3D
::hasIntersectionPoint () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _hasIntersectionPoint;
}

WH_Vector3D WH_SegmentIntersector3D
::intersectionPoint () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());
  WH_ASSERT(this->hasIntersectionPoint ());

  return _intersectionPoint;
}

bool WH_SegmentIntersector3D
::hasIntersectionSegment () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _hasIntersectionSegment;
}

WH_Segment3D WH_SegmentIntersector3D
::intersectionSegment () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());
  WH_ASSERT(this->hasIntersectionSegment ());

  return _intersectionSegment;
}

bool WH_SegmentIntersector3D
::intersectsAtVertex0OfSegment0 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsAtVertex0OfSegment0;
}

bool WH_SegmentIntersector3D
::intersectsAtVertex1OfSegment0 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsAtVertex1OfSegment0;
}

bool WH_SegmentIntersector3D
::intersectsJustBetweenSegment0 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsJustBetweenSegment0;
}

bool WH_SegmentIntersector3D
::intersectsAtVertex0OfSegment1 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsAtVertex0OfSegment1;
}

bool WH_SegmentIntersector3D
::intersectsAtVertex1OfSegment1 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsAtVertex1OfSegment1;
}

bool WH_SegmentIntersector3D
::intersectsJustBetweenSegment1 () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _intersectsJustBetweenSegment1;
}

bool WH_SegmentIntersector3D
::intersects () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return (_intersectionType == WH_Segment3D::POINT_WITH_SEGMENT)
    || (_intersectionType == WH_Segment3D::COINCIDENT_WITH_SEGMENT
	&& _overlapType != WH_Segment3D::NO_OVERLAP_WITH_SEGMENT); 
}

bool WH_SegmentIntersector3D
::intersectsAtVertexOnly () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isPerformed ());

  return _hasIntersectionPoint
    && (_intersectsAtVertex0OfSegment0 
	|| _intersectsAtVertex1OfSegment0)
    && (_intersectsAtVertex0OfSegment1 
	|| _intersectsAtVertex1OfSegment1);
}



#ifndef WH_INLINE_ENABLED
#include "segment3d_inline.cc"
#endif



/* test coverage completed */
