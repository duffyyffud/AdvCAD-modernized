/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inout3d.cc */
/* in-out checker of 3-D point within triangulated solid */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "inout3d.h"
#include "bucket3d.h"



#if 1

/* optimized version using WH_Triangle3D_IOC3D */

#if 1
#define TWO_DIRECTION_SEARCH
#endif

/* class WH_Triangle3D_IOC3D */

WH_Triangle3D_IOC3D
::WH_Triangle3D_IOC3D 
(const WH_Vector3D& v0, 
 const WH_Vector3D& v1,
 const WH_Vector3D& v2)
  : WH_Triangle3D (v0, v1, v2)
{
  WH_CVR_LINE;

  for (int iVertex = 0; iVertex < 3; iVertex++) {
    _vertex2Ds[iVertex] = _plane.parameterAt (_vertexs[iVertex]);
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}

WH_Triangle3D_IOC3D
::~WH_Triangle3D_IOC3D ()
{
  WH_CVR_LINE;
}

bool WH_Triangle3D_IOC3D
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle3D::checkInvariant ();
  
  return true;
}

bool WH_Triangle3D_IOC3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_Triangle3D::assureInvariant ();

  {
    WH_Vector3D v0 = this->vertex (0);
    WH_Vector3D v1 = this->vertex (1);
    WH_Vector3D v2 = this->vertex (2);
    WH_ASSERT(this->containsPointWhichIsOnPlane (v0));
    WH_ASSERT(this->containsPointWhichIsOnPlane (v1));
    WH_ASSERT(this->containsPointWhichIsOnPlane (v2));
    
    WH_Vector3D center = (v0 + v1 + v2) / 3;
    WH_ASSERT(this->containsPointWhichIsOnPlane (center));
  }

  return true;
}
  
bool WH_Triangle3D_IOC3D
::containsPointWhichIsOnPlane (const WH_Vector3D& position) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->plane ().contains (position));

  WH_CVR_LINE;

  bool result = false;

  WH_Vector2D position2D = _plane.parameterAt (position);
  double sign0 = WH_signedTriangleAreaAmong 
    (position2D, _vertex2Ds[0], _vertex2Ds[1]);
  double sign1 = WH_signedTriangleAreaAmong 
    (position2D, _vertex2Ds[1], _vertex2Ds[2]);
  double sign2 = WH_signedTriangleAreaAmong 
    (position2D, _vertex2Ds[2], _vertex2Ds[0]);
  if ((WH_le2 (0, sign0) && WH_le2 (0, sign1) && WH_le2 (0, sign2))
      || (WH_le2 (sign0, 0) && WH_le2 (sign1, 0) && WH_le2 (sign2, 0))) {
    WH_CVR_LINE;
    result = true;
  }
  
  return result;
}



/* class WH_InOutChecker3D */

WH_InOutChecker3D
::WH_InOutChecker3D 
(double faceSize)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (0, faceSize));

  WH_CVR_LINE;

  _isSetUp = false;
  _faceSize = faceSize;
  _minRange = WH_Vector3D (0, 0, 0);
  _maxRange = WH_Vector3D (0, 0, 0);
  _triangleBucket = nullptr;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}
 
WH_InOutChecker3D
::~WH_InOutChecker3D ()
{
  WH_CVR_LINE;

  // _triangleBucket automatically destroyed by unique_ptr
  WH_T_Delete (_triangle_s);
}

bool WH_InOutChecker3D
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(WH_lt (0, _faceSize));

  if (_isSetUp) {
    WH_CVR_LINE;
    WH_ASSERT(WH_lt (this->minRange (), this->maxRange ()));
    WH_ASSERT(3 < this->triangle_s ().size ());
    // make_unique cannot return nullptr - throws std::bad_alloc on failure
  } else {
    WH_CVR_LINE;
    WH_ASSERT(_triangleBucket == nullptr);
  }

  return true;
}

