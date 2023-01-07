#pragma once
#include "components/sprite.component.h"
#include "ecs/system.h"
#include "gfx/shader.h"

class SpriteSystem : public System {

private:
    unsigned int vao;
    Shader shader;

public:
    SpriteSystem();
    void entityAdded(Entity entity);
    void update();
};