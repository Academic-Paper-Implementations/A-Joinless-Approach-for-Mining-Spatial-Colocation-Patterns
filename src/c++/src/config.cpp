/**
 * @file config.cpp
 * @brief Implementation of configuration file loading
 */

#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>


// Load configuration from a file
AppConfig ConfigLoader::load(const std::string& configPath) {
    AppConfig config;  // Start with default values
    std::ifstream file(configPath);
    
    // If config file not found, use default values
    if (!file.is_open()) {
        std::cerr << "Warning: Config file not found, using defaults.\n";
        return config;
    }

    std::string line;
    
    // Parse each line in key=value format
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream is_line(line);
        std::string key;
        
        // Split line by '=' delimiter
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                // Map configuration keys to struct members
                if (key == "dataset_path") config.datasetPath = value;
                else if (key == "neighbor_distance") config.neighborDistance = std::stod(value);
                else if (key == "min_prevalence") config.minPrev = std::stod(value);
                else if (key == "min_cond_prob") config.minCondProb = std::stod(value);
                else if (key == "debug_mode") config.debugMode = (value == "true" || value == "1");
            }
        }
    }
    
    return config;
};