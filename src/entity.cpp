//
// Created by Willi on 8/5/2020.
//
#include "../include/entity.h"

EntityID Entity::entityIDCounter = 1;

Entity::Entity(std::string modelName, BlockID blockId) {
    this->modelName = std::move(modelName);
    this->blockId = blockId;
    this->transform = Transform();
    this->box = BoundingBox({1.0, 1.0, 1.0});
    this->entityID = Entity::entityIDCounter++;
    this->tex = TextureDatabase::getTextureByBlockId(this->blockId);
    this->model = ModelDatabase::getModelByName(this->modelName);
}

Entity::Entity(std::string modelName, BlockID blockId1, Transform transform1) {
    this->modelName = std::move(modelName);
    this->blockId = blockId1;
    this->transform = transform1;
    this->box = BoundingBox({1.0, 1.0, 1.0});
    this->entityID = Entity::entityIDCounter++;
    this->tex = TextureDatabase::getTextureByBlockId(this->blockId);
    this->model = ModelDatabase::getModelByName(this->modelName);
}

void Entity::draw(Shader &shader) {

    shader.setMat4("model", this->transform.getModelMatrix());

    // Get texture and bind
    this->tex->bindTexture();
    if (this->tex->getTextureType() == CUBEMAP) {
        shader.setBool("isCubeMap", true);
        shader.setInt("textureCubeMap", 1);
    } else {
        shader.setBool("isCubeMap", false);
        shader.setInt("texture2D", 0);
    }

    // Get Model through ModelDatabase and draw
    this->model->draw();
}



