#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>


// Load configuration from a file
AppConfig ConfigLoader::load(const std::string& configPath) {
    AppConfig config;
    std::ifstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Warning: Config file not found, using defaults.\n";
        return config;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                // Trim whitespace
                if (key == "datasetPath") config.datasetPath = value;
                else if (key == "neighborDistance") config.neighborDistance = std::stod(value);
                else if (key == "minPrev") config.minPrev = std::stod(value);
                else if (key == "minCondProb") config.minCondProb = std::stod(value);
                else if (key == "debugMode") config.debugMode = (value == "true" || value == "1");
            }
        }
    }
    return config;
};