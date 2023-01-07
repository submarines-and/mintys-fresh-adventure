#pragma once
#include "components/sprite.component.h"
#include "ecs/system.h"

class SpriteSystem : public System {

private:
    unsigned int vao;

public:
    SpriteSystem();
    void entityAdded(Entity entity);
    void update();
};