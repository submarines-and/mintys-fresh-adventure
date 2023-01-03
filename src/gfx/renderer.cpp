#include "renderer.h"
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

Shader* Renderer::loadShader(const char* shaderKey, const char* vertexPath, const char* fragmentPath)
{
    if (!shaders.count(shaderKey)) {
        shaders.emplace(shaderKey, new Shader(vertexPath, fragmentPath));
    }

    return shaders[shaderKey];
}

Shader* Renderer::getShader(const char* shaderKey)
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

void Renderer::renderSprite(const char* spritePath, const char* shaderKey, glm::vec2 position, glm::vec2 size, float rotate)
{
    auto shader = getShader(shaderKey);
    shader->start();

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(position, 0.0f));
    //   transform = glm::translate(transform, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    //   transform = glm::rotate(transform, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    //  transform = glm::translate(transform, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    transform = glm::scale(transform, glm::vec3(size, 1.0f));

    shader->setMat4("transform", transform);

    glActiveTexture(GL_TEXTURE0);
    auto sprite = loadSprite(spritePath);
    sprite->bind();

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
