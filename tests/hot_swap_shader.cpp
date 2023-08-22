/**
 * main.cpp
 * 
 * Full screen fragment shader with no inputs
 * 
 * Based on the LearnOpenGL article at https://learnopengl.com/Getting-started/Hello-Triangle
*/


#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>


// Updated when user resizes window
int window_width = 800;
int window_height = 600;
char *fragmentShaderPath = NULL;
unsigned int shaderProgram = 0;


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "in vec4 gl_FragCoord;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(gl_FragCoord.x, gl_FragCoord.y, 0.0f, 1.0f);\n"
    "}\n\0";


// Forward declarations
void framebuffer_size_callback(GLFWwindow*, int, int);
void process_input(GLFWwindow*);
int read_shader_source(char*, char**);
unsigned int build_shader_program(char *fragmentShaderSource, int *success, char *infoLog);
class RisingEdgeDetector {
public:
    RisingEdgeDetector(void);
    bool operator()(bool current);
private:
    bool last;
};


int main(int argc, char* argv[]) {
    int i, j;
    char c;
    char *execName = (char*)malloc(strlen(argv[0]) + 1);

    // get name of executable
    j = 0;
    for (i = 0; i < strlen(argv[0]); i++) {
        if (argv[0][i] == '\\' || argv[0][i] == '/')
            j = i + 1;
    }
    for (i = 0; j < strlen(argv[0]); j++) {
        execName[i++] = argv[0][j];
    }
    execName[i] = '\0';

    // handle arguments
    if (argc == 2) {
        fragmentShaderPath = argv[1];
        std::cout << "Using fragment shader path: " << fragmentShaderPath << std::endl;
    } else {
        fragmentShaderPath = "hot_swap_shader.frag";
        std::cout << "Usage: " << execName << " [frag_shader_path]" << std::endl;
        std::cout << "Fragment shader path not specified." << std::endl;
        std::cout << "Using fragment shader path: " << fragmentShaderPath << std::endl;
    }
    std::cout << "\n\n";

    std::cout << "Controls:" << std::endl;
    std::cout << "\tESC - exit." << std::endl;
    std::cout << "\tR   - reload shader from file." << std::endl;
    std::cout << "\n\n";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "GLFragmentShader", NULL, NULL);
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

    // read shader from file if it exists
    // ------------------------------------
    char *fragmentShaderFileSource = NULL;
    int success;
    success = read_shader_source(fragmentShaderPath, &fragmentShaderFileSource);
    switch (success) {
    case 1:
        std::cout << "Shader file not found. Exiting." << std::endl;
        goto program_end;
        break;
    case -1:
        std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
        std::cout << "source == NULL" << std::endl;
        goto program_end;
        break;
    case -2:
        std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
        std::cout << "*source != NULL" << std::endl;
        goto program_end;
        break;
    }
    if (fragmentShaderFileSource == NULL) {
        std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
        std::cout << "*source == NULL" << std::endl;
        goto program_end;
    }

    // build and compile shader program
    // ------------------------------------
    char infoLog[512];
    shaderProgram = build_shader_program(fragmentShaderFileSource, &success, infoLog);
    switch (success) {
    case -1:
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        goto program_end;
        break;
    case -2:
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        goto program_end;
        break;
    case -3:
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        goto program_end;
        break;
    }
    if (shaderProgram == 0) {
        std::cout << "ERROR::SHADER::PROGRAM::BUILDING::FAILED\nUnknown error." << std::endl;
        goto program_end;
    }
    std::cout << "Successfully built shader." << std::endl;
    if (fragmentShaderFileSource != NULL) {
        free(fragmentShaderFileSource);
        fragmentShaderFileSource = NULL;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         1.0f,  1.0f, 0.0f,  // top right
         1.0f, -1.0f, 0.0f,  // bottom right
        -1.0f, -1.0f, 0.0f,  // bottom left
        -1.0f,  1.0f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // render
        // ------
        glClear(GL_COLOR_BUFFER_BIT);

        // draw across the whole viewport
        glUseProgram(shaderProgram);

        // update uniforms
        // ---------------
        // viewportSize
        int viewportSizeLocation = glGetUniformLocation(shaderProgram, "viewportSize");
        glUniform2f(viewportSizeLocation, (float)window_width, (float)window_height);
        // mousePos
        int mousePosLocation = glGetUniformLocation(shaderProgram, "mousePos");
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glUniform2f(mousePosLocation, (float)xpos, (float)ypos);
        // time
        int timeLocation = glGetUniformLocation(shaderProgram, "time");
        float timeValue = glfwGetTime();
        glUniform1f(timeLocation, timeValue);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

program_end:
    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    free(execName);
    execName = NULL;
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
    static RisingEdgeDetector osr_reload_shader_key;
    char *fragmentShaderFileSource = NULL;
    int success;
    char infoLog[512];
    unsigned int newShaderProgram;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (osr_reload_shader_key(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)) {
        // read shader from file if it exists
        success = read_shader_source(fragmentShaderPath, &fragmentShaderFileSource);
        switch (success) {
        case 0:
            // succeeded
            break;
        case 1:
            std::cout << "Shader file not found." << std::endl;
            break;
        case -1:
            std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
            std::cout << "source == NULL" << std::endl;
            break;
        case -2:
            std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
            std::cout << "*source != NULL" << std::endl;
            break;
        default:
            std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
            std::cout << "Unknown error: " << success << std::endl;
            break;
        }
        if (fragmentShaderFileSource == NULL) {
            std::cout << "ERROR::READ_SHADER_SOURCE" << std::endl;
            std::cout << "*source == NULL" << std::endl;
        }

        if (success == 0) {
            // build and compile shader program
            newShaderProgram = build_shader_program(fragmentShaderFileSource, &success, infoLog);
            switch (success) {
            case 0:
                // succeeded
                break;
            case -1:
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                break;
            case -2:
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                break;
            case -3:
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                break;
            default:
                std::cout << "ERROR::SHADER::UNKNOWN_ERROR: " << success << std::endl;
                break;
            }
            if (newShaderProgram == 0) {
                std::cout << "ERROR::SHADER::PROGRAM::BUILDING::FAILED\nUnknown error." << std::endl;
            }
            if (success == 0 && newShaderProgram != 0) {
                std::cout << "Successfully built new shader." << std::endl;
                glDeleteProgram(shaderProgram);
                shaderProgram = newShaderProgram;
            }
        } else {
            std::cout << "ERROR read code: " << success << std::endl;
        }
        if (fragmentShaderFileSource != NULL) {
            free(fragmentShaderFileSource);
            fragmentShaderFileSource = NULL;
        }
    }
}


