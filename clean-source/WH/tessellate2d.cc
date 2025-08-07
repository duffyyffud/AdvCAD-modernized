/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* tessellate2d.cc */
/* tesselation of 2-D polygonal region with holes into triangles */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "tessellate2d.h"



/* class WH_AF2D_Vertex_TSLT */

WH_AF2D_Vertex_TSLT
::WH_AF2D_Vertex_TSLT 
(const WH_Vector2D& position,
 int loopId, 
 int loopVertexId) : WH_AF2D_Vertex_A (position)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 <= loopId);
  WH_ASSERT(0 <= loopVertexId);

  _loopId = loopId;
  _loopVertexId = loopVertexId;
}

WH_AF2D_Vertex_TSLT
::~WH_AF2D_Vertex_TSLT ()
{
}

bool WH_AF2D_Vertex_TSLT
::checkInvariant () const
{
  this->WH_AF2D_Vertex_A::checkInvariant ();
  
  WH_ASSERT(0 <= this->loopId ());
  WH_ASSERT(0 <= this->loopVertexId ());

  return true;
}

bool WH_AF2D_Vertex_TSLT
::assureInvariant () const
{
  this->WH_AF2D_Vertex_A::assureInvariant ();
  
  return true;
}

int WH_AF2D_Vertex_TSLT
::loopId () const
{
  return _loopId;
}

int WH_AF2D_Vertex_TSLT
::loopVertexId () const
{
  return _loopVertexId;
}



/* class WH_AF2D_Triangulator_TSLT */

WH_AF2D_Triangulator_TSLT
::WH_AF2D_Triangulator_TSLT ()
{
  _currentLoopId = 0;
  _currentLoopVertexId = 0;
  
  _firstVertex = WH_NULL;
  _prevVertex = WH_NULL;
}

WH_AF2D_Triangulator_TSLT
::~WH_AF2D_Triangulator_TSLT ()
{
}

bool WH_AF2D_Triangulator_TSLT
::checkInvariant () const
{
  this->WH_AF2D_Triangulator_A::checkInvariant ();
  
  WH_ASSERT(0 <= _currentLoopId);
  WH_ASSERT(0 <= _currentLoopVertexId);

  return true;
}

bool WH_AF2D_Triangulator_TSLT
::assureInvariant () const
{
  this->WH_AF2D_Triangulator_A::assureInvariant ();

  return true;
}

WH_AF2D_Vertex_A* WH_AF2D_Triangulator_TSLT
::createVertex 
(const WH_Vector2D& position)
{
  WH_AF2D_Vertex_A* result 
    = new WH_AF2D_Vertex_TSLT 
    (position, _currentLoopId, _currentLoopVertexId);
  WH_ASSERT(result != WH_NULL);

  _currentLoopVertexId++;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_AF2D_Edge_A* WH_AF2D_Triangulator_TSLT
::createEdge 
(WH_AF2D_Vertex_A* vertex0,
 WH_AF2D_Vertex_A* vertex1)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);

  WH_AF2D_Edge_A* result 
    = new WH_AF2D_Edge_A (vertex0, vertex1);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

WH_AF2D_Triangle_A* WH_AF2D_Triangulator_TSLT
::createTriangle 
(WH_AF2D_Vertex_A* vertex0,
 WH_AF2D_Vertex_A* vertex1,
 WH_AF2D_Vertex_A* vertex2)
{
  /* PRE-CONDITION */
  WH_ASSERT(vertex0 != WH_NULL);
  WH_ASSERT(vertex1 != WH_NULL);
  WH_ASSERT(vertex2 != WH_NULL);

  WH_AF2D_Triangle_A* result 
    = new WH_AF2D_Triangle_A (vertex0, vertex1, vertex2);
  WH_ASSERT(result != WH_NULL);
  
  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif
  
  return result;
}

double WH_AF2D_Triangulator_TSLT
::edgeLength () const
{
  return WH::HUGE_VALUE;
}

void WH_AF2D_Triangulator_TSLT
::perform ()
{
  WH_CVR_LINE;

  for (;;) {
    WH_AF2D_Edge_A* edge = this->getAnyEdge ();
    if (edge == WH_NULL) break;
    
    WH_AF2D_Edge_A* neighborEdge0;
    WH_AF2D_Edge_A* neighborEdge1;
    WH_AF2D_Vertex_A* vertex 
      = this->findOptimumVertex (edge,
				 neighborEdge0, neighborEdge1);
    this->advanceFrontWithExistingVertex 
      (edge, vertex, neighborEdge0, neighborEdge1);
  }
}

void WH_AF2D_Triangulator_TSLT
::beginLoop ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_firstVertex == WH_NULL);
  WH_ASSERT(_prevVertex == WH_NULL);

  /* nothing */
}

