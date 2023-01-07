#pragma once
#include <array>
#include <map>
#include "entity.h"

class IComponentArray {
public:
    IComponentArray() = default;
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
private:
    int entityCount = 0;
    std::array<T, MAX_ENTITIES> componentArray;
    std::map<Entity, int> entityToIndexMap;
    std::map<int, Entity> indexToEntityMap;

public:
    void insertData(Entity entity, T component)
    {
        int index = entityCount;

        entityToIndexMap[entity] = index;
        indexToEntityMap[index] = entity;
        componentArray[index] = component;
        entityCount++;
    }

    void removeData(Entity entity)
    {
        // Copy element at end into deleted element's place to maintain density
        int indexOfRemovedEntity = entityToIndexMap[entity];
        int indexOfLastElement = entityCount - 1;
        componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

        // Update map to point to moved spot
        Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
        entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLastElement);

        entityCount--;
    }

    T& getData(Entity entity)
    {
        return componentArray[entityToIndexMap[entity]];
    }

    void entityDestroyed(Entity entity) override
    {
        // Remove the entity's component if it existed
        if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            removeData(entity);
        }
    }
};