#include "renderer.h"
#include "global.h"
#include "opengl.h"
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer()
{
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
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
    glBindVertexArray(0);

    shader->stop();
}

void Renderer::renderTiles(TileAtlas atlas, std::vector<Tile> tiles)
{
    auto shader = getShader(atlas.shaderKey);
    shader->start();

    shader->setMat4("projection", global.camera->getProjectionMatrix());
    shader->setMat4("view", global.camera->getViewMatrix());
    shader->setInt("image", 0);
    shader->setVec2("atlasSize", atlas.size);

    glActiveTexture(GL_TEXTURE0);
    auto sprite = loadSprite(atlas.spritePath);
    sprite->bind();
    glBindVertexArray(quadVAO);

    for (auto tile : tiles) {
        shader->setVec2("offset", tile.getOffset(atlas));

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(tile.position, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.5f * tile.size.x, 0.5f * tile.size.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(tile.rotation + 180), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(-0.5f * tile.size.x, -0.5f * tile.size.y, 0.0f));
        transform = glm::scale(transform, glm::vec3(tile.size, 1.0f));

        shader->setMat4("transform", transform);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    glBindVertexArray(0);

    shader->stop();
}
