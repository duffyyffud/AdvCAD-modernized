/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for connector2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_CONNECTOR
#include <WH/connector.h>
#define WH_INCLUDED_WH_CONNECTOR
#endif

#ifndef WH_INCLUDED_WH_SEGMENT2D
#include <WH/segment2d.h>
#define WH_INCLUDED_WH_SEGMENT2D
#endif

#ifndef WH_INCLUDED_WH_TRIANGLE2D
#include <WH/triangle2d.h>
#define WH_INCLUDED_WH_TRIANGLE2D
#endif

class WH_CNCT2D_SegmentNode;
class WH_CNCT2D_SegmentCluster;
class WH_CNCT2D_SegmentConnector;

class WH_CNCT2D_TriangleNode;
class WH_CNCT2D_TriangleCluster;
class WH_CNCT2D_TriangleConnector;

class WH_CNCT2D_NonRegularTriangleCluster;
class WH_CNCT2D_NonRegularTriangleConnector;

/* WH_CNCT2D_SegmentConnector and related classes */

class WH_CNCT2D_SegmentNode : public WH_CNCT_ListNode {
 public:
  WH_CNCT2D_SegmentNode 
    (const WH_Segment2D& segment);
  virtual ~WH_CNCT2D_SegmentNode ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /*
    portId 0 -> segment.p0 ()
    portId 1 -> segment.p1 ()
  */

  /* base */
  WH_Segment2D segment () const;

  /* derived */

 protected:
  WH_Segment2D _segment;

  /* base */

  /* derived */

};

class WH_CNCT2D_SegmentCluster : public WH_CNCT_ListCluster {
 public:
  WH_CNCT2D_SegmentCluster (int id);
  virtual ~WH_CNCT2D_SegmentCluster ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void setOuterLoopCluster 
    (WH_CNCT2D_SegmentCluster* cluster);
  
  virtual void findInnerLoops 
    (WH_CNCT2D_SegmentConnector* connector);

  WH_CNCT2D_SegmentCluster* outerLoopCluster () const;
  
  const vector<WH_CNCT2D_SegmentCluster*>& innerLoopCluster_s () const;

  bool isOuterLoop () const;
  
  bool isInnerLoop () const;

  virtual void getSegments 
    (vector<WH_Segment2D>& segment_s_OUT) const;

  virtual void getRegularizedSegments 
    (vector<WH_Segment2D>& segment_s_OUT,
     vector<int>& endId_s_OUT) const;
  
  virtual void getVertexsFromSegments 
    (const vector<WH_Segment2D>& segment_s,
     vector<WH_Vector2D>& vertex_s_OUT) const;
  
  /* derived */
  
 protected:
  WH_CNCT2D_SegmentCluster* _outerLoopCluster;  /* not own */
  
  vector<WH_CNCT2D_SegmentCluster*> _innerLoopCluster_s;  /* not own */

  /* base */

  /* derived */

};

class WH_CNCT2D_SegmentConnector : public WH_CNCT_ListConnector_A {
 public:
  WH_CNCT2D_SegmentConnector ();
  virtual ~WH_CNCT2D_SegmentConnector ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void identifyLoops ();

  /* derived */
  
 protected:

  /* base */

  /* derived */
  virtual WH_CNCT_Cluster* createCluster (int clusterId);

  virtual void findConnection 
    (WH_CNCT_Node_A* node, int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT);

};

/* WH_CNCT2D_TriangleConnector and related classes */

class WH_CNCT2D_TriangleNode : public WH_CNCT_Node_A {
 public:
  WH_CNCT2D_TriangleNode 
    (const WH_Triangle2D& triangle);
  virtual ~WH_CNCT2D_TriangleNode ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /*
    portId i -> triangle.edge (i)
  */

  /* base */
  WH_Triangle2D triangle () const;

  /* derived */
  virtual void setConnectionAtPort 
    (int portId,
     WH_CNCT_Node_A* connectedNode, int connectedPortId);
  
  virtual int nPorts () const;

  virtual void getConnectionAtPort 
    (int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT) const;

 protected:
  WH_CNCT_Node_A* _connectedNodes[3];

  int _connectedPortIds[3];

  WH_Triangle2D _triangle;

  /* base */

  /* derived */

};

class WH_CNCT2D_TriangleCluster : public WH_CNCT_Cluster {
 public:
  WH_CNCT2D_TriangleCluster (int id);
  virtual ~WH_CNCT2D_TriangleCluster ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void extractBoundary ();

  WH_CNCT2D_SegmentConnector* boundaryConnector () const;

  virtual WH_CNCT2D_SegmentCluster* 
    outerLoopBoundaryCluster () const;

  virtual void getTriangles 
    (vector<WH_Triangle2D>& triangle_s_OUT) const;

  /* derived */
  
 protected:
  WH_CNCT2D_SegmentConnector* _boundaryConnector;  /* own */

  /* base */

  /* factory method */
  virtual WH_CNCT2D_SegmentConnector* createBoundaryConnector ();
  virtual WH_CNCT2D_SegmentNode* createBoundaryNode 
    (const WH_Segment2D& segment);

  /* derived */

};

class WH_CNCT2D_TriangleConnector : public WH_CNCT_Connector_A {
 public:
  WH_CNCT2D_TriangleConnector ();
  virtual ~WH_CNCT2D_TriangleConnector ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void extractBoundary ();

  /* derived */
  
 protected:

  /* base */

  /* derived */
  virtual WH_CNCT_Cluster* createCluster (int clusterId);

  virtual void findConnection 
    (WH_CNCT_Node_A* node, int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT);

};

/* WH_CNCT2D_NonRegularTriangleConnector and related classes */

/* use WH_CNCT2D_TriangleNode also */

class WH_CNCT2D_NonRegularTriangleCluster 
: public WH_CNCT2D_TriangleCluster {
 public:
  WH_CNCT2D_NonRegularTriangleCluster (int id);
  virtual ~WH_CNCT2D_NonRegularTriangleCluster ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

  /* derived */
  virtual void extractBoundary ();
  
 protected:

  /* base */

  /* derived */

};

class WH_CNCT2D_NonRegularTriangleConnector 
: public WH_CNCT2D_TriangleConnector {
 public:
  WH_CNCT2D_NonRegularTriangleConnector ();
  virtual ~WH_CNCT2D_NonRegularTriangleConnector ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addInternalBoundarySegment 
    (const WH_Segment2D& segment);
  
  const vector<WH_Segment2D>& internalBoundarySegment_s () const;

  /* derived */
  
 protected:
  vector<WH_Segment2D> _internalBoundarySegment_s;

  /* base */

  /* derived */
  virtual WH_CNCT_Cluster* createCluster (int clusterId);

  virtual void findConnection 
    (WH_CNCT_Node_A* node, int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT);

};
