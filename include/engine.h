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

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:
    Config config;
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    std::unordered_map<BlockID, std::vector<Entity>> entities{};
    std::unique_ptr<Player> player = std::make_unique<Player>();

public:

    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Engine.
    inline void addEntity(Entity &&entity) { entities[entity.getBlockID()].push_back(entity); }

    std::optional<Entity *> getEntityByWorldPos(const glm::vec3 worldPos);

    void mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos);

    GLFWwindow *getWindow() const;

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;
};

inline bool checkCollision(Entity &a, Entity &b) {

    bool collisionX = (a.getTransform().position.x < b.getTransform().position.x + b.box.dimensions.x &&
                       a.getTransform().position.x + a.box.dimensions.y > b.getTransform().position.x);
    bool collisionY = (a.getTransform().position.y < b.getTransform().position.y + b.box.dimensions.y &&
                       a.getTransform().position.y + a.box.dimensions.y > b.getTransform().position.y);
    bool collisionZ = (a.getTransform().position.z < b.getTransform().position.z + b.box.dimensions.z &&
                       a.getTransform().position.z + a.box.dimensions.z > b.getTransform().position.z);

    return collisionX && collisionY && collisionZ;
}
