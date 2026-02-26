
```
mmotor_project/
├── CMakeLists.txt
├── cmake/
│   ├── Platforms.cmake
│   └── Dependencies.cmake
├── engine/
│   src/
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
├── main.cpp
├── scenes/
│   └── GameScene.hpp
└── assets/
```
