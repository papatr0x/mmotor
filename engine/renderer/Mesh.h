//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_MESH_H
#define MMOTOR_MESH_H

#pragma once
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include <vector>

struct PosColorVertex {
    glm::vec3 position;
    uint32_t  color;

    static bgfx::VertexLayout layout() {
        bgfx::VertexLayout l;
        l.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
        return l;
    }
};

class Mesh {
public:
    bool create(const std::vector<PosColorVertex>& vertices,
                const std::vector<uint16_t>& indices);
    void destroy();

    bgfx::VertexBufferHandle vbh() const { return m_vbh; }
    bgfx::IndexBufferHandle  ibh() const { return m_ibh; }
    bool isValid() const {
        return bgfx::isValid(m_vbh) && bgfx::isValid(m_ibh);
    }

private:
    bgfx::VertexBufferHandle m_vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  m_ibh = BGFX_INVALID_HANDLE;
};

#endif //MMOTOR_MESH_H