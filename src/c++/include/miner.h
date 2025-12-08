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
        const std::vector<ColocationInstance>& instances
    );

    std::vector<Colocation> selectCoarsePrevalent(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        double minPrev
    );

    std::vector<Colocation> selectPrevColocations(
        const std::vector<Colocation>& candidates,
        const std::vector<ColocationInstance>& instances,
        double minPrev
    );

public:
    std::vector<ColocationRule> mineColocations(double minPrevalence, double minCondProb, NeighborhoodMgr* nbrMgr, const std::vector<SpatialInstance>& instances);
    // Public method
    std::vector<Colocation> generateCandidates(
        const std::vector<Colocation>& prevPrevalent
    );

    std::vector<ColocationRule> genColocationRule(
        const std::vector<Colocation>& prevalentColocations,
        double minCondProb
    );
};