#include "window.h"
#include "global.h"
#include <random>
#include <stdio.h>
#include <stdlib.h>

static void errorCallback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
#pragma unused(window)
#pragma unused(xOffset)
    global.camera->processScroll(yOffset);
}

void onResize(GLFWwindow* window, int width, int height)
{
#pragma unused(window)

    glViewport(0, 0, width, height);
    global.width = width;
    global.height = height;
}

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
#pragma unused(scancode)
#pragma unused(mods)

    if (key < 0) {
        return;
    }

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    // wireframe
    if (key == GLFW_KEY_P) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (key == GLFW_KEY_O) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // camera mode (third, first person)
    if (key == GLFW_KEY_3) {
        global.camera->thirdPerson = true;
    }
    else if (key == GLFW_KEY_1) {
        global.camera->thirdPerson = false;
    }

    switch (action) {
    case GLFW_PRESS:
        global.keys[key] = true;
        break;
    case GLFW_RELEASE:
        global.keys[key] = false;
        break;
    default:
        break;
    }
}

void Window::handleMouseMovement(GLFWwindow* window)
{
    GLdouble xposIn, yposIn;
    glfwGetCursorPos(window, &xposIn, &yposIn);

    global.mouse->position.x = static_cast<float>(xposIn);
    global.mouse->position.y = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = global.mouse->position.x;
        lastY = global.mouse->position.y;
        firstMouse = false;
    }

    global.mouse->offset.x = global.mouse->position.x - lastX;
    global.mouse->offset.y = lastY - global.mouse->position.y; // reversed since y-coordinates go from bottom to top

    lastX = global.mouse->position.x;
    lastY = global.mouse->position.y;

    // flag mouse button
    global.keys[GLFW_MOUSE_BUTTON_LEFT] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    global.keys[GLFW_MOUSE_BUTTON_RIGHT] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (global.keys[GLFW_MOUSE_BUTTON_LEFT] || global.keys[GLFW_MOUSE_BUTTON_RIGHT]) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    global.camera->processMouseMovement(global.mouse->offset.x, global.mouse->offset.y);
}

Window::Window(int width, int height, WindowFunction init, WindowFunction destroy, WindowFunctionWithTime update, WindowFunction render)
    : init(init),
      destroy(destroy),
      render(render),
      update(update),
      lastX(width / 2.0f),
      lastY(height / 2.0f)
{

    // seed random for the entire app
    srand((unsigned)time(NULL));

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

    window = glfwCreateWindow(width, height, "Mintys fresh adventure", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, handleInput);

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    // main init
    init();

    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleMouseMovement(window);
        global.camera->processKeyboard(deltaTime);
        update(deltaTime);

        glClearColor(255, 255, 255, 255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
        global.ticks++;
    }

    // main destroy
    destroy();
}
