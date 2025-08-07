/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* connector2d.cc : 2-D connector framework */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "connector2d.h"



/* WH_CNCT2D_SegmentNode class */

WH_CNCT2D_SegmentNode
::WH_CNCT2D_SegmentNode 
(const WH_Segment2D& segment)
  : _segment (segment)
{
  WH_CVR_LINE;
}

WH_CNCT2D_SegmentNode
::~WH_CNCT2D_SegmentNode ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_SegmentNode
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_ListNode::checkInvariant ();

  return true;
}

bool WH_CNCT2D_SegmentNode
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_ListNode::assureInvariant ();
  
  return true;
}

WH_Segment2D WH_CNCT2D_SegmentNode
::segment () const
{
  return _segment;
}



/* WH_CNCT2D_SegmentCluster class */

WH_CNCT2D_SegmentCluster
::WH_CNCT2D_SegmentCluster (int id)
  : WH_CNCT_ListCluster (id)
{
  WH_CVR_LINE;

  _outerLoopCluster = WH_NULL;
}

WH_CNCT2D_SegmentCluster
::~WH_CNCT2D_SegmentCluster ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_SegmentCluster
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_ListCluster::checkInvariant ();

  return true;
}

bool WH_CNCT2D_SegmentCluster
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_ListCluster::assureInvariant ();

  if (this->isSorted ()) {
    WH_CVR_LINE;
    
    int nNodes = (int)this->node_s ().size ();

    if (this->isSequential ()) {
      WH_CVR_LINE;
      WH_ASSERT(0 < nNodes);
    } else {
      WH_CVR_LINE;
      WH_ASSERT(2 < nNodes);
    }

    vector<WH_Segment2D> segment_s;
    this->getSegments 
      (segment_s);
    WH_ASSERT(0 < segment_s.size ());
    
    vector<WH_Vector2D> vertex_s;
    this->getVertexsFromSegments (segment_s,
				  vertex_s);
    WH_ASSERT(2 <= vertex_s.size ());
    
    vector<int> endId_s;
    this->getRegularizedSegments 
      (segment_s, endId_s);
    WH_ASSERT(0 < segment_s.size ());
    WH_ASSERT(endId_s.size () == segment_s.size ());

    int nSegs = (int)segment_s.size ();
    for (int iSeg = 0; iSeg < nSegs; iSeg++) {
      int endId = endId_s[iSeg];
      WH_ASSERT(0 <= endId);
      WH_ASSERT(endId < nNodes);
      if (0 < iSeg) {
	int prevEndId = endId_s[iSeg - 1];
	WH_ASSERT(prevEndId < endId);
      }
    }
    
    int lastEndId = endId_s[nSegs - 1];
    if (this->isSequential ()) {
      WH_ASSERT(lastEndId == nNodes - 1);
    }  else {
      WH_ASSERT(1 < lastEndId);
    }
  }


  return true;
}

void WH_CNCT2D_SegmentCluster
::setOuterLoopCluster (WH_CNCT2D_SegmentCluster* cluster)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isSorted ());
  WH_ASSERT(!this->isSequential ());
  WH_ASSERT(_outerLoopCluster == WH_NULL);
  WH_ASSERT(cluster != WH_NULL);
  
  _outerLoopCluster = cluster;
}

void WH_CNCT2D_SegmentCluster
::findInnerLoops (WH_CNCT2D_SegmentConnector* connector)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isSorted ());
  WH_ASSERT(connector != WH_NULL);

  WH_CVR_LINE;

  if (!this->isSequential ()) {
    WH_CVR_LINE;
    
    vector<WH_Segment2D> segment_s;
    this->getSegments (segment_s);
    vector<WH_Vector2D> vertex_s;
    this->getVertexsFromSegments (segment_s,
				  vertex_s);
    WH_Polygon2D poly (vertex_s);
    
    for (vector<WH_CNCT_Cluster*>::const_iterator
	   i_cluster = connector->cluster_s ().begin ();
	 i_cluster != connector->cluster_s ().end ();
	 i_cluster++) {
      WH_CNCT2D_SegmentCluster* cluster_i 
	= (WH_CNCT2D_SegmentCluster*)(*i_cluster);
      if (cluster_i == this) continue;
      
      WH_ASSERT(cluster_i->isSorted ());
      
      if (!cluster_i->isSequential ()) {
	WH_CVR_LINE;
	WH_CNCT2D_SegmentNode* anyNode 
	  = (WH_CNCT2D_SegmentNode*)cluster_i->node_s ()[0];
	WH_Vector2D anyPoint = anyNode->segment ().p0 ();
	
	if (poly.checkContainmentAt (anyPoint) == WH_Polygon2D::IN) {
	  _innerLoopCluster_s.push_back (cluster_i);
	  cluster_i->setOuterLoopCluster (this);
	}
      }
    }
  }
}

