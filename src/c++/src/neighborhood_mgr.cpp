#include "neighborhood_mgr.h"
#include "utils.h"
#include <algorithm>

void NeighborhoodMgr::buildFromPairs(const std::vector<SpatialInstance>& instances,
                                    const std::vector<std::pair<instanceID, instanceID>>& pairs) {
    auto allFeatures = getAllObjectTypes(instances);
    for (const auto& feature : allFeatures) {
        starNeighborhoods[feature] = std::vector<StarNeighborhood>{};
        for (const auto& instance : instances) {
            if (instance.type == feature) {
                StarNeighborhood starNeigh;
                starNeigh.center = &instance;
                starNeighborhoods[feature].push_back(starNeigh);
                for (const auto& pair : pairs) {
                    if (pair.first == instance.id) {
                        // Find the neighbor instance
                        auto it = std::find_if(instances.begin(), instances.end(),
                                               [&](const SpatialInstance& inst) {
                                                   return inst.id == pair.second;
                                               });
                        if (it != instances.end()) {
                            starNeighborhoods[feature].back().neighbors.push_back(&(*it));
                        }
                    }
                }
            }
        }
    };
    return;
}

const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& NeighborhoodMgr::getAllStarNeighborhoods() const {
    return starNeighborhoods;
}