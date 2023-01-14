#include "sprite.system.h"
#define STB_IMAGE_IMPLEMENTATION
#include "components/transform.component.h"
#include "gfx/opengl.h"
#include "global.h"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteSystem::SpriteSystem() : shader(Shader("shaders/sprite.vert", "shaders/sprite.frag"))
{
    stbi_set_flip_vertically_on_load(true);

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
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteSystem::entityAdded(Entity entity)
{
    auto& sprite = global.ecs->getComponent<SpriteComponent>(entity);
    glGenTextures(1, &sprite.textureId);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(sprite.textureFilePath, &width, &height, &nrChannels, 4);

    glBindTexture(GL_TEXTURE_2D, sprite.textureId);
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

void SpriteSystem::update(int ticks)
{
    shader.start();

    for (auto entity : entities) {
        auto& sprite = global.ecs->getComponent<SpriteComponent>(entity);
        auto transform = global.ecs->getComponent<TransformComponent>(entity);

        // apply animation
        // speed is number of game loops elapsed per frame
        if (sprite.animationFrames > 0) {
            sprite.atlasOffset.x = ticks / (sprite.animationFrames * 1) % sprite.animationFrames;
        }

        // animation frame
        shader.setInt("image", 0);
        shader.setVec2("atlasSize", sprite.atlasSize);
        shader.setVec2("atlasOffset", sprite.atlasOffset);

        // projection and view
        shader.setMat4("projection", global.camera->getProjectionMatrix());
        shader.setMat4("view", global.camera->getViewMatrix());

        // camera positions for billboarding
        shader.setVec3("cameraRight", global.camera->right);
        shader.setVec3("cameraUp", global.camera->up);

        // position and size
        shader.setVec3("position", transform.position);
        shader.setVec2("size", transform.size);
        shader.setVec2("rotation", sprite.rotation);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite.textureId);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
    }

    shader.stop();
}
