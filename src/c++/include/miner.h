#pragma once
#include "types.h"
#include "neighborhood_mgr.h"
#include <vector>
#include <map>
#include <functional>

// Progress callback type: (currentStep, totalSteps, message, percentage)
using ProgressCallback = std::function<void(int, int, const std::string&, double)>;

class JoinlessMiner {
private:
    double minPrev;
    NeighborhoodMgr* neighborhoodMgr;
    ProgressCallback progressCallback;

 
    std::vector <ColocationInstance> filterStarInstances(
        const std::vector<Colocation>& candidates,
        const std::pair<FeatureType, std::vector<StarNeighborhood>>& starNeigh
    );

    std::vector <ColocationInstance> filterCliqueInstances(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        const std::vector<ColocationInstance>& prevInstances
    );

    std::vector<Colocation> selectPrevColocations(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        double minPrev,
        const std::map<FeatureType, int>& featureCount
    );

public:
    std::vector<Colocation> mineColocations(
        double minPrevalence, 
        NeighborhoodMgr* nbrMgr, 
        const std::vector<SpatialInstance>& instances,
        ProgressCallback progressCb = nullptr
    );
    // Public method
    std::vector<Colocation> generateCandidates(
        const std::vector<Colocation>& prevPrevalent
    );
};