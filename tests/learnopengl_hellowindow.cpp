/**
 * learnopengl_hellowindow.cpp
 * 
 * Based on the LearnOpenGL article at https://learnopengl.com/Getting-started/Hello-Window
*/


#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <iostream>


// Updated when user resizes window
int window_width = 800;
int window_height = 600;


void framebuffer_size_callback(GLFWwindow*, int width, int height);
void process_input(GLFWwindow*);


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Set GL viewport size to fill window
    glViewport(0, 0, window_width, window_height);
    // Set GL viewport to automatically resize when the window is resized
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


/**
 * framebuffer_size_callback
 * 
 * Callback for GLFW to process window resize events.
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    window_width = width;
    window_height = height;
    glViewport(0, 0, window_width, window_height);
}


/**
 * process_input
 * 
 * Process GLFW window input.
*/
void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
