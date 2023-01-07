#pragma once
#include "shader.h"
#include <map>

class Renderer {
private:
    std::map<Shader::ShaderType, Shader*> shaders;

public:
    ~Renderer();

    Shader* getShader(Shader::ShaderType key);
    Shader* loadShader(Shader::ShaderType key, const char* vertexPath, const char* fragmentPath);
};