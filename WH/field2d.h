/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for field2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SPACE2D
#include <WH/space2d.h>
#define WH_INCLUDED_WH_SPACE2D
#endif

class WH_Field2D_A;
class WH_UssField2D;
class WH_NussField2D;

/* order of cell & grid index */
/* counter-clock-wise direction around rectangle */
/* 0 ... (0, 0) */
/* 1 ... (1, 0) */
/* 2 ... (1, 1) */
/* 3 ... (0, 1) */

class WH_Field2D_A {
 public:
  WH_Field2D_A 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
     int xCells, int yCells);
  virtual ~WH_Field2D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  static const int edgeVertexMap[4][2];

  /* base */
  WH_Vector2D minRange () const;
  WH_Vector2D maxRange () const;
  int xGrids () const;
  int yGrids () const;
  int xCells () const;
  int yCells () const;
  void getGridsIn 
    (int cx, int cy, 
     int gxs_OUT[4], int gys_OUT[4]) const;
  bool isOutOfRangeAt (int gx, int gy) const;
  bool isOutOfRangeIn (int cx, int cy) const;
  int gridIndexAt (int gx, int gy) const;
  int cellIndexIn (int cx, int cy) const;
  void getGridIndexsIn 
    (int cx, int cy, 
     int gridIndexs_OUT[4]) const;

  virtual WH_Vector2D cellSize (int cx, int cy) const = 0;
  virtual WH_Vector2D positionAt (int gx, int gy) const = 0;
  virtual void getPositionsIn 
    (int cx, int cy, 
     WH_Vector2D positions_OUT[4]) const;
  virtual bool getCellOn 
    (const WH_Vector2D& position,
     int& cx_OUT, int& cy_OUT, 
     WH_Vector2D& localCoord_OUT) const = 0;

  /* derived */

 protected:
  WH_Vector2D _minRange, _maxRange;
  int _xGrids, _yGrids;
  int _xCells, _yCells;

  /* base */

  /* derived */

};

/* u.s.s. : uniformly spatial subdivision */

/* value-based class */
class WH_UssField2D : public WH_Field2D_A {
 public:
  WH_UssField2D 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
     int xCells, int yCells);
  virtual ~WH_UssField2D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  virtual WH_Vector2D cellSize (int cx, int cy) const;
  virtual WH_Vector2D positionAt (int gx, int gy) const;
  virtual bool getCellOn 
    (const WH_Vector2D& position,
     int& cx_OUT, int& cy_OUT, 
     WH_Vector2D& localCoord_OUT) const;

 protected:
  WH_Vector2D _cellSize;

  /* base */

  /* derived */

};

/* n.u.s.s. : non-uniformly spatial subdivision */

/* value-based class */
class WH_NussField2D : public WH_Field2D_A {
 public:
  WH_NussField2D 
    (const WH_Vector2D& minRange, const WH_Vector2D& maxRange,
     int xCells, int yCells,
     double xCellSizes[], double yCellSizes[]);
  WH_NussField2D (const WH_NussField2D& field);
  virtual ~WH_NussField2D ();
  const WH_NussField2D& operator= (const WH_NussField2D& field);
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  double xCellSize (int cx) const;
  double yCellSize (int cy) const;

  /* derived */
  virtual WH_Vector2D cellSize (int cx, int cy) const;
  virtual WH_Vector2D positionAt (int gx, int gy) const;
  virtual bool getCellOn 
    (const WH_Vector2D& position,
     int& cx_OUT, int& cy_OUT, 
     WH_Vector2D& localCoord_OUT) const;

 protected:
  double* _xCellSizes;   /* OWN */   
  /* double _xCellSizes[_xCells] */
  double* _yCellSizes;   /* OWN */   
  /* double _yCellSizes[_yCells] */

  /* base */

  /* derived */

};

#ifdef WH_INLINE_ENABLED
#include <WH/field2d_inline.cc>
#endif
