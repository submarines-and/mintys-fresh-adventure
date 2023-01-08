#include "input.system.h"
#include "components/transform.component.h"
#include "global.h"

void InputSystem::update()
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        // reset each loop
        transform.direction = glm::vec3(0.0f);

        if (global.keys[GLFW_KEY_W])
            transform.direction.x = 1;

        if (global.keys[GLFW_KEY_S])
            transform.direction.x = -1;

        if (global.keys[GLFW_KEY_A])
            transform.direction.z = -1;

        if (global.keys[GLFW_KEY_D])
            transform.direction.z = 1;

        // fix horizontal movement
        if (transform.direction.x != 0.0f || transform.direction.z != 0.0f)
            transform.direction = glm::normalize(transform.direction);
    }
}