bool WH_InOutChecker3D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_ASSERT(_isSetUp);

  WH_T_AssureInvariant (_triangle_s);

  return true;
}
  
void WH_InOutChecker3D
::addFace 
(const WH_Vector3D& point0,
 const WH_Vector3D& point1,
 const WH_Vector3D& point2,
 const WH_Vector3D& normal)
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(WH_ne (point0, point1));
  WH_ASSERT(WH_ne (point0, point2));
  WH_ASSERT(WH_ne (point1, point2));
  WH_ASSERT(WH_ne (normal, WH_Vector3D::zero ()));
  WH_ASSERT(WH_isParallel (normal, WH_vectorProduct 
			   (point1 - point0, point2 - point0)));
  
  WH_CVR_LINE;

  WH_Vector3D n = normal.normalize ();
  WH_Vector3D n0 
    = WH_Triangle3D (point0, point1, point2).plane ().normal ();

  WH_Triangle3D_IOC3D* tri = WH_NULL;
  if (WH_eq (n0, n)) {
    WH_CVR_LINE;
    tri = new WH_Triangle3D_IOC3D (point0, point1, point2);
    WH_ASSERT(tri != WH_NULL);
  } else {
    WH_CVR_LINE;
    WH_ASSERT(WH_isReverse (n0, n));
    tri = new WH_Triangle3D_IOC3D (point0, point2, point1);
    WH_ASSERT(tri != WH_NULL);
  }
  WH_ASSERT(tri != WH_NULL);
  WH_ASSERT(WH_eq (tri->plane ().normal (), n));

  _triangle_s.push_back (tri);
}

void WH_InOutChecker3D
::setUp ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(3 < this->triangle_s ().size ());

  WH_CVR_LINE;

  /* set <_minRange> and <_maxRange> */
  _minRange = WH_Vector3D::hugeValue ();
  _maxRange = -WH_Vector3D::hugeValue ();
  for (vector<WH_Triangle3D_IOC3D*>::iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_Triangle3D_IOC3D* tri_i = (*i_tri);

    _minRange = WH_min (tri_i->minRange (), _minRange);
    _maxRange = WH_max (tri_i->maxRange (), _maxRange);
  }
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  /* create 3-D bucket, refine X-Y plane only */

  WH_Vector3D size = _maxRange - _minRange;
  WH_Vector3D extendedMinRange = _minRange - size / 11;
  WH_Vector3D extendedMaxRange = _maxRange + size / 13;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 11, 13 */
  WH_Vector3D extendedSize = extendedMaxRange - extendedMinRange;

  int xCells = (int)ceil (extendedSize.x / _faceSize + WH::eps);
  if (xCells / 2 == 0) xCells++;
  int yCells = (int)ceil (extendedSize.y / _faceSize + WH::eps);
  if (yCells / 2 == 0) yCells++;

#ifdef TWO_DIRECTION_SEARCH
  _triangleBucket = make_unique<WH_Bucket3D<WH_Triangle3D_IOC3D>>(
    extendedMinRange, extendedMaxRange, 
    xCells, yCells, 2);
#else
  _triangleBucket = make_unique<WH_Bucket3D<WH_Triangle3D_IOC3D>>(
    extendedMinRange, extendedMaxRange, 
    xCells, yCells, 1);
#endif
  // make_unique cannot return nullptr - throws std::bad_alloc on failure

  /* register <_triangle_s> into <_triangleBucket> */
  for (vector<WH_Triangle3D_IOC3D*>::iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_Triangle3D_IOC3D* tri_i = (*i_tri);

    _triangleBucket->addItemLastWithin 
      (tri_i->minRange (), tri_i->maxRange (), tri_i);
  }

  _isSetUp = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(_isSetUp);
#endif
}
  
double WH_InOutChecker3D
::faceSize () const
{
  return _faceSize;
}

WH_Vector3D WH_InOutChecker3D
::minRange () const
{
  return _minRange;
}

