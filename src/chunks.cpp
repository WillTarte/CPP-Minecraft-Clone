//
// Created by Willi on 8/13/2020.
//
#include <random>
#include <climits>
#include "../include/chunks.h"

Chunk::Chunk(unsigned int xInd, unsigned int zInd) {
    this->origin = std::make_pair(xInd, zInd);
}

std::optional<std::shared_ptr<Entity>> Chunk::getEntityByWorldPos(glm::vec3 worldPos) {

    for (auto &ent : entities) {
        glm::vec3 entityPos = ent.second->getTransform().getPosition();

        bool withinX = (int) worldPos.x == (int) entityPos.x;
        bool withinY = (int) worldPos.y == (int) entityPos.y;
        bool withinZ = (int) worldPos.z == (int) entityPos.z;

        if (withinX && withinZ && withinY) {
            return {ent.second};
        }
    }
    return {};
}

std::optional<std::shared_ptr<Entity>> Chunk::getEntityByBoxCollision(glm::vec3 worldPos, BoundingBox box) {
    for (auto &ent : entities) {

        bool xColl = (ent.second->getTransform().getPosition().x < worldPos.x + box.dimensions.x &&
                      ent.second->getTransform().getPosition().x + ent.second->box.dimensions.x > worldPos.x);
        bool yColl = (ent.second->getTransform().getPosition().y < worldPos.y + box.dimensions.y &&
                      ent.second->getTransform().getPosition().y + ent.second->box.dimensions.y > worldPos.y);
        bool zColl = (ent.second->getTransform().getPosition().z < worldPos.z + box.dimensions.z &&
                      ent.second->getTransform().getPosition().z + ent.second->box.dimensions.z > worldPos.z);

        if (xColl && yColl && zColl)
            return {ent.second};
    }
    return {};
}

bool Chunk::isBlockOutOfBounds(glm::vec2 xzCoords) const {

    return xzCoords.x<(float) (origin.first * EngineConstants::CHUNK_WIDTH) || xzCoords.x>(float)(
            origin.first * (EngineConstants::CHUNK_WIDTH + 1)) ||
           xzCoords.y<(float) (origin.second * EngineConstants::CHUNK_LENGTH) || xzCoords.y>(float)(
                   origin.second * (EngineConstants::CHUNK_LENGTH + 1));
}

void Chunk::renderChunk(Shader &shader, const ViewFrustum &frustum) {

    for (auto &pair : entitiesByBlockID) {
        std::shared_ptr<TextureInterface> tex = TextureDatabase::getTextureByBlockId(pair.first);
        tex->bindTexture();
        if (tex->getTextureType() == CUBEMAP) {
            shader.setBool("isCubeMap", true);
            shader.setInt("textureCubeMap", 1);
        } else {
            shader.setBool("isCubeMap", false);
            shader.setInt("texture2D", 0);
        }
        for (auto &ent : pair.second) {
            if (frustum.isBoxInFrustum(ent->getTransform().getPosition(), ent->box)) {
                ent->draw(shader);
            }
        }
    }
}

size_t Chunk::getNumberOfEntities() const {
    return entities.size();
}

bool Chunk::removeEntityByID(EntityID id) {

    auto lambda = [id](const std::shared_ptr <Entity> &e) { return e->getEntityID() == id; };

    for (auto &ent : entities) {
        if (ent.second->getEntityID() == id) {
            // remove from blockID map
            auto &vec = entitiesByBlockID[ent.second->getBlockID()];
            vec.erase(std::remove_if(vec.begin(), vec.end(), lambda), vec.end());

            // remove from entityID map
            entities.erase(id);

            return true;
        }
    }
    return false;
}

std::optional<std::shared_ptr<Chunk>> ChunkManager::getChunkByXZ(const glm::vec2 xzCoords) {

    std::pair<int, int> findKey;
    findKey.first = (int) xzCoords.x / EngineConstants::CHUNK_WIDTH;
    findKey.second = (int) xzCoords.y / EngineConstants::CHUNK_LENGTH;

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
        LOG(DEBUG) << "Did not find Chunk at coords " << xInd * EngineConstants::CHUNK_WIDTH << " "
                   << zInd * EngineConstants::CHUNK_LENGTH;
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
                    ((int) xzCoords.x / EngineConstants::CHUNK_WIDTH) + x,
                    ((int) xzCoords.y / EngineConstants::CHUNK_LENGTH) + z);
            if (neighborChunk.has_value())
                out.push_back((*neighborChunk));
        }
    }

    return out;
}

ChunkManager::ChunkManager(const WorldInfo &worldInfo) {

    for (unsigned int chunkX = 0; chunkX < worldInfo.getWidth() / EngineConstants::CHUNK_WIDTH; chunkX++) {
        for (unsigned int chunkZ = 0; chunkZ < worldInfo.getLength() / EngineConstants::CHUNK_LENGTH; chunkZ++) {
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

WorldInfo::WorldInfo(Config conf) {
    this->width = conf.worldSize;
    this->length = conf.worldSize;

    if (conf.seed == EngineConstants::RANDOM_SEED)
        this->seed = generateSeed();
    else
        this->seed = conf.seed;
}

WorldInfo::WorldInfo() {
    this->seed = generateSeed();
}

bool ChunkManager::removeEntityFromChunk(Entity &entity) {

    std::optional<std::shared_ptr<Chunk>> optChunk = getChunkByXZ(
            {entity.getTransform().getPosition().x, entity.getTransform().getPosition().z});

    if (optChunk.has_value()) {
        return (*optChunk)->removeEntityByID(entity.getEntityID());
    } else {
        LOG(DEBUG) << "Could not find chunk for entity at " << entity.getTransform().getPosition().x << " "
                   << entity.getTransform().getPosition().y << " " << entity.getTransform().getPosition().z;
        return false;
    }
}
