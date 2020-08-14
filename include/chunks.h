//
// Created by Willi on 8/13/2020.
//
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include "block.h"
#include "entity.h"

static constexpr unsigned int CHUNK_WIDTH = 16;
static constexpr unsigned int CHUNK_HEIGHT = 16;
static constexpr unsigned int CHUNK_LENGTH = 16;

static constexpr unsigned int WORLD_WIDTH = 128;
static constexpr unsigned int WORLD_HEIGHT = 16;
static constexpr unsigned int WORLD_LENGTH = 128;

// Some Shenanigans to be able to use the xz coords as a map key
// ----------------------------
struct VecXZ {
    float x;
    float z;

    VecXZ() = default;

    VecXZ(const glm::vec2 &xzVec) {
        x = xzVec.x;
        z = xzVec.y;
    }
};

inline bool operator==(const VecXZ &left, const VecXZ &right) noexcept {
    return (left.x == right.x) && (left.z == right.z);
}

namespace std {
    template<>
    struct hash<VecXZ> {
        size_t operator()(const VecXZ &vect) const noexcept {
            std::hash<decltype(vect.x)> hasher;

            auto hash1 = hasher(vect.x);
            auto hash2 = hasher(vect.z);

            return std::hash<decltype(vect.x)>{}((hash1 ^ hash2) >> 2);
        }
    };
}
// ----------------------------

class WorldInfo {
private:
    unsigned int width = WORLD_WIDTH;   //x
    unsigned int height = WORLD_HEIGHT; //y
    unsigned int length = WORLD_LENGTH; //z
    unsigned int seed;

    /// Generates a random seed for the world that's fed to the simplex noise generator
    static unsigned int generateSeed();

public:
    WorldInfo();

    unsigned int getWidth() const { return width; }

    unsigned int getHeight() const { return height; }

    unsigned int getLength() const { return length; }

    unsigned int getSeed() const { return seed; }
};

class Chunk {
private:
    std::unordered_map<BlockID, std::vector<Entity>> entities{};
    glm::vec2 origin; //XZ

    /** Checks if the given XZ coordinates
     *
     * @param xzCoords the coordinates to check, xz components
     * @return
     */
    bool isBlockOutOfBounds(glm::vec2 xzCoords);

public:
    Chunk(glm::vec2 origin);

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Engine.
    inline void addEntity(Entity &&entity) {

        if (isBlockOutOfBounds({entity.getTransform().getPosition().x, entity.getTransform().getPosition().z})) {
            LOG(DEBUG) << "Adding an entity to Chunk at " << origin.x << " " << origin.y << " that is out of bounds at "
                       << entity.getTransform().getPosition().x << " " << entity.getTransform().getPosition().z;
        }

        entities[entity.getBlockID()].push_back(entity);
    }

    /** Renders the entity in this Chunk
     *
     * @param shader the shader to use to draw the entities
     */
    void renderChunk(Shader &shader);

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

    glm::vec2 getChunkOrigin() const { return origin; }
};

class ChunkManager {
private:
    std::unordered_map<VecXZ, std::shared_ptr<Chunk>> chunks;
public:
    ChunkManager(const WorldInfo &worldInfo);

    /** Returns a specific chunk based on given XZ coordinates
     *
     * @param xzCoords the XZ coordinates
     * @return shared_ptr to a Chunk
     */
    std::optional<std::shared_ptr<Chunk>> getChunkbyXZ(const glm::vec2 xzCoords);

    /** Returns surrounding chunks (max of 9) in a square for given XZ coordinates
     *
     * @param xzCoords the xz coordinates
     * @return a vector
     */
    std::vector<std::shared_ptr<Chunk>> getSurroundingChunksByXZ(const glm::vec2 xzCoords);
};