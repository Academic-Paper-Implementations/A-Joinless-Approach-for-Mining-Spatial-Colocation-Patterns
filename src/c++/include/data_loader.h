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
};