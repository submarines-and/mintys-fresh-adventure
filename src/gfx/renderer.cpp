#include "renderer.h"

Renderer::~Renderer()
{
    for (auto s : shaders) {
        delete s.second;
    }
}

Shader* Renderer::getShader(Shader::ShaderType key)
{
    if (!shaders.count(key)) {
        printf("Shader not loaded %i\n", key);
    }

    return shaders[key];
}

Shader* Renderer::loadShader(Shader::ShaderType key, const char* vertexPath, const char* fragmentPath)
{
    if (!shaders.count(key)) {
        printf("Loading shader %s\n", vertexPath);
        shaders.emplace(key, new Shader(vertexPath, fragmentPath));
    }

    return shaders[key];
}
