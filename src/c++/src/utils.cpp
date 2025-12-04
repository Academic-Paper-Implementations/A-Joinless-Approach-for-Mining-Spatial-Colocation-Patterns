#include "utils.h"
#include <set>

// Helper function: Take all unique object types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances) {
    std::set<FeatureType> objectTypesSet;
    
    for (const auto& instance : instances) {
        FeatureType objectType = instance.id.substr(0, 1);
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