WH_CNCT2D_SegmentCluster* WH_CNCT2D_SegmentCluster
::outerLoopCluster () const
{
  return _outerLoopCluster;
}

const vector<WH_CNCT2D_SegmentCluster*>& WH_CNCT2D_SegmentCluster
::innerLoopCluster_s () const
{
  return _innerLoopCluster_s;
}

bool WH_CNCT2D_SegmentCluster
::isOuterLoop () const
{
  if (!this->isSequential ()
      && this->outerLoopCluster () == WH_NULL) {
    return true;
  }
  
  return false;
}

bool WH_CNCT2D_SegmentCluster
::isInnerLoop () const
{
  if (this->outerLoopCluster () != WH_NULL) {
    return true;
  }
  
  return false;
}

void WH_CNCT2D_SegmentCluster
::getSegments 
(vector<WH_Segment2D>& segment_s_OUT) const
{
  WH_CVR_LINE;

  segment_s_OUT.clear ();
  
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT2D_SegmentNode* node_i 
      = (WH_CNCT2D_SegmentNode*)(*i_node);
    segment_s_OUT.push_back (node_i->segment ());
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(segment_s_OUT.size () == this->node_s ().size ());
#endif
}

void WH_CNCT2D_SegmentCluster
::getRegularizedSegments 
(vector<WH_Segment2D>& segment_s_OUT,
 vector<int>& endId_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isSorted ());

  WH_CVR_LINE;

  segment_s_OUT.clear ();
  endId_s_OUT.clear ();

  const vector<WH_CNCT_Node_A*>& node_s = this->node_s ();
  int nNodes = (int)node_s.size ();
  WH_ASSERT(0 < nNodes);

  /* set the first facet as the current facet */
  WH_CNCT2D_SegmentNode* firstNode 
    = (WH_CNCT2D_SegmentNode*)node_s[0];
  WH_Segment2D firstSegment = firstNode->segment ();
  WH_Vector2D firstDir = firstSegment.line ().direction ();
  WH_Vector2D p0 = firstSegment.p0 ();
  WH_Vector2D p1 = firstSegment.p1 ();

  /* search backward */
  int iNodeBackward = nNodes - 1;
  if (!this->isSequential ()) {
    WH_CVR_LINE;

    WH_ASSERT(2 < nNodes);

    /* <this> is circular list, so search backward */
    for (; 0 <= iNodeBackward; iNodeBackward--) {
      WH_CNCT2D_SegmentNode* node 
	= (WH_CNCT2D_SegmentNode*)node_s[iNodeBackward];
      WH_Segment2D segment = node->segment ();
      WH_Vector2D dir = segment.line ().direction ();
      if (WH_eq (dir, firstDir)
	  || WH_isReverse (dir, firstDir)) {
	WH_CVR_LINE;                         /* NOT COVERED */
	
	/* 180 degree corner angle vertex */
	if (WH_eq (segment.p0 (), p0)) {
	  p0 = segment.p1 ();
	} else if (WH_eq (segment.p1 (), p0)) {
	  p0 = segment.p0 ();
	} else if (WH_eq (segment.p0 (), p1)) {
	  p1 = segment.p1 ();
	} else if (WH_eq (segment.p1 (), p1)) {
	  p1 = segment.p0 ();
	} else {
	  WH_ASSERT_NO_REACH;
	}
      } else {
	WH_CVR_LINE;
	break;
      }
    }
    WH_ASSERT(1 < iNodeBackward);
  }
  WH_ASSERT(WH_eq (p0, firstSegment.p0 ()) 
	    || WH_eq (p1, firstSegment.p1 ()));
  WH_ASSERT(WH_ne (p0, p1));
#ifndef NDEBUG
  {
    WH_Segment2D segment (p0, p1);
    WH_ASSERT(WH_eq (segment.line ().direction (), firstDir));
    WH_ASSERT(WH_le (firstSegment.length (), segment.length ()));
  }
