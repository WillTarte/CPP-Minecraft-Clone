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
        acceleration.y -= 30 * dt;
    }

    if (currentChunk.has_value()) {
        collide((*currentChunk), VelocityComponent::Y);
        collide((*currentChunk), VelocityComponent::X);
        collide((*currentChunk), VelocityComponent::Z);
        checkOnGround((*currentChunk));
    } else {
        // Out of bounds!
        LOG(INFO) << "Player is out of bounds at " << this->getTransform().getPosition().x << " "
                  << this->getTransform().getPosition().y << " " << this->getTransform().getPosition().z;
    }

    this->transform.translate(velocity);
    this->camera.Position = this->getTransform().getPosition() + glm::vec3(0.5f, 1.5f, 0.5f);
}


void Player::processInput(GLFWwindow *window, float dt,Engine *engine ) {


    float speed = 10;
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


    //code for deleting a block
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 1024 / (float) 768,0.1f, 100.0f);


        //found by trial and error 512 doesnt seem to be in the middle this is closer
        float mouse_x = 600.0;
        float mouse_y = 384.0;
        glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 1024 , 768);
        glm::vec3 ray_start = glm::unProject(glm::vec3(mouse_x, mouse_y, 0.0f), this->getPlayerView(), projection, viewport);
        glm::vec3 ray_end = glm::unProject(glm::vec3(mouse_x, mouse_y, 1.0f), this->getPlayerView(), projection, viewport);



        //may need to normalize!
        //ray_wor = glm::vec3(round(ray_wor.x-10), round(ray_wor.y - 25.5), round(ray_wor.z - 10));


           engine->removeEntity(ray_end,ray_start);
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){


        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 1024 / (float) 768,0.1f, 100.0f);


        float mouse_x = 512.0f;
        float mouse_y = 384.0f;
        glm::vec4 viewport = glm::vec4(0.0f, 0.0f, 1024.0f , 768.0f);
        glm::vec3 ray_start = glm::unProject(glm::vec3(mouse_x, mouse_y, 0.0f), this->getPlayerView(), projection, viewport);
        glm::vec3 ray_end = glm::unProject(glm::vec3(mouse_x, mouse_y, 1.0f), this->getPlayerView(), projection, viewport);



        //may need to normalize!
        //ray_wor = glm::vec3(round(ray_wor.x-10), round(ray_wor.y - 25.5), round(ray_wor.z - 10));


        engine->placeBlock(ray_end,ray_start);
    }
}

void Player::collide(const std::shared_ptr<Chunk> &currentChunk, VelocityComponent comp) {

    std::optional<Entity *> optEnt;

    if (comp == VelocityComponent::Y) {
        optEnt = (*currentChunk).getEntityByBoxCollision(
                this->getTransform().getPosition() + glm::vec3(0, velocity.y, 0),
                this->box);
        if (optEnt.has_value() && checkCollisionY(*this, *(*optEnt))) {
            if (velocity.y > 0.0f) {
                this->transform.position.y = glm::ceil((*optEnt)->getTransform().getPosition().y - 1.0f) - 0.01f;
                velocity.y = 0.0f;

            } else if (velocity.y < 0.0f) {
                std::cout << "\nBONK\n";
                this->transform.position.y = glm::floor((*optEnt)->getTransform().getPosition().y + 1.0f) + 0.01f;
                onGround = true;
                velocity.y = 0.0f;
            }
        }
    }

    if (comp == VelocityComponent::X) {
        optEnt = (*currentChunk).getEntityByBoxCollision(
                this->getTransform().getPosition() + glm::vec3(velocity.x, 0, 0),
                this->box);
        if (optEnt.has_value() && checkCollisionX(*this, *(*optEnt))) {
            velocity.x = 0.0f;
        }
    }

    if (comp == VelocityComponent::Z) {
        optEnt = (*currentChunk).getEntityByBoxCollision(
                this->getTransform().getPosition() + glm::vec3(0, 0, velocity.z),
                this->box);
        if (optEnt.has_value() && checkCollisionZ(*this, *(*optEnt))) {
            velocity.z = 0.0f;
        }
    }
}

void Player::checkOnGround(const std::shared_ptr<Chunk> &currentChunk) {
    std::optional<Entity *> optEntity = (*currentChunk).getEntityByBoxCollision(
            this->getTransform().getPosition() + glm::vec3(0.0f, -0.2f, 0.0f), this->box);

    onGround = optEntity.has_value();
}
