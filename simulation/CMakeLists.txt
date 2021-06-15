cmake_minimum_required(VERSION 3.0)

project(world-simulation)

set(CMAKE_BUILD_TYPE "Debug")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(JPEG REQUIRED)
find_package(Vulkan)
find_package(X11 REQUIRED)

include_directories("include")
include_directories(${Vulkan_INCLUDE_DIR})

set(ALL_LIBS ${Vulkan_LIBRARY})

file(GLOB SOURCE CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${ALL_LIBS})
target_link_libraries(${PROJECT_NAME} wayland-client)
target_link_libraries(${PROJECT_NAME} X11)
