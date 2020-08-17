//
// Created by Willi on 8/13/2020.
//
#include <random>
#include <climits>
#include "../include/chunks.h"

Chunk::Chunk(unsigned int xInd, unsigned int zInd) {
    for (auto ent : allBlockIDs) {
        entities.insert({ent, std::vector<Entity>()});
    }
    this->origin = std::make_pair(xInd, zInd);
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

            //if (isBlockOutOfBounds({worldPos.x, worldPos.z}))
            // return {};

            bool xColl = (ent.getTransform().getPosition().x < worldPos.x + box.dimensions.x &&
                          ent.getTransform().getPosition().x + ent.box.dimensions.x > worldPos.x);
            bool yColl = (ent.getTransform().getPosition().y < worldPos.y + box.dimensions.y &&
                          ent.getTransform().getPosition().y + ent.box.dimensions.y > worldPos.y);
            bool zColl = (ent.getTransform().getPosition().z < worldPos.z + box.dimensions.z &&
                          ent.getTransform().getPosition().z + ent.box.dimensions.z > worldPos.z);

            if (xColl && yColl && zColl)
                return {&ent};
        }
    }
    return {};
}

bool Chunk::isBlockOutOfBounds(glm::vec2 xzCoords) const {

    return xzCoords.x<(float) (origin.first * CHUNK_WIDTH) || xzCoords.x>(float)(origin.first * (CHUNK_WIDTH + 1)) ||
           xzCoords.y<(float) (origin.second * CHUNK_LENGTH) || xzCoords.y>(float)(origin.second * (CHUNK_LENGTH + 1));
}

void Chunk::renderChunk(Shader &shader) {

    for (auto &entsByBlock : entities) {
        // TODO: would it be possible to do instancing here?
        for (auto &ent : entsByBlock.second) {
            ent.draw(shader);
        }
    }
}

size_t Chunk::getNumberOfEntities() const {
    size_t total = 0;
    for (const auto &ents : entities)
        total += ents.second.size();
    return total;
}

bool Chunk::removeEntity(Entity &entity) {
    auto &entVec = this->entities[entity.getBlockID()];
    std::vector<Entity>::iterator it;
    for (it = entVec.begin(); it != entVec.end(); it++) {
        if (it->getEntityID() == entity.getEntityID()) {
            entVec.erase(it);
            return true;
        }
    }
    return false;
}

std::optional<std::shared_ptr<Chunk>> ChunkManager::getChunkByXZ(const glm::vec2 xzCoords) {

    std::pair<int, int> findKey;
    findKey.first = (int) xzCoords.x / CHUNK_WIDTH;
    findKey.second = (int) xzCoords.y / CHUNK_LENGTH;

    if (chunks.find(findKey) == chunks.end()) {
        LOG(DEBUG) << "Did not find Chunk at coords " << xzCoords.x << " " << xzCoords.y;
        return {};
    } else {
        return chunks[findKey];
    }
}

std::optional<std::shared_ptr<Chunk>> ChunkManager::getChunkByXZIndex(unsigned int xInd, unsigned int zInd) {
    std::pair<unsigned int, unsigned int> findKey = std::make_pair(xInd, zInd);

    if (chunks.find(findKey) == chunks.end()) {
        LOG(DEBUG) << "Did not find Chunk at coords " << xInd * CHUNK_WIDTH << " " << zInd * CHUNK_LENGTH;
        return {};
    } else {
        return chunks[findKey];
    }
}

std::vector<std::shared_ptr<Chunk>> ChunkManager::getSurroundingChunksByXZ(glm::vec2 xzCoords) {

    std::vector<std::shared_ptr<Chunk>> out;

    for (int x = -1; x <= 1; x++) {
        for (int z = -1; z <= 1; z++) {
            std::optional<std::shared_ptr<Chunk>> neighborChunk = getChunkByXZIndex(
                    ((int) xzCoords.x / CHUNK_WIDTH) + x,
                    ((int) xzCoords.y / CHUNK_LENGTH) + z);
            if (neighborChunk.has_value())
                out.push_back((*neighborChunk));
        }
    }

    return out;
}

ChunkManager::ChunkManager(const WorldInfo &worldInfo) {

    size_t num = 0;
    for (unsigned int chunkX = 0; chunkX < worldInfo.getWidth() / CHUNK_WIDTH; chunkX++) {
        for (unsigned int chunkZ = 0; chunkZ < worldInfo.getLength() / CHUNK_LENGTH; chunkZ++) {
            this->chunks.insert({std::make_pair(chunkX, chunkZ), std::make_shared<Chunk>(chunkX, chunkZ)});
        }
    }

}

size_t ChunkManager::getNumberOfEntities() const {
    size_t total = 0;
    for (const auto &chunk : chunks) {
        total += chunk.second->getNumberOfEntities();
    }
    return total;
}

int WorldInfo::generateSeed() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, INT_MAX);
    return dist(mt);
}

WorldInfo::WorldInfo() {
    this->seed = generateSeed();
}

bool ChunkManager::removeEntityFromChunk(Entity &entity) {

    std::optional<std::shared_ptr<Chunk>> optChunk = getChunkByXZ(
            {entity.getTransform().getPosition().x, entity.getTransform().getPosition().z});

    if (optChunk.has_value()) {
        return (*optChunk)->removeEntity(entity);
    } else {
        LOG(DEBUG) << "Could not find chunk for entity at " << entity.getTransform().getPosition().x << " "
                   << entity.getTransform().getPosition().y << " " << entity.getTransform().getPosition().z;
        return false;
    }

}
