#include "window.h"
#include <stdlib.h>
#include <stdio.h>

static void errorCallback(int code, const char* description)
{
    printf("GLFW error %d: %s\n", code, description);
}

void Window::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

Window::Window(int height, int width)
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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(height, width, "Mintys fresh adventure", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        return;
    }

    glfwMakeContextCurrent(window);

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

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
