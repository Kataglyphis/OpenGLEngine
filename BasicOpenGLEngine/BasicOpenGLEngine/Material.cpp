#include "Material.h"

Material::Material() {

	specular_intensity = 0.0f;
	shininess = 0.0f;

}

Material::Material(GLfloat spec_intensity, GLfloat shininess_factor) {

	specular_intensity = spec_intensity;
	shininess = shininess_factor;

}

void Material::use_material(GLuint specular_intensity_location, GLuint shininess_location) {

	glUniform1f(specular_intensity_location, specular_intensity);
	glUniform1f(shininess_location, shininess);

}

Material::~Material() {



}