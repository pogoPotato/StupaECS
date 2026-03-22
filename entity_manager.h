//STUPA STUDIO
//StupaEngine
//StupaECS_ v0.0.1 -- entity component system from STUPA 

#pragma once
#include "ecs.h"
#include <queue>
#include <array>
#include <cassert>

class EntityManager {
    std::queue<Entity> availableEntities;
    std::array<Signature, MAX_ENTITIES> signatures;
    uint32_t livingEntityCount = 0;

public:
    EntityManager(){
        // push all valid entity IDs into the queue
        for(Entity e = 1; e < MAX_ENTITIES; e++)
            availableEntities.push(e);
    }

    Entity createEntity(){
        assert(livingEntityCount < MAX_ENTITIES && "Too many entities!");
        Entity id = availableEntities.front();
        availableEntities.pop();
        livingEntityCount++;
        return id;
    }

    void destroyEntity(Entity entity){
        assert(entity < MAX_ENTITIES && "Entity out of range!");
        signatures[entity].reset();
        availableEntities.push(entity);
        livingEntityCount--;
    }

    void setSignature(Entity entity, Signature signature){
        assert(entity < MAX_ENTITIES && "Entity out of range!");
        signatures[entity] = signature;
    }

    Signature getSignature(Entity entity){
        assert(entity < MAX_ENTITIES && "Entity out of range!");
        return signatures[entity];
    }
};