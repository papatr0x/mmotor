//
// Created by Patricio Palma on 27-02-26.
//

#ifndef MMOTOR_CAMERASYSTEM_H
#define MMOTOR_CAMERASYSTEM_H

#pragma once
#include "ecs/Components.h"
#include "input/InputSystem.h"
#include <glm/gtc/matrix_transform.hpp>

class CameraSystem {
public:
    static void updateFree(CameraComponent& cam,
                           Transform& tf,
                           const InputSystem& input,
                           float dt)
    {
        const float speed    = 5.0f;
        const float rotSpeed = 2.0f;

        glm::vec3 forward = glm::normalize(cam.target - tf.position);
        glm::vec3 right   = glm::normalize(glm::cross(forward, cam.up));

        if (input.key(SDL_SCANCODE_W) == ActionState::Held)
            tf.position += forward * speed * dt;
        if (input.key(SDL_SCANCODE_S) == ActionState::Held)
            tf.position -= forward * speed * dt;
        if (input.key(SDL_SCANCODE_A) == ActionState::Held)
            tf.position -= right * speed * dt;
        if (input.key(SDL_SCANCODE_D) == ActionState::Held)
            tf.position += right * speed * dt;
        if (input.key(SDL_SCANCODE_Q) == ActionState::Held)
            tf.position -= cam.up * speed * dt;
        if (input.key(SDL_SCANCODE_E) == ActionState::Held)
            tf.position += cam.up * speed * dt;

        if (input.key(SDL_SCANCODE_LEFT) == ActionState::Held)
            cam.target = tf.position +
                glm::vec3(glm::rotate(glm::mat4(1.0f), rotSpeed * dt, cam.up) *
                glm::vec4(forward, 0.0f));
        if (input.key(SDL_SCANCODE_RIGHT) == ActionState::Held)
            cam.target = tf.position +
                glm::vec3(glm::rotate(glm::mat4(1.0f), -rotSpeed * dt, cam.up) *
                glm::vec4(forward, 0.0f));
        if (input.key(SDL_SCANCODE_UP) == ActionState::Held)
            cam.target = tf.position +
                glm::vec3(glm::rotate(glm::mat4(1.0f), rotSpeed * dt, right) *
                glm::vec4(forward, 0.0f));
        if (input.key(SDL_SCANCODE_DOWN) == ActionState::Held)
            cam.target = tf.position +
                glm::vec3(glm::rotate(glm::mat4(1.0f), -rotSpeed * dt, right) *
                glm::vec4(forward, 0.0f));
    }

    static void updateTargetFromParent(CameraComponent& cam,
                                       const Transform& parentTf)
    {
        cam.target = parentTf.position;
    }

    static glm::mat4 viewMatrix(const CameraComponent& cam, const Transform& tf) {
        return glm::lookAt(tf.position, cam.target, cam.up);
    }

    static glm::mat4 projMatrix(const CameraComponent& cam, float aspectRatio) {
        return glm::perspective(
            glm::radians(cam.fov),
            aspectRatio,
            cam.nearPlane,
            cam.farPlane
        );
    }
};

#endif //MMOTOR_CAMERASYSTEM_H