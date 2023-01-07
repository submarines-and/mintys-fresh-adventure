#include "sprite.h"
#define STB_IMAGE_IMPLEMENTATION
#include "global.h"
#include "opengl.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite(const char* filepath)
{
    loadFile(filepath);
    setupVao();

    global.renderer->loadShader(Shader::SPRITE, "shaders/sprite.vert", "shaders/sprite.frag");
}

void Sprite::loadFile(const char* filepath)
{
    // stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &id);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 4);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // and finally free image data
    stbi_image_free(data);
}

void Sprite::setupVao()
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
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Sprite::~Sprite()
{
    printf("Deleting sprite %i\n", id);
    glDeleteTextures(1, &id);
    glDeleteVertexArrays(1, &vao);
}

void Sprite::render()
{
    auto shader = global.renderer->getShader(Shader::SPRITE);
    shader->start();

    // animation frame
    shader->setInt("image", 0);
    shader->setVec2("atlasSize", atlasSize);
    shader->setVec2("offset", atlasOffset);

    // projection
    glm::mat4 projection = glm::perspective(glm::radians(global.camera->zoom), (float)global.width / (float)global.height, 0.1f, 1000.0f);
    shader->setMat4("projection", projection);
    shader->setMat4("view", global.camera->getViewMatrix());

    // world position
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, worldPosition);

    // optional rotation
    if (rotation > 0.0f) {
        transform = glm::translate(transform, glm::vec3(0.5f * worldSize.x, 0.5f * worldSize.y, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(-0.5f * worldSize.x, -0.5f * worldSize.y, 0.0f));
    }

    // scale
    transform = glm::scale(transform, glm::vec3(worldSize, 1.0f));
    shader->setMat4("transform", transform);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    shader->stop();
}