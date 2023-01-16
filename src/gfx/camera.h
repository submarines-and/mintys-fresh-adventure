#pragma once
#include <glm/glm.hpp>

class Camera {
private:
    float yaw = 0.0f;
    float pitch = -30.0f;

    float MOVEMENT_SPEED = 50.0f;
    float LOOK_SENTITIVITY = 0.1f;

    // camera Attributes
    glm::vec3 front;

private:
    void updateCameraVectors();

public:
    glm::vec3 position;
    glm::vec3 right;
    glm::vec3 up;

    float zoom = 20.0f;

public:
    Camera(glm::vec3 position);
    void centerOn(glm::vec3 position);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processKeyboard(float deltaTime);
    void processMouseMovement(float xOffset, float yOffset, bool leftButtonHeld, bool rightButtonHeld);
    void processScroll(float yOffset);
};