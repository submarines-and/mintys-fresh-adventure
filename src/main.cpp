#include "gfx/window.h"
#include "gfx/model.h"

Model* m = nullptr;

void init()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<float> vertices = {
     // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    std::vector<int> indices = {
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    m = new Model(vertices, indices, "shaders/test.vs", "shaders/test.fs", "assets/box.jpg");
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
    glClearColor(255, 255, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    m->render();
}

int main()
{
    Window window(init, destroy, update, render);
    window.loop();

    return 0;
}