//
// Created by Willi on 8/5/2020.
//
#include "../include/entity.h"

Entity::Entity(std::string modelName, BlockID blockId) {
    this->modelName = std::move(modelName);
    this->blockId = blockId;
}

void Entity::draw(Shader &shader) {
    shader.use();
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setInt("texture1", 0);
    // set shader uniforms from transform and stuff

    // Get texture and bind
    TextureDatabase::getTextureByBlockID(blockId)->bindTexture();

    // Get Model through ModelDatabase, bind and draw
    const Model *model = ModelDatabase::getModelByName(this->modelName);
    model->draw();
}
