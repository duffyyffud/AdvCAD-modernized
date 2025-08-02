/********************************************************************
 * Robust Geometric Predicates for AdvCAD
 * Implements exact arithmetic predicates to prevent numerical failures
 * Based on Jonathan Shewchuk's robust geometric predicates
 *********************************************************************/

#pragma once

#include "common.h"
#include "space2d.h"

namespace WH_RobustPredicates {

// Orientation test: returns > 0 if points are counterclockwise
// = 0 if collinear, < 0 if clockwise
double orient2d_robust(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc);

// Incircle test: returns > 0 if pd is inside circle through pa,pb,pc
// = 0 if on circle, < 0 if outside
double incircle_robust(const WH_Vector2D& pa, const WH_Vector2D& pb, 
                      const WH_Vector2D& pc, const WH_Vector2D& pd);

// Line-line intersection with robust computation
bool intersect_segments_robust(const WH_Vector2D& p1, const WH_Vector2D& q1,
                              const WH_Vector2D& p2, const WH_Vector2D& q2,
                              WH_Vector2D& intersection);

// Point-in-triangle test with robust predicates
bool point_in_triangle_robust(const WH_Vector2D& p, const WH_Vector2D& a,
                             const WH_Vector2D& b, const WH_Vector2D& c);

// Segment-triangle intersection for constraint recovery
bool segment_intersects_triangle_robust(const WH_Vector2D& seg_start, const WH_Vector2D& seg_end,
                                       const WH_Vector2D& tri_a, const WH_Vector2D& tri_b, const WH_Vector2D& tri_c);

// Adaptive precision: starts with double, escalates to exact if needed
enum PrecisionLevel {
    DOUBLE_PRECISION,
    EXTENDED_PRECISION,
    EXACT_PRECISION
};

class RobustOrientationTest {
public:
    static double test(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc,
                      PrecisionLevel max_precision = EXACT_PRECISION);
private:
    static double test_double(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc);
    static double test_exact(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc);
};

}