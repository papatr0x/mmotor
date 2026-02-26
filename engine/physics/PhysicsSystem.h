//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_PHYSICSSYSTEM_H
#define MMOTOR_PHYSICSSYSTEM_H

#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include "../ecs/Registry.h"
#include "../ecs/Components.h"
#include "PhysicsWorld.h"

// Sistema que sincroniza posiciones de Jolt → Transform del ECS
class PhysicsSystem {
public:
    static void update(Registry& registry, PhysicsWorld& world, float dt) {
        world.update(dt);

        // Sincronizar posición y rotación de cada RigidBody → Transform
        auto& bi = world.bodyInterface();
        for (auto& [entity, rb] : registry.view<RigidBody>()) {
            if (auto* tf = registry.get<Transform>(entity)) {
                JPH::BodyID id(rb.bodyId);
                JPH::RVec3 pos = bi.GetCenterOfMassPosition(id);
                JPH::Quat  rot = bi.GetRotation(id);

                tf->position = { pos.GetX(), pos.GetY(), pos.GetZ() };
                tf->rotation = { rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ() };
            }
        }
    }
};

#endif //MMOTOR_PHYSICSSYSTEM_H