#include "transform.system.h"
#include "components/transform.component.h"
#include "global.h"
#include <glm/glm.hpp>

void TransformSystem::update(float deltaTime)
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        // don't update static objects
        if (transform.speed <= 0.0f) {
            continue;
        }

        // save old position (which will later be used for collision detection)
        transform.positionLastFrame = transform.position;

        float distance = transform.speed * deltaTime;
        float dx = distance * glm::sin(glm::radians(transform.rotation.y));
        float dz = distance * glm::cos(glm::radians(transform.rotation.y));

        transform.position += glm::vec3(dx, 0.0f, dz);

        // clamp x/z to world edge
        if (transform.position.x <= 0.0f) {
            transform.position.x = 0.0f;
        }
        if (transform.position.z <= 0.0f) {
            transform.position.z = 0.0f;
        }
        if (transform.position.x >= global.world->worldHeight) {
            transform.position.x = global.world->worldHeight;
        }
        if (transform.position.z >= global.world->worldWidth) {
            transform.position.z = global.world->worldWidth;
        }
    }
}