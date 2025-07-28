/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_tpl3d.cc */

#include "gm3d_tpl3d.h"



/* class WH_TPL3D_Converter_GM3D */

WH_TPL3D_PolyBody* WH_TPL3D_Converter_GM3D
::createBody (WH_GM3D_Body* body)
{
  /* PRE-CONDITION */
  WH_ASSERT(body != WH_NULL);

  WH_TPL3D_PolyBody* result = new WH_TPL3D_PolyBody ();
  WH_ASSERT(result != WH_NULL);

  WH_TPL3D_PolyVolume* tplVolume = new WH_TPL3D_PolyVolume (result);
  WH_ASSERT(tplVolume != WH_NULL);

  map<WH_GM3D_Vertex*, WH_TPL3D_PolyVertex*> vertexMap;

  for (vector<WH_GM3D_Vertex*>::const_iterator 
	 i_vertex = body->vertex_s ().begin ();
       i_vertex != body->vertex_s ().end ();
       i_vertex++) {
    WH_GM3D_Vertex* vertex_i = (*i_vertex);
    WH_TPL3D_PolyVertex* tplVertex = new WH_TPL3D_PolyVertex 
      (result, vertex_i->point ());
    WH_ASSERT(tplVertex != WH_NULL);
    vertexMap[vertex_i] = tplVertex;
  }

  for (vector<WH_GM3D_Face*>::const_iterator 
	 i_face = body->face_s ().begin ();
       i_face != body->face_s ().end ();
       i_face++) {
    WH_GM3D_Face* face_i = (*i_face);
    
    WH_ASSERT(face_i->faceType () == WH_GM3D_Face::OUTER_BOUNDARY);

    WH_Vector3D outsideNormal;
    bool flag = face_i->getNormalToOutsideVolume 
      (outsideNormal);
    WH_ASSERT(flag);
    WH_Vector3D facePlaneNormal = face_i->plane ().normal ();
    bool isSameNormal = false;
    if (WH_eq (outsideNormal, facePlaneNormal)) {
      isSameNormal = true;
    }

    WH_TPL3D_PolyFace* tplFace = new WH_TPL3D_PolyFace 
      (result, WH_TPL3D_PolyFace::OUTER_BOUNDARY);
    WH_ASSERT(tplFace != WH_NULL);
    tplVolume->addFace (tplFace);

    {
      WH_GM3D_Loop* loop = face_i->outerLoop ();

      WH_TPL3D_PolyLoop* tplLoop = new WH_TPL3D_PolyLoop (tplFace);
      WH_ASSERT(tplLoop != WH_NULL);

      /* outer loop is counter clock-wise */
      WH_Polygon2D poly = loop->parameterPolygon ();

      if ((isSameNormal && poly.isClockWise ())
	  || (!isSameNormal && !poly.isClockWise ())) {
	for (vector<WH_GM3D_LoopVertexUse*>::const_reverse_iterator 
	       i_vertexUse = loop->vertexUse_s ().rbegin ();
	     i_vertexUse != loop->vertexUse_s ().rend ();
	     i_vertexUse++) {
	  WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	  WH_GM3D_Vertex* vertex = vertexUse_i->vertex ();
	  WH_TPL3D_PolyVertex* tplVertex = vertexMap[vertex];
	  WH_ASSERT(tplVertex != WH_NULL);
	  tplLoop->addVertex (tplVertex);
	}
      } else {
	for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	       i_vertexUse = loop->vertexUse_s ().begin ();
	     i_vertexUse != loop->vertexUse_s ().end ();
	     i_vertexUse++) {
	  WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	  WH_GM3D_Vertex* vertex = vertexUse_i->vertex ();
	  WH_TPL3D_PolyVertex* tplVertex = vertexMap[vertex];
	  WH_ASSERT(tplVertex != WH_NULL);
	  tplLoop->addVertex (tplVertex);
	}
      }

      tplFace->setOuterLoop (tplLoop, outsideNormal);
    }

    for (vector<WH_GM3D_Loop*>::const_iterator 
	   i_loop = face_i->innerLoop_s ().begin ();
	 i_loop != face_i->innerLoop_s ().end ();
	 i_loop++) {
      WH_GM3D_Loop* loop_i = (*i_loop);

      WH_TPL3D_PolyLoop* tplLoop = new WH_TPL3D_PolyLoop (tplFace);
      WH_ASSERT(tplLoop != WH_NULL);

      /* inner loop is clock-wise */
      WH_Polygon2D poly = loop_i->parameterPolygon ();

      if ((isSameNormal && poly.isClockWise ())
	  || (!isSameNormal && !poly.isClockWise ())) {
	for (vector<WH_GM3D_LoopVertexUse*>::const_iterator 
	       i_vertexUse = loop_i->vertexUse_s ().begin ();
	     i_vertexUse != loop_i->vertexUse_s ().end ();
	     i_vertexUse++) {
	  WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	  WH_GM3D_Vertex* vertex = vertexUse_i->vertex ();
	  WH_TPL3D_PolyVertex* tplVertex = vertexMap[vertex];
	  WH_ASSERT(tplVertex != WH_NULL);
	  tplLoop->addVertex (tplVertex);
	}
      } else {
	for (vector<WH_GM3D_LoopVertexUse*>::const_reverse_iterator 
	       i_vertexUse = loop_i->vertexUse_s ().rbegin ();
	     i_vertexUse != loop_i->vertexUse_s ().rend ();
	     i_vertexUse++) {
	  WH_GM3D_LoopVertexUse* vertexUse_i = (*i_vertexUse);
	  WH_GM3D_Vertex* vertex = vertexUse_i->vertex ();
	  WH_TPL3D_PolyVertex* tplVertex = vertexMap[vertex];
	  WH_ASSERT(tplVertex != WH_NULL);
	  tplLoop->addVertex (tplVertex);
	}
      }

      tplFace->addInnerLoop (tplLoop);
    }
  }

  result->makeEdgeUses ();
  result->updateConnection ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
  WH_ASSERT(result->assureInvariant ());
#endif

  return result;
}

