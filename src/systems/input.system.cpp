#include "input.system.h"
#include "components/sprite.component.h"
#include "components/transform.component.h"
#include "global.h"

void InputSystem::update()
{
    if (entities.size() == 0) {
        return;
    }

    // this system will always have 1 entry - the currently controlled unit
    auto entity = *next(entities.begin(), 0);
    auto& transform = global.ecs->getComponent<TransformComponent>(entity);

    if (global.keys[GLFW_KEY_W]) {
        transform.currentSpeed = transform.speed;
    }
    else if (global.keys[GLFW_KEY_S]) {
        transform.currentSpeed = -transform.speed;
    }
    else {
        transform.currentSpeed = 0.0f;
    }

    if (global.keys[GLFW_KEY_A]) {
        transform.currentTurnSpeed = transform.speed;
    }
    else if (global.keys[GLFW_KEY_D]) {
        transform.currentTurnSpeed = -transform.speed;
    }
    else {
        transform.currentTurnSpeed = 0.0f;
    }

    // jumping
    if (global.keys[GLFW_KEY_SPACE]) {
        transform.jump = true;
    }

    // center on controlled unit
    global.camera->centerOn(transform.position, transform.rotation);
}