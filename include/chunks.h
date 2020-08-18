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
#include "frustum.h"

constexpr unsigned int CHUNK_WIDTH = 16;
constexpr unsigned int CHUNK_HEIGHT = 16;
constexpr unsigned int CHUNK_LENGTH = 16;

constexpr unsigned int WORLD_WIDTH = 512;
constexpr unsigned int WORLD_HEIGHT = 16;
constexpr unsigned int WORLD_LENGTH = 512;

/// Contains basic world info (size and seed)
class WorldInfo {
private:
    unsigned int width = WORLD_WIDTH;   //x
    unsigned int height = WORLD_HEIGHT; //y
    unsigned int length = WORLD_LENGTH; //z
    int seed;

    /// Generates a random seed for the world that's fed to the simplex noise generator
    static int generateSeed();

public:
    WorldInfo();

    unsigned int getWidth() const { return width; }

    unsigned int getHeight() const { return height; }

    unsigned int getLength() const { return length; }

    int getSeed() const { return seed; }
};

/// A Chunk starts at some XZ index (from 0 to WORLD_LENGTH / CHUNK_LENGTH ...) and contains entities
class Chunk {
private:
    std::map<BlockID, std::vector<Entity>> entities{};
    std::pair<unsigned int, unsigned int> origin; // X / CHUNK_WIDTH, Z / CHUNK_LENGTH

    /** Checks if the given XZ coordinates are outside this Chunk
     *
     * @param xzCoords the coordinates to check, xz components
     * @return
     */
    bool isBlockOutOfBounds(glm::vec2 xzCoords) const;

public:
    Chunk(unsigned int xInd, unsigned int zInd);

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Chunk.
    inline void addEntity(Entity &&entity) {

        if (isBlockOutOfBounds({entity.getTransform().getPosition().x, entity.getTransform().getPosition().z})) {
            LOG(DEBUG) << "Adding an entity to Chunk at " << origin.first * CHUNK_WIDTH << " "
                       << origin.second * CHUNK_LENGTH << " that is out of bounds at "
                       << entity.getTransform().getPosition().x << " " << entity.getTransform().getPosition().z;
        }

        entities[entity.getBlockID()].push_back(std::move(entity));
    }

    /** Renders the entities in this Chunk
     *
     * @param shader the shader to use to draw the entities
     */
    void renderChunk(Shader &shader, const ViewFrustum &frustum);

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

    /** Removes an entity from this chunk (and therefore from the world). Invalidates any references to this entity.
     *
     * @param entity the entity to try remove.
     * @return true if succeeded, false otherwise.
     */
    bool removeEntity(Entity &entity);

    /// Returns the chunk's origin in world coordinates for the X and Z components
    glm::vec2 getChunkOrigin() const { return {origin.first * CHUNK_WIDTH, origin.second * CHUNK_LENGTH}; }

    inline size_t getNumberOfEntities() const;
};

/// The ChunkManager manages all the chunks in the world. The number of chunks depend on the world and chunk sizes.
class ChunkManager {
private:
    std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<Chunk>> chunks;
public:
    ChunkManager(const WorldInfo &worldInfo);

    /** Returns a specific chunk based on given XZ coordinates
     *
     * @param xzCoords the XZ coordinates
     * @return shared_ptr to a Chunk
     */
    std::optional<std::shared_ptr<Chunk>> getChunkByXZ(const glm::vec2 xzCoords);

    /** Returns surrounding chunks (max of 9) in a square for given XZ coordinates
     *
     * @param xzCoords the xz coordinates
     * @return a vector
     */
    std::vector<std::shared_ptr<Chunk>> getSurroundingChunksByXZ(const glm::vec2 xzCoords);

    /** Gets a chunk bases off a X and Z index (X * CHUNK_WIDTH, Z * CHUNK_LENGTH)
     *
     * @param xInd the x index
     * @param zInd the z index
     * @return a shared_ptr to a Chunk if found, or an empty optional
     */
    std::optional<std::shared_ptr<Chunk>> getChunkByXZIndex(unsigned int xInd, unsigned int zInd);


    /** Removes the given entity from the world (if found). Invalidates any references to that entity.
     *
     * @param entity the entity to try remove from the world.
     * @return true if succeeded, false otherwise.
     */
    bool removeEntityFromChunk(Entity &entity);

    size_t getNumberOfEntities() const;

    inline size_t getNumberOfChunks() const { return this->chunks.size(); };
};