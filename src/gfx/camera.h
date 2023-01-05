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
    float yaw = 90.0f;
    float pitch = 0.0f;

    float MOVEMENT_SPEED = 50.0f;
    float LOOK_SENTITIVITY = 0.1f;

    // camera Attributes
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;

private:
    void updateCameraVectors();

public:
    glm::vec3 position;
    float zoom = 20.0f;

public:
    Camera(glm::vec3 position);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processKeyboard(CameraDirection direction, float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool leftButtonHeld, bool rightButtonHeld);
    void processScroll(float yOffset);
};