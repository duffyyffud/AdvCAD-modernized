/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_sheetsetop.cc */

#ifndef WH_HEADER_WH_GM3D_SHEETSETOP
#define WH_HEADER_WH_GM3D_SHEETSETOP

#ifndef WH_INCLUDED_WH_GM3D_FACET
#include <WH/gm3d_facet.h>
#define WH_INCLUDED_WH_GM3D_FACET
#endif

class WH_GM2D_FacetBody;



class WH_GM3D_SheetSetOperator;



class WH_GM3D_SheetSetOperator {
 public:
  enum OperationType {
    UNION,
    INTERSECTION,
    SUBTRACTION
    /* <body0> - <body1> */
  };
  WH_GM3D_SheetSetOperator 
    (OperationType operationType,
     WH_GM3D_FacetBody* body0,
     WH_GM3D_FacetBody* body1,
     WH_GM3D_FacetBody* resultBody);
  virtual ~WH_GM3D_SheetSetOperator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  OperationType operationType () const;

  WH_GM3D_FacetBody* body0 () const;
  WH_GM3D_FacetBody* body1 () const;

  WH_GM3D_FacetBody* resultBody () const;

  virtual bool canPerform () const;

  /* derived */
  
 protected:
  OperationType _operationType;

  WH_GM3D_FacetBody* _body0;  /* not own */
  WH_GM3D_FacetBody* _body1;  /* not own */

  WH_GM3D_FacetBody* _resultBody;  /* not own */

  /* base */
  virtual WH_GM2D_FacetBody* createParameterFacetBody 
    (WH_GM3D_FacetBody* body,
     const WH_Plane3D& basePlane);
  
  virtual void makeResultBody 
    (WH_GM2D_FacetBody* body,
     const WH_Plane3D& basePlane);

  /* derived */
  
};



#endif /* WH_HEADER_WH_GM3D_SHEETSETOP */
