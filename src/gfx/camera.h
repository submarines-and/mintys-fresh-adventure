#pragma once
#include <glm/glm.hpp>

enum CameraDirection {
    UP,
    DOWN,
    RIGHT,
    LEFT,
};

class Camera {
private:
    float yaw = 0.0f;
    float pitch = -45.0f;

    float MOVEMENT_SPEED = 100.0f;
    float LOOK_SENTITIVITY = 0.1f;

    // camera Attributes
    glm::vec3 front;

private:
    void updateCameraVectors();

public:
    glm::vec3 position;
    glm::vec3 right;
    glm::vec3 up;

    float zoom = 30.0f;

public:
    Camera(glm::vec3 position);

    glm::mat4 getViewMatrix();

    void processKeyboard(CameraDirection direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool leftButtonHeld, bool rightButtonHeld);
    void processScroll(float yOffset);
};