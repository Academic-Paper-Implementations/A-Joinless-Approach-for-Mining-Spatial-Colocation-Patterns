#pragma once
#include "types.h"
#include <vector>
#include <set>
#include <string>
#include <chrono>
#include <map>

// Helper function: Take all unique object types from instances
std::vector<FeatureType> getAllObjectTypes(const std::vector<SpatialInstance>& instances);

// Helper function: count the number of instances for each feature
std::map<FeatureType, int> countInstancesByFeature(const std::vector<SpatialInstance>& instances);

SpatialInstance getInstanceByID(
    const std::vector<SpatialInstance>& instances, 
    const instanceID& id);

/**
* @brief Recursive helper to find all combinations of spatial instances
*        matching a candidate pattern within a star neighborhood.
* 
* @param candidatePattern The candidate colocation pattern being matched
* @param typeIndex Current index in the candidate pattern being processed
* @param currentInstance Current partial instance being built
* @param neighborMap Map of feature types to their neighboring instances
* @param results Vector to store the resulting colocation instances
*/
void findCombinations(
    const std::vector<FeatureType>& candidatePattern,
    int typeIndex,
    std::vector<const SpatialInstance*>& currentInstance,
    const std::unordered_map<FeatureType, std::vector<const SpatialInstance*>>& neighborMap,
    std::vector<ColocationInstance>& results);


/**
* @brief Print the duration of a processing step
* 
* @param stepName Name of the processing step
* @param start Start time point
* @param end End time point
*/
void printDuration(const std::string& stepName, std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point end);


/**
 * @brief Get current memory usage in megabytes
 *
 * Uses Windows API to retrieve the current process's memory usage.
 *
 * @return double Memory usage in MB
 */
double getMemoryUsageMB();