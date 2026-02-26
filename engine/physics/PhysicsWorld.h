//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_PHYSICSWORLD_H
#define MMOTOR_PHYSICSWORLD_H

#pragma once
#include <Jolt/Jolt.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyInterface.h>
#include <memory>

// Layers de física — el engine define las capas básicas
namespace PhysicsLayers {
    static constexpr JPH::ObjectLayer NON_MOVING = 0;
    static constexpr JPH::ObjectLayer MOVING     = 1;
    static constexpr JPH::uint NUM_LAYERS        = 2;
}

namespace BroadPhaseLayers {
    static constexpr JPH::BroadPhaseLayer NON_MOVING{0};
    static constexpr JPH::BroadPhaseLayer MOVING{1};
    static constexpr JPH::uint NUM_LAYERS{2};
}

// Mapeo ObjectLayer → BroadPhaseLayer
class BPLayerInterface final : public JPH::BroadPhaseLayerInterface {
public:
    JPH::uint GetNumBroadPhaseLayers() const override {
        return BroadPhaseLayers::NUM_LAYERS;
    }
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override {
        return layer == PhysicsLayers::NON_MOVING
            ? BroadPhaseLayers::NON_MOVING
            : BroadPhaseLayers::MOVING;
    }
    // Just for DEBUG
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override {
        switch ((JPH::BroadPhaseLayer::Type)layer) {
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING: return "NON_MOVING";
            case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:     return "MOVING";
            default: return "UNKNOWN";
        }
    }
};

class ObjVsBPFilter final : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer obj, JPH::BroadPhaseLayer bp) const override {
        if (obj == PhysicsLayers::NON_MOVING) return bp == BroadPhaseLayers::MOVING;
        return true;
    }
};

class ObjVsObjFilter final : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer a, JPH::ObjectLayer b) const override {
        if (a == PhysicsLayers::NON_MOVING) return b == PhysicsLayers::MOVING;
        if (a == PhysicsLayers::MOVING)     return true;
        return false;
    }
};

class PhysicsWorld {
public:
    bool init();
    void update(float dt);
    void shutdown();

    JPH::BodyInterface& bodyInterface();

    // Factory methods — el engine provee helpers para crear bodies comunes
    JPH::BodyID createBox(const JPH::Vec3& position, const JPH::Vec3& halfExtents,
                          JPH::EMotionType motionType, JPH::ObjectLayer layer);
    JPH::BodyID createPlane(const JPH::Plane& plane, JPH::ObjectLayer layer);

    JPH::BodyID createBoxWithSettings(const JPH::Vec3& position,
                                   const JPH::Vec3& halfExtents,
                                   JPH::EMotionType motionType,
                                   JPH::ObjectLayer layer,
                                   float restitution,
                                   float linearDamping);

    void destroyBody(JPH::BodyID id);

private:
    static constexpr int MAX_BODIES       = 1024;
    static constexpr int MAX_BODY_PAIRS   = 1024;
    static constexpr int MAX_CONTACT_CONSTRAINTS = 1024;

    std::unique_ptr<JPH::TempAllocatorImpl>    m_tempAllocator;
    std::unique_ptr<JPH::JobSystemThreadPool>  m_jobSystem;
    std::unique_ptr<JPH::PhysicsSystem>        m_physicsSystem;

    BPLayerInterface  m_bpLayerInterface;
    ObjVsBPFilter     m_objVsBPFilter;
    ObjVsObjFilter    m_objVsObjFilter;

    float m_accumulator{0.0f};
    static constexpr float FIXED_DT = 1.0f / 60.0f;
};

#endif //MMOTOR_PHYSICSWORLD_H