/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm2d.cc */

#pragma once
#ifndef WH_INCLUDED_WH_GM2D_BREP
#include <WH/gm2d_brep.h>
#define WH_INCLUDED_WH_GM2D_BREP
#endif

class WH_GM2D {
 public:
  static WH_GM2D_Body* createDotSet 
    (const vector<WH_Vector2D>& point_s);

  static WH_GM2D_Body* createWireSet 
    (const vector<WH_Segment2D>& seg_s);

  static WH_GM2D_Body* createVolume 
    (const WH_Polygon2D& polygon);

  static void add 
    (WH_GM2D_Body* blankBody,
     WH_GM2D_Body* toolBody  /* DELETE */);

  static void subtract 
    (WH_GM2D_Body* blankBody,
     WH_GM2D_Body* toolBody  /* DELETE */);

  static void stitch 
    (WH_GM2D_Body* blankBody,
     WH_GM2D_Body* toolBody  /* DELETE */);

  /* base */

  /* derived */
  
 protected:

  /* base */

  /* derived */
  
};
