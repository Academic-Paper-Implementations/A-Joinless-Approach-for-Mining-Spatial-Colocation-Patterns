/**
 * @file utils.cpp
 * @brief Implementation of utility functions for spatial instance processing
 */

#include "utils.h"
#include <set>

// Extract all unique feature types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances) {
    std::set<FeatureType> objectTypesSet;
    
    // Use set to automatically handle uniqueness
    for (const auto& instance : instances) {
        FeatureType objectType = instance.type;
        objectTypesSet.insert(objectType);
    }
    
    // Convert set to vector (will be sorted alphabetically)
    return std::vector<FeatureType>(objectTypesSet.begin(), objectTypesSet.end());
}

// Count the number of instances for each feature type
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances) {
    std::map<FeatureType, int> featureCount;
    
    for (const auto& instance : instances) {
        // Extract feature type from instance ID (first character)
        // Note: This assumes instance IDs are formatted as "A1", "B2", etc.
        FeatureType objectType = instance.id.substr(0, 1);
        featureCount[objectType]++;
    }
    
    return featureCount;
}


// Find a spatial instance by its ID
SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id) 
{
    // Linear search through all instances
    for (const auto& instance : instances) {
        if (instance.id == id) {
            return instance;
        }
    }
    
    // Return empty instance if not found
    return SpatialInstance{};
}