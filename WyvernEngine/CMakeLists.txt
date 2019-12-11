cmake_minimum_required(VERSION 2.9)
project(WyvernEngine)


if(WINDOWS)
    message( FATAL_ERROR "Windows is not supported")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libstdc++  -Wendif-labels -Wall -DGLEW_STATIC -DGLFW_STATIC -Wunused-function -m64")
# cmake_policy(SET CMP0079 NEW)

add_library(WyvernEngine 
    source/Entity.cpp source/Entity.h 
    source/World.cpp source/World.h
    source/EntityComponent.h source/EntityComponent.cpp
    source/components/Transform.cpp source/components/Transform.h 
    source/Helper.h source/Helper.cpp
    source/Window.cpp source/Window.h 
    source/rendering/Shader.cpp source/rendering/Shader.h
    source/components/Camera.cpp source/components/Camera.h 
    source/components/MeshRenderer.cpp source/components/MeshRenderer.h 
    source/rendering/Mesh.cpp source/rendering/Mesh.h
    source/rendering/RenderingSystem.cpp source/rendering/RenderingSystem.h
    source/rendering/Material.cpp source/rendering/Material.h
    source/rendering/Texture.cpp source/rendering/Texture.h
    source/ResourceManager.cpp source/ResourceManager.h 
    source/Input.cpp source/Input.h
    source/components/Material.cpp source/components/Material.h
    source/rendering/DebugRender.cpp source/rendering/DebugRender.h
    source/Profiler.cpp source/Profiler.h 
    source/components/CollisionBoxGroup.cpp source/components/CollisionBoxGroup.h
)

set_property(TARGET WyvernEngine PROPERTY CXX_STANDARD 11)

add_subdirectory(vendors/nanogui EXCLUDE_FROM_ALL)
target_link_libraries(WyvernEngine PUBLIC nanogui)
target_include_directories(WyvernEngine PUBLIC vendors/nanogui/include)
target_include_directories(WyvernEngine PUBLIC vendors/nanogui/ext/glfw/include)
target_include_directories(WyvernEngine PUBLIC vendors/nanogui/ext/eigen)
target_include_directories(WyvernEngine PUBLIC vendors/nanogui/ext/nanovg/src)
add_subdirectory(vendors/glm EXCLUDE_FROM_ALL)
target_include_directories(WyvernEngine PUBLIC vendors/glm)

target_link_libraries(WyvernEngine PUBLIC GLEW X11 Xrandr Xinerama Xxf86vm Xcursor pthread rt m dl)

add_subdirectory(sbigame)