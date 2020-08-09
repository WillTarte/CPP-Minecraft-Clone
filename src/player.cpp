//
// Created by Ewan on 8/8/2020.
//

#include "../include/player.h"

Player::Player() : Entity(ModelType::STEVE, BlockID::PLAYER, Transform({10, 2, 10}, {0.5, 0.5, 0.5}, {0, 0, 0})),
                   camera(glm::vec3(12.0f, 3.0f, 8.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH) {


}


//walk needs to update location of model and update the camera
void Player::walk(Camera_Movement direction, float deltaTime) {

    //not sure what global movement speed is iv replaced it with 0.5 for now
    float velocity = 0.75 * deltaTime;
    camera.ProcessKeyboard(direction, deltaTime);
    if (direction == FORWARD)
        this->getTransform().translate(glm::vec3(camera.Front.x, 0.0f, camera.Front.z) * velocity);
    if (direction == BACKWARD)
        this->getTransform().translate(glm::vec3(camera.Front.x, 0.0f, camera.Front.z) * -velocity);
    if (direction == LEFT)
        this->getTransform().translate(glm::vec3(camera.Right.x, 0.0f, camera.Right.z) * -velocity);
    if (direction == RIGHT)
        this->getTransform().translate(glm::vec3(camera.Right.x, 0.0f, camera.Right.z) * velocity);

}


//TODO if player looks the model should rotate
void Player::look(double xpos, double ypos) {


    static bool firstMouse = true;
    static float lastX = 1024.0f/2.0f;
    static float lastY = 768.0f/2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}