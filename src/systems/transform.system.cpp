#include "transform.system.h"
#include "components/transform.component.h"
#include "global.h"

void TransformSystem::update(float deltaTime)
{
    for (auto e : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(e);
        transform.position += transform.velocity * deltaTime;
    }
}