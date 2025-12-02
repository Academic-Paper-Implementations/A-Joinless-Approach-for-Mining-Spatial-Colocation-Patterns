#include "data_loader.h"
#include <iostream>

using namespace csv;

std::vector<SpatialInstance> DataLoader::load_csv(const std::string& filepath) {
    CSVReader reader(filepath);
    std::vector<SpatialInstance> instances;

    for (auto& row : reader) {
        SpatialInstance instance;
        instance.type = row["Feature"].get<FeatureType>();
        instance.id = instanceID(instance.type + std::to_string(row["InstanceID"].get<int>()));
        instance.x = row["X"].get<double>();
        instance.y = row["Y"].get<double>();
        instances.push_back(instance);
    }
    return instances;
}