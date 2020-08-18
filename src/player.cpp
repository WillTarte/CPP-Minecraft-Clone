//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::CUBE, BlockID::DIRT,
                          Transform({20.0, 30.0, 20.0}, {1, 1, 1}, {0, 0, 0})) {
    this->camera = Camera{};
}

Player::Player(glm::vec3 spawnPoint) : Entity(ModelType::CUBE, BlockID::DIRT,
                                              Transform(spawnPoint, {1, 1, 1}, {0, 0, 0})) {
    this->camera = Camera{};
}

void Player::jump() {
    if (onGround) {
        onGround = false;
        this->acceleration.y = 25.0f;
    }
}

void Player::look(GLFWwindow *window, double xpos, double ypos) {

    static double lastXpos = 0.0, lastYpos = 0.0;
    double changeX = xpos - lastXpos, changeY = ypos - lastYpos;

    //TODO: Player can rotate on Y axis
    //this->getTransform().rotate({0.0, 1.0, 0.0}, glm::radians(changeX * 0.05f));

    this->camera.Pitch -= static_cast<float>(changeY) * 0.5f;
    if (this->camera.Pitch > 89.0f)
        this->camera.Pitch = 89.0f;
    if (this->camera.Pitch < -89.0f)
        this->camera.Pitch = -89.0f;

    this->camera.Yaw += static_cast<float>(changeX) * 0.5f;

    this->camera.updateCameraVectors();
    glfwGetCursorPos(window, &lastXpos, &lastYpos);
}

/// Should do any collision/physics here
void Player::update(Engine *engine, float dt) {

    auto currentChunk = engine->chunkManager->getChunkByXZ(
            {getTransform().getPosition().x, getTransform().getPosition().z});

    velocity += acceleration;
    velocity *= dt;
    acceleration = {0, velocity.y, 0};

    if (!onGround) {
        acceleration.y -= 50 * dt;
    }

    if (currentChunk.has_value()) {
        collide((*currentChunk));
        checkOnGround((*currentChunk));
    } else {
        auto pos = this->getTransform().getPosition();
        // Automatically replace the player so that they're inside the world bounds.
        if (pos.x > WORLD_WIDTH) {
            this->setPosition(glm::vec3(pos.x-2, pos.y, pos.z));
        }
        else if (pos.x < 0) {
            this->setPosition(glm::vec3(pos.x+2, pos.y, pos.z));
        }
        else if (pos.z > WORLD_LENGTH) {
            this->setPosition(glm::vec3(pos.x, pos.y, pos.z-2));
        }
        else if (pos.z < 0) {
            this->setPosition(glm::vec3(pos.x, pos.y, pos.z+2));
        }

        // Out of bounds!
        LOG(INFO) << "Player is out of bounds at x: " << this->getTransform().getPosition().x << " y:"
                  << this->getTransform().getPosition().y << " z:" << this->getTransform().getPosition().z;
    }

    this->transform.translate(velocity);

    if (this->transform.getPosition().y < 0) {
        LOG(DEBUG) << "Player fell through the world!.";
        this->transform.position.y = 30.0f;
    }

    this->camera.Position = this->getTransform().getPosition() + glm::vec3(0.5f, 1.5f, 0.5f);
}


void Player::processInput(GLFWwindow *window) {

    float speed = 10;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(camera.Front + camera.Right);
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(camera.Front + -(camera.Right));
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(-(camera.Front) + -(camera.Right));
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        glm::vec3 dirVec = glm::normalize(-(camera.Front) + camera.Right);
        this->acceleration.x += dirVec.x * speed;
        this->acceleration.z += dirVec.z * speed;
    } else {

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            this->acceleration.x += camera.Front.x * speed;
            this->acceleration.z += camera.Front.z * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            this->acceleration.x += camera.Front.x * -speed;
            this->acceleration.z += camera.Front.z * -speed;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            this->acceleration.x += camera.Right.x * -speed;
            this->acceleration.z += camera.Right.z * -speed;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            this->acceleration.x += camera.Right.x * speed;
            this->acceleration.z += camera.Right.z * speed;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            this->jump();
        }
    }
}

void Player::collide(const std::shared_ptr<Chunk> &currentChunk) {

    std::optional<Entity *> optEnt;

    optEnt = (*currentChunk).getEntityByBoxCollision(
            this->getTransform().getPosition() + velocity,
            this->box);

    if (optEnt.has_value()) {
        if (velocity.y > 0.0f) {
            if (this->getTransform().getPosition().y + this->box.dimensions.y <
                (*optEnt)->getTransform().getPosition().y) {
                this->transform.position.y = glm::ceil((*optEnt)->getTransform().getPosition().y - 1.0f);
                velocity.y = 0.0f;
            }
        } else if (velocity.y < 0.0f) {
            if (this->getTransform().getPosition().y >
                (*optEnt)->getTransform().getPosition().y + (*optEnt)->box.dimensions.y) {
                this->transform.position.y = glm::floor((*optEnt)->getTransform().getPosition().y + 1.0f);
                onGround = true;
                velocity.y = 0.0f;
            }
        }
        velocity.x = 0.0f;
        velocity.z = 0.0f;
    }

}

void Player::checkOnGround(const std::shared_ptr<Chunk> &currentChunk) {
    std::optional<Entity *> optEntity = (*currentChunk).getEntityByBoxCollision(
            this->getTransform().getPosition() + glm::vec3(0.0f, -0.2f, 0.0f), this->box);

    onGround = optEntity.has_value();
}
