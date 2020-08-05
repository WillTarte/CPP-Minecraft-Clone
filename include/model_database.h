//
// Created by Willi on 8/5/2020.
//
#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include "../libs/easylogging++.h"
#include "model.h"

namespace fs = std::experimental::filesystem::v1;

class ModelDatabase {
private:
    ModelDatabase();

    ModelDatabase(const ModelDatabase &) = delete;

    ModelDatabase &operator=(const ModelDatabase &) = delete;

    static ModelDatabase instance;
    std::unordered_map<std::string, Model *> models;

public:

    static void init();

    static Model *getModelByName(std::string &modelName);
};

namespace ModelType {
    const std::string CUBE = "cube";
}