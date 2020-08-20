//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>
#include "libs/irrKLang/include/irrKlang.h"
#include "libs/easylogging++.h"
#include "include/engine.h"

INITIALIZE_EASYLOGGINGPP

#ifdef __unix__
namespace fs = std::filesystem;
#endif

#if defined __unix__ || _MSC_VER >= 1914
namespace fs = std::filesystem;
#else
namespace fs = std::filesystem;
#endif

void initLogging();

/// Main
int main(int argc, char *argv[]) {

    initLogging();


    irrklang::ISoundEngine *soundEngine = irrklang::createIrrKlangDevice();

    if (!soundEngine) {
        LOG(ERROR) << "Could not create sound device";
        return -1;
    }

    soundEngine->play2D((fs::current_path().string() + "./resources/sounds/calm.mp3").c_str(), true);

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