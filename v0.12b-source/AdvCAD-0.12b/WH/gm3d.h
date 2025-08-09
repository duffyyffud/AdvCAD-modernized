/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d.cc */

#ifndef WH_HEADER_WH_GM3D
#define WH_HEADER_WH_GM3D

#ifndef WH_INCLUDED_WH_GM3D_BREP
#include <WH/gm3d_brep.h>
#define WH_INCLUDED_WH_GM3D_BREP
#endif



class WH_GM3D {
 public:
  static WH_GM3D_Body* createDotSet 
    (const vector<WH_Vector3D>& point_s);

  static WH_GM3D_Body* createWireSet 
    (const vector<WH_Segment3D>& seg_s);

  static WH_GM3D_Body* createSheet 
    (const WH_Polygon3D& polygon);

  static WH_GM3D_Body* createVolume
    (const vector<WH_Vector3D>& outsideNormal_s,
     const vector<WH_Polygon3D>& polygon_s);

  static WH_GM3D_Body* createBox 
    (const WH_Vector3D& origin,
     const WH_Vector3D& extent);

  static WH_GM3D_Body* extrude 
    (WH_GM3D_Body* profileBody,
     const WH_Vector3D& offset);

  static WH_GM3D_Body* revolve 
    (WH_GM3D_Body* profileBody,
     const WH_Line3D& axis, int nDivisions);

  static void add 
    (WH_GM3D_Body* blankBody,
     WH_GM3D_Body* toolBody  /* DELETE */);

  static void subtract 
    (WH_GM3D_Body* blankBody,
     WH_GM3D_Body* toolBody  /* DELETE */);

  static void stitch 
    (WH_GM3D_Body* blankBody,
     WH_GM3D_Body* toolBody  /* DELETE */);

  /* base */

  /* derived */
  
 protected:

  /* base */

  /* derived */
  
};



#endif /* WH_HEADER_WH_GM3D */
