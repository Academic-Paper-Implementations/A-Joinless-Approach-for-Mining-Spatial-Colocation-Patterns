#include "config.h"
#include "data_loader.h"
#include "spatial_index.h"
#include "neighborhood_mgr.h"
#include "miner.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Load Configuration
    std::string config_path = (argc > 1) ? argv[1] : "../config.txt";
    AppConfig config = ConfigLoader::load(config_path);

    std::cout << "Running Joinless with d=" << config.neighborDistance << "...\n";

    // 2. Load Data
    auto instances = DataLoader::load_csv(config.datasetPath);

    // 3. Build Spatial Index (Truyền tham số d từ config)
    SpatialIndex spatial_idx(config.neighborDistance);
    auto neighborPairs = spatial_idx.findNeighborPair(instances);

    // 4. Materialize Neighborhoods
    NeighborhoodMgr neighbor_mgr;
    neighbor_mgr.buildFromPairs(instances, neighborPairs);
    for (const auto& [id, starNeigh] : neighbor_mgr.getAllStarNeighborhoods()) {
        std::cout << "Instance " << id << " has " << starNeigh.neighbors.size() << " neighbors.\n";
        for (const auto* neighbor : starNeigh.neighbors) {
            std::cout << "  Neighbor ID: " << neighbor->id << "\n";
        }
    }

    // // 5. Mining (Truyền threshold từ config)
    // JoinlessMiner miner(config.min_prevalence, &neighbor_mgr);
    // auto rules = miner.mine(objects);

    // // 6. Output (Có thể thêm logic ghi file dựa trên config.output_path)
    // // ...

    return 0;
}