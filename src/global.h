#pragma once
#include "ecs/ecs.h"
#include "gfx/camera.h"
#include "gfx/mouse.h"
#include "world/sky.h"
#include "world/world.h"

/** All globally available objects */
struct Global {

    /** Window with and height */
    int width, height;

    /** Game ticks */
    int ticks = 0;

    bool keys[GLFW_KEY_LAST];


    ECS* ecs;
    World* world;
    Sky* sky;

    Camera* camera;
    Mouse* mouse;
};

/** Will be created by main.cpp */
extern Global& global;