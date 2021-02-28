// BasicOpenGLEngine.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Window Dims
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = -3.1415926f / 180.f;
GLuint VAO, VBO, shader, uniform_model;

bool direction = true;
float tri_offst = 0.0f;
float tri_max_offset = 0.7f;
float tri_increment = 0.0005f;

float cur_angle = 0.0f;

bool size_direction = true;
float current_size = 0.4f;
float max_size = 0.8f;
float min_size = 0.1f;

//Vertex Shader 
static const char* vertex_shader = 
        "#version 330                                   \n\
                                                                    \n\
         layout (location = 0) in vec3 pos; \n\
                                                                    \n\
        out vec4 v_color;                              \n\
                                                                    \n\
        uniform mat4 model;                    \n\
                                                                    \n\
        void main() {                                       \n\
                                                                    \n\
            gl_Position = model * vec4(pos.x, pos.y, pos.z , 1.0);     \n\
            v_color = \n\
        }                                                          \n\
        ";

//fragment shader
static const char* fragment_shader =
        "#version 330                                   \n\
                                                                    \n\
        in vec4 v_color;                                \n\
                                                                    \n\
        out vec4 color;                                  \n\
                                                                    \n\
        void main() {                                       \n\
                                                                    \n\
            color = vec4(1.0, 0.0, 0.0, 1.0);     \n\
        }                                                          \n\
        ";

void create_triangle() {

    GLfloat vertices[] = {
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         0.f, 1.f, 0.f,
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

void add_shader(GLuint program, const char* shader_code, GLenum shader_type) {

    GLuint shader = glCreateShader(shader_type);

    const GLchar* code[1];
    code[0] = shader_code;

    GLint code_length[1];
    code_length[0] = strlen(shader_code);

    glShaderSource(shader, 1, code, code_length);
    glCompileShader(shader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result) {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader:  '%s'\n", shader_type, eLog);
        return;
    }

    glAttachShader(program, shader);

}

void compile_shaders() {

    shader = glCreateProgram();

    if (!shader) {
        printf("Error creating shader program!\n");
        return;
    }

    add_shader(shader, vertex_shader, GL_VERTEX_SHADER);
    add_shader(shader, fragment_shader, GL_FRAGMENT_SHADER);

    GLint result = 0;

    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);

    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniform_model = glGetUniformLocation(shader, "model");

}

int main()
{
    if (!glfwInit()) {

        printf("GLFW Init failed!");
        glfwTerminate();
        return 1;

    }

    // setup glfw window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile = no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

    if (!mainWindow) {

        printf("GLFW Window creation failed!");
        glfwTerminate();
        return 1;

    }

    // get buffer size information
    int bufferwidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferwidth, &bufferHeight);
    
    // set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {

        printf("GLEW Init failed!");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;

    }

    // setup viewport size
    glViewport(0,0,bufferwidth, bufferHeight);

    create_triangle();
    compile_shaders();

    // loop until window closed
    while (!glfwWindowShouldClose(mainWindow)) {

        // get and handle user input events
        glfwPollEvents();

        if (direction) {
            tri_offst += tri_increment;
        } else {
            tri_offst -= tri_increment;
        }

        if (abs(tri_offst) >= tri_max_offset) {
            direction = !direction;
        }

        cur_angle += 0.001f;
        
        if (cur_angle >= 360) {
            cur_angle -= 360;
        }

        if (size_direction) {
            current_size += 0.0001f;
        }
        else {
            current_size -= 0.0001f;
        }

        if (current_size >= max_size || current_size <= min_size) {

            size_direction = !size_direction;

        }

        // clear window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        
        glm::mat4 model(1.0f);

        model = glm::rotate(model, cur_angle*toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(current_size, current_size, 1.0f));
        model = glm::translate(model, glm::vec3(tri_offst, 0.0f, 0.0f));

        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);

    }


    return 0;
}
