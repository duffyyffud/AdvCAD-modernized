/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_setop.cc */

#ifndef WH_HEADER_WH_GM3D_SETOP
#define WH_HEADER_WH_GM3D_SETOP

#ifndef WH_INCLUDED_WH_GM3D_FACET
#include <WH/gm3d_facet.h>
#define WH_INCLUDED_WH_GM3D_FACET
#endif



class WH_GM3D_SetOperator;



class WH_GM3D_SetOperator {
 public:
  enum OperationType {
    UNION,
    INTERSECTION,
    SUBTRACTION
    /* <body0> - <body1> */
  };
  WH_GM3D_SetOperator 
    (OperationType operationType,
     WH_GM3D_FacetBody* body0,
     WH_GM3D_FacetBody* body1,
     WH_GM3D_FacetBody* resultBody);
  virtual ~WH_GM3D_SetOperator ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  OperationType operationType () const;

  WH_GM3D_FacetBody* body0 () const;
  WH_GM3D_FacetBody* body1 () const;

  WH_GM3D_FacetBody* resultBody () const;

  /* derived */
  
 protected:
  OperationType _operationType;

  WH_GM3D_FacetBody* _body0;  /* not own */
  WH_GM3D_FacetBody* _body1;  /* not own */

  WH_GM3D_FacetBody* _resultBody;  /* not own */

  /* base */
  virtual void divideBodyByBody 
    (WH_GM3D_FacetBody* bodyFrom, 
     WH_GM3D_FacetBody* bodyBy,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT  /* CREATE */);
  
  virtual void classifyFacets 
    (const vector<WH_GM3D_TriangleFacet*>& facet_s, 
     WH_GM3D_FacetBody* bodyBy,
     vector<WH_GM3D_TriangleFacet*>& inFacet_s_OUT,
     vector<WH_GM3D_TriangleFacet*>& onFacet_s_OUT,
     vector<WH_GM3D_TriangleFacet*>& outFacet_s_OUT);

  virtual void classifyOnFacets 
    (const vector<WH_GM3D_TriangleFacet*>& facet_s, 
     WH_GM3D_FacetBody* bodyBy,
     vector<WH_GM3D_TriangleFacet*>& onInFacet_s_OUT,
     vector<WH_GM3D_TriangleFacet*>& onOutFacet_s_OUT);
  
  virtual void collectFacets 
    (const vector<WH_GM3D_TriangleFacet*>& facet_s);

  /* derived */
  
};



#endif /* WH_HEADER_WH_GM3D_SETOP */
