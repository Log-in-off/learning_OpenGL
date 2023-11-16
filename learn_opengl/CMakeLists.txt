cmake_minimum_required(VERSION 3.13) # possibly not strict
cmake_policy(VERSION 3.0)
project(window)


set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

#set policy for OpenGL
cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
# note that the order is important for setting the libs
# use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

add_subdirectory(libs/glad_lib)
set(LIBS ${LIBS} FGLAD)

add_subdirectory(libs/shader_wrapper)
set(LIBS ${LIBS} shaderWrapper)



add_executable(window1  src/1_triangle/main.cpp)
target_link_libraries(window1 PUBLIC ${LIBS})

add_executable(window2 src/2_rectangle/main.cpp)
target_link_libraries(window2 ${LIBS})

add_executable(window3 src/3_task__two_triangles/main.cpp)
target_link_libraries(window3 ${LIBS})

add_executable(window4 src/4_task__two_triangles_up/main.cpp)
target_link_libraries(window4 ${LIBS})

add_executable(window5 src/5_task__two_triangles_and_two_shaders/main.cpp)
target_link_libraries(window5 ${LIBS})

add_executable(window6 src/6_task__invert_triangle/main.cpp)
target_link_libraries(window6 ${LIBS})

add_executable(window7 src/7_task__moving_triangle/main.cpp)
target_link_libraries(window7 ${LIBS})

add_executable(window8 src/8_task__black_triangle/main.cpp)
target_link_libraries(window8 ${LIBS})

add_executable(window9 src/9_texture/main.cpp)
target_link_libraries(window9 ${LIBS})

add_executable(window10 src/10_two_textures/main.cpp)
target_link_libraries(window10 ${LIBS})

add_executable(window11 src/11_rotate_texture/main.cpp)
target_link_libraries(window11 ${LIBS})

add_executable(window12 src/12_coordinate_systems/main.cpp)
#target_link_libraries(window12 PUBLIC ${LIBS} PUBLIC shaderWrapper)
target_link_libraries(window12 ${LIBS})

add_executable(window13 src/13_cube/main.cpp)
target_link_libraries(window13 ${LIBS})

add_executable(window14 src/14_camera_and_view_space/main.cpp)
target_link_libraries(window14 ${LIBS})

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glad_lib/include)
