//
// Created by Willi on 8/10/2020.
// inspired by https://github.com/Hopson97/MineCraft-One-Week-Challenge/blob/master/Source/Maths/Frustum.h
#pragma once

#include <array>
#include <glm/glm.hpp>
#include "entity.h"

struct Plane {

    Plane() = default;

    inline float distanceToPoint(const glm::vec3 &point) const {
        return glm::dot(point, normal) + distanceToOrigin;
    }

    glm::vec3 normal;
    float distanceToOrigin;
};

/// A view frustum is composed of 6 planes and dictates if a given bounding box is within the view.
class ViewFrustum {
public:

    /// updates the view frustum based on the player's view and the projection matrix
    void update(const glm::mat4 &proj, const glm::mat4 &view);

    /// Checks if the given box at the given position is within the view frustum.
    bool isBoxInFrustum(glm::vec3 position, BoundingBox box) const;

private:
    std::array<Plane, 6> planes;
};