#endif
  /* <iNodeBackward> is the end id of the last regularized segment. If
     <this> is circular, it is the previous index of the start id of
     the first regularized segment. */
  
  WH_Vector2D curDir = firstDir;

  /* search forward */
  int iNodeForward = 1;
  for (; iNodeForward <= iNodeBackward; iNodeForward++) {
    WH_CNCT2D_SegmentNode* node 
      = (WH_CNCT2D_SegmentNode*)node_s[iNodeForward];
    WH_Segment2D segment = node->segment ();
    WH_Vector2D dir = segment.line ().direction ();
    if (WH_eq (dir, curDir)
	|| WH_isReverse (dir, curDir)) {
      WH_CVR_LINE;

      /* 180 degree corner angle vertex */
      if (WH_eq (segment.p0 (), p0)) {
	p0 = segment.p1 ();
      } else if (WH_eq (segment.p1 (), p0)) {
	p0 = segment.p0 ();
      } else if (WH_eq (segment.p0 (), p1)) {
	p1 = segment.p1 ();
      } else if (WH_eq (segment.p1 (), p1)) {
	p1 = segment.p0 ();
      } else {
	WH_ASSERT_NO_REACH;
      }
    } else {
      WH_CVR_LINE;

      /* create a new regularized segment */
      WH_Segment2D newSegment (p0, p1);
      segment_s_OUT.push_back (newSegment);
      int endId = iNodeForward - 1;
      endId_s_OUT.push_back (endId);

      /* go to next search */
      curDir = dir;
      p0 = segment.p0 ();
      p1 = segment.p1 ();
    }
  }
  WH_ASSERT(iNodeForward == iNodeBackward + 1);

  {
    /* create the last regularized segment */
    
    WH_Segment2D newSegment (p0, p1);
    segment_s_OUT.push_back (newSegment);
    int endId = iNodeBackward;
    endId_s_OUT.push_back (endId);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < segment_s_OUT.size ());
  WH_ASSERT(endId_s_OUT.size () == segment_s_OUT.size ());
#endif
  /* Direction of each regularlized segment is the same as one of the
     first original segment of the regularlized segment.  If <this> is
     circular, direction of the first regularlized segment is the same
     as the first original segment of the cluster.  */
}

void WH_CNCT2D_SegmentCluster
::getVertexsFromSegments 
(const vector<WH_Segment2D>& segment_s,
 vector<WH_Vector2D>& vertex_s_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isSorted ());
  
  WH_CVR_LINE;
  
  vertex_s_OUT.clear ();
  
  int nSegs = (int)segment_s.size ();
  if (1 < nSegs) {
    WH_CVR_LINE;
    /* multiple segment case */
    
    for (int iSeg = 0; iSeg < nSegs - 1; iSeg++) {
      WH_Segment2D seg = segment_s[iSeg];
      WH_Segment2D nextSeg = segment_s[iSeg + 1];
      
      /* the first and the second vertexs */
      if (iSeg == 0) {
	WH_CVR_LINE;
	if (nextSeg.hasVertexAt (seg.p0 ())) {
	  WH_CVR_LINE;                         /* NOT COVERED */
	  vertex_s_OUT.push_back (seg.p1 ());
	  vertex_s_OUT.push_back (seg.p0 ());
	} else {
	  WH_CVR_LINE;
	  WH_ASSERT(nextSeg.hasVertexAt (seg.p1 ()));
	  vertex_s_OUT.push_back (seg.p0 ());
	  vertex_s_OUT.push_back (seg.p1 ());
	}
      }
      
      /* the next vertex */
      if (this->isSequential ()) {
	WH_CVR_LINE;
	/* sequential list -> polyline */
	
	if (seg.hasVertexAt (nextSeg.p0 ())) {
	  WH_CVR_LINE;
	  vertex_s_OUT.push_back (nextSeg.p1 ());
	} else {
	  WH_CVR_LINE;  
	  WH_ASSERT(seg.hasVertexAt (nextSeg.p1 ()));
	  vertex_s_OUT.push_back (nextSeg.p0 ());
	}
      } else {
	WH_CVR_LINE;
	/* cyclic list -> polygon */
	
	if (iSeg < nSegs - 2) {
	  WH_CVR_LINE;
	  if (seg.hasVertexAt (nextSeg.p0 ())) {
	    WH_CVR_LINE;
	    vertex_s_OUT.push_back (nextSeg.p1 ());
	  } else {
	    WH_CVR_LINE;
	    WH_ASSERT(seg.hasVertexAt (nextSeg.p1 ()));
	    vertex_s_OUT.push_back (nextSeg.p0 ());
	  }
	}
      }
    }

  } else if (nSegs == 1) {
    WH_CVR_LINE;
    /* single segment case */
    
    WH_Segment2D seg = segment_s[0];
    vertex_s_OUT.push_back (seg.p0 ());
    vertex_s_OUT.push_back (seg.p1 ());
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  if (this->isSequential ()) {
    WH_ASSERT(vertex_s_OUT.size () == segment_s.size () + 1);
  } else {
    WH_ASSERT(vertex_s_OUT.size () == segment_s.size ());
  }
#endif
}



