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
// #include "../scene/SceneManager.h"
// #include "../audio/AudioSystem.h"


class Engine {
public:
    static Engine& instance() {
        static Engine s_instance;
        return s_instance;
    }

    using GameUpdateFn = std::function<void(float dt)>;

    bool init(const std::string& title, int width, int height);
    void run(const GameUpdateFn& gameUpdate = nullptr);
    void shutdown();

    Renderer&      renderer()     { return *m_renderer; }
    PhysicsWorld&  physics()      { return *m_physics; }
    // AudioSystem&   audio()        { return *m_audio; }
    InputSystem&   input()        { return *m_input; }
    // SceneManager&  sceneManager() { return *m_sceneManager; }
    GameLoop&      loop()     { return m_loop; }

    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

private:
    Engine() = default;
    ~Engine() = default;

    std::unique_ptr<Renderer>     m_renderer;
    std::unique_ptr<PhysicsWorld> m_physics;
    // std::unique_ptr<AudioSystem>  m_audio;
    std::unique_ptr<InputSystem>  m_input;
    // std::unique_ptr<SceneManager> m_sceneManager;
    GameLoop                      m_loop;
    bool  m_initialized{false};
    SDL_Window* m_window;
};
#endif //MMOTOR_ENGINE_H