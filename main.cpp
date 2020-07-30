//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "include/engine.h"

/// Main
int main(int argc, char *argv[]) {


    auto engine = Engine({});

    //TODO: Would it be possible to move this to the engine constructor?
    glfwSetCursorPosCallback( engine.getWindow(), []( GLFWwindow* window, double x, double y )
    {
        Engine* engine = static_cast<Engine*>( glfwGetWindowUserPointer( window ) );
        engine->mouseCallbackFunc(x, y);
    } );

    engine.runLoop();
}