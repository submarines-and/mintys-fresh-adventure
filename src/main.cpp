#include "gfx/window.h"
#include "global.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

void init()
{
    global.renderer->loadShader("static", "shaders/static.vs", "shaders/static.fs");
    global.renderer->loadSprite("assets/krabbanklo.png");
}

void update()
{
}

void render()
{

    auto shader = global.renderer->getShader("static");
    shader->start();
    shader->setMat4("view", global.camera->getViewMatrix());
    shader->setMat4("projection", global.camera->getProjectionMatrix());
    shader->setInt("image", 0);

    global.renderer->renderSprite("assets/pyddelov.png", "static", glm::vec2(global.renderer->width, global.renderer->height), glm::vec2(200.0f, 200.0f), 0.0f);
}

void destroy()
{
    delete global.renderer;
}

int main()
{
    auto width = 1280;
    auto height = 720;

    Window window(width, height, init, update, render, destroy);
    global.renderer = new Renderer(width, height);
    global.camera = new Camera(glm::vec3(width / 2, height / 2, 0));

    window.loop();

    return 0;
}