# Mini Motor

The idea behind this project is basically to take third-party libraries and create a video game engine with them.

The initially defined features for the engine are:
- Compile and run on macOS and Windows. (maybe mobile in the near future?)
- 2D and 3D rendering.
- Sound.
- Basics physics.
- Gamepad, keyboard and mouse support.
- Use of modern C++.

It should also include a small game demonstrating that the engine works.

This is the basic structure for the engine directory.

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
