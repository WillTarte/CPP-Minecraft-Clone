#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "draw.h"
#include "scene.h"

/// Window size
static int WIDTH = 1024;
static int HEIGHT = 768;

/// Some global vars
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
float MoveSpeed = 20.0f;
static const glm::mat4 unitMat = glm::mat4(1.0f);

/// Render Mode
GLenum renderMode = GL_TRIANGLES;

/// Global world vector constants
static const glm::vec3 WorldCenter = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

///last space bar action
int lastSpaceAction = GLFW_RELEASE;

/// Camera Struct to hold data for the current view
struct Camera {
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float MouseSensitivity = 0.5f;
    float Zoom = 45.0f;
    glm::vec3 Position = {0.0f, 30.0f, 130.0f};
    glm::vec3 Front = {0.0f, 0.0f, 1.0f};
    glm::vec3 Up = {0.0f, 1.0f, 0.0f};
    glm::vec3 Right = {1.0f, 0.0f, 0.0f};

} worldCamera;

void randomizePosition(Drawable *model) {
    model->setPosition({rand() % 101 - 50, 0, rand() % 101 - 50});
}

/** Updates the camera vectors to update the current view correctly.
 *
 * @param camera the current camera (data)
 */
void updateCameraVectors(Camera &camera) {
    // calculate the new Front vector
    glm::vec3 tempfront;
    tempfront.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
    tempfront.y = sin(glm::radians(camera.Pitch));
    tempfront.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));

    camera.Front = glm::normalize(tempfront);
    // also re-calculate the Right and Up vector
    camera.Right = glm::normalize(glm::cross(camera.Front,
                                             WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    camera.Up = glm::normalize(glm::cross(camera.Right, camera.Front));
}

/// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void processMouseMovement(float xoffset, float yoffset, GLFWwindow *window, Camera &camera,
                          GLboolean constrainPitch = true) {

    xoffset *= camera.MouseSensitivity;
    yoffset *= camera.MouseSensitivity;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        camera.Yaw += xoffset;

    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        camera.Pitch += yoffset;
    }

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (camera.Pitch > 89.0f)
            camera.Pitch = 89.0f;
        if (camera.Pitch < -89.0f)
            camera.Pitch = -89.0f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        camera.Zoom -= (float) yoffset;
        if (camera.Zoom < 1.0f)
            camera.Zoom = 1.0f;
        if (camera.Zoom > 45.0f)
            camera.Zoom = 45.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors(camera);
}

glm::mat4 GetViewMatrix(Camera &camera) {
    return glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
}

/// So the program knows what model is selected
enum class SelectedModel {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    RESET,
    WORLD,
    LIGHT,
    RANDOMIZE
};

bool startWalking = 0;
int walkingCount = 0;

SelectedModel selectedModel = SelectedModel::WORLD;

/** Method to consume keyboard inputs to control the camera.
 *
 * @param window The currently active window
 * @param deltaTime The time elapsed since the last frame
 */
void modelSelect(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        selectedModel = SelectedModel::ONE;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        selectedModel = SelectedModel::TWO;

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        selectedModel = SelectedModel::THREE;

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        selectedModel = SelectedModel::FOUR;

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        selectedModel = SelectedModel::FIVE;

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        selectedModel = SelectedModel::WORLD;

    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
        selectedModel = SelectedModel::LIGHT;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        selectedModel = SelectedModel::RESET;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && lastSpaceAction == GLFW_RELEASE) {
        selectedModel = SelectedModel::RANDOMIZE;
        lastSpaceAction = GLFW_PRESS;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
        lastSpaceAction = GLFW_RELEASE;
}

/** Processes user input to control the models/world
 *
 * @param window The current active window
 * @param deltaTime The time since the last frame
 * @param objectModel The current model to be controlled
 */
