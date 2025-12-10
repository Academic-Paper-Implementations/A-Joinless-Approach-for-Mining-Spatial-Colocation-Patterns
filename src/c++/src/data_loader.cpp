/**
 * @file data_loader.cpp
 * @brief Implementation of CSV data loading for spatial instances
 */

#include "data_loader.h"
#include <iostream>

using namespace csv;

std::vector<SpatialInstance> DataLoader::load_csv(const std::string& filepath) {
    CSVReader reader(filepath);
    std::vector<SpatialInstance> instances;

    // Parse each row from the CSV file
    for (auto& row : reader) {
        SpatialInstance instance;
        
        // Map CSV columns to instance fields
        instance.type = row["Feature"].get<FeatureType>();
        
        // Generate unique ID by concatenating feature type with instance number (e.g., "A1", "B2")
        instance.id = instanceID(instance.type + std::to_string(row["Instance"].get<int>()));
        
        // Extract spatial coordinates
        instance.x = row["LocX"].get<double>();
        instance.y = row["LocY"].get<double>();
        
        instances.push_back(instance);
    }
    
    return instances;
}