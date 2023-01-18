#include "transform.system.h"
#include "components/transform.component.h"
#include "global.h"

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

        // update x, z
        transform.position.x += transform.direction.x * transform.speed * deltaTime;
        transform.position.z += transform.direction.z * transform.speed * deltaTime;

        // update y
        auto heightAtPosition = global.world->getTerrainHeight(transform.position, transform.size);

        // move up until jump height is reached
        if (!transform.isFalling && transform.direction.y > 0.0f && transform.position.y < (heightAtPosition + transform.jumpHeight)) {
            transform.position.y += GRAVITY * deltaTime;
        }
        else {
            // gravity, also set y direction to 0
            transform.direction.y = 0.0f;
            transform.isFalling = true;
            transform.position.y -= GRAVITY * deltaTime;
        }

        // clamp to ground
        // also reset y direction
        if (transform.position.y <= heightAtPosition) {
            transform.isFalling = false;
            transform.direction.y = 0.0f;
            transform.position.y = heightAtPosition;
        }

        // clamp x/z to world edge
        if (transform.position.x <= 0.0f) {
            transform.position.x = 0.0f;
        }
        if (transform.position.z <= 0.0f) {
            transform.position.z = 0.0f;
        }
    }
}