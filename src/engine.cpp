//
// Created by Willi on 7/30/2020.
//
#include "../include/engine.h"

Engine::Engine(Config config) {

    LOG(INFO) << "Initializing Engine ...";
    //do some processing based on config
    LOG(DEBUG) << "Config {windowHeight=" << config.windowHeight << ", windowWidth=" << config.windowWidth << "}";
    this->config = config;
    this->windowWidth = config.windowWidth;
    this->windowHeight = config.windowHeight;

    LOG(INFO) << "Inserting Entities into the World ...";
    this->entities = std::unordered_map<BlockID, std::vector<Entity>>();
    for (auto ent : allBlockIDs) {
        entities.insert({ent, std::vector<Entity>()});
    }

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

        player->processInput(this->window, deltaTime, this);
        player->update(this, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(config.fov), (float) windowWidth / (float) windowHeight,
                                                0.1f, 100.0f);

        // rendering stuff here
        basicShader.setMat4("view", player->getPlayerView());
        basicShader.setMat4("projection", projection);

        for (auto &blocksByID : this->entities) {
            // do some setting up per block type
            //iterate trough entites of that block type
            for (auto &blocks : blocksByID.second) {
                blocks.draw(basicShader);
            }
        }

        player->draw(basicShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

GLFWwindow *Engine::getWindow() const {
    return window;
}

//whenever the mouse is clicked this is called
//built with help from
//https://antongerdelan.net/opengl/raycasting.html



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Engine::mouseCallbackFunc(GLFWwindow *windowParam, double xpos, double ypos) {
    player->look(windowParam, xpos, ypos);
}

std::optional<Entity *> Engine::getEntityByWorldPos(const glm::vec3 worldPos) {
    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {
            glm::vec3 entityPos = ent.getTransform().getPosition();

            bool withinX = (int) worldPos.x == (int) entityPos.x;
            bool withinY = (int) worldPos.y == (int) entityPos.y;
            bool withinZ = (int) worldPos.z == (int) entityPos.z;

            if (withinX && withinZ && withinY) {
                return {&ent};
            }
        }
    }
    return {};
}

std::optional<Entity *> Engine::getEntityByBoxCollision(glm::vec3 worldPos, BoundingBox box) {
    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {

            bool xColl = (ent.getTransform().getPosition().x <= worldPos.x + box.dimensions.x &&
                          ent.getTransform().getPosition().x + ent.box.dimensions.x >= worldPos.x);
            bool yColl = (ent.getTransform().getPosition().y <= worldPos.y + box.dimensions.y &&
                          ent.getTransform().getPosition().y + ent.box.dimensions.y >= worldPos.y);
            bool zColl = (ent.getTransform().getPosition().z <= worldPos.z + box.dimensions.z &&
                          ent.getTransform().getPosition().z + ent.box.dimensions.z >= worldPos.z);

            if (xColl && yColl && zColl)
                return {&ent};
        }
    }
    return {};
}

void Engine::removeEntity(const glm::vec3 worldPos) {

    BoundingBox box = BoundingBox();
    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {

            bool xColl = (ent.getTransform().getPosition().x <= worldPos.x + box.dimensions.x &&
                          ent.getTransform().getPosition().x + ent.box.dimensions.x >= worldPos.x);
            bool yColl = (ent.getTransform().getPosition().y <= worldPos.y + box.dimensions.y &&
                          ent.getTransform().getPosition().y + ent.box.dimensions.y >= worldPos.y);
            bool zColl = (ent.getTransform().getPosition().z <= worldPos.z + box.dimensions.z &&
                          ent.getTransform().getPosition().z + ent.box.dimensions.z >= worldPos.z);

            if (xColl && yColl && zColl)
                entities.erase(ent.getBlockID());
        }
    }




}
