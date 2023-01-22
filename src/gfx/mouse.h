#pragma once
#include <glm/glm.hpp>

class Mouse {
public:
    glm::vec2 position;
    glm::vec2 offset;

private:
    bool isUnderGround(glm::vec3 position);
    bool intersectionInRange(float start, float end, glm::vec3 ray);
    glm::vec3 binarySearch(int count, float start, float finish, glm::vec3 ray);
    glm::vec3 getPointOnRay(glm::vec3 ray, float distance);
    glm::vec3 castRay();

public:
    glm::vec3 getWorldPosition();
};