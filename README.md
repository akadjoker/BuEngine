# BuEngine

**BuEngine** is a custom 2D game engine built in C++ on top of [raylib](https://www.raylib.com/).  
It is designed as a lightweight, flexible and hackable foundation for 2D games, prototypes, engine experiments and scripting-language integration.

The engine focuses on a small but practical runtime: scene management, 2D transforms, sprites, actors, tilemaps, particles, cameras, basic physics and collision detection.

> This project is a work in progress and is being developed as an experimental game-engine foundation.

---

## Overview

BuEngine provides a simple scene-graph style architecture where game objects are represented as nodes. Objects can be updated, rendered, grouped, layered and attached to other nodes.

The engine is built around a direct C++ API and uses raylib/rlgl for rendering, input, windowing and low-level graphics access.

Current focus areas include:

- Fast 2D rendering using textured quads
- Scene graph and node hierarchy
- Sprite and actor-based gameplay objects
- Tilemap rendering and collision helpers
- Particle emitters for visual effects
- Basic collision detection using AABB, circles and polygons
- Multiple camera regions and viewport support
- Minimal dependency design
- Simple API suitable for game prototypes and scripting integration

---

## Features

### Core

- `Node` base class for all objects
- `Node2D` for position, rotation, scale, pivot and transform handling
- Parent/child hierarchy
- Layer-based rendering order
- Active/visible object states
- Safe node removal through `kill()` and scene cleanup

### Math

- `Vec2` 2D vector type
- `Matrix2D` transform matrix
- Polar-coordinate helper
- AABB utilities
- Angle helpers, interpolation and movement utilities

### Rendering

- Textured quad rendering
- Sprite drawing with color tinting
- Texture clipping / atlas support
- Flip X/Y support
- Tile rendering
- Parallax backgrounds
- Low-level rendering through raylib and rlgl

### Game Objects

- `Sprite` for textured 2D objects
- `Actor` for velocity, acceleration, friction and gravity-based movement
- `Background` for static backgrounds
- `Parallax` for scrolling background layers
- `Emitter` for particles
- `TileMap` for grid-based levels

### Scene and Camera

- Singleton-style `Scene` manager
- Object update and render loop
- Node lookup by ID, name or layer
- Multiple regions/viewports
- Camera position, zoom, rotation and bounds
- Camera-follow style gameplay support

### Collision

- AABB intersection helpers
- Circle collision
- Polygon collision
- Circle-vs-polygon tests
- Simple solid shapes
- `place_free` / `place_meeting` style helpers inspired by classic 2D engines
- Movement helpers such as `move_and_collide`, `move_and_stop` and `move_and_slide`

### Assets

- Centralized `Assets` manager
- Texture loading by numeric ID
- Atlas loading
- Sub-graph support for sprite frames

---

## Example Projects

The repository includes several small example projects that demonstrate different parts of the engine:

| Example | Description |
| --- | --- |
| `shooter` | Top-down space shooter prototype with player movement, bullets, particles and camera follow |
| `tiles` | Platformer-style tilemap demo with animation, shooting, collision and camera zoom |
| `parallax` | Parallax background / scrolling example |
| `parents` | Parent-child node transform test |
| `box` | Basic object / scene test |
| `collision` | Collision and solid-shape experiments |

---

## Basic Usage

A minimal BuEngine game usually follows this structure:

```cpp
#include <raylib.h>
#include <Engine2D.hpp>

int main()
{
    InitWindow(1024, 720, "BuEngine Game");
    SetTargetFPS(60);

    Assets::as().LoadGraph("assets/player.png", 1);

    Sprite* player = new Sprite("Player", 1, 0, 1);
    player->position = Vec2(400, 300);
    player->set_center();

    Scene::as().add(player);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        Scene::as().update(GetFrameTime());

        EndDrawing();
    }

    Scene::as().clear();
    Assets::as().clear();
    CloseWindow();

    return 0;
}
```

---

## Example: Custom Actor

```cpp
struct Player : public Actor
{
    Player() : Actor("Player", 0, 1)
    {
        set_graph(1);
        set_center();
        add_circle_shape(20.0f);

        friction = Vec2(60.0f, 60.0f);
        position = Vec2(400.0f, 300.0f);
    }

    void update(double dt) override
    {
        acceleration = Vec2(0.0f, 0.0f);

        if (IsKeyDown(KEY_W)) move(120.0f, 0.0f, angle - 90.0f);
        if (IsKeyDown(KEY_A)) angularVelocity = 240.0f;
        if (IsKeyDown(KEY_D)) angularVelocity = -240.0f;

        Scene::as().set_camera(0, GetX(), GetY());
    }
};
```

---

## Project Goals

BuEngine is not intended to be a large general-purpose engine.  
The goal is to keep the codebase understandable, fast to iterate on and easy to modify.

Main design goals:

- Keep the engine small and transparent
- Prefer simple C++ systems over heavy abstractions
- Make gameplay code quick to write
- Keep rendering predictable and close to raylib/rlgl
- Support classic 2D game patterns such as sprites, actors, tilemaps and particles
- Provide a good foundation for future scripting integration

---

## Current Status

BuEngine is currently experimental.

What already works:

- Basic scene graph
- Sprite rendering
- Actors and simple movement
- Tilemap rendering
- Texture atlas loading
- Particle effects
- Camera regions
- Basic collision helpers
- Several playable/test examples

Areas still evolving:

- Renderer cleanup and batching design
- Collision result reporting
- Build system documentation
- Memory ownership rules
- API consistency
- Editor/tooling integration
- More complete examples and tutorials

---

## Suggested Roadmap

- [ ] Split the engine into clearer modules: `core`, `math`, `render`, `assets`, `physics`, `tilemap`, `particles`
- [ ] Add a formal `Hit2D` collision result structure
- [ ] Unify quad rendering into a cleaner `Renderer2D` API
- [ ] Improve documentation for each engine subsystem
- [ ] Add build instructions for Linux, Windows and Web targets
- [ ] Add more examples with small, focused gameplay features
- [ ] Add tests for math, transforms and collision functions
- [ ] Improve asset lifetime and ownership documentation
- [ ] Prepare the engine for integration with BuLang / ZEN VM scripting

---

## Dependencies

- C++
- raylib
- rlgl

Some third-party code may be included directly in the repository for convenience.

---

## Philosophy

BuEngine is built around the idea that a small custom engine can be more useful for learning, experimentation and tool-building than a large black-box framework.

It is meant to be read, changed, broken, rebuilt and improved.

The project is also part of a broader exploration into custom game-development technology, including scripting, virtual machines, tools and lightweight rendering systems.

---

## License

No license information has been defined yet.

Before using this project in production or redistributing it, add an explicit license file to the repository.
