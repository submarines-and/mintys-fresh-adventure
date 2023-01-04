#include "renderer.h"
#include "global.h"
#include "opengl.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
{
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

    unsigned int vbo;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Renderer::~Renderer()
{
    for (auto s : shaders) {
        delete s.second;
    }

    for (auto s : sprites) {
        delete s.second;
    }
}

Shader* Renderer::loadShader(Shader::ShaderType shaderKey, const char* vertexPath, const char* fragmentPath)
{
    if (!shaders.count(shaderKey)) {
        shaders.emplace(shaderKey, new Shader(vertexPath, fragmentPath));
    }

    return shaders[shaderKey];
}

Shader* Renderer::getShader(Shader::ShaderType shaderKey)
{
    return shaders[shaderKey];
}

Sprite* Renderer::loadSprite(const char* path)
{
    if (!sprites.count(path)) {
        sprites.emplace(path, new Sprite(path));
    }

    return sprites[path];
}

void Renderer::renderSprite(const char* spritePath, Shader::ShaderType shaderKey, glm::vec2 atlasSize, glm::vec2 atlasOffset, glm::vec2 worldPosition, glm::vec2 worldSize, float rotation)
{
    auto shader = getShader(shaderKey);
    shader->start();

    shader->setMat4("projection", global.camera->getProjectionMatrix());
    shader->setMat4("view", global.camera->getViewMatrix());
    shader->setInt("image", 0);
    shader->setVec2("atlasSize", atlasSize);
    shader->setVec2("offset", atlasOffset);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(worldPosition, 0.0f));
    transform = glm::translate(transform, glm::vec3(0.5f * worldSize.x, 0.5f * worldSize.y, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation + 180), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-0.5f * worldSize.x, -0.5f * worldSize.y, 0.0f));

    transform = glm::scale(transform, glm::vec3(worldSize, 1.0f));

    shader->setMat4("transform", transform);

    glActiveTexture(GL_TEXTURE0);
    auto sprite = loadSprite(spritePath);
    sprite->bind();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    shader->stop();
}

glm::mat4 debugMat4()
{
    glm::vec2 worldPosition(0, 0);
    glm::vec2 worldSize(32, 32);
    float rotation = 0.0f;

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(worldPosition, 0.0f));
    transform = glm::translate(transform, glm::vec3(0.5f * worldSize.x, 0.5f * worldSize.y, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation + 180), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, glm::vec3(-0.5f * worldSize.x, -0.5f * worldSize.y, 0.0f));
    transform = glm::scale(transform, glm::vec3(worldSize, 1.0f));

    return transform;
}

void Renderer::prepareTiles(std::vector<glm::mat4> transformations, std::vector<glm::vec2> textureOffsets)
{
    // create buffers
    unsigned int textureOffsetVbo;
    unsigned int transformationVbo;
    glGenBuffers(1, &textureOffsetVbo);
    glGenBuffers(1, &transformationVbo);

    // bind array
    glBindVertexArray(quadVAO);

    // offsets
    glBindBuffer(GL_ARRAY_BUFFER, textureOffsetVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textureOffsets.size(), &textureOffsets[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glVertexAttribDivisor(1, 1);

    // transformations (4 slots)
    glBindBuffer(GL_ARRAY_BUFFER, transformationVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transformations.size(), &transformations[0], GL_STATIC_DRAW);

    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
        glVertexAttribDivisor(2 + i, 1);
    }

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderTiles(TileAtlas atlas, int tileCount)
{
    auto shader = getShader(atlas.shaderKey);
    shader->start();

    shader->setMat4("projection", global.camera->getProjectionMatrix());
    shader->setMat4("view", global.camera->getViewMatrix());
    shader->setInt("image", 0);
    shader->setVec2("atlasSize", atlas.size);
    // shader->setMat4("transform", debugMat4());

    glActiveTexture(GL_TEXTURE0);
    auto sprite = loadSprite(atlas.spritePath);
    sprite->bind();

    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, tileCount);

    glBindVertexArray(0);
    shader->stop();
}
