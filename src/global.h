#pragma once
#include "gfx/camera.h"
#include "gfx/renderer.h"

/** All globally available objects */
struct Global {
    Camera* camera;
    Renderer* renderer;
};

/** Will be created by main.cpp */
extern Global& global;