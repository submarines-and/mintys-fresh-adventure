#pragma once
#include "ecs/system.h"

class TransformSystem : public System {
private:
    float GRAVITY = 3.0f;
    int TURN_FACTOR = 20;

public:
    void
    update(float deltaTime);
};