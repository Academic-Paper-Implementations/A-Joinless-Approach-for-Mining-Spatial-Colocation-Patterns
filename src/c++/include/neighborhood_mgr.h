#pragma once
#include "types.h"
#include <map>
#include <vector>

/*
    * NeighborhoodMgr class for managing star neighborhoods of spatial instances.
    */
class NeighborhoodMgr {
private:
    std::map<instanceID, StarNeighborhood> starNeighborhoods;

public:
    void buildFromPairs(const std::vector<SpatialInstance>& instances,
                        const std::vector<std::pair<instanceID, instanceID>>& pairs);

    const StarNeighborhood* getStarNeighborhood(instanceID id) const;
    const std::map<instanceID, StarNeighborhood>& getAllStarNeighborhoods() const;

    bool areNeighbors(instanceID id1, instanceID id2) const;
};