WH_Vector3D WH_InOutChecker3D
::maxRange () const
{
  return _maxRange;
}

const vector<WH_Triangle3D_IOC3D*>& WH_InOutChecker3D
::triangle_s () const
{
  return _triangle_s;
}

WH_InOutChecker3D::ContainmentType WH_InOutChecker3D
::checkContainmentPlusSideAt 
(const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  ContainmentType result = OUT;

  WH_Vector3D pointMinRange (position.x, position.y, _minRange.z);
  WH_Vector3D pointMaxRange (position.x, position.y, _maxRange.z);
  WH_Line3D line (pointMinRange, pointMaxRange);
  WH_ASSERT(line.contains (position));

  bool intersectionPointIsFound = false;
  double minZWhichIsGreaterThanPositionZ = WH::HUGE_VALUE;
  bool zNormalIsPlus = false;

  vector<WH_Triangle3D_IOC3D*> triangle_s;
  _triangleBucket->getItemsOn (position, 
			       triangle_s);
  for (vector<WH_Triangle3D_IOC3D*>::const_iterator 
	 i_tri = triangle_s.begin ();
       i_tri != triangle_s.end ();
       i_tri++) {
    WH_Triangle3D_IOC3D* tri_i = (*i_tri);
    
    WH_Plane3D plane = tri_i->plane ();
    if (plane.contains (position)) {
      WH_CVR_LINE;
      if (tri_i->containsPointWhichIsOnPlane (position)) {
	WH_CVR_LINE;
	result = ON;
	break;
      }
    }

    double a = plane.a ();
    double b = plane.b ();
    double c = plane.c ();
    double d = plane.d ();

    if (WH_eq (c, 0)) {
      WH_CVR_LINE;
      /* <tri_i> Is parallel to <line> */
      continue;
    }

    /* from here, <tri_i> should not be parallel to <line> */
    WH_ASSERT(WH_ne (c, 0));

    /* get <intersectionPoint> between <plane> and <line> */
    /* a * x + b * y + c * z + d == 0 */
    double intersectionPointZ 
      = -(a * position.x + b * position.y + d) / c;
    WH_Vector3D intersectionPoint 
      (position.x, position.y, intersectionPointZ);

    if (!tri_i->containsPointWhichIsOnPlane (intersectionPoint)) {
      WH_CVR_LINE;
      continue;
    }

    WH_ASSERT(WH_ne (position.z, intersectionPoint.z));

    WH_ASSERT(WH_lt (position.z, 
		     minZWhichIsGreaterThanPositionZ));
    
    /* if <intersectionPoint> is on the plus side of <position> */
    if (WH_lt (position.z, intersectionPoint.z)) {
      WH_CVR_LINE;
      
      if (WH_eq (intersectionPoint.z, 
		 minZWhichIsGreaterThanPositionZ)) {
	WH_CVR_LINE;
	WH_ASSERT(intersectionPointIsFound);
	if (WH_lt (0, c)) {
	  WH_CVR_LINE;
	  zNormalIsPlus = true;
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(WH_lt (c, 0));
	  /* nothing */
	  /* ignore another minus Z direction normal */
	}
      } else if (WH_lt (intersectionPoint.z, 
			minZWhichIsGreaterThanPositionZ)) {
	WH_CVR_LINE;
	/* update <minZWhichIsGreaterThanPositionZ> and
	   <intersectionPointIsFound>, and clear <zNormalIsPlus> */
	
	intersectionPointIsFound = true;
	minZWhichIsGreaterThanPositionZ = intersectionPoint.z;
	if (WH_lt (0, c)) {
	  WH_CVR_LINE;
	  zNormalIsPlus = true;
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(WH_lt (c, 0));
	  zNormalIsPlus = false;
	}
      }
    }

  }    

  if (result != ON) {
    WH_CVR_LINE;
    if (intersectionPointIsFound) {
      WH_CVR_LINE;
      WH_ASSERT(WH_lt (position.z, 
		       minZWhichIsGreaterThanPositionZ));
      if (zNormalIsPlus) {
	WH_CVR_LINE;
	result = IN;
      } else {
	WH_CVR_LINE;
	result = OUT;
      }
    } else {
      WH_CVR_LINE;
      result = OUT;
    }
  }
  
  return result;
}

