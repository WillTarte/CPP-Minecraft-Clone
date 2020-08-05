//
// Created by Willi on 8/5/2020.
//

#include "../include/entity.h"


Entity::Entity(std::string &modelName, BlockID blockId) {
    this->modelName = std::move(modelName);
    this->blockId = blockId;
}

void Entity::draw(Shader &shader) {

    shader.use();
    shader.setInt("texture1", 0);
    // set shader uniforms from transform and stuff

    // Get texture
    TextureDatabase::getTextureByBlockID(blockId)->bindTexture();

    // Get Model through ModelDatabase
    // Model& model = ModelDatabase::getModelByName(this->modelName);
    // model.bindBuffers();
    // model.draw();
}
