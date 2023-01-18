#pragma once
#include "ecs/system.h"

class TransformSystem : public System {
private:
    float GRAVITY = 15.0f;

public:
    void
    update(float deltaTime);
};