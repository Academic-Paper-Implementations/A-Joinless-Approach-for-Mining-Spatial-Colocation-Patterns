/**
 * @file utils.h
 * @brief Utility helper functions for spatial colocation mining
 */

#pragma once
#include "types.h"
#include <vector>
#include <set>
#include <string>
#include <map>

/**
 * @brief Get all unique feature types from spatial instances
 * 
 * Extracts and returns a sorted vector of all unique feature types
 * present in the given instances.
 * 
 * @param instances Vector of spatial instances
 * @return std::vector<FeatureType> Sorted vector of unique feature types
 */
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances);

/**
 * @brief Count the number of instances for each feature type
 * 
 * Creates a frequency map showing how many instances exist for each feature type.
 * 
 * @param instances Vector of spatial instances
 * @return std::map<FeatureType, int> Map from feature type to instance count
 */
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances);

/**
 * @brief Find a spatial instance by its ID
 * 
 * Searches for an instance with the specified ID. Returns an empty
 * SpatialInstance struct if no matching instance is found.
 * 
 * @param instances Vector of spatial instances to search
 * @param id Instance ID to find
 * @return SpatialInstance The found instance, or empty struct if not found
 */
SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id);