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

    // Get Model through ModelDatabase and draw
    this->model->draw();
}

Skybox::Skybox(const std::string &str, BlockID id) {
    this->blockId = id;
    this->modelName = str;
    this->tex = TextureDatabase::getTextureByBlockId(id);
    this->model = ModelDatabase::getModelByName(str);
}

void Skybox::draw(Shader &shader) {
    glDepthFunc(GL_LEQUAL);
    this->tex->bindTexture();
    shader.setInt("skybox", 1);
    // Get Model and draw
    this->model->draw();
    glDepthFunc(GL_LESS);
}


Sun::Sun(const std::string &str, BlockID id) : Entity(str, id) {}

void Sun::draw(Shader &shader) {

    shader.setMat4("model", this->transform.getModelMatrix());

    this->model->draw();
}

void Sun::update(float dt) {

    glm::quat rotatioQuat = glm::quat(glm::vec3(0.0f, 0.0f, dt / 60.0f));

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), this->getTransform().getPosition());

    glm::mat4 out = glm::toMat4(rotatioQuat) * translation;
    this->getTransform().setPosition(glm::vec3(out[3]));
}
