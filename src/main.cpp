#include "components/sprite.component.h"
#include "gfx/window.h"
#include "global.h"
#include "systems/sprite.system.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.renderer = new Renderer();
    global.world = new World(1000);
    global.camera = new Camera(glm::vec3(0.0f, 30.0f, 0.0f));
    global.ecs = new ECS();

    // Register systems
    auto spriteSystem = global.ecs->registerSystem<SpriteSystem>(std::vector<ComponentType>{
        global.ecs->getComponentType<SpriteComponent>(),
    });

    // create player
    auto player = global.ecs->createEntity();
    global.ecs->addComponent(player, SpriteComponent{
                                         .filepath = "assets/hood.png",
                                         .atlasSize = glm::vec2(8, 9),
                                         .atlasOffset = glm::vec2(0, 0),
                                         .worldPosition = glm::vec3(15.0f, 5.0f, -5.0f),
                                         .worldSize = glm::vec2(2.0f, 2.0f),
                                     });
}

void update()
{
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
    delete global.renderer;
}

int main()
{
    global.width = 1920;
    global.height = 1080;

    Window window(global.width, global.height, init, update, render, destroy);

    window.loop();

    return 0;
}
