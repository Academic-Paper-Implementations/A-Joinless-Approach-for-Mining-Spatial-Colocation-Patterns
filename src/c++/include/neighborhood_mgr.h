/**
 * @file neighborhood_mgr.h
 * @brief Star neighborhood management for spatial colocation mining
 * 
 * Provides data structures and methods for building and querying star neighborhoods,
 * which are central to the joinless colocation mining algorithm.
 */

#pragma once
#include "types.h"
#include <unordered_map>
#include <vector>

/**
 * @brief Manages star neighborhoods for all spatial instances
 * 
 * A star neighborhood consists of a center instance and all its spatial neighbors.
 * This class organizes star neighborhoods by feature type for efficient access
 * during the mining process.
 */
class NeighborhoodMgr {
private:
    /// Map from feature type to all star neighborhoods of that type
    std::unordered_map<FeatureType, std::vector<StarNeighborhood>> starNeighborhoods;

public:
    /**
     * @brief Build star neighborhoods from neighbor pairs
     * 
     * Constructs star neighborhoods by grouping neighbor pairs around their
     * center instances. Each instance becomes a center with its neighbors.
     * 
     * @param pairs Vector of neighbor pairs (from spatial index)
     */
    void buildFromPairs(const std::vector<std::pair<SpatialInstance, SpatialInstance>>& pairs);

    /**
     * @brief Get the star neighborhood for a specific instance
     * 
     * @param id Instance ID to look up
     * @return const StarNeighborhood* Pointer to star neighborhood, or nullptr if not found
     */
    const StarNeighborhood* getStarNeighborhood(instanceID id) const;
    
    /**
     * @brief Get all star neighborhoods organized by feature type
     * 
     * @return const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& 
     *         Map from feature types to their star neighborhoods
     */
    const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& getAllStarNeighborhoods() const;

    /**
     * @brief Check if two instances are neighbors
     * 
     * @param id1 First instance ID
     * @param id2 Second instance ID
     * @return bool True if instances are neighbors, false otherwise
     */
    bool areNeighbors(instanceID id1, instanceID id2) const;
};