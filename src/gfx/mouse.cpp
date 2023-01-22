#include "mouse.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

bool Mouse::isUnderGround(glm::vec3 position)
{
    auto height = global.world->getTerrainHeight(position);
    return position.y < height;
}

bool Mouse::intersectionInRange(float start, float end, glm::vec3 ray)
{
    glm::vec3 startPoint = getPointOnRay(ray, start);
    glm::vec3 endPoint = getPointOnRay(ray, end);

    return !isUnderGround(startPoint) && isUnderGround(endPoint);
}

glm::vec3 Mouse::getPointOnRay(glm::vec3 ray, float distance)
{
    glm::vec3 cameraPosition = global.camera->position;
    glm::vec3 start(cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glm::vec3 scaledRay(ray.x * distance, ray.y * distance, ray.z * distance);

    return start + scaledRay;
}

glm::vec3 Mouse::binarySearch(int count, float start, float finish, glm::vec3 ray)
{
    float half = start + ((finish - start) / 2.0f);

    // count is arbitrary
    if (count >= 200) {
        return getPointOnRay(ray, half);
    }

    // search
    if (intersectionInRange(start, half, ray)) {
        return binarySearch(count + 1, start, half, ray);
    }
    else {
        return binarySearch(count + 1, half, finish, ray);
    }
}

glm::vec3 Mouse::castRay()
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

glm::vec3 Mouse::getWorldPosition()
{
    auto ray = castRay();
    return binarySearch(0, 0, 300, ray);
}
