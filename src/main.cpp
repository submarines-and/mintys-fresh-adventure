#include "components/sprite.component.h"
#include "components/transform.component.h"
#include "gfx/window.h"
#include "global.h"
#include "systems/sprite.system.h"
#include "systems/transform.system.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.world = new World(1000);
    global.camera = new Camera(glm::vec3(0.0f, 30.0f, 0.0f));
    global.ecs = new ECS();

    // Register systems
    global.ecs->registerSystem<SpriteSystem>(std::vector<ComponentType>{
        global.ecs->getComponentType<SpriteComponent>(),
        global.ecs->getComponentType<TransformComponent>(),
    });

    global.ecs->registerSystem<TransformSystem>(std::vector<ComponentType>{
        global.ecs->getComponentType<TransformComponent>(),
    });

    // create player
    auto player = global.ecs->createEntity();
    global.ecs->addComponent(player, SpriteComponent{
                                         .textureFilePath = "assets/hood.png",
                                         .atlasSize = glm::vec2(8, 9),
                                         .atlasOffset = glm::vec2(0, 0),
                                     });

    global.ecs->addComponent(player, TransformComponent{
                                         .position = glm::vec3(15.0f, 5.0f, -5.0f),
                                         .size = glm::vec2(2.0f, 2.0f),
                                     });
}

void update()
{
    global.ecs->getSystem<TransformSystem>()->update();
}

void render()
{
    global.world->render();
    global.ecs->getSystem<SpriteSystem>()->update();
}

void destroy()
{
    delete global.ecs;
    delete global.camera;
    delete global.world;
}

int main()
{
    global.width = 1920;
    global.height = 1080;

    Window window(global.width, global.height, init, update, render, destroy);

    window.loop();

    return 0;
}
