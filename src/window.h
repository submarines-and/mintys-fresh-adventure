#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdlib.h>
#include <stdio.h>

class Window {
private:
    GLFWwindow* window;

    static void errorCallback(int code, const char* description)
    {
        printf("GLFW error %d: %s\n", code, description);
    }

public:
    Window()
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

        window = glfwCreateWindow(1000, 1000, "Mintys fresh adventure", NULL, NULL);
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

    ~Window()
    {
        printf("Destroy\n");
        if (window) {
            glfwDestroyWindow(window);
        }

        glfwTerminate();
    }

    void loop()
    {
        if (!window) {
            return;
        }

        while (!glfwWindowShouldClose(window)) {

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
};