/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for inout3d.cc */

#ifndef WH_HEADER_WH_INOUT3D
#define WH_HEADER_WH_INOUT3D

#ifndef WH_INCLUDED_WH_TRIANGLE3D
#include <WH/triangle3d.h>
#define WH_INCLUDED_WH_TRIANGLE3D
#endif

template <class Type> class WH_Bucket3D;



class WH_Triangle3D_IOC3D;
class WH_InOutChecker3D;



/* value-based class */
class WH_Triangle3D_IOC3D : public WH_Triangle3D {
 public:
  WH_Triangle3D_IOC3D 
    (const WH_Vector3D& v0, 
     const WH_Vector3D& v1,
     const WH_Vector3D& v2);
  virtual ~WH_Triangle3D_IOC3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual bool containsPointWhichIsOnPlane 
    (const WH_Vector3D& position) const;

  /* derived */

 protected:
  WH_Vector2D _vertex2Ds[3];

  /* base */

  /* derived */

};

/* value-based class */
/* heavy weight */
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

  const vector<WH_Triangle3D_IOC3D*>& triangle_s () const;

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

  vector<WH_Triangle3D_IOC3D*> _triangle_s;  /* OWN */

  WH_Bucket3D<WH_Triangle3D_IOC3D>* _triangleBucket;  /* OWN */
  
  /* base */
  virtual ContainmentType checkContainmentPlusSideAt 
    (const WH_Vector3D& position) const;

  virtual ContainmentType checkContainmentMinusSideAt 
    (const WH_Vector3D& position) const;

  /* derived */

};



#endif /* WH_HEADER_WH_INOUT3D */


