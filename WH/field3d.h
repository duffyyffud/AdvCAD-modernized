/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for field3d.cc */

#ifndef WH_HEADER_WH_FIELD3D
#define WH_HEADER_WH_FIELD3D

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif



class WH_Field3D_A;
class WH_UssField3D;
class WH_NussField3D;



/* order of cell & grid index */
/* counter-clock-wise direction around rectangle */
/* 0 ... (0, 0, 0) */
/* 1 ... (1, 0, 0) */
/* 2 ... (1, 1, 0) */
/* 3 ... (0, 1, 0) */
/* 4 ... (0, 0, 1) */
/* 5 ... (1, 0, 1) */
/* 6 ... (1, 1, 1) */
/* 7 ... (0, 1, 1) */

class WH_Field3D_A {
 public:
  WH_Field3D_A 
    (const WH_Vector3D& minRange, const WH_Vector3D& maxRange,
     int xCells, int yCells, int zCells);
  virtual ~WH_Field3D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  static const int edgeVertexMap[12][2];
  static const int faceVertexMap[6][4];
  static const int faceEdgeMap[6][4];

  /* base */
  WH_Vector3D minRange () const;
  WH_Vector3D maxRange () const;
  int xGrids () const;
  int yGrids () const;
  int zGrids () const;
  int xCells () const;
  int yCells () const;
  int zCells () const;
  void getGridsIn 
    (int cx, int cy, int cz, 
     int gxs_OUT[8], int gys_OUT[8], int gzs_OUT[8]) const;
  bool isOutOfRangeAt (int gx, int gy, int gz) const;
  bool isOutOfRangeIn (int cx, int cy, int cz) const;
  int gridIndexAt (int gx, int gy, int gz) const;
  int cellIndexIn (int cx, int cy, int cz) const;
  void getGridIndexsIn 
    (int cx, int cy, int cz, 
     int gridIndexs_OUT[8]) const;

  virtual WH_Vector3D cellSize (int cx, int cy, int cz) const = 0;
  virtual WH_Vector3D positionAt (int gx, int gy, int gz) const = 0;
  virtual void getPositionsIn 
    (int cx, int cy, int cz, 
     WH_Vector3D positions_OUT[8]) const;
  virtual bool getCellOn 
    (const WH_Vector3D& position,
     int& cx_OUT, int& cy_OUT, int& cz_OUT, 
     WH_Vector3D& localCoord_OUT) const = 0;

  /* derived */

 protected:
  WH_Vector3D _minRange, _maxRange;
  int _xGrids, _yGrids, _zGrids;
  int _xCells, _yCells, _zCells;

  /* base */

  /* derived */

};

/* u.s.s. : uniformly spatial subdivision */

/* value-based class */
class WH_UssField3D : public WH_Field3D_A {
 public:
  WH_UssField3D 
    (const WH_Vector3D& minRange, const WH_Vector3D& maxRange,
     int xCells, int yCells, int zCells);
  virtual ~WH_UssField3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  virtual WH_Vector3D cellSize (int cx, int cy, int cz) const;
  virtual WH_Vector3D positionAt (int gx, int gy, int gz) const;
  virtual bool getCellOn 
    (const WH_Vector3D& position,
     int& cx_OUT, int& cy_OUT, int& cz_OUT, 
     WH_Vector3D& localCoord_OUT) const;

 protected:
  WH_Vector3D _cellSize;

  /* base */

  /* derived */

};

/* n.u.s.s. : non-uniformly spatial subdivision */

/* value-based class */
class WH_NussField3D : public WH_Field3D_A {
 public:
  WH_NussField3D 
    (const WH_Vector3D& minRange, const WH_Vector3D& maxRange,
     int xCells, int yCells, int zCells,
     double xCellSizes[], double yCellSizes[], double zCellSizes[]);
  WH_NussField3D (const WH_NussField3D& field);
  virtual ~WH_NussField3D ();
  const WH_NussField3D& operator= (const WH_NussField3D& field);
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  double xCellSize (int cx) const;
  double yCellSize (int cy) const;
  double zCellSize (int cz) const;

  /* derived */
  virtual WH_Vector3D cellSize (int cx, int cy, int cz) const;
  virtual WH_Vector3D positionAt (int gx, int gy, int gz) const;
  virtual bool getCellOn 
    (const WH_Vector3D& position,
     int& cx_OUT, int& cy_OUT, int& cz_OUT, 
     WH_Vector3D& localCoord_OUT) const;

 protected:
  double* _xCellSizes;   /* OWN */   
  /* double _xCellSizes[_xCells] */
  double* _yCellSizes;   /* OWN */   
  /* double _yCellSizes[_yCells] */
  double* _zCellSizes;   /* OWN */   
  /* double _zCellSizes[_zCells] */

  /* base */

  /* derived */

};



#ifdef WH_INLINE_ENABLED
#include <WH/field3d_inline.cc>
#endif

#endif /* WH_HEADER_WH_FIELD3D */




