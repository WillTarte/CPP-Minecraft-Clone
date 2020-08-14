//
// Created by Willi on 7/30/2020.
//

#include <vector>
#include <glm/ext.hpp>
#include "../libs/easylogging++.h"
#include "../libs/FastNoise.h"
#include "../include/engine.h"


Engine::Engine(Config config) {

    LOG(INFO) << "Initializing Engine ...";
    //do some processing based on config
    LOG(DEBUG) << "Config {windowHeight=" << config.windowHeight << ", windowWidth=" << config.windowWidth << "}";
    this->config = config;
    this->windowWidth = config.windowWidth;
    this->windowHeight = config.windowHeight;

    LOG(INFO) << "Initializing GLFW ...";

    // Initialize GLFW
    glfwInit();
    this->
            window = glfwCreateWindow(config.windowWidth, config.windowHeight, "COMP 371 Final Project", nullptr,
                                      nullptr);
    glfwSetWindowUserPointer(window, this);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    LOG(INFO) << "Successfully initialized GLFW version " << glfwGetVersionString();

    LOG(DEBUG) << "Setting up GLFW callbacks.";
    auto keyCallback = [](GLFWwindow *windowParam, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(windowParam, true);
    };
    auto framebufferSizeCallback = [](GLFWwindow *windowParam, int width, int height) {
        glViewport(0, 0, width, height);
    };
    auto errorCallback = [](int error, const char *description) { fputs(description, stderr); };

    // Set current context and callbacks
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetErrorCallback(errorCallback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    LOG(INFO) << "Initializing GLEW ...";

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        //Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return;
    }
    int glMajor;
    int glMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glMinor);
    LOG(INFO) << "Using OpenGL version " << glMajor << "." << glMinor << ".";
    LOG(INFO) << "Successfully initialized GLEW version " << glewGetString(GLEW_VERSION) << ".";

    LOG(INFO) << "Generating World ... ";
    this->worldInfo = WorldInfo{};
    this->chunkManager = std::make_unique<ChunkManager>(this->worldInfo);

    LOG(INFO) << "Inserting Entities into the World ...";
    generateWorld();

    LOG(INFO) << "Generated world using seed " << worldInfo.getSeed() << ".";

    LOG(INFO) << "Engine is primed and ready.";
}

void Engine::runLoop() {
    double currentFrame = 0.0f;
    double deltaTime = 0.0f;    // time between current frame and last frame
    double lastFrame = 0.0f;

    //TODO this should not be here
    Shader basicShader = Shader("../resources/shaders/ModelVertexShader.glsl",
                                "../resources/shaders/ModelFragmentShader.glsl");
    basicShader.use();
    // ***********

    glfwSwapInterval(1);



    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        player->processInput(this->window);
        player->update(this, deltaTime);
        // --------------------

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(config.fov), (float) windowWidth / (float) windowHeight,
                                                0.1f, 100.0f);

        // rendering stuff here
        // --------------------
        basicShader.setMat4("view", player->getPlayerView());
        basicShader.setMat4("projection", projection);

        auto chunksToDraw = chunkManager->getSurroundingChunksByXZ(
                {player->getTransform().getPosition().x, player->getTransform().getPosition().z});
        LOG(INFO) << "Rendering " << chunksToDraw.size() << " Chunks.";
        for (auto chunk : chunksToDraw) {
            (*chunk).renderChunk(basicShader);
        }

        player->draw(basicShader);
        // --------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

GLFWwindow *Engine::getWindow() const {
    return window;
}

//TODO: Is there some way to add randomness to trees?
void Engine::addTree(int x, int y, int z) {

    auto chunk = this->chunkManager->getChunkbyXZ({x, z});

    for (int h = 0; h < 4; h++) {
        (*chunk)->addEntity(
                Entity(ModelType::CUBE, BlockID::OAK_LOG, Transform({x, y + h + 1, z}, {1, 1, 1}, {0, 0, 0})));
    }

    for (int l = -2; l < 3; l++) {
        for (int w = -2; w < 3; w++) {
            (*chunk)->addEntity(
                    Entity(ModelType::CUBE, BlockID::OAK_LEAVES,
                           Transform({x + l, y + 4, z + w}, {1, 1, 1}, {0, 0, 0})));
            (*chunk)->addEntity(
                    Entity(ModelType::CUBE, BlockID::OAK_LEAVES,
                           Transform({x + l, y + 5, z + w}, {1, 1, 1}, {0, 0, 0})));
        }
    }

    for (int l = -1; l < 2; l++) {
        for (int w = -1; w < 2; w++) {
            (*chunk)->addEntity(
                    Entity(ModelType::CUBE, BlockID::OAK_LEAVES,
                           Transform({x + l, y + 6, z + w}, {1, 1, 1}, {0, 0, 0})));
        }
    }

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::OAK_LEAVES, Transform({x, y + 7, z}, {1, 1, 1}, {0, 0, 0})));
}

void Engine::generateWorld() {
    auto noiseGen = FastNoise(worldInfo.getSeed());
    noiseGen.SetNoiseType(FastNoise::Simplex);

    for (unsigned int x = 0; x < this->worldInfo.getWidth(); x++) {
        for (unsigned int z = 0; z < this->worldInfo.getLength(); z++) {
            float tempHeight = noiseGen.GetNoise(x, 0, z) + 1;
            int height = round((tempHeight * 10) + 1) + 10;

            auto chunk = this->chunkManager->getChunkbyXZ({x, z});

            /// Commenting this out for now because it increases the number of blocks rendered from ~16k to ~246k
            /// Which means that the performance is dramatically worse. Once culling is done, this is good to include. 
            // Fill the world with stone under the block
//            for (int y = 0; y < this->world.height; y++) {
//                // If right below grass, fill with dirt
//                if (y > height - 4) {
//                    this->addEntity(
//                            Entity(ModelType::CUBE, BlockID::DIRT, Transform({x, y, z}, {1, 1, 1}, {0, 0, 0})));
//                }
//                // If not at bottom layer, fill with stone
//                else if (y != 0) {
//                    this->addEntity(
//                            Entity(ModelType::CUBE, BlockID::STONE, Transform({x, y, z}, {1, 1, 1}, {0, 0, 0})));
//                }
//                // Bottom layer is bedrock
//                else {
//                    this->addEntity(
//                            Entity(ModelType::CUBE, BlockID::BEDROCK, Transform({x, y, z}, {1, 1, 1}, {0, 0, 0})));
//                }
//            }

            if (height < 14) {
                height = 13;
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::STONE, Transform({x, height - 1, z}, {1, 1, 1}, {0, 0, 0})));
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::WATER, Transform({x, height, z}, {1, 1, 1}, {0, 0, 0})));
            }
            else {
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::DIRT_GRASS, Transform({x, height, z}, {1, 1, 1}, {0, 0, 0})));

                int tree = (x * height * z) ^worldInfo.getSeed();
                if (tree % 61 == 0) {
                    addTree(x, height, z);
                }
            }
        }
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Engine::mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos) {
    player->look(windowParam, xpos, ypos);
}