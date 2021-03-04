#include "PointLight.h"

PointLight::PointLight()
{
    position = glm::vec3(0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
                                        GLfloat a_intensity, GLfloat d_intensity,
                                        GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, 
                                        GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, a_intensity, d_intensity)
{
    position = glm::vec3(x_pos, y_pos, z_pos);
    constant = con;
    linear = lin;
    exponent = exp;
}

void PointLight::use_light(GLfloat ambient_intensity_location, GLfloat ambient_color_location,
                                              GLfloat diffuse_intensity_location,
                                              GLfloat position_location, GLfloat constant_location, GLfloat linear_location,
                                              GLfloat exponent_location) 
{

    glUniform3f(ambient_color_location, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_location, ambient_intensity);
    glUniform1f(diffuse_intensity_location, diffuse_intensity);

    glUniform3f(position_location, position.x, position.y, position.z);
    glUniform1f(constant_location, constant);
    glUniform1f(linear_location, linear);
    glUniform1f(exponent_location, exponent);

}

PointLight::~PointLight()
{
}