/* WH_CNCT2D_SegmentConnector class */

WH_CNCT2D_SegmentConnector
::WH_CNCT2D_SegmentConnector ()
{
  WH_CVR_LINE;
}

WH_CNCT2D_SegmentConnector
::~WH_CNCT2D_SegmentConnector ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_SegmentConnector
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_ListConnector_A::checkInvariant ();

  return true;
}

bool WH_CNCT2D_SegmentConnector
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_ListConnector_A::assureInvariant ();
  
  return true;
}

void WH_CNCT2D_SegmentConnector
::identifyLoops ()
{
  WH_CVR_LINE;

  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = _cluster_s.begin ();
       i_cluster != _cluster_s.end ();
       i_cluster++) {
    WH_CNCT2D_SegmentCluster* cluster_i 
      = (WH_CNCT2D_SegmentCluster*)(*i_cluster);

    cluster_i->findInnerLoops (this);
  }
}

WH_CNCT_Cluster* WH_CNCT2D_SegmentConnector
::createCluster (int clusterId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= clusterId);
  
  WH_CNCT_Cluster* result 
    = new WH_CNCT2D_SegmentCluster (clusterId);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_CNCT2D_SegmentConnector
::findConnection 
(WH_CNCT_Node_A* node, int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < node->nPorts ());

  WH_CVR_LINE;

  connectedNode_OUT = WH_NULL;
  connectedPortId_OUT = WH_NO_INDEX;

  /* search over all the segment nodes other than <node>, and find a
     segment node connected at either end point with <node> at the
     point of <portID> */

  /*
    portId 0 -> segment.p0 ()
    portId 1 -> segment.p1 ()
  */

  WH_CNCT2D_SegmentNode* segmentNode = (WH_CNCT2D_SegmentNode*)node;
  WH_Segment2D segment = segmentNode->segment ();
  WH_Vector2D point = (portId == 0) ? segment.p0 () : segment.p1 ();

  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    if (node_i == node) continue;

    WH_CNCT2D_SegmentNode* segmentNode_i 
      = (WH_CNCT2D_SegmentNode*)node_i;
    WH_Segment2D segment_i = segmentNode_i->segment ();
    if (WH_eq (segment_i.p0 (), point)) {
      WH_CVR_LINE;
      connectedNode_OUT = node_i;
      connectedPortId_OUT = 0;
      break;
    } else if (WH_eq (segment_i.p1 (), point)) {
      WH_CVR_LINE;
      connectedNode_OUT = node_i;
      connectedPortId_OUT = 1;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(connectedNode_OUT != WH_NULL 
	    || connectedNode_OUT == WH_NULL);
  if (connectedNode_OUT != WH_NULL) {
    WH_CVR_LINE;
    WH_ASSERT(0 <= connectedPortId_OUT);
    WH_ASSERT(connectedPortId_OUT < connectedNode_OUT->nPorts ());

    /* check if there is only one port */
    int count = 0;

    WH_CNCT2D_SegmentNode* segmentNode = (WH_CNCT2D_SegmentNode*)node;
    WH_Segment2D segment = segmentNode->segment ();
    WH_Vector2D point = (portId == 0) ? segment.p0 () : segment.p1 ();

    for (vector<WH_CNCT_Node_A*>::const_iterator
	   i_node = this->node_s ().begin ();
	 i_node != this->node_s ().end ();
	 i_node++) {
      WH_CNCT_Node_A* node_i = (*i_node);
      if (node_i == node) continue;
      
      WH_CNCT2D_SegmentNode* segmentNode_i 
	= (WH_CNCT2D_SegmentNode*)node_i;
      WH_Segment2D segment_i = segmentNode_i->segment ();
      if (segment_i.hasVertexAt (point)) {
	count++;
      }
    }

    WH_ASSERT(count == 1);
  }
#endif
}



/* WH_CNCT2D_TriangleNode class */

WH_CNCT2D_TriangleNode
::WH_CNCT2D_TriangleNode 
(const WH_Triangle2D& triangle)
  : _triangle (triangle)
{
  WH_CVR_LINE;

  _connectedNodes[0] = WH_NULL;
  _connectedNodes[1] = WH_NULL;
  _connectedNodes[2] = WH_NULL;

  _connectedPortIds[0] = WH_NO_INDEX;
  _connectedPortIds[1] = WH_NO_INDEX;
  _connectedPortIds[2] = WH_NO_INDEX;
}

WH_CNCT2D_TriangleNode
::~WH_CNCT2D_TriangleNode ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_TriangleNode
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_Node_A::checkInvariant ();

  return true;
}

