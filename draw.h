//
// Created by Willi on 7/5/2020.
//

#ifndef COMP_371_PROJECT_DRAW_H
#define COMP_371_PROJECT_DRAW_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class L8 {
public:
        GLuint vbo{}, vao{}, ebo{};
    GLsizei size{};
    Shader shader{};
    L8()
    {
        std::vector<glm::vec3> vertices{};
        this->shader = Shader("resources/shaders/L8vertexshader.glsl", "resources/shaders/dynamicshapefs.glsl");

        // cube vertices
        vertices.emplace_back(0.0f, 0.0f, 0.0f); // 0
        vertices.emplace_back(1.0f, 0.0f, 0.0f); // 1
        vertices.emplace_back(1.0f, 1.0f, 0.0f); // 2
        vertices.emplace_back(0.0f, 1.0f, 0.0f); // 3
        vertices.emplace_back(0.0f, 0.0f, 1.0f); // 4
        vertices.emplace_back(1.0f, 0.0f, 1.0f); // 5
        vertices.emplace_back(1.0f, 1.0f, 1.0f); // 6
        vertices.emplace_back(0.0f, 1.0f, 1.0f); // 7

        unsigned short cubeFaces[] = {
                //front
                0, 1, 2,
                2, 3, 0,
                //back
                4, 5, 6,
                6, 7, 4,
                //left
                4, 0, 3,
                3, 7, 4,
                //right
                1, 5, 6,
                6, 2, 1,
                //bottom
                0, 1, 5,
                5, 4, 0,
                //top
                3, 2, 6,
                6, 7, 3
        };
        this->size = sizeof(cubeFaces) / sizeof(unsigned short);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
    }

    ~L8()
    {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4& base_mvp) {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("base_mvp", base_mvp);
        shader.setMat4("transform_m", unitmat4);

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        glm::mat4 transformations = glm::translate(unitmat4, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("transform_m", transformations);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        transformations = glm::translate(unitmat4, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("transform_m", transformations);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        transformations = glm::translate(unitmat4, glm::vec3(0.0f, 2.0f, 0.0f));
        shader.setMat4("transform_m", transformations);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        transformations = glm::translate(unitmat4, glm::vec3(0.0f, 3.0f, 0.0f));
        shader.setMat4("transform_m", transformations);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);
    }
};

class GroundGrid
{
private:
    GLuint vao{}, vbo{};
    GLsizei size;
    Shader shader{};
public:
    GroundGrid()
    {
        this->shader = Shader("resources/shaders/basicvertexshader.glsl", "resources/shaders/GridFragmentShader.glsl");

        std::vector<glm::vec3> vertices;

        for (int i = 0; i <= 100; i++) //draw 100 horizontal and 100 vertical lines - 2 vertices per line
        {
            // horizontal
            vertices.emplace_back(glm::vec3(-50.0f, 0.0f, (float) i - 50.0f));
            vertices.emplace_back(glm::vec3(50.0f, 0.0f, (float) i - 50.0f));

            // vertical
            vertices.emplace_back(glm::vec3((float) i - 50.0f, 0.0f, -50));
            vertices.emplace_back(glm::vec3((float) i - 50.0f, 0.0f, 50.0f));
        }
        this->size = vertices.size();

        // Set up and bind VBO and VAO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
    }

    ~GroundGrid()
    {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4& base_mvp) {
        shader.use();
        shader.setMat4("base_mvp", base_mvp);
        glBindVertexArray(vao);

        glDrawArrays(GL_LINES, 0, size);
    }
};
#endif //COMP_371_PROJECT_DRAW_H
