#pragma once
#include <glm/glm.hpp>

enum CameraDirection {
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

class Camera {
public:
    glm::vec3 position;
    float rotation = 0.0f;

    Camera(glm::vec3 position);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processKeyboard(CameraDirection direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool left, bool right);
};