bool WH_CNCT2D_TriangleNode
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Node_A::assureInvariant ();
  
  return true;
}

WH_Triangle2D WH_CNCT2D_TriangleNode
::triangle () const
{
  return _triangle;
}

void WH_CNCT2D_TriangleNode
::setConnectionAtPort 
(int portId,
 WH_CNCT_Node_A* connectedNode, int connectedPortId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < this->nPorts ());
  WH_ASSERT(connectedNode != WH_NULL);
  WH_ASSERT(0 <= connectedPortId);
  WH_ASSERT(connectedPortId < connectedNode->nPorts ());
#ifndef NDEBUG
  {
    WH_CNCT_Node_A* connectedNode2;
    int connectedPortId2;
    this->getConnectionAtPort 
      (portId,
       connectedNode2, connectedPortId2);
    WH_ASSERT(connectedNode2 == WH_NULL);
  }
#endif

  _connectedNodes[portId] = connectedNode;
  _connectedPortIds[portId] = connectedPortId;
}
  
int WH_CNCT2D_TriangleNode
::nPorts () const
{
  return 3;
}
  
void WH_CNCT2D_TriangleNode
::getConnectionAtPort 
(int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < this->nPorts ());

  connectedNode_OUT = _connectedNodes[portId];
  connectedPortId_OUT = _connectedPortIds[portId];

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(connectedNode_OUT != WH_NULL 
	    || connectedNode_OUT == WH_NULL);
  if (connectedNode_OUT != WH_NULL) {
    WH_ASSERT(0 <= connectedPortId_OUT);
    WH_ASSERT(connectedPortId_OUT < connectedNode_OUT->nPorts ());
  }
#endif
}



/* WH_CNCT2D_TriangleCluster class */

WH_CNCT2D_TriangleCluster
::WH_CNCT2D_TriangleCluster (int id)
  : WH_CNCT_Cluster (id)
{
  WH_CVR_LINE;

  _boundaryConnector = WH_NULL;
}

WH_CNCT2D_TriangleCluster
::~WH_CNCT2D_TriangleCluster ()
{
  WH_CVR_LINE;

  delete _boundaryConnector;
}

bool WH_CNCT2D_TriangleCluster
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Cluster::checkInvariant ();

  return true;
}

bool WH_CNCT2D_TriangleCluster
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Cluster::assureInvariant ();

  vector<WH_Triangle2D> triangle_s;
  this->getTriangles 
    (triangle_s);
  WH_ASSERT(0 < triangle_s.size ());

  return true;
}

void WH_CNCT2D_TriangleCluster
::extractBoundary ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->boundaryConnector () == WH_NULL);

  WH_CVR_LINE;

  _boundaryConnector 
    = this->createBoundaryConnector ();

  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT2D_TriangleNode* node_i 
      = (WH_CNCT2D_TriangleNode*)(*i_node);
    WH_Triangle2D tri = node_i->triangle ();
    
    for (int iPort = 0; iPort < node_i->nPorts (); iPort++) {
      WH_CNCT_Node_A* connectedNode;
      int connectedPortId;
      node_i->getConnectionAtPort 
	(iPort,
	 connectedNode, connectedPortId);
      if (connectedNode == WH_NULL) {
	WH_CVR_LINE;
	/* boundary is found at <iPort> of <node_i> */
	
	WH_CNCT2D_SegmentNode* boundaryNode 
	  = this->createBoundaryNode (tri.edge (iPort));
	_boundaryConnector->addNode (boundaryNode);
      }
    }
  }

  _boundaryConnector->connect ();
  _boundaryConnector->identifyClusters ();
  _boundaryConnector->sortClusters ();
  _boundaryConnector->identifyLoops ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->boundaryConnector () != WH_NULL);
  {
    WH_CNCT2D_SegmentConnector* connector
      = this->boundaryConnector ();
    WH_ASSERT(0 < connector->cluster_s ().size ());
    
    for (vector<WH_CNCT_Cluster*>::const_iterator
	   i_cluster = connector->cluster_s ().begin ();
	 i_cluster != connector->cluster_s ().end ();
	 i_cluster++) {
      WH_CNCT2D_SegmentCluster* cluster_i 
	= (WH_CNCT2D_SegmentCluster*)(*i_cluster);
      
      WH_ASSERT(cluster_i->isSorted ());
      WH_ASSERT(!cluster_i->isSequential ());
      WH_ASSERT(2 < cluster_i->node_s ().size ());
    }
  }
#endif
}

