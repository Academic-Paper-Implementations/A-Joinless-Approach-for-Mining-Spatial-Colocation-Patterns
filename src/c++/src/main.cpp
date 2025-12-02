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
    std::cout << "Loaded " << instances.size() << " spatial instances.\n";

    // // 3. Build Spatial Index (Truyền tham số d từ config)
    // SpatialIndex spatial_idx(config.neighborDistance);
    // auto neighbor_pairs = spatial_idx.find_neighbor_pairs(objects);

    // // 4. Materialize Neighborhoods
    // NeighborhoodMgr neighbor_mgr;
    // neighbor_mgr.build_from_pairs(objects, neighbor_pairs);

    // // 5. Mining (Truyền threshold từ config)
    // JoinlessMiner miner(config.min_prevalence, &neighbor_mgr);
    // auto rules = miner.mine(objects);

    // // 6. Output (Có thể thêm logic ghi file dựa trên config.output_path)
    // // ...

    return 0;
}