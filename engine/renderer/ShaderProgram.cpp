//
// Created by Patricio Palma on 26-02-26.
//
#include "ShaderProgram.h"
#include <fstream>
#include <vector>
#include <iostream>

bgfx::ShaderHandle ShaderProgram::loadShader(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open shader: " << path << "\n";
        return BGFX_INVALID_HANDLE;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // bgfx necesita memoria con un byte extra al final
    const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size + 1));
    file.read(reinterpret_cast<char*>(mem->data), size);
    mem->data[size] = '\0';

    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    if (!bgfx::isValid(handle)) {
        std::cerr << "Failed to create shader from: " << path << "\n";
    }
    return handle;
}

bool ShaderProgram::load(const std::string& vsPath, const std::string& fsPath) {
    bgfx::ShaderHandle vs = loadShader(vsPath);
    bgfx::ShaderHandle fs = loadShader(fsPath);

    if (!bgfx::isValid(vs) || !bgfx::isValid(fs)) {
        return false;
    }

    m_program = bgfx::createProgram(vs, fs, true); // true = destroy shaders when program is destroyed
    return bgfx::isValid(m_program);
}

void ShaderProgram::destroy() {
    if (bgfx::isValid(m_program)) {
        bgfx::destroy(m_program);
        m_program = BGFX_INVALID_HANDLE;
    }
}