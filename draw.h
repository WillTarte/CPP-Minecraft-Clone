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

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

static const Material METALLIC = {glm::vec3(0.25f, 0.25f, 0.25f), glm::vec3(0.4f, 0.4f, 0.4f),
                                  glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f};
static const Material WOOD = {glm::vec3(0.2125f, 0.2125f, 0.2125f), glm::vec3(0.714f, 0.4284f, 0.18144f),
                              glm::vec3(0.393548f, 0.271906f, 0.166721f), 0.2f};

/// Cube class contains necessary data to draw a unit cube
class Cube {
private:
    GLuint vbo{};
public:
    GLuint vao{};
    const unsigned int size = sizeof(vertices) / (2 * sizeof(glm::vec3));

    Cube() {

        // Gen and bind buffers
        unsigned int vboUV;
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &vboUV);

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

        glBindBuffer(GL_ARRAY_BUFFER, vboUV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

        // texture attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *) 0);
    }

    virtual ~Cube() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    const glm::vec2 textureCoords[36] = {
            // back face
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 1.0f},

            // front face
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {1.0f, 0.0f},

            //left side
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {1.0f, 0.0f},

            // right side
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {1.0f, 0.0f},

            // bottom side
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f},

            // top side
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f},

    };

    /// vertex position data, normal data
    const glm::vec3 vertices[72] = {
            // back face - cw
            {0.0f,  0.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},
            {0.0f,  1.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},
            {1.0f,  0.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},
            {1.0f,  1.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},
            {1.0f,  0.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},
            {0.0f,  1.0f,  1.0f},
            {0.0f,  0.0f,  1.0f},

            // front face - ccw
            {0.0f,  0.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},
            {0.0f,  1.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},
            {1.0f,  1.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},
            {0.0f,  1.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  0.0f,  -1.0f},

            // left side - ccw
            {0.0f,  0.0f,  1.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  0.0f,  0.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  1.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  0.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  1.0f,  1.0f},
            {-1.0f, 0.0f,  0.0f},
            {0.0f,  0.0f,  0.0f},
            {-1.0f, 0.0f,  0.0f},

            // right side - cw
            {1.0f,  0.0f,  1.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  1.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  0.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  0.0f,  0.0f},
            {1.0f,  1.0f,  1.0f},
            {1.0f,  0.0f,  0.0f},

            // bottom side - cw
            {0.0f,  0.0f,  0.0f},
            {0.0f,  -1.0f, 0.0f},
            {0.0f,  0.0f,  1.0f},
            {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  1.0f},
            {0.0f,  -1.0f, 0.0f},
            {1.0f,  0.0f,  0.0f},
            {0.0f,  -1.0f, 0.0f},
            {0.0f,  0.0f,  1.0f},
            {0.0f,  -1.0f, 0.0f},

            // top side - ccw
            {0.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  1.0f},
            {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  1.0f},
            {0.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  1.0f},
            {0.0f,  1.0f,  0.0f},
            {1.0f,  1.0f,  0.0f},
            {0.0f,  1.0f,  0.0f}
    };

};

/// Drawable class is an abstract class for any object that is renderable
class Drawable {
private:
    glm::vec3 position{};
protected:
/// Render Mode
    GLenum renderMode = GL_TRIANGLES;
/// Matrix for transformations when drawing the object(example is object rotating on itself)
    glm::mat4 transform{};
public:

    Drawable() {
        transform = glm::mat4(1.0f);
        position = glm::vec3(0, 0, 0);
    }

    /// Transform Getter
    [[nodiscard]] virtual glm::mat4 getTransform() const { return transform; }

    /// Transform Setter
    virtual void setTransform(glm::mat4 newTransform) { transform = newTransform; }

    [[nodiscard]] virtual glm::vec3 getPosition() const { return position; }

    virtual void setPosition(glm::vec3 pos) { position = pos; }


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
    virtual void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const = 0;


    virtual ~Drawable() = default;
};

