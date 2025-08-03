/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* inline functions of afront2d.cc */

/* class WH_AF2D_Vertex_A */

WH_INLINE WH_Vector2D WH_AF2D_Vertex_A
::position () const
{
  return _position;
}



/* class WH_AF2D_Edge_A */

WH_INLINE WH_AF2D_Vertex_A* WH_AF2D_Edge_A
::vertex0 () const
{
  return _vertex0;
}

WH_INLINE WH_AF2D_Vertex_A* WH_AF2D_Edge_A
::vertex1 () const
{
  return _vertex1;
}

WH_INLINE WH_Vector2D WH_AF2D_Edge_A
::endPoint0 () const
{
  return this->vertex0 ()->position ();
}

WH_INLINE WH_Vector2D WH_AF2D_Edge_A
::endPoint1 () const
{
  return this->vertex1 ()->position ();
}



/* class WH_AF2D_Triangle_A */

WH_INLINE WH_AF2D_Vertex_A* WH_AF2D_Triangle_A
::vertex0 () const
{
  return _vertex0;
}

WH_INLINE WH_AF2D_Vertex_A* WH_AF2D_Triangle_A
::vertex1 () const
{
  return _vertex1;
}

WH_INLINE WH_AF2D_Vertex_A* WH_AF2D_Triangle_A
::vertex2 () const
{
  return _vertex2;
}

