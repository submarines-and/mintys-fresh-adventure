#pragma once
#include "camera.h"
#include "opengl.h"

// callback type for main loop
using WindowFunction = void (*)();
using WindowFunctionWithTime = void (*)(float deltaTime);

class Window {
private:
    GLFWwindow* window;
    WindowFunction init, destroy;
    WindowFunctionWithTime update, render;

    void handleMouseMovement(GLFWwindow* window);

    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;

public:
    Window(int width, int height, WindowFunction init, WindowFunction destroy, WindowFunctionWithTime update, WindowFunctionWithTime render);
    ~Window();

    void loop();
};