#pragma once
#include "opengl.h"

class Window {
    typedef void (*WindowFunction)();

private:
    void processInput(GLFWwindow* window);
    GLFWwindow* window;
    WindowFunction init, destroy, update, render;

public:
    Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);
    ~Window();

    void loop();
};