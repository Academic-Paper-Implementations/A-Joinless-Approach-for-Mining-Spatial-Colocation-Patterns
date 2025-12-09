#pragma once
#include "types.h"
#include "neighborhood_mgr.h"
#include <vector>
#include <map>


class JoinlessMiner {
private:
    double minPrev;
    NeighborhoodMgr* neighborhoodMgr;

 
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
    std::vector<Colocation> mineColocations(double minPrevalence, NeighborhoodMgr* nbrMgr, const std::vector<SpatialInstance>& instances);
    // Public method
    std::vector<Colocation> generateCandidates(
        const std::vector<Colocation>& prevPrevalent
    );
};