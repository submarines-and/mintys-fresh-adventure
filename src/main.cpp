#include "ecs/registry.h"
#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.sky = new Sky();
    global.world = new World(4);
    global.camera = new Camera(glm::vec3(30.0f, 60.0f, 50.0f));
    global.ecs = new ECS();

    // register all systems
    // this also ensures components/systems are imported in the main file
    Registry::addAllSystems();
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
    global.ecs->getSystem<ModelSystem>()->update();
    global.sky->render();
}

void destroy()
{
    delete global.ecs;
    delete global.camera;
    delete global.world;
    delete global.sky;
}

int main()
{

    global.width = 1920;
    global.height = 1080;

    Window window(global.width, global.height, init, destroy, update, render);

    window.loop();

    return 0;
}
