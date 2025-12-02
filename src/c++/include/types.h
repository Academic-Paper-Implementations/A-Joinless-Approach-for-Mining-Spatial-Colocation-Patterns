#pragma once
#include <string>
#include <vector>

// Type aliases for feature types and instance IDs
using FeatureType = std::string;
using instanceID = std::string;

// Type aliases for colocation features and their instances
using Colocation = std::vector<FeatureType>;
using ColocationInstance = std::vector<instanceID>;



// Structure representing a spatial data instance
struct SpatialInstance {
    FeatureType type;
    instanceID id;
    double x, y;
};

// Structure representing a star neighborhood
struct StarNeighborhood {
    const SpatialInstance* center;
    std::vector<const SpatialInstance*> neighbors;
};

// Structure representing a colocation rule with its participation index
struct ColocationRule {
    Colocation features;
    double participationIndex;
};