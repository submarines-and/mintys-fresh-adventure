#pragma once
#include "shader.h"
#include "sprite.h"
#include <map>

class Renderer {
private:
    unsigned int quadVAO;
    std::map<const char*, Sprite*> sprites;
    std::map<Shader::ShaderType, Shader*> shaders;

public:
    Renderer();
    ~Renderer();

    Sprite* loadSprite(const char* path);
    Shader* loadShader(Shader::ShaderType shaderKey, const char* vertexPath, const char* fragmentPath);
    Shader* getShader(Shader::ShaderType shaderKey);

    void renderSprite(const char* spritePath, Shader::ShaderType shaderKey, glm::vec2 atlasSize, glm::vec2 atlasOffset, glm::vec2 worldPosition, glm::vec2 worldSize, float rotation);

    void prepareTiles(std::vector<glm::mat4> transformations, std::vector<glm::vec2> textureOffsets, std::vector<glm::vec4> colors);
    void renderTiles();
};