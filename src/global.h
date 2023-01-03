#pragma once
#include "gfx/camera.h"
#include "gfx/renderer.h"
#include "world/world.h"

/** All globally available objects */
struct Global {
    Camera* camera;
    Renderer* renderer;
    World world;
};

/** Will be created by main.cpp */
extern Global& global;