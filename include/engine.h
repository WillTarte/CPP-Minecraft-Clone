//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include "shader.h"
#include "chunks.h"
#include "player.h"

#if defined __unix__ || _MSC_VER >= 1914
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem::v1;
#endif

/// Config for the application
struct Config {
    int windowWidth = 1024;
    int windowHeight = 768;
    float fov = 45.0f;
};

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:
    Config config;
    GLFWwindow *window;
    int windowWidth;
    int windowHeight;

    /// Generates the heightmap for the world using simplex method and renders it
    void generateWorld();

    /// Takes in a set of coordinates and renders a tree on top of that block
    void addTree(unsigned int x, unsigned int y, unsigned int z) const;

    WorldInfo worldInfo;
    std::unique_ptr<Player> player;

public:

    std::unique_ptr<ChunkManager> chunkManager;

    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    void mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos);
    void removeEntity(glm::vec3 dir);
    void placeBlock(glm::vec3 dir);

    bool checkIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayVector, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2);
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
