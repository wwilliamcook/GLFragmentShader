###############################################################################
# DEPENDENCIES
###############################################################################

set(GLAD_DIR "${PROJECT_SOURCE_DIR}/deps/glad_gl_core_33")


###############################################################################
# TESTS
###############################################################################

add_executable(glad_hellowindow2
               ${PROJECT_SOURCE_DIR}/tests/glad_hellowindow2.cpp)
target_include_directories(glad_hellowindow2 PRIVATE
                           ${PROJECT_SOURCE_DIR}/include
                           ${GLAD_DIR}/include
                           ${PROJECT_SOURCE_DIR}/deps/glfw/deps)
target_link_libraries(glad_hellowindow2
                      glad
                      glfw)