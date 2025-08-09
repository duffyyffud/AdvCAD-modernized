/********************************************************************
 * Topology Validator for Water-Tight Mesh Debugging
 * 
 * Non-invasive checkpoint tracking system to identify exactly where
 * mesh topology breaks during triangulation pipeline.
 *********************************************************************/

#ifndef WH_TOPOLOGY_VALIDATOR_H
#define WH_TOPOLOGY_VALIDATOR_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

// Forward declarations
class WH_DLN2D_Triangle;
class WH_DLN2D_Point;

/**
 * Checkpoint stages in the mesh generation pipeline
 */
enum TopologyCheckpoint {
    INITIAL_DELAUNAY,          // After initial Delaunay triangulation
    POST_CONSTRAINT_RECOVERY,  // After constraint recovery
    POST_DOMAIN_IDENTIFICATION, // After domain identification 
    POST_2D_REORDER,          // After triangle reordering in 2D
    POST_3D_CONVERSION,       // After conversion to 3D triangles
    FINAL_COLLECTION          // Final mesh collection
};

/**
 * Topology validation results for a single checkpoint
 */
struct TopologySnapshot {
    TopologyCheckpoint stage;
    int faceId;
    std::string stageName;
    
    // Triangle statistics
    int triangleCount;
    int validTriangleCount;
    int degenerateTriangleCount;
    
    // Edge connectivity
    int totalEdges;
    int boundaryEdges;        // Edges shared by only 1 triangle (holes)
    int manifoldEdges;        // Edges shared by exactly 2 triangles
    int nonManifoldEdges;     // Edges shared by 3+ triangles
    
    // Topology metrics
    int vertices;
    int faces;
    int edges;
    int eulerCharacteristic;  // V - E + F (should be 2 for closed surfaces)
    
    // Winding consistency
    bool hasConsistentWinding;
    int clockwiseTriangles;
    int counterClockwiseTriangles;
    
    // Status
    bool isWaterTight;
    std::string failureReason;
    
    TopologySnapshot() : 
        stage(INITIAL_DELAUNAY), faceId(-1), triangleCount(0), validTriangleCount(0),
        degenerateTriangleCount(0), totalEdges(0), boundaryEdges(0), manifoldEdges(0),
        nonManifoldEdges(0), vertices(0), faces(0), edges(0), eulerCharacteristic(0),
        hasConsistentWinding(true), clockwiseTriangles(0), counterClockwiseTriangles(0),
        isWaterTight(false) {}
};

/**
 * Non-invasive topology validator for backward tracing debugging
 */
class WH_TopologyValidator {
private:
    int _debugFaceId;
    std::vector<TopologySnapshot> _checkpoints;
    std::string _outputPrefix;
    bool _enabled;
    
    // Internal validation methods
    void analyzeEdgeConnectivity(const std::vector<WH_DLN2D_Triangle*>& triangles, TopologySnapshot& snapshot);
    void analyzeWindingConsistency(const std::vector<WH_DLN2D_Triangle*>& triangles, TopologySnapshot& snapshot);
    void calculateEulerCharacteristic(TopologySnapshot& snapshot);
    bool isTriangleDegenerate(WH_DLN2D_Triangle* triangle);
    
    // Edge representation for connectivity analysis
    struct Edge {
        int point1_id;
        int point2_id;
        
        Edge(int p1, int p2) : point1_id(std::min(p1, p2)), point2_id(std::max(p1, p2)) {}
        
        bool operator<(const Edge& other) const {
            if (point1_id != other.point1_id) return point1_id < other.point1_id;
            return point2_id < other.point2_id;
        }
    };
    
public:
    WH_TopologyValidator(int faceId = -1);
    ~WH_TopologyValidator();
    
    // Control methods
    void setDebugFaceId(int faceId) { _debugFaceId = faceId; }
    void setEnabled(bool enabled) { _enabled = enabled; }
    void setOutputPrefix(const std::string& prefix) { _outputPrefix = prefix; }
    
    // Checkpoint methods - non-invasive validation
    void recordCheckpoint(TopologyCheckpoint stage, 
                         const std::vector<WH_DLN2D_Triangle*>& triangles,
                         const std::string& customName = "");
    
    // Analysis methods
    void generateBackwardReport();
    void identifyFailureStage();
    TopologySnapshot getLastValidCheckpoint();
    TopologySnapshot getFirstBrokenCheckpoint();
    
    // Utility methods
    static std::string checkpointName(TopologyCheckpoint stage);
    static TopologyCheckpoint nextCheckpoint(TopologyCheckpoint current);
    
    // Access methods
    const std::vector<TopologySnapshot>& getCheckpoints() const { return _checkpoints; }
    bool isEnabled() const { return _enabled; }
    
    // Clear checkpoints for new face
    void reset() { _checkpoints.clear(); }
};

#endif // WH_TOPOLOGY_VALIDATOR_H