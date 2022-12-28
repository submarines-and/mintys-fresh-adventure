#pragma once
#include "gfx/camera.h"

/** All globally available objects */
struct Global {
    Camera camera;
};

/** Will be created by main.cpp */
extern Global& global;