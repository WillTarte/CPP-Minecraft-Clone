//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::CUBE, BlockID::DIRT,
                          Transform({20.0, 60.0, 20.0}, {1, 1, 1}, {0, 0, 0})) {
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
    acceleration = {0, velocity.y, 0};

    if (!onGround) {
        acceleration.y -= 20 * dt;
    }

    collide(engine, VelocityComponent::Y);
    collide(engine, VelocityComponent::X);
    collide(engine, VelocityComponent::Z);
    checkOnGround(engine);

    this->transform.translate(velocity);
    this->camera.Position = this->getTransform().getPosition() + glm::vec3(0.5f, 1.5f, 0.5f);
}


void Player::processInput(GLFWwindow *window, float dt) {

    float speed = 4;
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

void Player::collide(Engine *engine, VelocityComponent comp) {

    std::optional<Entity *> optEnt;

    if (comp == VelocityComponent::Y) {
        optEnt = engine->getEntityByBoxCollision(this->getTransform().getPosition() + glm::vec3(0, velocity.y, 0),
                                                 this->box);
        if (velocity.y > 0) {
            if (optEnt.has_value() && checkCollisionY(*this, *(*optEnt))) {
                std::cout << "\nBONK\n";
                this->transform.position.y = glm::ceil((*optEnt)->getTransform().getPosition().y - 1) - 0.01;
                velocity.y = 0;
            }
        } else if (velocity.y < 0) {
            if (optEnt.has_value() && checkCollisionY(*this, *(*optEnt))) {
                this->transform.position.y = glm::floor((*optEnt)->getTransform().getPosition().y + 1) + 0.01;
                onGround = true;
                velocity.y = 0;
            }
        }
    }

    if (comp == VelocityComponent::X) {
        optEnt = engine->getEntityByBoxCollision(this->getTransform().getPosition() + glm::vec3(velocity.x, 0, 0),
                                                 this->box);
        if (velocity.x > 0) {
            if (optEnt.has_value() && checkCollisionX(*this, *(*optEnt))) {
                velocity.x = 0;
            }
        } else if (velocity.x < 0) {
            if (optEnt.has_value() && checkCollisionX(*this, *(*optEnt))) {
                velocity.x = 0;
            }
        }
    }

    if (comp == VelocityComponent::Z) {
        optEnt = engine->getEntityByBoxCollision(this->getTransform().getPosition() + glm::vec3(0, 0, velocity.z),
                                                 this->box);
        if (velocity.z > 0) {
            if (optEnt.has_value() && checkCollisionZ(*this, *(*optEnt))) {
                velocity.z = 0;
                acceleration.z = 0;
            }
        } else if (velocity.z < 0) {
            if (optEnt.has_value() && checkCollisionZ(*this, *(*optEnt))) {
                velocity.z = 0;
            }
        }
    }
}

void Player::checkOnGround(Engine *engine) {
    std::optional<Entity *> optEntity = engine->getEntityByBoxCollision(
            this->getTransform().getPosition() + glm::vec3(0.0f, -0.2f, 0.0f), this->box);

    onGround = optEntity.has_value();
}
