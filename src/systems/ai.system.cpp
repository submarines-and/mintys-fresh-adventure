#include "ai.system.h"
#include "components/ai.component.h"
#include "components/transform.component.h"
#include "global.h"
#include "util/math.h"

void AISystem::update(float deltaTime)
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);
        auto& ai = global.ecs->getComponent<AIComponent>(entity);

        if (ai.distanceMoved == 0.0f) {
            // pick new direction
            transform.direction.x = Math::random(-1, 1);
            transform.direction.z = Math::random(-1, 1);
            ai.distanceMoved += deltaTime;
        }
        else if (ai.distanceMoved < 10.0f) {
            // move a few paces
            ai.distanceMoved += transform.speed * deltaTime;
        }
        else {
            // reset, which triggrs picking a new direction
            ai.distanceMoved = 0.0f;
        }

        // jump unless stationary
        if (transform.direction.x != 0.0f || transform.direction.z != 0.0f) {
            transform.direction.y = 1.0f;
        }
    }
}