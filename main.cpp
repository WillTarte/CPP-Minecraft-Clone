//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>
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
Config cliConfig();

/// Main
int main(int argc, char *argv[]) {

    initLogging();

    auto config = cliConfig();
    auto engine = Engine(config);

    LOG(INFO) << "Initializing resource databases.";

    TextureDatabase::init();
    ModelDatabase::init();
    SoundDatabase::init();

    LOG(INFO) << "Primary initialization done.";

    engine.init();

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

Config cliConfig() {
    Config conf{};
    std::cout << "Window size? (default " << EngineConstants::DEFAULT_WINDOW_WIDTH << "x"
              << EngineConstants::DEFAULT_WINDOW_HEIGHT << "): ";
    std::string winSize;
    std::getline(std::cin, winSize, '\n');
    if (!winSize.empty()) {
        int seperator = winSize.find('x');
        conf.windowWidth = stoi(winSize.substr(0, seperator));
        conf.windowHeight = stoi(winSize.substr(seperator + 1, winSize.size()));
    }

    std::cout << "World size? (s/m/l): ";
    std::string worldSize;
    std::getline(std::cin, worldSize);
    if (!worldSize.empty()) {
        switch(worldSize[0]) {
            case 's':
                conf.worldSize = EngineConstants::SMALL_WORLD;
                break;
            case 'm':
                conf.worldSize = EngineConstants::MEDIUM_WORLD;
                break;
            case 'l':
                conf.worldSize = EngineConstants::LARGE_WORLD;
                break;
            default: {
                conf.worldSize = EngineConstants::MEDIUM_WORLD;
            }
        }
    }

    std::cout << "World seed (random): ";
    std::string seed;
    std::getline(std::cin, seed);
    if (!seed.empty()) {
        conf.seed = stoi(seed);
    }

    std::cout << "Field of view (45): ";
    std::string fov;
    std::getline(std::cin, fov);
    if (!fov.empty()) {
        conf.fov = stof(fov);
    }

    std::cout.flush();
    return conf;
}