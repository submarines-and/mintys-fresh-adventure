#include "ai.system.h"
#include "components/transform.component.h"
#include "global.h"

void AISystem::update()
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        if (transform.direction.y == 0.0f) {
            transform.direction.y = 1.0f;
        }
    }
}