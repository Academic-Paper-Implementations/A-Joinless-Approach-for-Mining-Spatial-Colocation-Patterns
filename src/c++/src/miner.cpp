#include "miner.h"
#include "utils.h"
#include "neighborhood_mgr.h"
#include "types.h"
#include <algorithm>
#include <set>
#include <string>

std::vector<Colocation> JoinlessMiner::mineColocations(double minPrev, NeighborhoodMgr* neighborMgr, const std::vector<SpatialInstance>& instances) {
    int k = 2;
    std::vector<FeatureType> types = getAllObjectTypes(instances);
    std::map<FeatureType, int> featureCount = countInstancesByFeature(instances);
    std::vector<Colocation> prevColocations;
    std::vector<ColocationInstance> cliqueInstances;
    std::vector<ColocationInstance> prevCliqueInstances;
    std::vector<Colocation> allPrevalentColocations;


    for (auto t : types) prevColocations.push_back({t});

    while (!prevColocations.empty()) {
        std::vector<ColocationInstance> starInstances;
        std::vector<Colocation> candidates = generateCandidates(prevColocations);

        if (candidates.empty()) break;
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
            candidates = selectPrevColocations(candidates, starInstances, minPrev, featureCount);
            cliqueInstances = filterCliqueInstances(
                candidates,
                starInstances,
                prevCliqueInstances
            );
        }
        prevColocations = selectPrevColocations(
            candidates,
            cliqueInstances,
            minPrev,
            featureCount
        );

        if (!prevColocations.empty()) {
             allPrevalentColocations.insert(
                 allPrevalentColocations.end(), 
                 prevColocations.begin(), 
                 prevColocations.end()
             );
        }
        prevCliqueInstances = std::move(cliqueInstances);

        k++;
    }
    return allPrevalentColocations;
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


std::vector<ColocationInstance> JoinlessMiner::filterCliqueInstances(
    const std::vector<Colocation>& candidates,
    const std::vector<ColocationInstance>& instances,
    const std::vector<ColocationInstance>& prevInstances
) {
    std::vector<ColocationInstance> filteredInstances;
    for (const auto& instance : instances) {
        for (const auto& candidate : candidates) {
            std::set<FeatureType> instanceFeatures;
            for (const auto& instPtr : instance) {
                instanceFeatures.insert(instPtr->type);
            }

            std::set<FeatureType> candidateSet(candidate.begin(), candidate.end());

            if (std::includes(instanceFeatures.begin(), instanceFeatures.end(),
                              candidateSet.begin(), candidateSet.end())) {
                // Check all (k-1)-sized subsets exist in prevInstances
                bool allSubsetsExist = true;
                for (size_t i = 0; i < candidate.size(); ++i) {
                    Colocation subset = candidate;
                    subset.erase(subset.begin() + i);

                    bool subsetFound = false;
                    for (const auto& prevInstance : prevInstances) {
                        std::set<FeatureType> prevInstanceFeatures;
                        for (const auto& instPtr : prevInstance) {
                            prevInstanceFeatures.insert(instPtr->type);
                        }

                        std::set<FeatureType> subsetSet(subset.begin(), subset.end());
                        if (std::includes(prevInstanceFeatures.begin(), prevInstanceFeatures.end(),
                                          subsetSet.begin(), subsetSet.end())) {
                            subsetFound = true;
                            break;
                        }
                    }

                    if (!subsetFound) {
                        allSubsetsExist = false;
                        break;
                    }
                }

                if (allSubsetsExist) {
                    filteredInstances.push_back(instance);
                    break; // No need to check other candidates
                }
            }
        }
    }

    return filteredInstances;
}


std::vector<Colocation> JoinlessMiner::selectPrevColocations(const std::vector<Colocation>& candidates, const std::vector<ColocationInstance>& instances, double minPrev, const std::map<FeatureType, int>& featureCount) {
    std::vector<Colocation> coarsePrevalent;

    for (const auto& candidate : candidates) {
        //create map to track participating instances for each feature
        //eg: { "A": {"A1", "A2"}, "B": {"B1"} }
        std::map<FeatureType, std::set<std::string>> participatingInstances;

        std::set<FeatureType> candidateSet(candidate.begin(), candidate.end());

        for (const ColocationInstance& instance : instances) {
            std::set<FeatureType> instanceFeatures;
            
            for (const auto& instPtr : instance) {
                instanceFeatures.insert(instPtr->type);
            }

            if (std::includes(instanceFeatures.begin(), instanceFeatures.end(),
                              candidateSet.begin(), candidateSet.end())) {
                
                for (const auto& instPtr : instance) {
                    FeatureType fType = instPtr->type;
                    if (candidateSet.find(fType) != candidateSet.end()) {
                        participatingInstances[fType].insert(instPtr->id);
                    }
                }
            }
        }

        double min_participation_ratio = 1.0;
        bool possible = true;

        for (const auto& feature : candidate) {
            if (featureCount.find(feature) == featureCount.end() || featureCount.at(feature) == 0) {
                possible = false; 
                break;
            }
            //count total instances of this feature
            int totalFeatureCount = featureCount.at(feature);
            //count instances participating in this pattern
            int participatedCount = participatingInstances[feature].size();

            double ratio = static_cast<double>(participatedCount) / totalFeatureCount;
            
            if (ratio < min_participation_ratio) {
                min_participation_ratio = ratio;
            }
        }

        if (possible && min_participation_ratio >= minPrev) {
            coarsePrevalent.push_back(candidate);
        }
    }

    return coarsePrevalent;
}