#pragma once
#include "types.h"
#include <vector>
#include <set>
#include <string>
#include <map>

// Helper function: Take all unique object types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances);

// Helper function: count the number of instances for each feature
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances);