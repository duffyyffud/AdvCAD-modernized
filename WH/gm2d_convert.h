/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm2d_convert.cc */

#ifndef WH_HEADER_WH_GM2D_CONVERT
#define WH_HEADER_WH_GM2D_CONVERT

#ifndef WH_INCLUDED_WH_GM2D_BREP
#include <WH/gm2d_brep.h>
#define WH_INCLUDED_WH_GM2D_BREP
#endif

#ifndef WH_INCLUDED_WH_GM2D_FACET
#include <WH/gm2d_facet.h>
#define WH_INCLUDED_WH_GM2D_FACET
#endif



class WH_GM2D_ConverterFromBrepToFacet;
class WH_GM2D_ConverterFromFacetToBrep;



class WH_GM2D_ConverterFromBrepToFacet {
 public:
  WH_GM2D_ConverterFromBrepToFacet 
    (WH_GM2D_Body* brepBody,
     WH_GM2D_FacetBody* facetBody);
  virtual ~WH_GM2D_ConverterFromBrepToFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM2D_Body* brepBody () const;

  WH_GM2D_FacetBody* facetBody () const;

  /* derived */
  
 protected:
  WH_GM2D_Body* _brepBody;  /* not own */
  
  WH_GM2D_FacetBody* _facetBody;  /* not own */

  /* base */

  /* derived */
  
};

class WH_GM2D_ConverterFromFacetToBrep {
 public:
  WH_GM2D_ConverterFromFacetToBrep 
    (WH_GM2D_Body* brepBody,
     WH_GM2D_FacetBody* facetBody);
  virtual ~WH_GM2D_ConverterFromFacetToBrep ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM2D_Body* brepBody () const;

  WH_GM2D_FacetBody* facetBody () const;

  /* derived */
  
 protected:
  WH_GM2D_Body* _brepBody;  /* not own */
  
  WH_GM2D_FacetBody* _facetBody;  /* not own */

  /* base */
  virtual WH_GM2D_Vertex* registerVertex 
    (const WH_Vector2D& vertexPoint);
  
  /* derived */
  
};



#endif /* WH_HEADER_WH_GM2D_CONVERT */
