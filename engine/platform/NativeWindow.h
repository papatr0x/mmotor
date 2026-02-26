//
// Created by Patricio Palma on 26-02-26.
//

#ifndef MMOTOR_NATIVEWINDOW_H
#define MMOTOR_NATIVEWINDOW_H
#pragma once
#include <SDL3/SDL.h>

struct NativeHandles {
    void* windowHandle{nullptr};
    void* displayHandle{nullptr}; // solo Linux
};

inline NativeHandles getNativeHandles(SDL_Window* window) {
    NativeHandles handles{};

#if defined(__APPLE__)
    // macOS/iOS — necesitamos el CAMetalLayer
    handles.windowHandle = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);

#elif defined(_WIN32)
    handles.windowHandle = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);

#elif defined(__ANDROID__)
    handles.windowHandle = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_ANDROID_WINDOW_POINTER, nullptr);

#elif defined(__linux__)
    handles.windowHandle = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_X11_WINDOW_NUMBER, nullptr);
    handles.displayHandle = SDL_GetPointerProperty(
        SDL_GetWindowProperties(window),
        SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
#endif

    return handles;
}
#endif //MMOTOR_NATIVEWINDOW_H