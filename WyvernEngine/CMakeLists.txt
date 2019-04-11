cmake_minimum_required(VERSION 2.9)
set(PROJECT_NAME WyvernEngine)
project(${PROJECT_NAME})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libstdc++ -static-libgcc -Wendif-labels -Wall -DGLEW_STATIC -Wunused-function -m64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -static-libstdc++  -Wendif-labels -Wall -DGLEW_STATIC -DGLFW_STATIC -Wunused-function -m64")

#set(CMAKE_BUILD_TYPE release)

#if(WINDOWS)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-pthreads")
#endif()

#stb_iamge.h
include_directories(SYSTEM dependency/include)

if (UNIX)
    #find_package(PkgConfig REQUIRED)
    ##find_package(Freetype REQUIRED)
    #pkg_search_module(GLFW REQUIRED glfw3)

link_directories(dependency/linux/libs)
else ()
    include_directories(SYSTEM dependency/windows/include)
    link_directories(dependency/windows/libs)
endif ()

set(SOURCE_FILES source/Entity.cpp source/Entity.h source/World.cpp source/World.h
        source/EntityComponent.h source/EntityComponent.cpp
        source/components/Transform.cpp source/components/Transform.h source/Helper.h source/Helper.cpp
        source/Window.cpp source/Window.h source/rendering/Shader.cpp source/rendering/Shader.h
        source/components/Camera.cpp source/components/Camera.h source/components/MeshRenderer.cpp
        source/components/MeshRenderer.h source/rendering/Mesh.cpp source/rendering/Mesh.h
        source/rendering/Mesh.cpp source/rendering/Mesh.h
        source/rendering/RenderingSystem.cpp source/rendering/RenderingSystem.h
        source/rendering/Material.cpp source/rendering/Material.h
        source/rendering/Texture.cpp source/rendering/Texture.h
        source/ResourceManager.cpp source/ResourceManager.h source/Input.cpp source/Input.h
        source/components/Material.cpp source/components/Material.h
        source/rendering/DebugRender.cpp source/rendering/DebugRender.h
        source/components/DebugRenderer.cpp source/components/DebugRenderer.h source/Profiler.cpp source/Profiler.h source/components/CollisionBoxGroup.cpp source/components/CollisionBoxGroup.h)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} GLEW glfw3 X11 Xrandr Xinerama Xxf86vm Xcursor pthread rt m dl nanogui)
else ()
    target_link_libraries(${PROJECT_NAME} glew32s opengl32 glfw3 nanogui)
endif ()

add_subdirectory(sbigame)