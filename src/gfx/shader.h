#pragma once
#include <vector>
#include "opengl.h"

class Shader {
public:
    GLuint id;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void start();
    void stop();
};