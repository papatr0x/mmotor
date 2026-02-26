cmake_minimum_required(VERSION 4.0)

project(MMotor VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### ENGINE SECTION

set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BGFX_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        bgfx
        GIT_REPOSITORY "https://github.com/bkaradzic/bgfx.cmake.git"
        GIT_TAG        "v1.139.9155-513"
)

set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG        "release-3.4.2"
)

set(TARGET_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(TARGET_HELLO_WORLD OFF CACHE BOOL "" FORCE)
set(TARGET_PERFORMANCE_TEST OFF CACHE BOOL "" FORCE)
set(TARGET_SAMPLES OFF CACHE BOOL "" FORCE)
set(TARGET_VIEWER OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics.git"
        GIT_TAG        "v5.5.0"
        SOURCE_SUBDIR  "Build"
)
FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY "https://github.com/mackron/miniaudio.git"
        GIT_TAG        "0.11.24"
)
set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG        "1.0.3"
)
set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
        json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG        "v3.12.0"
)
FetchContent_Declare(
        stb
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG        "master"
)
FetchContent_MakeAvailable(bgfx SDL3 JoltPhysics json miniaudio glm stb)

# Engine library
file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")
add_library(MMotorEngine STATIC ${ENGINE_SOURCES})
target_include_directories(MMotorEngine PUBLIC engine)
target_link_libraries(MMotorEngine PUBLIC
        SDL3::SDL3
        bgfx
        Jolt
        miniaudio
)

### GAME SECTION

file(GLOB_RECURSE GAME_SOURCES "game/src/*.cpp")
add_executable(MMotorGame ${GAME_SOURCES})
target_link_libraries(MMotorGame PRIVATE MMotorEngine)
