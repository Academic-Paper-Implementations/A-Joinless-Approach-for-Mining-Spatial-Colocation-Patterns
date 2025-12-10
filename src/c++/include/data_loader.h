/**
 * @file data_loader.h
 * @brief CSV data loading utilities for spatial instances
 * 
 * Provides functionality to load spatial instance data from CSV files
 * into memory for processing by the mining algorithm.
 */

#pragma once
#include "types.h"
#include "csv.hpp"
#include <string>
#include <vector>

/**
 * @brief Utility class for loading spatial data from CSV files
 * 
 * Handles parsing of CSV files containing spatial instance data with
 * feature types, instance numbers, and x,y coordinates.
 */
class DataLoader {
public:
    /**
     * @brief Load spatial instances from a CSV file
     * 
     * Expects CSV format with columns: Feature, Instance, LocX, LocY
     * - Feature: Feature type (e.g., "A", "B", "Restaurant")
     * - Instance: Instance number (integer)
     * - LocX: X coordinate (double)
     * - LocY: Y coordinate (double)
     * 
     * @param filepath Path to the CSV file to load
     * @return std::vector<SpatialInstance> Vector of loaded spatial instances
     * @note Instance IDs are generated as: FeatureType + InstanceNumber (e.g., "A1", "B2")
     */
    static std::vector<SpatialInstance> load_csv(const std::string& filepath);
};