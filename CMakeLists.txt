cmake_minimum_required(VERSION 2.8.11)
cmake_policy(VERSION 2.8.11)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(PROJECT_NAME 3D_engine)
set(EXECUTABLE_NAME test_engine)

project(${PROJECT_NAME})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -g -O3")

add_executable(
  ${EXECUTABLE_NAME}
  src/main.c
  src/window.c
  src/input.c
	src/scene.c
	src/model.c
  src/camera.c
  src/utils.c
  src/shadow.c
  src/primitives.c
  src/light.c
  src/materials.c
  src/rendering.c
  src/shader.c
)


INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${SDL2_LIBRARIES})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${OPENGL_glu_LIBRARY})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${GLUT_LIBRARIES})
TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${GLEW_LIBRARIES})

TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} m)
