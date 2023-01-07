#pragma once
#include "component-manager.h"
#include "entity-manager.h"
#include "system-manager.h"
#include <memory>
#include <vector>

class ECS {
private:
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<SystemManager> systemManager;

public:
    ECS()
    {
        // Create pointers to each manager
        componentManager = std::make_unique<ComponentManager>();
        entityManager = std::make_unique<EntityManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    ~ECS()
    {
        componentManager.release();
        entityManager.release();
        systemManager.release();
    }

    /*

       Entity methods
    */
    Entity createEntity()
    {
        return entityManager->createEntity();
    }

    void destroyEntity(Entity entity)
    {
        entityManager->destroyEntity(entity);
        componentManager->entityDestroyed(entity);
        systemManager->entityDestroyed(entity);
    }

    /*

        Component methods
    */
    template <typename T>
    void registerComponent()
    {
        componentManager->registerComponent<T>();
    }

    template <typename T>
    void addComponent(Entity entity, T component)
    {
        componentManager->addComponent<T>(entity, component);

        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), true);
        entityManager->setSignature(entity, signature);

        systemManager->entitySignatureChanged(entity, signature);
    }

    template <typename T>
    void removeComponent(Entity entity)
    {
        componentManager->removeComponent<T>(entity);

        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), false);
        entityManager->setSignature(entity, signature);

        systemManager->entitySignatureChanged(entity, signature);
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        return componentManager->getComponent<T>(entity);
    }

    template <typename T>
    ComponentType getComponentType()
    {
        return componentManager->getComponentType<T>();
    }

    /*

        System methods
    */
    template <typename T>
    std::shared_ptr<T> registerSystem(std::vector<ComponentType> components)
    {
        return systemManager->registerSystem<T>(components);
    }

    template <typename T>
    std::shared_ptr<T> getSystem()
    {
        return systemManager->getSystem<T>();
    }
};