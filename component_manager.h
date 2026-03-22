//STUPA STUDIO
//StupaEngine 
//StupaECS_ v0.0.1 -- entity component system from STUPA 

#pragma once
#include "ecs.h"
#include <cassert>
#include <unordered_map>
#include <memory>
#include <typeindex>


// component array class and its implementation
class IComponentArray{
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray{
    T components[MAX_ENTITIES];
    std::unordered_map<Entity, size_t> entityToindex;
    std::unordered_map<size_t, Entity> indexToEntity;
    size_t size = 0;

public:
    void insert(Entity entity, T component){
        assert(entityToindex.find(entity) == entityToindex.end() && "Entity already has component");
        size_t index = size;
        entityToindex[entity] = index;
        indexToEntity[index] = entity;
        components[index] = component;
        size++;
    }

    void remove(Entity entity){
        assert(entityToindex.find(entity) != entityToindex.end() && "Entity does not have component");
        size_t indexRemoved = entityToindex[entity];
        size_t indexLast = size - 1;
        components[indexRemoved] = components[indexLast];
        Entity lastEntity = indexToEntity[indexLast];
        entityToindex[lastEntity] = indexRemoved;
        indexToEntity[indexRemoved] = lastEntity;
        entityToindex.erase(entity);
        indexToEntity.erase(indexLast);
        size--;
    }

    T& get(Entity entity){
        assert(entityToindex.find(entity) != entityToindex.end() && "Entity does not have component");
        return components[entityToindex[entity]];
    }

    void entityDestroyed(Entity entity) override{
        if(entityToindex.find(entity) != entityToindex.end())
            remove(entity);
    }
};


//component managermnt system class and its implementation
class ComponentManager {
    std::unordered_map<std::type_index, ComponentType> componentTypes;
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays;
    ComponentType nextComponentType = 0;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> getArray(){
        std::type_index type = typeid(T);
        assert(componentTypes.find(type) != componentTypes.end() && "Component not registered");
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[type]);
    }

public:
    template<typename T>
    void registerComponent(){
        std::type_index type = typeid(T);
        assert(componentTypes.find(type) == componentTypes.end() && "Component already registered");
        componentTypes[type] = nextComponentType++;
        componentArrays[type] = std::make_shared<ComponentArray<T>>();
    }

    template<typename T>
    ComponentType getComponentType(){
        std::type_index type = typeid(T);
        assert(componentTypes.find(type) != componentTypes.end() && "Component not registered");
        return componentTypes[type];
    }

    template<typename T>
    void addComponent(Entity entity, T component){
        getArray<T>()->insert(entity, component);
    }

    template<typename T>
    void removeComponent(Entity entity){
        getArray<T>()->remove(entity);
    }

    template<typename T>
    T& getComponent(Entity entity){
        return getArray<T>()->get(entity);
    }

    void entityDestroyed(Entity entity){
        for(auto& pair : componentArrays)
            pair.second->entityDestroyed(entity);
    }
};