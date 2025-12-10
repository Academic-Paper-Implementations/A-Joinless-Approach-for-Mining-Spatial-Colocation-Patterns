/**
 * @file config.h
 * @brief Configuration management for the joinless colocation mining application
 * 
 * Provides structures and utilities for loading and managing application
 * configuration from external files.
 */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 * @brief Configuration structure containing all application settings
 * 
 * Holds I/O paths, algorithm parameters, and system settings for the
 * joinless colocation pattern mining algorithm.
 */
struct AppConfig {
    // I/O Settings
    std::string datasetPath;    ///< Path to input CSV dataset file
    std::string outputPath;     ///< Path to output file for colocation rules

    // Algorithm Parameters
    double neighborDistance;    ///< Distance threshold for spatial neighbors
    double minPrev;            ///< Minimum prevalence threshold for patterns
    double minCondProb;        ///< Minimum conditional probability for rules

    // System Settings
    bool debugMode;            ///< Enable debug output messages

    /**
     * @brief Constructor with default configuration values
     * 
     * Initializes configuration with sensible defaults that can be
     * overridden by loading from a config file.
     */
    AppConfig()
        : datasetPath("data/sample_data.csv"),
          outputPath("src/c++/output/rules.txt"),
          neighborDistance(5.0),
          minPrev(0.6),
          minCondProb(0.5),
          debugMode(false) {}
};


/**
 * @brief Utility class for loading application configuration from files
 * 
 * Provides static methods to parse configuration files in key=value format
 * and populate AppConfig structures.
 */
class ConfigLoader {
public:
    /**
     * @brief Load configuration from a file
     * 
     * @param configPath Path to the configuration file
     * @return AppConfig Populated configuration structure (uses defaults if file not found)
     */
    static AppConfig load(const std::string& configPath);
};