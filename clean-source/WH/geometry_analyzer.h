/********************************************************************
 * Geometry Analyzer for Mesh Size Validation
 * Analyzes geometry to determine appropriate mesh size ranges
 *********************************************************************/

#pragma once

#ifndef WH_INCLUDED_WH_GM3D
#include <WH/gm3d.h>
#define WH_INCLUDED_WH_GM3D
#endif

#ifndef WH_INCLUDED_WH_SPACE3D
#include <WH/space3d.h>
#define WH_INCLUDED_WH_SPACE3D
#endif

#include <limits>

class WH_GeometryAnalyzer {
public:
    struct GeometryMetrics {
        double boundingBoxDiagonal;    // Overall model size
        double minimumFeatureSize;      // Smallest edge/gap
        double averageFeatureSize;      // Typical feature size
        double recommendedMeshSize;     // Based on analysis
        double minimumSafeMeshSize;     // Absolute minimum
        double maximumUsefulMeshSize;   // Larger would undersample
        
        // Additional metrics
        double minimumEdgeLength;       // Shortest edge in model
        double minimumFaceArea;         // Smallest face area
        double aspectRatio;             // Bounding box aspect ratio
        int totalFaces;
        int totalEdges;
        int totalVertices;
        
        GeometryMetrics();
        void print() const;
    };
    
    // Main analysis function
    static GeometryMetrics analyze(const WH_GM3D_Body& body);
    
    // Validation functions
    static bool isMeshSizeAppropriate(double meshSize, const GeometryMetrics& metrics);
    static double adjustMeshSize(double requested, const GeometryMetrics& metrics);
    
    // Detailed analysis functions
    static double computeMinimumFeatureSize(const WH_GM3D_Body& body);
    static double computeMinimumEdgeLength(const WH_GM3D_Body& body);
    static double computeMinimumGap(const WH_GM3D_Body& body);
    static double computeMinimumThickness(const WH_GM3D_Body& body);
    
    // Helper functions
    static void analyzeFace(const WH_GM3D_Face* face, GeometryMetrics& metrics);
    static void analyzeEdge(const WH_GM3D_Edge* edge, GeometryMetrics& metrics);
    
    // Mesh size recommendation based on geometry
    static double recommendMeshSize(const GeometryMetrics& metrics);
    
    // Constants for mesh size rules
    static constexpr double MINIMUM_FEATURE_RATIO = 0.05;  // 5% of min feature
    static constexpr double MAXIMUM_FEATURE_RATIO = 0.5;   // 50% of min feature
    static constexpr double RECOMMENDED_FEATURE_RATIO = 0.2; // 20% of min feature
    static constexpr double NUMERICAL_PRECISION_RATIO = 1e-6; // Relative to bbox
    static constexpr int MINIMUM_ELEMENTS_PER_FEATURE = 10;
};

// Inline implementations
inline WH_GeometryAnalyzer::GeometryMetrics::GeometryMetrics()
    : boundingBoxDiagonal(0.0)
    , minimumFeatureSize(std::numeric_limits<double>::max())
    , averageFeatureSize(0.0)
    , recommendedMeshSize(0.0)
    , minimumSafeMeshSize(0.0)
    , maximumUsefulMeshSize(0.0)
    , minimumEdgeLength(std::numeric_limits<double>::max())
    , minimumFaceArea(std::numeric_limits<double>::max())
    , aspectRatio(1.0)
    , totalFaces(0)
    , totalEdges(0)
    , totalVertices(0)
{}