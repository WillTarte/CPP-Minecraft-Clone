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
    LOG(INFO) << "Config {windowHeight=" << config.windowHeight << ", windowWidth=" << config.windowWidth << ", fov="
              << config.fov << "}";
    this->config = config;
    this->worldInfo = WorldInfo(config);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

}

void Engine::init() {

    LOG(INFO) << "Generating World with size " << this->worldInfo.getWidth() << "x" << this->worldInfo.getLength();
    this->chunkManager = std::make_unique<ChunkManager>(this->worldInfo);

    LOG(INFO) << "Inserting Entities into the World ...";
    generateWorld();

    LOG(INFO) << "Number of entities: " << this->chunkManager->getNumberOfEntities();
    LOG(INFO) << "Number of Chunks: " << this->chunkManager->getNumberOfChunks();

    LOG(INFO) << "Generated world using seed " << worldInfo.getSeed() << ".";
    this->player = std::make_unique<Player>(
            glm::vec3(this->worldInfo.getWidth() / 2, 32.0f, this->worldInfo.getLength() / 2));

    LOG(INFO) << "Creating Skybox";
    skybox = std::make_unique<Skybox>(ModelType::SKYBOX, BlockID::SKYBOX);
    skybox->getTransform().setPosition(
            glm::vec3((player->getTransform().getPosition().x - EngineConstants::CHUNK_WIDTH * 2), 10,
                      (player->getTransform().getPosition().z - EngineConstants::CHUNK_LENGTH * 2)));
    skybox->getTransform().scaleBy(glm::vec3(EngineConstants::CHUNK_WIDTH * 4, EngineConstants::CHUNK_HEIGHT * 4,
                                             EngineConstants::CHUNK_LENGTH * 4));

    LOG(INFO) << "Engine is primed and ready.";
}

void Engine::runLoop() {

    //TODO should this be here?
    Shader basicShader = Shader((fs::current_path().string() + "/resources/shaders/ModelVertexShader.glsl").c_str(),
                                (fs::current_path().string() + "/resources/shaders/ModelFragmentShader.glsl").c_str());
    basicShader.use();

    ViewFrustum frustum = ViewFrustum();
    // ***********

    glfwSwapInterval(1);

    double t = 0.0f;
    double dt = 1.0f / 30.0f;
    double currentTime = glfwGetTime();

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        nbFrames++;
        if (newTime - lastTime >= 1.0f) {
            std::cout << "\nms/frame " << 1000.0 / double(nbFrames) << "\n";
            nbFrames = 0;
            lastTime += 1.0;
        }

        while (frameTime > 0.0f) {
            double deltaTime = frameTime < dt ? frameTime : dt;
            frameTime -= deltaTime;
            t += deltaTime;
        }

        player->processInput(this);
        player->update(this, static_cast<float>(dt));
        // --------------------

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(config.fov), (float) windowWidth / (float) windowHeight,
                                                0.1f, 96.0f);

        // rendering stuff here
        // --------------------
        basicShader.setMat4("view", player->getPlayerView());
        basicShader.setMat4("projection", projection);

        auto chunksToDraw = chunkManager->getSurroundingChunksByXZ(
                {player->getTransform().getPosition().x, player->getTransform().getPosition().z});
        LOG(DEBUG) << "Rendering " << chunksToDraw.size() << " Chunks.";
        for (const auto &chunk : chunksToDraw) {
            chunk->renderChunk(basicShader, frustum);
        }

        player->draw(basicShader);


        skybox->getTransform().setPosition(
                glm::vec3((player->getTransform().getPosition().x - EngineConstants::CHUNK_WIDTH * 2), -1,
                          (player->getTransform().getPosition().z - EngineConstants::CHUNK_LENGTH * 2)));
        glDisable(GL_CULL_FACE);
        skybox->draw(basicShader);
        glEnable(GL_CULL_FACE);

        // --------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Engine::mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos) {
    player->look(windowParam, xpos, ypos);
}

