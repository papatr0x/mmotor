# Mini Motor

This project is a custom video game engine developed as a practical exploration of engine 
architecture. By integrating established third-party libraries, the goal is to build a robust, 
feature-rich framework that demonstrates modern software design principles.

## Core Features
* **Cross-Platform:** Compiles and runs natively on macOS and Windows (with mobile support planned for future releases).
* **Graphics:** Support for both 2D and 3D rendering pipelines.
* **Audio:** Integrated sound and audio management.
* **Physics:** Basic physics simulation for movement and collision detection.
* **Input Handling:** Comprehensive support for gamepads, keyboards, and mice.
* **Modern C++:** Built utilizing modern C++ standards (C++23) to ensure clean, efficient, and maintainable code.

## Demo Project
To validate the engine's core systems, the repository includes a small, fully playable demo game built entirely on this framework.

---

## Directory Structure
The engine's architecture is organized into the following main directories:

```
mmotor_project/
├── CMakeLists.txt
├── engine/
│   ├── core/
│   │   ├── Engine.hpp / Engine.cpp        # Orquestador principal
│   │   ├── GameLoop.hpp / GameLoop.cpp    # Fixed timestep loop
│   │   ├── Logger.hpp                     # Logging centralizado
│   │   └── Assert.hpp
│   ├── ecs/
│   │   ├── Registry.hpp                   # ECS propio ligero
│   │   ├── Entity.hpp
│   │   └── Component.hpp
│   ├── renderer/
│   │   ├── Renderer.hpp / Renderer.cpp    # Abstracción bgfx
│   │   ├── Mesh.hpp
│   │   ├── Texture.hpp
│   │   └── ShaderProgram.hpp
│   ├── physics/
│   │   ├── PhysicsWorld.hpp               # Wrapper Jolt
│   │   └── Colliders.hpp
│   ├── audio/
│   │   └── AudioSystem.hpp               # Wrapper SoLoud
│   ├── input/
│   │   ├── InputSystem.hpp               # Wrapper SDL3 input
│   │   └── InputAction.hpp               # Action mapping
│   ├── scene/
│   │   ├── Scene.hpp
│   │   └── SceneManager.hpp
│   └── platform/
│       ├── Window.hpp
│       └── PlatformLayer.hpp             # Abstracciones OS-específicas
└── game/
│   ├── src/
│   │   └── main.cpp
│   ├── scenes/
│   └── GameScene.hpp
└── assets/
```
