//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "libs/easylogging++.h"
#include "include/engine.h"

INITIALIZE_EASYLOGGINGPP

void initLogging();

/// Main
int main(int argc, char *argv[]) {

    initLogging();

    auto engine = Engine({});

    LOG(INFO) << "Initializing resource databases.";

    TextureDatabase::init();
    ModelDatabase::init();

    LOG(INFO) << "Primary initialization done.";

    //TODO: Would it be possible to move this to the engine constructor?
    glfwSetCursorPosCallback(engine.getWindow(), [](GLFWwindow *window, double x, double y) {
        auto *engine = static_cast<Engine *>( glfwGetWindowUserPointer(window));
        engine->mouseCallbackFunc(window, x, y);
    });

    engine.runLoop();
}

/// Sets up some logging attributes from config file and flags
void initLogging() {

    // Load logging config from file
    el::Configurations conf("easylogging.config");

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);

    //Reconfigure all loggers
    el::Loggers::reconfigureAllLoggers(conf);
}