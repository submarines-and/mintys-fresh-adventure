#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.world.generate(100);
    global.renderer->loadShader(Shader::TILE, "shaders/tile.vert", "shaders/tile.frag");
}

void update()
{
}

void render()
{
    auto center = glm::vec2(global.camera->position.x + global.width / 2, global.camera->position.y + global.height / 2);
    global.renderer->renderSprite("assets/pyddelov.png", Shader::TILE, glm::vec2(4, 1), glm::vec2(0, 0), center, glm::vec2(32, 32), global.camera->rotation);

    for (auto t : global.world.tiles) {
        global.renderer->renderSprite("assets/terrain.png", Shader::TILE, glm::vec2(20, 20), glm::vec2(((int)t.z % 20), (int)t.z / 20), glm::vec2(t.x * 32, t.y * 32), glm::vec2(32, 32), 0);
    }
}

void destroy()
{
    delete global.renderer;
}

int main()
{
    global.width = 32 * 40;
    global.height = 32 * 20;

    Window window(global.width, global.height, init, update, render, destroy);
    global.renderer = new Renderer();
    global.camera = new Camera(glm::vec3(0, 0, 0));

    window.loop();

    return 0;
}