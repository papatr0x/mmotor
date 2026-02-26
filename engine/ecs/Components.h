//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_COMPONENTS_H
#define MMOTOR_COMPONENTS_H

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

#endif //MMOTOR_COMPONENTS_H