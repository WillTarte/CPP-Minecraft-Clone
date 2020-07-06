//
// Created by Ewan on 7/3/2020.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"

class Axis
{
public:

    Axis();
    void Draw(Shader &shader);
private:
    GLuint localVAO;
};


Axis::Axis() {


    const GLfloat arrowLines[] =
            {
                    //read row by row
                    //two arrowLines corespodning to a line
                    //(first three pos)(last 4 are color)

                    //Xaxis
                    //main line
                    0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    //one side of arrow
                    3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    2.5f, 0.0f,  0.2f, 1.0f, 0.0f, 0.0f, 1.0f,
                    //other side of arrow
                    3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    2.5f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f, 1.0f,

                    //pattern repeats for respective axis
                    //Yaxis
                    0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 3.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 3.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.2f, 2.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 3.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    -0.2f, 2.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

                    //Zaxis
                    0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    0.0f, 0.0f,  3.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    0.0f, 0.0f,  3.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    0.2f, 0.0f,  2.5f, 0.0f, 0.0f, 1.0f, 1.0f,
                    0.0f, 0.0f,  3.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    -0.2f, 0.0f,  2.5f, 0.0f, 0.0f, 1.0f, 1.0f,
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);

    //colors are stored in the location 1 with 4 values
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
}





//axis takes in a reference to a shader and then uses it
void Axis::Draw(Shader &shader)
{

    shader.use();
    shader.setMat4("model", glm::mat4(1.0f));

    glBindVertexArray(this->localVAO);
    //xaxis
    glDrawArrays(GL_LINES, 0, 6);

    //yaxis
    glDrawArrays(GL_LINES, 6, 6);

    //zaxis
    glDrawArrays(GL_LINES, 12, 6);

    glBindVertexArray(0);
}