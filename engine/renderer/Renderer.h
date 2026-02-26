//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_RENDERER_H
#define MMOTOR_RENDERER_H

#pragma once

#include "ShaderProgram.h"
#include "Mesh.h"

class Renderer {
public:
    bool init(void* nativeWindowHandle, void* nativeDisplayHandle, int width, int height);
    void beginFrame();
    void endFrame();
    void shutdown();
    void onResize(int width, int height);

    void submit(const Mesh& mesh, const ShaderProgram& shader);

    int width()  const { return m_width; }
    int height() const { return m_height; }

private:
    int  m_width{0};
    int  m_height{0};
    bool m_initialized{false};
};

#endif //MMOTOR_RENDERER_H