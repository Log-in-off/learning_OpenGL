add_library(FGLAD src/glad.c)

target_include_directories(FGLAD INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
