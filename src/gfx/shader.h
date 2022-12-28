#pragma once
#include <vector>
#include "opengl.h"
#include <glm/glm.hpp>

class Shader {
public:
    GLuint id;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void start();
    void stop();

    void setMat4(const char* name, const glm::mat4& mat) const;
};