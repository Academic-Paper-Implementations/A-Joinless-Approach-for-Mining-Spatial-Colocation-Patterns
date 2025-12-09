// #include "config.h"
// #include "data_loader.h"
// #include "spatial_index.h"
// #include "neighborhood_mgr.h"
// #include "miner.h"
// #include "utils.h"
// #include <iostream>

// int main(int argc, char* argv[]) {
//     // 1. Load Configuration
//     std::string config_path = (argc > 1) ? argv[1] : "../config.txt";
//     AppConfig config = ConfigLoader::load(config_path);

//     std::cout << "Running Joinless with d=" << config.neighborDistance << "...\n";

//     // 2. Load Data
//     auto instances = DataLoader::load_csv(config.datasetPath);

//     // 3. Build Spatial Index (Truyền tham số d từ config)
//     SpatialIndex spatial_idx(config.neighborDistance);
//     auto neighborPairs = spatial_idx.findNeighborPair(instances);
//     for (const auto& pair : neighborPairs) {
//         std::cout << "Neighbor Pair: " << pair.first.id << " - " << pair.second.id << "\n";
//     }

//     // 4. Materialize Neighborhoods
//     NeighborhoodMgr neighbor_mgr;
//     neighbor_mgr.buildFromPairs(neighborPairs);
//     for (const auto& starNeighborhood : neighbor_mgr.getAllStarNeighborhoods()) {
//         std::cout << "Star Neighborhoods for Feature: " << starNeighborhood.first << "\n";
//         for (const auto& star : starNeighborhood.second) {
//             std::cout << "  Center: " << star.center->id << " Neighbors: ";
//             for (const auto& neighbor : star.neighbors) {
//                 std::cout << neighbor->id << " ";
//             }
//             std::cout << "\n";
//         }
//     }

//     // 5. Mine Colocation Patterns (Truyền tham số minPrev và minCondProb từ config)
//     JoinlessMiner miner;
//     auto rules = miner.mineColocations(config.minPrev, config.minCondProb, &neighbor_mgr, instances);
//     for (const auto& rule : rules) {
//         std::cout << "Colocation Rule:\n";
//         for (const auto& antecedent : rule) {
//             std::cout << "  Antecedent: ";
//             for (const auto& feature : antecedent.first) {
//                 std::cout << feature << " ";
//             }
//             std::cout << " -> Consequent: ";
//             for (const auto& feature : antecedent.second) {
//                 std::cout << feature << " ";
//             }
//             std::cout << "\n";
//         }
//     }
//     return 0;
// }
#include "config.h"
#include "data_loader.h"
#include "spatial_index.h"
#include "neighborhood_mgr.h"
#include "miner.h"
#include "utils.h"
#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Load Configuration
    std::string config_path = (argc > 1) ? argv[1] : "../config.txt";
    AppConfig config = ConfigLoader::load(config_path);

    std::cout << "=== Running Joinless Miner ===\n";
    std::cout << "Neighbor Distance (d): " << config.neighborDistance << "\n";
    std::cout << "Min Prevalence: " << config.minPrev << "\n";

    // 2. Load Data
    auto instances = DataLoader::load_csv(config.datasetPath);
    std::cout << "Loaded " << instances.size() << " instances.\n";

    // 3. Build Spatial Index & 4. Neighborhoods
    NeighborhoodMgr neighbor_mgr;
    {
        // Scope này để hủy spatial_idx sau khi dùng xong cho đỡ tốn RAM
        SpatialIndex spatial_idx(config.neighborDistance);
        auto neighborPairs = spatial_idx.findNeighborPair(instances);
        std::cout << "Found " << neighborPairs.size() << " neighbor pairs.\n";
        neighbor_mgr.buildFromPairs(neighborPairs);
    }

    // 5. Mine Colocation Patterns
    // LƯU Ý: Đã bỏ tham số config.minCondProb vì hàm mineColocations mới không cần nữa
    JoinlessMiner miner;
    std::cout << "\n--- START MINING PROCESS ---\n";
    
    auto patterns = miner.mineColocations(config.minPrev, &neighbor_mgr, instances);

    std::cout << "\n=== FINAL RESULTS ===\n";
    std::cout << "Total Prevalent Patterns Discovered: " << patterns.size() << "\n";
    
    for (const auto& pattern : patterns) {
        std::cout << "Pattern: { ";
        for (const auto& feature : pattern) {
            std::cout << feature << " ";
        }
        std::cout << "}\n";
    }

    return 0;
}