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

    verticesOut.clear();
    std::string meshName;
    for (unsigned int i = 0; i < shapes.size(); i++) {
        for (auto index : shapes.at(i).mesh.indices) {
            auto vertInd = index.vertex_index;
            auto normInd = index.normal_index;
            auto texInd = index.texcoord_index;
            auto vertex = Vertex{};
            vertex.position = {attrib.vertices.at(vertInd), attrib.vertices.at(vertInd+1), attrib.vertices.at(vertInd+2)};
            vertex.normal = {attrib.normals.at(normInd), attrib.normals.at(normInd + 1), attrib.normals.at(normInd + 2)};
            vertex.textureCoordinate = {attrib.texcoords.at(texInd), attrib.texcoords.at(texInd + 1)};
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
