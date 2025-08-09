/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for field.cc */

#ifndef WH_HEADER_WH_FIELD
#define WH_HEADER_WH_FIELD

#ifndef WH_INCLUDED_WH_SCALAR
#include <WH/scalar.h>
#define WH_INCLUDED_WH_SCALAR
#endif



class WH_Field_A;
class WH_UssField;
class WH_NussField;



class WH_Field_A {
 public:
  WH_Field_A 
    (double minRange, double maxRange,
     int xCells);
  virtual ~WH_Field_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  double minRange () const;
  double maxRange () const;
  int xGrids () const;
  int xCells () const;
  void getGridsIn 
    (int cx, 
     int gxs_OUT[2]) const;
  bool isOutOfRangeAt (int gx) const;
  bool isOutOfRangeIn (int cx) const;
  int gridIndexAt (int gx) const;
  int cellIndexIn (int cx) const;
  void getGridIndexsIn 
    (int cx, 
     int gridIndexs_OUT[2]) const;

  virtual double cellSize (int cx) const = 0;
  virtual double positionAt (int gx) const = 0;
  virtual void getPositionsIn 
    (int cx, 
     double positions_OUT[2]) const;
  virtual bool getCellOn 
    (double position,
     int& cx_OUT, 
     double& localCoord_OUT) const = 0;

  /* derived */

 protected:
  double _minRange, _maxRange;
  int _xGrids;
  int _xCells;

  /* base */

  /* derived */

};

/* u.s.s. : uniformly spatial subdivision */

/* value-based class */
class WH_UssField : public WH_Field_A {
 public:
  WH_UssField 
    (double minRange, double maxRange,
     int xCells);
  virtual ~WH_UssField ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  virtual double cellSize (int cx) const;
  virtual double positionAt (int gx) const;
  virtual bool getCellOn 
    (double position,
     int& cx_OUT, 
     double& localCoord_OUT) const;

 protected:
  double _cellSize;

  /* base */

  /* derived */

};

/* n.u.s.s. : non-uniformly spatial subdivision */

/* value-based class */
class WH_NussField : public WH_Field_A {
 public:
  WH_NussField 
    (double minRange, double maxRange,
     int xCells,
     double xCellSizes[]);
  WH_NussField (const WH_NussField& field);
  virtual ~WH_NussField ();
  const WH_NussField& operator= (const WH_NussField& field);
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  double xCellSize (int cx) const;

  /* derived */
  virtual double cellSize (int cx) const;
  virtual double positionAt (int gx) const;
  virtual bool getCellOn 
    (double position,
     int& cx_OUT, 
     double& localCoord_OUT) const;

 protected:
  double* _xCellSizes;   /* OWN */   
  /* double _xCellSizes[_xCells] */

  /* base */

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/field_inline.cc>
#endif

#endif /* WH_HEADER_WH_FIELD */




