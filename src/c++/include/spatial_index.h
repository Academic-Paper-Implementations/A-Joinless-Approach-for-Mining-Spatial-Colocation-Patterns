/**
 * @file spatial_index.h
 * @brief Spatial indexing and neighbor search functionality
 * 
 * Provides utilities for finding spatial neighbors based on distance thresholds,
 * which is a fundamental operation in spatial colocation mining.
 */

#pragma once
#include "types.h"
#include <vector>

/**
 * @brief Manages spatial indexing and neighbor pair discovery
 * 
 * Implements spatial neighbor search using distance-based criteria.
 * Currently uses a brute-force approach suitable for moderate-sized datasets.
 */
class SpatialIndex {
private:
    double distanceThreshold;  ///< Distance threshold for neighbor determination

    /**
     * @brief Calculate Euclidean distance between two spatial instances
     * 
     * @param a First spatial instance
     * @param b Second spatial instance
     * @return double Euclidean distance between a and b
     */
    double euclideanDist(const SpatialInstance& a, const SpatialInstance& b);
    
public:
    /**
     * @brief Constructor to initialize SpatialIndex with a distance threshold
     * 
     * @param distThresh Maximum distance for two instances to be considered neighbors
     */
    SpatialIndex(double distThresh);

    /**
     * @brief Find all neighbor pairs within the distance threshold
     * 
     * Uses brute-force comparison (O(n²)) to find all pairs of instances
     * whose Euclidean distance is less than or equal to the threshold.
     * 
     * @param instances Vector of all spatial instances to search
     * @return std::vector<std::pair<SpatialInstance, SpatialInstance>> All neighbor pairs found
     * @note Returns pairs where first.id < second.id (no duplicates)
     */
    std::vector<std::pair<SpatialInstance, SpatialInstance>> findNeighborPair(const std::vector<SpatialInstance>& instances);
};