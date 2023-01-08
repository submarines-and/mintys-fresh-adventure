#pragma once
#include "components/collision.component.h"
#include "components/input.component.h"
#include "components/sprite.component.h"
#include "components/transform.component.h"
#include "global.h"
#include "systems/collision.system.h"
#include "systems/input.system.h"
#include "systems/sprite.system.h"
#include "systems/transform.system.h"

class Registry {
public:
    static void addAllSystems()
    {
        global.ecs->registerSystem<SpriteSystem>(std::vector<ComponentType>{
            global.ecs->getComponentType<SpriteComponent>(),
            global.ecs->getComponentType<TransformComponent>(),
        });

        global.ecs->registerSystem<TransformSystem>(std::vector<ComponentType>{
            global.ecs->getComponentType<TransformComponent>(),
        });

        global.ecs->registerSystem<InputSystem>(std::vector<ComponentType>{
            global.ecs->getComponentType<InputComponent>(),
        });

        global.ecs->registerSystem<CollisionSystem>(std::vector<ComponentType>{
            global.ecs->getComponentType<CollisionComponent>(),
            global.ecs->getComponentType<TransformComponent>(),
        });
    }
};