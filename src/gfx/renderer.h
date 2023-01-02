#pragma once
#include "shader.h"
#include "sprite.h"
#include <map>

class Renderer {
private:
    unsigned int quadVAO;
    std::map<const char*, Sprite*> sprites;
    std::map<const char*, Shader*> shaders;

public:
    Renderer(int width, int height);
    ~Renderer();

    int width, height;

    Sprite* loadSprite(const char* path);
    Shader* loadShader(const char* shaderKey, const char* vertexPath, const char* fragmentPath);
    Shader* getShader(const char* shaderKey);

    void renderSprite(const char* spritePath, const char* shaderKey, glm::vec2 position, glm::vec2 size, float rotate);
};