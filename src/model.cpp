//
// Created by Willi on 7/30/2020.
//

#include <glm/gtx/matrix_decompose.hpp>
#include "../include/model.h"
#include "../include/objloader.h"

Model::Model(Mesh &mesh) {

    this->modelName = mesh.meshName + "Model";
    this->numVertices = mesh.vertices.size();

    glGenVertexArrays(1, &vaoID);
    glGenBuffers(1, &vboID);
    glGenBuffers(1, &iboId);

    glBindVertexArray(vaoID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);


    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) sizeof(glm::vec3));

    // texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) (sizeof(glm::vec3) * 2));

    glBindVertexArray(0);
}

void Model::bindBuffers() {
    glBindVertexArray(vaoID);
}

void Model::draw(Shader &shader) {

    bindBuffers();
    shader.setMat4("model", glm::mat4(1.0f));
    shader.setInt("texture1", 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
