// BasicOpenGLEngine.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#define STB_IMAGE_IMPLEMENTATION

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
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

const float to_radians = 3.14159265f / 180.f;

MyWindow main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;
Camera camera;

Texture ornament1;
Texture ornament2;

Light main_light;

GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

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
        //format:
        //x     y        z       u      v
        -1.f,   -1.f,   0.f,   0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
         1.f,  -1.f,    0.f,  1.0f, 0.0f,
         0.f,  1.f,     0.f,  0.5f, 1.0f,
    };

    Mesh* obj_1 = new Mesh();
    obj_1->create_mesh(vertices, indices, 20,12);
    mesh_list.push_back(obj_1);

    Mesh* obj_2 = new Mesh();
    obj_2->create_mesh(vertices, indices, 20, 12);
    mesh_list.push_back(obj_2);
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

    glm::vec3 start_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 start_up = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat start_yaw = -90.0f;
    GLfloat start_pitch = 0.0f;
    GLfloat start_move_speed = 5.0f;
    GLfloat start_turn_speed = 0.1f;
    camera = Camera(start_position, start_up, start_yaw, start_pitch, start_move_speed, start_turn_speed);

    ornament1 = Texture(_strdup("Textures/brick.png"));
    ornament1.load_texture();
    ornament2 = Texture(_strdup("Textures/dirt.png"));
    ornament2.load_texture();

    main_light = Light(1.0f, 1.0f, 1.0f, 0.2f);

    GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0, uniform_ambient_intensity = 0, uniform_ambient_color = 0;
    glm::mat4 projection = glm::perspective(45.f, main_window.get_buffer_width()/main_window.get_buffer_height(), 0.1f, 100.f);

    // loop until window closed
    while (!main_window.get_should_close()) {

        GLfloat now = glfwGetTime();
        delta_time = now - last_time;
        last_time = now;

        // get and handle user input events
        glfwPollEvents();

        //handle events
        camera.key_control(main_window.get_keys(), delta_time);
        camera.mouse_control(main_window.get_x_change(), main_window.get_y_change());

        // clear window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader_list[0].use_shader();
        uniform_model = shader_list[0].get_model_location();
        uniform_projection = shader_list[0].get_projection_location();
        uniform_view = shader_list[0].get_view_location();
        uniform_ambient_intensity = shader_list[0].get_ambient_intensity_location();
        uniform_ambient_color = shader_list[0].get_ambient_color_location();

        main_light.use_light(uniform_ambient_intensity, uniform_ambient_color);
        
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.calculate_viewmatrix()));
        ornament1.use_texture();
        mesh_list[0]->render_mesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        ornament2.use_texture();
        mesh_list[1]->render_mesh();

        glUseProgram(0);

        main_window.swap_buffers();

    }


    return 0;
}
