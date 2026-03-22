//STUPA STUDIO
//StupaEngine
// StupaECS_ v0.0.1 -- entity component system from STUPA 

#pragma once
#include "ecs.h"
#include "entity_manager.h"
#include "component_manager.h"
#include "system_manager.h"
#include <memory>

class World {
    std::unique_ptr<EntityManager> entityManager;
    std::unique_ptr<ComponentManager> componentManager;
    std::unique_ptr<SystemManager> systemManager;

public:
    void init(){
        entityManager = std::make_unique<EntityManager>();
        componentManager = std::make_unique<ComponentManager>();
        systemManager = std::make_unique<SystemManager>();
    }

    // entity
    Entity createEntity(){
        return entityManager->createEntity();
    }

    void destroyEntity(Entity entity){
        entityManager->destroyEntity(entity);
        componentManager->entityDestroyed(entity);
        systemManager->entityDestroyed(entity);
    }

    // component
    template<typename T>
    void registerComponent(){
        componentManager->registerComponent<T>();
    }

    template<typename T>
    void addComponent(Entity entity, T component){
        componentManager->addComponent<T>(entity, component);
        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), true);
        entityManager->setSignature(entity, signature);
        systemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    void removeComponent(Entity entity){
        componentManager->removeComponent<T>(entity);
        auto signature = entityManager->getSignature(entity);
        signature.set(componentManager->getComponentType<T>(), false);
        entityManager->setSignature(entity, signature);
        systemManager->entitySignatureChanged(entity, signature);
    }

    template<typename T>
    T& getComponent(Entity entity){
        return componentManager->getComponent<T>(entity);
    }

    template<typename T>
    ComponentType getComponentType(){
        return componentManager->getComponentType<T>();
    }

    // system
    template<typename T>
    std::shared_ptr<T> registerSystem(){
        return systemManager->registerSystem<T>();
    }

    template<typename T>
    void setSystemSignature(Signature signature){
        systemManager->setSignature<T>(signature);
    }
};