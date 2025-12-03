#pragma once
#include "types.h"
#include <unordered_map>
#include <vector>

/*
    * NeighborhoodMgr class for managing star neighborhoods of spatial instances.
    */
class NeighborhoodMgr {
private:
    std::unordered_map<FeatureType, std::vector<StarNeighborhood>> starNeighborhoods;

public:
    void buildFromPairs(const std::vector<SpatialInstance>& instances,
                        const std::vector<std::pair<instanceID, instanceID>>& pairs);

    const StarNeighborhood* getStarNeighborhood(instanceID id) const;
    const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& getAllStarNeighborhoods() const;

    bool areNeighbors(instanceID id1, instanceID id2) const;
};