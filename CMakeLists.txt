cmake_minimum_required(VERSION 3.5...3.20)

project(GLFragmentShader)


###############################################################################
# DEPENDENCIES
###############################################################################

set(GLAD_DIR "${PROJECT_SOURCE_DIR}/deps/glad_gl_core_33")
set(GLFW_DIR "${PROJECT_SOURCE_DIR}/deps/glfw")

add_subdirectory(${GLFW_DIR})
add_library(glad ${GLAD_DIR}/src/gl.c
            ${GLAD_DIR}/include/glad/gl.h
            ${GLAD_DIR}/include/KHR/khrplatform.h)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)


###############################################################################
# SRC
###############################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/src)


###############################################################################
# TESTS
###############################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)


###############################################################################
# EXAMPLES
###############################################################################

add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
