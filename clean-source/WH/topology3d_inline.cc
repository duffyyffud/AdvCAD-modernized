/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of topology3d.cc */



/* class WH_TPL3D_Vertex_A */

WH_INLINE WH_TPL3D_Body_A* WH_TPL3D_Vertex_A
::body () const
{
  return _body;
}

WH_INLINE const vector<WH_TPL3D_Face_A*>& WH_TPL3D_Vertex_A
::connectedFace_s () const
{
  return _connectedFace_s;
}

WH_INLINE const vector<WH_TPL3D_Edge_A*>& WH_TPL3D_Vertex_A
::connectedEdge_s () const
{
  return _connectedEdge_s;
}



/* class WH_TPL3D_Curve_A */



/* class WH_TPL3D_Edge_A */

WH_INLINE WH_TPL3D_Body_A* WH_TPL3D_Edge_A
::body () const
{
  return _body;
}

WH_INLINE WH_TPL3D_Vertex_A* WH_TPL3D_Edge_A
::vertex0 () const
{
  return _vertex0;
}

WH_INLINE WH_TPL3D_Vertex_A* WH_TPL3D_Edge_A
::vertex1 () const
{
  return _vertex1;
}

WH_INLINE const vector<WH_TPL3D_Face_A*>& WH_TPL3D_Edge_A
::connectedFace_s () const
{
  return _connectedFace_s;
}

WH_INLINE WH_TPL3D_Curve_A* WH_TPL3D_Edge_A
::curve () const
{
  return _curve;
}



/* class WH_TPL3D_LoopVertexUse_A */

WH_INLINE WH_TPL3D_Loop_A* WH_TPL3D_LoopVertexUse_A
::loop () const
{
  return _loop;
}

WH_INLINE WH_TPL3D_Vertex_A* WH_TPL3D_LoopVertexUse_A
::vertex () const
{
  return _vertex;
}



/* class WH_TPL3D_Curve2D_A */



/* class WH_TPL3D_LoopEdgeUse_A */

WH_INLINE WH_TPL3D_Loop_A* WH_TPL3D_LoopEdgeUse_A
::loop () const
{
  return _loop;
}

WH_INLINE WH_TPL3D_Edge_A* WH_TPL3D_LoopEdgeUse_A
::edge () const
{
  return _edge;
}

WH_INLINE WH_TPL3D_Curve2D_A* WH_TPL3D_LoopEdgeUse_A
::curve () const
{
  return _curve;
}



/* class WH_TPL3D_Loop_A */

WH_INLINE WH_TPL3D_Face_A* WH_TPL3D_Loop_A
::face () const
{
  return _face;
}

WH_INLINE const vector<WH_TPL3D_LoopVertexUse_A*>& WH_TPL3D_Loop_A
::vertexUse_s () const
{
  return _vertexUse_s;
}

WH_INLINE const vector<WH_TPL3D_LoopEdgeUse_A*>& WH_TPL3D_Loop_A
::edgeUse_s () const
{
  return _edgeUse_s;
}



/* class WH_TPL3D_Surface_A */



/* class WH_TPL3D_Face_A */

WH_INLINE WH_TPL3D_Body_A* WH_TPL3D_Face_A
::body () const
{
  return _body;
}

WH_INLINE WH_TPL3D_Loop_A* WH_TPL3D_Face_A
::outerLoop () const
{
  return _outerLoop;
}

WH_INLINE const vector<WH_TPL3D_Loop_A*>& WH_TPL3D_Face_A
::innerLoop_s () const
{
  return _innerLoop_s;
}

WH_INLINE const vector<WH_TPL3D_Loop_A*>& WH_TPL3D_Face_A
::otherLoop_s () const
{
  return _otherLoop_s;
}

WH_INLINE WH_TPL3D_Surface_A* WH_TPL3D_Face_A
::surface () const
{
  return _surface;
}



/* class WH_TPL3D_Volume_A */

WH_INLINE WH_TPL3D_Body_A* WH_TPL3D_Volume_A
::body () const
{
  return _body;
}

WH_INLINE const vector<WH_TPL3D_Face_A*>& WH_TPL3D_Volume_A
::face_s () const
{
  return _face_s;
}



/* class WH_TPL3D_Body_A */

WH_INLINE const vector<WH_TPL3D_Vertex_A*>& WH_TPL3D_Body_A
::vertex_s () const
{
  return _vertex_s;
}

WH_INLINE const vector<WH_TPL3D_Edge_A*>& WH_TPL3D_Body_A
::edge_s () const
{
  return _edge_s;
}

WH_INLINE const vector<WH_TPL3D_Face_A*>& WH_TPL3D_Body_A
::face_s () const
{
  return _face_s;
}

WH_INLINE const vector<WH_TPL3D_Volume_A*>& WH_TPL3D_Body_A
::volume_s () const
{
  return _volume_s;
}

