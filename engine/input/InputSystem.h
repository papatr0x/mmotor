//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_INPUTSYSTEM_H
#define MMOTOR_INPUTSYSTEM_H

#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>
#include <vector>

enum class ActionState { Released, JustPressed, Held, JustReleased };

class InputSystem {
public:
    void update() {
        m_prevKeys = m_currKeys;
        m_prevButtons = m_currButtons;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) m_quit = true;
        }

        int numKeys = 0;
        const bool* keys = SDL_GetKeyboardState(&numKeys);
        m_currKeys.assign(keys, keys + numKeys);

        // Gamepad
        if (m_gamepad) {
            for (int i = 0; i < SDL_GAMEPAD_BUTTON_COUNT; ++i) {
                m_currButtons[i] = SDL_GetGamepadButton(
                    m_gamepad, static_cast<SDL_GamepadButton>(i));
            }
        }
    }

    ActionState key(SDL_Scancode sc) const {
        bool prev = sc < m_prevKeys.size() && m_prevKeys[sc];
        bool curr = sc < m_currKeys.size() && m_currKeys[sc];
        return stateFrom(prev, curr);
    }

    ActionState button(SDL_GamepadButton btn) const {
        bool prev = m_prevButtons.count(btn) && m_prevButtons.at(btn);
        bool curr = m_currButtons.count(btn) && m_currButtons.at(btn);
        return stateFrom(prev, curr);
    }

    bool quitRequested() const { return m_quit; }

private:
    static ActionState stateFrom(bool prev, bool curr) {
        if (!prev && curr)  return ActionState::JustPressed;
        if (prev  && curr)  return ActionState::Held;
        if (prev  && !curr) return ActionState::JustReleased;
        return ActionState::Released;
    }

    std::vector<bool> m_currKeys, m_prevKeys;
    std::unordered_map<int, bool> m_currButtons, m_prevButtons;
    SDL_Gamepad* m_gamepad{nullptr};
    bool m_quit{false};
};

#endif //MMOTOR_INPUTSYSTEM_H