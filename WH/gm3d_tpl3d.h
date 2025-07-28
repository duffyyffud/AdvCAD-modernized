/********************************************************************
 Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,
 the Japan Society for the Promotion of Science (JSPS)
 All Rights Reserved
 *********************************************************************/

/* header file for gm3d_tpl3d.cc */

#ifndef WH_HEADER_WH_GM3D_TPL3D
#define WH_HEADER_WH_GM3D_TPL3D

#ifndef WH_INCLUDED_WH_POLY_TPL3D
#include <WH/poly_tpl3d.h>
#define WH_INCLUDED_WH_POLY_TPL3D
#endif

#ifndef WH_INCLUDED_WH_GM3D
#include <WH/gm3d.h>
#define WH_INCLUDED_WH_GM3D
#endif



class WH_TPL3D_Converter_GM3D;



class WH_TPL3D_Converter_GM3D {
 public:
  static WH_TPL3D_PolyBody* createBody (WH_GM3D_Body* body);
};



#endif /* WH_HEADER_WH_GM3D_TPL3D */
