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

        if (ai.distanceMoved == 0.0f && !ai.commandIssued) {
            // pick new direction or stay still

            if (Math::random(0, 9) < 7) {
                transform.rotation.y = Math::random(0, 360);
                transform.currentSpeed = transform.speed;
            }
            else {
                transform.currentSpeed = 0.0f;
            }

            ai.commandIssued = true;
        }
        else if (ai.distanceMoved < 10.0f) {
            // move a few paces
            ai.distanceMoved += transform.speed * deltaTime;

            // jump if moving
            if (transform.currentSpeed > 0.0f) {
                transform.jump = true;
            }
        }
        else {
            // reset, which triggrs picking a new direction
            ai.distanceMoved = 0.0f;
            ai.commandIssued = false;
        }
    }
}