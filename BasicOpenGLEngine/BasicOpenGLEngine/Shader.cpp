#include "Shader.h"

Shader::Shader() {

	shader_id = 0;
	uniform_model = 0;
    //uniform_view = 0;
	uniform_projection = 0;

    point_light_counter = 0;
    spot_light_counter = 0;
}

void Shader::create_from_files(const char* vertex_location, const char* fragment_location) {

    std::string vertex_string = read_file(vertex_location);
    std::string fragment_string = read_file(fragment_location);

    const char* vertex_code = vertex_string.c_str();
    const char* fragment_code = fragment_string.c_str();

    compile_shader(vertex_code, fragment_code);
}

void Shader::create_from_string(const char* vertex_code, const char* fragment_code) {
	compile_shader(vertex_code, fragment_code);
}

std::string Shader::read_file(const char* file_location) {
    std::string content;
    std::ifstream file_stream(file_location, std::ios::in);

    if (!file_stream.is_open()) {
        printf("Failed to read %s. File does not exist.", file_location);
        return "";
    }

    std::string line = "";
    while (!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}

void Shader::compile_shader(const char* vertex_code, const char* fragment_code) {

    shader_id = glCreateProgram();

    if (!shader_id) {
        printf("Error creating shader program!\n");
        return;
    }

    add_shader(shader_id, vertex_code, GL_VERTEX_SHADER);
    add_shader(shader_id, fragment_code, GL_FRAGMENT_SHADER);

    GLint result = 0;

    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader_id);
    glGetProgramiv(shader_id, GL_LINK_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader_id);

    glGetProgramiv(shader_id, GL_VALIDATE_STATUS, &result);

    if (!result) {
        glGetProgramInfoLog(shader_id, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniform_model = glGetUniformLocation(shader_id, "model");
    uniform_projection = glGetUniformLocation(shader_id, "projection");
    uniform_view = glGetUniformLocation(shader_id, "view");
    uniform_directional_light.uniform_color = glGetUniformLocation(shader_id, "directional_light.base.color");
    uniform_directional_light.uniform_ambient_intensity = glGetUniformLocation(shader_id, "directional_light.base.ambient_intensity");
    uniform_directional_light.uniform_diffuse_intensity = glGetUniformLocation(shader_id, "directional_light.base.diffuse_intensity");
    uniform_directional_light.uniform_direction = glGetUniformLocation(shader_id, "directional_light.direction");
    uniform_specular_intensity = glGetUniformLocation(shader_id, "material.specular_intensity");
    uniform_shininess = glGetUniformLocation(shader_id, "material.shininess");
    uniform_eye_position = glGetUniformLocation(shader_id, "eye_position");

    uniform_point_light_count = glGetUniformLocation(shader_id, "point_light_count");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.color", i);
        uniform_point_light[i].uniform_color = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.ambient_intensity", i);
        uniform_point_light[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].base.diffuse_intensity", i);
        uniform_point_light[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].position", i);
        uniform_point_light[i].uniform_position = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].constant", i);
        uniform_point_light[i].uniform_constant = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].linear", i);
        uniform_point_light[i].uniform_linear = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "point_lights[%zd].exponent", i);
        uniform_point_light[i].uniform_exponent = glGetUniformLocation(shader_id, loc_buff);
    }

    uniform_spot_light_count = glGetUniformLocation(shader_id, "spot_light_count");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++) {

        char loc_buff[100] = { '\0' };

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.base.color", i);
        uniform_spot_light[i].uniform_color = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.base.ambient_intensity", i);
        uniform_spot_light[i].uniform_ambient_intensity = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.base.diffuse_intensity", i);
        uniform_spot_light[i].uniform_diffuse_intensity = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.position", i);
        uniform_spot_light[i].uniform_position = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.constant", i);
        uniform_spot_light[i].uniform_constant = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.linear", i);
        uniform_spot_light[i].uniform_linear = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].base.exponent", i);
        uniform_spot_light[i].uniform_exponent = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].direction", i);
        uniform_spot_light[i].uniform_direction = glGetUniformLocation(shader_id, loc_buff);

        snprintf(loc_buff, sizeof(loc_buff), "spot_lights[%zd].edge", i);
        uniform_spot_light[i].uniform_edge = glGetUniformLocation(shader_id, loc_buff);
    }

    uniform_texture = glGetUniformLocation(shader_id, "first_texture");
    uniform_directional_light_transform = glGetUniformLocation(shader_id, "directional_light_transform");
    uniform_directional_shadow_map = glGetUniformLocation(shader_id, "directional_shadow_map");
}

