#pragma once
#include "types.h"
#include <vector>

/*
    * SpatialIndex class for managing spatial indexing and neighbor searches.
    */
class SpatialIndex {
private:
    // Distance threshold for neighbor determination
    double distanceThreshold;

    // Calculate Euclidean distance between two spatial instances
    double euclideanDist(const SpatialInstance& a, const SpatialInstance& b);
    
public:
    // Constructor to initialize SpatialIndex with a distance threshold
    SpatialIndex(double distThresh);

    // Find all neighbor pairs within the distance threshold
    std::vector<std::pair<int, int>> findNeighborPair(const std::vector<SpatialInstance>& instances);
};