void WH_AF2D_Triangulator_TSLT
::addLoopVertex (const WH_Vector2D& position)
{
  WH_AF2D_Vertex_A* newVertex 
    = this->createVertex (position);
  this->addVertex (newVertex);

  if (_firstVertex == WH_NULL) {
    WH_ASSERT(_prevVertex == WH_NULL);

    _firstVertex = newVertex;
  } else {
    WH_ASSERT(_prevVertex != WH_NULL);

    WH_AF2D_Edge_A* newEdge 
      = this->createEdge (_prevVertex, newVertex);
    this->addEdge (newEdge);
  }
  _prevVertex = newVertex;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_firstVertex != WH_NULL);
  WH_ASSERT(_prevVertex != WH_NULL);
#endif
}

void WH_AF2D_Triangulator_TSLT
::endLoop ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_firstVertex != WH_NULL);
  WH_ASSERT(_prevVertex != WH_NULL);

  WH_AF2D_Edge_A* newEdge 
    = this->createEdge (_prevVertex, _firstVertex);
  this->addEdge (newEdge);

  _firstVertex = WH_NULL;
  _prevVertex = WH_NULL;

  _currentLoopId++;
  _currentLoopVertexId = 0;

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_firstVertex == WH_NULL);
  WH_ASSERT(_prevVertex == WH_NULL);
#endif
}



/* class WH_TSLT_Tessellator2D */

WH_TSLT_Tessellator2D
::WH_TSLT_Tessellator2D ()
{
  WH_CVR_LINE;

  _triangulator = new WH_AF2D_Triangulator_TSLT ();
  _currentLoop = WH_NULL;
}
 
WH_TSLT_Tessellator2D
::~WH_TSLT_Tessellator2D ()
{
  WH_CVR_LINE;

  WH_T_Delete (_loop_s);
  WH_T_Delete (_triangle_s);
  delete _triangulator;
  delete _currentLoop;
}

bool WH_TSLT_Tessellator2D
::checkInvariant () const
{
  WH_CVR_LINE;

  return true;
}

bool WH_TSLT_Tessellator2D
::assureInvariant () const
{
  WH_CVR_LINE;

  this->checkInvariant ();

  return true;
}
  
void WH_TSLT_Tessellator2D
::beginLoop ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_currentLoop == WH_NULL);

  _currentLoop = new Loop ();
  WH_ASSERT(_currentLoop != WH_NULL);

  _triangulator->beginLoop ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_currentLoop != WH_NULL);
#endif
}

void WH_TSLT_Tessellator2D
::addLoopVertex (const WH_Vector2D& position)
{
  /* PRE-CONDITION */
  WH_ASSERT(_currentLoop != WH_NULL);

  _currentLoop->vertex_s.push_back (position);

  _triangulator->addLoopVertex (position);
}

void WH_TSLT_Tessellator2D
::endLoop ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_currentLoop != WH_NULL);
  WH_ASSERT(3 <= _currentLoop->vertex_s.size ());

  _loop_s.push_back (_currentLoop);
  _currentLoop = WH_NULL;

  _triangulator->endLoop ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(_currentLoop == WH_NULL);
#endif
}

void WH_TSLT_Tessellator2D
::perform ()
{
  /* PRE-CONDITION */
  WH_ASSERT(_currentLoop == WH_NULL);
  WH_ASSERT(_triangle_s.size () == 0);
  WH_ASSERT(0 < _loop_s.size ());

  _triangulator->perform ();

  for (vector<WH_AF2D_Triangle_A*>::const_iterator 
	 i_tri = _triangulator->triangle_s ().begin ();
       i_tri != _triangulator->triangle_s ().end ();
       i_tri++) {
    WH_AF2D_Triangle_A* tri_i = (*i_tri);

    WH_AF2D_Vertex_TSLT* vertex0 
      = (WH_AF2D_Vertex_TSLT*)tri_i->vertex0 ();
    WH_AF2D_Vertex_TSLT* vertex1 
      = (WH_AF2D_Vertex_TSLT*)tri_i->vertex1 ();
    WH_AF2D_Vertex_TSLT* vertex2 
      = (WH_AF2D_Vertex_TSLT*)tri_i->vertex2 ();

    Triangle* newTri = new Triangle ();
    WH_ASSERT(newTri != WH_NULL);
    
    newTri->loopId0 = vertex0->loopId ();
    newTri->vertexId0 = vertex0->loopVertexId ();
    newTri->loopId1 = vertex1->loopId ();
    newTri->vertexId1 = vertex1->loopVertexId ();
    newTri->loopId2 = vertex2->loopId ();
    newTri->vertexId2 = vertex2->loopVertexId ();

    _triangle_s.push_back (newTri);
  }

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(0 < _triangle_s.size ());
#endif
}
  
const vector<WH_TSLT_Tessellator2D::Loop*>& WH_TSLT_Tessellator2D
::loop_s () const
{
  return _loop_s;
}

const vector<WH_TSLT_Tessellator2D::Triangle*>& WH_TSLT_Tessellator2D
::triangle_s () const
{
  return _triangle_s;
}



/* not yet covered */
