//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <iostream>
#include "../libs/tiny_obj_loader.h"
#include "mesh.h"

//todo what if multiple meshes per obj file?
/** Loads and reads data from an .obj file
 *
 * @param path the path to the .obj file
 * @return a pair of the mesh name and set of vertices
 */
inline std::pair<std::string, std::vector<Vertex>> loadOBJ(const char *path) {
    std::string meshName;
    std::vector<Vertex> verticesOut;

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

    for (unsigned int i = 0; i < shapes.size(); i++) {
        for (auto index : shapes.at(i).mesh.indices) {
            auto vertInd = index.vertex_index;
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
    return std::make_pair(meshName, std::move(verticesOut));
}

/** Generates a Mesh from a .obj file;
 *
 * @param filePath the path to the .obj file
 */
inline Mesh makeMeshFromFile(const std::string &filePath) {
    std::pair<std::string, std::vector<Vertex>> meshData = loadOBJ(filePath.c_str());
    Mesh outMesh;

    outMesh.meshName = meshData.first;
    outMesh.vertices = meshData.second;

    return std::move(outMesh);
}
