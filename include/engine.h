//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include "../libs/easylogging++.h"
#include "mesh.h"
#include "objloader.h"
#include "texture.h"
#include "shader.h"
#include "model.h"
#include "texture_database.h"
#include "entity.h"
#include "model_database.h"
#include "player.h"

/// Config for the application
struct Config {
    int windowWidth = 1024;
    int windowHeight = 768;
    float fov = 45.0f;
};

#define WORLD_LENGTH_X_WIDTH 8
#define WORLD_HEIGHT 16

struct World {
    int map[WORLD_LENGTH_X_WIDTH][WORLD_HEIGHT][WORLD_LENGTH_X_WIDTH];
    int seed;
};

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:
    Config config;
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    std::unordered_map<BlockID, std::vector<Entity>> entities{};

    // Generates the heightmap for the world using simplex method and renders it
    void generateWorld();

    // Generates a random seed for the world that's fed to the simplex noise generator
    void generateSeed();

    // Takes in a set of coordinates and renders a tree on top of that block
    void drawTree(int x, int y, int z);

    std::unique_ptr<World> world;
    std::unique_ptr<Player> player = std::make_unique<Player>();

public:

    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

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

    void mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos);

    GLFWwindow *getWindow() const;

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;
};

/** Check if there is a collision between 2 entities on the X axis
 *
 * @param a first entity
 * @param b second entity
 * @return if the entities collide
 */
inline bool checkCollisionX(Entity &a, Entity &b) {

    bool collisionMinX = (a.getTransform().position.x < b.getTransform().position.x + b.box.dimensions.x &&
            a.getTransform().position.x > b.getTransform().position.x);
    bool collisionMaxX = (
            a.getTransform().position.x + a.box.dimensions.x < b.getTransform().position.x + b.box.dimensions.x &&
            a.getTransform().position.x + a.box.dimensions.x > b.getTransform().position.x);
    return collisionMinX || collisionMaxX;
}

/** Check if there is a collision between 2 entities on the Y axis
 *
 * @param a first entity
 * @param b second entity
 * @return if the entities collide
 */
inline bool checkCollisionY(Entity &a, Entity &b) {

    bool collisionMinY = (a.getTransform().position.y < b.getTransform().position.y + b.box.dimensions.y &&
            a.getTransform().position.y > b.getTransform().position.y);
    bool collisionMaxY = (
            a.getTransform().position.y + a.box.dimensions.y < b.getTransform().position.y + b.box.dimensions.y &&
            a.getTransform().position.y + a.box.dimensions.y > b.getTransform().position.y);
    return collisionMinY || collisionMaxY;
}

/** Check if there is a collision between 2 entities on the Z axis
 *
 * @param a first entity
 * @param b second entity
 * @return if the entities collide
 */
inline bool checkCollisionZ(Entity &a, Entity &b) {

    bool collisionMinZ = (a.getTransform().position.z < b.getTransform().position.z + b.box.dimensions.z &&
            a.getTransform().position.z + a.box.dimensions.z > b.getTransform().position.z);
    bool collisionMaxZ = (
            a.getTransform().position.z + a.box.dimensions.z < b.getTransform().position.z + b.box.dimensions.z &&
            a.getTransform().position.z + a.box.dimensions.z > b.getTransform().position.z);
    return collisionMinZ || collisionMaxZ;
}
