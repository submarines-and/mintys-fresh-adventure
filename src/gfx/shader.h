#pragma once
#include <vector>
#include "opengl.h"

class Shader {
private:
    GLuint shaderId;

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void start();
    void stop();
};