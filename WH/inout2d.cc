/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inout2d.cc */
/* in-out checker of 2-D point within faceted solid */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "inout2d.h"
#include "bucket2d.h"



/* class WH_InOutChecker2D */

WH_InOutChecker2D
::WH_InOutChecker2D 
(double edgeSize)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (0, edgeSize));

  WH_CVR_LINE;

  _isSetUp = false;
  _edgeSize = edgeSize;
  _minRange = WH_Vector2D (0, 0);
  _maxRange = WH_Vector2D (0, 0);
  _segmentBucket = nullptr;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}
 
WH_InOutChecker2D
::~WH_InOutChecker2D ()
{
  WH_CVR_LINE;

  // _segmentBucket automatically destroyed by unique_ptr
  WH_T_Delete (_segment_s);
}

bool WH_InOutChecker2D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (0, _edgeSize));

  if (_isSetUp) {
    WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
    WH_ASSERT(2 < this->segment_s ().size ());
    WH_ASSERT(_segmentBucket != nullptr);
  } else {
    WH_ASSERT(_segmentBucket == nullptr);
  }

  return true;
}

bool WH_InOutChecker2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(_isSetUp);

  return true;
}
  
void WH_InOutChecker2D
::addEdge 
(const WH_Vector2D& point0,
 const WH_Vector2D& point1,
 const WH_Vector2D& normal)
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(WH_ne (point0, point1));
  WH_ASSERT(WH_ne (normal, WH_Vector2D::zero ()));
  WH_ASSERT(WH_isPerpendicular (normal, point1 - point0));
  
  WH_CVR_LINE;

  WH_Vector2D n = normal.normalize ();
  WH_Segment2D seg0 (point0, point1);
  WH_Vector2D n0 = seg0.line ().normal ();

  WH_Segment2D* seg = WH_NULL;
  if (WH_eq (n0, n)) {
    WH_CVR_LINE;
    seg = new WH_Segment2D (point0, point1);
    WH_ASSERT(seg != WH_NULL);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(WH_isReverse (n0, n));
    seg = new WH_Segment2D (point1, point0);
    WH_ASSERT(seg != WH_NULL);
  }
  WH_ASSERT(seg != WH_NULL);
  WH_ASSERT(WH_eq (seg->line ().normal (), n));

  _segment_s.push_back (seg);
}

void WH_InOutChecker2D
::setUp ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(2 < this->segment_s ().size ());

  WH_CVR_LINE;

  /* set <_minRange> and <_maxRange> */
  _minRange = WH_Vector2D::hugeValue ();
  _maxRange = -WH_Vector2D::hugeValue ();
  for (vector<WH_Segment2D*>::const_iterator 
	 i_seg = _segment_s.begin ();
       i_seg != _segment_s.end ();
       i_seg++) {
    WH_Segment2D* seg_i = (*i_seg);

    _minRange = WH_min (seg_i->minRange (), _minRange);
    _maxRange = WH_max (seg_i->maxRange (), _maxRange);
  }
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  /* create 3-D bucket, refine X-Y plane only */

  WH_Vector2D size = _maxRange - _minRange;
  WH_Vector2D extendedMinRange = _minRange - size / 11;
  WH_Vector2D extendedMaxRange = _maxRange + size / 13;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 11, 13 */
  WH_Vector2D extendedSize = extendedMaxRange - extendedMinRange;

  int xCells = (int)ceil (extendedSize.x / _edgeSize + WH::eps);
  if (xCells / 2 == 0) xCells++;

  _segmentBucket = make_unique<WH_Bucket2D<WH_Segment2D>>(
    extendedMinRange, extendedMaxRange, 
    xCells, 1);
  WH_ASSERT(_segmentBucket != nullptr);

  /* register <_segment_s> into <_segmentBucket> */
  for (vector<WH_Segment2D*>::iterator 
	 i_seg = _segment_s.begin ();
       i_seg != _segment_s.end ();
       i_seg++) {
    WH_Segment2D* seg_i = (*i_seg);
    
    _segmentBucket->addItemLastWithin 
      (seg_i->minRange (), seg_i->maxRange (), seg_i);
  }

  _isSetUp = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(_isSetUp);
#endif
}
  
double WH_InOutChecker2D
::edgeSize () const
{
  return _edgeSize;
}

WH_Vector2D WH_InOutChecker2D
::minRange () const
{
  return _minRange;
}

WH_Vector2D WH_InOutChecker2D
::maxRange () const
{
  return _maxRange;
}

const vector<WH_Segment2D*>& WH_InOutChecker2D
::segment_s () const
{
  return _segment_s;
}

