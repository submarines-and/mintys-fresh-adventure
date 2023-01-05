#pragma once
#include "camera.h"
#include "opengl.h"

// callback type for main loop
using WindowFunction = void (*)();

class Window {
private:
    GLFWwindow* window;
    WindowFunction init, update, render, destroy;

    void handleInput(GLFWwindow* window);
    void handleMouseMovement(GLFWwindow* window);

    float deltaTime = 0.0f;
    int ticks = 0;

    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;

public:
    Window(int width, int height, WindowFunction init, WindowFunction update, WindowFunction render, WindowFunction destroy);
    ~Window();

    void loop();
};