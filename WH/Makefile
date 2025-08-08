########################################################################### 
# Copyright (C) 2002 Shinobu Yoshimura, University of Tokyo,              #
# the Japan Society for the Promotion of Science (JSPS)                   #
# All Rights Reserved                                                     #
########################################################################### 

#
# C++ compiler
#   GNU
CC = g++

# Include directory flags
INCLUDES = -I..

# Optimization flags
OPTIMIZE = -O

# Profile flags
PROFILE = -pg

# Debug flags
DEBUG = -g

# Warning flags
#   GNU
WARNING = -Wall

# C++ complier flags
#   debug 
#CFLAGS = $(COMPILER) $(INCLUDES) $(WARNING) $(DEBUG)
#   optimize 
CFLAGS = $(COMPILER) $(INCLUDES) $(OPTIMIZE) \
	-DWH_PRE_ONLY -DNDEBUG -DWH_INLINE_ENABLED
#   profile
#CFLAGS = $(COMPILER) $(INCLUDES) $(OPTIMIZE) $(PROFILE) \
#	-DWH_PRE_ONLY -DNDEBUG -DWH_INLINE_ENABLED

# Basic libraries
LIBS = -lm

# C++ linker flags (excluding libraries)
#   
LDFLAGS = 
#   optimize 
#LDFLAGS = 
#   profile 
#LDFLAGS = $(PROFILE)

# Archiver
AR = ar 

# Archive flags
ARFLAGS = crsv

.SUFFIXES: .cc

.cc.o:
	$(CC) -c $< $(CFLAGS)



all   : libWH.a



#  
#  WH basic library
#

BASEH  = common.h coverage.h scalar.h \
	sorter.h hashtable.h \
	field.h bucket.h \
	space2d.h field2d.h \
	segment2d.h polygon2d.h triangle2d.h \
	bucket2d.h inout2d.h \
	space3d.h field3d.h \
	segment3d.h polygon3d.h triangle3d.h \
	polyhedron3d.h tetrahedron3d.h \
	bucket3d.h inout3d.h \
	afront2d.h \
	delaunay2d.h constdel2d.h \
	delaunay3d.h \
	connector.h connector2d.h \
	tessellate2d.h 
BASEO  = common.o coverage.o scalar.o \
	sorter.o hashtable.o \
	field.o bucket.o \
	space2d.o field2d.o \
	segment2d.o polygon2d.o triangle2d.o \
	bucket2d.o inout2d.o \
	space3d.o field3d.o \
	segment3d.o polygon3d.o triangle3d.o \
	polyhedron3d.o tetrahedron3d.o \
	bucket3d.o inout3d.o \
	afront2d.o \
	delaunay2d.o constdel2d.o \
	delaunay3d.o \
	connector.o connector2d.o \
	tessellate2d.o debug_levels.o 

common.o      : common.h 
coverage.o    : common.h \
	coverage.h 
scalar.o      : common.h coverage.h \
	scalar.h scalar_inline.cc 

sorter.o      : common.h coverage.h scalar.h \
	sorter.h sorter_inline.cc 
hashtable.o   : common.h coverage.h scalar.h \
	hashtable.h
field.o       : common.h coverage.h scalar.h \
	field.h field_inline.cc 
bucket.o      : common.h coverage.h scalar.h field.h \
	bucket.h bucket_inline.cc 

space2d.o     : common.h coverage.h scalar.h \
	space2d.h space2d_inline.cc
segment2d.o   : common.h coverage.h scalar.h \
	space2d.h \
	segment2d.h segment2d_inline.cc
polygon2d.o   : common.h coverage.h scalar.h \
	space2d.h segment2d.h \
	polygon2d.h 
triangle2d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h \
	triangle2d.h 
field2d.o     : common.h coverage.h scalar.h \
	space2d.h \
	field2d.h field2d_inline.cc
bucket2d.o    : common.h coverage.h scalar.h \
	space2d.h field2d.h \
	bucket2d.h bucket2d_inline.cc
inout2d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h field2d.h bucket2d.h \
	inout2d.h

space3d.o     : common.h coverage.h scalar.h \
	space2d.h \
	space3d.h space3d_inline.cc
segment3d.o   : common.h coverage.h scalar.h \
	space2d.h segment2d.h \
	space3d.h \
	segment3d.h segment3d_inline.cc
polygon3d.o   : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h \
	space3d.h segment3d.h \
	polygon3d.h 
triangle3d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	space3d.h segment3d.h polygon3d.h \
	triangle3d.h 
polyhedron3d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h \
	space3d.h segment3d.h polygon3d.h \
	polyhedron3d.h 
tetrahedron3d.o : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	space3d.h segment3d.h polygon3d.h triangle3d.h \
	polyhedron3d.h \
	tetrahedron3d.h
field3d.o     : common.h coverage.h scalar.h \
	space2d.h \
	space3d.h \
	field3d.h field3d_inline.cc
bucket3d.o    : common.h coverage.h scalar.h \
	space2d.h \
	space3d.h field3d.h \
	bucket3d.h bucket3d_inline.cc
inout3d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	space3d.h segment3d.h polygon3d.h triangle3d.h \
	field3d.h bucket3d.h \
	inout3d.h

afront2d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	field2d.h bucket2d.h \
	afront2d.h afront2d_inline.cc 
delaunay2d.o  : common.h coverage.h scalar.h hashtable.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	delaunay2d.h delaunay2d_inline.cc
constdel2d.o  : common.h coverage.h scalar.h hashtable.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h delaunay2d.h \
	constdel2d.h constdel2d_inline.cc
