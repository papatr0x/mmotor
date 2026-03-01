//
// Created by Patricio Palma on 26-02-26.
//

#include "Renderer.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bx/math.h>
#include <iostream>

bool Renderer::init(void* nativeWindowHandle, void* nativeDisplayHandle, int width, int height) {
    m_width  = width;
    m_height = height;

    // Pass native window handle to bgfx
    bgfx::PlatformData pd{};
    pd.nwh  = nativeWindowHandle;

    // Init bgfx — choose Metal on macOS, DX12/Vulkan on Windows
    bgfx::Init init;
    init.type              = bgfx::RendererType::Count; // Auto-detect
    init.resolution.width  = static_cast<uint32_t>(width);
    init.resolution.height = static_cast<uint32_t>(height);
    init.resolution.reset  = BGFX_RESET_VSYNC;
    init.platformData      = pd;

    // For apple you MUST call this to avoid a deadlock calling bgfx::init()
    // this enable single threaded mode
    // bgfx::renderFrame();

    if (!bgfx::init(init)) {
        std::cerr << "bgfx::init failed\n";
        return false;
    }

    // Color de clear — gris oscuro
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x3a3a3aff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0,
        static_cast<uint16_t>(width),
        static_cast<uint16_t>(height));

    m_initialized = true;
    std::cout << "Renderer initialized ("
              << bgfx::getRendererName(bgfx::getRendererType()) << ")\n";
    return true;
}

void Renderer::beginFrame() {
    // bgfx::renderFrame(); // Required in single-thread mode
    bgfx::setViewRect(0, 0, 0,
        static_cast<uint16_t>(m_width),
        static_cast<uint16_t>(m_height));
    bgfx::touch(0); // Force render in view 0, even with no drawcalls
}

void Renderer::endFrame() {
    bgfx::frame();
}

void Renderer::onResize(int width, int height) {
    m_width  = width;
    m_height = height;
    bgfx::reset(
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        BGFX_RESET_VSYNC);
}

void Renderer::shutdown() {
    if (m_initialized) {
        bgfx::shutdown();
        m_initialized = false;
        std::cout << "Renderer shutdown\n";
    }
}

void Renderer::submit(const Mesh& mesh, const ShaderProgram& shader, const Transform& transform) {
    if (!mesh.isValid() || !shader.isValid()) return;

    glm::mat4 t = glm::translate(glm::mat4(1.0f), transform.position);
    glm::mat4 r = glm::mat4_cast(transform.rotation);
    glm::mat4 s = glm::scale(glm::mat4(1.0f), transform.scale);
    glm::mat4 m = t * r * s;

    float model[16];
    memcpy(model, glm::value_ptr(m), sizeof(float) * 16);
    bgfx::setTransform(model);

    bgfx::setVertexBuffer(0, mesh.vbh());
    bgfx::setIndexBuffer(mesh.ibh());
    bgfx::setState(BGFX_STATE_DEFAULT);
    bgfx::submit(0, shader.handle());
}

void Renderer::setCamera(const glm::mat4& view, const glm::mat4& proj) {
    m_view = view;
    m_proj = proj;

    // Convertir a float arrays para bgfx
    float viewArr[16];
    float projArr[16];
    memcpy(viewArr, glm::value_ptr(view), sizeof(float) * 16);
    memcpy(projArr, glm::value_ptr(proj), sizeof(float) * 16);

    bgfx::setViewTransform(0, viewArr, projArr);
}