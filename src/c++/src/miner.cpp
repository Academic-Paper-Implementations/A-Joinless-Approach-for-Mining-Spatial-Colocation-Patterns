/**
 * @file miner.cpp
 * @brief Implementation of the joinless colocation pattern mining algorithm
 * 
 * This file contains the core mining logic including candidate generation,
 * star instance filtering, clique instance filtering, and prevalence calculation.
 */

#include "miner.h"
#include "utils.h"
#include "neighborhood_mgr.h"
#include "types.h"
#include <algorithm>
#include <set>
#include <string>
#include <omp.h> 

std::vector<Colocation> JoinlessMiner::mineColocations(
    double minPrev, 
    NeighborhoodMgr* neighborhoodMgr, 
    const std::vector<SpatialInstance>& instances,
    ProgressCallback progressCb
) {
    // Assign parameters to member variables for use in other methods
    this->progressCallback = progressCb;
    
    int k = 2;
    std::vector<FeatureType> types = getAllObjectTypes(instances);
    std::map<FeatureType, int> featureCount = countInstancesByFeature(instances);
    std::vector<Colocation> prevColocations;
    std::vector<ColocationInstance> cliqueInstances;
    std::vector<ColocationInstance> prevCliqueInstances;
    std::vector<Colocation> allPrevalentColocations;

    // Estimate total iterations (max pattern size is number of types)
    int maxK = types.size();
    int currentIteration = 0;
    int totalIterations = 0; // Will be updated as we go

    if (progressCallback) {
        progressCallback(0, maxK, "Initializing mining process...", 0.0);
    }

    // Initialize with size-1 patterns (individual feature types)
    for (auto t : types) prevColocations.push_back({t});

    while (!prevColocations.empty()) {
        currentIteration++;
        totalIterations = currentIteration;
        
        // Calculate progress: use a conservative estimate that we might go up to maxK
        // But cap at 95% until we're actually done
        double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
        
        if (progressCallback) {
            progressCallback(currentIteration, maxK, 
                "Processing k=" + std::to_string(k) + " patterns...", 
                progressPercent);
        }
        
        std::vector<ColocationInstance> starInstances;
        
        // Generate candidate patterns for current level k
        std::vector<Colocation> candidates = generateCandidates(prevColocations);

        if (candidates.empty()) {
            if (progressCallback) {
                progressCallback(currentIteration, maxK, 
                    "No more candidates found. Mining completed.", 
                    100.0);
            }
            break;
        }
        
        if (progressCallback) {
            double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
            progressCallback(currentIteration, maxK, 
                "Filtering star instances for " + std::to_string(candidates.size()) + " candidates...", 
                progressPercent);
        }
        
        // Filter star instances for each feature type
        for (auto t: types) {
            for (const auto& starNeigh : neighborhoodMgr->getAllStarNeighborhoods()) {
                if (starNeigh.first == t){
                    std::vector<ColocationInstance> found = filterStarInstances(candidates, starNeigh);
                    starInstances.insert(starInstances.end(), found.begin(), found.end());
                }
            }
        }
        
        // For k=2, star instances are clique instances (no further filtering needed)
        if (k==2){
            cliqueInstances = starInstances;
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Found " + std::to_string(starInstances.size()) + " star instances (k=2)...", 
                    progressPercent);
            }
        } else {
            // For k>2, apply coarse filtering and clique instance filtering
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Selecting prevalent colocations (coarse filter)...", 
                    progressPercent);
            }
            candidates = selectPrevColocations(candidates, starInstances, minPrev, featureCount);
            
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Filtering clique instances...", 
                    progressPercent);
            }
            cliqueInstances = filterCliqueInstances(
                candidates,
                starInstances,
                prevCliqueInstances
            );
        }
        
        // Select prevalent colocations based on participation ratio
        prevColocations = selectPrevColocations(

        if (!prevColocations.empty()) {
             allPrevalentColocations.insert(
                 allPrevalentColocations.end(), 
                 prevColocations.begin(), 
                 prevColocations.end()
             );
             
             if (progressCallback) {
                 double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                 progressCallback(currentIteration, maxK, 
                     "Found " + std::to_string(prevColocations.size()) + " prevalent k=" + std::to_string(k) + " colocations", 
                     progressPercent);
             }
        } else {
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "No prevalent k=" + std::to_string(k) + " colocations found", 
                    progressPercent);
            }
        }
        
        prevCliqueInstances = std::move(cliqueInstances);

        k++;  // Move to next pattern size
    }
    
    if (progressCallback) {
        progressCallback(maxK, maxK, 
            "Mining completed! Total prevalent colocations: " + std::to_string(allPrevalentColocations.size()), 
            100.0);
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
    
    // Filter candidates that have this feature type as their first element (center)
    std::vector<const Colocation*> relevantCandidates;
    for (const auto& cand : candidates) {
        if (!cand.empty() && cand[0] == centerType) {
            relevantCandidates.push_back(&cand);
        }
    }

    if (relevantCandidates.empty()) return filteredInstances;

    // Iterate through all star neighborhoods of this feature type
    for (const auto& star : starNeigh.second) {
        
        // Build a map from feature type to neighbor instance for fast lookup
        // Using const pointer because star neighborhood contains const pointers
        std::unordered_map<FeatureType, const SpatialInstance*> neighborMap;
        
        for (auto neighbor : star.neighbors) {
            neighborMap[neighbor->type] = neighbor;
        }

        for (const auto* candPtr : relevantCandidates) {
            const auto& candidate = *candPtr;
            
            ColocationInstance instance;
            instance.reserve(candidate.size());
            
            // Add center instance as first element
            instance.push_back(star.center); 

            bool isFullPattern = true;

            // Check if all other features in the candidate pattern exist in this star's neighbors

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
    // STEP 1: LOOKUP OPTIMIZATION (PRE-CALCULATION)
    // Instead of iterating through prevInstances in nested loops (O(N*M)),
    // we collect all patterns (sets of FeatureTypes) from prevInstances into a Set.
    // This reduces existence checking to O(log M) or O(1).
    std::set<std::set<FeatureType>> validSubPatterns;
    for (const auto& prevInst : prevInstances) {
        std::set<FeatureType> pTypes;
        for (const auto& ptr : prevInst) {
            pTypes.insert(ptr->type);
        }
        validSubPatterns.insert(pTypes);
    }

    // STEP 2: MULTI-THREADING PREPARATION
    // Get maximum number of threads
    int num_threads = omp_get_max_threads();
    // Create separate buffers for each thread to avoid race conditions during push_back
    std::vector<std::vector<ColocationInstance>> thread_buffers(num_threads);

    // STEP 3: PARALLEL PROCESSING
    #pragma omp parallel for
    for (size_t i = 0; i < instances.size(); ++i) {
        const auto& instance = instances[i];
        int thread_id = omp_get_thread_num();

        // Extract feature types from current instance for comparison
        std::set<FeatureType> instanceFeatures;
        for (const auto& instPtr : instance) {
            instanceFeatures.insert(instPtr->type);
        }

        for (const auto& candidate : candidates) {
            std::set<FeatureType> candidateSet(candidate.begin(), candidate.end());

            // Check if instance contains this candidate pattern
            if (std::includes(instanceFeatures.begin(), instanceFeatures.end(),
                              candidateSet.begin(), candidateSet.end())) {
                
                bool allSubsetsExist = true;

                // Check all (k-1) subsets of the candidate
                for (size_t k = 0; k < candidate.size(); ++k) {
                    // Create subset by removing the k-th element
                    std::set<FeatureType> subsetSet = candidateSet;
                    auto it = subsetSet.begin();
                    std::advance(it, k);  // Move iterator to position k
                    subsetSet.erase(it);

                    // FAST LOOKUP: Check if subset exists in previous data
                    // Instead of looping through prevInstances, just find in the set created in step 1
                    if (validSubPatterns.find(subsetSet) == validSubPatterns.end()) {
                        allSubsetsExist = false;
                        break;
                    }
                }

                if (allSubsetsExist) {
                    // Write to current thread's private buffer (thread-safe)
                    thread_buffers[thread_id].push_back(instance);
                    break;  // Found matching candidate, no need to check other candidates for this instance
                }
            }
        }
    }

    // STEP 4: MERGE RESULTS
    // Aggregate data from thread buffers into main result vector
    std::vector<ColocationInstance> filteredInstances;
    for (const auto& buffer : thread_buffers) {
        filteredInstances.insert(filteredInstances.end(), buffer.begin(), buffer.end());
    }

    return filteredInstances;
}


std::vector<Colocation> JoinlessMiner::selectPrevColocations(
    const std::vector<Colocation>& candidates, 
    const std::vector<ColocationInstance>& instances, 
    double minPrev, 
    const std::map<FeatureType, int>& featureCount) 
{
    std::vector<Colocation> coarsePrevalent;

    // STEP 1: Data structure for aggregation
    // Key: Candidate (Pattern)
    // Value: Map<FeatureType, Set<InstanceID>> - count unique instances for each feature
    std::map<Colocation, std::map<FeatureType, std::set<std::string>>> candidateStats;

    // Initialize stats map for all candidates (ensures all candidates are present even without instances)
    // This step costs O(C), very fast compared to O(C*I)
    for (const auto& cand : candidates) {
        candidateStats[cand];  // Create empty entry
    }

    // STEP 2: Single pass through instances
    // Complexity: O(I * K * log(K)) where K is pattern length (usually very small)
    for (const ColocationInstance& instance : instances) {
        // 2a. Extract pattern from instance
        // Example: Instance has A1, B1 -> Pattern is {A, B}
        Colocation patternKey;
        for (const auto& instPtr : instance) {
            patternKey.push_back(instPtr->type);
        }
        // Ensure patternKey is sorted to match key in map (if candidate is already sorted)
        // std::sort(patternKey.begin(), patternKey.end()); 

        // 2b. Check if this pattern is in the candidates of interest
        auto it = candidateStats.find(patternKey);
        if (it != candidateStats.end()) {
            // 2c. Update participating instances statistics
            // it->second is map<Feature, Set<ID>>
            for (const auto& instPtr : instance) {
                it->second[instPtr->type].insert(instPtr->id);
            }
        }
    }

    // STEP 3: Calculate ratios and filter (iterate through candidates)
    // Complexity: O(C * K)
    for (const auto& item : candidateStats) {
        const Colocation& candidate = item.first;
        const auto& participatingMap = item.second;  // Map<Feature, Set<ID>>

        double min_participation_ratio = 1.0;
        bool possible = true;
        
        // If no instances participate -> participatingMap may be missing features or have empty sets
        
        for (const auto& feature : candidate) {
            // Get total global instance count for this feature
            auto totalIt = featureCount.find(feature);
            if (totalIt == featureCount.end() || totalIt->second == 0) {
                possible = false;
                break;
            }
            double totalFeatureCount = static_cast<double>(totalIt->second);

            // Get count of instances participating in this pattern
            int participatedCount = 0;
            auto partIt = participatingMap.find(feature);
            if (partIt != participatingMap.end()) {
                participatedCount = partIt->second.size();
            }

            double ratio = participatedCount / totalFeatureCount;
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