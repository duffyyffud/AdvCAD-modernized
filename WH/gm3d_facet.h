/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_facet.cc */

#pragma once
#ifndef WH_INCLUDED_WH_TRIANGLE3D
#include <WH/triangle3d.h>
#define WH_INCLUDED_WH_TRIANGLE3D
#endif

#ifndef WH_INCLUDED_WH_INOUT3D
#include <WH/inout3d.h>
#define WH_INCLUDED_WH_INOUT3D
#endif

class WH_CNCT2D_SegmentCluster;
class WH_CNCT2D_TriangleCluster;

class WH_GM3D_SegmentFacet;
class WH_GM3D_TriangleFacet;
class WH_GM3D_PolygonFacet;
class WH_GM3D_RegularFacetGroup;
class WH_GM3D_NonRegularFacetGroup;
class WH_GM3D_FacetBody;

class WH_GM3D_SegmentFacet {
 public:
  static void divideFacetsBySegments 
    (const vector<WH_Segment3D>& segmentBy_s,
     vector<WH_GM3D_SegmentFacet*>& facet_s_IO  /* DELETE && CREATE */);

  static void divideFacetsByPoints 
    (const vector<WH_Vector3D>& pointBy_s,
     vector<WH_GM3D_SegmentFacet*>& facet_s_IO  /* DELETE && CREATE */);

  WH_GM3D_SegmentFacet 
    (const WH_Vector3D& firstPoint,
     const WH_Vector3D& lastPoint,
     bool isExplicit);
  virtual ~WH_GM3D_SegmentFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void changeAsExplicit ();

  virtual bool createDividedFacetsBySegment 
    (const WH_Segment3D& segmentBy, 
     vector<WH_GM3D_SegmentFacet*>& facet_s_OUT  /* CREATE */);

  virtual bool createDividedFacetsByPoint 
    (const WH_Vector3D& pointBy, 
     vector<WH_GM3D_SegmentFacet*>& facet_s_OUT  /* CREATE */);

  /* factory method */
  virtual WH_GM3D_SegmentFacet* createCopy ();
  virtual WH_GM3D_SegmentFacet* createCopyBetween 
    (const WH_Vector3D& firstPoint,
     const WH_Vector3D& lastPoint);

  WH_Vector3D firstPoint () const;
  WH_Vector3D lastPoint () const;
  
  bool isExplicit () const;

  WH_Segment3D segment () const;

  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;
  
  /* derived */
  
 protected:
  WH_Vector3D _firstPoint;
  WH_Vector3D _lastPoint;

  bool _isExplicit;

  /* base */

  /* derived */
  
};

class WH_GM3D_TriangleFacet {
 public:
  static void divideFacetsByTriangles 
    (const vector<WH_Triangle3D>& triangleBy_s,
     vector<WH_GM3D_TriangleFacet*>& facet_s_IO  /* DELETE && CREATE */);

  static void divideFacetsBySegments 
    (const vector<WH_Segment3D>& segmentBy_s,
     vector<WH_GM3D_TriangleFacet*>& facet_s_IO  /* DELETE && CREATE */);

  static void divideFacetsByPoints 
    (const vector<WH_Vector3D>& pointBy_s,
     vector<WH_GM3D_TriangleFacet*>& facet_s_IO  /* DELETE && CREATE */);

  WH_GM3D_TriangleFacet 
    (const WH_Plane3D& plane,
     const WH_Triangle2D& parameterTriangle,
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume,
     int faceId);
  /* Normal of <plane> is oriented toward front side. */
  virtual ~WH_GM3D_TriangleFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void reverseNormal ();

  virtual void changeFromOuterToInner ();

  virtual bool createDividedFacetsByTriangle 
    (const WH_Triangle3D& triangleBy, 
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT  /* CREATE */);

  virtual bool createDividedFacetsBySegment 
    (const WH_Segment3D& segmentBy, 
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT  /* CREATE */);

  virtual bool createDividedFacetsByPoint 
    (const WH_Vector3D& pointBy, 
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT  /* CREATE */);

