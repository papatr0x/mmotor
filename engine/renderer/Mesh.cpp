#include "Mesh.h"

bool Mesh::create(const std::vector<PosColorVertex>& vertices,
                  const std::vector<uint16_t>& indices) {
    auto layout = PosColorVertex::layout();

    m_vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(vertices.data(), sizeof(PosColorVertex) * vertices.size()),
        layout
    );

    m_ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(indices.data(), sizeof(uint16_t) * indices.size())
    );

    return isValid();
}

void Mesh::destroy() {
    if (bgfx::isValid(m_vbh)) bgfx::destroy(m_vbh);
    if (bgfx::isValid(m_ibh)) bgfx::destroy(m_ibh);
    m_vbh = BGFX_INVALID_HANDLE;
    m_ibh = BGFX_INVALID_HANDLE;
}