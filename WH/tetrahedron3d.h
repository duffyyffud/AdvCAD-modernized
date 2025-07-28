/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for tetrahedron3d.cc */

#ifndef WH_HEADER_WH_TETRAHEDRON3D
#define WH_HEADER_WH_TETRAHEDRON3D

#ifndef WH_INCLUDED_WH_POLYHEDRON3D
#include <WH/polyhedron3d.h>
#define WH_INCLUDED_WH_POLYHEDRON3D
#endif

#ifndef WH_INCLUDED_WH_TRIANGLE3D
#include <WH/triangle3d.h>
#define WH_INCLUDED_WH_TRIANGLE3D
#endif



class WH_Tetrahedron3D_A;
class WH_Tetrahedron3D;



class WH_Tetrahedron3D_A : public WH_Polyhedron3D_A {
 public:
  static const int nTetrahedronVertexs;
  static const int nTetrahedronEdges;
  static const int nTetrahedronFaces;
  static const int edgeVertexMap[6][2];
  static const int faceVertexMap[4][3];
  static const int faceEdgeMap[4][3];

  WH_Tetrahedron3D_A ();
  virtual ~WH_Tetrahedron3D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual WH_Triangle3D faceTriangle (int iFace) const;
  virtual double signedVolume () const;
  virtual bool isClockWise () const;
  virtual void getCircumcenter 
    (WH_Vector3D& center_OUT, double& radius_OUT) const;

  /* derived */

  /* core abstract operations */
  virtual int nVertexs () const;
  virtual WH_Vector3D vertex (int iVertex) const = 0;
  virtual int nEdges () const;
  virtual void getVertexIndexsOfEdge 
    (int iEdge,
     int& iVertex0_OUT, int& iVertex1_OUT) const;
  virtual int nFaces () const;
  virtual int nVertexsOfFace (int iFace) const;
  virtual int nEdgesOfFace (int iFace) const;
  virtual int vertexIndexOfFace 
    (int iFace, int iVertexOfFace) const;
  virtual int edgeIndexOfFace 
    (int iFace, int iEdgeOfFace) const;

  virtual WH_Polygon3D face (int iFace) const;
  virtual double surfaceArea () const;
  virtual double volume () const;
  virtual WH_Vector3D centerOfGravity () const;
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector3D& position) const;

 protected:
  
  /* base */

  /* derived */

};



/* value-based class */
class WH_Tetrahedron3D : public WH_Tetrahedron3D_A {
 public:
  WH_Tetrahedron3D ();
  WH_Tetrahedron3D 
    (const WH_Vector3D& v0, 
     const WH_Vector3D& v1,
     const WH_Vector3D& v2,
     const WH_Vector3D& v3);
  virtual ~WH_Tetrahedron3D ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  friend ostream& operator<< 
    (ostream& out, const WH_Tetrahedron3D& tetrahedron);

  /* base */

  /* derived */

  /* core abstract operations */
  virtual WH_Vector3D vertex (int iVertex) const;

 protected:
  WH_Vector3D _vertexs[4];

  /* base */

  /* derived */

};



#endif /* WH_HEADER_WH_TETRAHEDRON3D */
