#include "ecs/registry.h"
#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.mouse = new Mouse();
    global.camera = new Camera();
    global.sky = new Sky();
    global.ecs = new ECS();
    global.world = new World(4);

    // register all systems
    // this also ensures components/systems are imported in the main file
    Registry::addAllSystems();

    auto sheep = global.ecs->createEntity();
    global.ecs->addComponent(sheep, TransformComponent{
                                        .position = glm::vec3(50, 0, 50),
                                        .speed = 20.0f,
                                    });
    global.ecs->addComponent(sheep, ModelComponent{.modelFilePath = "obj/sheep.obj"});
    global.ecs->addComponent(sheep, InputComponent());
}

void update(float deltaTime)
{
    global.ecs->getSystem<InputSystem>()->update();
    global.ecs->getSystem<AISystem>()->update(deltaTime);
    global.ecs->getSystem<TransformSystem>()->update(deltaTime);
    global.ecs->getSystem<CollisionSystem>()->update();
}

void render()
{
    global.world->render();
    global.ecs->getSystem<ModelSystem>()->update();
    global.sky->render();
}

void destroy()
{
    delete global.camera;
    delete global.ecs;
    delete global.mouse;
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
