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

#include "GlobalValues.h"

#include "Mesh.h"
#include "Shader.h"
#include "MyWindow.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

const float to_radians = 3.14159265f / 180.f;

MyWindow main_window;
std::vector<Mesh*> mesh_list;
std::vector<Shader> shader_list;
Camera camera;

Texture ornament1;
Texture ornament2;
Texture plain;

Material shiny_material;
Material dull_material;

DirectionalLight main_light;
PointLight point_lights[MAX_POINT_LIGHTS];
SpotLight spot_lights[MAX_SPOT_LIGHTS];

GLfloat delta_time = 0.0f;
GLfloat last_time = 0.0f;

//Vertex Shader 
static const char* vertex_shader = "Shaders/shader.vert";
//fragment shader
static const char* fragment_shader = "Shaders/shader.frag";

void calc_average_normals(unsigned int* indices, unsigned int index_count, GLfloat* vertices, unsigned int vertex_count, unsigned int vertex_length, unsigned int normal_offset) {

    for (size_t i = 0; i < index_count; i+= 3) {

        unsigned int in0 = indices[i] * vertex_length;
        unsigned int in1 = indices[i + 1] * vertex_length;
        unsigned int in2 = indices[i + 2] * vertex_length;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = normalize(glm::cross(v1,v2)); 

        in0 += normal_offset;
        in1 += normal_offset;
        in2 += normal_offset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < vertex_count/vertex_length; i++) {

        unsigned int n_offset = i * vertex_length + normal_offset;
        glm::vec3 vec(vertices[n_offset], vertices[n_offset + 1], vertices[n_offset + 2]);
        vec = glm::normalize(vec);

        vertices[n_offset] = vec.x;
        vertices[n_offset + 1] = vec.y;
        vertices[n_offset + 2] = vec.z;
    }

}

void create_objects() {

    unsigned int indices[] = {
        0,3,1,
        1,3,2,
        2,3,0,
        0,1,2
    };

    GLfloat vertices[] = {
        //format:
        //x     y        z              u      v              n_x   n_y   n_z
        -1.f,   -1.f,   0.f,         0.0f, 0.0f,         0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,       0.5f, 0.0f,         0.0f, 0.0f, 0.0f,
         1.f,  -1.f,    0.f,        1.0f, 0.0f,          0.0f, 0.0f, 0.0f,
         0.f,  1.f,     0.f,        0.5f, 1.0f,          0.0f, 0.0f, 0.0f
    };

    unsigned int floor_indices[] = {
        0,2,1,
        1,2,3
    };

    GLfloat floor_vertices[] = {
        //format:
        //x     y        z              u      v                 n_x   n_y   n_z
        -10.f,   0.f,   -10.f,       0.0f, 0.0f,         0.0f, -1.0f, 0.0f,
        10.0f, 0.f, -10.0f,       10.f, 0.0f,         0.0f, -1.0f, 0.0f,
         -10.f,  0.f,    10.f,      0.f, 10.0f,          0.0f, -1.0f, 0.0f,
         10.f,  0.f,     10.f,      10.f, 10.0f,        0.0f, -1.0f, 0.0f
    };

    calc_average_normals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj_1 = new Mesh();
    obj_1->create_mesh(vertices, indices, 32,12);
    mesh_list.push_back(obj_1);

    Mesh* obj_2 = new Mesh();
    obj_2->create_mesh(vertices, indices, 32, 12);
    mesh_list.push_back(obj_2);

    Mesh* obj_3 =  new Mesh();
    obj_3->create_mesh(floor_vertices, floor_indices, 32, 6);
    mesh_list.push_back(obj_3);
}

void create_shaders() {
    Shader* shader1 = new Shader();
    shader1->create_from_files(vertex_shader, fragment_shader);
    shader_list.push_back(*shader1);
}

int main()
{
    
    main_window = MyWindow(1024, 768);
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
    plain = Texture(_strdup("Textures/plain.png"));
    plain.load_texture();

    shiny_material = Material(4.0f, 256);
    dull_material = Material(0.3f, 4);

    main_light = DirectionalLight(1.0f, 1.0f, 1.0f,
                                                        0.0f, 0.0f,
                                                        0.0f, 0.0f, -1.0f);

    unsigned int point_light_count = 0;

    point_lights[0] = PointLight(0.0f, 1.0f, 0.0f,
                                                    0.1f, 1.0f,
                                                    -4.0f, 0.0f, 0.0f,
                                                    0.3f, 0.2f, 0.1f);

    //point_light_count++;


    point_lights[1] = PointLight(0.0f, 0.0f, 1.0f,
                                                    0.0f, 0.1f,
                                                    4.0f, 0.0f, 0.0f,
                                                    0.3f, 0.2f, 0.1f);

    //point_light_count++;

    unsigned int spot_light_count = 0;

    spot_lights[0] = SpotLight(1.0f, 1.0f, 1.0f,
                                                    0.0f, 1.0f,
                                                    0.0f, 0.0f, 0.0f,
                                                    0.0f, -1.0f, 0.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    20.0f);

    spot_light_count++;

    spot_lights[1] = SpotLight(1.0f, 1.0f, 1.0f,
                                                    0.0f, 1.0f,
                                                    0.0f, 1.5f, 0.0f,
                                                    -100.0f, -1.0f, 0.0f,
                                                    1.0f, 0.0f, 0.0f,
                                                    20.0f);

    spot_light_count++;

    GLuint uniform_projection = 0, uniform_model = 0, uniform_view = 0, uniform_eye_position = 0,
                 uniform_specular_intensity = 0, uniform_shininess = 0;

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
        
        uniform_eye_position = shader_list[0].get_eye_position_location();
        uniform_specular_intensity = shader_list[0].get_specular_intensity_location();
        uniform_shininess = shader_list[0].get_shininess_location();

        spot_lights[0].set_flash(camera.get_camera_position(), camera.get_camera_position());
        
        shader_list[0].set_directional_light(&main_light);
        shader_list[0].set_point_lights(point_lights, point_light_count);
        shader_list[0].set_spot_lights(spot_lights, spot_light_count);
        
        glUniformMatrix4fv(uniform_projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniform_view, 1, GL_FALSE, glm::value_ptr(camera.calculate_viewmatrix()));
        glUniform3f(uniform_eye_position, camera.get_camera_position().x, camera.get_camera_position().y, camera.get_camera_position().z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        ornament1.use_texture();
        shiny_material.use_material(uniform_specular_intensity, uniform_shininess);
        mesh_list[0]->render_mesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        ornament2.use_texture();
        dull_material.use_material(uniform_specular_intensity, uniform_shininess);
        mesh_list[1]->render_mesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        //model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(model));
        ornament2.use_texture();
        shiny_material.use_material(uniform_specular_intensity, uniform_shininess);
        mesh_list[2]->render_mesh();

        glUseProgram(0);

        main_window.swap_buffers();

    }


    return 0;
}
