/**
 * @file miner.h
 * @brief Joinless colocation pattern mining algorithm implementation
 * 
 * Implements the joinless approach for mining spatial colocation patterns,
 * which avoids expensive spatial joins by using star neighborhoods and
 * clique-based instance filtering.
 */

#pragma once
#include "types.h"
#include "neighborhood_mgr.h"
#include <vector>
#include <map>
#include <functional>

/**
 * @brief Progress callback function type
 * 
 * Callback signature: void(currentStep, totalSteps, message, percentage)
 * Used to report mining progress to the caller.
 */
using ProgressCallback = std::function<void(int, int, const std::string&, double)>;

/**
 * @brief Implements the joinless colocation pattern mining algorithm
 * 
 * This class provides the core mining functionality using a level-wise approach
 * similar to Apriori, but without expensive spatial joins. Instead, it uses
 * star neighborhoods and clique-based filtering for efficient pattern discovery.
 */
class JoinlessMiner {
private:
    double minPrev;                          ///< Minimum prevalence threshold
    NeighborhoodMgr* neighborhoodMgr;        ///< Pointer to neighborhood manager
    ProgressCallback progressCallback;        ///< Optional progress reporting callback

    /**
     * @brief Filter star instances that match candidate patterns
     * 
     * For each star neighborhood, checks if it contains instances matching
     * the candidate patterns. This is the first filtering step.
     * 
     * @param candidates Candidate colocation patterns to check
     * @param starNeigh Star neighborhoods for a specific feature type
     * @return std::vector<ColocationInstance> Filtered colocation instances
     */
    std::vector <ColocationInstance> filterStarInstances(
        const std::vector<Colocation>& candidates,
        const std::pair<FeatureType, std::vector<StarNeighborhood>>& starNeigh
    );

    /**
     * @brief Filter clique instances from star instances
     * 
     * Ensures that all (k-1) subsets of a k-size pattern exist in previous
     * level's instances. Uses parallel processing for performance.
     * 
     * @param candidates Candidate patterns to verify
     * @param instances Star instances from current level
     * @param prevInstances Clique instances from previous level (k-1)
     * @return std::vector<ColocationInstance> Filtered clique instances
     */
    std::vector <ColocationInstance> filterCliqueInstances(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        const std::vector<ColocationInstance>& prevInstances
    );

    /**
     * @brief Select prevalent colocations based on participation ratio
     * 
     * Calculates the participation ratio for each feature in each candidate
     * pattern and selects those meeting the minimum prevalence threshold.
     * 
     * @param candidates Candidate patterns to evaluate
     * @param instances Colocation instances to analyze
     * @param minPrev Minimum prevalence threshold
     * @param featureCount Total count of instances per feature type
     * @return std::vector<Colocation> Prevalent colocation patterns
     */
    std::vector<Colocation> selectPrevColocations(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        double minPrev,
        const std::map<FeatureType, int>& featureCount
    );

public:
    /**
     * @brief Mine all prevalent colocation patterns from spatial data
     * 
     * Main entry point for the joinless mining algorithm. Uses a level-wise
     * approach to discover all prevalent colocation patterns of increasing size.
     * 
     * @param minPrevalence Minimum prevalence threshold (0.0 to 1.0)
     * @param nbrMgr Pointer to initialized neighborhood manager
     * @param instances Vector of all spatial instances
     * @param progressCb Optional callback for progress reporting
     * @return std::vector<Colocation> All discovered prevalent colocation patterns
     */
    std::vector<Colocation> mineColocations(
        double minPrevalence, 
        NeighborhoodMgr* nbrMgr, 
        const std::vector<SpatialInstance>& instances,
        ProgressCallback progressCb = nullptr
    );
    
    /**
     * @brief Generate (k+1)-size candidate patterns from k-size prevalent patterns
     * 
     * Uses Apriori-gen approach: joins patterns with matching prefixes and
     * prunes candidates whose subsets are not all prevalent.
     * 
     * @param prevPrevalent Vector of prevalent patterns from previous level
     * @return std::vector<Colocation> Generated candidate patterns for next level
     */
    std::vector<Colocation> generateCandidates(
        const std::vector<Colocation>& prevPrevalent
    );
};