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

    L8() {
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
                0, 1, 3, //ccw
                2, 3, 1,
                //back
                4, 7, 5, //cw
                6, 5, 7,
                //left
                4, 0, 7, //ccw
                3, 7, 0,
                //right
                5, 6, 1, //cw
                2, 1, 6,
                //bottom
                0, 4, 1, //cw
                5, 1, 4,
                //top
                3, 2, 7, //ccw
                6, 7, 2
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

    ~L8() {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4 &baseMvp) const {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("base_mvp", baseMvp);

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        /* L ------ */
        shader.setMat4("transform_m", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */

        /* 8 ------ */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("transform_m", transformScaled);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform_m", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */
    }
};

class GroundGrid {
private:
    GLuint vao{}, vbo{};
    GLsizei size;
    Shader shader{};
public:
    GroundGrid() {
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

    ~GroundGrid() {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4 &baseMvp) {
        shader.use();
        shader.setMat4("base_mvp", baseMvp);
        glBindVertexArray(vao);

        glDrawArrays(GL_LINES, 0, size);
    }
};


class H3
{

private:
    GLuint vbo, vao, ebo;
    GLsizei size;
    Shader shader;
    std::vector<glm::vec3> vertices;
    const GLushort indices[36] = {
        //front
                   0, 1, 3, 
                   2, 3, 1,
                   //back
                   4, 7, 5, 
                   6, 5, 7,
                   //left
                   4, 0, 7, 
                   3, 7, 0,
                   //right
                   5, 6, 1, 
                   2, 1, 6,
                   //bottom
                   0, 4, 1, 
                   5, 1, 4,
                   //top
                   3, 2, 7, 
                   6, 7, 2
    };

public:

    H3() {


        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        vertices.emplace_back(glm::vec3(-0.5, -0.5, 0.5));
        vertices.emplace_back(glm::vec3(0.5, -0.5, 0.5));
        vertices.emplace_back(glm::vec3(0.5, 0.5, 0.5));
        vertices.emplace_back(glm::vec3(-0.5, 0.5, 0.5));
        vertices.emplace_back(glm::vec3(-0.5, -0.5, -0.5));
        vertices.emplace_back(glm::vec3(0.5, -0.5, -0.5));
        vertices.emplace_back(glm::vec3(0.5, 0.5, -0.5));
        vertices.emplace_back(glm::vec3(-0.5, 0.5, -0.5));


        this->size = sizeof(indices) / sizeof(unsigned short);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
    }





    //unit cube
    void DrawCube(glm::mat4 base, glm::mat4 transform)
    {
        shader.use();
        shader.setMat4("base_mvp", base);
        shader.setMat4("transform", transform);

        glBindVertexArray(vao);
        
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_SHORT, nullptr);


    }
    void DrawH3(glm::mat4 base) {

        //draw H
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.5f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

        //draw 3
        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 3.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 1.5f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.25f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);


        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.75f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.5f, 0.0f));
        rotationMatrix = glm::mat4(1.0f);
        DrawCube(base, translationMatrix * rotationMatrix * scalingMatrix);

    }
};

#endif //COMP_371_PROJECT_DRAW_H
