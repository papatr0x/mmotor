//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_SHADERPROGRAM_H
#define MMOTOR_SHADERPROGRAM_H

#pragma once
#include <bgfx/bgfx.h>
#include <string>

class ShaderProgram {
public:
    bool load(const std::string& vsPath, const std::string& fsPath);
    void destroy();

    bgfx::ProgramHandle handle() const { return m_program; }
    bool isValid() const { return bgfx::isValid(m_program); }

private:
    bgfx::ShaderHandle loadShader(const std::string& path);
    bgfx::ProgramHandle m_program = BGFX_INVALID_HANDLE;
};

#endif //MMOTOR_SHADERPROGRAM_H