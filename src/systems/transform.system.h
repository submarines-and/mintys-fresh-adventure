#pragma once
#include "ecs/system.h"

class TransformSystem : public System {
private:
    float GRAVITY = 9.82f;

public:
    void
    update(float deltaTime);
};