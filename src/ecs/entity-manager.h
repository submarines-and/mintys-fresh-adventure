#pragma once
#include "entity.h"
#include "signature.h"
#include <array>
#include <queue>

class EntityManager {
private:
    std::queue<Entity> entityIds{};
    int activeEntities = 0;
    std::array<Signature, MAX_ENTITIES> signatures{};

public:
    EntityManager()
    {
        // init available entity ids
        for (Entity e = 0; e < MAX_ENTITIES; e++) {
            entityIds.push(e);
        }
    }

    Entity createEntity()
    {
        if (activeEntities >= MAX_ENTITIES) {
            printf("Entity limit reached\n");
            return -1;
        }

        activeEntities++;
        Entity id = entityIds.front();
        entityIds.pop();
        return id;
    }

    void destroyEntity(Entity entity)
    {
        signatures[entity].reset();
        entityIds.push(entity);
        activeEntities--;
    }

    void setSignature(Entity entity, Signature signature)
    {
        signatures[entity] = signature;
    }

    Signature getSignature(Entity entity)
    {
        return signatures[entity];
    }
};