#pragma once
#include "opengl.h"

class Window {
private:
    GLFWwindow* window;
    void processInput(GLFWwindow* window);

public:
    Window(int height, int width);
    ~Window();

    void loop();
};