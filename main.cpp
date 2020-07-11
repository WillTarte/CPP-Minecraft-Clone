#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "draw.h"

/// Window size
static const int WIDTH = 800;
static const int HEIGHT = 600;

/// Render Mode
GLenum renderMode = GL_TRIANGLES;

/// Camera
Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;


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

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        selectedModel = RESET;

}


void processInput(GLFWwindow *window, double deltaTime, glm::mat4 &modelMatrix) {



    /* Scale Up and Down */
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.01, 1.01, 1.01));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.99f, 0.99f, 0.99f));
    /* ----------------- */

    /* Move and Rotate models */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.1f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.1f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.1f, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.1f, 0.0f, 0.0f));
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    /* ---------------------- */

    /* Weird rotation stuff that assignment doesnt explain well */
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    /* -------------------------------------------------------- */

    /* Change Render Mode */
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        renderMode = GL_POINTS;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        renderMode = GL_LINES;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        renderMode = GL_TRIANGLES;
    /* ------------------ */

    /*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(RIGHT, deltaTime);*/
}

/** Callback for whenever the window size is changed.
 *
 * @param window The currently active window
 * @param width The new width
 * @param height The new height
 */
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
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

    camera.processMouseMovement(xoffset, yoffset, window);
}

/** Callback for whenever the scroll wheel is used to control the camera zoom.
 *
 * @param window The currently active window
 * @param xoffset The x offset (unused)
 * @param yoffset The scroll offset
 */
/// glfw: whenever the mouse scroll wheel scrolls, this callback is called
/// ----------------------------------------------------------------------
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

/// glfw: error callback
/// --------------------
void errorCallback(int error, const char *description) {
    fputs(description, stderr);
}


/// Main
int main(int argc, char *argv[]) {
    // Necessary variables

    // timing
    double deltaTime = 0.0f;    // time between current frame and last frame
    double lastFrame = 0.0f;

    // *********************

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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
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
    L8 will = L8();
    H3 h3 = H3();
    Axis axis = Axis(Shader("resources/shaders/BasicVertexShader.glsl", "resources/shaders/BasicFragmentShader.glsl"));

    // MVP matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    //model matrices
    glm::mat4 L8model = glm::mat4(1.0f);
    glm::mat4 H3model = glm::mat4(1.0f);

    glEnable(GL_CULL_FACE);
    glLineWidth(3.0f);
    // Render loop
    while (!glfwWindowShouldClose(window)) {


        // per-frame time logic
        // --------------------
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input(s)
        //modelSelect(window);
        glfwSetKeyCallback(window, modelSelect);
        if(selectedModel == ONE) {
            processInput(window, deltaTime, L8model);
        }
        if(selectedModel == TWO){
            processInput(window, deltaTime, H3model);
        }

        if(selectedModel == RESET){
            L8model = glm::mat4(1.0f);
            H3model = glm::mat4(1.0f);
        }

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Projection and View matrices
        projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        view = camera.getViewMatrix();

        // draw objects
        grid.draw(projection * view * model, renderMode);

        axis.draw(projection * view * model, renderMode);

        will.draw(projection * view * glm::translate(L8model, glm::vec3(43.0f, 0.0f, 49.0f)), renderMode);

        h3.draw(projection * view * H3model, renderMode);


        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate once the window should be closed
    glfwTerminate();
    return 0;
}



