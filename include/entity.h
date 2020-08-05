//
// Created by Willi on 8/5/2020.
//

#ifndef COMP_371_PROJECT_ENTITY_H
#define COMP_371_PROJECT_ENTITY_H

#include <string>
#include "shader.h"
#include "texture_database.h"

class Entity {

private:
    std::string modelName; // should we do by ID ?
    /* Transform transform; */
    /* AABB boundingBox; */
    BlockID blockId;
public:

    Entity(std::string &modelName, BlockID blockId1);

    void draw(Shader &shader);
};

#endif //COMP_371_PROJECT_ENTITY_H
