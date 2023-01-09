#pragma once
#include "ecs/system.h"
#include "gfx/shader.h"
#include <vector>

class ModelSystem : public System {
private:
    Shader shader;

public:
    ModelSystem();
    void entityAdded(Entity entity);
    void update();
};