cmake_minimum_required(VERSION 3.14)

SET(PROJ_NAME "Game")
project(${PROJ_NAME})

set(CMAKE_CXX_STANDARD 14)

file(GLOB GAME Game/*.cpp Game/*.h)
file(GLOB ENGINE Engine3D/*.cpp Engine3D/*.c  Engine3D/*.h)
file(GLOB DISPLAY DisplayGLFW/*.cpp DisplayGLFW/*.h)

include_directories(${PROJECT_SOURCE_DIR}/res/includes ${PROJECT_SOURCE_DIR}/Engine3D
 ${PROJECT_SOURCE_DIR}/DisplayGLFW)
#add_subdirectory(${PROJECT_SOURCE_DIR}/res)

add_library(Display ${DISPLAY})
add_library(Engine ${ENGINE})

add_executable(${PROJ_NAME} ${GAME})

target_link_libraries(${PROJ_NAME} Engine Display ${PROJECT_SOURCE_DIR}/res/libs/glew32s.lib ${PROJECT_SOURCE_DIR}/res/libs/glfw3.lib opengl32.lib)

