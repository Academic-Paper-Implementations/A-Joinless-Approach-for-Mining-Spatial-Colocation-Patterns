#include "data_loader.h"
#include <iostream>

using namespace csv;

std::vector<SpatialInstance> DataLoader::load_csv(const std::string& filepath) {
    CSVReader reader(filepath);
    std::vector<SpatialInstance> instances;

    for (auto& row : reader) {
        SpatialInstance instance;
        instance.type = row["Feature"].get<FeatureType>();
        instance.id = instanceID(instance.type + std::to_string(row["Instance"].get<int>()));
        instance.x = row["LocX"].get<double>();
        instance.y = row["LocY"].get<double>();
        instances.push_back(instance);
    }
    return instances;
}