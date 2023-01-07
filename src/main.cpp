#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.renderer = new Renderer();
    global.world = new World(1000);
    global.camera = new Camera(glm::vec3(0.0f, 30.0f, 0.0f));

    auto player = global.renderer->loadSprite(Sprite::PLAYER, "assets/hood.png");
    player->atlasSize = glm::vec2(8, 9);
    player->worldPosition = glm::vec3(15.0f, 5.0f, -5.0f);
    player->worldSize = glm::vec2(2.0f, 2.0f);
}

void update()
{
}

void render()
{
    global.world->render();
    global.renderer->getSprite(Sprite::PLAYER)->render();
}

void destroy()
{
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
