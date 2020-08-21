//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <optional>
#include "engine_constants.h"
#include "frustum.h"
#include "shader.h"
#include "player.h"
#include "chunks.h"

#if defined __unix__ || _MSC_VER >= 1914
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem::v1;
#endif


/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:
    GLFWwindow *window{};
    unsigned int windowWidth = EngineConstants::DEFAULT_WINDOW_WIDTH;
    unsigned int windowHeight = EngineConstants::DEFAULT_WINDOW_HEIGHT;
    Config config;
    WorldInfo worldInfo;
    std::unique_ptr<Player> player;
    std::unique_ptr<ChunkManager> chunkManager;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<Sun> sun;

    /// Generates the heightmap for the world using simplex method and renders it
    void generateWorld();

    /// Takes in a set of coordinates and renders a tree on top of that block
    void addTree(unsigned int x, unsigned int y, unsigned int z) const;

    /// Takes in a set of coordinates and renders the model H3 top of that block
    void addH3(unsigned int x, unsigned int y, unsigned int z) const;
    void addL8(unsigned int x, unsigned int y, unsigned int z) const;
    void addP8(unsigned int x, unsigned int y, unsigned int z) const;
    void addH7(unsigned int x, unsigned int y, unsigned int z) const;
    void addA2(unsigned int x, unsigned int y, unsigned int z) const;

public:
    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    /// initializes the game world
    void init();

    /// callback for mouse movement, used for user input
    void mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos);

    [[nodiscard]] inline GLFWwindow *getWindow() const { return window; };

    [[nodiscard]] inline unsigned int getWindowWidth() const { return windowWidth; }

    [[nodiscard]] inline unsigned int getWindowHeight() const { return windowHeight; }

    inline Config &getConfig() { return config; }

    WorldInfo &getWorldInfo() { return worldInfo; };

    inline std::unique_ptr<ChunkManager> &getChunkManager() { return chunkManager; }

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;
};
