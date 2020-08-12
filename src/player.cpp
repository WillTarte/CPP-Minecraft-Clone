//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::CUBE, BlockID::DIRT,
                          Transform({20.0, 2.0, 20.0}, {1, 1, 1}, {0, 0, 0})) {
    this->camera = Camera{};
    //this->camera.Position = this->getTransform().getPosition();
}

void Player::jump(float dt) {
    if (onGround) {
        onGround = false;
        this->acceleration.y += 100;
    }
}

void Player::look(GLFWwindow *window, double xpos, double ypos) {

    static double lastXpos = 0.0, lastYpos = 0.0;
    double changeX = xpos - lastXpos, changeY = ypos - lastYpos;

    // Player can rotate on Y axis
    this->getTransform().rotate({0.0, 1.0, 0.0}, glm::radians(changeX * 0.05f));

    this->camera.Pitch -= changeY * 0.5;
    if (this->camera.Pitch > 89.0f)
        this->camera.Pitch = 89.0f;
    if (this->camera.Pitch < -89.0f)
        this->camera.Pitch = -89.0f;

    this->camera.Yaw += changeX * 0.5;

    this->camera.updateCameraVectors();
    glfwGetCursorPos(window, &lastXpos, &lastYpos);
}

/// Should do any collision/physics here
void Player::update(Engine *engine, float dt) {

    velocity += acceleration;
    velocity *= dt;
    acceleration = {0, 0, 0};

    if (!onGround) {
        velocity.y -= 2 * dt;
    }

    //std::cout << "Before: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl;

    collide(engine, velocity, VelocityComponent::Y, dt);
    collide(engine, velocity, VelocityComponent::X, dt);
    collide(engine, velocity, VelocityComponent::Z, dt);

    //std::cout << "After: " << velocity.x << " " << velocity.y << " " << velocity.z << std::endl << std::endl;

    this->transform.translate(velocity);

    std::cout << this->getTransform().getPosition().x << " " << this->getTransform().getPosition().y << " "
              << this->getTransform().getPosition().z << std::endl;

    this->camera.Position = this->getTransform().getPosition() + glm::vec3(0.5f, 1.5f, 0.5f);
}


void Player::processInput(GLFWwindow *window, float dt) {

    float speed = 5;
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
        this->jump(dt);
    }
}

void Player::collide(Engine *engine, glm::vec3 &vel, VelocityComponent comp, float dt) {

    std::optional<Entity *> optEnt;

    if (comp == VelocityComponent::Y) {
        if (vel.y > 0) {
            optEnt = engine->getEntityByWorldPos(
                    this->getTransform().position + glm::vec3(0, this->box.dimensions.y, 0) +
                    glm::vec3(0, velocity.y, 0));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Positive Y \n";
                this->transform.position.y = glm::ceil((*optEnt)->getTransform().getPosition().y - 1);
                velocity.y = 0;
            }
        } else if (vel.y < 0) {
            optEnt = engine->getEntityByWorldPos(this->getTransform().position + glm::vec3(velocity.x, 0, 0));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Negative Y \n";
                this->transform.position.y = glm::floor((*optEnt)->getTransform().getPosition().y + 1);
                onGround = true;
                velocity.y = 0;
            }
        }
    }

    if (comp == VelocityComponent::X) {
        if (vel.x > 0) {
            optEnt = engine->getEntityByWorldPos(
                    this->getTransform().position + glm::vec3(this->box.dimensions.x, 0, 0) +
                    glm::vec3(velocity.x, 0, 0));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Positive X \n";
                velocity.x = 0;
            }
        } else if (vel.x < 0) {
            optEnt = engine->getEntityByWorldPos(this->getTransform().position + glm::vec3(velocity.x, 0, 0));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Negative X \n";
                velocity.x = 0;
            }
        }

    }

    if (comp == VelocityComponent::Z) {
        if (vel.z > 0) {
            optEnt = engine->getEntityByWorldPos(
                    this->getTransform().position + glm::vec3(0, 0, this->box.dimensions.z) +
                    glm::vec3(0, 0, velocity.z));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Positive Z \n";
                velocity.z = 0;
            }
        } else if (vel.z < 0) {
            optEnt = engine->getEntityByWorldPos(this->getTransform().position + glm::vec3(0, 0, velocity.z));
            if (optEnt.has_value() && checkCollision(*this, *(*optEnt))) {
                std::cout << " Negative Z \n";
                velocity.z = 0;
            }
        }
    }
}