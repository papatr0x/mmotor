#include "core/Engine.h"
#include "renderer/Mesh.h"
#include "renderer/ShaderProgram.h"
#include <iostream>

int main() {
    auto& engine = Engine::instance();

    if (!engine.init("MiniMotor Engine Game", 1280, 720)) {
        return EXIT_FAILURE;
    }

    // Triángulo de prueba — lógica del juego, no del engine
    Mesh triangle;
    std::vector<PosColorVertex> vertices = {
        { { -0.5f, -0.5f, 0.0f }, 0xff0000ff },
        { {  0.5f, -0.5f, 0.0f }, 0xff00ff00 },
        { {  0.0f,  0.5f, 0.0f }, 0xffff0000 },
    };
    std::vector<uint16_t> indices = { 0, 1, 2 };
    triangle.create(vertices, indices);

    ShaderProgram shader;
    bool shaderLoaded = shader.load(
        "engine/renderer/shaders/compiled/vs_simple.bin",
        "engine/renderer/shaders/compiled/fs_simple.bin"
    );
    std::cout << "Shader loaded: " << shaderLoaded << "\n";
    std::cout << "Shader valid: " << shader.isValid() << "\n";
    engine.run([&](float dt) {

        // here goes the real thing

        if (engine.input().key(SDL_SCANCODE_ESCAPE) == ActionState::JustPressed) {
            engine.loop().stop();
        }
        engine.renderer().submit(triangle, shader);
    });

    triangle.destroy();
    shader.destroy();
    engine.shutdown();

    return EXIT_SUCCESS;
}