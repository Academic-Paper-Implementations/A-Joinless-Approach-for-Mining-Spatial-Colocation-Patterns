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

    std::cout << "Running Joinless with d=" << config.neighborDistance << "...\n";

    // 2. Load Data
    auto instances = DataLoader::load_csv(config.datasetPath);

    // 3. Build Spatial Index (Truyền tham số d từ config)
    SpatialIndex spatial_idx(config.neighborDistance);
    auto neighborPairs = spatial_idx.findNeighborPair(instances);
    for (const auto& pair : neighborPairs) {
        std::cout << "Neighbor Pair: " << pair.first.id << " - " << pair.second.id << "\n";
    }

    // 4. Materialize Neighborhoods
    NeighborhoodMgr neighbor_mgr;
    neighbor_mgr.buildFromPairs(neighborPairs);
    for (const auto& starNeighborhood : neighbor_mgr.getAllStarNeighborhoods()) {
        std::cout << "Star Neighborhoods for Feature: " << starNeighborhood.first << "\n";
        for (const auto& star : starNeighborhood.second) {
            std::cout << "  Center: " << star.center->id << " Neighbors: ";
            for (const auto& neighbor : star.neighbors) {
                std::cout << neighbor->id << " ";
            }
            std::cout << "\n";
        }
    }

    // 5. Mine Colocation Patterns
    // LƯU Ý: Đã bỏ tham số config.minCondProb vì hàm mineColocations mới không cần nữa
    // 5. Mine Colocation Patterns
    JoinlessMiner miner;
    auto colocations = miner.mineColocations(config.minPrev, &neighbor_mgr, instances);
    
    // In kết quả
    std::cout << "\nFound " << colocations.size() << " prevalent colocations:\n";
    for (const auto& colocation : colocations) {
        std::cout << "Colocation: ";
        for (size_t i = 0; i < colocation.size(); ++i) {
            if (i > 0) std::cout << " - ";
            std::cout << colocation[i];
        }
        std::cout << "\n";
    }
}