WH_CNCT2D_SegmentConnector* WH_CNCT2D_TriangleCluster
::createBoundaryConnector ()
{
  WH_CNCT2D_SegmentConnector* result 
    = new WH_CNCT2D_SegmentConnector ();
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_CNCT2D_SegmentNode* WH_CNCT2D_TriangleCluster
::createBoundaryNode 
(const WH_Segment2D& segment)
{
  WH_CNCT2D_SegmentNode* result 
    = new WH_CNCT2D_SegmentNode (segment);
  WH_ASSERT(result != WH_NULL);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

WH_CNCT2D_SegmentConnector* WH_CNCT2D_TriangleCluster
::boundaryConnector () const
{
  return _boundaryConnector;
}

WH_CNCT2D_SegmentCluster* WH_CNCT2D_TriangleCluster
::outerLoopBoundaryCluster () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->boundaryConnector () != WH_NULL);
  
  WH_CVR_LINE;

  WH_CNCT2D_SegmentCluster* result = WH_NULL;

  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = this->boundaryConnector ()->cluster_s ().begin ();
       i_cluster != this->boundaryConnector ()->cluster_s ().end ();
       i_cluster++) {
    WH_CNCT2D_SegmentCluster* cluster_i 
      = (WH_CNCT2D_SegmentCluster*)(*i_cluster);
    
    if (cluster_i->isOuterLoop ()) {
      WH_CVR_LINE;
      result = cluster_i;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}

void WH_CNCT2D_TriangleCluster
::getTriangles 
(vector<WH_Triangle2D>& triangle_s_OUT) const
{
  WH_CVR_LINE;

  triangle_s_OUT.clear ();
  
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT2D_TriangleNode* node_i 
      = (WH_CNCT2D_TriangleNode*)(*i_node);
    triangle_s_OUT.push_back (node_i->triangle ());
  }
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(triangle_s_OUT.size () == this->node_s ().size ());
#endif
}



/* WH_CNCT2D_TriangleConnector class */

WH_CNCT2D_TriangleConnector
::WH_CNCT2D_TriangleConnector ()
{
  WH_CVR_LINE;
}

WH_CNCT2D_TriangleConnector
::~WH_CNCT2D_TriangleConnector ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_TriangleConnector
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Connector_A::checkInvariant ();

  return true;
}

bool WH_CNCT2D_TriangleConnector
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_Connector_A::assureInvariant ();
  
  return true;
}

void WH_CNCT2D_TriangleConnector
::extractBoundary ()
{
  WH_CVR_LINE;

  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = _cluster_s.begin ();
       i_cluster != _cluster_s.end ();
       i_cluster++) {
    WH_CNCT2D_TriangleCluster* cluster_i 
      = (WH_CNCT2D_TriangleCluster*)(*i_cluster);

    cluster_i->extractBoundary ();
  }
}

WH_CNCT_Cluster* WH_CNCT2D_TriangleConnector
::createCluster (int clusterId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= clusterId);
  
  WH_CNCT_Cluster* result 
    = new WH_CNCT2D_TriangleCluster (clusterId);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_CNCT2D_TriangleConnector
::findConnection 
(WH_CNCT_Node_A* node, int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < node->nPorts ());

  WH_CVR_LINE;

  connectedNode_OUT = WH_NULL;
  connectedPortId_OUT = WH_NO_INDEX;

  /* search over all the triangle nodes other than <node>, and find a
     triangle node connected at either edge with <node> at the edge of
     <portID> */

  /*
    portId i -> triangle.edge (i)
  */

  WH_CNCT2D_TriangleNode* triangleNode = (WH_CNCT2D_TriangleNode*)node;
  WH_Triangle2D tri = triangleNode->triangle ();
  WH_Segment2D seg = tri.edge (portId);
  
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    if (node_i == node) continue;
    
    WH_CNCT2D_TriangleNode* triangleNode_i 
      = (WH_CNCT2D_TriangleNode*)node_i;
    WH_Triangle2D tri_i = triangleNode_i->triangle ();

    bool found = false;
    for (int iEdge = 0; iEdge < tri_i.nEdges (); iEdge++) {
#if 1   /* optimized version */
      WH_Vector2D v0 = tri_i.vertex (iEdge);
      WH_Vector2D v1 = tri_i.vertex ((iEdge + 1) % 3);
      if (seg.hasVertexAt (v0) && seg.hasVertexAt (v1)) {
	WH_CVR_LINE;
	connectedNode_OUT = node_i;
	connectedPortId_OUT = iEdge;
	found = true;
	break;
      }
#else   /* original version */
      WH_Segment2D seg_i = tri_i.edge (iEdge);
      if (WH_eq (seg_i, seg) || WH_isReverse (seg_i, seg)) {
	WH_CVR_LINE;
	connectedNode_OUT = node_i;
	connectedPortId_OUT = iEdge;
	found = true;
	break;
      }
#endif
    }
    if (found) {
      WH_CVR_LINE;
      break;
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(connectedNode_OUT != WH_NULL 
	    || connectedNode_OUT == WH_NULL);
  if (connectedNode_OUT != WH_NULL) {
    WH_CVR_LINE;
    WH_ASSERT(0 <= connectedPortId_OUT);
    WH_ASSERT(connectedPortId_OUT < connectedNode_OUT->nPorts ());

    /* check if there is only one port */
    int count = 0;

    WH_CNCT2D_TriangleNode* triangleNode = (WH_CNCT2D_TriangleNode*)node;
    WH_Triangle2D tri = triangleNode->triangle ();
    WH_Segment2D seg = tri.edge (portId);

    for (vector<WH_CNCT_Node_A*>::const_iterator
	   i_node = this->node_s ().begin ();
	 i_node != this->node_s ().end ();
	 i_node++) {
      WH_CNCT_Node_A* node_i = (*i_node);
      if (node_i == node) continue;
      
      WH_CNCT2D_TriangleNode* triangleNode_i 
	= (WH_CNCT2D_TriangleNode*)node_i;
      WH_Triangle2D tri_i = triangleNode_i->triangle ();
      for (int iEdge = 0; iEdge < tri_i.nEdges (); iEdge++) {
	WH_Segment2D seg_i = tri_i.edge (iEdge);
	if (WH_eq (seg_i, seg) || WH_isReverse (seg_i, seg)) {
	  count++;
	}
      }
    }

    WH_ASSERT(count == 1);
  }
#endif
}



