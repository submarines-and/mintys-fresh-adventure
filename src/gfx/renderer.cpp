#include "renderer.h"

Renderer::~Renderer()
{
    for (auto s : shaders) {
        delete s.second;
    }

    for (auto s : sprites) {
        delete s.second;
    }
}

Sprite* Renderer::getSprite(Sprite::SpriteType key)
{
    if (!sprites.count(key)) {
        printf("Sprite not loaded %i\n", key);
    }

    return sprites[key];
}

Shader* Renderer::getShader(Shader::ShaderType key)
{
    if (!shaders.count(key)) {
        printf("Shader not loaded %i\n", key);
    }

    return shaders[key];
}

Sprite* Renderer::loadSprite(Sprite::SpriteType key, const char* spritePath)
{
    if (!sprites.count(key)) {
        printf("Loading sprite %s\n", spritePath);
        sprites.emplace(key, new Sprite(spritePath));
    }

    return sprites[key];
}

Shader* Renderer::loadShader(Shader::ShaderType key, const char* vertexPath, const char* fragmentPath)
{
    if (!shaders.count(key)) {
        printf("Loading shader %s\n", vertexPath);
        shaders.emplace(key, new Shader(vertexPath, fragmentPath));
    }

    return shaders[key];
}
