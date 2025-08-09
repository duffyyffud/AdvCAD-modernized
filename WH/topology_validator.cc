/********************************************************************
 * Topology Validator Implementation
 * 
 * Backward tracing debugging system for water-tight mesh validation
 *********************************************************************/

#include "topology_validator.h"
#include "triangle2d.h"
#include "delaunay2d.h"
#include <algorithm>
#include <map>
#include <set>
#include <sstream>
#include <cmath>

WH_TopologyValidator::WH_TopologyValidator(int faceId) : 
    _debugFaceId(faceId), _outputPrefix("topology_debug"), _enabled(true) {
}

WH_TopologyValidator::~WH_TopologyValidator() {
}

void WH_TopologyValidator::recordCheckpoint(TopologyCheckpoint stage, 
                                           const std::vector<WH_DLN2D_Triangle*>& triangles,
                                           const std::string& customName) {
    if (!_enabled) return;
    
    TopologySnapshot snapshot;
    snapshot.stage = stage;
    snapshot.faceId = _debugFaceId;
    snapshot.stageName = customName.empty() ? checkpointName(stage) : customName;
    snapshot.triangleCount = triangles.size();
    
    // Count valid vs degenerate triangles
    snapshot.validTriangleCount = 0;
    snapshot.degenerateTriangleCount = 0;
    
    for (WH_DLN2D_Triangle* tri : triangles) {
        if (isTriangleDegenerate(tri)) {
            snapshot.degenerateTriangleCount++;
        } else {
            snapshot.validTriangleCount++;
        }
    }
    
    // Analyze edge connectivity (critical for water-tight detection)
    analyzeEdgeConnectivity(triangles, snapshot);
    
    // Analyze winding consistency 
    analyzeWindingConsistency(triangles, snapshot);
    
    // Calculate Euler characteristic
    calculateEulerCharacteristic(snapshot);
    
    // Determine if water-tight
    snapshot.isWaterTight = (snapshot.boundaryEdges == 0 && 
                           snapshot.nonManifoldEdges == 0 &&
                           snapshot.hasConsistentWinding &&
                           snapshot.degenerateTriangleCount == 0);
    
    if (!snapshot.isWaterTight) {
        std::ostringstream reason;
        if (snapshot.boundaryEdges > 0) reason << "BoundaryEdges:" << snapshot.boundaryEdges << " ";
        if (snapshot.nonManifoldEdges > 0) reason << "NonManifoldEdges:" << snapshot.nonManifoldEdges << " ";
        if (!snapshot.hasConsistentWinding) reason << "InconsistentWinding ";
        if (snapshot.degenerateTriangleCount > 0) reason << "DegenerateTriangles:" << snapshot.degenerateTriangleCount;
        snapshot.failureReason = reason.str();
    }
    
    _checkpoints.push_back(snapshot);
    
    // Log checkpoint if debugging this face
    if (_debugFaceId >= 0) {
        std::cout << "CHECKPOINT [" << snapshot.stageName << "] Face " << _debugFaceId 
                  << ": Triangles=" << snapshot.triangleCount
                  << " BoundaryEdges=" << snapshot.boundaryEdges
                  << " NonManifoldEdges=" << snapshot.nonManifoldEdges
                  << " WaterTight=" << (snapshot.isWaterTight ? "YES" : "NO");
        if (!snapshot.isWaterTight) {
            std::cout << " (" << snapshot.failureReason << ")";
        }
        std::cout << std::endl;
    }
}

void WH_TopologyValidator::analyzeEdgeConnectivity(const std::vector<WH_DLN2D_Triangle*>& triangles, 
                                                  TopologySnapshot& snapshot) {
    std::map<Edge, int> edgeCount;
    std::set<int> vertexSet;
    
    // Build edge-triangle connectivity map
    for (WH_DLN2D_Triangle* tri : triangles) {
        if (!tri) continue;
        
        // Add vertices to set
        for (int i = 0; i < 3; i++) {
            if (tri->point(i)) {
                vertexSet.insert(tri->point(i)->id());
            }
        }
        
        // Add edges
        for (int i = 0; i < 3; i++) {
            int next = (i + 1) % 3;
            if (tri->point(i) && tri->point(next)) {
                Edge edge(tri->point(i)->id(), tri->point(next)->id());
                edgeCount[edge]++;
            }
        }
    }
    
    // Analyze edge connectivity
    snapshot.boundaryEdges = 0;
    snapshot.manifoldEdges = 0; 
    snapshot.nonManifoldEdges = 0;
    snapshot.totalEdges = edgeCount.size();
    
    for (const auto& edgePair : edgeCount) {
        int count = edgePair.second;
        if (count == 1) {
            snapshot.boundaryEdges++;  // Hole in mesh
        } else if (count == 2) {
            snapshot.manifoldEdges++;  // Proper manifold edge
        } else {
            snapshot.nonManifoldEdges++;  // Non-manifold (3+ triangles share edge)
        }
    }
    
    snapshot.vertices = vertexSet.size();
    snapshot.faces = triangles.size();
    snapshot.edges = snapshot.totalEdges;
}

