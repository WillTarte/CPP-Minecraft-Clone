//
// Created by Ewan on 7/3/2020.
//

#include <glm/glm.hpp>
#include "axis.h"


const GLfloat vertices[] =
        {

                //read row by row
                //two successive rows correspond to a line
                //first set of three entries are location of vertice
                //second set of three is for color

                //last entry is to indicate it is a vertice

                //Xaxis
                0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                2.5f, 0.0f,  0.2f, 1.0f, 0.0f, 0.0f, 1.0f,
                3.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                2.5f, 0.0f, -0.2f, 1.0f, 0.0f, 0.0f, 1.0f,



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


Axis::Axis() {


    //creates a VBO and then connects that to to the vertices
    //VAO
    GLuint VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(3*sizeof(float)));

    glBindVertexArray(0);
}

Axis::~Axis()
{
}

void Axis::Draw(Shader &shader)
{

    shader.use();
    shader.setInt("self_color", 1);
    shader.setMat4("model", glm::mat4(1.0f));

    glBindVertexArray(this->VAO);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, 3*2);
    glDrawArrays(GL_LINES, 6, 3*2);
    glDrawArrays(GL_LINES, 12, 3*2);
    glLineWidth(0.5f);

    glBindVertexArray(0);
}