/**
 * read_shader_source
 * 
 * params:
 *  sourcePath: path to shader source file
 *  source: pointer to cstring to put shader source in
 * 
 * returns:
 *  success status
*/
int read_shader_source(char *sourcePath, char **source) {
    std::ifstream file;
    int i, srcLen;
    char c;

    if (source == NULL) {
        return -1;
    }

    if (*source != NULL) {
        return -2;
    }

    // Read shader from file if it exists
    file.open(sourcePath);
    if (file) {
        // std::cout << "Reading shader from file:\n  ";
        srcLen = 0;
        while (file.get(c)) srcLen++;
        *source = (char *)malloc(srcLen + 1);
        file.close();
        file.open(sourcePath);
        for (i = 0; file.get(c) && i < srcLen; i++) {
            if (c == '\r') continue;
            (*source)[i] = c;
            // std::cout << c;
            if (c == '\n' && i < srcLen - 1) {
                // std::cout << "  ";
            }
        }
        (*source)[i] = '\0';
        // std::cout << std::endl;
        file.close();

        return 0;
    } else {
        return 1;
    }
}


/**
 * build_shader_program
 * 
 * params:
 *  fragmentShaderSource: fragment shader source code string
 *  success: result status output
 * 
 * returns:
 *  shaderProgram: GL shader program
 *      0 -> shader build succeeded
 *      -1 -> failed to compile vertex shader
 *      -2 -> failed to compile fragment shader
 *      -3 -> failed to link shaders
*/
unsigned int build_shader_program(char *fragmentShaderSource, int *success, char *infoLog) {
    unsigned int shaderProgram, vertexShader, fragmentShader;

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, success);
    if (!*success) {
        glGetProgramInfoLog(vertexShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        *success = -1;
        return 0;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, success);
    if (!*success) {
        glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        *success = -2;
        return 0;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetShaderiv(fragmentShader, GL_LINK_STATUS, success);
    if (!*success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(shaderProgram);
        *success = -3;
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    *success = 0;
    return shaderProgram;
}


RisingEdgeDetector::RisingEdgeDetector(void) {
    this->last = false;
}

bool RisingEdgeDetector::operator()(bool current) {
    bool output = false;
    if (current && !(this->last)) output = true;
    this->last = current;
    return output;
}
