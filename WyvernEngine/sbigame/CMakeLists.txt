cmake_minimum_required(VERSION 2.9)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wendif-labels -Wall -DGLEW_STATIC -Wunused-function -m64")

include_directories(${CMAKE_SOURCE_DIR}/source)

set(SOURCE_FILES_SBI_GAME source/main.cpp source/Entities.h source/Entities.cpp
        source/scripts/FreeMove.cpp source/scripts/FreeMove.h
        source/level/Level.h source/level/Level.cpp
        source/UI.cpp source/UI.h
        source/WorldManager.cpp source/WorldManager.h source/components/SegmentRef.cpp source/components/SegmentRef.h source/scripts/PlayerController.cpp source/scripts/PlayerController.h source/scripts/HammerScript.cpp source/scripts/HammerScript.h source/scripts/SwingScript.cpp source/scripts/SwingScript.h)
#if(WINDOWS)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-pthreads")
#endif()


add_executable(sbi_game ${SOURCE_FILES_SBI_GAME})

if (UNIX)
    target_link_libraries(sbi_game GLEW GL glfw3 X11 Xrandr Xinerama Xxf86vm Xcursor pthread rt m dl nanogui WyvernEngine)
    #    target_link_libraries(sbi_game GLEW GL ${GLFW_STATIC_LIBRARIES} nanogui ${FREETYPE_LIBRARIES} WyvernEngine)
else ()
    target_link_libraries(sbi_game glew32s glfw3 opengl32 WyvernEngine)
endif ()