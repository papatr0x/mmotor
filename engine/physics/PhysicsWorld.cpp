//
// Created by Patricio Palma on 26-02-26.
//

#include "PhysicsWorld.h"
#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/PlaneShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <iostream>

bool PhysicsWorld::init() {
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    m_tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024); // 10MB
    m_jobSystem     = std::make_unique<JPH::JobSystemThreadPool>(
        JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, 2);

    m_physicsSystem = std::make_unique<JPH::PhysicsSystem>();
    m_physicsSystem->Init(
        MAX_BODIES, 0, MAX_BODY_PAIRS, MAX_CONTACT_CONSTRAINTS,
        m_bpLayerInterface, m_objVsBPFilter, m_objVsObjFilter);

    m_physicsSystem->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));

    std::cout << "PhysicsWorld initialized\n";
    return true;
}

void PhysicsWorld::update(float dt) {
    m_physicsSystem->Update(dt, 1, m_tempAllocator.get(), m_jobSystem.get());
}

void PhysicsWorld::shutdown() {
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
    std::cout << "PhysicsWorld shutdown\n";
}

JPH::BodyInterface& PhysicsWorld::bodyInterface() {
    return m_physicsSystem->GetBodyInterface();
}

JPH::BodyID PhysicsWorld::createBox(const JPH::Vec3& position, const JPH::Vec3& halfExtents,
                                     JPH::EMotionType motionType, JPH::ObjectLayer layer) {
    auto shape = new JPH::BoxShape(halfExtents);
    JPH::BodyCreationSettings settings(
        shape, JPH::RVec3(position), JPH::Quat::sIdentity(), motionType, layer);

    JPH::Body* body = m_physicsSystem->GetBodyInterface().CreateBody(settings);
    m_physicsSystem->GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
    return body->GetID();
}

JPH::BodyID PhysicsWorld::createPlane(const JPH::Plane& plane, JPH::ObjectLayer layer) {
    auto shape = new JPH::PlaneShape(plane);
    JPH::BodyCreationSettings settings(
        shape, JPH::RVec3::sZero(), JPH::Quat::sIdentity(),
        JPH::EMotionType::Static, layer);

    JPH::Body* body = m_physicsSystem->GetBodyInterface().CreateBody(settings);
    m_physicsSystem->GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::DontActivate);
    return body->GetID();
}

JPH::BodyID PhysicsWorld::createBoxWithSettings(const JPH::Vec3& position,
                                                 const JPH::Vec3& halfExtents,
                                                 JPH::EMotionType motionType,
                                                 JPH::ObjectLayer layer,
                                                 float restitution,
                                                 float linearDamping) {
    auto shape = new JPH::BoxShape(halfExtents);
    JPH::BodyCreationSettings settings(
        shape, JPH::RVec3(position), JPH::Quat::sIdentity(), motionType, layer);

    settings.mRestitution    = restitution;   // 0 = no rebota, 1 = rebote perfecto
    settings.mLinearDamping  = linearDamping; // 0 = sin fricción de aire, > 0 = se frena

    JPH::Body* body = m_physicsSystem->GetBodyInterface().CreateBody(settings);
    m_physicsSystem->GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
    return body->GetID();
}

void PhysicsWorld::destroyBody(JPH::BodyID id) {
    auto& bi = m_physicsSystem->GetBodyInterface();
    bi.RemoveBody(id);
    bi.DestroyBody(id);
}