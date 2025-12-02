#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
    * Configuration structure for application settings
    */
struct AppConfig {
    // I/O Settings
    std::string datasetPath;
    std::string outputPath;

    // Algorithm Parameters
    double neighborDistance;
    double minPrev;
    double minCondProb;

    // System Settings
    bool debugMode;

    // Constructor with default values
    AppConfig()
        : datasetPath("data/sample_joinless.csv"),
          outputPath("src/c++/output/rules.txt"),
          neighborDistance(10.0),
          minPrev(0.3),
          minCondProb(0.5),
          debugMode(false) {}
};


/*
    * ConfigLoader class for loading application configuration from a file.
    */
class ConfigLoader {
public:
    static AppConfig load(const std::string& configPath);
};