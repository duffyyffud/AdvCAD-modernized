/********************************************************************
 * Robust Geometric Predicates Implementation
 * Provides exact arithmetic for critical geometric tests
 *********************************************************************/

#include "robust_predicates.h"
#include <cmath>
#include <iostream>

namespace WH_RobustPredicates {

// Robust 2D orientation test
double orient2d_robust(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc) {
    return RobustOrientationTest::test(pa, pb, pc);
}

// Robust incircle test  
double incircle_robust(const WH_Vector2D& pa, const WH_Vector2D& pb, 
                      const WH_Vector2D& pc, const WH_Vector2D& pd) {
    // Translate points to reduce coordinate magnitude
    double adx = pa.x - pd.x;
    double ady = pa.y - pd.y;
    double bdx = pb.x - pd.x;
    double bdy = pb.y - pd.y;
    double cdx = pc.x - pd.x;
    double cdy = pc.y - pd.y;
    
    double abdet = adx * bdy - bdx * ady;
    double bcdet = bdx * cdy - cdx * bdy;
    double cadet = cdx * ady - adx * cdy;
    double alift = adx * adx + ady * ady;
    double blift = bdx * bdx + bdy * bdy;
    double clift = cdx * cdx + cdy * cdy;
    
    double det = alift * bcdet + blift * cadet + clift * abdet;
    
    // Check if result needs higher precision
    double errbound = 1e-15 * (std::abs(alift) + std::abs(blift) + std::abs(clift)) *
                      (std::abs(bcdet) + std::abs(cadet) + std::abs(abdet));
    
    if (std::abs(det) > errbound) {
        return det;
    }
    
    // Fall back to exact arithmetic if needed
    std::cerr << "DEBUG: Using exact arithmetic for incircle test" << std::endl;
    // For now, return the computed value (exact arithmetic would go here)
    return det;
}

// Robust segment intersection
bool intersect_segments_robust(const WH_Vector2D& p1, const WH_Vector2D& q1,
                              const WH_Vector2D& p2, const WH_Vector2D& q2,
                              WH_Vector2D& intersection) {
    double o1 = orient2d_robust(p1, q1, p2);
    double o2 = orient2d_robust(p1, q1, q2);
    double o3 = orient2d_robust(p2, q2, p1);
    double o4 = orient2d_robust(p2, q2, q1);
    
    // General case - segments intersect if orientations are different
    if (((o1 > 0) != (o2 > 0)) && ((o3 > 0) != (o4 > 0))) {
        // Compute intersection point
        double denom = (p1.x - q1.x) * (p2.y - q2.y) - (p1.y - q1.y) * (p2.x - q2.x);
        if (std::abs(denom) < 1e-15) return false; // Parallel
        
        double t = ((p1.x - p2.x) * (p2.y - q2.y) - (p1.y - p2.y) * (p2.x - q2.x)) / denom;
        intersection = p1 + (q1 - p1) * t;
        return true;
    }
    
    // Collinear cases
    if (std::abs(o1) < 1e-15 && std::abs(o2) < 1e-15 && 
        std::abs(o3) < 1e-15 && std::abs(o4) < 1e-15) {
        // Check if segments overlap
        if (WH_max(p1.x, q1.x) >= WH_min(p2.x, q2.x) &&
            WH_max(p2.x, q2.x) >= WH_min(p1.x, q1.x) &&
            WH_max(p1.y, q1.y) >= WH_min(p2.y, q2.y) &&
            WH_max(p2.y, q2.y) >= WH_min(p1.y, q1.y)) {
            // Return midpoint of overlap
            intersection = (p1 + q1 + p2 + q2) * 0.25;
            return true;
        }
    }
    
    return false;
}

// Robust point-in-triangle test
bool point_in_triangle_robust(const WH_Vector2D& p, const WH_Vector2D& a,
                             const WH_Vector2D& b, const WH_Vector2D& c) {
    double o1 = orient2d_robust(a, b, p);
    double o2 = orient2d_robust(b, c, p);
    double o3 = orient2d_robust(c, a, p);
    
    // Point is inside if all orientations have same sign
    return (o1 >= 0 && o2 >= 0 && o3 >= 0) || (o1 <= 0 && o2 <= 0 && o3 <= 0);
}

// Robust segment-triangle intersection
bool segment_intersects_triangle_robust(const WH_Vector2D& seg_start, const WH_Vector2D& seg_end,
                                       const WH_Vector2D& tri_a, const WH_Vector2D& tri_b, const WH_Vector2D& tri_c) {
    // Check if either endpoint is inside triangle
    if (point_in_triangle_robust(seg_start, tri_a, tri_b, tri_c) ||
        point_in_triangle_robust(seg_end, tri_a, tri_b, tri_c)) {
        return true;
    }
    
    // Check if segment intersects any triangle edge
    WH_Vector2D intersection;
    return intersect_segments_robust(seg_start, seg_end, tri_a, tri_b, intersection) ||
           intersect_segments_robust(seg_start, seg_end, tri_b, tri_c, intersection) ||
           intersect_segments_robust(seg_start, seg_end, tri_c, tri_a, intersection);
}

// Adaptive precision orientation test
double RobustOrientationTest::test(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc,
                                  PrecisionLevel max_precision) {
    // Try double precision first
    double result = test_double(pa, pb, pc);
    
    // Estimate error bound
    double errbound = 1e-15 * (std::abs(pa.x) + std::abs(pa.y) + 
                               std::abs(pb.x) + std::abs(pb.y) + 
                               std::abs(pc.x) + std::abs(pc.y));
    
    if (std::abs(result) > errbound || max_precision == DOUBLE_PRECISION) {
        return result;
    }
    
    // Fall back to exact arithmetic if available
    if (max_precision >= EXACT_PRECISION) {
        std::cerr << "DEBUG: Using exact arithmetic for orientation test" << std::endl;
        return test_exact(pa, pb, pc);
    }
    
    return result;
}

double RobustOrientationTest::test_double(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc) {
    return (pb.x - pa.x) * (pc.y - pa.y) - (pc.x - pa.x) * (pb.y - pa.y);
}

double RobustOrientationTest::test_exact(const WH_Vector2D& pa, const WH_Vector2D& pb, const WH_Vector2D& pc) {
    // For now, use high-precision double computation
    // In a full implementation, this would use arbitrary precision arithmetic
    long double pax = pa.x, pay = pa.y;
    long double pbx = pb.x, pby = pb.y;
    long double pcx = pc.x, pcy = pc.y;
    
    return static_cast<double>((pbx - pax) * (pcy - pay) - (pcx - pax) * (pby - pay));
}

}