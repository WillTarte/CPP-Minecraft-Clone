//
// Created by Willi on 7/30/2020.
//

#include <vector>
#include <glm/ext.hpp>
#include "../libs/easylogging++.h"
#include "../libs/FastNoise.h"
#include "../include/engine.h"


Engine::Engine(Config config) {

    LOG(INFO) << "Initializing Engine hello ..."<< config.windowHeight << ", windowWidth=" << config.windowWidth << "}";
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

    LOG(INFO) << "Generating World ... ";
    this->worldInfo = WorldInfo{};
    this->chunkManager = std::make_unique<ChunkManager>(this->worldInfo);

    LOG(INFO) << "Inserting Entities into the World ...";
    generateWorld();

    LOG(INFO) << "Number of entities: " << this->chunkManager->getNumberOfEntities();
    LOG(INFO) << "Number of Chunks: " << this->chunkManager->getNumberOfChunks();

    LOG(INFO) << "Generated world using seed " << worldInfo.getSeed() << ".";
    this->player = std::make_unique<Player>(glm::vec3(WORLD_WIDTH / 2, 32.0f, WORLD_LENGTH / 2));


    LOG(INFO) << "Engine is primed and ready.";

}

void Engine::runLoop() {

    //TODO this should not be here
    Shader basicShader = Shader((fs::current_path().string() + "/resources/shaders/ModelVertexShader.glsl").c_str(),
                                (fs::current_path().string() + "/resources/shaders/ModelFragmentShader.glsl").c_str());
    basicShader.use();
    // ***********

    glfwSwapInterval(1);

    double t = 0.0f;
    double dt = 1.0f / 30.0f;
    double currentTime = glfwGetTime();

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        currentTime = newTime;

        while (frameTime > 0.0f) {
            double deltaTime = frameTime < dt ? frameTime : dt;
            frameTime -= deltaTime;
            t += deltaTime;
        }


        player->processInput(this->window);
        player->update(this, static_cast<float>(dt));
        // --------------------
        //not sure if I need it this way
//=======
//
//        player->update(this, static_cast<float>(deltaTime));
//        player->processInput(this->window, deltaTime, this);
//
//>>>>>>> removeBlocksUpdate

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
        LOG(DEBUG) << "Rendering " << chunksToDraw.size() << " Chunks.";
        for (const auto &chunk : chunksToDraw) {
            chunk->renderChunk(basicShader);
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


//whenever the mouse is clicked this is called
//built with help from
//https://antongerdelan.net/opengl/raycasting.html



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
            int height = round((tempHeight * 10) + 1) + 10;

            auto chunk = this->chunkManager->getChunkByXZ({x, z});

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


           // if (xColl && yColl && zColl)
         //       return {&ent};

        }
    }

    //spawn platform
    auto chunk = this->chunkManager->getChunkByXZ({WORLD_WIDTH / 2, WORLD_LENGTH / 2});
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2, 30, WORLD_LENGTH / 2}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 + 1, 30, WORLD_LENGTH / 2}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 + 1, 30, WORLD_LENGTH / 2 + 1}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2, 30, WORLD_LENGTH / 2 + 1}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 - 1, 30, WORLD_LENGTH / 2 + 1}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 - 1, 30, WORLD_LENGTH / 2}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 - 1, 30, WORLD_LENGTH / 2 - 1}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2, 30, WORLD_LENGTH / 2 - 1}, {1, 1, 1}, {0, 0, 0})));
    (*chunk)->addEntity(
            Entity(ModelType::CUBE, BlockID::STONE,
                   Transform({WORLD_WIDTH / 2 + 1, 30, WORLD_LENGTH / 2 - 1}, {1, 1, 1}, {0, 0, 0})));

}



