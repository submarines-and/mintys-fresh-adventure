#include "ai.system.h"
#include "components/transform.component.h"
#include "global.h"

void AISystem::update()
{
    for (auto entity : entities) {
        auto& transformComponent = global.ecs->getComponent<TransformComponent>(entity);
        transformComponent.direction.y = 1.0f;
    }
}