#pragma once
#include "shader.h"
#include "sprite.h"
#include <map>

class Renderer {
private:
    std::map<Sprite::SpriteType, Sprite*> sprites;
    std::map<Shader::ShaderType, Shader*> shaders;

public:
    ~Renderer();

    Sprite* getSprite(Sprite::SpriteType key);
    Shader* getShader(Shader::ShaderType key);

    Sprite* loadSprite(Sprite::SpriteType key, const char* spritePath);
    Shader* loadShader(Shader::ShaderType key, const char* vertexPath, const char* fragmentPath);
};