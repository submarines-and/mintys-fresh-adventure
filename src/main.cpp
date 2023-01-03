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
    global.renderer->loadShader("static", "shaders/static.vert", "shaders/static.frag");
}

void update()
{
}

void render()
{
    auto shader = global.renderer->getShader("static");
    shader->start();
    shader->setMat4("projection", global.camera->getProjectionMatrix());
    shader->setMat4("view", global.camera->getViewMatrix());
    shader->setInt("image", 0);

    auto center = glm::vec2(global.camera->position.x + global.width / 2, global.camera->position.y + global.height / 2);

    global.renderer->renderSprite("assets/pyddelov.png", "static", center, glm::vec2(32, 32), global.camera->rotation);
    shader->setVec2("tilePosition", glm::vec2(0, 0));

    for (auto t : global.world.tiles) {
        shader->setVec2("tilePosition", glm::vec2(t.z, 0));
        global.renderer->renderSprite("assets/terrain.png", "static", glm::vec2(t.x * 32, t.y * 32), glm::vec2(32, 32), 0);
    }

    shader->stop();
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