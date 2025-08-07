/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* connector.cc : connector framework */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "connector.h"



/* WH_CNCT_Node_A class */

WH_CNCT_Node_A
::WH_CNCT_Node_A ()
{
  WH_CVR_LINE;

  _id = WH_NO_INDEX;
  _cluster = WH_NULL;
}

WH_CNCT_Node_A
::~WH_CNCT_Node_A ()
{
  WH_CVR_LINE;
}

bool WH_CNCT_Node_A
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(0 < this->nPorts ());

  return true;
}

bool WH_CNCT_Node_A
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->checkInvariant ();
  
  for (int iPort = 0; iPort < this->nPorts (); iPort++) {
    WH_CNCT_Node_A* connectedNode;
    int connectedPortId;
    this->getConnectionAtPort 
      (iPort,
       connectedNode, connectedPortId);
    if (connectedNode != WH_NULL) {
      WH_CVR_LINE;

      WH_CNCT_Node_A* connectedNode2;
      int connectedPortId2;
      connectedNode->getConnectionAtPort 
	(connectedPortId,
	 connectedNode2, connectedPortId2);
      WH_ASSERT(connectedNode2 == this);
      WH_ASSERT(connectedPortId2 == iPort);
    }
  }

  return true;
}

void WH_CNCT_Node_A
::setId (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->id () == WH_NO_INDEX);
  WH_ASSERT(0 <= id);
  
  _id = id;
}

void WH_CNCT_Node_A
::setCluster (WH_CNCT_Cluster* cluster)
{
  /* PRE-CONDITION */
  WH_ASSERT(this->cluster () == WH_NULL);
  WH_ASSERT(cluster != WH_NULL);
  
  _cluster = cluster;
}
  
void WH_CNCT_Node_A
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

  /* PURE VIRTUAL */
}
  
int WH_CNCT_Node_A
::id () const
{
  return _id;
}
  
WH_CNCT_Cluster* WH_CNCT_Node_A
::cluster () const
{
  return _cluster;
}
  
void WH_CNCT_Node_A
::getConnectionAtPort 
(int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT) const
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < this->nPorts ());

  /* PURE VIRTUAL */

  connectedNode_OUT = WH_NULL;
  connectedPortId_OUT = WH_NO_INDEX;

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



/* WH_CNCT_Cluster class */

WH_CNCT_Cluster
::WH_CNCT_Cluster (int id)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= id);

  WH_CVR_LINE;

  _id = id;
}

WH_CNCT_Cluster
::~WH_CNCT_Cluster ()
{
  WH_CVR_LINE;
}

bool WH_CNCT_Cluster
::checkInvariant () const
{
  WH_CVR_LINE;

  WH_ASSERT(0 <= this->id ());

  return true;
}

bool WH_CNCT_Cluster
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->checkInvariant ();
  
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    WH_ASSERT(node_i->cluster () == this);
  }

  return true;
}

void WH_CNCT_Cluster
::addNode (WH_CNCT_Node_A* node)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(node->cluster () == this);
  
  _node_s.push_back (node);
}

int WH_CNCT_Cluster
::id () const
{
  return _id;
}

const vector<WH_CNCT_Node_A*>& WH_CNCT_Cluster
::node_s () const
{
  return _node_s;
}



/* WH_CNCT_Connector_A class */

WH_CNCT_Connector_A
::WH_CNCT_Connector_A ()
{
  WH_CVR_LINE;
}

WH_CNCT_Connector_A
::~WH_CNCT_Connector_A ()
{
  WH_CVR_LINE;

  WH_T_Delete (_cluster_s);
  WH_T_Delete (_node_s);
}

bool WH_CNCT_Connector_A
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_CNCT_Connector_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  WH_T_AssureInvariant (this->node_s ());
  WH_T_AssureInvariant (this->cluster_s ());

  int nodeCount = 0;
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    WH_ASSERT(node_i->id () == nodeCount);
    nodeCount++;
  }
  
  int clusterCount = 0;
  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = this->cluster_s ().begin ();
       i_cluster != this->cluster_s ().end ();
       i_cluster++) {
    WH_CNCT_Cluster* cluster_i = (*i_cluster);
    WH_ASSERT(cluster_i->id () == clusterCount);
    clusterCount++;
  }
  
  return true;
}

