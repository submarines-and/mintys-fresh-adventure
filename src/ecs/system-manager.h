#pragma once
#include "signature.h"
#include "system.h"
#include <map>
#include <memory>

class SystemManager {
private:
    // Map from system type string pointer to a signature
    std::map<const char*, Signature> signatures{};

    // Map from system type string pointer to a system pointer
    std::map<const char*, std::shared_ptr<System>> systems{};

public:
    template <typename T>
    std::shared_ptr<T> registerSystem(std::vector<ComponentType> components)
    {
        const char* typeName = typeid(T).name();

        // Create a pointer to the system and return it so it can be used externally
        auto system = std::make_shared<T>();
        systems.insert({typeName, system});

        // set signature
        Signature signature;
        for (auto componentName : components) {
            signature.set(componentName);
        }
        setSignature<T>(signature);

        return system;
    }

    template <typename T>
    void setSignature(Signature signature)
    {
        const char* typeName = typeid(T).name();

        if (systems.find(typeName) == systems.end()) {
            printf("System not registered, can not set signature\n");
            return;
        }

        // Set the signature for this system
        signatures.insert({typeName, signature});
    }

    void entityDestroyed(Entity entity)
    {
        // Erase a destroyed entity from all system lists
        // entities is a set so no check needed
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->entities.erase(entity);
        }
    }

    void entitySignatureChanged(Entity entity, Signature entitySignature)
    {
        // Notify each system that an entity's signature changed
        for (auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            // Entity signature matches system signature - insert into set
            if ((entitySignature & systemSignature) == systemSignature) {
                system->entities.insert(entity);
                system->entityAdded(entity);
            }
            else {
                // Entity signature does not match system signature - erase from set
                system->entities.erase(entity);
                system->entityRemoved(entity);
            }
        }
    }

    template <typename T>
    std::shared_ptr<T> getSystem()
    {
        const char* typeName = typeid(T).name();

        if (systems.find(typeName) == systems.end()) {
            printf("System not registered, can not set signature\n");
            return nullptr;
        }

        return std::static_pointer_cast<T>(systems[typeName]);
    }
};