//
// Created by Ewan on 8/8/2020.
//

#ifndef COMP_371_PROJECT_PLAYER_H
#define COMP_371_PROJECT_PLAYER_H

#endif //COMP_371_PROJECT_PLAYER_

#include "entity.h"
#include "camera.h"


//damn opengl does it in 2d need to figure out 3D
enum Direction {
    POSX,
    NEGX,
    POSZ,
    NEGZ
};
class Player : public Entity {

public:
    Player();

    glm::mat4 getPlayerView() { return camera.getViewMatrix(); }

    void walk(Camera_Movement direction, float deltaTime);

    void look(double xpos, double ypos);

    void jump(float deltaTime);

    void gravity(float deltaTime);

    void horizontalCollision(Direction direction, float deltaTime);

private:
    Camera camera;


};