WH_CNCT_Cluster* WH_CNCT_Connector_A
::createCluster (int clusterId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= clusterId);
  
  WH_CNCT_Cluster* result 
    = new WH_CNCT_Cluster (clusterId);
  
  /* POST-CONDITION */
  WH_ASSERT(result != WH_NULL);
  
  return result;
}

void WH_CNCT_Connector_A
::findConnection 
(WH_CNCT_Node_A* node, int portId,
 WH_CNCT_Node_A*& connectedNode_OUT, 
 int& connectedPortId_OUT)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(0 <= portId);
  WH_ASSERT(portId < node->nPorts ());

  /* PURE VIRTUAL */

  connectedNode_OUT = WH_NULL;
  connectedPortId_OUT = WH_NO_INDEX;

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

void WH_CNCT_Connector_A
::addNode (WH_CNCT_Node_A* node)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(node->id () == WH_NO_INDEX);
  
  int nodeId = (int)_node_s.size ();
  node->setId (nodeId);
  _node_s.push_back (node);

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(node->id () == (int)this->node_s ().size () - 1);
#endif
}

void WH_CNCT_Connector_A
::connect ()
{
  WH_CVR_LINE;
  
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);

    for (int iPort = 0; iPort < node_i->nPorts (); iPort++) {
      WH_CNCT_Node_A* connectedNode;
      int connectedPortId;
      node_i->getConnectionAtPort 
	(iPort,
	 connectedNode, connectedPortId);
      if (connectedNode == WH_NULL) {
	WH_CVR_LINE;
	this->findConnection 
	  (node_i, iPort,
	   connectedNode, connectedPortId);
	if (connectedNode != WH_NULL) {
	  WH_CVR_LINE;
	  node_i->setConnectionAtPort
	    (iPort,
	     connectedNode, connectedPortId);
	  connectedNode->setConnectionAtPort
	    (connectedPortId,
	     node_i, iPort);
	}
      }
    }
  }
}
  
void WH_CNCT_Connector_A
::setClusterOnNode (WH_CNCT_Node_A* node, WH_CNCT_Cluster* cluster)
{
  /* PRE-CONDITION */
  WH_ASSERT(node != WH_NULL);
  WH_ASSERT(node->cluster () == WH_NULL);
  WH_ASSERT(cluster != WH_NULL);
  
  WH_CVR_LINE;

  node->setCluster (cluster);
  cluster->addNode (node);

  for (int iPort = 0; iPort < node->nPorts (); iPort++) {
    WH_CNCT_Node_A* connectedNode;
    int connectedPortId;
    node->getConnectionAtPort 
      (iPort,
       connectedNode, connectedPortId);
    if (connectedNode != WH_NULL) {
      WH_CVR_LINE;

      if (connectedNode->cluster () == WH_NULL) {
	WH_CVR_LINE;
	/* recursive call */
	this->setClusterOnNode (connectedNode, cluster);
      }
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(node->cluster () != WH_NULL);
#endif
}

void WH_CNCT_Connector_A
::identifyClusters ()
{
  /* PRE-CONDITION */
  WH_ASSERT(this->cluster_s ().size () == 0);
#ifndef NDEBUG
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    WH_ASSERT(node_i->cluster () == WH_NULL);
  }
#endif  

  WH_CVR_LINE;

  /* mark over all the node with cluster using recursive call to
     <setClusterOnNode ()> */
  {
    vector<WH_CNCT_Node_A*>::const_iterator
      i_node = this->node_s ().begin ();
    for (;;) {
      
      /* find a node which does not have cluster yet */
      WH_CNCT_Node_A* nodeWithoutCluster = WH_NULL;
      for (;
	   i_node != this->node_s ().end ();
	   i_node++) {
	WH_CNCT_Node_A* node_i = (*i_node);
	if (node_i->cluster () == WH_NULL) {
	  WH_CVR_LINE;
	  nodeWithoutCluster = node_i;
	  break;
	}
      }
      if (nodeWithoutCluster == WH_NULL) {
	WH_CVR_LINE;
	/* All the nodes are marked with cluster */
	break;
      }
      
      /* create a new cluster */
      int clusterId = (int)_cluster_s.size ();
      WH_CNCT_Cluster* cluster = this->createCluster (clusterId);
      _cluster_s.push_back (cluster);
      
      /* start of recursive call */
      this->setClusterOnNode 
	(nodeWithoutCluster, cluster);
    }
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    WH_ASSERT(node_i->cluster () != WH_NULL);
  }
#endif
}
  
const vector<WH_CNCT_Node_A*>& WH_CNCT_Connector_A
::node_s () const
{
  return _node_s;
}

const vector<WH_CNCT_Cluster*>& WH_CNCT_Connector_A
::cluster_s () const
{
  return _cluster_s;
}



/* WH_CNCT_ListNode class */

WH_CNCT_ListNode
::WH_CNCT_ListNode ()
{
  WH_CVR_LINE;

  _connectedNodes[0] = WH_NULL;
  _connectedNodes[1] = WH_NULL;

  _connectedPortIds[0] = WH_NO_INDEX;
  _connectedPortIds[1] = WH_NO_INDEX;
}

WH_CNCT_ListNode
::~WH_CNCT_ListNode ()
{
  WH_CVR_LINE;
}

bool WH_CNCT_ListNode
::checkInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_Node_A::checkInvariant ();

  return true;
}

