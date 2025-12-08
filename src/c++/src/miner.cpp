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
        std::vector<Colocation> candidates = generateCandidates(prevColocations);
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
    std::vector<Colocation> candidates;
    
    if (prevPrevalent.empty()) {
        return candidates;
    }
    
    size_t patternSize = prevPrevalent[0].size();
    
    std::set<Colocation> prevSet(prevPrevalent.begin(), prevPrevalent.end());
    
    // Generate candidate
    for (size_t i = 0; i < prevPrevalent.size(); i++) {
        for (size_t j = i + 1; j < prevPrevalent.size(); j++) {
            // Take prefix of k-1 first element
            Colocation prefix1(prevPrevalent[i].begin(), 
                             prevPrevalent[i].end() - 1);
            Colocation prefix2(prevPrevalent[j].begin(), 
                             prevPrevalent[j].end() - 1);
            
            // Just join when the prefix is equal
            if (prefix1 != prefix2) {
                continue;
            }
            
            // Generate new candidate
            std::set<FeatureType> candidateSet(prevPrevalent[i].begin(), 
                                              prevPrevalent[i].end());
            candidateSet.insert(prevPrevalent[j].back());
            
            if (candidateSet.size() != patternSize + 1) {
                continue;
            }
            
            Colocation candidate(candidateSet.begin(), candidateSet.end());
            
            // APRIORI PRUNING
            bool allSubsetsValid = true;
            std::vector<FeatureType> candFeatures = candidate;
            
            for (size_t idx = 0; idx < candFeatures.size(); idx++) {
                Colocation subset = candFeatures; 
                subset.erase(subset.begin() + idx);
                
                if (prevSet.find(subset) == prevSet.end()) {
                    allSubsetsValid = false;
                    break;
                }
            }
            
            if (allSubsetsValid) {
                candidates.push_back(candidate);
            }
        }
    }
    
    // Remove duplicate
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), 
                     candidates.end());
    
    return candidates;
}

std::vector<ColocationInstance> JoinlessMiner::filterStarInstances(const std::vector<Colocation>& candidates, 
    const StarNeighborhood& starNeigh) {
}