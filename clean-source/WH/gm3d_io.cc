/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* gm3d_io.cc : the file I/O module of 3-D geometry modeling library */

#if 0
#define WH_COVERAGE_ENABLED
#endif

#include "gm3d_io.h"



/* module procedures */

static void PushSheet 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  WH_CVR_LINE;

  int nVertexs;
  in >> nVertexs;
  WH_ASSERT(2 < nVertexs);
  
  vector<WH_Vector3D> vertex_s;
  for (int iVertex = 0; iVertex < nVertexs; iVertex++) {
    WH_Vector3D vertex;
    in >> vertex.x;
    in >> vertex.y;
    in >> vertex.z;
    vertex_s.push_back (vertex);
  }
  
  WH_Polygon3D poly (vertex_s);
  WH_GM3D_Body* body 
    = WH_GM3D::createSheet (poly);
  bodyStack_IO.push_back (body);
}

static void PushCircle 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  WH_CVR_LINE;

  WH_Vector3D center;
  in >> center.x >> center.y >> center.z;
  
  WH_Vector3D xAxis;
  in >> xAxis.x >> xAxis.y >> xAxis.z;
  WH_ASSERT(WH_ne (xAxis, WH_Vector3D::zero ()));
  
  WH_Vector3D normal;
  in >> normal.x >> normal.y >> normal.z;
  WH_ASSERT(WH_ne (normal, WH_Vector3D::zero ()));
  
  int nDivisions;
  in >> nDivisions;
  WH_ASSERT(1 < nDivisions);
  
  double radius = xAxis.length ();
  WH_Vector3D yAxis 
    = WH_vectorProduct (normal, xAxis).normalize () * radius;
  
  vector<WH_Vector3D> vertex_s;
  for (int iDiv = 0; iDiv < nDivisions; iDiv++) {
    double angle = M_PI * 2 / nDivisions * iDiv;
    WH_Vector3D vertex 
      (center + xAxis * cos (angle) + yAxis * sin (angle));
    vertex_s.push_back (vertex);
  }
  
  WH_Polygon3D poly (vertex_s);
  WH_GM3D_Body* body 
    = WH_GM3D::createSheet (poly);
  bodyStack_IO.push_back (body);
}

static void PushBox 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  WH_CVR_LINE;

  WH_Vector3D origin;
  in >> origin.x >> origin.y >> origin.z;
  
  WH_Vector3D extent;
  in >> extent.x >> extent.y >> extent.z;
  WH_ASSERT(WH_ne (extent, WH_Vector3D::zero ()));
  
  WH_GM3D_Body* body 
    = WH_GM3D::createBox (origin, extent);
  bodyStack_IO.push_back (body);
}

static void ExtrudeFirst 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(1 <= bodyStack_IO.size ());
  
  WH_CVR_LINE;

  WH_Vector3D offset;
  in >> offset.x >> offset.y >> offset.z;
  WH_ASSERT(WH_ne (offset, WH_Vector3D::zero ()));
  
  WH_GM3D_Body* profileBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  
  WH_GM3D_Body* solidBody 
    = WH_GM3D::extrude (profileBody, offset);
  bodyStack_IO.push_back (solidBody);
  
  delete profileBody;
  profileBody = WH_NULL;
}

static void RevolveFirst 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(1 <= bodyStack_IO.size ());
  
  WH_CVR_LINE;

  WH_Vector3D point0;
  WH_Vector3D point1;
  in >> point0.x >> point0.y >> point0.z;
  in >> point1.x >> point1.y >> point1.z;
  WH_ASSERT(WH_ne (point0, point1));
  WH_Line3D axis (point0, point1);
  
  int nDivisions;
  in >> nDivisions;
  WH_ASSERT(1 < nDivisions);
  
  WH_GM3D_Body* profileBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  
  WH_GM3D_Body* solidBody 
    = WH_GM3D::revolve (profileBody, axis, nDivisions);
  bodyStack_IO.push_back (solidBody);
  
  delete profileBody;
  profileBody = WH_NULL;
}

static void AddFirstAndSecond 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= bodyStack_IO.size ());
  
  WH_CVR_LINE;

  WH_GM3D_Body* toolBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  WH_GM3D_Body* blankBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  
  WH_GM3D::add (blankBody, toolBody);
  bodyStack_IO.push_back (blankBody);
}

static void SubtractFirstFromSecond 
(ifstream& in, vector<WH_GM3D_Body*>& bodyStack_IO)
{
  /* PRE-CONDITION */
  WH_ASSERT(2 <= bodyStack_IO.size ());
  
  WH_CVR_LINE;

  WH_GM3D_Body* toolBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  WH_GM3D_Body* blankBody = bodyStack_IO.back ();
  bodyStack_IO.pop_back ();
  
  WH_GM3D::subtract (blankBody, toolBody);
  bodyStack_IO.push_back (blankBody);
}



/* class WH_GM3D_IO */

WH_GM3D_Body* WH_GM3D_IO
::createBodyFromFile 
(const string& fileName)
{
  /* PRE-CONDITION */
  WH_ASSERT(0 < fileName.length ());

  WH_CVR_LINE;

  WH_GM3D_Body* result = WH_NULL;

  vector<WH_GM3D_Body*> bodyStack;

  ifstream in (fileName.c_str ());
  WH_ASSERT(in);

  for (;;) {
    string word;
    in >> word;

    if (word[0] == '#') {
      string line;
      getline (in, line);

      cerr << line << endl;

    } else if (word == "sheet") {
      PushSheet (in, bodyStack);
    } else if (word == "circle") {
      PushCircle (in, bodyStack);
    } else if (word == "box") {
      PushBox (in, bodyStack);
    } else if (word == "extrude") {
      ExtrudeFirst (in, bodyStack);
    } else if (word == "revolve") {
      RevolveFirst (in, bodyStack);
    } else if (word == "add") {
      AddFirstAndSecond (in, bodyStack);
    } else if (word == "subtract") {
      SubtractFirstFromSecond (in, bodyStack);
    } else {
      break;
    }
  }

  WH_ASSERT(bodyStack.size () == 1);
  result = bodyStack.back ();

  /* POST-CONDITION */
#ifndef WH_PRE_ONLY
  WH_ASSERT(result != WH_NULL);
#endif

  return result;
}
