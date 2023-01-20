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

        // increase rotation
        transform.rotation.y += transform.currentTurnSpeed * TURN_FACTOR * deltaTime;

        float distanceMoved = transform.currentSpeed * deltaTime;
        float dx = distanceMoved * glm::sin(glm::radians(transform.rotation.y));
        float dz = distanceMoved * glm::cos(glm::radians(transform.rotation.y));

        // boost up if intent is to jumo
        if (transform.jump && !transform.isJumping) {
            transform.currentUpwardSpeed = transform.speed;
            transform.isJumping = true;
        }

        // jumping
        transform.currentUpwardSpeed -= (GRAVITY * transform.speed) * deltaTime;
        float dy = transform.currentUpwardSpeed * deltaTime;
        transform.position += glm::vec3(dx, dy, dz);

        // clamp y to terrain height
        auto heightAtPosition = global.world->getTerrainHeight(transform.position);
        if (transform.position.y < heightAtPosition) {
            transform.currentUpwardSpeed = 0.0f;
            transform.position.y = heightAtPosition;

            // end of jump
            transform.isJumping = false;
            transform.jump = false;
        }

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