#pragma once
#include "opengl.h"

// callback type for main loop
using WindowFunction = void (*)();

class Window {
private:
    GLFWwindow* window;
    WindowFunction init, destroy, update, render;

public:
    Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);
    ~Window();

    void loop();
};