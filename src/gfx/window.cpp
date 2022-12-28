#include "window.h"
#include <stdlib.h>
#include <stdio.h>
#include "global.h"

static void errorCallback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::handleInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        global.camera.processKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        global.camera.processKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        global.camera.processKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        global.camera.processKeyboard(RIGHT, deltaTime);
}

void Window::handleMouseMovement(GLFWwindow* window)
{
    GLdouble xposIn, yposIn;
    glfwGetCursorPos(window, &xposIn, &yposIn);

    float xPos = static_cast<float>(xposIn);
    float yPos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    global.camera.processMouseMovement(xoffset, yoffset);
}

void Window::handleScroll(GLFWwindow* window)
{
    // https://github.com/glfw/glfw/issues/356
    // global.camera.processMouseScroll(static_cast<float>(yoffset));
}

Window::Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render)
    : init(init),
      destroy(destroy),
      update(update),
      render(render)
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        printf("Failed to initializing GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(1280, 720, "Mintys fresh adventure", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load GLAD\n");
        return;
    }

    glfwSwapInterval(1);
}

Window::~Window()
{

    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}

void Window::loop()
{
    if (!window) {
        return;
    }

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_DEPTH_TEST);
    init();

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(window);
        handleMouseMovement(window);
        handleScroll(window);
        update();

        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
}