bool WH_CNCT_ListNode
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Node_A::assureInvariant ();
  
  return true;
}

void WH_CNCT_ListNode
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

  _connectedNodes[portId] = (WH_CNCT_ListNode*)connectedNode;
  _connectedPortIds[portId] = connectedPortId;
}
  
int WH_CNCT_ListNode
::nPorts () const
{
  return 2;
}
  
void WH_CNCT_ListNode
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



/* WH_CNCT_ListCluster class */

WH_CNCT_ListCluster
::WH_CNCT_ListCluster (int id)
  : WH_CNCT_Cluster (id)
{
  WH_CVR_LINE;

  _isSorted = false;
  _isSequential = false;
}

WH_CNCT_ListCluster
::~WH_CNCT_ListCluster ()
{
  WH_CVR_LINE;
}

bool WH_CNCT_ListCluster
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Cluster::checkInvariant ();

  return true;
}

bool WH_CNCT_ListCluster
::assureInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Cluster::assureInvariant ();

  for (vector<WH_CNCT_Node_A*>::const_iterator
	 i_node = this->node_s ().begin ();
       i_node != this->node_s ().end ();
       i_node++) {
    WH_CNCT_Node_A* node_i = (*i_node);
    WH_ASSERT(node_i->nPorts () == 2);
  }

  if (this->isSorted ()) {
    WH_CVR_LINE;
    
    WH_ASSERT(0 < this->node_s ().size ());

    const vector<WH_CNCT_Node_A*>& node_s = this->node_s ();
    int nNodes = (int)node_s.size ();
    for (int iNode = 0; iNode < nNodes; iNode++) {
      WH_CNCT_Node_A* node = node_s[iNode];
      
      WH_CNCT_Node_A* prevNode = WH_NULL;
      if (0 < iNode) {
        prevNode = node_s[iNode - 1];
      } else {
	if (this->isSequential ()) {
	  prevNode = WH_NULL;
	} else {  /* cyclic list */
	  prevNode = node_s[nNodes - 1];
	}
      }
      WH_CNCT_Node_A* nextNode = WH_NULL;
      if (iNode < nNodes - 1) {
        nextNode = node_s[iNode + 1];
      } else {
	if (this->isSequential ()) {
	  nextNode = WH_NULL;
	} else {  /* cyclic list */
	  nextNode = node_s[0];
	}
      }

      WH_CNCT_Node_A* connectedNode0;
      int connectedPortId0;
      node->getConnectionAtPort 
	(0,
	 connectedNode0, connectedPortId0);
      
      WH_CNCT_Node_A* connectedNode1;
      int connectedPortId1;
      node->getConnectionAtPort 
	(1,
	 connectedNode1, connectedPortId1);
      
      WH_ASSERT((connectedNode0 == prevNode
		 && connectedNode1 == nextNode)
		|| (connectedNode1 == prevNode
		    && connectedNode0 == nextNode));
    }
  }

  return true;
}

