//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <iostream>
#include "../libs/tiny_obj_loader.h"
#include "mesh.h"

//todo what if multiple meshes?
inline std::string loadOBJ(const char * path, std::vector<Vertex>& verticesOut) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path);

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    for (int i = 0; i < attrib.vertices.size() - 2; i += 3) {
        std::cout << attrib.vertices.at(i) << " ";
        std::cout << attrib.vertices.at(i + 1) << " ";
        std::cout << attrib.vertices.at(i + 2) << std::endl << std::endl;
    }

    verticesOut.clear();
    std::string meshName;
    for (unsigned int i = 0; i < shapes.size(); i++) {
        for (auto index : shapes.at(i).mesh.indices) {
            auto vertInd = index.vertex_index;
            //std::cout << vertInd << std::endl;
            auto normInd = index.normal_index;
            auto texInd = index.texcoord_index;
            auto vertex = Vertex{};
            vertex.position = {attrib.vertices.at(3 * vertInd), attrib.vertices.at(3 * vertInd + 1),
                               attrib.vertices.at(3 * vertInd + 2)};
            vertex.normal = {attrib.normals.at(3 * normInd), attrib.normals.at(3 * normInd + 1),
                             attrib.normals.at(3 * normInd + 2)};
            vertex.textureCoordinate = {attrib.texcoords.at(2 * texInd), attrib.texcoords.at(2 * texInd + 1)};
            verticesOut.emplace_back(vertex);
            meshName = shapes.at(i).name;
        }
    }
    return meshName;
}

inline void makeMeshFromFile(const std::string& filePath, Mesh& outMesh) {
    std::vector<Vertex> vertices;
    std::string meshName = loadOBJ(filePath.c_str(), vertices);

    outMesh.meshName = meshName;
    outMesh.vertices = std::move(vertices);
}
