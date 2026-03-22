//STUPA STUDIO
//StupaEngine
// StupaECS_ v0.0.1 -- entity component system from STUPA 

#pragma once
#include "ecs.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <set>

// base system class - all game systems inherit from this
class System {
public:
    std::set<Entity> entities;
};

class SystemManager {
    std::unordered_map<std::type_index, Signature> signatures;
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
    template<typename T>
    std::shared_ptr<T> registerSystem(){
        std::type_index type = typeid(T);
        assert(systems.find(type) == systems.end() && "System already registered");
        auto system = std::make_shared<T>();
        systems[type] = system;
        return system;
    }

    template<typename T>
    void setSignature(Signature signature){
        std::type_index type = typeid(T);
        assert(systems.find(type) != systems.end() && "System not registered");
        signatures[type] = signature;
    }

    void entityDestroyed(Entity entity){
        for(auto& pair : systems)
            pair.second->entities.erase(entity);
    }

    void entitySignatureChanged(Entity entity, Signature entitySignature){
        for(auto& pair : systems){
            auto& type = pair.first;
            auto& system = pair.second;
            auto& systemSignature = signatures[type];
            if((entitySignature & systemSignature) == systemSignature)
                system->entities.insert(entity);
            else
                system->entities.erase(entity);
        }
    }
};