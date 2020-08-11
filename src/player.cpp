//
// Created by Ewan on 8/8/2020.
//

#include "../include/player.h"


//.23 makes it approximatly 3 tall
Player::Player() : Entity(ModelType::STEVE, BlockID::PLAYER, Transform({10, 1, 10}, {0.23, 0.23, 0.23}, {0, 0, 0})),
                   camera(glm::vec3(11.0f, 4.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH) {


}


//TODO need to also move the hitbox while moving

//walk needs to update location of model and update the camera
void Player::walk(Camera_Movement direction, float deltaTime) {

    //not sure what global movement speed is iv replaced it with 0.5 for now
    float velocity = 2.5 * deltaTime;

    if (direction == FORWARD)
        this->getTransform().translate(glm::vec3(camera.Front.x, 0.0f, camera.Front.z) * velocity);
    if (direction == BACKWARD)
        this->getTransform().translate(glm::vec3(camera.Front.x, 0.0f, camera.Front.z) * -velocity);
    if (direction == LEFT)
        this->getTransform().translate(glm::vec3(camera.Right.x, 0.0f, camera.Right.z) * -velocity);
    if (direction == RIGHT)
        this->getTransform().translate(glm::vec3(camera.Right.x, 0.0f, camera.Right.z) * velocity);

    this->updateHitbox();



    camera.changePosition(glm::vec3(this->maxX-0.75f,  this->maxY, this->maxZ-0.75f));

}


void Player::jump(float deltaTime) {
    this->getTransform().translate(glm::vec3(0.0f, 2.0f, 0.0));
    camera.ProcessKeyboard(UP, deltaTime);

    this->updateHitbox();
}

void Player::gravity(float deltaTime) {
    this->getTransform().translate(glm::vec3(0.0f, -0.05f, 0.0));
    camera.ProcessKeyboard(DOWN, deltaTime);

    this->updateHitbox();
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

    ////TODO needs to be a better way of attaching the camera to the box
    //this->getTransform().rotate(glm::vec3(0.0f,1.0f,0.0f),glm::radians(xoffset));
   // this->updateHitbox();




  //  glm::vec3 noRotation = glm::vec3(this->maxX-0.75,  this->maxY, this->maxZ-0.75);
   // currentYaw += (xoffset *0.1);
   //    float yawRadians = glm::radians(currentYaw);
    //    glm::vec3 noRotation = glm::vec3(this->maxX-0.75,  this->maxY, this->maxZ-0.75);
    //    float rotatedX = cos(currentYaw) * (point.x - center.x) - sin(currentYaw) * (point.y-center.y) + center.x;
    //    float rotatedZ = sin(currentYaw) * (point.x - center.x) + cos(currentYaw) * (point.y - center.y) + center.y;
    //return new createjs.Point(rotatedX,rotatedY);
    //need to get a current angle and then apply that to the change


    //glm::vec3 rotation = glm::normalize(this->getTransform().getRotation() * noRotation)
   // camera.changePosition(noRotation);
}

void Player::horizontalCollision(Direction direction, float deltaTime) {

    if(direction == Direction::POSX)
        this->getTransform().translate(glm::vec3(-0.25f, 0.0f, 0.0));
    if(direction == Direction::NEGX)
        this->getTransform().translate(glm::vec3(0.25f, 0.0f, 0.0));
    if(direction == Direction::POSZ)
        this->getTransform().translate(glm::vec3(0.0f, 0.0f, -0.25));
    if(direction == Direction::NEGZ)
        this->getTransform().translate(glm::vec3(0.0f, 0.0f, 0.25));



    //TODO just attach camera to location of player
    Camera_Movement cameraDirection;

    if (direction == Direction::POSX)
        cameraDirection = Camera_Movement::FORWARD;
    if (direction == Direction::NEGX)
        cameraDirection = Camera_Movement::BACKWARD;
    if (direction == Direction::POSZ)
        cameraDirection = Camera_Movement::LEFT;
    if (direction == Direction::NEGZ)
        cameraDirection = Camera_Movement::RIGHT;


    //camera.ProcessKeyboard(cameraDirection, deltaTime);

    //TODO update camera on collision
    this->updateHitbox();

    camera.changePosition(glm::vec3(this->maxX-0.75,  this->maxY, this->maxZ-0.75));
}

