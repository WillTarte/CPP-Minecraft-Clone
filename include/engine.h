//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "../include/mesh.h"
#include "../include/objloader.h"
#include "../include/texture.h"
#include "../include/shader.h"
#include "../include/model.h"

struct Config {
    int windowWidth = 1024;
    int windowHeight = 768;
    float fov = 45.0f;
};

class Engine{
private:

    Config config;
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    Camera camera = Camera(glm::vec3(5.0f, 1.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH);

public:

    explicit Engine(Config config);

    void runLoop();
    void processInput(float deltatime);
    void mouseCallbackFunc(double xpos, double ypos);

    GLFWwindow *getWindow() const;

    Engine(const Engine &) = delete;
    Engine & operator=(const Engine &) = delete;
};
