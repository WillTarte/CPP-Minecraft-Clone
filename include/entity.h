//
// Created by Willi on 8/5/2020.
//
#pragma once

#include <string>
#include "shader.h"
#include "texture_database.h"
#include "model_database.h"
#include "transform.h"

typedef unsigned long long int EntityID;

struct BoundingBox {
    BoundingBox(glm::vec3 dim) {
        this->dimensions = dim;
    }

    BoundingBox() = default;

    glm::vec3 dimensions = {1.0f, 1.0f, 1.0f};
};

/// An entity is an object that exists within the world
class Entity {
private:
    std::string modelName;
    BlockID blockId;
    EntityID entityID;

protected:
    Transform transform;
    static EntityID entityIDCounter;

    std::shared_ptr<TextureInterface> tex;
    std::shared_ptr<Model> model;
public:
    BoundingBox box{};

    virtual ~Entity() = default;

    Entity(std::string modelName, BlockID blockId1);

    Entity(std::string modelName, BlockID blockId1, Transform transform1);

    ///move assignment
    Entity &operator=(Entity &&other) {
        this->entityID = std::move(other.entityID);
        this->modelName = std::move(other.modelName);
        this->blockId = std::move(other.blockId);
        this->transform = std::move(other.transform);
        this->box = std::move(other.box);
        this->tex = TextureDatabase::getTextureByBlockId(this->blockId);
        this->model = ModelDatabase::getModelByName(this->modelName);
        return *this;
    }

    ///move constructor
    Entity(Entity &&other) {
        this->entityID = std::move(other.entityID);
        this->modelName = std::move(other.modelName);
        this->blockId = std::move(other.blockId);
        this->transform = std::move(other.transform);
        this->box = std::move(other.box);
        this->tex = TextureDatabase::getTextureByBlockId(this->blockId);
        this->model = ModelDatabase::getModelByName(this->modelName);
        this->box = other.box;
    }

    /** Draws the entity into the world
     * @param shader the shader used to customize the render pipeline
     */
    virtual void draw(Shader &shader);

    /// Gets the blockID of this entity
    inline BlockID getBlockID() const { return this->blockId; }

    /// Gets this entity's transform by reference
    inline Transform &getTransform() { return this->transform; }

    /// Gets this entities unique ID
    inline EntityID getEntityID() { return entityID; }
};

class Skybox {
private:
    std::shared_ptr<TextureInterface> tex;
    std::shared_ptr<Model> model;
    std::string modelName;
    BlockID blockId;
public:
    Skybox(const std::string &str, BlockID id);

    void draw(Shader &shader);
};

class Sun : public Entity {
public:
    Sun(const std::string &str, BlockID id);

    void draw(Shader &shader) override;

    void update(float dt);
};