WH_InOutChecker3D::ContainmentType WH_InOutChecker3D
::checkContainmentMinusSideAt 
(const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  ContainmentType result = OUT;

  WH_Vector3D pointMinRange (position.x, position.y, _minRange.z);
  WH_Vector3D pointMaxRange (position.x, position.y, _maxRange.z);
  WH_Line3D line (pointMinRange, pointMaxRange);
  WH_ASSERT(line.contains (position));

  bool intersectionPointIsFound = false;
  double maxZWhichIsLesserThanPositionZ = -WH::HUGE_VALUE;
  bool zNormalIsPlus = false;

  vector<WH_Triangle3D_IOC3D*> triangle_s;
  _triangleBucket->getItemsOn (position, 
			       triangle_s);
  for (vector<WH_Triangle3D_IOC3D*>::const_iterator 
	 i_tri = triangle_s.begin ();
       i_tri != triangle_s.end ();
       i_tri++) {
    WH_Triangle3D_IOC3D* tri_i = (*i_tri);
    
    WH_Plane3D plane = tri_i->plane ();
    if (plane.contains (position)) {
      WH_CVR_LINE;
      if (tri_i->containsPointWhichIsOnPlane (position)) {
	WH_CVR_LINE;
	result = ON;
	break;
      }
    }

    double a = plane.a ();
    double b = plane.b ();
    double c = plane.c ();
    double d = plane.d ();

    if (WH_eq (c, 0)) {
      WH_CVR_LINE;
      /* <tri_i> Is parallel to <line> */
      continue;
    }

    /* from here, <tri_i> should not be parallel to <line> */
    WH_ASSERT(WH_ne (c, 0));

    /* get <intersectionPoint> between <plane> and <line> */
    /* a * x + b * y + c * z + d == 0 */
    double intersectionPointZ 
      = -(a * position.x + b * position.y + d) / c;
    WH_Vector3D intersectionPoint 
      (position.x, position.y, intersectionPointZ);

    if (!tri_i->containsPointWhichIsOnPlane (intersectionPoint)) {
      WH_CVR_LINE;
      continue;
    }
    
    WH_ASSERT(WH_ne (position.z, intersectionPoint.z));

    WH_ASSERT(WH_lt (maxZWhichIsLesserThanPositionZ,
		     position.z));
    
    /* if <intersectionPoint> is on the minus side of <position> */
    if (WH_lt (intersectionPoint.z, position.z)) {
      WH_CVR_LINE;
      
      if (WH_eq (intersectionPoint.z, 
		 maxZWhichIsLesserThanPositionZ)) {
	WH_CVR_LINE;
	WH_ASSERT(intersectionPointIsFound);
	if (WH_lt (c, 0)) {
	  WH_CVR_LINE;
	  zNormalIsPlus = false;
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(WH_lt (0, c));
	  /* nothing */
	  /* ignore another plus Z direction normal */
	}
      } else if (WH_lt (maxZWhichIsLesserThanPositionZ,
			intersectionPoint.z)) {
	WH_CVR_LINE;
	/* update <maxZWhichIsLesserThanPositionZ> and
	   <intersectionPointIsFound>, and clear <zNormalIsPlus> */
	
	intersectionPointIsFound = true;
	maxZWhichIsLesserThanPositionZ = intersectionPoint.z;
	if (WH_lt (0, c)) {
	  WH_CVR_LINE;
	  zNormalIsPlus = true;
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(WH_lt (c, 0));
	  zNormalIsPlus = false;
	}
      }
    }

  }    

  if (result != ON) {
    WH_CVR_LINE;
    if (intersectionPointIsFound) {
      WH_CVR_LINE;
      WH_ASSERT(WH_lt (maxZWhichIsLesserThanPositionZ,
		       position.z));
      if (zNormalIsPlus) {
	WH_CVR_LINE;
	result = OUT;
      } else {
	WH_CVR_LINE;
	result = IN;
      }
    } else {
      WH_CVR_LINE;                                  /* NOT COVERED */
      result = OUT;
    }
  }
  
  return result;
}

