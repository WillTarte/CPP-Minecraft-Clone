//
// Created by Willi on 8/5/2020.
//
#include "../include/model_database.h"

ModelDatabase ModelDatabase::instance;

ModelDatabase::ModelDatabase() {
    this->models = std::unordered_map<std::string, std::shared_ptr<Model>>();
}

void ModelDatabase::init() {

    LOG(INFO) << "Initializing Model Database ...";

    for (const auto &file : fs::directory_iterator(fs::current_path().parent_path().string() + "/resources/models/")) {
        LOG(INFO) << "Processing " + file.path().filename().string() + " for model data.";

        // Insert modelName => Model
        Mesh mesh = makeMeshFromFile("./resources/models/" + file.path().filename().string());

        if (file.path().filename().string() == "steve.obj") {
            mesh.meshName = "steve";
        }
        std::shared_ptr<Model> model = std::make_shared<Model>(mesh);
        instance.models.insert({model->getModelName(), model});

        LOG(INFO) << "Finished processing " + file.path().filename().string() + ".";
    }
}

std::shared_ptr<Model> ModelDatabase::getModelByName(const std::string &modelName) {
    return instance.models[modelName];
}

