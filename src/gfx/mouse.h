#pragma once
#include <glm/glm.hpp>

class Mouse {
public:
    glm::vec2 position;
    glm::vec2 offset;

public:
    glm::vec3 getWorldPosition();
};