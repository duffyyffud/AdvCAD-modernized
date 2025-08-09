/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for polyhedron3d.cc */

#ifndef WH_HEADER_WH_POLYHEDRON3D
#define WH_HEADER_WH_POLYHEDRON3D

#ifndef WH_INCLUDED_WH_POLYGON3D
#include <WH/polygon3d.h>
#define WH_INCLUDED_WH_POLYGON3D
#endif



class WH_Polyhedron3D_A;
class WH_Polyhedron3D;



class WH_Polyhedron3D_A {
 public:
  WH_Polyhedron3D_A ();
  virtual ~WH_Polyhedron3D_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* core abstract operations */
  virtual int nVertexs () const = 0;
  virtual WH_Vector3D vertex (int iVertex) const = 0;
  virtual int nEdges () const = 0;
  virtual void getVertexIndexsOfEdge 
    (int iEdge,
     int& iVertex0_OUT, int& iVertex1_OUT) const = 0;
  virtual int nFaces () const = 0;
  virtual int nVertexsOfFace (int iFace) const = 0;
  virtual int vertexIndexOfFace 
    (int iFace, int iVertexOfFace) const = 0;
  virtual int edgeIndexOfFace 
    (int iFace, int iEdgeOfFace) const = 0;

  virtual WH_Segment3D edge (int iEdge) const;
  virtual int nEdgesOfFace (int iFace) const;
  virtual int nextVertexIndexOfFace 
    (int iFace, int iVertexOfFace) const;
  virtual int prevVertexIndexOfFace 
    (int iFace, int iVertexOfFace) const;
  virtual int nextEdgeIndexOfFace 
    (int iFace, int iEdgeOfFace) const;
  virtual int prevEdgeIndexOfFace 
    (int iFace, int iEdgeOfFace) const;
  virtual WH_Vector3D vertexOfFace 
    (int iFace, int iVertexOfFace) const;
  virtual WH_Segment3D edgeOfFace 
    (int iFace, int iEdgeOfFace) const;
  virtual WH_Polygon3D face (int iFace) const;

  virtual bool hasAnyDuplicatedVertex () const;
  virtual bool hasAnyCrossingEdge () const;
  virtual bool hasAnyIllegularFace () const;
  virtual bool isRegular () const;
  virtual double surfaceArea () const;
  virtual double volume () const;
  virtual WH_Vector3D centerOfGravity () const;
  virtual WH_Vector3D minRange () const;
  virtual WH_Vector3D maxRange () const;
  virtual bool hasVertexAt (const WH_Vector3D& position) const;
  virtual bool hasVertexAtEveryPointIn
    (const vector<WH_Vector3D>& point_s) const;
  enum ContainmentType {
    IN, OUT, ON
  };
  virtual ContainmentType 
    checkContainmentAt (const WH_Vector3D& position) const;

  /* derived */

 protected:
  
  /* base */
  virtual bool 
    containsOnBoundary (const WH_Vector3D& position) const;
  virtual bool 
    containsInside (const WH_Vector3D& position) const;

  /* derived */

};



#endif /* WH_HEADER_WH_POLYHEDRON3D */