  /* factory method */
  virtual WH_GM3D_TriangleFacet* createCopy ();
  virtual WH_GM3D_TriangleFacet* createCopyAmong 
    (const WH_Vector2D& parameter0,
     const WH_Vector2D& parameter1,
     const WH_Vector2D& parameter2);

  WH_Plane3D plane () const;
     
  WH_Triangle3D triangle () const;
  WH_Triangle2D parameterTriangle () const;

  bool frontSideIsInsideVolume () const;
  bool backSideIsInsideVolume () const;

  int faceId () const;
  
  enum FaceType {
    SHEET,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };
  FaceType faceType () const;

  virtual bool getNormalToOutsideVolume 
    (WH_Vector3D& normal_OUT) const;
  
  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;
  
  virtual void getParameterRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;
  
  /* derived */
  
 protected:
  WH_Plane3D _plane;

  WH_Triangle3D _triangle;
  WH_Triangle2D _parameterTriangle;

  WH_Vector3D _minRange; 
  WH_Vector3D _maxRange; 

  WH_Vector2D _parameterMinRange; 
  WH_Vector2D _parameterMaxRange; 
  
  bool _frontSideIsInsideVolume;
  bool _backSideIsInsideVolume;
  
  int _faceId;

  /* base */
  virtual void checkIntersectionOfDF
    (WH_Triangle2D::EdgeIntersectionData edgeData[3],
     vector<int>& vertexId_s_OUT,
     vector<int>& edgeId_s_OUT,
     vector<WH_Vector2D>& edgePoint_s_OUT);