GLuint Shader::get_projection_location() {
    return uniform_projection;
}

GLuint Shader::get_view_location() {
    return uniform_view;
}


GLuint Shader::get_model_location() {
    return uniform_model;
}

GLuint Shader::get_ambient_intensity_location() {

    return uniform_directional_light.uniform_ambient_intensity;

}
GLuint Shader::get_ambient_color_location() {

    return uniform_directional_light.uniform_color;

}

GLuint Shader::get_diffuse_intensity_location() {

    return uniform_directional_light.uniform_diffuse_intensity;

}

GLuint Shader::get_direction_location() {

    return uniform_directional_light.uniform_direction;

}

GLuint Shader::get_specular_intensity_location()
{
    return uniform_specular_intensity;
}

GLuint Shader::get_shininess_location()
{
    return uniform_shininess;
}

GLuint Shader::get_eye_position_location()
{
    return uniform_eye_position;
}

void Shader::set_directional_light(DirectionalLight* d_light)
{

    d_light->use_light(uniform_directional_light.uniform_ambient_intensity, uniform_directional_light.uniform_color,
                                    uniform_directional_light.uniform_diffuse_intensity, uniform_directional_light.uniform_direction);

}

void Shader::set_point_lights(PointLight* p_light, unsigned int light_count)
{

    if (light_count > MAX_POINT_LIGHTS) {
        light_count = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniform_point_light_count, light_count);

    for (size_t i = 0; i < light_count; i++) {

        p_light[i].use_light(uniform_point_light[i].uniform_ambient_intensity, uniform_point_light[i].uniform_color, 
            uniform_point_light[i].uniform_diffuse_intensity, uniform_point_light[i].uniform_position, 
            uniform_point_light[i].uniform_constant, uniform_point_light[i].uniform_linear, uniform_point_light[i].uniform_exponent);
    }

}

void Shader::set_spot_lights(SpotLight* s_light, unsigned int light_count)
{

    if (light_count > MAX_SPOT_LIGHTS) {
        light_count = MAX_SPOT_LIGHTS;
    }

    glUniform1i(uniform_spot_light_count, light_count);

    for (size_t i = 0; i < light_count; i++) {

        s_light[i].use_light(uniform_spot_light[i].uniform_ambient_intensity, uniform_spot_light[i].uniform_color,
            uniform_spot_light[i].uniform_diffuse_intensity, uniform_spot_light[i].uniform_position, uniform_spot_light[i].uniform_direction, 
            uniform_spot_light[i].uniform_constant, uniform_spot_light[i].uniform_linear, uniform_spot_light[i].uniform_exponent,
            uniform_spot_light[i].uniform_edge);
    }

}

void Shader::set_texture(GLuint texture_unit)
{
    glUniform1i(uniform_texture, texture_unit);
}

void Shader::set_directional_shadow_map(GLuint texture_unit)
{
    glUniform1i(uniform_directional_shadow_map, texture_unit);
}

void Shader::set_directional_light_transform(glm::mat4* light_transform)
{
    glUniformMatrix4fv(uniform_directional_light_transform, 1, GL_FALSE, glm::value_ptr(*light_transform));
}

void Shader::use_shader() {
    glUseProgram(shader_id);
}

void Shader::clear_shader() {
    if (shader_id != 0) {
        glDeleteProgram(shader_id);
        shader_id = 0;
    }

    uniform_model = 0;
    uniform_projection = 0;
}

void Shader::add_shader(GLuint program, const char* shader_code, GLenum shader_type) {

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


Shader::~Shader() {

    clear_shader();

}