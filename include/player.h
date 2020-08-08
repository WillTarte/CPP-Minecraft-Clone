//
// Created by Ewan on 8/8/2020.
//

#ifndef COMP_371_PROJECT_PLAYER_H
#define COMP_371_PROJECT_PLAYER_H

#endif //COMP_371_PROJECT_PLAYER_H


#include "entity.h"
#include "camera.h"

class Player : public Entity{

public:
    Player();
    glm::mat4 getPlayerView() { return camera.getViewMatrix(); }
    void walk(Camera_Movement direction, float deltaTime);
    void look(double xpos, double ypos);

private:
    Camera camera;
    float posX, posY, posZ;

};