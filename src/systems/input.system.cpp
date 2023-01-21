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

    // apply rotation
    if (global.keys[GLFW_MOUSE_BUTTON_RIGHT]) {

        // 1 time line up player with current camera ange
        // might move this to a "on button down" trigger instead
        if (!cameraSynced) {
            cameraSynced = true;
            transform.rotation.y = 180 - global.camera->yaw;
            global.camera->angleAroundPlayer = 0.0f;
        }

        // rotate normally
        transform.rotation.y -= global.mouse->offset.x * 0.5f;
    }
    else {
        // reset sync
        cameraSynced = false;
    }

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
        transform.currentStrafeSpeed = transform.speed;
    }
    else if (global.keys[GLFW_KEY_D]) {
        transform.currentStrafeSpeed = -transform.speed;
    }
    else {
        transform.currentStrafeSpeed = 0.0f;
    }

    // jumping
    if (global.keys[GLFW_KEY_SPACE]) {
        transform.jump = true;
    }

    // center on controlled unit
    global.camera->centerOn(transform.position, transform.rotation);
}