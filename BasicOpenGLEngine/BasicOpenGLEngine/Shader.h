#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "GlobalValues.h"

class Shader
{
public:
	Shader();

	void create_from_string(const char* vertex_code, const char* fragment_code);
	void create_from_files(const char* vertex_location, const char* fragment_location);

	std::string read_file(const char* file_location);

	GLuint get_projection_location();
	GLuint get_view_location();
	GLuint get_model_location();
	GLuint get_ambient_intensity_location();
	GLuint get_ambient_color_location();
	GLuint get_diffuse_intensity_location();
	GLuint get_direction_location();
	GLuint get_specular_intensity_location();
	GLuint get_shininess_location();
	GLuint get_eye_position_location();

	void set_directional_light(DirectionalLight* d_light);
	void set_point_lights(PointLight* p_light, unsigned int light_count);
	void set_spot_lights(SpotLight* s_light, unsigned int light_count);
	void set_texture(GLuint texture_unit);
	void set_directional_shadow_map(GLuint texture_unit);
	void set_directional_light_transform(glm::mat4 * light_transform);

	void use_shader();
	void clear_shader();

	~Shader();

private:
	int point_light_counter;
	int spot_light_counter;

	GLuint shader_id, uniform_model, uniform_view, uniform_projection, uniform_eye_position,
				uniform_specular_intensity, uniform_shininess,
				uniform_texture,
			    uniform_directional_light_transform, uniform_directional_shadow_map;

	struct {
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;

		GLuint uniform_direction;
	} uniform_directional_light;

	GLuint uniform_point_light_count;

	struct {
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;

		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;
	} uniform_point_light[MAX_POINT_LIGHTS];

	GLuint uniform_spot_light_count;

	struct {
		GLuint uniform_color;
		GLuint uniform_ambient_intensity;
		GLuint uniform_diffuse_intensity;

		GLuint uniform_position;
		GLuint uniform_constant;
		GLuint uniform_linear;
		GLuint uniform_exponent;

		GLuint uniform_direction;
		GLuint uniform_edge;
	} uniform_spot_light[MAX_SPOT_LIGHTS];

	void add_shader(GLuint program, const char* shader_code, GLenum shader_type);
	void compile_shader(const char* vertex_code, const char* fragment_code);
};

