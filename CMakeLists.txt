cmake_minimum_required(VERSION 4.0)

project(motorcito VERSION 0.1 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### ENGINE SECTION
FetchContent_Declare(
        bgfx
        GIT_REPOSITORY "https://github.com/bkaradzic/bgfx.cmake.git"
        GIT_TAG        "v1.139.9155-513"
)
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG        "release-3.4.2"
)
FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics.git"
        GIT_TAG        "v5.5.0"
        SOURCE_SUBDIR  "Build"
)
FetchContent_Declare(
        SoLoud
        GIT_REPOSITORY "https://github.com/jarikomppa/soloud.git"
        GIT_TAG        "RELEASE_20200207"
)
FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm.git"
        GIT_TAG        "1.0.3"
)
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
FetchContent_MakeAvailable(bgfx SDL3 JoltPhysics SoLoud glm json stb)


# Engine library
file(GLOB_RECURSE ENGINE_SOURCES "engine/src/*.cpp")
add_library(motorcito STATIC ${ENGINE_SOURCES})
target_include_directories(motorcito PUBLIC engine/src)
target_link_libraries(motorcito PUBLIC
        SDL3::SDL3
        bgfx
        Jolt
        glm::glm
)

### GAME SECTION

file(GLOB_RECURSE GAME_SOURCES "game/src/*.cpp")

add_executable(game ${GAME_SOURCES})
target_link_libraries(game PRIVATE motorcito)

# Platform specifics
if(APPLE)
    if(IOS)
        set_target_properties(game PROPERTIES
                MACOSX_BUNDLE TRUE
                XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
        )
    endif()
endif()

if(ANDROID)
    add_library(game SHARED game/main.cpp) # Android necesita .so
endif()