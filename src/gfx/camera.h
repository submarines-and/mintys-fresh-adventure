#pragma once
#include <glm/glm.hpp>

class Camera {
private:
    float LOOK_SENTITIVITY = 0.1f;
    float pitch = 10.0f;
    float zoom = 30.0f;

    // First person only
public:
    float MOVEMENT_SPEED = 50.0f;

    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 front;

    // Third person only
public:
    float angleAroundPlayer = 0.0f;
    glm::vec3 targetPosition = glm::vec3(0, 0, 0);

    // Shared
public:
    glm::vec3 position = glm::vec3(30.0f, 60.0f, 50.0f);
    bool thirdPerson = true;
    float yaw = 0.0f;

public:
    void centerOn(glm::vec3 targetPosition, glm::vec3 rotation);
    void updateCameraAngles();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processMouseMovement(float xOffset, float yOffset);
    void processScroll(float yOffset);
    void processKeyboard(float deltaTime);
};