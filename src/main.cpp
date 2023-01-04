#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init(int ticks)
{
    global.renderer->loadShader(Shader::TILE, "shaders/tile.vert", "shaders/tile.frag");
    global.renderer->loadShader(Shader::SPRITE, "shaders/sprite.vert", "shaders/sprite.frag");
}

void update(int ticks)
{
}

void render(int ticks)
{
    auto center = glm::vec2(global.camera->position.x + global.width / 2, global.camera->position.y + global.height / 2);

    auto animationframe = (int)(ticks / 20) % 8;
    global.renderer->renderSprite("assets/hood.png", Shader::SPRITE, glm::vec2(8, 9), glm::vec2(animationframe, 3), center, glm::vec2(128, 128), global.camera->rotation);

    global.renderer->renderTiles();
}

void destroy(int ticks)
{
    delete global.renderer;
    delete global.camera;
}

int main()
{
    global.width = 1920;
    global.height = 1080;

    Window window(global.width, global.height, init, update, render, destroy);
    global.renderer = new Renderer();

    global.world.generate(500, TileAtlas{
                                   .spritePath = "assets/forest.jpg",
                                   .atlasSize = glm::vec2(1, 1),
                                   .tileSize = glm::vec2(64, 64),
                               });

    global.camera = new Camera(glm::vec3(global.world.width / 2, global.world.height / 2, 0));

    window.loop();

    return 0;
}