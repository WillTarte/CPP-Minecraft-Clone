//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position{};
    glm::vec3 normal{};
    glm::vec2 textureCoordinate{};
};

struct Mesh {
    std::string meshName{};
    std::vector<Vertex> vertices{};
};
