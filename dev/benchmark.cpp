#include <chrono>
#include <iostream>
#include <vector>
#include "WH/space3d.h"
#include "WH/polygon3d.h"
#include "WH/sorter.h"

using namespace std;
using namespace std::chrono;

void benchmark_vector_operations() {
    cout << "=== Vector Operations Benchmark ===" << endl;
    
    const int iterations = 1000000;
    
    // Test constexpr constructor performance
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        WH_Vector3D v1(1.0, 2.0, 3.0);
        WH_Vector3D v2 = -v1;
        volatile double x = v2.x; // Prevent optimization
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Vector construction + unary minus (" << iterations << " iterations): " 
         << duration.count() << " microseconds" << endl;
    cout << "Average per operation: " << (double)duration.count() / iterations << " microseconds" << endl;
}

void benchmark_polygon_move_semantics() {
    cout << "\n=== Polygon Move Semantics Benchmark ===" << endl;
    
    const int iterations = 10000;
    const int vertices = 100;
    
    // Create test data
    vector<WH_Vector3D> vertex_data;
    for (int i = 0; i < vertices; ++i) {
        vertex_data.push_back(WH_Vector3D((double)i, (double)i*2, (double)i*3));
    }
    
    // Test move operations
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        WH_Polygon3D poly1(vertex_data);
        WH_Polygon3D poly2 = std::move(poly1); // Move constructor
        WH_Polygon3D poly3;
        poly3 = std::move(poly2); // Move assignment
        volatile int n = poly3.nVertexs(); // Prevent optimization
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Polygon move operations (" << iterations << " iterations, " << vertices << " vertices): " 
         << duration.count() << " microseconds" << endl;
    cout << "Average per operation: " << (double)duration.count() / iterations << " microseconds" << endl;
}

void benchmark_sorter_move_semantics() {
    cout << "\n=== Sorter Move Semantics Benchmark ===" << endl;
    
    const int iterations = 10000;
    const int items = 1000;
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        WH_Sorter sorter1;
        sorter1.initialize(items);
        for (int j = 0; j < items; ++j) {
            sorter1.setItemAt(j, nullptr, (double)j);
        }
        
        WH_Sorter sorter2 = std::move(sorter1); // Move constructor
        WH_Sorter sorter3;
        sorter3 = std::move(sorter2); // Move assignment
        volatile int n = sorter3.nItems(); // Prevent optimization
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Sorter move operations (" << iterations << " iterations, " << items << " items): " 
         << duration.count() << " microseconds" << endl;
    cout << "Average per operation: " << (double)duration.count() / iterations << " microseconds" << endl;
}

void benchmark_constexpr_math() {
    cout << "\n=== Constexpr Math Functions Benchmark ===" << endl;
    
    const int iterations = 10000000;
    
    auto start = high_resolution_clock::now();
    volatile double result = 0.0;
    for (int i = 0; i < iterations; ++i) {
        double a = (double)i;
        double b = (double)(i + 1);
        result += WH_min(a, b) + WH_max(a, b) + WH_interpolate(a, b, 0.5);
    }
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    cout << "Constexpr math operations (" << iterations << " iterations): " 
         << duration.count() << " microseconds" << endl;
    cout << "Average per operation: " << (double)duration.count() / iterations << " microseconds" << endl;
}

int main() {
    cout << "AdvCAD Performance Benchmark - Modernized Version" << endl;
    cout << "=================================================" << endl;
    
    benchmark_vector_operations();
    benchmark_polygon_move_semantics();
    benchmark_sorter_move_semantics();
    benchmark_constexpr_math();
    
    cout << "\nBenchmark complete!" << endl;
    return 0;
}