  virtual void createDFOfCase_E
    (int edgeId, 
     const WH_Vector2D& edgePoint,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  virtual void createDFOfCase_I
    (const WH_Vector2D& insidePoint,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  virtual void createDFOfCase_EE_Coinsident
    (int edgeId, 
     const WH_Vector2D& edgePoint0,
     const WH_Vector2D& edgePoint1,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  virtual void createDFOfCase_EE_NoCoinsident
    (int edgeId0, 
     const WH_Vector2D& edgePoint0,
     int edgeId1, 
     const WH_Vector2D& edgePoint1,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  virtual void createDFOfCase_EI
    (int edgeId, 
     const WH_Vector2D& edgePoint,
     const WH_Vector2D& insidePoint,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  virtual void createDFOfCase_II
    (const WH_Vector2D& insidePoint0,
     const WH_Vector2D& insidePoint1,
     vector<WH_GM3D_TriangleFacet*>& facet_s_OUT);

  /* derived */
  
};

class WH_GM3D_PolygonFacet {
 public:
  WH_GM3D_PolygonFacet 
    (const WH_Plane3D& plane,
     const WH_Polygon2D& outerLoopParameterPolygon, 
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);
  /* Normal of <plane> is oriented toward front side. */
  virtual ~WH_GM3D_PolygonFacet ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void setFaceId (int id);
  /* called from WH_GM3D_FacetBody when this is added into it */

  virtual void addInnerLoop 
    (const WH_Polygon2D& parameterPolygon);
  
  virtual void addOffLoopEdgeSegment 
    (const WH_Segment2D& parameterSegment);
  
  virtual void addOffLoopVertexPoint 
    (const WH_Vector2D& parameter);
  
  virtual void insertEdgeSegment 
    (const WH_Segment3D& segment);
  
  virtual void insertVertexPoint 
    (const WH_Vector3D& point);
  
  virtual void generateTriangleFacets ();
  
  virtual void reverseNormal ();
  
  WH_Plane3D plane () const;
  
  WH_Polygon3D outerLoopPolygon () const; 
  WH_Polygon2D outerLoopParameterPolygon () const; 
  
  const vector<WH_Polygon3D>& innerLoopPolygon_s () const; 
  const vector<WH_Polygon2D>& innerLoopParameterPolygon_s () const; 
  
  const vector<WH_Segment3D>& offLoopEdgeSegment_s () const; 
  const vector<WH_Segment2D>& offLoopParameterEdgeSegment_s () const; 

  const vector<WH_Vector3D>& offLoopVertexPoint_s () const; 
  const vector<WH_Vector2D>& offLoopParameterVertexPoint_s () const; 

  bool frontSideIsInsideVolume () const;
  bool backSideIsInsideVolume () const;
  
  int faceId () const;

  bool isRegular () const;
  
  enum FaceType {
    SHEET,
    INNER_BOUNDARY,
    OUTER_BOUNDARY
  };
  FaceType faceType () const;
  
  const vector<WH_GM3D_TriangleFacet*>& triangleFacet_s () const;
  
  virtual bool getNormalToOutsideVolume 
    (WH_Vector3D& normal_OUT) const;
  
  virtual void getRange 
    (WH_Vector3D& minRange_OUT, 
     WH_Vector3D& maxRange_OUT) const;
  
  virtual void getParameterRange 
    (WH_Vector2D& minRange_OUT, 
     WH_Vector2D& maxRange_OUT) const;
  
  virtual WH_Polygon2D::ContainmentType checkContainmentAt 
    (const WH_Vector2D& parameter) const;
  
  virtual bool contains 
    (const WH_Vector3D& point) const;
  
  virtual bool justContains 
    (const WH_Vector3D& point) const;

  /* derived */
  
 protected:
  WH_Plane3D _plane;
  
  WH_Polygon3D _outerLoopPolygon; 
  WH_Polygon2D _outerLoopParameterPolygon; 

  WH_Vector3D _minRange; 
  WH_Vector3D _maxRange; 

  WH_Vector2D _parameterMinRange; 
  WH_Vector2D _parameterMaxRange; 

  vector<WH_Polygon3D> _innerLoopPolygon_s; 
  vector<WH_Polygon2D> _innerLoopParameterPolygon_s; 

  vector<WH_Segment3D> _offLoopEdgeSegment_s; 
  vector<WH_Segment2D> _offLoopParameterEdgeSegment_s; 

  vector<WH_Vector3D> _offLoopVertexPoint_s; 
  vector<WH_Vector2D> _offLoopParameterVertexPoint_s; 

  vector<WH_GM3D_TriangleFacet*> _triangleFacet_s;   /* own */

  bool _frontSideIsInsideVolume;
  bool _backSideIsInsideVolume;
  
  int _faceId;

  /* base */
  virtual bool insertVertexPointOnEdgeOfLoop 
    (const WH_Vector2D& parameter,
     WH_Polygon3D& poly_IO,
     WH_Polygon2D& paramPoly_IO);

  virtual void insertVertexPointOnLoop 
    (const WH_Vector2D& parameter);

  virtual void insertVertexPointOffLoop 
    (const WH_Vector2D& parameter);

  /* factory method */
  virtual WH_GM3D_TriangleFacet* createTriangleFacet 
    (const WH_Vector2D& parameter0,
     const WH_Vector2D& parameter1,
     const WH_Vector2D& parameter2);

  /* derived */
  
};

class WH_GM3D_RegularFacetGroup {
 public:
  WH_GM3D_RegularFacetGroup 
    (WH_GM3D_FacetBody* body,
     const WH_Plane3D& plane);
  virtual ~WH_GM3D_RegularFacetGroup ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addTriangleFacet 
    (WH_GM3D_TriangleFacet* facet  /* ADOPT */);

  virtual void generatePolygonFacets ();

  WH_GM3D_FacetBody* body () const;

  WH_Plane3D plane () const;

  const vector<WH_GM3D_TriangleFacet*>& triangleFacet_s () const;

  /* derived */

 protected:
  WH_GM3D_FacetBody* _body;

  WH_Plane3D _plane;

  vector<WH_GM3D_TriangleFacet*> _triangleFacet_s;  /* own */

  /* base */
  virtual void divideIncompatibleTriangleFacets ();
  
  virtual WH_Polygon2D parameterPolygonOfBoundaryCluster 
    (const WH_CNCT2D_SegmentCluster* boundaryCluster,
     const WH_Plane3D& planeOfNewFacet) const;
  
  virtual void generatePolygonFacetOfCluster 
    (const WH_CNCT2D_TriangleCluster* cluster);

  /* derived */

};

class WH_GM3D_NonRegularFacetGroup {
 public:
  WH_GM3D_NonRegularFacetGroup 
    (WH_GM3D_FacetBody* body,
     int faceId);
  virtual ~WH_GM3D_NonRegularFacetGroup ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addTriangleFacet 
    (WH_GM3D_TriangleFacet* facet  /* ADOPT */);

  virtual void generatePolygonFacets ();

  WH_GM3D_FacetBody* body () const;

  int faceId () const;

  const vector<WH_GM3D_TriangleFacet*>& triangleFacet_s () const;

  /* derived */

 protected:
  WH_GM3D_FacetBody* _body;

  int _faceId;

  vector<WH_GM3D_TriangleFacet*> _triangleFacet_s;  /* own */

  /* base */
  virtual WH_Polygon2D parameterPolygonOfBoundaryCluster 
    (const WH_CNCT2D_SegmentCluster* boundaryCluster) const;
  
  virtual void generatePolygonFacetOfCluster 
    (const WH_CNCT2D_TriangleCluster* cluster);

  /* derived */

};

class WH_GM3D_FacetBody {
 public:
  WH_GM3D_FacetBody (bool isRegular);
  virtual ~WH_GM3D_FacetBody ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* base */
  virtual void addVertexPoint (const WH_Vector3D& point);
  
  virtual void addSegmentFacet 
    (WH_GM3D_SegmentFacet* facet  /* ADOPT */);
  
  virtual void addPolygonFacet 
    (WH_GM3D_PolygonFacet* facet  /* ADOPT */);
  
  virtual void generateTriangleFacets ();

  virtual void addTriangleFacet 
    (WH_GM3D_TriangleFacet* facet  /* ADOPT */);

  virtual void generatePolygonFacets ();

  virtual void reverseNormal ();

  virtual void setUpInOutCheck ();

  /* factory method */
  virtual WH_GM3D_SegmentFacet* createSegmentFacet 
    (const WH_Vector3D& firstPoint,
     const WH_Vector3D& lastPoint,
     bool isExplicit);
  virtual WH_GM3D_TriangleFacet* createTriangleFacet 
    (const WH_Plane3D& plane,
     const WH_Triangle2D& parameterTriangle,
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume,
     int faceId);
  virtual WH_GM3D_PolygonFacet* createPolygonFacet 
    (const WH_Plane3D& plane,
     const WH_Polygon2D& outerLoopParameterPolygon, 
     bool frontSideIsInsideVolume,
     bool backSideIsInsideVolume);

  bool isRegular () const;

  bool normalIsReversed () const;

  const vector<WH_Vector3D>& vertexPoint_s () const;
  
  const vector<WH_GM3D_SegmentFacet*>& segmentFacet_s () const;
  
  const vector<WH_GM3D_PolygonFacet*>& polygonFacet_s () const;
  
  const vector<WH_GM3D_TriangleFacet*>& triangleFacet_s () const;
  
  enum BodyType {
    EMPTY,
    DOT_SET,
    WIRE_SET,
    SHEET_SET,
    VOLUME,
    OTHER
  };
  virtual BodyType bodyType () const;

  virtual WH_InOutChecker3D::ContainmentType 
    checkContainmentAt 
    (const WH_Vector3D& point) const;

  virtual WH_GM3D_SegmentFacet* findSegmentFacet 
    (const WH_Vector3D& point0,
     const WH_Vector3D& point1) const;
  
  virtual WH_GM3D_PolygonFacet* findPolygonFacet 
    (const vector<WH_Vector3D>& point_s) const;

  /* derived */
  
 protected:
  static int _faceCount;

  bool _isRegular;

  bool _normalIsReversed;

  vector<WH_Vector3D> _vertexPoint_s;
  
  vector<WH_GM3D_SegmentFacet*> _segmentFacet_s;  /* own */

  vector<WH_GM3D_PolygonFacet*> _polygonFacet_s;  /* own */

  vector<WH_GM3D_TriangleFacet*> _triangleFacet_s;  /* own */

  WH_InOutChecker3D* _inOutChecker;  /* own */

  /* base */
  virtual void generateRegularPolygonFacets ();

  virtual void generateNonRegularPolygonFacets ();

  /* derived */
  
};
