//
// Created by Ewan on 7/3/2020.
//



#include <vector>
#include <tuple>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

class Axis
{
public:

    Axis();
    ~Axis();

    void Draw(Shader &shader);

private:
    GLuint VAO;
};