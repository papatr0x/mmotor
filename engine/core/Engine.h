//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_ENGINE_H
#define MMOTOR_ENGINE_H

#pragma once
#include "GameLoop.h"
#include "renderer/Renderer.h"
#include "physics/PhysicsWorld.h"
#include "input/InputSystem.h"
#include "ecs/Registry.h"

// #include "../scene/SceneManager.h"
// #include "../audio/AudioSystem.h"

class Engine {
public:
    static Engine& instance() {
        static Engine s_instance;
        return s_instance;
    }

    using UpdateFn = std::function<void(float dt)>;
    using RenderFn = std::function<void(float alpha)>;
    using GameUpdateFn = std::function<void(float dt)>;

    bool init(const std::string& title, int width, int height);
    void run(UpdateFn gameUpdate = nullptr, RenderFn gameRender = nullptr);
    void shutdown();

    InputSystem&   input()        { return *m_input; }
    Renderer&      renderer()     { return *m_renderer; }
    PhysicsWorld&  physics()      { return *m_physics; }
    Registry&      registry()     { return *m_registry; }
    GameLoop&      loop()     { return m_loop; }

    // AudioSystem&   audio()        { return *m_audio; }
    // SceneManager&  sceneManager() { return *m_sceneManager; }

    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

private:
    Engine() = default;
    ~Engine() = default;

    void engineUpdate(float dt);
    void engineRender(float alpha);

    std::unique_ptr<Renderer>     m_renderer;
    std::unique_ptr<PhysicsWorld> m_physics;
    std::unique_ptr<InputSystem>  m_input;
    std::unique_ptr<Registry>     m_registry;
    GameLoop                      m_loop;
    // std::unique_ptr<SceneManager> m_sceneManager;
    // std::unique_ptr<AudioSystem>  m_audio;
    bool  m_initialized{false};
    SDL_Window* m_window{nullptr};

    UpdateFn m_gameUpdate;
    RenderFn m_gameRender;
};
#endif //MMOTOR_ENGINE_H