#include "utils.h"
#include <set>
#include <chrono>
#include <windows.h>
#include <psapi.h>
#include <iostream> 
#include <iomanip>
#include <windows.h>
#include <psapi.h>

// Helper function: Take all unique object types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances) {
    std::set<FeatureType> objectTypesSet;
    
    for (const auto& instance : instances) {
        FeatureType objectType = instance.type;
        objectTypesSet.insert(objectType);
    }
    
    return std::vector<FeatureType>(objectTypesSet.begin(), objectTypesSet.end());
}

// Helper function: count the number of instances for each feature
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances) {
    std::map<FeatureType, int> featureCount;
    
    for (const auto& instance : instances) {
        FeatureType objectType = instance.id.substr(0, 1);
        featureCount[objectType]++;
    }
    
    return featureCount;
}


SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id) 
{
    for (const auto& instance : instances) {
        if (instance.id == id) {
            return instance;
        }
    }
    return SpatialInstance{};
}


void findCombinations(
    const std::vector<FeatureType>& candidatePattern,
    int typeIndex,
    std::vector<const SpatialInstance*>& currentInstance,
    const std::unordered_map<FeatureType, std::vector<const SpatialInstance*>>& neighborMap,
    std::vector<ColocationInstance>& results) 
{
    // Base case: if we've matched all types in the candidate pattern
    if (typeIndex >= candidatePattern.size()) {
        results.push_back(currentInstance);
        return;
    }
    FeatureType currentType = candidatePattern[typeIndex];

    auto it = neighborMap.find(currentType);
    if (it != neighborMap.end()) {
        for (const auto* neighbor : it->second) {
            currentInstance.push_back(neighbor);
            findCombinations(candidatePattern, typeIndex + 1, currentInstance, neighborMap, results);
            currentInstance.pop_back();
        }
    }
}


void printDuration(const std::string& stepName, std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end) {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[PERF] " << stepName << ": " << duration << " ms" << std::endl;
}


double getMemoryUsageMB() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return static_cast<double>(pmc.WorkingSetSize) / (1024.0 * 1024.0);
    }
    return 0.0;
}