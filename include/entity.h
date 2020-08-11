//
// Created by Willi on 8/5/2020.
//
#pragma once

#include <string>
#include "shader.h"
#include "texture_database.h"
#include "model_database.h"
#include "transform.h"

struct BoundingBox {
    BoundingBox(glm::vec3 pos, glm::vec3 dim) {
        this->position = pos;
        this->dimensions = dim;
    }

    BoundingBox() = default;

    void updateBox(const glm::vec3 pos) { this->position = pos; }

    glm::vec3 position;
    glm::vec3 dimensions;
};

/// An entity is an object that exists within the world
class Entity {

private:
    std::string modelName;
    BlockID blockId;
    Transform transform;
public:
    BoundingBox box{};
    glm::vec3 velocity{};
    glm::vec3 acceleration{};

    Entity(std::string modelName, BlockID blockId1);

    Entity(std::string modelName, BlockID blockId1, Transform transform1);

    /** Draws the entity into the world
     * @param shader the shader used to customize the render pipeline
     */
    void draw(Shader &shader);

    /// Gets the blockID of this entity
    inline BlockID getBlockID() const { return this->blockId; }

    /// Gets this entity's transform
    inline Transform &getTransform() { return this->transform; }
};