void processInput(GLFWwindow *window, double deltaTime, Drawable *objectModel) {

    /* Scale Up and Down */
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        objectModel->setTransform(glm::scale(objectModel->getTransform(), glm::vec3(1.01, 1.01, 1.01)));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        objectModel->setTransform(glm::scale(objectModel->getTransform(), glm::vec3(0.99, 0.99, 0.99)));
    /* ----------------- */

    /* Move and Rotate models */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setPosition(objectModel->getPosition() + glm::vec3(0.0f, MoveSpeed * deltaTime, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        objectModel->setTransform(
                glm::rotate(objectModel->getTransform(), glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setPosition(objectModel->getPosition() + glm::vec3(0.0f, -MoveSpeed * deltaTime, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        objectModel->setTransform(
                glm::rotate(objectModel->getTransform(), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setPosition(objectModel->getPosition() + glm::vec3(-MoveSpeed * deltaTime, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        objectModel->setTransform(
                glm::rotate(objectModel->getTransform(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setPosition(objectModel->getPosition() + glm::vec3(MoveSpeed * deltaTime, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        objectModel->setTransform(
                glm::rotate(objectModel->getTransform(), glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    ///code for walking models
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){

        startWalking = 1;
            //attempting to shear this model

            glm::mat4 shear =
                    {
                            { 1.0f, 0.0f, -1.0f, 0.0f },
                            { 0.0f, 1.0f, -1.0f, 0.0f },
                            { 0.0f, 0.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f, 1.0f },
                    };

            objectModel->setPosition(objectModel->getPosition() + glm::vec3(0.0f, 0.0, 1.0f));
            objectModel->setTransform(shear);

        }



        ///code for walking models
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){

            startWalking = 1;
            //attempting to shear this model

            glm::mat4 shear =
                    {
                            { 1.0f, 0.0f, 1.0f, 0.0f },
                            { 0.0f, 1.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f, 1.0f },
                    };

            objectModel->setPosition(objectModel->getPosition() + glm::vec3(0.0f, 0.0, -1.0f));
            objectModel->setTransform(shear);

        }


    if(startWalking == 1){

        walkingCount += 1;

        if(walkingCount ==30){
            startWalking = 0;
            walkingCount = 0;
            glm::mat4 stand =
                    {
                            { 1.0f, 0.0f, 0.0f, 0.0f },
                            { 0.0f, 1.0f, 0.0f, 0.0f },
                            { 0.0f, 0.0f, 1.0f, 0.0f },
                            { 0.0f, 0.0f, 0.0f, 1.0f },
                    };


            objectModel->setTransform(stand);
        }



    }

    /* ---------------------- */

    /* Change Render Mode */
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        renderMode = GL_POINTS;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        renderMode = GL_LINES;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        renderMode = GL_TRIANGLES;
    /* ------------------ */
}

/** Processes user input for the world
 *
 * @param window
 * @param deltaTime
 * @param worldModelMatrix
 */
void processInputWorld(GLFWwindow *window, double deltaTime, glm::mat4 &worldModelMatrix, Light &lightSource) {

    /* Scale Up and Down */
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        worldModelMatrix = glm::scale(worldModelMatrix, glm::vec3(1.01, 1.01, 1.01));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        worldModelMatrix = glm::scale(worldModelMatrix, glm::vec3(0.99f, 0.99f, 0.99f));
    /* ----------------- */

    /* Move and Rotate models */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(0.0f, MoveSpeed * deltaTime, 0.0f));
        lightSource.setLightPosition(lightSource.getLightPos() + glm::vec3(0.0f, MoveSpeed * deltaTime, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(0.0f, -MoveSpeed * deltaTime, 0.0f));
        lightSource.setLightPosition(lightSource.getLightPos() + glm::vec3(0.0f, -MoveSpeed * deltaTime, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(-MoveSpeed * deltaTime, 0.0f, 0.0f));
        lightSource.setLightPosition(lightSource.getLightPos() + glm::vec3(-MoveSpeed * deltaTime, 0.0f, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(MoveSpeed * deltaTime, 0.0f, 0.0f));
        lightSource.setLightPosition(lightSource.getLightPos() + glm::vec3(MoveSpeed * deltaTime, 0.0f, 0.0f));
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    /* ---------------------- */

    /* Weird rotation stuff that assignment doesnt explain well */
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    /* -------------------------------------------------------- */
}

/** Processes inputs for a Light
 *
 * @param window
 * @param deltaTime
 * @param light
 */
void processInputLightSource(GLFWwindow *window, double deltaTime, Light &light) {
    //WASD -> UP LEFT DOWN RIGHT
    // up arrow / down arrow -> forwards and backwards

    float velocity = MoveSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() + (glm::vec3(0.0f, 0.0f, 1.0f * velocity)));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() - (glm::vec3(0.0f, 0.0f, 1.0f * velocity)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() + (glm::vec3(0.0f, 1.0f * velocity, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() - (glm::vec3(0.0f, 1.0f * velocity, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() - (glm::vec3(1.0f * velocity, 0.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        light.setLightPosition(light.getLightPos() + (glm::vec3(1.0f * velocity, 0.0f, 0.0f)));
}


/** Callback for whenever the window size is changed.
 *
 * @param window The currently active window
 * @param width The new width
 * @param height The new height
 */
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}

/** Callback for mouse movement which controls the camera.
 *
 * @param window The currently active window
 * @param xpos The mouse's x position
 * @param ypos The mouse's y position
 */
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    processMouseMovement(xoffset, yoffset, window, worldCamera);
}

/// glfw: error callback
/// --------------------
void errorCallback(int error, const char *description) {
    fputs(description, stderr);
}

void renderQuad();

/// Main
int main(int argc, char *argv[]) {
    // Necessary variables

    // timing
    double deltaTime = 0.0f;    // time between current frame and last frame
    double lastFrame = 0.0f;

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set current context and callbacks
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, modelSelect);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetErrorCallback(errorCallback);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        //Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Initialize models
    GroundGrid grid = GroundGrid();
    Axis axis = Axis();
    Light light = Light();

    ModelL modelL = ModelL();
    modelL.setPosition(glm::vec3(43.0f, 0.0f, 49.0f));
    Model8 model8 = Model8();

    ModelH modelH = ModelH();
    modelH.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    Model3 model3 = Model3();




    ModelA modelA = ModelA();
    modelA.setPosition(glm::vec3(30.0f, 0.0f, -50.0f));
    Model2 model2 = Model2();

    ModelP modelP = ModelP();
    modelP.setPosition(glm::vec3(-50.0f, 0.0f, -50.0f));
    Model6 model6 = Model6();

    ModelH modelH_2 = ModelH();
    modelH_2.setPosition(glm::vec3(-49.5f, 0.1f, 49.0f));
    Model7 model7 = Model7();

    glm::mat4 output = glm::translate(unitMat, glm::vec3(4.0, 6.5, 0.0));
    output = glm::scale(output, glm::vec3(1.75, 1.75, 1.75));

    glm::mat4 bigOutput = glm::translate(unitMat, glm::vec3(8.0, 10.5, 0.0));
    bigOutput = glm::scale(bigOutput, glm::vec3(3.0, 3.0, 3.0));

    Sphere bubbleWill = Sphere(output);
    SceneNode bubbleWillNode = SceneNode(&bubbleWill, "bubbleWill");
    Sphere bubbleH3 = Sphere(output);
    SceneNode bubbleH3Node = SceneNode(&bubbleH3, "bubbleH3");
    Sphere bubbleEwan = Sphere(bigOutput);
    SceneNode bubbleEwanNode = SceneNode(&bubbleEwan, "bubbleEwan");
    Sphere bubblePhil = Sphere(output);
    SceneNode bubblePhilNode = SceneNode(&bubblePhil, "bubblePhil");
    Sphere bubbleMoh = Sphere(output);
    SceneNode bubbleMohNode = SceneNode(&bubbleMoh, "bubbleMoh");


    // Initialize Nodes
    SceneNode gridNode = SceneNode(&grid, "GroundGrid");
    SceneNode axisNode = SceneNode(&axis, "Axis");
    SceneNode lightNode = SceneNode(&light, "WorldLight");


    SceneNode modelLNode = SceneNode(&modelL, "modelL");
    SceneNode model8Node = SceneNode(&model8, "model8");
    modelLNode.addChild(&model8Node);
    modelLNode.addChild(&bubbleWillNode);

    SceneNode modelHNode = SceneNode(&modelH, "modelH");
    SceneNode model3Node = SceneNode(&model3, "model3");
    modelHNode.addChild(&model3Node);
    modelHNode.addChild(&bubbleH3Node);

    SceneNode modelANode = SceneNode(&modelA, "modelA");
    SceneNode model2Node = SceneNode(&model2, "model2");
    modelANode.addChild(&model2Node);
    modelANode.addChild(&bubbleEwanNode);

    SceneNode modelPNode = SceneNode(&modelP, "modelP");
    SceneNode model6Node = SceneNode(&model6, "model6");
    modelPNode.addChild(&model6Node);
    modelPNode.addChild(&bubblePhilNode);

    SceneNode modelH_2Node = SceneNode(&modelH_2, "modelH_2");
    SceneNode model7Node = SceneNode(&model7, "model7");
    modelH_2Node.addChild(&model7Node);
    modelH_2Node.addChild(&bubbleMohNode);

    // Set up scene
    Scene world = Scene();
    world.addNode(&lightNode);
    world.addNode(&gridNode);
    world.addNode(&axisNode);
    world.addNode(&modelLNode);
    world.addNode(&modelHNode);
    world.addNode(&modelANode);
    world.addNode(&modelPNode);
    world.addNode(&modelH_2Node);

    // MVP matrices
    glm::mat4 worldModelMatrix = glm::mat4(1.0f);

    updateCameraVectors(worldCamera);
    glm::mat4 view = glm::lookAt(worldCamera.Position, WorldCenter, WorldUp);

    glm::mat4 projection = glm::mat4(1.0f);

    //TODO: fix light + culling
    //glEnable(GL_CULL_FACE);

    glLineWidth(3.0f);

    Shader debugDepthQuad("resources/shaders/DebugDepthQuadVertex.glsl",
                          "resources/shaders/DebugDepthQuadFragment.glsl");
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input Processing
        if (selectedModel == SelectedModel::ONE) {
            processInput(window, deltaTime, &modelH);
        }
        if (selectedModel == SelectedModel::TWO) {
            processInput(window, deltaTime, &modelL);
        }
        if (selectedModel == SelectedModel::THREE) {
            processInput(window, deltaTime, &modelA);
        }
        if (selectedModel == SelectedModel::FOUR) {
            processInput(window, deltaTime, &modelP);
        }
        if (selectedModel == SelectedModel::FIVE) {
            processInput(window, deltaTime, &modelH_2);
        }
        if (selectedModel == SelectedModel::WORLD) {
            processInputWorld(window, deltaTime, worldModelMatrix, light);
        }
        if (selectedModel == SelectedModel::LIGHT) {
            processInputLightSource(window, deltaTime, light);
        }
        // Resets the world orientation and position
        if (selectedModel == SelectedModel::RESET) {
            modelL.setTransform(unitMat);
            modelH.setTransform(unitMat);
            modelA.setTransform(unitMat);
            modelP.setTransform(unitMat);
            modelH_2.setTransform(unitMat);
            light.setLightPosition(glm::vec3(0.0f, 30.0f, 0.0f));
            light.setTransform(unitMat);
            worldModelMatrix = unitMat;
        }

        if (selectedModel == SelectedModel::RANDOMIZE) {
            randomizePosition(&modelL);
            randomizePosition(&modelH);
            randomizePosition(&modelA);
            randomizePosition(&modelP);
            randomizePosition(&modelH_2);
            selectedModel = SelectedModel::WORLD;
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, world.depthMap);
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.1f, far_plane = 200.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)world.SHADOW_WIDTH / (GLfloat)world.SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
        lightView = glm::lookAt(light.getLightPos(), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        //TODO: once culling is fixed; glCullFace(GL_FRONT);
        world.drawShadows(worldModelMatrix, lightSpaceMatrix);
        //TODO: once culling is fixed; glCullFace(GL_BACK);

        // Render Scene
        // reset viewport
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Projection matrix
        projection = glm::perspective(glm::radians(worldCamera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 250.0f);
        view = GetViewMatrix(worldCamera);

        // Draw models
        world.changeRenderMode(renderMode);
        world.draw(worldModelMatrix, view, projection, lightSpaceMatrix, worldCamera.Position, light.getLightParams());

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, world.depthMap);
        //renderQuad();

        //std::cout << light.getLightPos().x << " " << light.getLightPos().y << " " << light.getLightPos().z << std::endl;

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate once the window should be closed
    glfwTerminate();
    return 0;
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}