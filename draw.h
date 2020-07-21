//
// Created by Willi on 7/5/2020.
//

#ifndef COMP_371_PROJECT_DRAW_H
#define COMP_371_PROJECT_DRAW_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"


/// Cube class contains necessary data to draw a unit cube
class Sphere {
private:
    GLuint vbo{}, ebo{};
    std::vector<float> vertices{};
    std::vector<float> normals{};
    std::vector<float> texCoords{};
    std::vector<int> indices{};
    std::vector<float> interleavedVertices{};


public:
    GLuint vao{};

    Sphere() {
        //need to create interleaved ones

        //runs the function to generate verts
        createVertices();
        buildInterleavedVertices();
        createIndices();

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);           // for vertex data
        glBufferData(GL_ARRAY_BUFFER,                   // target
                     interleavedVertices.size() * sizeof(float), // data size, # of bytes
                     interleavedVertices.data(),   // ptr to vertex data
                     GL_STATIC_DRAW);                   // usage


        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);   // for index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
                     indices.size() * sizeof(int),             // data size, # of bytes
                     indices.data(),               // ptr to index data
                     GL_STATIC_DRAW);                   // usage


        // set attrib arrays with stride and offset
        int stride = 32;     // should be 32 bytes
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void *) 0);           //for verts
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void *) (sizeof(float) * 3));   //for normals
        glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void *) (sizeof(float) * 6));     //for the textures

        // activate attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }

    virtual ~Sphere() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }


    void draw(const glm::mat4 &mvp) const {

        Shader shader = Shader("resources/shaders/SphereVertexShader.glsl",
                               "resources/shaders/SphereFragmentShader.glsl");

        //took out the local transform
        shader.use();
        shader.setMat4("base_mvp", mvp);

        glBindVertexArray(vao);


        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        // draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,                    // primitive type
                       indices.size(),          // # of indices
                       GL_UNSIGNED_INT,                 // data type
                       (void *) 0);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    }

    void createVertices() {

        //just for now setting it toi a defualt setup
        float PI = 3.1415926535;
        float radius = 3.0f;
        int sectorCount = 36;
        int stackCount = 18;
        bool smooth = true;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi


                std::cout << sectorAngle << "\n";
                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

               // if(sectorAngle <= PI) {           //cuts it in half
                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    // normalized vertex normal (nx, ny, nz)
                    nx = x * lengthInv;
                    ny = y * lengthInv;
                    nz = z * lengthInv;
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);

                    // vertex tex coord (s, t) range between [0, 1]
                    s = (float) j / sectorCount;
                    t = (float) i / stackCount;
                    texCoords.push_back(s);
                    texCoords.push_back(t);
               // }
            }
        }
    }

    void createIndices() {

        float PI = 3.1415926535;
        float radius = 1.0f;
        int sectorCount = 36;
        int stackCount = 18;

        // generate CCW index list of sphere triangles

        int k1, k2;
        for (int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0) {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1)) {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

    }


    void buildInterleavedVertices() {

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2) {
            interleavedVertices.push_back(vertices[i]);
            interleavedVertices.push_back(vertices[i + 1]);
            interleavedVertices.push_back(vertices[i + 2]);

            interleavedVertices.push_back(normals[i]);
            interleavedVertices.push_back(normals[i + 1]);
            interleavedVertices.push_back(normals[i + 2]);

            interleavedVertices.push_back(texCoords[j]);
            interleavedVertices.push_back(texCoords[j + 1]);
        }
    }


};

/// Cube class contains necessary data to draw a unit cube
class Cube {
private:
    GLuint vbo{}, ebo{};
public:
    GLuint vao{};
    const unsigned int size = sizeof(cubeFaces) / sizeof(unsigned short);