#ifdef TWO_DIRECTION_SEARCH  
/* optimized version */

WH_InOutChecker3D::ContainmentType WH_InOutChecker3D
::checkContainmentAt (const WH_Vector3D& position) const
{
  WH_CVR_LINE;

  ContainmentType result = OUT;

  if (!WH_between (position, _minRange, _maxRange)) {
    WH_CVR_LINE;
    return result;
  }

  WH_Vector3D bucketCenter = 
    (_triangleBucket->minRange () 
     + _triangleBucket->maxRange ()) / 2;
  if (WH_le (bucketCenter.z, position.z)) {
    WH_CVR_LINE;
    result = this->checkContainmentPlusSideAt (position);
  } else {
    WH_CVR_LINE;
    result = this->checkContainmentMinusSideAt (position);
  }
  return result;
}

#else  /* TWO_DIRECTION_SEARCH */
/* simple version */

WH_InOutChecker3D::ContainmentType WH_InOutChecker3D
::checkContainmentAt (const WH_Vector3D& position) const
{
  ContainmentType result = OUT;

  if (!WH_between (position, _minRange, _maxRange)) {
    return result;
  }

  WH_Vector3D pointMinRange (position.x, position.y, _minRange.z);
  WH_Vector3D pointMaxRange (position.x, position.y, _maxRange.z);
  WH_Line3D line (pointMinRange, pointMaxRange);
  WH_ASSERT(line.contains (position));

  bool intersectionPointIsFound = false;
  double minZWhichIsGreaterThanPositionZ = WH::HUGE_VALUE;
  bool zNormalIsPlus = false;

  vector<WH_Triangle3D_IOC3D*> triangle_s;
  _triangleBucket->getItemsOn (position, 
			       triangle_s);
  for (vector<WH_Triangle3D_IOC3D*>::const_iterator 
	 i_tri = triangle_s.begin ();
       i_tri != triangle_s.end ();
       i_tri++) {
    WH_Triangle3D_IOC3D* tri_i = (*i_tri);
    
    WH_Plane3D plane = tri_i->plane ();
    if (plane.contains (position)) {
      if (tri_i->containsPointWhichIsOnPlane (position)) {
	result = ON;
	break;
      }
    }

    double a = plane.a ();
    double b = plane.b ();
    double c = plane.c ();
    double d = plane.d ();

    if (WH_eq (c, 0)) {
      /* <tri_i> Is parallel to <line> */
      continue;
    }

    /* from here, <tri_i> should not be parallel to <line> */
    WH_ASSERT(WH_ne (c, 0));

    /* get <intersectionPoint> between <plane> and <line> */
    /* a * x + b * y + c * z + d == 0 */
    double intersectionPointZ 
      = -(a * position.x + b * position.y + d) / c;
    WH_Vector3D intersectionPoint 
      (position.x, position.y, intersectionPointZ);

    if (!tri_i->containsPointWhichIsOnPlane (intersectionPoint)) {
      continue;
    }
    
    WH_ASSERT(WH_ne (position.z, intersectionPoint.z));

    WH_ASSERT(WH_lt (position.z, 
		     minZWhichIsGreaterThanPositionZ));

    /* if <intersectionPoint> is on the plus side of <position> */
    if (WH_lt (position.z, intersectionPoint.z)) {

      if (WH_eq (intersectionPoint.z, 
		 minZWhichIsGreaterThanPositionZ)) {
	WH_ASSERT(intersectionPointIsFound);
	if (WH_lt (0, c)) {
	  zNormalIsPlus = true;
	} else {
	  WH_ASSERT(WH_lt (c, 0));
	  /* nothing */
	  /* ignore another minus Z direction normal */
	}
      } else if (WH_lt (intersectionPoint.z, 
			minZWhichIsGreaterThanPositionZ)) {
	/* update <minZWhichIsGreaterThanPositionZ> and
           <intersectionPointIsFound>, and clear <zNormalIsPlus> */

	intersectionPointIsFound = true;
	minZWhichIsGreaterThanPositionZ = intersectionPoint.z;
	if (WH_lt (0, c)) {
	  zNormalIsPlus = true;
	} else {
	  WH_ASSERT(WH_lt (c, 0));
	  zNormalIsPlus = false;
	}
      }
    }

  }    

  if (result != ON) {
    if (intersectionPointIsFound) {
      WH_ASSERT(WH_lt (position.z, 
		       minZWhichIsGreaterThanPositionZ));
      if (zNormalIsPlus) {
	result = IN;
      } else {
	result = OUT;
      }
    } else {
      result = OUT;
    }
  }

  return result;
}

