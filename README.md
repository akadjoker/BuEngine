🎮⚙️ Custom 2D Game Engine On Top of  raylib 🚀💻

Overview

This repository contains a custom 2D game engine built with a focus on flexibility, modularity, and efficient rendering using the Raylib library. The engine is designed to handle various game objects such as sprites, actors, backgrounds, tilemaps, and particles, while providing tools for collision detection, transformation, and physics simulation. Below is a summary of the key components and features:

Key Components
1. Math Utilities

    Vec2: A vector class for 2D operations with methods for addition, subtraction, multiplication, division, rotation, normalization, and more.
    Polar: A class representing polar coordinates with methods for scaling and rotation.
    Matrix2D: A 2D transformation matrix class for handling translations, rotations, scaling, and skewing.

2. Rendering and Geometry

    rVertex & rQuad: Structures for handling vertex data and quads, used in rendering operations.
    Graph: Represents a texture and its clipping region, with optional sub-graphs.
    AABB: Axis-aligned bounding box for collision detection and spatial representation.
    Shape & Triangle: Basic geometric shapes, including circles and polygons, used for collision detection.

3. Game Objects

    Node: The base class for all game objects, providing basic properties such as visibility, position, and hierarchy management.
    Node2D: Inherits from Node, adding 2D-specific properties like position, scale, pivot, and rotation, with support for collision detection and transformation.
    Sprite: A specialized Node2D that renders textures with optional flipping and color tinting.
    Actor: A dynamic Sprite with added physics properties such as velocity, acceleration, and friction.
    Emitter: Handles particle generation and updates, useful for effects like smoke, fire, or explosions.
    Background: A Node for rendering background images, with support for parallax scrolling.
    TileMap: Manages grid-based maps for efficient rendering and collision detection, useful for games with tiled environments.

4. Scene Management

    Scene: Manages the collection of Nodes, including rendering, updating, and collision detection. Supports multiple regions for splitting the game world into different viewports.
    Region: Manages camera settings and world boundaries, allowing for complex scene layouts.

5. Physics and Collision

    SolidShape, SolidCircle, SolidPolygon: Used for handling physics-based collisions, integrated with the Scene for collision detection between different game objects.
    Collision Detection: Various functions for detecting intersections between shapes, circles, and polygons.

6. Utility Classes

    NodeList: A linked list implementation for managing Nodes within the Scene.
    Assets: Manages the loading and retrieval of graphical assets like textures and atlases.
    Timer: Provides basic timing functionality, useful for tracking time intervals, delta time, and game loops.

Features

    2D Transformations: Includes rotation, scaling, translation, and skewing.
    Collision Detection: Supports AABB, circle, and polygon collisions with precise detection methods.
    Particle System: Customizable emitters for creating visual effects.
    Tilemaps: Efficient rendering and collision handling for grid-based environments.
    Scene Graph: A hierarchical structure to manage and render game objects with support for layers and regions.
    Physics Integration: Basic physics with properties like velocity, acceleration, friction, and elasticity.
    Modular and Extensible: Easy to extend the engine with custom nodes, shapes, and behaviors.

Minimal Dependencies: The engine is designed to be lightweight, with minimal dependencies on the C++ Standard Library (STL). Custom implementations of common data structures, such as vectors and maps, are used to ensure optimal performance and memory management.

Getting Started

To start using the engine, include the necessary header files in your project, and follow these basic steps:

    Initialize Raylib and the Scene.
    Load assets using the Assets class.
    Create and add Nodes like Sprites, Actors, and TileMaps to the Scene.
    Implement game logic by overriding OnUpdate and OnRender methods in your custom Node classes.
    Use the Scene class to manage your game loop, including updating and rendering.

Dependencies

    Raylib: The engine is built on top of the Raylib library, which provides low-level access to graphics, input, and audio.

This engine is a work in progress and is designed to be highly customizable and efficient for 2D game development. Contributions and suggestions are welcome!