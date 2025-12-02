#pragma once
#include "types.h"
#include "csv.hpp"
#include <string>
#include <vector>

/*
    * DataLoader class for loading spatial instances from CSV files.
    */
class DataLoader {
public:
    // Load spatial instances from a CSV file
    static std::vector<SpatialInstance> load_csv(const std::string& filepath);
    
    // Get feature code from feature name
    static FeatureType get_feature_code(const std::string& feature_name);

    // Get feature name from feature code
    static std::string get_feature_name(FeatureType code);
};