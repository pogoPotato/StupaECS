# StupaECS v0.0.1
A lightweight, header-only Entity Component System (ECS) library for C++ game engines and applications.

Built as part of the **Stupa Engine** project by Stupa Studio.

---

## What is ECS?
Entity Component System is an architectural pattern used in game development where:
- **Entity** — a unique ID representing any object in your world
- **Component** — pure data attached to an entity (position, velocity, health etc)
- **System** — logic that operates on entities with specific components

This separates data from logic, making your code cache-friendly, modular, and easy to extend.

---

## Features
- Header-only — just drop the files in and include
- Up to 5000 entities by default (configurable)
- Up to 32 component types
- Packed component arrays for cache efficiency
- Signature-based system filtering
- Simple clean API

---

## Installation
Just copy the header files into your project:
```
ecs/
  ecs.h
  component_manager.h
  entity_manager.h
  system_manager.h
  world.h
```

Include `world.h` and you're good to go.

---

## Quick Start
```cpp
#include "StupaECS/world.h"

// Define your components
struct Transform {
    float x, y, z;
};

struct Velocity {
    float dx, dy, dz;
};

int main(){
    World world;
    world.init();

    // Register components
    world.registerComponent<Transform>();
    world.registerComponent<Velocity>();

    // Create an entity
    Entity e = world.createEntity();

    // Add components
    world.addComponent(e, Transform{0.0f, 0.0f, 0.0f});
    world.addComponent(e, Velocity{1.0f, 0.0f, 0.0f});

    // Get and modify components
    Transform& t = world.getComponent<Transform>(e);
    t.x += 1.0f;

    // Destroy entity
    world.destroyEntity(e);

    return 0;
}
```

---

## Creating Systems
```cpp
#include "StupaECS/world.h"

class MovementSystem : public System {
public:
    void update(World& world, float dt){
        for(Entity e : entities){
            auto& t = world.getComponent<Transform>(e);
            auto& v = world.getComponent<Velocity>(e);
            t.x += v.dx * dt;
            t.y += v.dy * dt;
            t.z += v.dz * dt;
        }
    }
};

int main(){
    World world;
    world.init();

    world.registerComponent<Transform>();
    world.registerComponent<Velocity>();

    // Register system and set its signature
    auto movementSystem = world.registerSystem<MovementSystem>();
    Signature sig;
    sig.set(world.getComponentType<Transform>());
    sig.set(world.getComponentType<Velocity>());
    world.setSystemSignature<MovementSystem>(sig);

    // Create entities
    Entity e = world.createEntity();
    world.addComponent(e, Transform{0.0f, 0.0f, 0.0f});
    world.addComponent(e, Velocity{1.0f, 0.0f, 0.0f});

    // Game loop
    while(true){
        movementSystem->update(world, 0.016f);
    }
}
```

---

## Configuration
In `ecs.h` you can change the limits:
```cpp
const Entity MAX_ENTITIES = 5000;     // max entities in the world
const ComponentType MAX_COMPONENTS = 32;  // max component types
```

---

## Upcoming Features
- **View/Query system** — iterate entities by component combination: `world.view<Transform, Velocity>()`
- **Entity tags** — tag entities for fast lookup by category
- **Component events** — callbacks on component add/remove
- **Prefabs** — save and load entity templates
- **Multithreading support** — run systems in parallel with job system
- **Sparse sets** — faster component storage for large entity counts
- **Archetype system** — group entities by signature for better cache performance
- **Serialization** — save and load world state
- **ImGui debug panel** — inspect entities and components at runtime

---

## License
MIT License — free to use in personal and commercial projects.

---

## Part of Stupa Engine
StupaECS is the core ECS library powering **Stupa Engine**, an inHouse game engine built by Stupa Studio.