void WH_TopologyValidator::analyzeWindingConsistency(const std::vector<WH_DLN2D_Triangle*>& triangles,
                                                    TopologySnapshot& snapshot) {
    snapshot.clockwiseTriangles = 0;
    snapshot.counterClockwiseTriangles = 0;
    
    for (WH_DLN2D_Triangle* tri : triangles) {
        if (!tri || !tri->point(0) || !tri->point(1) || !tri->point(2)) continue;
        
        // Calculate signed area to determine winding
        double x1 = tri->point(0)->position().x;
        double y1 = tri->point(0)->position().y;
        double x2 = tri->point(1)->position().x;
        double y2 = tri->point(1)->position().y;
        double x3 = tri->point(2)->position().x;
        double y3 = tri->point(2)->position().y;
        
        double signedArea = ((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2.0;
        
        if (signedArea > 0) {
            snapshot.counterClockwiseTriangles++;
        } else if (signedArea < 0) {
            snapshot.clockwiseTriangles++;
        }
        // signedArea == 0 would be degenerate, counted separately
    }
    
    // Consistent winding means all triangles have same orientation
    snapshot.hasConsistentWinding = (snapshot.clockwiseTriangles == 0 || snapshot.counterClockwiseTriangles == 0);
}

void WH_TopologyValidator::calculateEulerCharacteristic(TopologySnapshot& snapshot) {
    // Euler characteristic: V - E + F = 2 for closed surfaces (water-tight)
    snapshot.eulerCharacteristic = snapshot.vertices - snapshot.edges + snapshot.faces;
}

bool WH_TopologyValidator::isTriangleDegenerate(WH_DLN2D_Triangle* triangle) {
    if (!triangle || !triangle->point(0) || !triangle->point(1) || !triangle->point(2)) {
        return true;
    }
    
    // Check for zero area (collinear points)
    double x1 = triangle->point(0)->position().x;
    double y1 = triangle->point(0)->position().y;
    double x2 = triangle->point(1)->position().x;
    double y2 = triangle->point(1)->position().y;
    double x3 = triangle->point(2)->position().x;
    double y3 = triangle->point(2)->position().y;
    
    double area = std::abs((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2.0;
    
    return area < 1e-12;  // Effectively zero area
}

void WH_TopologyValidator::generateBackwardReport() {
    if (_checkpoints.empty()) return;
    
    std::string filename = _outputPrefix + "_face" + std::to_string(_debugFaceId) + "_backward_trace.txt";
    std::ofstream report(filename);
    
    report << "=== BACKWARD TRACING TOPOLOGY REPORT ===" << std::endl;
    report << "Face ID: " << _debugFaceId << std::endl;
    report << "Total Checkpoints: " << _checkpoints.size() << std::endl;
    report << std::endl;
    
    // Find the failure transition point
    TopologySnapshot lastValid;
    TopologySnapshot firstBroken;
    bool foundTransition = false;
    
    for (size_t i = 0; i < _checkpoints.size(); i++) {
        const TopologySnapshot& checkpoint = _checkpoints[i];
        
        report << "--- CHECKPOINT " << (i+1) << ": " << checkpoint.stageName << " ---" << std::endl;
        report << "Triangles: " << checkpoint.triangleCount << " (Valid: " << checkpoint.validTriangleCount 
               << ", Degenerate: " << checkpoint.degenerateTriangleCount << ")" << std::endl;
        report << "Topology: V=" << checkpoint.vertices << " E=" << checkpoint.edges 
               << " F=" << checkpoint.faces << " (Euler χ=" << checkpoint.eulerCharacteristic << ")" << std::endl;
        report << "Edges: Boundary=" << checkpoint.boundaryEdges << " Manifold=" << checkpoint.manifoldEdges 
               << " NonManifold=" << checkpoint.nonManifoldEdges << std::endl;
        report << "Winding: Consistent=" << (checkpoint.hasConsistentWinding ? "YES" : "NO")
               << " (CW=" << checkpoint.clockwiseTriangles << " CCW=" << checkpoint.counterClockwiseTriangles << ")" << std::endl;
        report << "WATER-TIGHT: " << (checkpoint.isWaterTight ? "YES" : "NO");
        if (!checkpoint.isWaterTight) {
            report << " (" << checkpoint.failureReason << ")";
        }
        report << std::endl;
        
        // Track transition from valid to broken
        if (!foundTransition) {
            if (checkpoint.isWaterTight) {
                lastValid = checkpoint;
            } else if (lastValid.isWaterTight) {
                firstBroken = checkpoint;
                foundTransition = true;
            }
        }
        
        report << std::endl;
    }
    
    // Backward analysis conclusion
    report << "=== BACKWARD ANALYSIS CONCLUSION ===" << std::endl;
    if (foundTransition) {
        report << "TOPOLOGY BREAKS BETWEEN:" << std::endl;
        report << "  LAST VALID: " << lastValid.stageName << " (Water-tight: YES)" << std::endl;
        report << "  FIRST BROKEN: " << firstBroken.stageName << " (Failures: " << firstBroken.failureReason << ")" << std::endl;
        report << std::endl;
        report << "SURGICAL FIX TARGET: The implementation between " << lastValid.stageName 
               << " and " << firstBroken.stageName << std::endl;
        report << "ROOT CAUSE: " << firstBroken.failureReason << std::endl;
    } else {
        report << "No clear transition found - all checkpoints may be broken or all valid" << std::endl;
    }
    
    report.close();
    
    if (_debugFaceId >= 0) {
        std::cout << "BACKWARD TRACE: Report generated: " << filename << std::endl;
        if (foundTransition) {
            std::cout << "TOPOLOGY BREAKS: " << lastValid.stageName << " → " << firstBroken.stageName 
                      << " (" << firstBroken.failureReason << ")" << std::endl;
        }
    }
}

void WH_TopologyValidator::identifyFailureStage() {
    TopologySnapshot lastValid = getLastValidCheckpoint();
    TopologySnapshot firstBroken = getFirstBrokenCheckpoint();
    
    if (lastValid.isWaterTight && !firstBroken.isWaterTight) {
        std::cout << "FAILURE IDENTIFIED: Topology breaks between " 
                  << lastValid.stageName << " and " << firstBroken.stageName << std::endl;
        std::cout << "FAILURE MODE: " << firstBroken.failureReason << std::endl;
    }
}

TopologySnapshot WH_TopologyValidator::getLastValidCheckpoint() {
    for (int i = _checkpoints.size() - 1; i >= 0; i--) {
        if (_checkpoints[i].isWaterTight) {
            return _checkpoints[i];
        }
    }
    return TopologySnapshot(); // No valid checkpoint found
}

TopologySnapshot WH_TopologyValidator::getFirstBrokenCheckpoint() {
    for (const TopologySnapshot& checkpoint : _checkpoints) {
        if (!checkpoint.isWaterTight) {
            return checkpoint;
        }
    }
    return TopologySnapshot(); // No broken checkpoint found
}

std::string WH_TopologyValidator::checkpointName(TopologyCheckpoint stage) {
    switch (stage) {
        case INITIAL_DELAUNAY: return "INITIAL_DELAUNAY";
        case POST_CONSTRAINT_RECOVERY: return "POST_CONSTRAINT_RECOVERY";
        case POST_DOMAIN_IDENTIFICATION: return "POST_DOMAIN_IDENTIFICATION";
        case POST_2D_REORDER: return "POST_2D_REORDER";
        case POST_3D_CONVERSION: return "POST_3D_CONVERSION";
        case FINAL_COLLECTION: return "FINAL_COLLECTION";
        default: return "UNKNOWN_CHECKPOINT";
    }
}

TopologyCheckpoint WH_TopologyValidator::nextCheckpoint(TopologyCheckpoint current) {
    switch (current) {
        case INITIAL_DELAUNAY: return POST_CONSTRAINT_RECOVERY;
        case POST_CONSTRAINT_RECOVERY: return POST_DOMAIN_IDENTIFICATION;
        case POST_DOMAIN_IDENTIFICATION: return POST_2D_REORDER;
        case POST_2D_REORDER: return POST_3D_CONVERSION;
        case POST_3D_CONVERSION: return FINAL_COLLECTION;
        case FINAL_COLLECTION: return FINAL_COLLECTION; // Terminal state
        default: return POST_CONSTRAINT_RECOVERY;
    }
}