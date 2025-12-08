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
            for (const auto& starNeigh : neighborhoodMgr->getAllStarNeighborhoods()) {
                if (starNeigh.first == t){
                    std::vector<ColocationInstance> found = filterStarInstances(candidates, starNeigh);
                    starInstances.insert(starInstances.end(), found.begin(), found.end());
                }
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


std::vector<ColocationInstance> JoinlessMiner::filterStarInstances(
    const std::vector<Colocation>& candidates, 
    const std::pair<FeatureType, std::vector<StarNeighborhood>>& starNeigh) 
{
    std::vector<ColocationInstance> filteredInstances;
    FeatureType centerType = starNeigh.first;
    
    // Lọc candidate (Đoạn này OK)
    std::vector<const Colocation*> relevantCandidates;
    for (const auto& cand : candidates) {
        if (!cand.empty() && cand[0] == centerType) {
            relevantCandidates.push_back(&cand);
        }
    }

    if (relevantCandidates.empty()) return filteredInstances;

    // Duyệt qua các ngôi sao
    for (const auto& star : starNeigh.second) {
        
        // --- SỬA LỖI TẠI ĐÂY ---
        // Thêm 'const' vào SpatialInstance* vì star là const
        std::unordered_map<FeatureType, const SpatialInstance*> neighborMap;
        
        for (auto neighbor : star.neighbors) {
            neighborMap[neighbor->type] = neighbor; // Giờ đã gán được
        }

        for (const auto* candPtr : relevantCandidates) {
            const auto& candidate = *candPtr;
            
            ColocationInstance instance;
            instance.reserve(candidate.size());
            
            // Lưu ý: ColocationInstance cũng cần phải lưu chứa (const SpatialInstance*)
            // Nếu struct ColocationInstance của bạn đang lưu (SpatialInstance*), 
            // bạn cần const_cast (không khuyến khích) hoặc sửa định nghĩa struct đó.
            instance.push_back(star.center); 

            bool isFullPattern = true;

            for (size_t i = 1; i < candidate.size(); ++i) {
                auto it = neighborMap.find(candidate[i]);
                
                if (it != neighborMap.end()) {
                    instance.push_back(it->second);
                } else {
                    isFullPattern = false;
                    break; 
                }
            }

            if (isFullPattern) {
                filteredInstances.push_back(instance);
            }
        }
    }

    return filteredInstances;
}