/* class WH_CNCT2D_NonRegularTriangleCluster */

WH_CNCT2D_NonRegularTriangleCluster
::WH_CNCT2D_NonRegularTriangleCluster (int id)
  : WH_CNCT2D_TriangleCluster (id)
{
  WH_CVR_LINE;
}

WH_CNCT2D_NonRegularTriangleCluster
::~WH_CNCT2D_NonRegularTriangleCluster ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_NonRegularTriangleCluster
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT2D_TriangleCluster::checkInvariant ();

  return true;
}

bool WH_CNCT2D_NonRegularTriangleCluster
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT2D_TriangleCluster::assureInvariant ();

  return true;
}

void WH_CNCT2D_NonRegularTriangleCluster
::extractBoundary ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->boundaryConnector () == WH_NULL);

  WH_CVR_LINE;

  _boundaryConnector 
    = this->createBoundaryConnector ();

  vector<WH_Segment2D> seg_s;
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT2D_TriangleNode* node_i 
      = (WH_CNCT2D_TriangleNode*)(*i_node);
    WH_Triangle2D tri = node_i->triangle ();
    
    for (int iPort = 0; iPort < node_i->nPorts (); iPort++) {
      WH_CNCT_Node_A* connectedNode;
      int connectedPortId;
      node_i->getConnectionAtPort 
	(iPort,
	 connectedNode, connectedPortId);
      if (connectedNode == WH_NULL) {
	WH_CVR_LINE;
	/* boundary is found at <iPort> of <node_i> */

	seg_s.push_back (tri.edge (iPort));
      }
    }
  }

  /* remove duplicated segments, because they are along internal
     boundary */
  vector<WH_Segment2D> remainingSeg_s;
  for (vector<WH_Segment2D>::const_iterator
	 i_seg = seg_s.begin ();
       i_seg != seg_s.end ();
       i_seg++) {
    WH_Segment2D seg_i = (*i_seg);

    bool duplicationIsFound = false;
    for (vector<WH_Segment2D>::const_iterator
	   j_seg = seg_s.begin ();
	 j_seg != seg_s.end ();
	 j_seg++) {
      if (i_seg == j_seg) continue;
      WH_Segment2D seg_j = (*j_seg);
      if (WH_eq (seg_i, seg_j) || WH_isReverse (seg_i, seg_j)) {
	WH_CVR_LINE;
	duplicationIsFound = true;
	break;
      }
    }
    
    if (!duplicationIsFound) {
      WH_CVR_LINE;
      remainingSeg_s.push_back (seg_i);
    }
  }

  for (vector<WH_Segment2D>::const_iterator
	 i_seg = remainingSeg_s.begin ();
       i_seg != remainingSeg_s.end ();
       i_seg++) {
    WH_Segment2D seg_i = (*i_seg);
    WH_CNCT2D_SegmentNode* boundaryNode 
      = this->createBoundaryNode (seg_i);
    _boundaryConnector->addNode (boundaryNode);
  }

  _boundaryConnector->connect ();
  _boundaryConnector->identifyClusters ();
  _boundaryConnector->sortClusters ();
  _boundaryConnector->identifyLoops ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->boundaryConnector () != WH_NULL);
  {
    WH_CNCT2D_SegmentConnector* connector
      = this->boundaryConnector ();
    WH_ASSERT(0 < connector->cluster_s ().size ());
    
    for (vector<WH_CNCT_Cluster*>::const_iterator
	   i_cluster = connector->cluster_s ().begin ();
	 i_cluster != connector->cluster_s ().end ();
	 i_cluster++) {
      WH_CNCT2D_SegmentCluster* cluster_i 
	= (WH_CNCT2D_SegmentCluster*)(*i_cluster);
      
      WH_ASSERT(cluster_i->isSorted ());
      WH_ASSERT(!cluster_i->isSequential ());
      WH_ASSERT(2 < cluster_i->node_s ().size ());
    }
  }
