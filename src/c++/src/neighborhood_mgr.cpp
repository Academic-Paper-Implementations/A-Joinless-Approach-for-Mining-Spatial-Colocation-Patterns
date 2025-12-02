#include "neighborhood_mgr.h"
#include <algorithm>

void NeighborhoodMgr::buildFromPairs(const std::vector<SpatialInstance>& instances,
                                    const std::vector<std::pair<instanceID, instanceID>>& pairs) {
    for (const auto& instance : instances) {
        starNeighborhoods[instance.id] = StarNeighborhood{&instance, {}};
    };
    for (const auto& pair : pairs) {
        const instanceID& id1 = pair.first;

        auto it1 = starNeighborhoods.find(id1);

        if (it1 != starNeighborhoods.end()) {
            it1->second.neighbors.push_back(starNeighborhoods[pair.second].center);
        }
    }
/* Sort the key and value in starNeighborhoods*/
    for (auto& [id, starNeigh] : starNeighborhoods) {
        std::sort(starNeigh.neighbors.begin(), starNeigh.neighbors.end(),
                  [](const SpatialInstance* a, const SpatialInstance* b) {
                      return a->id < b->id;
                  });
    }

    return;
}

const std::map<instanceID, StarNeighborhood>& NeighborhoodMgr::getAllStarNeighborhoods() const {
    return starNeighborhoods;
}