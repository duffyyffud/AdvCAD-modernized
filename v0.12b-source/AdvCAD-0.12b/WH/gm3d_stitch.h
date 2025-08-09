/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_stitch.cc */

#ifndef WH_HEADER_WH_GM3D_STITCH
#define WH_HEADER_WH_GM3D_STITCH

#ifndef WH_INCLUDED_WH_GM3D_FACET
#include <WH/gm3d_facet.h>
#define WH_INCLUDED_WH_GM3D_FACET
#endif



class WH_GM3D_Stitcher;



class WH_GM3D_Stitcher {
 public:
  WH_GM3D_Stitcher 
    (WH_GM3D_FacetBody* body0,
     WH_GM3D_FacetBody* body1,
     WH_GM3D_FacetBody* resultBody);
  virtual ~WH_GM3D_Stitcher();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void perform ();
  
  WH_GM3D_FacetBody* body0 () const;
  WH_GM3D_FacetBody* body1 () const;

  WH_GM3D_FacetBody* resultBody () const;

  /* derived */
  
 protected:
  WH_GM3D_FacetBody* _body0;  /* not own */
  WH_GM3D_FacetBody* _body1;  /* not own */

  WH_GM3D_FacetBody* _resultBody;  /* not own */

  /* base */
  virtual void divideBodyByBody 
    (WH_GM3D_FacetBody* bodyFrom, 
     WH_GM3D_FacetBody* bodyBy,
     vector<WH_GM3D_SegmentFacet*>& segmentFacet_s_OUT,  /* CREATE */
     vector<WH_GM3D_TriangleFacet*>& triangleFacet_s_OUT  /* CREATE */);

  virtual void collectExplicitVertexPoints 
    (vector<WH_Vector3D>& point_s_OUT);

  virtual void changeSegmentFacetsAsExplicit 
    (WH_GM3D_FacetBody* bodyBy,
     vector<WH_GM3D_SegmentFacet*>& facet_s_IO);

  virtual void clearDuplicatedSegmentFacets 
    (WH_GM3D_FacetBody* bodyBy,
     const vector<WH_GM3D_SegmentFacet*>& facet_s,
     vector<WH_GM3D_SegmentFacet*>& remainingFacet_s_OUT);
  
  virtual void clearDuplicatedTriangleFacets 
    (WH_GM3D_FacetBody* bodyBy,
     const vector<WH_GM3D_TriangleFacet*>& facet_s,
     vector<WH_GM3D_TriangleFacet*>& remainingFacet_s_OUT);

  virtual void collectTriangleFacets 
    (const vector<WH_GM3D_TriangleFacet*>& facet_s);

  virtual void collectSegmentFacets 
    (const vector<WH_GM3D_SegmentFacet*>& facet_s);

  /* derived */
  
};



#endif /* WH_HEADER_WH_GM3D_STITCH */

