/**
 * @file spatial_index.cpp
 * @brief Implementation of spatial indexing and neighbor search
 */

#include "spatial_index.h"
#include <cmath>
#include <iostream>

SpatialIndex::SpatialIndex(double distThresh)
    : distanceThreshold(distThresh)
{

}

double SpatialIndex::euclideanDist(const SpatialInstance& a, const SpatialInstance& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<std::pair<SpatialInstance, SpatialInstance>> SpatialIndex::findNeighborPair(const std::vector<SpatialInstance>& instances) {
    std::vector<std::pair<SpatialInstance, SpatialInstance>> neighborPairs;

    int N = instances.size();

    // Brute-force approach: compare all pairs of instances (O(n²))
    // Suitable for moderate-sized datasets; for large datasets, consider spatial indexing (e.g., R-tree)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            double dist = euclideanDist(instances[i], instances[j]);
            
            // Check if distance is within threshold
            if (dist <= distanceThreshold) {
                neighborPairs.push_back({instances[i], instances[j]});
            }
        }
    }
    
    return neighborPairs;
}