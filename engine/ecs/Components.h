//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_COMPONENTS_H
#define MMOTOR_COMPONENTS_H

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using Entity = std::uint32_t;

// Posición, rotación y escala de una entidad
struct Transform {
    glm::vec3 position{0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 scale{1.0f};
};

// Referencia al body de Jolt — el engine lo sincroniza con Transform
struct RigidBody {
    uint32_t bodyId{0};
};

struct CameraComponent {
    glm::vec3 target{0.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float fov{60.0f};
    float nearPlane{0.1f};
    float farPlane{100.0f};
};

struct SupportArm {
    Entity    parent{0};
    glm::vec3 offset{0.f, 2.f, 5.f};
    bool      inheritRotation{false};
};

#endif //MMOTOR_COMPONENTS_H