#endif  /* TWO_DIRECTION_SEARCH */



#else

/* basic version using WH_Triangle3D class */



class WH_InOutChecker3D {
 public:
  WH_InOutChecker3D 
    (double faceSize);
  virtual ~WH_InOutChecker3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addFace 
    (const WH_Vector3D& point0,
     const WH_Vector3D& point1,
     const WH_Vector3D& point2,
     const WH_Vector3D& normal);

  virtual void setUp ();
  
  double faceSize () const;

  WH_Vector3D minRange () const;
  WH_Vector3D maxRange () const;

  const vector<WH_Triangle3D*>& triangle_s () const;

  enum ContainmentType {
    IN, OUT, ON
  };
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector3D& position) const;
  
  /* derived */
  
 protected:
  bool _isSetUp;

  double _faceSize;

  WH_Vector3D _minRange;
  WH_Vector3D _maxRange;

  vector<WH_Triangle3D*> _triangle_s;  /* OWN */

  unique_ptr<WH_Bucket3D<WH_Triangle3D>> _triangleBucket;  /* OWN */
  
  /* base */

  /* derived */

};



/* class WH_InOutChecker3D */

WH_InOutChecker3D
::WH_InOutChecker3D 
(double faceSize)
{
  /* PRE-CONDITION */
  WH_ASSERT(WH_lt (0, faceSize));

  _isSetUp = false;
  _faceSize = faceSize;
  _minRange = WH_Vector3D (0, 0, 0);
  _maxRange = WH_Vector3D (0, 0, 0);
  _triangleBucket = nullptr;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
#endif
}
 
WH_InOutChecker3D
::~WH_InOutChecker3D ()
{
  // _triangleBucket automatically destroyed by unique_ptr
  WH_T_Delete (_triangle_s);
}

bool WH_InOutChecker3D
::checkInvariant () const
{
  WH_ASSERT(WH_lt (0, _faceSize));

  if (_isSetUp) {
    WH_ASSERT(lt (this->minRange (), this->maxRange ()));
    WH_ASSERT(3 < this->triangle_s ().size ());
    // make_unique cannot return nullptr - throws std::bad_alloc on failure
  } else {
    WH_ASSERT(_triangleBucket == nullptr);
  }

  return true;
}

bool WH_InOutChecker3D
::assureInvariant () const
{
  this->checkInvariant ();

  WH_ASSERT(_isSetUp);

  return true;
}
  
