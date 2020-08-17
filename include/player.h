//
// Created by Ewan on 8/8/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include "entity.h"
#include "camera.h"
#include "chunks.h"

class Engine;

enum VelocityComponent {
    X,
    Y,
    Z
};

class Player : public Entity {
public:
    Player();

    Player(glm::vec3 spawnPoint);

    /// Returns the player's current view matrix
    glm::mat4 getPlayerView() { return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up); }

    /// Updates the player (movement physics and collision physics)
    void update(Engine *engine, float dt);

    /// Processes the player's inputs

    void processInput(Engine *engine);


    void removeEntity(glm::vec3 dir, Engine *engine);
    void placeBlock(glm::vec3 dir, Engine *engine);
    bool checkIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayVector, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2);

    /// Manipulates the camera's view based on the player's mouse input
    void look(GLFWwindow *window, double xpos, double ypos);

private:
    Camera camera;
    bool onGround = true;
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
    /// Jumps
    void jump();

    /** Checks the player's collision based on a given axis
     *
     * @param engine the engine
     * @param comp the velocity component/axis to verify collisions
     */
    void collide(const std::shared_ptr<Chunk> &currentChunk);

    /** Checks if the player is on the ground (gravity)
     *
     * @param currentChunk the current chunk
     */
    void checkOnGround(const std::shared_ptr<Chunk> &currentChunk);
};