#include "gfx/window.h"
#include "gfx/model.h"
#include "gfx/shader.h"

ModelLoader loader;
ShaderLoader shader;

Model m;

void init()
{
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<float> vertices = {
        // first triangle
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, 0.5f, 0.0f,  // top left
                            // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    m = loader.load(vertices);
    m.shaderId = shader.load();
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