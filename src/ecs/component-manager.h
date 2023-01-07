#pragma once
#include "component-array.h"
#include "component.h"
#include <map>
#include <memory>

class ComponentManager {
private:
    // Map from type string pointer to a component type
    std::map<const char*, ComponentType> componentTypes{};

    // Map from type string pointer to a component array
    std::map<const char*, std::shared_ptr<IComponentArray>> componentArray{};

    // The component type to be assigned to the next registered component - starting at 0
    ComponentType nextComponentType{};

    // Convenience function to get the statically casted pointer to the ComponentArray of type T.
    template <typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray()
    {
        const char* typeName = typeid(T).name();

        if (componentTypes.find(typeName) == componentTypes.end()) {
            registerComponent<T>();
        }

        return std::static_pointer_cast<ComponentArray<T>>(componentArray[typeName]);
    }

    template <typename T>
    void registerComponent()
    {
        const char* typeName = typeid(T).name();

        // Add this component type to the component type map
        componentTypes.insert({typeName, nextComponentType});

        // Create a ComponentArray pointer and add it to the component arrays map
        componentArray.insert({typeName, std::make_shared<ComponentArray<T>>()});

        // Increment the value so that the next component registered will be different
        ++nextComponentType;
    }

public:
    template <typename T>
    ComponentType getComponentType()
    {
        const char* typeName = typeid(T).name();

        if (componentTypes.find(typeName) == componentTypes.end()) {
            registerComponent<T>();
        }

        // Return this component's type - used for creating signatures
        return componentTypes[typeName];
    }

    template <typename T>
    void addComponent(Entity entity, T component)
    {
        // Add a component to the array for an entity
        getComponentArray<T>()->insertData(entity, component);
    }

    template <typename T>
    void removeComponent(Entity entity)
    {
        // Remove a component from the array for an entity
        getComponentArray<T>()->removeData(entity);
    }

    template <typename T>
    T& getComponent(Entity entity)
    {
        // Get a reference to a component from the array for an entity
        return getComponentArray<T>()->getData(entity);
    }

    void entityDestroyed(Entity entity)
    {
        // Notify each component array that an entity has been destroyed
        // If it has a component for that entity, it will remove it
        for (auto const& pair : componentArray) {
            auto const& component = pair.second;

            component->entityDestroyed(entity);
        }
    }
};