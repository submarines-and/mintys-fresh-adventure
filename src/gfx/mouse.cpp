#include "mouse.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 Mouse::getWorldPosition()
{
    // normalized device coordinates
    float normalizedX = (2.0f * position.x) / global.width - 1.0f;
    float normalizedY = (2.0f * position.y) / global.height - 1.0f;

    // clip coordinates
    glm::vec4 clipCoordinates(normalizedX, normalizedY, -1.0f, 1.0f);

    // eye coordinates
    glm::vec4 eyeCoords = glm::inverse(global.camera->getProjectionMatrix()) * clipCoordinates;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

    // world coordinates
    glm::vec4 rayWorld = glm::inverse(global.camera->getViewMatrix()) * eyeCoords;
    glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    mouseRay = glm::normalize(mouseRay);

    return mouseRay;
}