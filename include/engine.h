//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include "frustum.h"
#include "shader.h"
#include "chunks.h"
#include "player.h"

#if defined __unix__ || _MSC_VER >= 1914
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem::v1;
#endif


#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768
#define RANDOM_WORLD_SEED -1

/// Config for the application
struct Config {
    int windowWidth = DEFAULT_WINDOW_WIDTH;
    int windowHeight = DEFAULT_WINDOW_HEIGHT;
    int seed = RANDOM_WORLD_SEED;
    int worldSize = WORLD_WIDTH;
    float fov = 45.0f;
};

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:
    GLFWwindow *window;
    unsigned int windowWidth;
    unsigned int windowHeight;
    Config config;
    WorldInfo worldInfo;
    std::unique_ptr<Player> player;
    std::unique_ptr<ChunkManager> chunkManager;
    std::unique_ptr<Entity> skybox;

    /// Generates the heightmap for the world using simplex method and renders it
    void generateWorld();

    /// Takes in a set of coordinates and renders a tree on top of that block
    void addTree(unsigned int x, unsigned int y, unsigned int z) const;

public:
    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    ///
    void init();

    void mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos);

    inline GLFWwindow *getWindow() const { return window; };

    inline unsigned int getWindowWidth() const { return windowWidth; }

    inline unsigned int getWindowHeight() const { return windowHeight; }

    inline Config getConfig() { return config; }

    inline WorldInfo getWorldInfo() { return worldInfo; };

    inline std::unique_ptr<ChunkManager> &getChunkManager() { return chunkManager; }

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;
};
