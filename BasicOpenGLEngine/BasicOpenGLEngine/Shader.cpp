#include "Shader.h"

Shader::Shader() {

	shader_id = 0;
	uniform_model = 0;
    uniform_view = 0;
	uniform_projection = 0;

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