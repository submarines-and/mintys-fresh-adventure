#pragma once
#include "camera.h"
#include "opengl.h"

// callback type for main loop
using WindowFunction = void (*)();

class Window {
private:
    GLFWwindow* window;
    WindowFunction init, destroy, update, render;

    void handleInput(GLFWwindow* window);
    void handleMouseMovement(GLFWwindow* window);
    void handleScroll(GLFWwindow* window);

    float deltaTime = 0.0f;
    float lastX = 1280 / 2.0f;
    float lastY = 720 / 2.0f;
    bool firstMouse = true;

public:
    Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);
    ~Window();

    void loop();
};