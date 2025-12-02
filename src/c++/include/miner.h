#pragma once
#include "types.h"
#include "neighborhood_mgr.h"
#include <vector>
#include <map>


class JoinlessMiner {
private:
    double minPrev;
    NeighborhoodMgr* neighborhoodMgr;

    std::vector <Colocation> generateCandidates(const std::vector<Colocation>& prevPrevalent, int k);

    std::vector <ColocationInstance> filterStarInstances(
        const Colocation& candidate,
        const std::vector<SpatialInstance>& instances
    );

    std::vector <ColocationInstance> filterCliqueInstances(
        const Colocation& colocation,
        const std::vector<ColocationInstance>& instances,
        int totalObjectCount
    );

public:
    JoinlessMiner(double minPrevalence, NeighborhoodMgr* nbrMgr);

    std::vector<ColocationRule> mineColocations(const std::vector<SpatialInstance>& instances);
};