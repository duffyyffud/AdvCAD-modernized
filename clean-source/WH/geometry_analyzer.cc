/********************************************************************
 * Geometry Analyzer Implementation
 * Analyzes 3D geometry to determine appropriate mesh size ranges
 *********************************************************************/

#include "geometry_analyzer.h"
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

void WH_GeometryAnalyzer::GeometryMetrics::print() const {
    cout << "Geometry Analysis Results:" << endl;
    cout << "  Bounding box diagonal: " << boundingBoxDiagonal << endl;
    cout << "  Minimum feature size: " << minimumFeatureSize << endl;
    cout << "  Average feature size: " << averageFeatureSize << endl;
    cout << "  Minimum edge length: " << minimumEdgeLength << endl;
    cout << "  Aspect ratio: " << aspectRatio << endl;
    cout << "  Total faces: " << totalFaces << endl;
    cout << "  Total edges: " << totalEdges << endl;
    cout << "  Total vertices: " << totalVertices << endl;
    cout << "  Recommended mesh size: " << recommendedMeshSize << endl;
    cout << "  Safe mesh size range: [" << minimumSafeMeshSize 
         << ", " << maximumUsefulMeshSize << "]" << endl;
}

WH_GeometryAnalyzer::GeometryMetrics WH_GeometryAnalyzer::analyze(const WH_GM3D_Body& body) {
    GeometryMetrics metrics;
    
    // Get bounding box
    WH_Vector3D minCorner, maxCorner;
    body.getRange(minCorner, maxCorner);
    
    WH_Vector3D diagonal = maxCorner - minCorner;
    metrics.boundingBoxDiagonal = diagonal.length();
    
    // Compute aspect ratio
    double sizeX = abs(diagonal.x);
    double sizeY = abs(diagonal.y); 
    double sizeZ = abs(diagonal.z);
    
    double maxSize = max({sizeX, sizeY, sizeZ});
    double minSize = min({sizeX, sizeY, sizeZ});
    metrics.aspectRatio = (minSize > 0) ? maxSize / minSize : 1.0;
    
    // Analyze all geometric features
    double totalEdgeLength = 0.0;
    int edgeCount = 0;
    
    // Count and analyze faces
    const vector<WH_GM3D_Face*>& faces = body.face_s();
    for (vector<WH_GM3D_Face*>::const_iterator i_face = faces.begin();
         i_face != faces.end(); i_face++) {
        WH_GM3D_Face* face = *i_face;
        if (face != WH_NULL) {
            analyzeFace(face, metrics);
            metrics.totalFaces++;
        }
    }
    
    // Count and analyze edges
    const vector<WH_GM3D_Edge*>& edges = body.edge_s();
    for (vector<WH_GM3D_Edge*>::const_iterator i_edge = edges.begin();
         i_edge != edges.end(); i_edge++) {
        WH_GM3D_Edge* edge = *i_edge;
        if (edge != WH_NULL) {
            analyzeEdge(edge, metrics);
            metrics.totalEdges++;
            
            WH_Segment3D segment = edge->segment();
            double length = segment.length();
            totalEdgeLength += length;
            edgeCount++;
        }
    }
    
    // Count vertices
    metrics.totalVertices = body.vertex_s().size();
    
    // Compute average feature size
    if (edgeCount > 0) {
        metrics.averageFeatureSize = totalEdgeLength / edgeCount;
    }
    
    // Set minimum feature size based on analysis
    metrics.minimumFeatureSize = min(metrics.minimumEdgeLength, 
                                    metrics.minimumFeatureSize);
    
    // Compute mesh size recommendations
    metrics.recommendedMeshSize = recommendMeshSize(metrics);
    metrics.minimumSafeMeshSize = max(
        metrics.minimumFeatureSize * MINIMUM_FEATURE_RATIO,
        metrics.boundingBoxDiagonal * NUMERICAL_PRECISION_RATIO
    );
    metrics.maximumUsefulMeshSize = metrics.minimumFeatureSize * MAXIMUM_FEATURE_RATIO;
    
    return metrics;
}

void WH_GeometryAnalyzer::analyzeFace(const WH_GM3D_Face* face, GeometryMetrics& metrics) {
    if (face == WH_NULL) return;
    
    // Get face bounding box to estimate characteristic size
    WH_Vector3D minRange, maxRange;
    face->getRange(minRange, maxRange);
    
    WH_Vector3D diagonal = maxRange - minRange;
    double area = diagonal.x * diagonal.y + diagonal.y * diagonal.z + diagonal.x * diagonal.z;
    
    if (area > 0 && area < metrics.minimumFaceArea) {
        metrics.minimumFaceArea = area;
    }
    
    // Update minimum feature size based on face diagonal
    double faceCharacteristicSize = diagonal.length();
    if (faceCharacteristicSize > 0 && faceCharacteristicSize < metrics.minimumFeatureSize) {
        metrics.minimumFeatureSize = faceCharacteristicSize;
    }
}

