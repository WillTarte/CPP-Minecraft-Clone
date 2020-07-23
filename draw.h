//
// Created by Willi on 7/5/2020.
//

#ifndef COMP_371_PROJECT_DRAW_H
#define COMP_371_PROJECT_DRAW_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

/// struct that holds light parameters
struct LightParams {
    glm::vec3 lightPos = glm::vec3(0.0f, 30.0f, 0.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
};

struct MVPL {
    const glm::mat4 model;
    const glm::mat4 view;
    const glm::mat4 projection;
    const glm::mat4 lsm;
};

/// Cube class contains necessary data to draw a unit cube
class Cube {
private:
    GLuint vbo{};
public:
    GLuint vao{};
    const unsigned int size = sizeof(vertices) / (2 * sizeof(glm::vec3));

    Cube() {

        // Gen and bind buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        // bind buffers
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Feed data to buffers
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void *) 0);

        // normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void *) (1 * sizeof(glm::vec3)));
    }

    virtual ~Cube() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    /// EBO defining the cube faces
    /*const unsigned short cubeFaces[36] = {
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
    };*/
    /// VBO defining the cube vertices
    /*const glm::vec3 cubeVertices[8] = {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f}
    };*/
    /// vertex position data and normal data
    const glm::vec3 vertices[72] = {
            // back face - cw
            {0.0f,  0.0f,  1.0f}, {0.0f,  0.0f,  1.0f},
            {0.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},
            {1.0f,  0.0f,  1.0f}, {0.0f,  0.0f,  1.0f},
            {1.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},
            {1.0f,  0.0f,  1.0f}, {0.0f,  0.0f,  1.0f},
            {0.0f,  1.0f,  1.0f}, {0.0f,  0.0f,  1.0f},

            // front face - ccw
            {0.0f,  0.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},
            {0.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},
            {1.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},
            {0.0f,  1.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f}, {0.0f,  0.0f,  -1.0f},

            // left side - ccw
            {0.0f,  0.0f,  1.0f}, {-1.0f, 0.0f,  0.0f},
            {0.0f,  0.0f,  0.0f}, {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  1.0f}, {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  0.0f}, {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  1.0f}, {-1.0f, 0.0f,  0.0f},
            {0.0f,  0.0f,  0.0f}, {-1.0f, 0.0f,  0.0f},

            // right side - cw
            {1.0f,  0.0f,  1.0f}, {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f}, {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  0.0f}, {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f}, {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  1.0f}, {1.0f,  0.0f,  0.0f},

            // bottom side - cw
            {0.0f,  0.0f,  0.0f}, {0.0f,  -1.0f, 0.0f},
            {0.0f,  0.0f,  1.0f}, {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  0.0f}, {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  1.0f}, {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  0.0f}, {0.0f,  -1.0f, 0.0f},
            {0.0f,  0.0f,  1.0f}, {0.0f,  -1.0f, 0.0f},

            // top side - ccw
            {0.0f,  1.0f,  0.0f}, {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  0.0f}, {0.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  1.0f}, {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  0.0f}, {0.0f,  1.0f,  0.0f}
    };

};

/// Drawable class is an abstract class for any object that is renderable
class Drawable {
private:
    /// Matrix for transformations when drawing the object(example is object rotating on itself)
    glm::mat4 transform{};
protected:
/// Render Mode
    GLenum renderMode = GL_TRIANGLES;
public:

    Drawable() { transform = glm::mat4(1.0f); }

    /// Transform Getter
    [[nodiscard]] virtual glm::mat4 getTransform() const { return transform; }

    /// Transform Setter
    virtual void setTransform(glm::mat4 newTransform) { transform = newTransform; }

    /// Render Mode setter
    virtual void setRenderMode(GLenum newRenderMode) { renderMode = newRenderMode; }