void WH_InOutChecker3D
::addFace 
(const WH_Vector3D& point0,
 const WH_Vector3D& point1,
 const WH_Vector3D& point2,
 const WH_Vector3D& normal)
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(WH_ne (point0, point1));
  WH_ASSERT(WH_ne (point0, point2));
  WH_ASSERT(WH_ne (point1, point2));
  WH_ASSERT(WH_ne (normal, WH_Vector3D::ZERO));
  WH_ASSERT(WH_isParallel (normal, vectorProduct 
			   (point1 - point0, point2 - point0)));
  
  WH_Vector3D n = normal.normalize ();
  WH_Triangle3D tri0 (point0, point1, point2);
  WH_Vector3D n0 = tri0.plane ().normal ();

  WH_Triangle3D* tri = WH_NULL;
  if (WH_eq (n0, n)) {
    tri = new WH_Triangle3D (point0, point1, point2);
    WH_ASSERT(tri != WH_NULL);
  } else {
    WH_ASSERT(WH_isReverse (n0, n));
    tri = new WH_Triangle3D (point0, point2, point1);
    WH_ASSERT(tri != WH_NULL);
  }
  WH_ASSERT(tri != WH_NULL);
  WH_ASSERT(WH_eq (tri->plane ().normal (), n));

  _triangle_s.push_back (tri);
}

void WH_InOutChecker3D
::setUp ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!_isSetUp);
  WH_ASSERT(3 < this->triangle_s ().size ());

  /* set <_minRange> and <_maxRange> */
  _minRange = WH_Vector3D::HUGE_VALUE;
  _maxRange = -WH_Vector3D::HUGE_VALUE;
  for (vector<WH_Triangle3D*>::iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_Triangle3D* tri_i = (*i_tri);

    _minRange = WH_min (tri_i->minRange (), _minRange);
    _maxRange = WH_max (tri_i->maxRange (), _maxRange);
  }
  WH_ASSERT(WH_lt (_minRange, _maxRange));

  /* create 3-D bucket, refine X-Y plane only */

  WH_Vector3D size = _maxRange - _minRange;
  WH_Vector3D extendedMinRange = _minRange - size / 11;
  WH_Vector3D extendedMaxRange = _maxRange + size / 13;
  /* NEED TO REDEFINE */
  /* MAGIC NUMBER : 11, 13 */
  WH_Vector3D extendedSize = extendedMaxRange - extendedMinRange;

  int xCells = (int)ceil (extendedSize.x / _faceSize + WH::eps);
  if (xCells / 2 == 0) xCells++;
  int yCells = (int)ceil (extendedSize.y / _faceSize + WH::eps);
  if (yCells / 2 == 0) yCells++;

  _triangleBucket = make_unique<WH_Bucket3D<WH_Triangle3D>>(
    extendedMinRange, extendedMaxRange, 
    xCells, yCells, 1);
  // make_unique cannot return nullptr - throws std::bad_alloc on failure

  /* register <_triangle_s> into <_triangleBucket> */
  for (vector<WH_Triangle3D*>::iterator 
	 i_tri = _triangle_s.begin ();
       i_tri != _triangle_s.end ();
       i_tri++) {
    WH_Triangle3D* tri_i = (*i_tri);

    _triangleBucket->addItemLastWithin 
      (tri_i->minRange (), tri_i->maxRange (), tri_i);
  }

  _isSetUp = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->checkInvariant ());
  WH_ASSERT(_isSetUp);
#endif
}
  
double WH_InOutChecker3D
::faceSize () const
{
  return _faceSize;
}

WH_Vector3D WH_InOutChecker3D
::minRange () const
{
  return _minRange;
}

WH_Vector3D WH_InOutChecker3D
::maxRange () const
{
  return _maxRange;
}

const vector<WH_Triangle3D*>& WH_InOutChecker3D
::triangle_s () const
{
  return _triangle_s;
}

