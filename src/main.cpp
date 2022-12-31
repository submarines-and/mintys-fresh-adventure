#include "gfx/window.h"
#include "global.h"
#include "model/model.h"

/** Init global state and make accessible for main function. */
static Global global_instance;
Global& global = global_instance;

Model* m = nullptr;
Shader* shader = nullptr;
void init()
{
    global.camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    m = new Model("assets/tree.obj");
    shader = new Shader("shaders/test.vs", "shaders/test.fs");
}

void destroy()
{
    delete m;
}

void update()
{
}

void render()
{
    shader->start();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(global.camera.Zoom), (float)1280 / (float)720, 0.1f, 100.0f);
    glm::mat4 view = global.camera.getViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    // render the loaded model
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    transform = glm::scale(transform, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    shader->setMat4("transform", transform);

    m->draw(*shader);

    shader->stop();
}

int main()
{
    Window window(init, destroy, update, render);
    window.loop();

    return 0;
}