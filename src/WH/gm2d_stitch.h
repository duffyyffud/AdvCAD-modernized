/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm2d_stitch.cc */

#pragma once
#ifndef WH_INCLUDED_WH_GM2D_FACET
#include <WH/gm2d_facet.h>
#define WH_INCLUDED_WH_GM2D_FACET
#endif

class WH_GM2D_Stitcher;

class WH_GM2D_Stitcher {
 public:
  WH_GM2D_Stitcher 
    (WH_GM2D_FacetBody* body0,
     WH_GM2D_FacetBody* body1,
     WH_GM2D_FacetBody* resultBody);
  virtual ~WH_GM2D_Stitcher();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM2D_FacetBody* body0 () const;
  WH_GM2D_FacetBody* body1 () const;

  WH_GM2D_FacetBody* resultBody () const;

  /* derived */
  
 protected:
  WH_GM2D_FacetBody* _body0;  /* not own */
  WH_GM2D_FacetBody* _body1;  /* not own */

  WH_GM2D_FacetBody* _resultBody;  /* not own */

  /* base */
  virtual void divideBodyByBody 
    (WH_GM2D_FacetBody* bodyFrom, 
     WH_GM2D_FacetBody* bodyBy,
     vector<WH_GM2D_SegmentFacet*>& facet_s_OUT  /* CREATE */);
    
  virtual void collectExplicitVertexPoints 
    (vector<WH_Vector2D>& point_s_OUT);

  virtual void clearDuplicatedFacets 
    (WH_GM2D_FacetBody* bodyFrom,
     const vector<WH_GM2D_SegmentFacet*>& facetFrom_s,
     const vector<WH_GM2D_SegmentFacet*>& facetBy_s,
     vector<WH_GM2D_SegmentFacet*>& remainingFacet_s_IO,
     vector<WH_GM2D_SegmentFacet*>& changingFacet_s_IO);

  /* derived */
  
};
