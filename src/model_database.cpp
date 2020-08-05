//
// Created by Willi on 8/5/2020.
//

#include "../include/model_database.h"
#include "../include/objloader.h"

ModelDatabase ModelDatabase::instance;

ModelDatabase::ModelDatabase() {
    this->models = std::unordered_map<std::string, Model *>();
}

void ModelDatabase::init() {

    LOG(INFO) << "Initializing Model Database ...";

    for (const auto &file : fs::directory_iterator(fs::current_path().parent_path().string() + "/resources/models/")) {
        LOG(INFO) << "Processing " + file.path().filename().string() + " for model data.";

        // Insert modelName => Model
        Mesh mesh = makeMeshFromFile("./resources/models/" + file.path().filename().string());

        Model *model = new Model(mesh);
        instance.models.insert({model->getModelName(), model});

        LOG(INFO) << "Finished processing " + file.path().filename().string() + ".";
    }
}

Model *ModelDatabase::getModelByName(std::string &modelName) {
    return instance.models[modelName];
}

