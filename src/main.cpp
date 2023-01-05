#include "gfx/camera.h"
#include "gfx/renderer.h"
#include "gfx/window.h"
#include "global.h"

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.renderer = new Renderer();
    global.camera = new Camera();
    global.world = new World();
}

void update()
{
}

void render()
{
    global.world->render();
}

void destroy()
{
}

int main()
{
    global.width = 1920;
    global.height = 1080;

    Window window(global.width, global.height, init, update, render, destroy);

    window.loop();

    return 0;
}
