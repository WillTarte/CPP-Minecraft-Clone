//
// Created by Willi on 8/13/2020.
//
#include <random>
#include <climits>
#include "../include/chunks.h"

Chunk::Chunk(glm::vec2 origin) {
    for (auto ent : allBlockIDs) {
        entities.insert({ent, std::vector<Entity>()});
    }
    this->origin = origin;
}

std::optional<Entity *> Chunk::getEntityByWorldPos(glm::vec3 worldPos) {

    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {
            glm::vec3 entityPos = ent.getTransform().getPosition();

            bool withinX = (int) worldPos.x == (int) entityPos.x;
            bool withinY = (int) worldPos.y == (int) entityPos.y;
            bool withinZ = (int) worldPos.z == (int) entityPos.z;

            if (withinX && withinZ && withinY) {
                return {&ent};
            }
        }
    }
    return {};
}

std::optional<Entity *> Chunk::getEntityByBoxCollision(glm::vec3 worldPos, BoundingBox box) {
    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {

            bool xColl = (ent.getTransform().getPosition().x <= worldPos.x + box.dimensions.x &&
                          ent.getTransform().getPosition().x + ent.box.dimensions.x >= worldPos.x);
            bool yColl = (ent.getTransform().getPosition().y <= worldPos.y + box.dimensions.y &&
                          ent.getTransform().getPosition().y + ent.box.dimensions.y >= worldPos.y);
            bool zColl = (ent.getTransform().getPosition().z <= worldPos.z + box.dimensions.z &&
                          ent.getTransform().getPosition().z + ent.box.dimensions.z >= worldPos.z);

            if (xColl && yColl && zColl)
                return {&ent};
        }
    }
    return {};
}

bool Chunk::isBlockOutOfBounds(glm::vec2 xzCoords) {

    return xzCoords.x<this->origin.x || xzCoords.x>(this->origin.x + CHUNK_WIDTH) ||
           xzCoords.y<this->origin.y || xzCoords.y>(this->origin.y + CHUNK_LENGTH);

}

void Chunk::renderChunk(Shader &shader) {

    for (auto &entsByBlock : entities) {
        // TODO: would it be possible to do instancing here?
        for (auto &ent : entsByBlock.second) {
            ent.draw(shader);
        }
    }
}

std::optional<std::shared_ptr<Chunk>> ChunkManager::getChunkbyXZ(glm::vec2 xzCoords) {

    for (auto &chunkByPos : chunks) {
        if ((int) (chunkByPos.first.x / CHUNK_WIDTH) == (int) chunkByPos.second->getChunkOrigin().x &&
            (int) (chunkByPos.first.z / CHUNK_LENGTH) == (int) chunkByPos.second->getChunkOrigin().y) {
            return chunkByPos.second;
        }
    }

    LOG(INFO) << "Did not find Chunk at coords " << xzCoords.x << " " << xzCoords.y;
    return {};
}

std::vector<std::shared_ptr<Chunk>> ChunkManager::getSurroundingChunksByXZ(glm::vec2 xzCoords) {

    std::vector<std::shared_ptr<Chunk>> out;
    std::optional<std::shared_ptr<Chunk>> mainChunk = getChunkbyXZ(xzCoords);
    if (mainChunk.has_value())
        out.push_back((*mainChunk));
    else
        LOG(INFO) << "Unable to find main chunk at " << xzCoords.x << " " << xzCoords.y;

    for (int x = -1; x <= 1; x++) {
        for (int z = -1; z <= 1; z++) {
            std::optional<std::shared_ptr<Chunk>> neighborChunk = getChunkbyXZ(
                    xzCoords + glm::vec2(x * CHUNK_WIDTH, z * CHUNK_LENGTH));
            if (neighborChunk.has_value())
                out.push_back((*neighborChunk));
            else
                LOG(INFO) << "Unable to find neighbor chunk at "
                          << xzCoords.x + glm::vec2(x * CHUNK_WIDTH, z * CHUNK_LENGTH).x << " "
                          << xzCoords.y + glm::vec2(x * CHUNK_WIDTH, z * CHUNK_LENGTH).y;
        }
    }

    return out;
}

ChunkManager::ChunkManager(const WorldInfo &worldInfo) {

    for (unsigned int chunkX = 0; chunkX < worldInfo.getWidth(); chunkX += CHUNK_WIDTH) {
        for (unsigned int chunkZ = 0; chunkZ < worldInfo.getLength(); chunkZ += CHUNK_LENGTH) {
            this->chunks.insert({glm::vec2(chunkX, chunkZ), std::make_unique<Chunk>(glm::vec2(chunkX, chunkZ))});
        }
    }

}

unsigned int WorldInfo::generateSeed() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<unsigned int> dist(0, UINT_MAX);
    return dist(mt);
}

WorldInfo::WorldInfo() {
    this->seed = generateSeed();
}
