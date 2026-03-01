#include "core/Engine.h"
#include "ecs/Components.h"
#include "physics/PhysicsSystem.h"
#include "camera/CameraSystem.h"
#include "renderer/Mesh.h"
#include "renderer/ShaderProgram.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>

int main() {
    auto& engine = Engine::instance();
    if (!engine.init("MMotor", 1280, 720)) return EXIT_FAILURE;

    auto& registry = engine.registry();

    // Piso
    Entity floor = registry.create();
    registry.emplace<Transform>(floor);
    JPH::BodyID floorBody = engine.physics().createPlane(
        JPH::Plane(JPH::Vec3(0.f, 1.f, 0.f), 0.f),
        PhysicsLayers::NON_MOVING
    );
    registry.emplace<RigidBody>(floor, RigidBody{floorBody.GetIndexAndSequenceNumber()});

    // Caja
    Entity box = registry.create();
    registry.emplace<Transform>(box, Transform{glm::vec3{0.f, 5.f, 0.f}});
    JPH::BodyID boxBody = engine.physics().createBoxWithSettings(
        JPH::Vec3(0.f, 5.f, 0.f),
        JPH::Vec3(0.5f, 0.5f, 0.5f),
        JPH::EMotionType::Dynamic,
        PhysicsLayers::MOVING,
        0.6f, 0.1f
    );
    registry.emplace<RigidBody>(box, RigidBody{boxBody.GetIndexAndSequenceNumber()});

    // Cámara — entidad simple con Transform y CameraComponent
    Entity camera = registry.create();
    registry.emplace<Transform>(camera, Transform{glm::vec3{0.f, 3.f, 8.f}});
    auto& camComp = registry.emplace<CameraComponent>(camera);
    camComp.target = glm::vec3{0.f, 0.f, 0.f}; // mira al origen

    // Mesh caja
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
        0,1,2, 0,2,3,
        4,6,5, 4,7,6,
        0,3,7, 0,7,4,
        1,5,6, 1,6,2,
        3,2,6, 3,6,7,
        0,4,5, 0,5,1,
    };
    boxMesh.create(vertices, indices);

    ShaderProgram shader;
    shader.load(
        "engine/renderer/shaders/compiled/vs_simple.bin",
        "engine/renderer/shaders/compiled/fs_simple.bin"
    );

    engine.run(
        [&](float dt) {
            if (engine.input().key(SDL_SCANCODE_ESCAPE) == ActionState::JustPressed) {
                engine.loop().stop();
            }

            PhysicsSystem::update(registry, engine.physics(), dt);

            // Mover cámara libre con WASD
            auto* camTf = registry.get<Transform>(camera);
            if (camTf) {
                CameraSystem::updateFree(camComp, *camTf, engine.input(), dt);

                float aspect = float(engine.renderer().width()) /
                               float(engine.renderer().height());
                engine.renderer().setCamera(
                    CameraSystem::viewMatrix(camComp, *camTf),
                    CameraSystem::projMatrix(camComp, aspect)
                );
            }
        },
        [&](float alpha) {
            if (auto* tf = registry.get<Transform>(box)) {
                engine.renderer().submit(boxMesh, shader, *tf);
            }
        }
    );

    engine.physics().destroyBody(JPH::BodyID(floorBody.GetIndexAndSequenceNumber()));
    engine.physics().destroyBody(JPH::BodyID(boxBody.GetIndexAndSequenceNumber()));
    boxMesh.destroy();
    shader.destroy();
    engine.shutdown();
    return EXIT_SUCCESS;
}