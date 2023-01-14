#pragma once
#include "ecs/ecs.h"
#include "gfx/camera.h"
#include "world/sky.h"
#include "world/world.h"

/** All globally available objects */
struct Global {

    /** Window with and height */
    int width, height;

    bool keys[GLFW_KEY_LAST];

    ECS* ecs;
    World* world;
    Sky* sky;

    Camera* camera;
};

/** Will be created by main.cpp */
extern Global& global;