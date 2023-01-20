#include "camera.h"
#include "gfx/opengl.h"
#include "global.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::centerOn(glm::vec3 targetPosition, glm::vec3 rotation)
{
    if (!thirdPerson) {
        return;
    }

    auto horizontalDistance = zoom * glm::cos(glm::radians(pitch));
    auto verticalDistance = zoom * glm::sin(glm::radians(pitch));

    float theta = rotation.y + angleAroundPlayer;
    float offsetX = horizontalDistance * glm::sin(glm::radians(theta));
    float offsetZ = horizontalDistance * glm::cos(glm::radians(theta));

    position.x = targetPosition.x - offsetX;
    position.z = targetPosition.z - offsetZ;
    position.y = targetPosition.y + verticalDistance;

    yaw = 180 - (rotation.y + angleAroundPlayer);
    this->targetPosition = targetPosition;
}

void Camera::updateFirstPersonView()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getViewMatrix()
{
    if (!thirdPerson) {
        return glm::lookAt(position, position + front, up);
    }

    return glm::lookAt(position, targetPosition, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(zoom), (float)global.width / (float)global.height, 0.001f, 1000000.0f);
}

void Camera::processMouseMovement(float xOffset, float yOffset)
{
    // rotate camera when mouse buttons are held
    if (!global.keys[GLFW_MOUSE_BUTTON_LEFT] && !global.keys[GLFW_MOUSE_BUTTON_RIGHT]) {
        return;
    }

    xOffset *= LOOK_SENTITIVITY;
    yOffset *= LOOK_SENTITIVITY;

    if (thirdPerson) {
        pitch -= yOffset;

        // right button controls player, which indirectly controls camera as well
        if (!global.keys[GLFW_MOUSE_BUTTON_RIGHT]) {
            angleAroundPlayer -= xOffset;
        }
    }
    else {
        pitch += yOffset;
        yaw += xOffset;
        updateFirstPersonView();
    }
}

void Camera::processScroll(float yOffset)
{
    yOffset *= LOOK_SENTITIVITY;

    if (zoom >= 1.0f && zoom <= 50.0f)
        zoom -= yOffset;

    if (zoom <= 1.0f)
        zoom = 1.0f;

    if (zoom >= 80.0f)
        zoom = 80.0f;
}

void Camera::processKeyboard(float deltaTime)
{
    // check camera mode
    if (thirdPerson) {
        return;
    }

    float velocity = MOVEMENT_SPEED * deltaTime;

    if (global.keys[GLFW_KEY_W])
        position += front * velocity;

    if (global.keys[GLFW_KEY_S])
        position -= front * velocity;

    if (global.keys[GLFW_KEY_A])
        position -= right * velocity;

    if (global.keys[GLFW_KEY_D])
        position += right * velocity;

    // clamp to ground
    auto heightAtPosition = global.world->getTerrainHeight(position);
    if (position.y < heightAtPosition) {
        position.y = heightAtPosition;
    }

    updateFirstPersonView();
}