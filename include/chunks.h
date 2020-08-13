//
// Created by Willi on 8/13/2020.
//
#pragma once

#include <map>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <optional>
#include "entity.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 16
#define CHUNK_LENGTH 16

class Chunk {
private:
    std::unordered_map<BlockID, std::vector<Entity>> entities{};
    glm::vec2 origin; //XZ
public:
    Chunk(glm::vec2 origin);

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Engine.
    inline void addEntity(Entity &&entity) { entities[entity.getBlockID()].push_back(entity); }

    /** Returns an entity in absolute world position
     *
     * @param worldPos the world pos (truncates to integers)
     * @return an optional pointer to an entity
     */
    std::optional<Entity *> getEntityByWorldPos(const glm::vec3 worldPos);

    /** Returns an entity based on a bounding box overlap
     *
     * @param worldPos the world position
     * @param box the bounding box
     * @return an optional pointer to an entity
     */
    std::optional<Entity *> getEntityByBoxCollision(glm::vec3 worldPos, BoundingBox box);
};

class ChunkManager {
private:
    std::map<glm::vec2, std::shared_ptr<Chunk>> chunks;
public:
    ChunkManager() = default;

    /** Returns a specific chunk based on given XZ coordinates
     *
     * @param xzCoords the XZ coordinates
     * @return shared_ptr to a Chunk
     */
    std::shared_ptr<Chunk> getChunkbyXZ(const glm::vec2 xzCoords);

    /** Returns surrounding chunks (max of 9) in a square for given XZ coordinates
     *
     * @param xzCoords the xz coordinates
     * @return a vector
     */
    std::vector<std::shared_ptr<Chunk>> getSurroundingChunksByXZ(const glm::vec2 xzCoords);
};