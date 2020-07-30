//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Model {
private:
    GLuint vaoID, vboID, iboId;
    GLuint numVertices;
    Texture texture;
    std::string modelName;
public:

    Model(Mesh& mesh, Texture texture1);

    void bindBuffers();

    void draw(Shader& shader);
};
