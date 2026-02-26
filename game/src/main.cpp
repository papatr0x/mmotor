#include "core/Engine.h"

int main() {
    auto& engine = Engine::instance();

    if (!engine.init("MiniMotor Engine Game", 1280, 720)) {
        return -1;
    }

    engine.run([](float dt) {

        // here goes the real thing

        auto& input = Engine::instance().input();
        if (input.key(SDL_SCANCODE_ESCAPE) == ActionState::JustPressed) {
            Engine::instance().loop().stop();
        }
    });

    engine.shutdown();

    return 0;
}