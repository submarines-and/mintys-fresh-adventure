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
    return glm::ortho(0.0f, (float)global.width, 0.0f, (float)global.height, 0.0f, 100.0f);
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

    /*
    // clamp
    auto lastTile = global.world.tiles.back();
    auto maxX = lastTile.position.x * 32 - global.width;
    auto maxY = lastTile.position.y * 32 - global.height;

    position.x = position.x < 0 ? 0 : (position.x > maxX ? maxX : position.x);
    position.y = position.y < 0 ? 0 : (position.y > maxY ? maxY : position.y);
    */
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool left, bool right)
{
    if (right) {
        rotation += xOffset;
    }
}