void Engine::removeEntity(glm::vec3 dir) {

    //CAMERA VECTOR IS REALLY THE END POINT OF THE RAY

    //PLAYER POS IS THE start

    Entity *closestEnt;
    float previousDistance;

    glm::vec3 currentPlayerPos = player->getTransform().getPosition();

    currentPlayerPos.y = currentPlayerPos.y +1;
    bool firstLoop = false;
    bool blockWithinFive = false;


    //get current chunk
    std::optional<std::shared_ptr<Chunk>> chunk = this->chunkManager->getChunkByXZ({currentPlayerPos.x, currentPlayerPos.z});


    for (auto &blocksById : chunk.value()->getEntities()) {
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


            bool check1 = checkIntersection(currentPlayerPos,dir,V0,V2,V1);

            bool check2 = checkIntersection(currentPlayerPos,dir,V0,V3,V2);

            bool check3 = checkIntersection(currentPlayerPos,dir,V2,V3,V4);

            bool check4 = checkIntersection(currentPlayerPos,dir,V2,V4,V5);

            bool check5 = checkIntersection(currentPlayerPos,dir,V1,V2,V5);

            bool check6 = checkIntersection(currentPlayerPos,dir,V1,V5,V6);

            bool check7 = checkIntersection(currentPlayerPos,dir,V0,V7,V4);

            bool check8 = checkIntersection(currentPlayerPos,dir,V0,V4,V3);

            bool check9 = checkIntersection(currentPlayerPos,dir,V5,V4,V7);

            bool check10 = checkIntersection(currentPlayerPos,dir,V5,V7,V6);

            bool check11 = checkIntersection(currentPlayerPos,dir,V0,V6,V7);

            bool check12 = checkIntersection(currentPlayerPos,dir,V0,V1,V6);

            if(check1 || check2 || check3 || check4 || check5 || check6 || check7 || check8 || check9 || check10 ||check11 || check12){
              //  std::cout << "intersection";
               // std::cout << blockPos.x << " " <<  blockPos.y << " " << blockPos.z << "\n";

               if(firstLoop == false){
                   closestEnt = &ent;
                   previousDistance = 1000;
               }

                //TODO ONE change from reseting the transfrom to deleting the object

                glm::vec3 currentEntPos = ent.getTransform().getPosition();

                float distance = sqrt((pow( currentPlayerPos.x - currentEntPos.x, 2) +  pow(currentPlayerPos.y - currentEntPos.y, 2) +  pow(currentPlayerPos.z - currentEntPos.z, 2)));


                std::cout << "distance" << distance << "\n";


                //check if block is within 5 blocks
                if(distance <= 5) {
                    if(!blockWithinFive){
                        blockWithinFive = true;
                    }

                    if(distance< previousDistance){
                        closestEnt = &ent;
                        previousDistance = distance;
                    }

                }

            }

        }
    }

    //now we have the closet block in the

    if(blockWithinFive){
        closestEnt->resetTransform();
    }
}




void Engine::placeBlock(glm::vec3 dir) {

    Entity *closestEnt;
    float previousDistance;



    glm::vec3 currentPlayerPos = player->getTransform().getPosition();

    currentPlayerPos.y = currentPlayerPos.y +1;

    bool firstLoop = false;
    bool blockWithinFive = false;

    //get current chunk
    std::optional<std::shared_ptr<Chunk>> chunk = this->chunkManager->getChunkByXZ({currentPlayerPos.x, currentPlayerPos.z});


    for (auto &blocksById : chunk.value()->getEntities()) {
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


            bool check1 = checkIntersection(currentPlayerPos,dir,V0,V2,V1);

            bool check2 = checkIntersection(currentPlayerPos,dir,V0,V3,V2);

            bool check3 = checkIntersection(currentPlayerPos,dir,V2,V3,V4);

            bool check4 = checkIntersection(currentPlayerPos,dir,V2,V4,V5);

            bool check5 = checkIntersection(currentPlayerPos,dir,V1,V2,V5);

            bool check6 = checkIntersection(currentPlayerPos,dir,V1,V5,V6);

            bool check7 = checkIntersection(currentPlayerPos,dir,V0,V7,V4);

            bool check8 = checkIntersection(currentPlayerPos,dir,V0,V4,V3);

            bool check9 = checkIntersection(currentPlayerPos,dir,V5,V4,V7);

            bool check10 = checkIntersection(currentPlayerPos,dir,V5,V7,V6);

            bool check11 = checkIntersection(currentPlayerPos,dir,V0,V6,V7);

            bool check12 = checkIntersection(currentPlayerPos,dir,V0,V1,V6);

            if(check1 || check2 || check3 || check4 || check5 || check6 || check7 || check8 || check9 || check10 ||check11 || check12){
                //  std::cout << "intersection";
                // std::cout << blockPos.x << " " <<  blockPos.y << " " << blockPos.z << "\n";

                if(firstLoop == false){
                    closestEnt = &ent;
                    previousDistance = 1000;
                }

                //TODO ONE change from reseting the transfrom to deleting the object

                glm::vec3 currentEntPos = ent.getTransform().getPosition();

                float distance = sqrt((pow( currentPlayerPos.x - currentEntPos.x, 2) +  pow(currentPlayerPos.y - currentEntPos.y, 2) +  pow(currentPlayerPos.z - currentEntPos.z, 2)));





                //check if block is within 5 blocks
                if(distance <= 5) {
                    if(!blockWithinFive){
                        blockWithinFive = true;
                    }

                    if(distance< previousDistance){
                        closestEnt = &ent;
                        previousDistance = distance;
                    }

                }

            }

        }
    }


    //currently only places blocks on top
    if(blockWithinFive){
        glm::vec3 currentEntPos = closestEnt->getTransform().getPosition();

        //here we have to figure out what direction the block is from the
        float additionX = 0.0;
        float additionY = 0.0;
        float additionZ = 0.0;



        glm::vec3 newBlockPlacement = glm::vec3(currentEntPos.x - round(dir.x) ,currentEntPos.y -round(dir.y) , currentEntPos.z -round(dir.z));
        (*chunk)->addEntity((Entity(ModelType::CUBE, BlockID::DIRT, Transform({newBlockPlacement}, {1, 1, 1}, {0, 0, 0}))));
       // this->addEntity(Entity(ModelType::CUBE, BlockID::DIRT, Transform({newBlockPlacement}, {1, 1, 1}, {0, 0, 0})));
    }

}



//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
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
        //std::cout << point.x << " " <<  point.y << " " << point.z << "\n";
        return true;
    } else
        return false;
}