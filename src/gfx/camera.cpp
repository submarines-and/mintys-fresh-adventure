#include "camera.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) : position(position)
{
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::ortho(0.0f, (float)global.width, 0.0f, (float)global.height, -100.0f, 100.0f);
}

void Camera::processKeyboard(CameraDirection direction, float deltaTime)
{
    float velocity = 1000.0f * deltaTime;

    if (direction == UP)
        position.y += velocity;

    if (direction == DOWN)
        position.y -= velocity;

    if (direction == LEFT)
        position.x -= velocity;

    if (direction == RIGHT)
        position.x += velocity;

    glm::normalize(position);

    // clamp
    auto maxX = global.world.width - global.width;
    auto maxY = global.world.height - global.height;

    position.x = position.x < 0 ? 0 : (position.x > maxX ? maxX : position.x);
    position.y = position.y < 0 ? 0 : (position.y > maxY ? maxY : position.y);
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool leftButtonHeld, bool rightButtonHeld)
{
    if (!rightButtonHeld) {
        return;
    }

    const float SENSITIVITY = 0.1f;
    xOffset *= SENSITIVITY;
    yOffset *= SENSITIVITY;

    // yaw += xOffset;
    pitch += yOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // also re-calculate the Right and Up vector
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
}