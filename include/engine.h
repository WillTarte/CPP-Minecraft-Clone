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

/// Basically the entry point into the game. Orchestrates all the systems.
class Engine {
private:

    Config config;
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    Camera camera = Camera(glm::vec3(5.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);

public:

    explicit Engine(Config config);

    /// Runs the main game loop.
    void runLoop();

    /// Processes keyboard + mouse inputs
    void processInput(float deltatime);

    void mouseCallbackFunc(double xpos, double ypos);

    GLFWwindow *getWindow() const;

    Engine(const Engine &) = delete;

    Engine & operator=(const Engine &) = delete;
};