void WH_GeometryAnalyzer::analyzeEdge(const WH_GM3D_Edge* edge, GeometryMetrics& metrics) {
    if (edge == WH_NULL) return;
    
    WH_Segment3D segment = edge->segment();
    double length = segment.length();
    if (length > 0 && length < metrics.minimumEdgeLength) {
        metrics.minimumEdgeLength = length;
    }
}

double WH_GeometryAnalyzer::computeMinimumFeatureSize(const WH_GM3D_Body& body) {
    double minFeatureSize = std::numeric_limits<double>::max();
    
    // Check all edges
    const vector<WH_GM3D_Edge*>& edges = body.edge_s();
    for (vector<WH_GM3D_Edge*>::const_iterator i_edge = edges.begin();
         i_edge != edges.end(); i_edge++) {
        WH_GM3D_Edge* edge = *i_edge;
        if (edge != WH_NULL) {
            WH_Segment3D segment = edge->segment();
            double length = segment.length();
            if (length > 0 && length < minFeatureSize) {
                minFeatureSize = length;
            }
        }
    }
    
    return minFeatureSize;
}

double WH_GeometryAnalyzer::computeMinimumEdgeLength(const WH_GM3D_Body& body) {
    return computeMinimumFeatureSize(body); // Same for now
}

double WH_GeometryAnalyzer::computeMinimumGap(const WH_GM3D_Body& body) {
    // TODO: Implement gap analysis between faces
    // For now, return edge-based estimate
    return computeMinimumFeatureSize(body) * 0.5;
}

double WH_GeometryAnalyzer::computeMinimumThickness(const WH_GM3D_Body& body) {
    // TODO: Implement thickness analysis
    // For now, return edge-based estimate  
    return computeMinimumFeatureSize(body) * 0.1;
}

double WH_GeometryAnalyzer::recommendMeshSize(const GeometryMetrics& metrics) {
    // Use 20% of minimum feature size as default recommendation
    double recommended = metrics.minimumFeatureSize * RECOMMENDED_FEATURE_RATIO;
    
    // Ensure it's within safe bounds
    recommended = max(recommended, metrics.minimumSafeMeshSize);
    recommended = min(recommended, metrics.maximumUsefulMeshSize);
    
    return recommended;
}

bool WH_GeometryAnalyzer::isMeshSizeAppropriate(double meshSize, const GeometryMetrics& metrics) {
    // Rule 1: Must be above numerical precision limit
    if (meshSize < metrics.boundingBoxDiagonal * NUMERICAL_PRECISION_RATIO) {
        cerr << "WARNING: Mesh size " << meshSize 
             << " below numerical precision limit " 
             << (metrics.boundingBoxDiagonal * NUMERICAL_PRECISION_RATIO) << endl;
        return false;
    }
    
    // Rule 2: Should be at least 5% of minimum feature
    if (meshSize < metrics.minimumFeatureSize * MINIMUM_FEATURE_RATIO) {
        cerr << "WARNING: Mesh size " << meshSize 
             << " too small for geometry features (min: " 
             << (metrics.minimumFeatureSize * MINIMUM_FEATURE_RATIO) << ")" << endl;
        return false;
    }
    
    // Rule 3: Should not exceed 50% of minimum feature
    if (meshSize > metrics.minimumFeatureSize * MAXIMUM_FEATURE_RATIO) {
        cerr << "WARNING: Mesh size " << meshSize 
             << " too large, may miss features (max: " 
             << (metrics.minimumFeatureSize * MAXIMUM_FEATURE_RATIO) << ")" << endl;
        return false;
    }
    
    // Rule 4: Should provide sufficient resolution
    if (metrics.minimumFeatureSize / meshSize < MINIMUM_ELEMENTS_PER_FEATURE) {
        cerr << "WARNING: Insufficient mesh resolution for features (need at least " 
             << MINIMUM_ELEMENTS_PER_FEATURE << " elements per feature)" << endl;
        return false;
    }
    
    return true;
}

double WH_GeometryAnalyzer::adjustMeshSize(double requested, const GeometryMetrics& metrics) {
    double adjusted = requested;
    
    // Enforce numerical precision minimum
    double numericalMin = metrics.boundingBoxDiagonal * NUMERICAL_PRECISION_RATIO;
    if (adjusted < numericalMin) {
        cerr << "INFO: Adjusting mesh size from " << requested 
             << " to " << numericalMin << " (numerical precision limit)" << endl;
        adjusted = numericalMin;
    }
    
    // Enforce feature-based minimum
    double featureMin = metrics.minimumFeatureSize * MINIMUM_FEATURE_RATIO;
    if (adjusted < featureMin) {
        cerr << "INFO: Adjusting mesh size from " << adjusted
             << " to " << featureMin << " (feature size limit)" << endl;
        adjusted = featureMin;
    }
    
    // Warn if exceeding maximum useful size
    double featureMax = metrics.minimumFeatureSize * MAXIMUM_FEATURE_RATIO;
    if (adjusted > featureMax) {
        cerr << "WARNING: Mesh size " << adjusted 
             << " exceeds recommended maximum " << featureMax 
             << " (may miss geometric features)" << endl;
        // Don't automatically reduce - user may want coarse mesh
    }
    
    return adjusted;
}