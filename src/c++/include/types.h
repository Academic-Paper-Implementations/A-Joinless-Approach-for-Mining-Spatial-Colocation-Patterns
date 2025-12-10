/**
 * @file types.h
 * @brief Core data structures and type definitions for spatial colocation mining
 * 
 * This file defines the fundamental data types used throughout the joinless
 * colocation pattern mining algorithm, including spatial instances, neighborhoods,
 * and colocation patterns.
 */

#pragma once
#include <string>
#include <unordered_map>
#include <vector>

// Type alias for feature types (e.g., "Restaurant", "Hotel")
using FeatureType = std::string;

// Type alias for instance identifiers (e.g., "A1", "B2")
using instanceID = std::string;

// Type alias for a colocation pattern (ordered list of feature types)
using Colocation = std::vector<FeatureType>;

// Type alias for a colocation instance (pointers to spatial instances forming a pattern)
using ColocationInstance = std::vector<const struct SpatialInstance*>;

// Type alias for colocation rules (mapping from antecedent to consequent patterns)
using ColocationRule = std::unordered_map<Colocation, Colocation>;


/**
 * @brief Represents a single spatial data instance with location and type information
 * 
 * Each spatial instance has a feature type (e.g., "Restaurant"), a unique identifier,
 * and x,y coordinates representing its location in 2D space.
 */
struct SpatialInstance {
    FeatureType type;  ///< Feature type of this instance
    instanceID id;     ///< Unique identifier for this instance
    double x, y;       ///< Spatial coordinates (x, y)
};

/**
 * @brief Represents a star neighborhood centered on a spatial instance
 * 
 * A star neighborhood consists of a center instance and all instances
 * within the distance threshold from the center.
 */
struct StarNeighborhood {
    const SpatialInstance* center;                      ///< Center instance of the star
    std::vector<const SpatialInstance*> neighbors;      ///< All neighbors within distance threshold
};