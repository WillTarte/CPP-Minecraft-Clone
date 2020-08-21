//
// Created by Willi on 8/13/2020.
//
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <optional>
#include <unordered_map>
#include "engine_constants.h"
#include "block.h"
#include "entity.h"
#include "frustum.h"

/// Config for the application
struct Config {
    int windowWidth = EngineConstants::DEFAULT_WINDOW_WIDTH;
    int windowHeight = EngineConstants::DEFAULT_WINDOW_HEIGHT;
    int seed = EngineConstants::RANDOM_SEED;
    int worldSize = EngineConstants::SMALL_WORLD;
    float fov = 45.0f;
};

/// Contains basic world info (size and seed)
class WorldInfo {
private:
    unsigned int width = EngineConstants::SMALL_WORLD;   //x
    unsigned int height = EngineConstants::DEFAULT_WORLD_HEIGHT; //y
    unsigned int length = EngineConstants::SMALL_WORLD; //z
    int seed;

    /// Generates a random seed for the world that's fed to the simplex noise generator
    static int generateSeed();

public:
    WorldInfo();

    explicit WorldInfo(Config config);

    [[nodiscard]] unsigned int getWidth() const { return width; }

    [[nodiscard]] unsigned int getHeight() const { return height; }

    [[nodiscard]] unsigned int getLength() const { return length; }

    [[nodiscard]] int getSeed() const { return seed; }
};

/// A Chunk starts at some XZ index (from 0 to WORLD_LENGTH / CHUNK_LENGTH ...) and contains entities
class Chunk {
private:
    std::map<EntityID, std::shared_ptr<Entity>> entities{};
    std::map<BlockID, std::vector<std::shared_ptr<Entity>>> entitiesByBlockID{};
    std::pair<unsigned int, unsigned int> origin; // X / CHUNK_WIDTH, Z / CHUNK_LENGTH

    /** Checks if the given XZ coordinates are outside this Chunk
     *
     * @param xzCoords the coordinates to check, xz components
     * @return
     */
    [[nodiscard]] bool isBlockOutOfBounds(glm::vec2 xzCoords) const;

public:
    Chunk(unsigned int xInd, unsigned int zInd);

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Chunk.
    inline void addEntity(Entity &&entity) {
        if (isBlockOutOfBounds({entity.getTransform().getPosition().x, entity.getTransform().getPosition().z})) {
            LOG(DEBUG) << "Adding an entity to Chunk at " << origin.first * EngineConstants::CHUNK_WIDTH << " "
                       << origin.second * EngineConstants::CHUNK_LENGTH << " that is out of bounds at "
                       << entity.getTransform().getPosition().x << " " << entity.getTransform().getPosition().z;
        }

        std::shared_ptr<Entity> ent = std::make_shared<Entity>(std::move(entity));
        entities[ent->getEntityID()] = ent;
        entitiesByBlockID[ent->getBlockID()].push_back(ent);
    }

    /// Returns a reference to this chunk's entities
    std::map<EntityID, std::shared_ptr<Entity>> &getEntities() {
        return entities;
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
    std::optional<std::shared_ptr<Entity>> getEntityByWorldPos(glm::vec3 worldPos);

    /** Returns an entity based on a bounding box overlap
     *
     * @param worldPos the world position
     * @param box the bounding box
     * @return an optional pointer to an entity
     */
    std::optional<std::shared_ptr<Entity>> getEntityByBoxCollision(glm::vec3 worldPos, BoundingBox box);

    /** Removes an entity from this chunk (and therefore from the world). Invalidates any references to this entity.
     *
     * @param id the entity to remove's ID
     * @return true if sucessfull, false otherwise
     */
    bool removeEntityByID(EntityID id);

    /// Returns the chunk's origin in world coordinates for the X and Z components
    [[nodiscard]] glm::vec2 getChunkOrigin() const {
        return {origin.first * EngineConstants::CHUNK_WIDTH, origin.second * EngineConstants::CHUNK_LENGTH};
    }

    [[nodiscard]] inline size_t getNumberOfEntities() const;
};

/// The ChunkManager manages all the chunks in the world. The number of chunks depend on the world and chunk sizes.
class ChunkManager {
private:
    std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<Chunk>> chunks;
public:
    explicit ChunkManager(const WorldInfo &worldInfo);

    /** Returns a specific chunk based on given XZ coordinates
     *
     * @param xzCoords the XZ coordinates
     * @return shared_ptr to a Chunk
     */
    std::optional<std::shared_ptr<Chunk>> getChunkByXZ(glm::vec2 xzCoords);

    /** Returns surrounding chunks (max of 9) in a square for given XZ coordinates
     *
     * @param xzCoords the xz coordinates
     * @return a vector
     */
    std::vector<std::shared_ptr<Chunk>> getSurroundingChunksByXZ(glm::vec2 xzCoords);

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

    [[nodiscard]] size_t getNumberOfEntities() const;

    [[nodiscard]] inline size_t getNumberOfChunks() const { return this->chunks.size(); };
};