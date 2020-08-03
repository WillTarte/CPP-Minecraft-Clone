//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <vector>
#include <glm/glm.hpp>

/// A vertex has a position, a normal vector and texture coordinates
struct Vertex {
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 textureCoordinate{};
};

/// A Mesh has a name, and a set of vertices
struct Mesh {
    std::string meshName{};
    std::vector<Vertex> vertices{};
};
