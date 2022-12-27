#pragma once
#include <vector>
#include "opengl.h"

struct Shader {
    GLuint id;
};

class ShaderLoader {
private:
    std::vector<GLuint> shaders;

public:
    ~ShaderLoader();
    Shader load(const char* vertexPath, const char* fragmentPath);
};