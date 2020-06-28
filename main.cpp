#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("GLEW test");
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        // Problem with init of glew
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    return 0;
}



