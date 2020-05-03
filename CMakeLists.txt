cmake_minimum_required(VERSION 2.6)
project(RAPTORBAKERY)

include_directories(
  /usr/X11R6/include
  /usr/local/include
  ${PROJECT_SOURCE_DIR}/src
)

link_directories(
  /usr/X11R6/lib
  /usr/local/lib
)

add_library(re
  src/re/Context.c
  src/re/Renderer.c
  src/re/mathutil.c
  src/re/Buffer.c
  src/re/internal.c
  src/re/Shader.c
  src/re/Mesh.c
  src/re/Texture.c
  src/re/util.c
)
if(WIN32)
  target_link_libraries(re glew32 opengl32 winmm imm32 version setupapi)
else()
  target_link_libraries(re GLEW GL m)
endif()

add_executable(rpreview
  src/rpreview/main.c
  src/rpreview/Model.c
)
target_link_libraries(rpreview re SDL2)

