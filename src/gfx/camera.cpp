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

void Camera::processKeyboard(CameraDirection direction, float deltaTime)
{
    float velocity = MOVEMENT_SPEED * deltaTime;

    if (direction == UP)
        position += front * velocity;

    if (direction == DOWN)
        position -= front * velocity;

    if (direction == LEFT)
        position -= right * velocity;

    if (direction == RIGHT)
        position += right * velocity;

    glm::normalize(position);
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool leftButtonHeld, bool rightButtonHeld)
{
    // only move when mouse held
    if (!rightButtonHeld) {
        return;
    }

    xOffset *= LOOK_SENTITIVITY;
    yOffset *= LOOK_SENTITIVITY;

    // always add to y
    pitch += yOffset;
    yaw += xOffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::processScroll(float yOffset)
{
    yOffset *= LOOK_SENTITIVITY;

    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yOffset;

    if (zoom <= 1.0f)
        zoom = 1.0f;

    if (zoom >= 45.0f)
        zoom = 45.0f;
}
