cmake_minimum_required(VERSION 3.15)
project(tournament)

add_compile_options(
  -O3 -march=native -mtune=generic -ffast-math -DNDEBUG -lto)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-DGLEW_STATIC)

set(GLFW_DIR "external/glfw-3.3.9")
set(GLM_DIR "external/glm")
set(IMGUI_DIR "external/imgui")

include_directories(${GLEW_DIR}/include)
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
include_directories(external/glew-2.1.0/include)
include_directories(include)
include_directories(external)

add_subdirectory(${GLFW_DIR})

find_package(OpenGL REQUIRED)

set(SOURCE_FILES
  src/main.cpp
  src/Renderer.cpp
  src/AppElement.cpp
  src/DrawList.cpp
  src/App.cpp
  src/Scene.cpp
  src/Controls.cpp
  src/Util.cpp
  src/Shader.cpp
  src/GUI.cpp
  src/Translator.cpp
  src/Pair.cpp
  src/Projectile.cpp
  src/Target.cpp
  src/Trajectory.cpp
  src/Body.cpp
  src/Model.cpp
  src/QuaternionUtil.cpp
  src/Noise.cpp
  src/Settings.cpp
  src/Terrain.cpp
  src/Obstacle.cpp
  src/Removable.cpp

  external/stb_perlin.cpp

  external/imgui/backends/imgui_impl_glfw.cpp
  external/imgui/backends/imgui_impl_opengl3.cpp

  external/glew-2.1.0/src/glew.c

  external/imgui/imgui.cpp
  external/imgui/imgui_tables.cpp
  external/imgui/imgui_demo.cpp
  external/imgui/imgui_widgets.cpp
  external/imgui/imgui_draw.cpp
)

add_executable(tournament ${SOURCE_FILES})

target_link_libraries(tournament glfw OpenGL::GL)

if (WIN32)
    target_link_libraries(tournament opengl32)
elseif (UNIX)
    target_link_libraries(tournament dl)
endif()

