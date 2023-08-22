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
                           ${GLAD_DIR}/include)
target_link_libraries(glad_hellowindow2
                      glad
                      glfw)

add_executable(learnopengl_hellowindow
               ${PROJECT_SOURCE_DIR}/tests/learnopengl_hellowindow.cpp)
target_include_directories(learnopengl_hellowindow PRIVATE
                           ${PROJECT_SOURCE_DIR}/include
                           ${GLAD_DIR}/include)
target_link_libraries(learnopengl_hellowindow
                      glad
                      glfw)

add_executable(learnopengl_hellotriangle
               ${PROJECT_SOURCE_DIR}/tests/learnopengl_hellotriangle.cpp)
target_include_directories(learnopengl_hellotriangle PRIVATE
                           ${PROJECT_SOURCE_DIR}/include
                           ${GLAD_DIR}/include)
target_link_libraries(learnopengl_hellotriangle
                      glad
                      glfw)

add_executable(hot_swap_shader
               ${PROJECT_SOURCE_DIR}/tests/hot_swap_shader.cpp)
target_include_directories(hot_swap_shader PRIVATE
                           ${PROJECT_SOURCE_DIR}/include
                           ${GLAD_DIR}/include)
target_link_libraries(hot_swap_shader
                      glad
                      glfw)
