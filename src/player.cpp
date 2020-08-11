//
// Created by Ewan on 8/8/2020.
//
#include "../include/player.h"
#include "../include/engine.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::STEVE, BlockID::PLAYER,
                          Transform({20.0, 2.0, 20.0}, {0.23, 0.23, 0.23}, {0, 0, 0})) {
    this->box.updateBox(this->getTransform().getPosition());
    this->camera = Camera{};
    this->camera.Position = this->getTransform().getPosition();
}

void Player::jump() {
    if (onGround) {
        onGround = false;
        this->acceleration.y += 500;
    }
}

//TODO if player looks the model should rotate
void Player::look(GLFWwindow *window, double xpos, double ypos) {

    static double lastXpos = 0.0, lastYpos = 0.0;
    double changeX = xpos - lastXpos, changeY = ypos - lastYpos;

    // Player can rotate on Y axis
    this->getTransform().rotate({0.0, 1.0, 0.0}, glm::radians(changeX * 0.05f));

    this->camera.Pitch += changeY * 0.5;
    this->camera.Yaw += changeX * 0.5;

    this->camera.updateCameraVectors();
    glfwGetCursorPos(window, &lastXpos, &lastYpos);
}

void Player::update(Engine *engine, float dt) {

    this->velocity += this->acceleration;
    this->acceleration = {0, 0, 0};

    if (!onGround) {
        this->velocity.y -= 40 * dt;
    }

    this->getTransform().translate(glm::vec3(velocity.x * dt, 0.0, 0.0));

    std::optional<BoundingBox> optEntity = engine->getEntityBoxByWorldPos(
            {this->getTransform().getPosition().x, 0.0, 0.0});
    if (optEntity.has_value())
        collide(optEntity.value(), glm::vec3(velocity.x, 0.0, 0.0), dt);

    this->getTransform().translate(glm::vec3(0.0, velocity.y * dt, 0.0));

    optEntity = engine->getEntityBoxByWorldPos({0.0, this->getTransform().getPosition().y, 0.0});
    if (optEntity.has_value())
        collide(optEntity.value(), glm::vec3(0.0, velocity.y, 0.0), dt);

    this->getTransform().translate(glm::vec3(0.0, 0.0, velocity.z * dt));

    optEntity = engine->getEntityBoxByWorldPos({0.0, 0.0, this->getTransform().getPosition().z});
    if (optEntity.has_value())
        collide(optEntity.value(), glm::vec3(0.0, 0.0, velocity.z), dt);


    this->box.updateBox(this->getTransform().getPosition());
    this->camera.Position = this->getTransform().getPosition();

    this->velocity.x *= 0.95; //slows down
    this->velocity.z *= 0.95;
}

void Player::processInput(GLFWwindow *window) {

    glm::vec3 rotAngles = glm::eulerAngles(this->getTransform().getRotation());

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->acceleration.x += -glm::cos(glm::radians(rotAngles.y + 90));
        this->acceleration.z += -glm::sin(glm::radians(rotAngles.y + 90));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->acceleration.x += glm::cos(glm::radians(rotAngles.y + 90));
        this->acceleration.z += glm::sin(glm::radians(rotAngles.y + 90));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->acceleration.x += -glm::cos(glm::radians(rotAngles.y));
        this->acceleration.z += -glm::sin(glm::radians(rotAngles.y));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->acceleration.x += glm::cos(glm::radians(rotAngles.y));
        this->acceleration.z += glm::sin(glm::radians(rotAngles.y));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->jump();
    }
}

void Player::collide(const BoundingBox &entityBox, glm::vec3 vel, float dt) {

    if (vel.y > 0) {
        float yDisp = (this->getTransform().getPosition().y + this->box.dimensions.y) - entityBox.position.y;
        this->getTransform().translate(glm::vec3(0.0f, -yDisp, 0.0f));
        this->velocity.y = 0;
    } else if (vel.y < 0) {
        float yDisp = (entityBox.position.y + entityBox.dimensions.y) - this->getTransform().getPosition().y;
        this->getTransform().translate(glm::vec3(0.0f, yDisp, 0.0f));
        this->velocity.y = 0;
        this->onGround = true;
    }

    if (vel.x > 0) {
        float xDisp = (this->getTransform().getPosition().x + this->box.dimensions.x) - entityBox.position.x;
        this->getTransform().translate(glm::vec3(-xDisp, 0.0f, 0.0f));
        this->velocity.x = 0;
    } else if (vel.x < 0) {
        float xDisp = (entityBox.position.x + entityBox.dimensions.x) - this->getTransform().getPosition().x;
        this->getTransform().translate(glm::vec3(xDisp, 0.0f, 0.0f));
        this->velocity.x = 0;
    }

    if (vel.z > 0) {
        float zDisp = (this->getTransform().getPosition().z + this->box.dimensions.z) - entityBox.position.z;
        this->getTransform().translate(glm::vec3(0.0f, 0.0f, -zDisp));
        this->velocity.z = 0;
    } else if (vel.z < 0) {
        float zDisp = (entityBox.position.z + entityBox.dimensions.z) - this->getTransform().getPosition().z;
        this->getTransform().translate(glm::vec3(0.0f, 0.0f, zDisp));
        this->velocity.z = 0;
    }
}
