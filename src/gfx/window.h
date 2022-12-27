#pragma once
#include "opengl.h"

class Window {
    typedef void (*WindowFunction)();

private:
    GLFWwindow* window;
    WindowFunction init, destroy, update, render;

public:
    Window(WindowFunction init, WindowFunction destroy, WindowFunction update, WindowFunction render);
    ~Window();

    void loop();
};