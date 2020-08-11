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
    bool onGround;

    void jump();

    void collide(const BoundingBox &entityBox, glm::vec3 vel, float dt);
};