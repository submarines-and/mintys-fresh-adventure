#include "transform.system.h"
#include "components/transform.component.h"
#include "global.h"

void TransformSystem::update(float deltaTime)
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        if (transform.speed <= 0.0f) {
            continue;
        }

        transform.positionLastFrame = transform.position;
        transform.position += transform.direction * transform.speed * deltaTime;

        transform.position.y = global.world->getTerrainHeight(transform.position, transform.size);
    }
}