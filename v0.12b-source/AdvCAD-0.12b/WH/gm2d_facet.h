/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm2d_facet.cc */

#ifndef WH_HEADER_WH_GM2D_FACET
#define WH_HEADER_WH_GM2D_FACET

#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif

#ifndef WH_INCLUDED_WH_INOUT2D
#include <WH/inout2d.h>
#define WH_INCLUDED_WH_INOUT2D
#endif



class WH_GM2D_SegmentFacet;
class WH_GM2D_FacetBody;



class WH_GM2D_SegmentFacet {
 public:
  static void divideFacetsBySegments 
    (const vector<WH_Segment2D>& segmentBy_s,
     vector<WH_GM2D_SegmentFacet*>& facet_s_IO  /* DELETE && CREATE */);

  static void divideFacetsByPoints 
    (const vector<WH_Vector2D>& pointBy_s,
     vector<WH_GM2D_SegmentFacet*>& facet_s_IO  /* DELETE && CREATE */);

  WH_GM2D_SegmentFacet 
    (const WH_Vector2D& firstPoint,
     const WH_Vector2D& lastPoint,
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);
  virtual ~WH_GM2D_SegmentFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void reverseNormal ();

  virtual void changeFromOuterToInner ();

  virtual bool createDividedFacetsBySegment 
    (const WH_Segment2D& segmentBy, 
     vector<WH_GM2D_SegmentFacet*>& facet_s_OUT  /* CREATE */);

  virtual bool createDividedFacetsByPoint 
    (const WH_Vector2D& pointBy, 
     vector<WH_GM2D_SegmentFacet*>& facet_s_OUT  /* CREATE */);

  /* factory method */
  virtual WH_GM2D_SegmentFacet* createCopy ();
  virtual WH_GM2D_SegmentFacet* createCopyBetween 
    (const WH_Vector2D& firstPoint,
     const WH_Vector2D& lastPoint);

  WH_Vector2D firstPoint () const;
  WH_Vector2D lastPoint () const;
  
  bool frontSideIsInsideVolume () const;
  bool backSideIsInsideVolume () const;
  
  /* 
             front
     first -------- last
             back     
   */

  enum EdgeType {
    WIRE,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };
  EdgeType edgeType () const;

  WH_Segment2D segment () const;

  virtual bool getNormalToOutsideVolume 
    (WH_Vector2D& normal_OUT) const;
  
  virtual void getRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;
  
  /* derived */
  
 protected:
  WH_Vector2D _firstPoint;
  WH_Vector2D _lastPoint;
  
  bool _frontSideIsInsideVolume;
  bool _backSideIsInsideVolume;
  
  /* base */

  /* derived */
  
};

class WH_GM2D_FacetBody {
 public:
  WH_GM2D_FacetBody (bool isRegular);
  virtual ~WH_GM2D_FacetBody ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addVertexPoint (const WH_Vector2D& point);
  
  virtual void addSegmentFacet 
    (WH_GM2D_SegmentFacet* facet  /* ADOPT */);

  virtual void regularizeFacets ();
  
  virtual void reverseNormal ();

  virtual void setUpInOutCheck ();

  /* factory method */
  virtual WH_GM2D_SegmentFacet* createSegmentFacet 
    (const WH_Vector2D& firstPoint,
     const WH_Vector2D& lastPoint,
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);

  bool isRegular () const;

  bool normalIsReversed () const;

  const vector<WH_Vector2D>& vertexPoint_s () const;
  
  const vector<WH_GM2D_SegmentFacet*>& segmentFacet_s () const;
  
  enum BodyType {
    EMPTY,
    DOT_SET,
    WIRE_SET,
    VOLUME,
    OTHER
  };
  virtual BodyType bodyType () const;

  virtual WH_InOutChecker2D::ContainmentType 
    checkContainmentAt 
    (const WH_Vector2D& point) const;

  /* derived */
  
 protected:
  bool _isRegular;

  bool _normalIsReversed;

  vector<WH_Vector2D> _vertexPoint_s;
  
  vector<WH_GM2D_SegmentFacet*> _segmentFacet_s;  /* own */

  WH_InOutChecker2D* _inOutChecker;  /* own */

  /* base */
  
  /* derived */
  
};



#endif /* WH_HEADER_WH_GM2D_FACET */
