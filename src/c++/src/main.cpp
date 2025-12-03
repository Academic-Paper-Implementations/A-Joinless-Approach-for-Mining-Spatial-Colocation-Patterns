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
        std::cout << "Neighbor Pair: " << pair.first << " - " << pair.second << "\n";
    }

    // 4. Materialize Neighborhoods
    NeighborhoodMgr neighbor_mgr;
    neighbor_mgr.buildFromPairs(instances, neighborPairs);
    for (const auto& [feature, starNeighs] : neighbor_mgr.getAllStarNeighborhoods()) {
        std::cout << "Feature: " << feature << "\n";
        for (const auto& starNeigh : starNeighs) {
            std::cout << "  Center: " << starNeigh.center->id << " Neighbors: ";
            for (const auto& neighbor : starNeigh.neighbors) {
                std::cout << neighbor->id << " ";
            }
            std::cout << "\n";
        }
    }

    // 5. Test Mining - Generate Candidates
    std::cout << "\n========== Testing Candidate Generation ==========\n";
    
    JoinlessMiner miner(config.minPrev, &neighbor_mgr);
    
    // Get number of features
    auto allFeatures = getAllObjectTypes(instances);
    int numFeatures = allFeatures.size();
    
    std::vector<Colocation> candidates = {};
    
    // Generate candidates for k=1 to k=numFeatures
    for (int k = 1; k <= numFeatures; ++k) {
        std::cout << "\nGenerating candidates for k=" << k << "...\n";
        candidates = miner.generateCandidates(candidates, k, instances);
        std::cout << "Number of k=" << k << " candidates: " << candidates.size() << "\n";
        
        for (const auto& candidate : candidates) {
            std::cout << "  Candidate: ";
            for (const auto& feature : candidate) {
                std::cout << feature << " ";
            }
            std::cout << "\n";
        }
        
        if (candidates.empty()) {
            std::cout << "No more candidates. Stopping.\n";
            break;
        }
    }
    
    std::cout << "\n========== Done ==========\n";
    
    // // 5. Mining (Truyền threshold từ config)
    // JoinlessMiner miner(config.min_prevalence, &neighbor_mgr);
    // auto rules = miner.mine(objects);

    // // 6. Output (Có thể thêm logic ghi file dựa trên config.output_path)
    // // ...

    return 0;
}