delaunay3d.o  : common.h coverage.h scalar.h hashtable.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	space3d.h segment3d.h polygon3d.h triangle3d.h \
	polyhedron3d.h tetrahedron3d.h \
	delaunay3d.h delaunay3d_inline.cc
connector.o     : common.h coverage.h scalar.h \
	connector.h
connector2d.o  : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	connector.h connector2d.h
tessellate2d.o : common.h coverage.h scalar.h \
	space2d.h segment2d.h polygon2d.h triangle2d.h \
	field2d.h bucket2d.h afront2d.h \
	tessellate2d.h



#  
#  WH 2-D geometry modeling library
#

GM2DH  = gm2d_brep.h gm2d_facet.h gm2d_convert.h \
	gm2d_setop.h gm2d_stitch.h \
	gm2d.h
GM2DO  = gm2d_brep.o gm2d_facet.o gm2d_convert.o \
	gm2d_setop.o gm2d_stitch.o \
	gm2d.o

gm2d_brep.o         : $(BASEH) gm2d_brep.h
gm2d_facet.o        : $(BASEH) gm2d_facet.h
gm2d_convert.o      : $(BASEH) gm2d_brep.h gm2d_facet.h gm2d_convert.h
gm2d_setop.o        : $(BASEH) gm2d_facet.h gm2d_setop.h
gm2d_stitch.o       : $(BASEH) gm2d_facet.h gm2d_stitch.h
gm2d.o              : $(BASEH) gm2d_brep.h gm2d_facet.h gm2d_convert.h \
	 gm2d_setop.h gm2d_stitch.h



#  
#  WH 3-D geometry modeling library section
#

GM3DH  = gm3d_brep.h gm3d_facet.h gm3d_convert.h \
	gm3d_setop.h gm3d_sheetsetop.h gm3d_stitch.h \
	gm3d.h gm3d_io.h
GM3DO  = gm3d_brep_vertex.o gm3d_brep_edge.o \
	gm3d_brep_face.o gm3d_brep.o \
	gm3d_facet_seg.o gm3d_facet_tri.o \
	gm3d_facet_poly.o gm3d_facet.o \
	gm3d_convert.o \
	gm3d_setop.o gm3d_sheetsetop.o gm3d_stitch.o \
	gm3d.o gm3d_io.o

gm3d_brep_vertex.o  : $(BASEH) gm3d_brep.h
gm3d_brep_edge.o    : $(BASEH) gm3d_brep.h
gm3d_brep_face.o    : $(BASEH) gm3d_brep.h
gm3d_brep.o         : $(BASEH) gm3d_brep.h
gm3d_facet_seg.o    : $(BASEH) gm3d_facet.h
gm3d_facet_tri.o    : $(BASEH) gm3d_facet.h
gm3d_facet_poly.o   : $(BASEH) gm3d_facet.h
gm3d_facet.o        : $(BASEH) gm3d_facet.h
gm3d_convert.o      : $(BASEH) gm3d_brep.h gm3d_facet.h \
	gm3d_convert.h
gm3d_setop.o        : $(BASEH) gm3d_facet.h gm3d_setop.h
gm3d_sheetsetop.o   : $(BASEH) gm2d_facet.h gm2d_setop.h \
	gm3d_facet.h gm3d_sheetsetop.h
gm3d_stitch.o       : $(BASEH) gm3d_facet.h gm3d_stitch.h
gm3d.o              : $(BASEH) gm3d_brep.h gm3d_facet.h \
	gm3d_convert.h gm3d_setop.h gm3d_stitch.h gm3d.h
gm3d_io.o           : $(BASEH) gm3d_brep.h gm3d_facet.h \
	gm3d_convert.h gm3d_setop.h gm3d_stitch.h gm3d.h gm3d_io.h



#  
#  WH 3-D topology framework library 
#

TPL3DH  = topology3d.h poly_tpl3d.h gm3d_tpl3d.h
TPL3DO  = topology3d.o poly_tpl3d.o gm3d_tpl3d.o

topology3d.o  : $(BASEH) topology3d.h \
	topology3d_inline.cc
poly_tpl3d.o  : $(BASEH) topology3d.h poly_tpl3d.h
gm3d_tpl3d.o  : $(BASEH) $(GM3DH) topology3d.h poly_tpl3d.h gm3d_tpl3d.h



#  
#  WH 3-D mesh generation library 
#

MG3DH  = mg3d_base.h mg3d.h mg3d_delaunay2d.h mg3d_delaunay3d.h
MG3DO  = mg3d_base.o mg3d.o mg3d_delaunay2d.o mg3d_delaunay3d.o geometry_analyzer.o robust_cdt.o robust_predicates.o

mg3d_base.o        : $(BASEH) $(TPL3DH) \
	mg3d_base.h 
mg3d_delaunay2d.o   : $(BASEH) $(TPL3DH) \
	mg3d_base.h mg3d.h mg3d_delaunay2d.h
mg3d_delaunay3d.o   : $(BASEH) $(TPL3DH) \
	mg3d_base.h mg3d.h mg3d_delaunay3d.h 
mg3d.o            : $(BASEH) $(TPL3DH) \
	mg3d_base.h mg3d.h mg3d_delaunay2d.h mg3d_delaunay3d.h

debug_levels.o    : common.h debug_levels.h
geometry_analyzer.o : $(BASEH) $(GM3DH) geometry_analyzer.h
robust_cdt.o      : $(BASEH) robust_cdt.h robust_predicates.h
robust_predicates.o : $(BASEH) robust_predicates.h



#  
#  WH library archive
#

libWH.a : $(BASEO) $(GM2DO) $(GM3DO) $(TPL3DO) $(MG3DO)
	$(AR) $(ARFLAGS) $@ $^



clean      : 
	rm -f *.o *.a



