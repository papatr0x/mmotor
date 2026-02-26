//
// Created by Patricio Palma on 26-02-26.
//
#include "Engine.h"
#include "platform/NativeWindow.h"
#include <SDL3/SDL.h>
#include <iostream>

bool Engine::init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != true) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    // Make a window
    m_window = SDL_CreateWindow(title.c_str(), width, height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_METAL);
    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }

    auto handles = getNativeHandles(m_window);
    if (!handles.windowHandle) {
        std::cerr << "Failed to get native window handle\n";
        return false;
    }

    m_input = std::make_unique<InputSystem>();
    m_renderer = std::make_unique<Renderer>();
    m_physics = std::make_unique<PhysicsWorld>();

    if (!m_physics->init()) {
        return false;
    }

    if (!m_renderer->init(handles.windowHandle, handles.displayHandle, width, height)) {
        return false;
    }

    // Show window and raise it to front
    SDL_ShowWindow(m_window);
    SDL_RaiseWindow(m_window);

    // Pump events. Cocoa proceses the window before game loop
    SDL_PumpEvents();

    m_initialized = true;
    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")\n";
    return true;
}

void Engine::run(const GameUpdateFn& gameUpdate) {
    if (!m_initialized) return;

    m_loop.run(
        [this, gameUpdate](float dt) {
            m_input->update();

            if (m_input->quitRequested()) {
                m_loop.stop();
                return;
            }
            if (gameUpdate) {
                gameUpdate(dt);
}
        },
        [this](float alpha) {
            m_renderer->beginFrame();
            // render a game screen
            m_renderer->endFrame();
        }
    );
}

void Engine::shutdown() {
    if (m_physics) {
        m_physics->shutdown();
    }
    if (m_renderer) {
        m_renderer->shutdown();
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
    m_initialized = false;
    std::cout << "Engine shutdown\n";
}