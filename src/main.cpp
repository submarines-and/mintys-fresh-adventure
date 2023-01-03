#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
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
    shader->setVec2("tilePosition", glm::vec2(0, 0));

    auto center = glm::vec2(global.camera->position.x + global.renderer->width / 2, global.camera->position.y + global.renderer->height / 2);
    global.renderer->renderSprite("assets/pyddelov.png", "static", center, glm::vec2(32, 32), 0.0f);


    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            global.renderer->renderSprite("assets/terrain.png", "static", glm::vec2(i * 64, j * 64), glm::vec2(64, 64), 0.0f);
        }
    }


    shader->stop();
}

void destroy()
{
    delete global.renderer;
}

int main()
{
    auto width = 32 * 40;
    auto height = 32 * 20;

    Window window(width, height, init, update, render, destroy);
    global.renderer = new Renderer(width, height);
    global.camera = new Camera(glm::vec3(-1, -1, 0));

    window.loop();

    return 0;
}