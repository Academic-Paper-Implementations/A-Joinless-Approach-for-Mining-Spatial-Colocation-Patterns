/**
 * @file utils.cpp
 * @brief Implementation of utility helper functions
 */

#include "utils.h"
#include <set>

// Get all unique feature types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances) {
    // Use a set to automatically handle uniqueness
    std::set<FeatureType> objectTypesSet;
    
    for (const auto& instance : instances) {
        FeatureType objectType = instance.type;
        objectTypesSet.insert(objectType);
    }
    
    // Convert set to vector (set maintains sorted order)
    return std::vector<FeatureType>(objectTypesSet.begin(), objectTypesSet.end());
}

// Count the number of instances for each feature type
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances) {
    std::map<FeatureType, int> featureCount;
    
    for (const auto& instance : instances) {
        // Extract feature type from instance ID
        // Assumes ID format is: FeatureType + Number (e.g., "A1", "B2")
        // Takes first character as feature type
        FeatureType objectType = instance.id.substr(0, 1);
        featureCount[objectType]++;
    }
    
    return featureCount;
}


SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id) 
{
    // Linear search for instance with matching ID
    for (const auto& instance : instances) {
        if (instance.id == id) {
            return instance;
        }
    }
    
    // Return empty instance if not found
    return SpatialInstance{};
}