#pragma once
#include "gfx/camera.h"
#include "gfx/renderer.h"

/** All globally available objects */
struct Global {

    /** Window with and height */
    int width, height;

    Camera* camera;
    Renderer* renderer;
};

/** Will be created by main.cpp */
extern Global& global;