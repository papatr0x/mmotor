//
// Created by Patricio Palma on 26-02-26.
//
#include "Engine.h"
#include <SDL3/SDL.h>
#include <iostream>

bool Engine::init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != true) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    // make a window
    m_window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);
    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }

    m_input = std::make_unique<InputSystem>();

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
        [](float alpha) {
            // Render — still empty... maybe some day
        }
    );
}

void Engine::shutdown() {
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
    m_initialized = false;
    std::cout << "Engine shutdown\n";
}