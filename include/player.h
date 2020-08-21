//
// Created by Ewan on 8/8/2020.
//
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <array>
#include "entity.h"
#include "sound_database.h"
#include "camera.h"

class Chunk;

class Engine;

class Player : public Entity {
public:
    Player();

    Player(glm::vec3 spawnPoint);

    /// Returns the player's current view matrix
    glm::mat4 getPlayerView() { return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up); }

    /// Updates the player (movement physics and collision physics)
    void update(Engine *engine, float dt);

    void draw(Shader &shader) override;

    /// Processes the player's inputs
    void processInput(Engine *engine);

    /// Manipulates the camera's view based on the player's mouse input
    void look(GLFWwindow *window, double xpos, double ypos);

    inline void setPosition(glm::vec3 position) {
        this->transform.setPosition(position);
    }

    Camera camera;
private:
    bool onGround = true;
    bool firstPerson = true;
    glm::vec3 cameraDisplacement = glm::vec3(0.5f, 1.5f, 0.5f);
    glm::vec3 velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 acceleration = {0.0f, 0.0f, 0.0f};
    BlockID selectedBlockID = BlockID::DIRT;

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

    /// Tries to remove an entity from the world based on player input
    void removeEntity(Engine *engine) const;

    /// Tries to place a block based on player input
    void placeBlock(Engine *engine);
};