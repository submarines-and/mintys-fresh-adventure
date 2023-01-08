#include "ecs/registry.h"
#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.world = new World(1000);
    global.camera = new Camera(glm::vec3(10.0f, 30.0f, 50.0f));
    global.ecs = new ECS();

    // register all systems
    // this also ensures components/systems are imported in the main file
    Registry::addAllSystems();

    // create player
    auto player = global.ecs->createEntity();
    global.ecs->addComponent(player, SpriteComponent{
                                         .textureFilePath = "assets/hood.png",
                                         .atlasSize = glm::vec2(8, 9),
                                         .atlasOffset = glm::vec2(0, 5),
                                         .animationFrames = 8,
                                     });

    global.ecs->addComponent(player, TransformComponent{
                                         .position = glm::vec3(30.0f, 15.0f, 50.0f),
                                         .size = glm::vec2(5.0f, 5.0f),
                                     });

    global.ecs->addComponent(player, InputComponent());
    global.ecs->addComponent(player, CollisionComponent());
}

void update(int ticks, float deltaTime)
{
    global.ecs->getSystem<InputSystem>()->update();
    global.ecs->getSystem<TransformSystem>()->update(deltaTime);
    global.ecs->getSystem<CollisionSystem>()->update();
}

void render(int ticks, float deltaTime)
{
    global.world->render();
    global.ecs->getSystem<SpriteSystem>()->update(ticks);
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

    Window window(global.width, global.height, init, destroy, update, render);

    window.loop();

    return 0;
}
