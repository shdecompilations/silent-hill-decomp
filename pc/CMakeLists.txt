cmake_minimum_required(VERSION 3.20)
project(SilentHillPC C CXX)

# Silent Hill PC Port — Recompilation target
# Builds the decompiled PSX C sources as a native x86_64 executable
# using SDL2 + bgfx (or sdl-gpu fallback) instead of PSX psyq libs.
#
# Usage:
#   cmake -S pc -B build-pc -DCMAKE_BUILD_TYPE=RelWithDebInfo
#   cmake --build build-pc -j
#   ./build-pc/silenthill
#
# Requires original game assets extracted to `assets/USA/` and `rom/USA/`
# (from `make extract` with a retail disc image).

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(SH_USE_BGFX "Use bgfx for rendering (otherwise SDL_GPU)" OFF)
option(SH_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(SH_MATCHING "Verify matching against PSX binary (adds -Werror)" OFF)

# ---------------------------------------------------------------------------
# Platform detection
# ---------------------------------------------------------------------------
if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX)
endif()

# ---------------------------------------------------------------------------
# Dependencies — fetched via FetchContent if not found system-wide
# ---------------------------------------------------------------------------
include(FetchContent)

find_package(SDL2 QUIET)
if (NOT SDL2_FOUND)
    message(STATUS "SDL2 not found system-wide, fetching via FetchContent")
    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.8
    )
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL2)
endif()

if (SH_USE_BGFX)
    # bgfx + bx + bimg
    FetchContent_Declare(bgfx
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(bgfx)
    add_compile_definitions(SH_RENDER_BGFX=1)
else()
    # SDL3 GPU would be ideal, but SDL2 + raw OpenGL is the fallback we ship
    find_package(OpenGL REQUIRED)
    add_compile_definitions(SH_RENDER_GL=1)
endif()

# ---------------------------------------------------------------------------
# Core compile definitions — matches original but retargeted
# ---------------------------------------------------------------------------
add_compile_definitions(
    PC_PORT=1
    VER_USA=1
    _LANGUAGE_C
    USE_INCLUDE_ASM=0
    SKIP_ASM=1
    NON_MATCHING=1
)

# Make PSX VRAM addresses relocatable — the original hardcodes 0x800C9578 etc.
# On PC we allocate overlays dynamically and fix up pointers at load time.
add_compile_definitions(
    SH_SHIFTABLE=1
    SH_PC_RELOCATABLE_OVERLAYS=1
)

# Force-include PC compat header for every translation unit
add_compile_options(-include ${CMAKE_SOURCE_DIR}/include/pc_compat.h)

# Warnings
add_compile_options(-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-builtin-macro-redefined)

if (SH_ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

# ---------------------------------------------------------------------------
# Include paths — mirror original Makefile's -Iinclude etc., plus PC shims
# ---------------------------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../src
    ${CMAKE_SOURCE_DIR}/../assets/USA
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../build/USA  # generated headers
)

# PC compat header lives in pc/include/pc_compat.h
include_directories(${CMAKE_SOURCE_DIR}/include)

# ---------------------------------------------------------------------------
# Collect decompiled sources — same list as Makefile, but for host compiler
# ---------------------------------------------------------------------------

# Main executable (SLUS_007.07)
file(GLOB MAIN_SOURCES
    ${CMAKE_SOURCE_DIR}/../src/main/*.c
)

# Bodyprog (BODYPROG.BIN) — engine
file(GLOB BODYPROG_SOURCES
    ${CMAKE_SOURCE_DIR}/../src/bodyprog/*.c
    ${CMAKE_SOURCE_DIR}/../src/bodyprog/**/*.c
)

# Screens
file(GLOB SCREEN_SOURCES
    ${CMAKE_SOURCE_DIR}/../src/screens/**/*.c
)

# Maps — use BUILD_MAP filter if set, else all
if (DEFINED ENV{BUILD_MAP})
    file(GLOB MAP_SOURCES
        ${CMAKE_SOURCE_DIR}/../src/maps/$ENV{BUILD_MAP}/*.c
    )
else()
    file(GLOB MAP_SOURCES
        ${CMAKE_SOURCE_DIR}/../src/maps/*.c
        ${CMAKE_SOURCE_DIR}/../src/maps/**/*.c
    )
endif()

# PC platform layer
file(GLOB PC_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/**/*.c
)

# Exclude lib/* PSX assembly — we use C fallbacks
list(FILTER BODYPROG_SOURCES EXCLUDE REGEX ".*libkpad.*|.*libsd.*")

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------
add_executable(silenthill
    ${MAIN_SOURCES}
    ${BODYPROG_SOURCES}
    ${SCREEN_SOURCES}
    ${MAP_SOURCES}
    ${PC_SOURCES}
)

target_include_directories(silenthill PRIVATE
    ${CMAKE_SOURCE_DIR}/../include
    ${CMAKE_SOURCE_DIR}/../include/psyq
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/pc
)

target_link_libraries(silenthill PRIVATE
    SDL2::SDL2
    m
)

if (SH_USE_BGFX)
    target_link_libraries(silenthill PRIVATE bgfx bx bimg)
else()
    target_link_libraries(silenthill PRIVATE OpenGL::GL)
endif()

# On Linux, need pthread, dl
if (UNIX AND NOT APPLE)
    target_link_libraries(silenthill PRIVATE pthread dl)
endif()

# ---------------------------------------------------------------------------
# Custom target: verify matching still holds for PSX build
# ---------------------------------------------------------------------------
add_custom_target(check-matching
    COMMAND make -C ${CMAKE_SOURCE_DIR}/.. GAME_VERSION=USA CHECKSUM=1 build
    COMMENT "Verifying PSX build still matches retail binary"
)

# ---------------------------------------------------------------------------
# Install / packaging
# ---------------------------------------------------------------------------
install(TARGETS silenthill DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/README.md DESTINATION share/doc/silenthill)

message(STATUS "Silent Hill PC port configured:")
message(STATUS "  Renderer: ${SH_USE_BGFX}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Sources: main=${MAIN_SOURCES} bodyprog=${BODYPROG_SOURCES} maps=${MAP_SOURCES}")
