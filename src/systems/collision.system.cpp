#include "collision.system.h"
#include "components/transform.component.h"
#include "global.h"

void CollisionSystem::update()
{
    for (auto entity : entities) {
        auto& transform = global.ecs->getComponent<TransformComponent>(entity);

        auto terrainType = global.world->getTerrainType(transform.position);

        switch (terrainType) {
        case Biome::WATER:
            //   case Biome::ROCK:
            //    case Biome::MOUNTAIN:
            transform.position = transform.positionLastFrame;
            break;

        default:
            break;
        }
    }
}