WH_InOutChecker2D::ContainmentType WH_InOutChecker2D
::checkContainmentAt (const WH_Vector2D& position) const
{
  WH_CVR_LINE;

  ContainmentType result = OUT;

  if (!WH_between (position, _minRange, _maxRange)) {
    WH_CVR_LINE;
    return result;
  }

  WH_Vector2D pointMinRange (position.x, _minRange.y);
  WH_Vector2D pointMaxRange (position.x, _maxRange.y);
  WH_Line2D line (pointMinRange, pointMaxRange);
  WH_ASSERT(line.contains (position));

  bool plusSideIntersectionPointIsFound = false;
  bool yNormalIsPlus = false;
  double minYWhichIsGreaterThanPositionY = WH::HUGE_VALUE;

  vector<WH_Segment2D*> segment_s;
  _segmentBucket->getItemsWithin
    (pointMinRange, pointMaxRange, 
     segment_s);
  for (vector<WH_Segment2D*>::const_iterator 
	 i_seg = segment_s.begin ();
       i_seg != segment_s.end ();
       i_seg++) {
    WH_Segment2D* seg_i = (*i_seg);
    
    WH_Vector2D intersectionPoint;
    WH_Segment2D::WithLineIntersectionType flag 
      = seg_i->checkIntersectionWith (line,
				      intersectionPoint);
    if (flag == WH_Segment2D::POINT_WITH_LINE) {
      WH_CVR_LINE;
      WH_ASSERT(WH_eq (intersectionPoint.x, position.x));
      
      if (WH_eq (intersectionPoint.y, position.y)) {
	WH_CVR_LINE;
	/* <position> is on boundary */
	result = ON;
	break;
      }
    } else if (flag == WH_Segment2D::COINCIDENT_WITH_LINE) {
      WH_CVR_LINE;
      if (seg_i->contains (position)) {
	WH_CVR_LINE;
	/* <position> is on boundary */
	result = ON;
	break;
      }
      /* <seg_i> is parallel to <line> */
      continue;
    } else {
      WH_CVR_LINE;
      continue;
    }
    
    /* from here, <seg> should not be parallel to <line> */
    WH_Vector2D normal = seg_i->line ().normal ();
    WH_ASSERT(WH_ne (normal.y, 0));
    
    /* from here, there should be only one intersection point.  the
       intersection point should be different from <position> */
    WH_ASSERT(WH_ne (position, intersectionPoint));
    WH_ASSERT(WH_eq (position.x, intersectionPoint.x));
    WH_ASSERT(WH_ne (position.y, intersectionPoint.y));

    WH_ASSERT(WH_lt (position.y, 
		     minYWhichIsGreaterThanPositionY));

    /* if <intersectionPoint> is on the plus side of <position> */
    if (WH_lt (position.y, intersectionPoint.y)) {
      WH_CVR_LINE;

      if (WH_eq (intersectionPoint.y, 
		 minYWhichIsGreaterThanPositionY)) {
	WH_CVR_LINE;
	WH_ASSERT(plusSideIntersectionPointIsFound);
	if (WH_lt (0, normal.y)) {
	  WH_CVR_LINE;
	  yNormalIsPlus = true;
	} else {
	  WH_CVR_LINE;                       /* NOT COVERED */
	  WH_ASSERT(WH_lt (normal.y, 0));
	  /* nothing */
	  /* ignore another minus Y direction normal */
	}
      } else if (WH_lt (intersectionPoint.y, 
			minYWhichIsGreaterThanPositionY)) {
	WH_CVR_LINE;
	/* update <minYWhichIsGreaterThanPositionY> and
           <plusSideIntersectionPointIsFound>, and clear
           <yNormalIsPlus> */

	plusSideIntersectionPointIsFound = true;
	minYWhichIsGreaterThanPositionY = intersectionPoint.y;
	if (WH_lt (0, normal.y)) {
	  WH_CVR_LINE;
	  yNormalIsPlus = true;
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(WH_lt (normal.y, 0));
	  yNormalIsPlus = false;
	}
      }
    }
  }    

  if (result != ON) {
    WH_CVR_LINE;
    if (plusSideIntersectionPointIsFound) {
      WH_CVR_LINE;
      WH_ASSERT(WH_lt (position.y, 
		       minYWhichIsGreaterThanPositionY));
      if (yNormalIsPlus) {
	WH_CVR_LINE;
	result = IN;
      } else {
	WH_CVR_LINE;
	result = OUT;
      }
    } else {
      WH_CVR_LINE;                          /* NOT COVERED */
      result = OUT;
    }
  }

  return result;
}



/* not yet covered 
WH_InOutChecker2D::ContainmentType WH_InOutChecker2D
::checkContainmentAt (const WH_Vector2D& position) const
*/
