#pragma once
#include "ecs/system.h"

class TransformSystem : public System {
private:
    float GRAVITY = 9.82f;
    int TURN_FACTOR = 40;

public:
    void
    update(float deltaTime);
};