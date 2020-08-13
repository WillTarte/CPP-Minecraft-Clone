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

void Engine::removeEntity(const glm::vec3 cameraVector, const glm::vec3 playerPos) {


    BoundingBox box = BoundingBox();
    for (auto &blocksById : entities) {
        for (auto &ent : blocksById.second) {





            glm::vec3 blockPos = ent.getTransform().getPosition();

            //TODO remove all the +0 lol
            glm::vec3 V0 =  glm::vec3(blockPos.x+0, blockPos.y+0, blockPos.z+0);
            glm::vec3 V1 =  glm::vec3(blockPos.x+1, blockPos.y+0, blockPos.z+0);
            glm::vec3 V2 =  glm::vec3(blockPos.x+1, blockPos.y+1, blockPos.z+0);
            glm::vec3 V3 =  glm::vec3(blockPos.x+0, blockPos.y+1, blockPos.z+0);
            glm::vec3 V4 =  glm::vec3(blockPos.x+0, blockPos.y+1, blockPos.z+1);
            glm::vec3 V5 =  glm::vec3(blockPos.x+1, blockPos.y+1, blockPos.z+1);
            glm::vec3 V6 =  glm::vec3(blockPos.x+1, blockPos.y+0, blockPos.z+1);
            glm::vec3 V7 =  glm::vec3(blockPos.x+0, blockPos.y+0, blockPos.z+1);


            bool check1 = checkIntersection(playerPos,cameraVector,V0,V2,V1);

            bool check2 = checkIntersection(playerPos,cameraVector,V0,V3,V2);

            bool check3 = checkIntersection(playerPos,cameraVector,V2,V3,V4);

            bool check4 = checkIntersection(playerPos,cameraVector,V2,V4,V5);

            bool check5 = checkIntersection(playerPos,cameraVector,V1,V2,V5);

            bool check6 = checkIntersection(playerPos,cameraVector,V1,V5,V6);

            bool check7 = checkIntersection(playerPos,cameraVector,V0,V7,V4);

            bool check8 = checkIntersection(playerPos,cameraVector,V0,V4,V3);

            bool check9 = checkIntersection(playerPos,cameraVector,V5,V4,V7);

            bool check10 = checkIntersection(playerPos,cameraVector,V5,V7,V6);

            bool check11 = checkIntersection(playerPos,cameraVector,V0,V6,V7);

            bool check12 = checkIntersection(playerPos,cameraVector,V0,V1,V6);

            if(check1 || check2 || check3 || check4 || check5 || check6 || check7 || check8 || check9 || check10 ||check11 || check12){
                std::cout << "intersection";
                std::cout << blockPos.x << " " <<  blockPos.y << " " << blockPos.z << "\n";
            }
            
            //just try with one face
//            glm::vec3 top = glm::vec3(blockPos.x, blockPos.y +1.0, blockPos.z + 1.0);
//            glm::vec3 left = glm::vec3(blockPos.x, blockPos.y, blockPos.z + 1.0);
//
//            bool intersectionCheck = checkIntersection(playerPos,cameraVector,blockPos,top,left);
//
//            glm::vec3 top1 = glm::vec3(blockPos.x, blockPos.y+1.0, blockPos.z);
//            glm::vec3 top2 = glm::vec3(blockPos.x+1.0, blockPos.y+1.0, blockPos.z);
//            glm::vec3 top3 = glm::vec3(blockPos.x+1.0, blockPos.y+1.0, blockPos.z+1.0);
//
//            bool topCheck = checkIntersection(playerPos,cameraVector,top1,top2,top3);
//            if(intersectionCheck){
//                std::cout << "sideINTERSECTION";
//            }
//
//            if(topCheck){
//                std::cout << "topINTERSECTION";
//            }
//           if(worldPos.y == entY) {
//               std::cout <<true;
//               entities.erase(ent.getBlockID());
//           }
        }
    }





}


bool Engine::checkIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayVector, glm::vec3 vertex0, glm::vec3 vertex1, glm::vec3 vertex2) {
    const float EPSILON = 0.0000001;
    glm::vec3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = glm::cross(rayVector, edge2);
    a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return false;
    f = 1 / a;
    s = rayOrigin - vertex0;
    u = f * glm::dot(s, h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = glm::cross(s, edge1);
    v = f * glm::dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    float t = f * glm::dot(edge2, q);
    if (t > EPSILON) {
        glm::vec3 plus = glm::normalize(rayVector) * (t * glm::length(rayVector));
        glm::vec3 point = rayOrigin + plus;
        std::cout << point.x << " " <<  point.y << " " << point.z << "\n";
        return true;
    } else
        return false;
}