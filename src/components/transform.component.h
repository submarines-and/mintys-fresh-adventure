#pragma once
#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    /** Speed when moving or jumping */
    float speed = 0.0f;

    float currentSpeed = 0.0f;
    float currentUpwardSpeed = 0.0f;

    /** Set to true if you want to jump */
    bool jump = false;
    bool isJumping = false;

    /** For collision */
    glm::vec3 positionLastFrame = glm::vec3(0.0f, 0.0f, 0.0f);
};