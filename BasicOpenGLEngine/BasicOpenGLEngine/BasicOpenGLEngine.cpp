// BasicOpenGLEngine.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"

const float to_radians = 3.14159265f / 180.f;

MyWindow main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;

//Vertex Shader 
static const char* vertex_shader = "Shaders/shader.vert";
//fragment shader
static const char* fragment_shader = "Shaders/shader.frag";

void create_objects() {

    unsigned int indices[] = {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };

    GLfloat vertices[] = {
        -1.f, -1.f, 0.f,
        0.0f, -1.0f, 1.0f,
         1.f, -1.f, 0.f,
         0.f, 1.f, 0.f,
    };

    Mesh* obj_triangle = new Mesh();
    obj_triangle->create_mesh(vertices, indices, 12,12);
    mesh_list.push_back(obj_triangle);
}

void create_shaders() {
    Shader* shader1 = new Shader();
    shader1->create_from_files(vertex_shader, fragment_shader);
    shader_list.push_back(*shader1);
}

int main()
{
    
    main_window = MyWindow(800, 600);
    main_window.initialize();

    create_objects();
    create_shaders();

    GLuint uniform_projection = 0, uniform_model = 0;
    glm::mat4 projection = glm::perspective(45.f, main_window.get_buffer_width()/main_window.get_buffer_height(), 0.1f, 100.f);

    // loop until window closed
    while (!main_window.get_should_close()) {

        // get and handle user input events
        glfwPollEvents();

        // clear window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_list[0].use_shader();
        uniform_model = shader_list[0].get_model_location();
        uniform_projection = shader_list[0].get_projection_location();
        
        glm::mat4 model(1.0f);

        ///model = glm::rotate(model, current_angle * to_radians, glm::vec3(0.0f,1.0f,0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
        mesh_list[0]->render_mesh();

        glUseProgram(0);

        main_window.swap_buffers();

    }


    return 0;
}
