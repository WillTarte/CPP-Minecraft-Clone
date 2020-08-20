//
// Created by Willi on 8/18/2020.
// inspired by https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/Source/Maths/Frustum.h

#include "../include/frustum.h"

enum Planes {
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

bool ViewFrustum::isBoxInFrustum(glm::vec3 position, BoundingBox box) const {

    for (auto &plane : planes) {

        glm::vec3 vn = position, vp = position;

        if (plane.normal.x > 0)
            vp.x += box.dimensions.x;
        if (plane.normal.y > 0)
            vp.y += box.dimensions.y;
        if (plane.normal.z > 0)
            vp.z += box.dimensions.z;

        if (plane.normal.x < 0)
            vn.x += box.dimensions.x;
        if (plane.normal.y < 0)
            vn.y += box.dimensions.y;
        if (plane.normal.z < 0)
            vn.z += box.dimensions.z;

        if (plane.distanceToPoint(vp) < 0) {
            return false;
        } else if (plane.distanceToPoint(vn) < 0) {
            return true;
        }
    }
    return true;
}

void ViewFrustum::update(const glm::mat4 &proj, const glm::mat4 &view) {

    glm::mat4 pv = proj * view;
    // left
    planes[Planes::LEFT].normal.x = pv[0][3] + pv[0][0];
    planes[Planes::LEFT].normal.y = pv[1][3] + pv[1][0];
    planes[Planes::LEFT].normal.z = pv[2][3] + pv[2][0];
    planes[Planes::LEFT].distanceToOrigin = pv[3][3] + pv[3][0];

    // right
    planes[Planes::RIGHT].normal.x = pv[0][3] - pv[0][0];
    planes[Planes::RIGHT].normal.y = pv[1][3] - pv[1][0];
    planes[Planes::RIGHT].normal.z = pv[2][3] - pv[2][0];
    planes[Planes::RIGHT].distanceToOrigin = pv[3][3] - pv[3][0];

    // bottom
    planes[Planes::BOTTOM].normal.x = pv[0][3] + pv[0][1];
    planes[Planes::BOTTOM].normal.y = pv[1][3] + pv[1][1];
    planes[Planes::BOTTOM].normal.z = pv[2][3] + pv[2][1];
    planes[Planes::BOTTOM].distanceToOrigin = pv[3][3] + pv[3][1];

    // top
    planes[Planes::TOP].normal.x = pv[0][3] - pv[0][1];
    planes[Planes::TOP].normal.y = pv[1][3] - pv[1][1];
    planes[Planes::TOP].normal.z = pv[2][3] - pv[2][1];
    planes[Planes::TOP].distanceToOrigin = pv[3][3] - pv[3][1];

    // near
    planes[Planes::FRONT].normal.x = pv[0][3] + pv[0][2];
    planes[Planes::FRONT].normal.y = pv[1][3] + pv[1][2];
    planes[Planes::FRONT].normal.z = pv[2][3] + pv[2][2];
    planes[Planes::FRONT].distanceToOrigin = pv[3][3] + pv[3][2];

    // far
    planes[Planes::BACK].normal.x = pv[0][3] - pv[0][2];
    planes[Planes::BACK].normal.y = pv[1][3] - pv[1][2];
    planes[Planes::BACK].normal.z = pv[2][3] - pv[2][2];
    planes[Planes::BACK].distanceToOrigin = pv[3][3] - pv[3][2];

    for (auto &plane : planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distanceToOrigin /= length;
    }

}
