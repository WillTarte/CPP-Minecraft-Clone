//
// Created by Willi on 7/5/2020.
//

#ifndef COMP_371_PROJECT_DRAW_H
#define COMP_371_PROJECT_DRAW_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Drawable {
public:
    virtual void draw(const glm::mat4 &mvp, GLenum renderMode) const = 0;

    virtual ~Drawable() = default;
};

class L8 : Drawable {
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

    ~L8() override {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4 &mvp, GLenum renderMode) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("base_mvp", mvp);

        glBindVertexArray(vao);

        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        /* L ------ */
        shader.setMat4("transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */

        /* 8 ------ */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("transform", transformScaled);
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */
    }
};

class GroundGrid : Drawable {
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

    ~GroundGrid() override {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void draw(const glm::mat4 &mvp, GLenum renderMode) const override {
        shader.use();
        shader.setMat4("base_mvp", mvp);
        glBindVertexArray(vao);

        glDrawArrays(GL_LINES, 0, size);
    }
};

//Class definition
class Axis : Drawable {
private:
    Shader shader;
    GLuint localVAO{};
public:

    explicit Axis(Shader axisShader) : shader(axisShader) {
        //axisShader = Shader("resources/shaders/BasicVertexShader.glsl","resources/shaders/BasicFragmentShader.glsl");
        const GLfloat arrowLines[] =
                {
                        //read row by row
                        //two arrowLines coresponding to a line
                        //(first three pos)(second 3 are color)

                        //Xaxis
                        //set to blue
                        //main line
                        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                        5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                        //one side of arrow
                        5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                        4.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
                        //other side of arrow
                        5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                        4.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

                        //Zaxis
                        //set to green
                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.5f, 0.0f, 4.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        -0.5f, 0.0f, 4.5f, 0.0f, 1.0f, 0.0f, 1.0f,

                        //Yaxis
                        //Set to red
                        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.5f, 4.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        -0.5f, 4.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,


                };

        //creates a localVBO and then connects that to to the arrowLines


        //binding the buffers
        GLuint localVBO;
        glGenVertexArrays(1, &this->localVAO);
        glGenBuffers(1, &localVBO);
        glBindVertexArray(this->localVAO);
        glBindBuffer(GL_ARRAY_BUFFER, localVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(arrowLines), arrowLines, GL_STATIC_DRAW);

        //arrowLines are stored in location 0 with three values
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);

        //colors are stored in the location 1 with 4 values
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindVertexArray(0);
    };

    ~Axis() {
        glDeleteProgram(shader.ID);
        glDeleteVertexArrays(1, &localVAO);
    }

    //draw function takes in the mvp matrix from the current scene and applies them to local shader
    void draw(const glm::mat4 &mvp, GLenum renderMode) const override {
        //enabling the shader to be used
        shader.use();
        shader.setMat4("base_mvp", mvp);

        //binding array that was created in constructor
        glBindVertexArray(this->localVAO);

        glDrawArrays(GL_LINES, 0, 6);
        glDrawArrays(GL_LINES, 6, 6);
        glDrawArrays(GL_LINES, 12, 6);

        //releasing the vertex array
        glBindVertexArray(0);
    };
};

class H3 : Drawable {

private:
    GLuint vbo{}, vao{}, ebo{};
    GLsizei size;
    Shader shader{};
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

    //unit cube
    void drawCube(glm::mat4 base, glm::mat4 transform, GLenum renderMode) const {
        shader.use();
        shader.setMat4("base_mvp", base);
        shader.setMat4("transform", transform);

        glBindVertexArray(vao);

        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
    }

public:

    H3() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        std::vector<glm::vec3> vertices;
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

    void draw(const glm::mat4 &mvp, GLenum renderMode) const override {

        //draw H
        glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.5f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.5f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

        //draw 3
        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 3.0f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.5f, 1.5f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.25f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);


        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 2.75f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.5f, 1.0f));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.5f, 0.0f));
        drawCube(mvp, translationMatrix * scalingMatrix, renderMode);

    }
};

#endif //COMP_371_PROJECT_DRAW_H
