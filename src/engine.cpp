//
// Created by Willi on 7/30/2020.
//

#include <functional>
#include "../include/engine.h"

Engine::Engine(Config config) {
    //do some processing based on config
    this->config = config;
    this->windowWidth = config.windowWidth;
    this->windowHeight = config.windowHeight;

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->
            window = glfwCreateWindow(config.windowWidth, config.windowHeight, "COMP 371 Final Project", nullptr,
                                      nullptr);
    glfwSetWindowUserPointer( window, this);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        //Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return;
    }
    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void Engine::runLoop() {
    double currentFrame = 0.0f;
    double deltaTime = 0.0f;    // time between current frame and last frame
    double lastFrame = 0.0f;

    /*glm::mat4 worldModelMatrix = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(worldCamera.Position, WorldCenter, WorldUp);

    glm::mat4 projection = glm::mat4(1.0f);*/

    //TODO this should not be here
    Mesh mesh{};
    makeMeshFromFile("../resources/models/cube.obj", mesh);

    Shader basicShader = Shader("../resources/shaders/ModelVertexShader.glsl", "../resources/shaders/ModelFragmentShader.glsl");
    Texture dirtTexture = Texture("../resources/textures/dirt.png");
    Model dirtBlock = Model(mesh, dirtTexture);

    glfwSwapInterval( 1 );
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        // --------------------
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(config.fov), (float) windowWidth / (float) windowHeight,
                                                0.1f, 100.0f);

        // rendering stuff here
        // TODO this should be a for loop over entities to draw them
        basicShader.use();
        basicShader.setMat4("view", camera.getViewMatrix());
        basicShader.setMat4("projection", projection);
        dirtBlock.draw(basicShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Engine::processInput(float deltaTime)
{
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

GLFWwindow* Engine::getWindow() const {
    return window;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Engine::mouseCallbackFunc(double xpos, double ypos)
{
    static bool firstMouse = true;
    static float lastX = 1024.0f/2.0f;
    static float lastY = 768.0f/2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset, true);
}