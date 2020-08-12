//
// Created by Ewan on 8/8/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "camera.h"
#include "entity.h"

class Engine;

class Player : public Entity {
public:
    Player();

    glm::mat4 getPlayerView() { return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up); }

    void update(Engine *engine, float dt);

    void processInput(GLFWwindow *window);

    void look(GLFWwindow *window, double xpos, double ypos);

private:
    Camera camera;
    bool onGround = true;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};

    void jump();

    void collide(Engine *engine, const glm::vec3 vel, float dt);
};