WH_InOutChecker3D::ContainmentType WH_InOutChecker3D
::checkContainmentAt (const WH_Vector3D& position) const
{
  ContainmentType result = OUT;

  if (!WH_between (position, _minRange, _maxRange)) {
    return result;
  }

  WH_Vector3D pointMinRange (position.x, position.y, _minRange.z);
  WH_Vector3D pointMaxRange (position.x, position.y, _maxRange.z);
  WH_Line3D line (pointMinRange, pointMaxRange);
  WH_ASSERT(line.contains (position));

  bool plusSideIntersectionPointIsFound = false;
  bool zNormalIsPlus = false;
  double minZWhichIsGreaterThanPositionZ = WH::HUGE_VALUE;

  vector<WH_Triangle3D*> triangle_s;
  _triangleBucket->getItemsWithin
    (pointMinRange, pointMaxRange, 
     triangle_s);
  for (vector<WH_Triangle3D*>::const_iterator 
	 i_tri = triangle_s.begin ();
       i_tri != triangle_s.end ();
       i_tri++) {
    WH_Triangle3D* tri_i = (*i_tri);
    
    WH_Vector3D point0;
    WH_Vector3D point1;
    bool flag = tri_i->hasIntersectionWith 
      (line,
       point0, point1);
    if (!flag) continue;
    
    WH_ASSERT(WH_eq (point0.x, point1.x));
    WH_ASSERT(WH_eq (point0.y, point1.y));
    WH_ASSERT(WH_eq (point0.x, position.x));
    WH_ASSERT(WH_eq (point0.y, position.y));
    
    if (WH_between (position.z, point0.z, point1.z)) {
      /* <position> is on boundary */
      WH_ASSERT(WH_eq (position.z, point0.z)
	      || WH_eq (position.z, point1.z)
	      || WH_justBetween (position.z, point0.z, point1.z));
      result = ON;
      break;
    }
    
    WH_Vector3D normal = tri_i->plane ().normal ();
    if (WH_eq (normal.z, 0)) {
      /* <tri_i> is parallel to <line> */
      continue;
    }
    
    /* from here, <tri_i> should not be parallel to <line> */
    WH_ASSERT(WH_ne (normal.z, 0));
    
    /* from here, there should be only one intersection point.  the
       intersection point should be different from <position> */
    WH_ASSERT(WH_eq (point0, point1));
    WH_Vector3D intersectionPoint = point0;
    WH_ASSERT(WH_ne (position, intersectionPoint));
    WH_ASSERT(WH_eq (position.x, intersectionPoint.x));
    WH_ASSERT(WH_eq (position.y, intersectionPoint.y));
    WH_ASSERT(WH_ne (position.z, intersectionPoint.z));

    WH_ASSERT(WH_lt (position.z, 
		     minZWhichIsGreaterThanPositionZ));

    /* if <intersectionPoint> is on the plus side of <position> */
    if (WH_lt (position.z, intersectionPoint.z)) {

      if (WH_eq (intersectionPoint.z, 
		 minZWhichIsGreaterThanPositionZ)) {
	WH_ASSERT(plusSideIntersectionPointIsFound);
	if (WH_lt (0, normal.z)) {
	  zNormalIsPlus = true;
	} else {
	  WH_ASSERT(WH_lt (normal.z, 0));
	  /* nothing */
	  /* ignore another minus Z direction normal */
	}
      } else if (WH_lt (intersectionPoint.z, 
			minZWhichIsGreaterThanPositionZ)) {
	/* update <minZWhichIsGreaterThanPositionZ> and
           <plusSideIntersectionPointIsFound>, and clear
           <zNormalIsPlus> */

	plusSideIntersectionPointIsFound = true;
	minZWhichIsGreaterThanPositionZ = intersectionPoint.z;
	if (WH_lt (0, normal.z)) {
	  zNormalIsPlus = true;
	} else {
	  WH_ASSERT(WH_lt (normal.z, 0));
	  zNormalIsPlus = false;
	}
      }
    }
  }    

  if (result != ON) {
    if (plusSideIntersectionPointIsFound) {
      WH_ASSERT(WH_lt (position.z, 
		       minZWhichIsGreaterThanPositionZ));
      if (zNormalIsPlus) {
	result = IN;
      } else {
	result = OUT;
      }
    } else {
      result = OUT;
    }
  }

  return result;
}



#endif



/* test coverage completed */
