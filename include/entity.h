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

public:
    BoundingBox box{};

    Entity(std::string modelName, BlockID blockId1);

    Entity(std::string modelName, BlockID blockId1, Transform transform1);

    ///move assignment
    Entity &operator=(Entity &&other) {
        std::swap(*this, other);
        return *this;
    }

    ///move constructor
    Entity(Entity &&other) {
        this->modelName = other.modelName;
        this->entityID = other.entityID;
        this->blockId = other.blockId;
        this->transform = other.transform;
        this->box = other.box;
    }

    /** Draws the entity into the world
     * @param shader the shader used to customize the render pipeline
     */
    void draw(Shader &shader);

    /// Gets the blockID of this entity
    inline BlockID getBlockID() const { return this->blockId; }

    /// Gets this entity's transform by reference
    inline Transform &getTransform() { return this->transform; }

    /// Gets this entities unique ID
    inline EntityID getEntityID() { return entityID; }
};
