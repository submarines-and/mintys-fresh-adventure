#pragma once
#include <glm/glm.hpp>

class Camera {
private:
    float yaw = 0.0f;
    float pitch = 10.0f;
    float zoom = 20.0f;
    float angleAroundPlayer = 0.0f;
    float LOOK_SENTITIVITY = 0.1f;

    /** First person variables*/
public:
    float MOVEMENT_SPEED = 50.0f;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 front;

    /** Shared */
public:
    glm::vec3 position = glm::vec3(30.0f, 60.0f, 50.0f);
    glm::vec3 targetPosition = glm::vec3(0, 0, 0);

    /** Following player */
    bool thirdPerson = true;

public:
    void centerOn(glm::vec3 targetPosition, glm::vec3 rotation);
    void updateFirstPersonView();

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processMouseMovement(float xOffset, float yOffset);
    void processScroll(float yOffset);
    void processKeyboard(float deltaTime);
};