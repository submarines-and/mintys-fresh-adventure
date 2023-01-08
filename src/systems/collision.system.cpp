#include "collision.system.h"
#include "components/transform.component.h"
#include "global.h"

void CollisionSystem::update()
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        auto terrainType = Biome::GRASS;

        switch (terrainType) {
        case Biome::SHALLOW_WATER:
        case Biome::DEEP_WATER:
        case Biome::ROCK:
        case Biome::MOUNTAIN:
            transform.position = transform.positionLastFrame;
            break;

        default:
            break;
        }
    }
}