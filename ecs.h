//STUPA STUDIO
//StupaEngine
// StupaECS_ v0.0.1 -- entity component system from STUPA 


#pragma once
#include<cstdint>
#include<bitset>

//unique ID  -- as ENTITY is just a unique ID
using Entity = uint32_t;
const Entity MAX_ENTITIES = 5000;
const Entity INVALID_ENTITIES = 0;


// Types of component now component id 
using ComponentType = uint8_t;
const ComponentType MAX_COMPONENTS = 32;

//now a signature to assist whhihc component an entity has
using Signature = std::bitset<MAX_COMPONENTS>;


