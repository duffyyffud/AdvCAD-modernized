/********************************************************************
 * Robust Constrained Delaunay Triangulator for AdvCAD
 * Provides fallback strategies and robust constraint recovery
 *********************************************************************/

#pragma once

#include "constdel2d.h"
#include "robust_predicates.h"

// Enhanced CDT with robust constraint recovery and fallback strategies
class WH_RobustCDT_Triangulator : public WH_CDLN2D_Triangulator {
public:
    WH_RobustCDT_Triangulator();
    virtual ~WH_RobustCDT_Triangulator();
    
    // Main triangulation with fallback strategies
    virtual void perform() override;
    
    // Enable debugging for specific face IDs
    void setDebugFaceId(int faceId) { _debugFaceId = faceId; }
    
    // Statistics and diagnostics
    struct TriangulationStats {
        int constraintsRecovered = 0;
        int constraintsFailed = 0;
        int fallbacksUsed = 0;
        bool usedExactPredicates = false;
        bool usedFallbackStrategy = false;
        std::string failureReason;
    };
    
    const TriangulationStats& getStats() const { return _stats; }

protected:
    // Enhanced constraint recovery with multiple strategies
    virtual void fitBoundary() override;
    
    // Robust constraint recovery methods
    bool recoverConstraintSegment_robust(WH_CDLN2D_BoundarySegment* segment);
    bool insertConstraintSegment_incremental(WH_CDLN2D_BoundarySegment* segment);
    bool insertConstraintSegment_swapping(WH_CDLN2D_BoundarySegment* segment);
    
    // Fallback triangulation strategies
    bool performFallbackTriangulation();
    bool earClippingTriangulation();
    bool monotonePartitionTriangulation();
    bool simpleFanTriangulation();
    
    // Validation and cleanup
    bool validateTriangulation();
    bool hasMixedTriangles();
    void removeDummyTriangles();
    void removeDummyPoints_robust();
    
    // Debugging support
    void dumpTriangulationState(const std::string& stage);
    void visualizeFailedConstraints();
    
private:
    int _debugFaceId = -1;
    TriangulationStats _stats;
    
    // Strategy selection based on geometry complexity
    enum TriangulationStrategy {
        STANDARD_CDT,
        ROBUST_CDT_WITH_RECOVERY,
        EAR_CLIPPING_FALLBACK,
        MONOTONE_PARTITION_FALLBACK,
        SIMPLE_FAN_FALLBACK
    };
    
    TriangulationStrategy selectStrategy();
    bool isSimplePolygon();
    double estimateGeometryComplexity();
};

// Factory function for creating appropriate triangulator
WH_CDLN2D_Triangulator* createRobustTriangulator(int faceId = -1);