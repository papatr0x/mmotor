#include "core/Engine.h"
#include "ecs/Registry.h"
#include "ecs/Components.h"
#include "physics/PhysicsSystem.h"
#include "renderer/Mesh.h"
#include "renderer/ShaderProgram.h"
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>
#include <iostream>

int main() {
    auto& engine = Engine::instance();
    if (!engine.init("MMotor", 1280, 720)) return EXIT_FAILURE;

    Registry registry;

    // Piso estático
    Entity floor = registry.create();
    registry.emplace<Transform>(floor);
    JPH::BodyID floorBody = engine.physics().createPlane(
        JPH::Plane(JPH::Vec3(0.f, 1.f, 0.f), 0.f),
        PhysicsLayers::NON_MOVING
    );
    registry.emplace<RigidBody>(floor, RigidBody{floorBody.GetIndexAndSequenceNumber()});

    // Caja dinámica — cae y rebota :)
    Entity box = registry.create();
    registry.emplace<Transform>(box, glm::vec3{0.f, 5.f, 0.f});
    JPH::BodyID boxBody = engine.physics().createBoxWithSettings(
        JPH::Vec3(0.f, 5.f, 0.f),
        JPH::Vec3(0.5f, 0.5f, 0.5f),
        JPH::EMotionType::Dynamic,
        PhysicsLayers::MOVING,
        0.5f,  // restitución — rebota al 60% de la altura cada vez
        0.1f   // linear damping — se frena gradualmente en el aire
    );
    registry.emplace<RigidBody>(box, RigidBody{boxBody.GetIndexAndSequenceNumber()});

    // Mesh cubo simple para visualizar la caja
    Mesh boxMesh;
    std::vector<PosColorVertex> vertices = {
        { { -0.5f, -0.5f, -0.5f }, 0xff0000ff },
        { {  0.5f, -0.5f, -0.5f }, 0xff0000ff },
        { {  0.5f,  0.5f, -0.5f }, 0xff0000ff },
        { { -0.5f,  0.5f, -0.5f }, 0xff0000ff },
        { { -0.5f, -0.5f,  0.5f }, 0xff00ff00 },
        { {  0.5f, -0.5f,  0.5f }, 0xff00ff00 },
        { {  0.5f,  0.5f,  0.5f }, 0xff00ff00 },
        { { -0.5f,  0.5f,  0.5f }, 0xff00ff00 },
    };
    std::vector<uint16_t> indices = {
        0,1,2, 0,2,3, // front
        4,6,5, 4,7,6, // back
        0,3,7, 0,7,4, // left
        1,5,6, 1,6,2, // right
        3,2,6, 3,6,7, // top
        0,4,5, 0,5,1, // bottom
    };
    boxMesh.create(vertices, indices);

    ShaderProgram shader;
    shader.load(
        "engine/renderer/shaders/compiled/vs_simple.bin",
        "engine/renderer/shaders/compiled/fs_simple.bin"
    );

    engine.run([&](float dt) {
        if (engine.input().key(SDL_SCANCODE_ESCAPE) == ActionState::JustPressed) {
            engine.loop().stop();
        }

        // Actualizar física y sincronizar con ECS
        PhysicsSystem::update(registry, engine.physics(), dt);

        // Log posición de la caja
        if (auto* tf = registry.get<Transform>(box)) {
            // std::cout << "Box Y: " << tf->position.y << "\n";
            engine.renderer().submit(boxMesh, shader, *tf);
        }


    });

    engine.physics().destroyBody(JPH::BodyID(floorBody.GetIndexAndSequenceNumber()));
    engine.physics().destroyBody(JPH::BodyID(boxBody.GetIndexAndSequenceNumber()));
    boxMesh.destroy();
    shader.destroy();
    engine.shutdown();
    return EXIT_SUCCESS;
}