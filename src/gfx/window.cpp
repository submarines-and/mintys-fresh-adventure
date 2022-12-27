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

Window::Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render) : init(init), destroy(destroy), update(update), render(render)
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

    window = glfwCreateWindow(1280, 720, "Mintys fresh adventure", NULL, NULL);
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
    destroy();

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

    init();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        update();
        render();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
