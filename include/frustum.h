//
// Created by Willi on 8/10/2020.
// inspired by https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/Source/Maths/Frustum.h
#pragma once

#include <array>
#include <glm/glm.hpp>
#include "entity.h"

struct Plane {

    Plane() = default;

    [[nodiscard]] inline float distanceToPoint(const glm::vec3 &point) const {
        return glm::dot(point, normal) + distanceToOrigin;
    }

    glm::vec3 normal;
    float distanceToOrigin;
};

class ViewFrustum {
public:

    void update(const glm::mat4 &proj, const glm::mat4 &view);

    [[nodiscard]] bool isBoxInFrustum(glm::vec3 position, BoundingBox box) const;

private:
    std::array<Plane, 6> planes;
};
