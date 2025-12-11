#include "config.h"
#include "data_loader.h"
#include "spatial_index.h"
#include "neighborhood_mgr.h"
#include "miner.h"
#include "utils.h"
#include <iostream>
#include <iomanip>

int main(int argc, char* argv[]) {
    // 1. Load Configuration
    std::cout << "[DEBUG] Step 1: Loading configuration...\n";
    std::string config_path = (argc > 1) ? argv[1] : "src/c++/config.txt";
    AppConfig config = ConfigLoader::load(config_path);
    std::cout << "[DEBUG] Step 1: Configuration loaded successfully.\n";
    std::cout << "Running Joinless with d=" << config.neighborDistance << "...\n";

    // 2. Load Data
    std::cout << "[DEBUG] Step 2: Loading data from " << config.datasetPath << "...\n";
    auto instances = DataLoader::load_csv(config.datasetPath);
    std::cout << "[DEBUG] Step 2: Loaded " << instances.size() << " instances.\n";

    // 3. Build Spatial Index (Truyền tham số d từ config)
    std::cout << "[DEBUG] Step 3: Building spatial index with d=" << config.neighborDistance << "...\n";
    SpatialIndex spatial_idx(config.neighborDistance);
    auto neighborPairs = spatial_idx.findNeighborPair(instances);
    std::cout << "[DEBUG] Step 3: Found " << neighborPairs.size() << " neighbor pairs.\n";
    for (const auto& pair : neighborPairs) {
        std::cout << "Neighbor Pair: " << pair.first.id << " - " << pair.second.id << "\n";
    }

    // 4. Materialize Neighborhoods
    std::cout << "[DEBUG] Step 4: Materializing neighborhoods...\n";
    NeighborhoodMgr neighbor_mgr;
    neighbor_mgr.buildFromPairs(neighborPairs);
    std::cout << "[DEBUG] Step 4: Neighborhoods materialized.\n";
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
    std::cout << "[DEBUG] Step 5: Mining colocation patterns with minPrev=" << config.minPrev << "...\n";
    JoinlessMiner miner;
    
    // Define progress callback
    auto progressCallback = [](int currentStep, int totalSteps, const std::string& message, double percentage) {
        std::cout << "\r[PROGRESS] " << std::fixed << std::setprecision(1) << percentage 
                  << "% (" << currentStep << "/" << totalSteps << ") - " << message;
        std::cout.flush();
        
        // Print newline when completed
        if (percentage >= 100.0) {
            std::cout << std::endl;
        }
    };
    
    auto colocations = miner.mineColocations(config.minPrev, &neighbor_mgr, instances, progressCallback);
    std::cout << "[DEBUG] Step 5: Mining completed.\n";
    
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
    std::cout << "[DEBUG] All steps completed successfully.\n";
    return 0;
}