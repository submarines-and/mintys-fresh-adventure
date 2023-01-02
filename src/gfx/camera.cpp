#include "camera.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) : position(position) {}

glm::mat4 Camera::getViewMatrix()
{
    auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(glm::vec3(position.x, position.y, 20.0f), cameraFront + position, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::ortho(0.0f, (float)global.renderer->width, 0.0f, (float)global.renderer->height, 0.0f, 100.0f);
}

void Camera::processKeyboard(CameraDirection direction, float deltaTime)
{
    float velocity = 200.0f * deltaTime;

    if (direction == UP)
        position.y -= velocity;

    if (direction == DOWN)
        position.y += velocity;

    if (direction == LEFT)
        position.x += velocity;

    if (direction == RIGHT)
        position.x -= velocity;

    glm::normalize(position);
}

void Camera::processMouseMovement(float xOffset, float yOffset)
{
}