void WH_CNCT_ListCluster
::sort ()
{
  /* PRE-CONDITION */
  WH_ASSERT(!this->isSorted ());

  if (0 < _node_s.size ()) {
    WH_CVR_LINE;

    WH_CNCT_Node_A* firstNode = _node_s[0];
    
    _isSequential = true;

    list<WH_CNCT_Node_A*> sortedNode_s;
    sortedNode_s.push_back (firstNode);

    /* search forward of <firstNode>, <nextPortId> == 1 */
    WH_CNCT_Node_A* curNode = firstNode;
    int nextPortId = 1;
    for (;;) {
      WH_ASSERT(curNode != WH_NULL);

      WH_CNCT_Node_A* connectedNode;
      int connectedPortId;
      curNode->getConnectionAtPort 
	(nextPortId,
	 connectedNode, connectedPortId);
      if (connectedNode == WH_NULL) {
	WH_CVR_LINE;
	/* the cluster is sequential */

	break;
      } else if (connectedNode == firstNode) {
	WH_CVR_LINE;
	/* the cluster is cyclic */

	_isSequential = false;
	break;
      }
      WH_CNCT_Node_A* nextNode = connectedNode;
      nextPortId = (connectedPortId == 0) ? 1 : 0;
      sortedNode_s.push_back (nextNode);

      curNode = nextNode;
    }

    if (_isSequential) {
      WH_CVR_LINE;

      /* search backward of <firstNode>, <nextPortId> == 0 */
      curNode = firstNode;
      nextPortId = 0;
      for (;;) {
	WH_ASSERT(curNode != WH_NULL);
	
	WH_CNCT_Node_A* connectedNode;
	int connectedPortId;
	curNode->getConnectionAtPort 
	  (nextPortId,
	   connectedNode, connectedPortId);
	if (connectedNode == WH_NULL) {
	  WH_CVR_LINE;
	  break;
	} else {
	  WH_ASSERT(connectedNode != firstNode);
	}
	WH_CNCT_Node_A* nextNode = connectedNode;
	WH_ASSERT(nextNode != WH_NULL);
	nextPortId = (connectedPortId == 0) ? 1 : 0;
	sortedNode_s.push_front (nextNode);
	
	curNode = nextNode;
      }
    }

    WH_ASSERT(sortedNode_s.size () == _node_s.size ());
    WH_T_Move (sortedNode_s, _node_s);
  }

  _isSorted = true;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(this->isSorted ());
#endif
}

bool WH_CNCT_ListCluster
::isSorted () const
{
  return _isSorted;
}

bool WH_CNCT_ListCluster
::isSequential () const
{
  /* PRE-CONDITION */
  WH_ASSERT(this->isSorted ());

  return _isSequential;
}



/* WH_CNCT_ListConnector_A class */

WH_CNCT_ListConnector_A
::WH_CNCT_ListConnector_A ()
{
  WH_CVR_LINE;
}

WH_CNCT_ListConnector_A
::~WH_CNCT_ListConnector_A ()
{
  WH_CVR_LINE;
}

bool WH_CNCT_ListConnector_A
::checkInvariant () const
{
  WH_CVR_LINE;
  
  this->WH_CNCT_Connector_A::checkInvariant ();

  return true;
}

bool WH_CNCT_ListConnector_A
::assureInvariant () const
{
  WH_CVR_LINE;

  this->WH_CNCT_Connector_A::assureInvariant ();
  
  return true;
}

WH_CNCT_Cluster* WH_CNCT_ListConnector_A
::createCluster (int clusterId)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= clusterId);
  
  WH_CNCT_Cluster* result 
    = new WH_CNCT_ListCluster (clusterId);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
  WH_ASSERT(result != WH_NULL);
  
  return result;
}

void WH_CNCT_ListConnector_A
::sortClusters ()
{
  WH_CVR_LINE;

  for (vector<WH_CNCT_Cluster*>::const_iterator
	 i_cluster = this->cluster_s ().begin ();
       i_cluster != this->cluster_s ().end ();
       i_cluster++) {
    WH_CNCT_ListCluster* cluster_i 
      = (WH_CNCT_ListCluster*)(*i_cluster);
    cluster_i->sort ();
  }
}



/* test coverage completed */







