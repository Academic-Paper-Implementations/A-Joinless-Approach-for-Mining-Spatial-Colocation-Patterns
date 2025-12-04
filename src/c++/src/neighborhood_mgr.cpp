#include "neighborhood_mgr.h"
#include "utils.h"
#include <algorithm>

void NeighborhoodMgr::buildFromPairs(const std::vector<std::pair<SpatialInstance, SpatialInstance>>& pairs) {
    for (const auto& pair : pairs) {
        const SpatialInstance& center = pair.first;
        const SpatialInstance& neighbor = pair.second;
        auto& vec = starNeighborhoods[pair.first.type];

        auto it = std::find_if(vec.begin(), vec.end(), [&](const StarNeighborhood& sn) {
            return sn.center->id == center.id;
        });

        if (it != vec.end()) {
            it->neighbors.push_back(&neighbor);
        } else {
            StarNeighborhood newStarNeigh;
            newStarNeigh.center = &center;
            newStarNeigh.neighbors.push_back(&neighbor);
            vec.push_back(newStarNeigh);
        }
    }
    return;
}

const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& NeighborhoodMgr::getAllStarNeighborhoods() const {
    return starNeighborhoods;
}