/// Class that holds necessary data to draw a sphere
class Sphere : public Drawable {
private:
    Shader shader{};
    GLuint vbo{}, ebo{};
    std::vector<float> vertices{};
    std::vector<float> normals{};
    std::vector<float> texCoords{};
    std::vector<int> indices{};
    std::vector<float> interleavedVertices{};

public:
    GLuint vao{};

    explicit Sphere(const glm::mat4 transform) {


        //getting the size displacement
        this->transform = transform;



        //need to create interleaved ones
        this->shader = Shader("resources/shaders/SphereVertexShader.glsl", "resources/shaders/SphereFragmentShader.glsl",
                              "resources/textures/bubble.jpg");

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

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void *) 0);           //for verts

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void *) (sizeof(float) * 3));   //for normals

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void *) (sizeof(float) * 6));     //for the textures
    }

    ~Sphere() override {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteVertexArrays(1, &vao);
    }


    void
    draw(MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //took out the local transform
        shader.use();
        shader.setMat4("model", mvpl.model * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);

        glBindVertexArray(vao);


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,                    // primitive type
                       indices.size(),          // # of indices
                       GL_UNSIGNED_INT,                 // data type
                       (void *) nullptr);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // draw a sphere with VBO
        glDrawElements(GL_TRIANGLES,                    // primitive type
                       indices.size(),          // # of indices
                       GL_UNSIGNED_INT,                 // data type
                       (void *) nullptr);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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


                //std::cout << sectorAngle << "\n";
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

/// The Ground Grid for the world
class GroundGrid : public Drawable {
private:
    GLuint vao{}, vbo{}, vboUV{};
    GLsizei size;
    Shader shader{};
    Material material{};
public:
    GroundGrid() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/GridFragmentShader.glsl",
                              "resources/textures/chessboard_floor.jpg");
        this->size = sizeof(vertices) / sizeof(glm::vec3);
        this->material = METALLIC;

        // Set up and bind VBO and VAO
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &vboUV);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 2 * sizeof(glm::vec3), nullptr);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void *) sizeof(glm::vec3));

        glBindBuffer(GL_ARRAY_BUFFER, vboUV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    }

    ~GroundGrid() override {
        glDeleteProgram(shader.ID);
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);
        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLES, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        //TODO does this need anything?

        /*depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, size);*/
    }

    static constexpr glm::vec3 vertices[12] = {
            {-50.0f, 0.0f, -50.0f},
            {0.0f,   1.0f, 0.0f},
            {50.0f,  0.0f, -50.0f},
            {0.0f,   1.0f, 0.0f},
            {50.0f,  0.0f, 50.0f},
            {0.0f,   1.0f, 0.0f},
            {50.0f,  0.0f, 50.0f},
            {0.0f,   1.0f, 0.0f},
            {-50.0f, 0.0f, 50.0f},
            {0.0f,   1.0f, 0.0f},
            {-50.0f, 0.0f, -50.0f},
            {0.0f,   1.0f, 0.0f}
    };

    static constexpr glm::vec2 texCoords[6] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f}
    };
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) nullptr);

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        // Empty on purpose
    }
};

/// Model for a student
class ModelL : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    ModelL() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/crate1_wood.png");
        this->material = WOOD;
    }

    ~ModelL() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* L ------ */
        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        glm::mat4 unitmat4 = glm::mat4(1.0f);

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* L ------ */
        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(3.0f, 1.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);

        shader.setMat4("local_transform", glm::scale(unitmat4, glm::vec3(1.0f, 5.0f, 1.0f)));
        glDrawArrays(renderMode, 0, size);
        /* -------- */
    }
};

/// Model for a student
class Model8 : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    Model8() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/metallic_surface.png");
        this->material = METALLIC;
    }

    ~Model8() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        glm::mat4 unitmat4 = glm::mat4(1.0f);

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

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
class Model3 : public Cube, public Drawable {
private:
    Shader shader{};
    Material material{};
public:

    Model3() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/metallic_surface.png");
        this->material = METALLIC;
    }

    ~Model3() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f));
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f));

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
class ModelH : public Cube, public Drawable {
private:
    Shader shader{};
    Material material{};
public:

    ModelH() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl", "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/crate1_wood.png");
        this->material = WOOD;
    }

    ~ModelH() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

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
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

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
    }
};