//TODO: Is there some way to add randomness to trees?
void Engine::addTree(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

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
            int height = static_cast<int>(round((tempHeight * 10) + 1)) + 10;

            auto chunk = this->chunkManager->getChunkByXZ({x, z});

            if (height < 14) {
                height = 13;
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::STONE, Transform({x, height - 1, z}, {1, 1, 1}, {0, 0, 0})));
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::WATER, Transform({x, height, z}, {1, 1, 1}, {0, 0, 0})));

                for (int i = height - 2; i >= 0; i--) {
                    if (i > 0) {
                        (*chunk)->addEntity(
                                Entity(ModelType::CUBE, BlockID::STONE, Transform({x, i, z}, {1, 1, 1}, {0, 0, 0})));
                    } else {
                        (*chunk)->addEntity(
                                Entity(ModelType::CUBE, BlockID::BEDROCK, Transform({x, i, z}, {1, 1, 1}, {0, 0, 0})));
                    }
                }
            } else {
                (*chunk)->addEntity(
                        Entity(ModelType::CUBE, BlockID::DIRT_GRASS, Transform({x, height, z}, {1, 1, 1}, {0, 0, 0})));

                int tree = (x * height * z) ^worldInfo.getSeed();
                if (tree % 61 == 0) {
                    addTree(x, height, z);
                }

                for (int i = height - 1; i >= 0; i--) {
                    if (i >= 8) {
                        (*chunk)->addEntity(
                                Entity(ModelType::CUBE, BlockID::DIRT, Transform({x, i, z}, {1, 1, 1}, {0, 0, 0})));
                    } else if (i > 0) {
                        (*chunk)->addEntity(
                                Entity(ModelType::CUBE, BlockID::STONE, Transform({x, i, z}, {1, 1, 1}, {0, 0, 0})));
                    } else {
                        (*chunk)->addEntity(
                                Entity(ModelType::CUBE, BlockID::BEDROCK, Transform({x, i, z}, {1, 1, 1}, {0, 0, 0})));
                    }
                }
            }
        }
    }

    for (unsigned int j = 0; j < 5; j++) {
        while (true) {
            int x = (rand() % 32) + static_cast<int>(this->worldInfo.getWidth()) / 2;
            int z = (rand() % 32) + static_cast<int>(this->worldInfo.getLength()) / 2;
            int height[6];
            for (unsigned int i = 0; i < 6; i++) {
                height[i] = static_cast<int>(round(((noiseGen.GetNoise(x + i, 0, z) + 1) * 10.0f) + 1.0f) + 10.0f);
            }

            if (height[0] >= 14 && height[5] == height[0] &&
                ((((x + 5) * height[5] * z) ^ worldInfo.getSeed()) % 61 != 0) &&
                ((((x + 4) * height[4] * z) ^ worldInfo.getSeed()) % 61 != 0) &&
                ((((x + 3) * height[3] * z) ^ worldInfo.getSeed()) % 61 != 0) &&
                ((((x + 2) * height[2] * z) ^ worldInfo.getSeed()) % 61 != 0) &&
                ((((x + 1) * height[1] * z) ^ worldInfo.getSeed()) % 61 != 0) &&
                (((x * height[0] * z) ^ worldInfo.getSeed()) % 61 != 0)) {
                if (j == 0) {
                    addA2(x, height[0], z);
                } else if (j == 1) {
                    addP8(x, height[0], z);
                }
                else if(j==2){
                    addL8(x, height[0], z);
                }else if(j==3){
                    addH7(x, height[0], z);
                } else if(j==4) {
                    addH3(x, height[0], z);
                }


                break;
            }
        }
    }


    //spawn platform
    auto chunk = this->chunkManager->getChunkByXZ({this->worldInfo.getWidth() / 2, this->worldInfo.getLength() / 2});
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2, 30, this->worldInfo.getLength() / 2}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 + 1, 30, this->worldInfo.getLength() / 2}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 + 1, 30, this->worldInfo.getLength() / 2 + 1}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2, 30, this->worldInfo.getLength() / 2 + 1}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 - 1, 30, this->worldInfo.getLength() / 2 + 1}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 - 1, 30, this->worldInfo.getLength() / 2}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 - 1, 30, this->worldInfo.getLength() / 2 - 1}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2, 30, this->worldInfo.getLength() / 2 - 1}, {1, 1, 1},
                             {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({this->worldInfo.getWidth() / 2 + 1, 30, this->worldInfo.getLength() / 2 - 1}, {1, 1, 1},
                             {0, 0, 0})));

}



void Engine::addH3(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

    //make H
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


    //make 3
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1, z}, {1, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 2.1, z}, {1, 0.75, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 3.2, z}, {1, 0.8, 1}, {0, 0, 0})));



    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


}

void Engine::addH7(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

    //make H
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


    //make 7
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 3.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 3.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+6, y + 3.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+6, y + 2.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 1.75, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1, z}, {1, 1, 1}, {0, 0, 0})));

}

void Engine::addA2(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

    //make A
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1, y + 3.25, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1, y + 2, z}, {1, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+2, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


    //make 2
     (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 3.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 3.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 2.5, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1.75, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1, z}, {1, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 1, z}, {1, 0.8, 1}, {0, 0, 0})));

}

void Engine::addL8(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

    //make L
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1, y + 1, z}, {1, 1, 1}, {0, 0, 0})));

    //make 8
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1, z}, {1, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 2.1, z}, {1, 0.75, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 3.2, z}, {1, 0.8, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

}


void Engine::addP8(unsigned int x, unsigned int y, unsigned int z) const {

    auto chunk = this->chunkManager->getChunkByXZ({x, z});

    //make P
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+0.975, y + 3.2, z}, {0.8, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+0.975, y + 2, z}, {0.8, 0.75, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1.8, y + 3, z}, {0.8, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::RUBY, Transform({x+1.8, y + 2, z}, {0.8, 1, 1}, {0, 0, 0})));

    //make 8
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+3, y + 3, z}, {1, 1, 1}, {0, 0, 0})));


    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 1, z}, {1, 0.8, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 2.1, z}, {1, 0.75, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+4, y + 3.2, z}, {1, 0.8, 1}, {0, 0, 0})));

    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 1, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 2, z}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::GOLD, Transform({x+5, y + 3, z}, {1, 1, 1}, {0, 0, 0})));

}