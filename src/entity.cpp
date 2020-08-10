//
// Created by Willi on 8/5/2020.
//
#include "../include/entity.h"

Entity::Entity(std::string modelName, BlockID blockId) {
    this->modelName = std::move(modelName);
    this->blockId = blockId;
    this->transform = Transform();

    std::cout << this->modelName;
    //round about way to do set the hitbox
    glm::vec3 Pos = this->transform.getPosition();
    if (this->modelName == "cubeModel"){
        this->minX = Pos.x;
        this->minY = Pos.y;
        this->minZ = Pos.z;

        this->maxX= Pos.x +1.0;
        this->maxY= Pos.y +1.0;
        this->maxZ= Pos.z +1.0;
    }
    else if (this->modelName == "steveModel"){
        std::cout << Pos.x << Pos.y << Pos.z;
        this->minX = Pos.x;
        this->minY = Pos.y;
        this->minZ = Pos.z;

        this->maxX= Pos.x +1.0f;
        this->maxY= Pos.y +2.0;
        this->maxZ= Pos.z + 1.0;
    }
}

void Entity::draw(Shader &shader) {

    shader.setMat4("model", this->transform.getModelMatrix());

    // Get texture and bind
    std::shared_ptr<TextureInterface> texture = TextureDatabase::getTextureByBlockId(blockId);
    texture->bindTexture();
    if (texture->getTextureType() == CUBEMAP) {
        shader.setBool("isCubeMap", true);
        shader.setInt("textureCubeMap", 1);
    } else {
        shader.setBool("isCubeMap", false);
        shader.setInt("texture2D", 0);
    }

    // Get Model through ModelDatabase and draw
    ModelDatabase::getModelByName(this->modelName)->draw();
}

Entity::Entity(std::string modelName, BlockID blockId1, Transform transform1) {
    this->modelName = std::move(modelName);
    this->blockId = blockId1;
    this->transform = transform1;


    //round about way to do set the hitbox
    glm::vec3 Pos = this->transform.getPosition();
    if (this->modelName == "cubeModel"){
        this->minX = Pos.x;
        this->minY = Pos.y;
        this->minZ = Pos.z;

        this->maxX= Pos.x +1.0;
        this->maxY= Pos.y +1.0;
        this->maxZ= Pos.z +1.0;
    }
    else if (this->modelName == "steveModel"){
        std::cout << Pos.x << Pos.y << Pos.z;
        this->minX = Pos.x;
        this->minY = Pos.y;
        this->minZ = Pos.z;

        this->maxX= Pos.x +1.0;
        this->maxY= Pos.y +2.0;
        this->maxZ= Pos.z + 1.0;
    }
}

//update hitbox is only called for the player so the increases are to that
void Entity::updateHitbox() {
    glm::vec3 Pos = this->transform.getPosition();
    this->minX = Pos.x;
    this->minY = Pos.y;
    this->minZ = Pos.z;

    this->maxX= Pos.x +1.0;
    this->maxY= Pos.y +2.0;
    this->maxZ= Pos.z + 1.0;

}
