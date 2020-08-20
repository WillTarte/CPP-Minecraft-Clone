//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include "objloader.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"

/// A model is an object that is renderable by OpenGL
class Model {
private:
    GLuint vaoID{}, vboID{}, iboId{};
    GLuint numVertices;
    std::string modelName;

public:
    Model(Mesh &mesh);

    /** Draws this model with the given shader.
     *
     * @param shader the shader
     */
    void draw() const;

    inline std::string getModelName() const { return this->modelName; }

    /// Binds this model's buffers for rendering
    void bindBuffers() const;

    void destroyBuffers();
};
