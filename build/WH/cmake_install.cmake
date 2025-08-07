# Install script for directory: /home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/build/WH/libWH.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/WH" TYPE FILE FILES
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/afront2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/bucket.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/bucket2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/bucket3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/common.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/connector.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/connector2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/constdel2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/coverage.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/debug_levels.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/delaunay2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/delaunay3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/field.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/field2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/field3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/geometry_analyzer.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d_brep.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d_convert.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d_facet.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d_setop.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm2d_stitch.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_brep.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_convert.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_facet.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_io.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_setop.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_sheetsetop.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_stitch.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/gm3d_tpl3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/hashtable.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/inout2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/inout3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/mg3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/mg3d_base.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/mg3d_delaunay2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/mg3d_delaunay3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/poly_tpl3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/polygon2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/polygon3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/polyhedron3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/robust_cdt.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/robust_predicates.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/scalar.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/segment2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/segment3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/sorter.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/space2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/space3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/tessellate2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/tetrahedron3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/topology3d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/triangle2d.h"
    "/home/miyoshi/workspace/wsCpp/AdvCAD-0.12b/WH/triangle3d.h"
    )
endif()

