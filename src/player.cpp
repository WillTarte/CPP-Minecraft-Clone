//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::STEVE, BlockID::PLAYER,
                          Transform({20.0, 2.0, 20.0}, {0.23, 0.23, 0.23}, {0, 0, 0})) {
    this->camera = Camera{};
    this->camera.Position = this->getTransform().getPosition();
}

void Player::jump() {
    if (onGround) {
        onGround = false;
        this->acceleration.y += 20.0f;
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

    this->transform.translate({velocity.x, velocity.y, velocity.z});

    collide(engine, {velocity.x, 0.0f, 0.0f}, dt);
    collide(engine, {0.0f, velocity.y, 0.0f}, dt);
    collide(engine, {0.0f, 0.0f, velocity.z}, dt);


    this->camera.Position = this->getTransform().getPosition() + glm::vec3(1.0f, 1.0f, 1.0f);
    velocity.x *= 0.95;
    velocity.z *= 0.95;
}


void Player::processInput(GLFWwindow *window) {

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
        this->jump();
    }
}

void Player::collide(Engine *engine, glm::vec3 vel, float dt) {

    for (int x : {0, 1}) {
        for (int y : {0, 1}) {
            for (int z : {0, 1}) {
                std::optional<Entity *> optEntity = engine->getEntityByWorldPos(
                        this->transform.getPosition() + glm::vec3(x, y, z));

                if (optEntity.has_value()) {

                    std::cout << "Player " << this->getTransform().getPosition().x << " "
                              << this->getTransform().getPosition().y << " " << this->getTransform().getPosition().z;
                    std::cout << " Cube " << (*optEntity)->getTransform().getPosition().x << " "
                              << (*optEntity)->getTransform().getPosition().y << " "
                              << (*optEntity)->getTransform().getPosition().z << std::endl;

                    /*if (vel.y > 0) {
                        this->transform.position.y += y - box.dimensions.y;
                        velocity.y = 0;
                    }
                    else if (vel.y < 0) {
                        onGround = true;
                        this->transform.position.y += y + box.dimensions.y + 1;
                        velocity.y = 0;
                    }

                    if (vel.x > 0) {
                        this->transform.position.x += x - box.dimensions.x;
                    }
                    else if (vel.x < 0) {
                        this->transform.position.x += x + box.dimensions.x + 1;
                    }

                    if (vel.z > 0) {
                        this->transform.position.z += z - box.dimensions.z;
                    }
                    else if (vel.z < 0) {
                        this->transform.position.z += z + box.dimensions.z + 1;
                    }*/

                    if (vel.y > 0) {
                        this->transform.position.y += 1;
                        velocity.y = 0;
                    } else if (vel.y < 0) {
                        onGround = true;
                        this->transform.position.y += 1;
                        velocity.y = 0;
                    }

                    if (vel.x > 0) {
                        this->transform.position.x += 1;
                    } else if (vel.x < 0) {
                        this->transform.position.x += 1;
                    }

                    if (vel.z > 0) {
                        this->transform.position.z += 1;
                    } else if (vel.z < 0) {
                        this->transform.position.z += 1;
                    }

                    /*if (vel.y > 0) {
                        float yDisp = -(this->getTransform().getPosition().y + this->box.dimensions.y) - (*optEntity)->getTransform().getPosition().y;
                        this->transform.translate({0.0f, yDisp, 0.0f});
                        velocity.y = 0;
                    } else if (vel.y < 0) {
                        float yDisp = ((*optEntity)->getTransform().getPosition().y + (*optEntity)->box.dimensions.y) - this->getTransform().getPosition().y;
                        onGround = true;
                        this->transform.translate({0.0f, yDisp, 0.0f});
                        velocity.y = 0;
                    }

                    if (vel.x > 0) {
                        float xDisp = -(this->getTransform().getPosition().x + this->box.dimensions.x) - (*optEntity)->getTransform().getPosition().x;
                        this->transform.translate({ xDisp, 0.0f, 0.0f});
                        velocity.x = 0;
                    } else if (vel.x < 0) {
                        float xDisp = ((*optEntity)->getTransform().getPosition().x + (*optEntity)->box.dimensions.x) - this->getTransform().getPosition().x;
                        this->transform.translate({ xDisp, 0.0f, 0.0f});
                        velocity.x = 0;
                    }

                    if (vel.z > 0) {
                        float zDisp = -(this->getTransform().getPosition().z + this->box.dimensions.z) - (*optEntity)->getTransform().getPosition().z;
                        this->transform.translate({0.0f, 0.0f, zDisp});
                        velocity.z = 0;
                    } else if (vel.z < 0) {
                        float zDisp = ((*optEntity)->getTransform().getPosition().z + (*optEntity)->box.dimensions.z) - this->getTransform().getPosition().z;
                        this->transform.translate({0.0f, 0.0f,zDisp});
                        velocity.z = 0;
                    }*/
                }
            }
        }
    }
}
