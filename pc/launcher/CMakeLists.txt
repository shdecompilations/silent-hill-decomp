cmake_minimum_required(VERSION 3.20)
project(SilentHillLauncher CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

find_package(SDL2 QUIET)
if(NOT SDL2_FOUND)
  FetchContent_Declare(SDL2 GIT_REPOSITORY https://github.com/libsdl-org/SDL.git GIT_TAG release-2.30.8)
  set(SDL_TEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL2)
endif()

find_package(OpenGL REQUIRED)

# Dear ImGui — fetched as source, no install needed
FetchContent_Declare(imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.9
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(imgui PUBLIC SDL2::SDL2 OpenGL::GL)

add_executable(silenthill-launcher
  src/launcher.cpp
)

target_link_libraries(silenthill-launcher PRIVATE imgui SDL2::SDL2 OpenGL::GL)
target_include_directories(silenthill-launcher PRIVATE
  ${CMAKE_SOURCE_DIR}/../../include
  ${CMAKE_SOURCE_DIR}/../include
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)

# On Linux need pthread/dl
if(UNIX AND NOT APPLE)
  target_link_libraries(silenthill-launcher PRIVATE pthread dl)
endif()

message(STATUS "Launcher: ImGui ${imgui_SOURCE_DIR}")
