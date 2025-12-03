#include "miner.h"
#include "utils.h"
#include "neighborhood_mgr.h"
#include <algorithm>
#include <set>
#include <string>

JoinlessMiner::JoinlessMiner(double minPrev, NeighborhoodMgr* neighborMgr, const std::vector<SpatialInstance>& instances)
    : minPrev(minPrev), neighborhoodMgr(neighborMgr) {
    int k = 2;
    std::vector<FeatureType> types = getAllObjectTypes(instances);
    std::vector<Colocation> prevColocations;
    for (auto t : types) prevColocations.push_back({t});

    while (!prevColocations.empty()) {
        std::vector<Colocation> candidates = generateCandidates(
            std::vector<Colocation>(prevColocations.begin(), prevColocations.end()), 
            k, 
            instances
        );
        for (int i=0; i<getAllObjectTypes(instances).size(); ++i) {
            for (const auto& starNeighborhood: neighborhoodMgr->getAllStarNeighborhoods()) {   
                if (prevColocations[i][0]==starNeighborhood.first) {
                    std::vector<ColocationInstance> starInstances = filterStarInstances(
                        candidates,
                        starNeighborhood,
                    );
                    }
                }
            }
        }
    }
}

std::vector<Colocation> JoinlessMiner::generateCandidates(
    const std::vector<Colocation>& prevPrevalent, 
    int k,
    const std::vector<SpatialInstance>& instances) 
{
    std::vector<Colocation> candidates;
    
    if (k == 1) {
        // Create singleton colocation from prevalent features
        auto allFeatures = getAllObjectTypes(instances);
        for (const auto& feature : allFeatures) {
            candidates.push_back({feature});
        }
        
        return candidates;
    }
    
    // If k >= 2: Generate from F(k-1) × F(1)
    int n = prevPrevalent.size();
    
    for (int i = 0; i < n; ++i) {
        const Colocation& c1 = prevPrevalent[i];
        
        auto allFeatures = getAllObjectTypes(instances);
        
        for (const auto& newObj : allFeatures) {
            // Condition 1: Avoid duplicates - newObj must be > last object
            if (newObj > c1.back()) {
                Colocation candidate = c1;
                candidate.push_back(newObj);
                
                // Condition 2: Check Apriori property
                // Check if all (k-1) subsets of candidate are prevalent
                // Here, since c1 is already prevalent, we only need to check if any subset is not prevalent
                // But since we generate from prevPrevalent, all subsets are prevalent
                
                // Check if candidate does not already exist
                if (std::find(candidates.begin(), candidates.end(), candidate) == candidates.end()) {
                    candidates.push_back(candidate);
                }
            }
        }
    }
    
    return candidates;
}

std::vector<ColocationInstance> JoinlessMiner::filterStarInstances(const std::vector<Colocation>& candidates, 
    const std::unordered_map<FeatureType, std::vector<StarNeighborhood>>& starNeighborhood) {
        
    std::vector<ColocationInstance> starInstances;
    
    for (const auto& candidate: candidates) {
        for (const auto& instance : instances) {
            if (instance.type == candidate[0]) {
                starInstances.push_back({instance.id});
            }
        }
    }
        
    return starInstances;
}