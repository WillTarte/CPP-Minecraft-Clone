//
// Created by Ewan on 7/3/2020.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"


//Class definition
class Axis
{
public:

    Axis(Shader axisShader);
    void Draw(glm::mat4 model, glm::mat4 view , glm::mat4 projection);
private:
    Shader axisShader;
    GLuint localVAO;
};


Axis::Axis(Shader axisShader) : axisShader(axisShader) {

    //axisShader = Shader("resources/shaders/BasicVertexShader.vs","resources/shaders/BasicFragmentShader.fs");
    const GLfloat arrowLines[] =
            {
                    //read row by row
                    //two arrowLines coresponding to a line
                    //(first three pos)(second 3 are color)

                    //Xaxis
                    //set to blue
                    //main line
                    0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    5.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    //one side of arrow
                    5.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    4.5f, 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
                    //other side of arrow
                    5.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
                    4.5f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f,

                    //Zaxis
                    //set to green
                    0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.5f, 0.0f,  4.5f, 0.0f, 1.0f, 0.0f, 1.0f,
                    0.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                    -0.5f, 0.0f,  4.5f, 0.0f, 1.0f, 0.0f, 1.0f,



                    //Yaxis
                    //Set to red
                    0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    0.5f, 4.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    0.0f, 5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                    -0.5f, 4.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,


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





//draw function takes in the matricxes from the current scene and applied them to local shader
void Axis::Draw(glm::mat4 model, glm::mat4 view , glm::mat4 projection)
{


    //enabling the shader to be used
    axisShader.use();
    axisShader.setMat4("Model", model);
    axisShader.setMat4("View", view);
    axisShader.setMat4("Projection", projection);


    //binding array that was created in constructor
    glBindVertexArray(this->localVAO);

    glDrawArrays(GL_LINES, 0, 6);
    glDrawArrays(GL_LINES, 6, 6);
    glDrawArrays(GL_LINES, 12, 6);


    //releasing the vertex array
    glBindVertexArray(0);
}