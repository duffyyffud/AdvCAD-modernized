/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_convert.cc */

#pragma once
#ifndef WH_INCLUDED_WH_GM3D_BREP
#include <WH/gm3d_brep.h>
#define WH_INCLUDED_WH_GM3D_BREP
#endif

#ifndef WH_INCLUDED_WH_GM3D_FACET
#include <WH/gm3d_facet.h>
#define WH_INCLUDED_WH_GM3D_FACET
#endif

class WH_GM3D_ConverterFromBrepToFacet;
class WH_GM3D_ConverterFromFacetToBrep;

class WH_GM3D_ConverterFromBrepToFacet {
 public:
  WH_GM3D_ConverterFromBrepToFacet 
    (WH_GM3D_Body* brepBody,
     WH_GM3D_FacetBody* facetBody);
  virtual ~WH_GM3D_ConverterFromBrepToFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM3D_Body* brepBody () const;

  WH_GM3D_FacetBody* facetBody () const;

  /* derived */
  
 protected:
  WH_GM3D_Body* _brepBody;  /* not own */
  
  WH_GM3D_FacetBody* _facetBody;  /* not own */

  /* base */

  /* derived */
  
};

class WH_GM3D_ConverterFromFacetToBrep {
 public:
  WH_GM3D_ConverterFromFacetToBrep 
    (WH_GM3D_Body* brepBody,
     WH_GM3D_FacetBody* facetBody);
  virtual ~WH_GM3D_ConverterFromFacetToBrep ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM3D_Body* brepBody () const;

  WH_GM3D_FacetBody* facetBody () const;

  /* derived */
  
 protected:
  WH_GM3D_Body* _brepBody;  /* not own */
  
  WH_GM3D_FacetBody* _facetBody;  /* not own */

  /* base */
  virtual WH_GM3D_Vertex* registerVertex 
    (const WH_Vector3D& vertexPoint);
  
  virtual WH_GM3D_Edge* registerEdge 
    (WH_GM3D_Vertex* firstVertex,
     WH_GM3D_Vertex* lastVertex);

  virtual void makeLoop 
    (WH_GM3D_Loop* loop,
     const WH_Polygon3D& polygon);

  /* derived */
  
};
