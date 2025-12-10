/**
 * @file utils.h
 * @brief Utility functions for spatial instance processing
 * 
 * Provides helper functions for extracting feature types, counting instances,
 * and looking up instances by ID.
 */

#pragma once
#include "types.h"
#include <vector>
#include <set>
#include <string>
#include <map>

/**
 * @brief Extract all unique feature types from a set of instances
 * 
 * @param instances Vector of spatial instances to process
 * @return std::vector<FeatureType> Sorted vector of unique feature types
 */
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances);

/**
 * @brief Count the number of instances for each feature type
 * 
 * @param instances Vector of spatial instances to count
 * @return std::map<FeatureType, int> Map from feature type to instance count
 */
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances);

/**
 * @brief Find a spatial instance by its ID
 * 
 * @param instances Vector of spatial instances to search
 * @param id Instance ID to find
 * @return SpatialInstance The found instance, or empty SpatialInstance{} if not found
 */
SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id);