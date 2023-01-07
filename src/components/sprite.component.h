#pragma once
#include <glm/glm.hpp>

struct SpriteComponent {
    unsigned int textureId;
    const char* textureFilePath;
    glm::vec2 atlasSize = glm::vec2(1, 1);
    glm::vec2 atlasOffset = glm::vec2(0, 0);

    int animationFrames = 0;
    int animationSpeed = 50;
};