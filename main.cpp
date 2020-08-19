//#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "libs/easylogging++.h"
#include "include/engine.h"

INITIALIZE_EASYLOGGINGPP

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

Config cliConfig() {
    Config conf{};
    std::cout << "Window size? (default " << DEFAULT_WINDOW_WIDTH << "x" << DEFAULT_WINDOW_HEIGHT << "): ";
    std::string winSize;
    std::getline(std::cin, winSize, '\n');
    if (!winSize.empty()) {
        int seperator = winSize.find('x');
        conf.windowWidth = stoi(winSize.substr(0,seperator));
        conf.windowHeight = stoi(winSize.substr(seperator));
    }

    std::cout << "World size? (s/M/l): ";
    std::string worldSize;
    std::getline(std::cin, worldSize);
    if (!worldSize.empty()) {
        switch(worldSize[0]) {
            case 's': conf.worldSize = SMALL_WORLD;
            case 'm': conf.worldSize = MEDIUM_WORLD;
            case 'l': conf.worldSize = LARGE_WORLD;
            default: {
                conf.worldSize = MEDIUM_WORLD;
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
        conf.fov = stoi(fov);
    }

    return conf;
}