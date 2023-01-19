#include "input.system.h"
#include "components/sprite.component.h"
#include "components/transform.component.h"
#include "global.h"

void InputSystem::update()
{
    // this will always have 1 entry, the currently controlled player
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);
        auto& sprite = global.ecs->getComponent<SpriteComponent>(entity);

        if (global.keys[GLFW_KEY_W]) {
            //  sprite.rotation.y = 1;
        }

        if (global.keys[GLFW_KEY_S]) {
            //  sprite.rotation.y = -1;
        }

        if (global.keys[GLFW_KEY_A]) {
            sprite.rotation.x = -1;
        }

        if (global.keys[GLFW_KEY_D]) {
            sprite.rotation.x = 1;
        }

        // jumping
        if (global.keys[GLFW_KEY_SPACE]) {
        }

        global.camera->centerOn(transform.position);
    }
}