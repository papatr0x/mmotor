//
// Created by Patricio Palma on 26-02-26.
//

#include "Renderer.h"
#include <iostream>

bool Renderer::init(void* nativeWindowHandle, void* nativeDisplayHandle, int width, int height) {
    m_width  = width;
    m_height = height;

    // Pass native window handle to bgfx
    bgfx::PlatformData pd{};
    pd.nwh  = nativeWindowHandle;
    // pd.ndt  = nativeDisplayHandle; // required for linux?

    // Init bgfx — choose Metal on macOS, DX12/Vulkan on Windows
    bgfx::Init init;
    init.type              = bgfx::RendererType::Count; // Auto-detect
    init.resolution.width  = static_cast<uint32_t>(width);
    init.resolution.height = static_cast<uint32_t>(height);
    init.resolution.reset  = BGFX_RESET_VSYNC;
    init.platformData      = pd;

    // For apple you MUST call this to avoid a deadlock calling bgfx::init()
    bgfx::renderFrame();

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
    bgfx::renderFrame(); // Required in single-thread mode
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