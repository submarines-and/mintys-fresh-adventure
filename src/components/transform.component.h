#pragma once
#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    float speed = 0.0f;
    float jumpHeight = 5.0f;

    /** Override any Y direction until unit hits ground */
    bool isFalling = false;

    /** For collision */
    glm::vec3 positionLastFrame = glm::vec3(0.0f, 0.0f, 0.0f);
};