    Cube() {

        // Gen and bind buffers
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Feed data to buffers
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeFaces), cubeFaces, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0);
    }

    virtual ~Cube() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }

    /// EBO defining the cube faces
    const unsigned short cubeFaces[36] = {
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
    /// VBO defining the cube vertices
    const glm::vec3 cubeVertices[8] = {
            {0.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 0.0f, 1.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f},
            {0.0f, 1.0f, 1.0f}
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
    draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const = 0;

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
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/GridFragmentShader.glsl");

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

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {
        shader.use();
        shader.setMat4("model", model * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(vao);

        glDrawArrays(GL_LINES, 0, size);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) 0);

        //colors are stored in the location 1 with 4 values
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*) (3 * sizeof(float)));

        glBindVertexArray(0);
    };

    ~Axis() override {
        glDeleteProgram(shader.ID);
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    //draw function takes in the mvp matrix from the current scene and applies them to local shader
    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {
        //enabling the shader to be used
        shader.use();
        shader.setMat4("model", model * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

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
};

/// Model for a student
class L8 : public Cube, public Drawable {
public:
    Shader shader{};

    L8() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        this->setTransform(
                glm::translate(this->getTransform(), glm::vec3(43.0f, 0.0f, 49.0f)));
    }

    ~L8() override {
        glDeleteProgram(shader.ID);
    }

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f)) * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(vao);

        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        /* L ------ */
        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */

        /* 8 ------ */
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", transformScaled);
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
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

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {

        glBindVertexArray(vao);
        shader.use();

        shader.setMat4("model", glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f)) * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f));
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f));

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //draw 3
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
    }
};

/// Model for a student
class P6 : public Cube, public Drawable {
public:
    Shader shader{};

    P6() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        this->setTransform(
                glm::translate(this->getTransform(), glm::vec3(-50.0f, 0.0f, -50.0f)));
    }

    ~P6() override {
        glDeleteProgram(shader.ID);
    }

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f)) * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(vao);

        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        /* P ------ */
        glm::mat4 transform = glm::translate(unitmat4, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */

        /* 6 ------ */
        transform = glm::translate(unitmat4, glm::vec3(4.0f, 0.0f, 0.0f));
        transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));
        shader.setMat4("local_transform", transformScaled);
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transformScaled, glm::vec3(0.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(0.0f, 3.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(transform, glm::vec3(2.0f, 1.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
    }
};

/// Model for a student
class H7 : public Cube, public Drawable {
public:
    Shader shader{};

    H7() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        this->setTransform(
                glm::translate(this->getTransform(), glm::vec3(-49.5f, 0.1f, 49.0f)));
    }

    ~H7() override {
        glDeleteProgram(shader.ID);
    }

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {

        glBindVertexArray(vao);
        shader.use();
        shader.setMat4("model", glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f)) * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f)); // 5 cubes stacked in y
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f)); // 2 cubes stacked in x

        //draw H
        shader.setMat4("local_transform", y5);
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(2.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(1.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //Draws the number 7
        shader.setMat4("local_transform", glm::translate(y5, glm::vec3(6.0f, 0.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(5.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(unitmat, glm::vec3(7.0f, 2.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", glm::translate(x2, glm::vec3(2.0f, 4.0f, 0.0f)));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
    }
};

/// Model for a student
class A2 : public Cube, public Drawable {
public:
    Shader shader{};

    A2() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl");
        this->setTransform(
                glm::translate(this->getTransform(), glm::vec3(30.0f, 0.0f, -50.0f)));
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

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const override {

        shader.use();
        shader.setMat4("model", glm::translate(model,glm::vec3(0.0f, 0.0f, 0.0f)) * getTransform());
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);

        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);


        glm::mat4 input(1);
        /* A ------ */

        //left side
        shader.setMat4("local_transform", scaleandTranslate(0.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //right side
        shader.setMat4("local_transform", scaleandTranslate(6.0, 0.0, 0.0, 2.0, 8.0, 1.0, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //top
        shader.setMat4("local_transform", scaleandTranslate(1.0, 8.0, 0.0, 6.0, 1.0, 1.0, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        shader.setMat4("local_transform", scaleandTranslate(2.0, 9.0, 0.0, 4.0, 1.0, 1.0, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(2.0, 4.0, 0.0, 4.0, 2.0, 1.0, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        glm::mat4 movedTwo = scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4);

        /* 2 ------ */

        //base
        shader.setMat4("local_transform", scaleandTranslate(10, 0, 0, 6, 2, 1, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //leftside
        shader.setMat4("local_transform", scaleandTranslate(10, 2, 0, 2, 4, 1, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //middle
        shader.setMat4("local_transform", scaleandTranslate(12, 4, 0, 2, 2, 1, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //rightside
        shader.setMat4("local_transform", scaleandTranslate(14, 4, 0, 2, 4, 1, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);

        //top
        shader.setMat4("local_transform", scaleandTranslate(10, 8, 0, 6, 2, 1, unitmat4));
        glDrawElements(renderMode, size, GL_UNSIGNED_SHORT, nullptr);
        /* -------- */
    }

};


#endif //COMP_371_PROJECT_DRAW_H
