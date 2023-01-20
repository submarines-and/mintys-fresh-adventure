#pragma once
#include "ecs/system.h"

class InputSystem : public System {

private:
    bool cameraSynced = false;

public:
    void update();
};