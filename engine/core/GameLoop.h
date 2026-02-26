#ifndef MMOTOR_GAMELOOP_H
#define MMOTOR_GAMELOOP_H
#pragma once

#include <chrono>
#include <functional>

class GameLoop {
public:
    using UpdateFn = std::function<void(float dt)>;
    using RenderFn = std::function<void(float alpha)>;

    static constexpr float FRAMES_SEC = 60.0f;
    static constexpr float FIXED_DT = 1.0f / FRAMES_SEC;

    void run(UpdateFn update, RenderFn render) {
        using Clock = std::chrono::steady_clock;
        auto previous = Clock::now();
        float accumulator = 0.0f;

        m_running = true;
        while (m_running) {
            auto current  = Clock::now();
            float elapsed = std::chrono::duration<float>(current - previous).count();
            previous = current;

            // Spiral of death protection
            accumulator += std::min(elapsed, 0.25f);

            while (accumulator >= FIXED_DT) {
                update(FIXED_DT);
                accumulator -= FIXED_DT;
            }

            render(accumulator / FIXED_DT); // alpha para interpolación
        }
    }

    void stop() { m_running = false; }

private:
    bool m_running{false};
};

#endif //MMOTOR_GAMELOOP_H