    /** Draws (renders) the object on the screen.
     *
     * @param mvp The Projection * View * Model matrix
     * @param renderMode The current render mode. Example: GL_TRIANGLES
     */
    virtual void
    draw(MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const = 0;

    /** Draws depth information for shadow rendering
     *
     * @param model the model matrix
     * @param depthShader the shader used to calculate depth information
     */
    virtual void drawShadows(const glm::mat4 &model, Shader &depthShader) const = 0;


    virtual ~Drawable() = default;
};

/// The Ground Grid for the world
class GroundGrid : public Drawable {
private:
    GLuint vao{}, vbo{};
    GLsizei size;
    Shader shader{};
public:
    GroundGrid() {
        this->shader = Shader("resources/shaders/GridVertexShader.glsl", "resources/shaders/GridFragmentShader.glsl");

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

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        shader.use();
        shader.setMat4("model", mvpl.model * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);

        glBindVertexArray(vao);

        glDrawArrays(GL_LINES, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {
        //TODO does this need anything?
    }
};

/// The world Axis
class Axis : public Drawable {
private:
    Shader shader{};
    GLuint vao{}, vbo{};
public:

    Axis() {
        shader = Shader("resources/shaders/AxisVertexShader.glsl", "resources/shaders/AxisFragmentShader.glsl");
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
                        //Zaxis
                        //set to green
                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                        //Yaxis
                        //Set to red
                        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

                };

        //creates a localVBO and then connects that to to the arrowLines

        //binding the buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(arrowLines), arrowLines, GL_STATIC_DRAW);

        //arrowLines are stored in location 0 with three values
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);

        //colors are stored in the location 1 with 4 values
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));

        glBindVertexArray(0);
    };

    ~Axis() override {
        glDeleteProgram(shader.ID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    //draw function takes in the mvp matrix from the current scene and applies them to local shader
    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        //enabling the shader to be used
        shader.use();
        shader.setMat4("model", mvpl.model * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);

        glLineWidth(3.0f);
        //binding array that was created in constructor
        glBindVertexArray(this->vao);

        glDrawArrays(GL_LINES, 0, 6);
        glDrawArrays(GL_LINES, 6, 6);
        glDrawArrays(GL_LINES, 12, 6);
        glLineWidth(1.0f);
        //releasing the vertex array
        glBindVertexArray(0);
    };

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {
        //TODO does this need anything?
    }
};

/// Model for a student
class L8 : public Cube, public Drawable {
public:
    Shader shader{};

    L8() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
    }

    ~L8() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, glm::vec3(43.0f, 0.0f, 49.0f)) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        shader.setVec3("lightColor", lp.lightColor);
        //shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* L ------ */
        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */

        /* 8 ------ */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", transformScaled);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {
        glm::mat4 unitmat4 = glm::mat4(1.0f);

        depthShader.setMat4("model", model * getTransform());

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* L ------ */
        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */

        /* 8 ------ */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", transformScaled);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */
    }
};

/// Model for a student
class H3 : public Cube, public Drawable {
private:
    Shader shader{};

public:

    H3() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
    }

    ~H3() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        glBindVertexArray(vao);
        shader.use();
        shader.setMat4("model", mvpl.model * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        shader.setVec3("lightColor", lp.lightColor);
        //shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f));
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f));

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        //draw 3
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {

        depthShader.setMat4("model", model * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f));
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f));

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        //draw 3
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }
};

/// Model for a student
class P6 : public Cube, public Drawable {
public:
    Shader shader{};

    P6() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
    }

    ~P6() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, glm::vec3(-50.0f, 0.0f, -50.0f)) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        shader.setVec3("lightColor", lp.lightColor);
        //shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* P ------ */
        glm::mat4 transform = glm::translate(unitmat4, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */

        /* 6 ------ */
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));
        shader.setMat4("local_transform", transformScaled);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {
        glm::mat4 unitmat4(1);

        depthShader.setMat4("model", model * getTransform());

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* P ------ */
        glm::mat4 transform = glm::translate(unitmat4, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */

        /* 6 ------ */
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));
        shader.setMat4("local_transform", transformScaled);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }
};

/// Model for a student
class H7 : public Cube, public Drawable {
public:
    Shader shader{};

    H7() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
    }

    ~H7() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        glBindVertexArray(vao);
        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, glm::vec3(-49.5f, 0.1f, 49.0f)) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        shader.setVec3("lightColor", lp.lightColor);
        //shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f)); // 5 cubes stacked in y
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f)); // 2 cubes stacked in x

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        //Draws the number 7
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(5.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(7.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {

        depthShader.setMat4("model", model * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f)); // 5 cubes stacked in y
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f)); // 2 cubes stacked in x

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        //Draws the number 7
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(5.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(7.0f, 2.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawArrays(renderMode, 0, size);
    }
};

