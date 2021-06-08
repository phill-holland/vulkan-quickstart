cmake_minimum_required(VERSION 3.0)

project(sadistic)

set(CMAKE_BUILD_TYPE "Debug")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(JPEG REQUIRED)

include_directories("steam")
include_directories("/usr/include/SDL2")
include_directories("include")

file(GLOB SOURCE CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.cpp")

link_directories("steam/lib/linux64")

find_library(SDL2_MIXER_LIBRARY
  NAMES SDL2_mixer
  HINTS
    ENV SDL2MIXERDIR
    ENV SDL2DIR
    ${SDL2_MIXER_NO_DEFAULT_PATH_CMD}
  PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
  PATHS ${SDL2_MIXER_PATH}
  DOC "Where the SDL2_mixer Library can be found"
)

add_library(SDL2::Mixer UNKNOWN IMPORTED)

find_library(SDL2_LIBRARY
  NAMES SDL2 SDL-2.0
  HINTS
    ENV SDL2DIR
    ${SDL2_NO_DEFAULT_PATH_CMD}
  PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
  PATHS ${SDL2_PATH}
  DOC "Where the SDL2 Library can be found"
)

add_library(SDL2::Main INTERFACE IMPORTED)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} X11)
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${SDL2_MIXER_LIBRARY})