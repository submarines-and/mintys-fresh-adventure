#pragma once
#include <set>
#include "entity.h"

/** Base system class*/
class System {
public:
    // All relevant entities for this system
    std::set<Entity> entities;
};