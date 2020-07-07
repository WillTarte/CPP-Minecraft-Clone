#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "shader.h"
#include "camera.h"
#include "axis.h"


/// Window size
static const int WIDTH = 800;
static const int HEIGHT = 600;

/// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

/** Method to consume keyboard inputs to control the camera.
 *
 * @param window The currently active window
 * @param deltaTime The time elapsed since the last frame
 */
void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

/** Callback for whenever the window size is changed.
 *
 * @param window The currently active window
 * @param width The new width
 * @param height The new height
 */
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

/** Callback for whenever the scroll wheel is used to control the camera zoom.
 *
 * @param window The currently active window
 * @param xoffset The x offset (unused)
 * @param yoffset The scroll offset
 */
/// glfw: whenever the mouse scroll wheel scrolls, this callback is called
/// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

/// glfw: error callback
/// --------------------
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

/// Method to draw the 100x100 ground grid with it's middle centered at (0,0,0)
//TODO: Eventually move this "object" to it's own class like all other "objects"
void drawGrid() {

    //float vertices[2400];
    std::vector<glm::vec3> vertices;

    for (int i = 0; i <= 100; i++) //draw 100 horizontal and 100 vertical lines - 2 vertices per line
    {
        // horizontal
        vertices.emplace_back(glm::vec3(-50.0f, 0.0f, (float) i - 50.0f));
        vertices.emplace_back(glm::vec3(50.0f, 0.0f, (float) i - 50.0f));

        // vertical
        vertices.emplace_back(glm::vec3((float) i - 50.0f, 0.0f, -50));
        vertices.emplace_back(glm::vec3((float) i - 50.0f, 0.0f, 50.0f));
    }

    // Set up and bind VBO and VAO
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), (void*) 0);
    glEnableVertexAttribArray(0);


    glDrawArrays(GL_LINES, 0, vertices.size());
}

/// Main
int main(int argc, char* argv[]) {
    // Necessary variables

    // timing
    float deltaTime = 0.0f;    // time between current frame and last frame
    float lastFrame = 0.0f;

    // *********************

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetErrorCallback(error_callback);

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

    // Grid shader
    //TODO: Is there a better way to handle shaders (once we have many)
    Shader gridShader = Shader("resources/shaders/GridVertexShader.vs", "resources/shaders/GridFragmentShader.fs");

    // MVP matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    Axis axis = Axis(Shader("resources/shaders/BasicVertexShader.vs", "resources/shaders/BasicFragmentShader.fs"));
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input(s)
        processInput(window, deltaTime);

        gridShader.use();

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update Projection and View matrices
        projection = glm::perspective(glm::radians(camera.Zoom), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
        view = camera.GetViewMatrix();

        // Set the MVP matrices in the ground grid shader
        //TODO: Better way to handle this?
        gridShader.setMat4("Model", model);
        gridShader.setMat4("View", view);
        gridShader.setMat4("Projection", projection);

        drawGrid();



        axis.Draw(model,view,projection);



// Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //Terminate once the window should be closed
    glfwTerminate();
    return 0;
}



