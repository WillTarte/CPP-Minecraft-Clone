//
// Created by Willi on 8/5/2020.
//

#ifndef COMP_371_PROJECT_ENTITY_H
#define COMP_371_PROJECT_ENTITY_H

#include <string>
#include "shader.h"
#include "texture_database.h"
#include "model_database.h"
#include "transform.h"

/// An entity is an object that exists within the world
class Entity {

private:
    std::string modelName;

    //bounding box is defined by the min point and the max

    BlockID blockId;
    Transform transform;
public:
    float minX, minY, minZ;
    float maxX, maxY, maxZ;

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

    void updateHitbox();

};

#endif //COMP_371_PROJECT_ENTITY_H
