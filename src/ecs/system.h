#pragma once
#include "entity.h"
#include <set>

/** Base system class*/
class System {
public:
    // All relevant entities for this system
    std::set<Entity> entities;

    virtual void entityAdded(Entity entity)
    {
#pragma unused(entity);
    }
};