/// Model for a student
class A2 : public Cube, public Drawable {
public:
    Shader shader{};

    A2() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
    }

    ~A2() override {
        glDeleteProgram(shader.ID);
    }

    static glm::mat4
    scaleandTranslate(int xTran, int yTran, int zTran, int xScale, int yScale, int zScale, glm::mat4 unitmat4) {


        glm::mat4 output = glm::translate(unitmat4, glm::vec3(xTran, yTran, zTran));
        output = glm::scale(output, glm::vec3(xScale, yScale, zScale));
        return output;

    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, glm::vec3(30.0f, 0.0f, -50.0f)) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        shader.setVec3("lightColor", lp.lightColor);
        //shader.setInt("diffuseTexture", 0);
        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);

        glDrawArrays(renderMode, 0, size);


        glm::mat4 input(1);
        /* A ------ */

        //left side
        shader.setMat4("local_transform", scaleandTranslate(0.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //right side
        shader.setMat4("local_transform", scaleandTranslate(6.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //top
        shader.setMat4("local_transform", scaleandTranslate(1.0, 8.0, 0.0, 6.0, 1.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", scaleandTranslate(2.0, 9.0, 0.0, 4.0, 1.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(2.0, 4.0, 0.0, 4.0, 2.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        glm::mat4 movedTwo = scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4);

        /* 2 ------ */

        //base
        shader.setMat4("local_transform", scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //leftside
        shader.setMat4("local_transform", scaleandTranslate(10, 2, 0, 2, 4, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(12, 4, 0, 2, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //rightside
        shader.setMat4("local_transform", scaleandTranslate(14, 4, 0, 2, 4, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //top
        shader.setMat4("local_transform", scaleandTranslate(10, 8, 0, 6, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);
        /* -------- */
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {

        depthShader.setMat4("model", model * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);

        glDrawArrays(renderMode, 0, size);


        glm::mat4 input(1);
        /* A ------ */

        //left side
        shader.setMat4("local_transform", scaleandTranslate(0.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //right side
        shader.setMat4("local_transform", scaleandTranslate(6.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //top
        shader.setMat4("local_transform", scaleandTranslate(1.0, 8.0, 0.0, 6.0, 1.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", scaleandTranslate(2.0, 9.0, 0.0, 4.0, 1.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(2.0, 4.0, 0.0, 4.0, 2.0, 1.0, unitmat4));
        glDrawArrays(renderMode, 0, size);

        glm::mat4 movedTwo = scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4);

        /* 2 ------ */

        //base
        shader.setMat4("local_transform", scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //leftside
        shader.setMat4("local_transform", scaleandTranslate(10, 2, 0, 2, 4, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(12, 4, 0, 2, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //rightside
        shader.setMat4("local_transform", scaleandTranslate(14, 4, 0, 2, 4, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);

        //top
        shader.setMat4("local_transform", scaleandTranslate(10, 8, 0, 6, 2, 1, unitmat4));
        glDrawArrays(renderMode, 0, size);
    }

};

/// light source
class Light : public Cube, public Drawable {
private:
    Shader shader{};
    LightParams lightParams{};
public:

    Light() {
        this->shader = Shader("resources/shaders/LightVertexShader.glsl", "resources/shaders/LightFragmentShader.glsl");
    }

    explicit Light(LightParams lp) {
        lightParams = lp;
    }

    ~Light() override {
        glDeleteProgram(shader.ID);
    }

    [[nodiscard]] inline LightParams getLightParams() { return lightParams; }

    [[nodiscard]] inline glm::vec3 getLightPos() const { return lightParams.lightPos; }

    [[nodiscard]] inline glm::vec3 getLightColor() const { return lightParams.lightColor; }

    inline void setLightPosition(glm::vec3 newPos) { this->lightParams = {newPos, lightParams.lightColor}; }

    inline void setLightColor(glm::vec3 newColor) { this->lightParams = {lightParams.lightPos, newColor}; }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        glBindVertexArray(vao);
        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, lightParams.lightPos) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);

        glDrawArrays(renderMode, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader) const override {
        //TODO probably nothing here
    }
};


#endif //COMP_371_PROJECT_DRAW_H