/// Model for a student
class ModelP : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    ModelP() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl",
                              "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/crate1_wood.png");
        this->material = WOOD;
    }

    ~ModelP() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

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
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        glm::mat4 unitmat4(1);

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

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
    }
};

/// Model for a student
class Model6 : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    Model6() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl",
                              "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/metallic_surface.png");
        this->material = METALLIC;
    }

    ~Model6() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {
        glm::mat4 unitmat4(1);

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glm::mat4 transform = glm::translate(unitmat4, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 transformScaled = glm::scale(transform, glm::vec3(3.0f, 1.0f, 1.0f));

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        glm::mat4 unitmat4(1);

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        /* 6 ------ */
        glm::mat4 transform = glm::translate(unitmat4, glm::vec3(0.0f, 0.0f, 0.0f));
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
    }
};

class Model7 : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    Model7() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl",
                              "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/metallic_surface.png");
        this->material = METALLIC;
    }

    ~Model7() override {
        glDeleteProgram(shader.ID);
    }

    void
    draw(const MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos) const override {

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);


        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f)); // 5 cubes stacked in y
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f)); // 2 cubes stacked in x

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat = glm::mat4(1.0f);
        glm::mat4 y5 = glm::scale(unitmat, glm::vec3(1.0f, 5.0f, 1.0f)); // 5 cubes stacked in y
        glm::mat4 x2 = glm::scale(unitmat, glm::vec3(2.0f, 1.0f, 1.0f)); // 2 cubes stacked in x

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
class Model2 : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    Model2() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl",
                              "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/metallic_surface.png");
        this->material = METALLIC;
    }

    ~Model2() override {
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
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);
        glm::mat4 input(1);

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

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);

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

/// Model for a student
class ModelA : public Cube, public Drawable {
public:
    Shader shader{};
    Material material{};

    ModelA() {
        this->shader = Shader("resources/shaders/ModelVertexShader.glsl",
                              "resources/shaders/ModelFragmentShader.glsl",
                              "resources/textures/crate1_wood.png");
        this->material = WOOD;
    }

    ~ModelA() override {
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
        shader.setMat4("model", glm::translate(mvpl.model, getPosition()) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);
        shader.setMat4("lightSpaceMatrix", mvpl.lsm);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightPos", lp.lightPos);
        // shader.setVec3("light.ambient", lp.lightColor * 0.5f);
        //shader.setVec3("light.diffuse", lp.lightColor * 0.2f);
        // shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // material properties
        // shader.setVec3("material.ambient", material.ambient);
        // shader.setVec3("material.diffuse", material.diffuse);
        // shader.setVec3("material.specular", material.specular);
        // shader.setFloat("material.shininess", material.shininess);

        shader.setInt("shadowMap", 1);

        glBindVertexArray(vao);

        glm::mat4 unitmat4(1);
        glm::mat4 input(1);

        glDrawArrays(renderMode, 0, size);

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

    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {

        depthShader.setMat4("lightSpaceMatrix", glm::translate(lsm, getPosition()) * transform);
        //depthShader.setMat4("model", glm::translate(model, getPosition()) * getTransform());

        glBindVertexArray(vao);

        glDrawArrays(renderMode, 0, size);

        /* A ------ */
        glm::mat4 unitmat4 = glm::mat4(1.0f);
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
    }
};


/// light source
class Light : public Cube, public Drawable {
private:
    Shader shader{};
    LightParams lightParams{};
public:

    Light() {
        this->shader = Shader("resources/shaders/LightVertexShader.glsl",
                              "resources/shaders/LightFragmentShader.glsl");
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

        shader.use();
        shader.setMat4("model", glm::translate(mvpl.model, lightParams.lightPos) * getTransform());
        shader.setMat4("view", mvpl.view);
        shader.setMat4("projection", mvpl.projection);

        glBindVertexArray(vao);
        glDrawArrays(renderMode, 0, size);
    }

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm) const override {
        //TODO probably nothing here
    }
};

#endif //COMP_371_PROJECT_DRAW_H