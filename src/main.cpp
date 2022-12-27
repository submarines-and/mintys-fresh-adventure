#include "gfx/window.h"
#include "gfx/model.h"
ModelLoader loader;
Model m;

void init()
{
    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f,  // right
        0.0f, 0.5f, 0.0f    // top
    };

    m = loader.load(vertices);
}

void destroy()
{
}

void update()
{
}

void render()
{
    glClearColor(255, 255, 255, 255);
    glClear(GL_COLOR_BUFFER_BIT);

    loader.render(m);
}

int main()
{
    Window window(init, destroy, update, render);
    window.loop();

    return 0;
}