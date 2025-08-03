/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for connector.cc */

#pragma once
#ifndef WH_INCLUDED_WH_SCALAR
#include <WH/scalar.h>
#define WH_INCLUDED_WH_SCALAR
#endif

class WH_CNCT_Node_A;
class WH_CNCT_Cluster;
class WH_CNCT_Connector_A;

class WH_CNCT_ListNode;
class WH_CNCT_ListCluster;
class WH_CNCT_ListConnector_A;

/* connector framework classes */

class WH_CNCT_Node_A {
 public:
  WH_CNCT_Node_A ();
  virtual ~WH_CNCT_Node_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;
  
  /* 
     A node has 0-* ports.
     A port connects to 0-1 ports.
     These ports are connected each other.
  */

  /* base */
  virtual void setId (int id);

  virtual void setCluster (WH_CNCT_Cluster* cluster);
  
  virtual void setConnectionAtPort 
    (int portId,
     WH_CNCT_Node_A* connectedNode, int connectedPortId) = 0;
  
  int id () const;
  
  WH_CNCT_Cluster* cluster () const;
  
  virtual int nPorts () const = 0;

  virtual void getConnectionAtPort 
    (int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT) const = 0;
  
  /* derived */

 protected:
  int _id;

  WH_CNCT_Cluster* _cluster;

  /* base */

  /* derived */

};

class WH_CNCT_Cluster {
 public:
  WH_CNCT_Cluster (int id);
  virtual ~WH_CNCT_Cluster ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addNode (WH_CNCT_Node_A* node);

  int id () const;
  
  const vector<WH_CNCT_Node_A*>& node_s () const;

  /* derived */
  
 protected:
  int _id;
  
  vector<WH_CNCT_Node_A*> _node_s;

  /* base */

  /* derived */

};

class WH_CNCT_Connector_A {
 public:
  WH_CNCT_Connector_A ();
  virtual ~WH_CNCT_Connector_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void addNode (WH_CNCT_Node_A* node  /* ADOPT */);

  virtual void connect ();
  /* template method */
  /* connect nodes each other */

  virtual void identifyClusters ();
  /* template method */
  /* gather nodes connected each other as clusters */
  
  const vector<WH_CNCT_Node_A*>& node_s () const;
  
  const vector<WH_CNCT_Cluster*>& cluster_s () const;

  /* derived */
  
 protected:
  vector<WH_CNCT_Node_A*> _node_s;  /* own */

  vector<WH_CNCT_Cluster*> _cluster_s;   /* own */

  /* base */

  /* factory methods */
  virtual WH_CNCT_Cluster* createCluster (int clusterId);

  virtual void findConnection 
    (WH_CNCT_Node_A* node, int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT) = 0;
  /* hook method */

  virtual void setClusterOnNode 
    (WH_CNCT_Node_A* node, WH_CNCT_Cluster* cluster);

  /* derived */

};

/* list connector framework classes */

class WH_CNCT_ListNode : public WH_CNCT_Node_A {
 public:
  WH_CNCT_ListNode ();
  virtual ~WH_CNCT_ListNode ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */

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
  WH_CNCT_ListNode* _connectedNodes[2];

  int _connectedPortIds[2];

  /* base */

  /* derived */

};

class WH_CNCT_ListCluster : public WH_CNCT_Cluster {
 public:
  WH_CNCT_ListCluster (int id);
  virtual ~WH_CNCT_ListCluster ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void sort ();

  bool isSorted () const;

  bool isSequential () const;

  /* derived */
  
 protected:
  bool _isSorted;

  bool _isSequential;

  /* base */

  /* derived */

};

class WH_CNCT_ListConnector_A : public WH_CNCT_Connector_A {
 public:
  WH_CNCT_ListConnector_A ();
  virtual ~WH_CNCT_ListConnector_A ();
  virtual bool checkInvariant () const;
  virtual bool assureInvariant () const;

  /* base */
  virtual void sortClusters ();

  /* derived */
  
 protected:

  /* base */

  /* derived */
  virtual WH_CNCT_Cluster* createCluster (int clusterId);
  
  virtual void findConnection 
    (WH_CNCT_Node_A* node, int portId,
     WH_CNCT_Node_A*& connectedNode_OUT, 
     int& connectedPortId_OUT) = 0;

};
