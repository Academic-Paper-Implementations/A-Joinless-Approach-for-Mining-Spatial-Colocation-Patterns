#include "miner.h"
#include "utils.h"
#include "neighborhood_mgr.h"
#include "types.h"
#include <algorithm>
#include <set>
#include <string>

std::vector<ColocationRule> JoinlessMiner::mineColocations(double minPrev, double minCondProb, NeighborhoodMgr* neighborMgr, const std::vector<SpatialInstance>& instances) {
    int k = 2;
    std::vector<FeatureType> types = getAllObjectTypes(instances);
    std::vector<Colocation> prevColocations;
    std::vector<ColocationInstance> starInstances;
    std::vector<ColocationInstance> cliqueInstances;
    std::vector<ColocationRule> discoveredRules;

    for (auto t : types) prevColocations.push_back({t});

    while (!prevColocations.empty()) {
        std::vector<Colocation> candidates = generateCandidates(
            std::vector<Colocation>(prevColocations.begin(), prevColocations.end())
        );
        for (auto t: types) {
            for (const auto& starNeigh : neighborhoodMgr->getAllStarNeighborhoods().at(t)) {
                std::vector<ColocationInstance> found = filterStarInstances(candidates, starNeigh);
                starInstances.insert(starInstances.end(), found.begin(), found.end());
            }
        }
        if (k==2){
            cliqueInstances = starInstances;
        }else{
            candidates = selectCoarsePrevalent(candidates, starInstances, minPrev);
            cliqueInstances = filterCliqueInstances(
                candidates,
                starInstances
            );
        }
        prevColocations = selectPrevColocations(
            candidates,
            cliqueInstances,
            minPrev
        );
        std::vector<ColocationRule> newRules = genColocationRule(prevColocations, minCondProb);

        discoveredRules.insert(
            discoveredRules.end(),
            newRules.begin(),
            newRules.end()
        );
        k++;
    }
    return discoveredRules;
}


std::vector<Colocation> JoinlessMiner::generateCandidates(
    const std::vector<Colocation>& prevPrevalent) 
{
    
}

std::vector<ColocationInstance> JoinlessMiner::filterStarInstances(const std::vector<Colocation>& candidates, 
    const StarNeighborhood& starNeigh) {
}