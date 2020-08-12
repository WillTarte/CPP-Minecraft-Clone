//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../libs/easylogging++.h"
#include "camera.h"
#include "mesh.h"
#include "objloader.h"
#include "texture.h"
#include "shader.h"
#include "model.h"
#include "texture_database.h"
#include "entity.h"
#include "model_database.h"

/// Config for the application
struct Config {
    int windowWidth = 1024;
    int windowHeight = 768;
    float fov = 45.0f;
};

struct World {
    int map[512][16][512];
    int seed;
};

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:

    Config config;
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    //TODO: camera should be part of / attached to Player
    Camera camera = Camera(glm::vec3(-5.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);
    std::unordered_map<BlockID, std::vector<Entity>> entities{};
    void generateWorld();
    void generateSeed();
    World* world;

public:

    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    /// Processes keyboard + mouse inputs
    void processInput(float deltatime);

    /// Adds an entity to the world. NB: the entity should be an rvalue. If it is an lvalue, its ownership should be moved using std::move.
    /// <br/><br/>In other words, this method gives ownership of the entity to the Engine.
    inline void addEntity(Entity &&entity) { entities[entity.getBlockID()].push_back(entity); }

    void mouseCallbackFunc(double xpos, double ypos);

    GLFWwindow *getWindow() const;

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;
};