#endif
}



/* class WH_CNCT2D_NonRegularTriangleConnector */

WH_CNCT2D_NonRegularTriangleConnector
::WH_CNCT2D_NonRegularTriangleConnector ()
{
  WH_CVR_LINE;
}

WH_CNCT2D_NonRegularTriangleConnector
::~WH_CNCT2D_NonRegularTriangleConnector ()
{
  WH_CVR_LINE;
}

bool WH_CNCT2D_NonRegularTriangleConnector
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT2D_TriangleConnector::checkInvariant ();

  return true;
}

bool WH_CNCT2D_NonRegularTriangleConnector
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT2D_TriangleConnector::assureInvariant ();
  
  return true;
}

void WH_CNCT2D_NonRegularTriangleConnector
::addInternalBoundarySegment (const WH_Segment2D& segment)
{
  _internalBoundarySegment_s.push_back (segment);
}

const vector<WH_Segment2D>& WH_CNCT2D_NonRegularTriangleConnector
::internalBoundarySegment_s () const
{
  return _internalBoundarySegment_s;
}

WH_CNCT_Cluster* WH_CNCT2D_NonRegularTriangleConnector
::createCluster (int clusterId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= clusterId);
  
  WH_CNCT_Cluster* result 
    = new WH_CNCT2D_NonRegularTriangleCluster (clusterId);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

void WH_CNCT2D_NonRegularTriangleConnector
::findConnection 
(WH_CNCT_Node_A* node, int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < node->nPorts ());

  WH_CVR_LINE;

  connectedNode_OUT = WH_NULL;
  connectedPortId_OUT = WH_NO_INDEX;

  /* search over all the triangle nodes other than <node>, and find a
     triangle node connected at either edge with <node> at the edge of
     <portId> */

  /*
    portId i -> triangle.edge (i)
  */

  WH_CNCT2D_TriangleNode* triangleNode = (WH_CNCT2D_TriangleNode*)node;
  WH_Triangle2D tri = triangleNode->triangle ();
  WH_Segment2D seg = tri.edge (portId);
  
  /* If the edge segment <seg> of <portId> is along any internal
     boundary segment, stop searching. */

  bool internalBoundaryIsFound = false;
  for (vector<WH_Segment2D>::const_iterator
	 i_seg = this->internalBoundarySegment_s ().begin ();
       i_seg != this->internalBoundarySegment_s ().end ();
       i_seg++) {
    WH_Segment2D seg_i = (*i_seg);
    
    WH_SegmentIntersector2D intersector (seg_i, seg);
    intersector.perform ();
    if (intersector.hasIntersectionSegment ()) {
      WH_CVR_LINE;
      WH_ASSERT(WH_eq (intersector.intersectionSegment (), seg)
		|| WH_isReverse (intersector.intersectionSegment (), seg));
      internalBoundaryIsFound = true;
      break;
    }
  }

  if (!internalBoundaryIsFound) {
    WH_CVR_LINE;

    this->WH_CNCT2D_TriangleConnector::findConnection 
      (node, portId,
       connectedNode_OUT, connectedPortId_OUT);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(connectedNode_OUT != WH_NULL 
	    || connectedNode_OUT == WH_NULL);
  if (connectedNode_OUT != WH_NULL) {
    WH_CVR_LINE;
    WH_ASSERT(0 <= connectedPortId_OUT);
    WH_ASSERT(connectedPortId_OUT < connectedNode_OUT->nPorts ());
  }
#endif
}



/* not yet covered 
void WH_CNCT2D_SegmentCluster
::getRegularizedSegments 
void WH_CNCT2D_SegmentCluster
::getVertexsFromSegments 

class WH_CNCT2D_NonRegularTriangleCluster
class WH_CNCT2D_NonRegularTriangleConnector
*/


