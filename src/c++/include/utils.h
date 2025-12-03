#pragma once
#include "types.h"
#include <vector>
#include <set>
#include <string>
#include <map>

// Helper function: Take all unique object types from instances
inline std::vector<std::string> getAllObjectTypes(const std::vector<SpatialInstance>& instances) {
    std::set<std::string> objectTypesSet;
    
    for (const auto& instance : instances) {
        std::string objectType = instance.id.substr(0, 1);
        objectTypesSet.insert(objectType);
    }
    
    return std::vector<std::string>(objectTypesSet.begin(), objectTypesSet.end());
}

// Helper function: count the number of instances for each feature
inline std::map<std::string, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances) {
    std::map<std::string, int> featureCount;
    
    for (const auto& instance : instances) {
        std::string objectType = instance.id.substr(0, 1);
        featureCount[objectType]++;
    }
    
    return featureCount;
}

// Helper function: filter features that satisfy min_prevalence (for k=1)
inline std::vector<std::string> filterPrevalentFeatures(
    const std::vector<SpatialInstance>& instances,
    double minPrevalence) 
{
    std::vector<std::string> prevalentFeatures;
    int totalInstances = instances.size();
    
    auto featureCount = countInstancesByFeature(instances);
    
    for (const auto& [feature, count] : featureCount) {
        double prevalenceIndex = static_cast<double>(count) / totalInstances;
        
        if (prevalenceIndex >= minPrevalence) {
            prevalentFeatures.push_back(feature);
        }
    }
    
    return prevalentFeatures;
}