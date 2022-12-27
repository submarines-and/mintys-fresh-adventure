#include "gfx/window.h"
#include "gfx/model.h"

void init()
{
}

void destroy()
{
}


void update()
{
}


void render()
{
}


int main()
{
    Window window(init, destroy, update, render);
    window.loop();

    return 0;
}