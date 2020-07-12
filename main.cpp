#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "draw.h"

/// Window size
static const int WIDTH = 1024;
static const int HEIGHT = 768;

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
enum selectedModel {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    RESET,
    WORLD
};

selectedModel selectedModel = WORLD;

/** Method to consume keyboard inputs to control the camera.
 *
 * @param window The currently active window
 * @param deltaTime The time elapsed since the last frame
 */
void modelSelect(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        selectedModel = ONE;

    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        selectedModel = TWO;

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        selectedModel = THREE;

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        selectedModel = FOUR;

    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        selectedModel = FIVE;

    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        selectedModel = WORLD;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        selectedModel = RESET;
}

/** Processes user input to control the models/world
 *
 * @param window The current active window
 * @param deltaTime The time since the last frame
 * @param objectModel The current model to be controlled
 */
void processInput(GLFWwindow* window, double deltaTime, Drawable* objectModel) {

    /* Scale Up and Down */
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        objectModel->setLocalTransform(glm::scale(objectModel->getLocalTransform(), glm::vec3(1.01, 1.01, 1.01)));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        objectModel->setLocalTransform(glm::scale(objectModel->getLocalTransform(), glm::vec3(0.99, 0.99, 0.99)));
    /* ----------------- */

    /* Move and Rotate models */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::translate(objectModel->getLocalTransform(), glm::vec3(0.0f, MoveSpeed * deltaTime, 0.0f)));
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::rotate(objectModel->getLocalTransform(), glm::radians(-5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::translate(objectModel->getLocalTransform(), glm::vec3(0.0f, -MoveSpeed * deltaTime, 0.0f)));
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::rotate(objectModel->getLocalTransform(), glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::translate(objectModel->getLocalTransform(), glm::vec3(-MoveSpeed * deltaTime, 0.0f, 0.0f)));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::rotate(objectModel->getLocalTransform(), glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::translate(objectModel->getLocalTransform(), glm::vec3(MoveSpeed * deltaTime, 0.0f, 0.0f)));
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        objectModel->setLocalTransform(
                glm::rotate(objectModel->getLocalTransform(), glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
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
void processInputWorld(GLFWwindow *window, double deltaTime, glm::mat4 &worldModelMatrix) {

    /* Scale Up and Down */
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        worldModelMatrix = glm::scale(worldModelMatrix, glm::vec3(1.01, 1.01, 1.01));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        worldModelMatrix = glm::scale(worldModelMatrix, glm::vec3(0.99f, 0.99f, 0.99f));
    /* ----------------- */

    /* Move and Rotate models */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(0.0f, MoveSpeed * deltaTime, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(0.0f, -MoveSpeed * deltaTime, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(-MoveSpeed * deltaTime, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        worldModelMatrix = glm::rotate(worldModelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        worldModelMatrix = glm::translate(worldModelMatrix, glm::vec3(MoveSpeed * deltaTime, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
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
}

/** Callback for mouse movement which controls the camera.
 *
 * @param window The currently active window
 * @param xpos The mouse's x position
 * @param ypos The mouse's y position
 */
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
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
void errorCallback(int error, const char* description) {
    fputs(description, stderr);
}


/// Main
int main(int argc, char* argv[]) {
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", nullptr, nullptr);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    L8 will = L8();
    H3 h3 = H3();
    A2 ewan = A2();
    P6 phil = P6();
    H7 moh = H7();

    // MVP matrices
    glm::mat4 worldModelMatrix = glm::mat4(1.0f);

    updateCameraVectors(worldCamera);
    glm::mat4 view = glm::lookAt(worldCamera.Position, WorldCenter, WorldUp);

    glm::mat4 projection = glm::mat4(1.0f);

    glEnable(GL_CULL_FACE);

    glLineWidth(3.0f);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input Processing
        if (selectedModel == ONE) {
            processInput(window, deltaTime, &h3);
        }
        if (selectedModel == TWO) {
            processInput(window, deltaTime, &will);
        }
        if (selectedModel == THREE) {
            processInput(window, deltaTime, &ewan);
        }
        if (selectedModel == FOUR) {
            processInput(window, deltaTime, &phil);
        }
        if (selectedModel == FIVE) {
            processInput(window, deltaTime, &moh);
        }
        if (selectedModel == WORLD) {
            processInputWorld(window, deltaTime, worldModelMatrix);
        }
        // Resets the world orientation and position
        if (selectedModel == RESET) {
            will.setLocalTransform(unitMat);
            h3.setLocalTransform(unitMat);
            ewan.setLocalTransform(unitMat);
            phil.setLocalTransform(unitMat);
            moh.setLocalTransform(unitMat);
            worldModelMatrix = unitMat;
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Projection matrix
        projection = glm::perspective(glm::radians(worldCamera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 250.0f);
        view = GetViewMatrix(worldCamera);

        // Draw models
        grid.draw(projection * view * worldModelMatrix, renderMode);

        axis.draw(projection * view * worldModelMatrix, renderMode);

        will.draw(projection * view * worldModelMatrix * glm::translate(unitMat, glm::vec3(43.0f, 0.0f, 49.0f)),
                  renderMode);

        h3.draw(projection * view * worldModelMatrix * unitMat, renderMode);

        ewan.draw(projection * view * worldModelMatrix * glm::translate(unitMat, glm::vec3(30.0f, 0.0f, -50.0f)),
                  renderMode);

        phil.draw(projection * view * worldModelMatrix * glm::translate(unitMat, glm::vec3(-50.0f, 0.0f, -50.0f)),
                  renderMode);

        moh.draw(projection * view * worldModelMatrix * glm::translate(unitMat, glm::vec3(-49.5f, 0.1f, 49.0f)),
                 renderMode);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate once the window should be closed
    glfwTerminate();
    return 0;
}