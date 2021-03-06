#include "PointLight.h"

PointLight::PointLight()
{
    position = glm::vec3(0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(GLfloat shadow_width, GLfloat shadow_height,
                                        GLfloat near, GLfloat far, 
                                        GLfloat red, GLfloat green, GLfloat blue,
                                        GLfloat a_intensity, GLfloat d_intensity,
                                        GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, 
                                        GLfloat con, GLfloat lin, GLfloat exp) : Light(shadow_width,shadow_height,
                                                                                                                        red, green, blue,
                                                                                                                        a_intensity, d_intensity)
{
    position = glm::vec3(x_pos, y_pos, z_pos);
    constant = con;
    linear = lin;
    exponent = exp;
    far_plane = far;

    float aspect = (float)shadow_width / (float)shadow_height;
    light_proj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    shadow_map = new OmniShadowMap();
    shadow_map->init((GLuint)shadow_width, (GLuint)shadow_height);
}

void PointLight::use_light(GLuint ambient_intensity_location, GLuint ambient_color_location,
                                              GLuint diffuse_intensity_location,
                                              GLuint position_location, GLuint constant_location, GLuint linear_location,
                                              GLuint exponent_location)
{

    glUniform3f(ambient_color_location, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_location, ambient_intensity);
    glUniform1f(diffuse_intensity_location, diffuse_intensity);

    glUniform3f(position_location, position.x, position.y, position.z);
    glUniform1f(constant_location, constant);
    glUniform1f(linear_location, linear);
    glUniform1f(exponent_location, exponent);

}

std::vector<glm::mat4> PointLight::calculate_light_transform()
{
    std::vector<glm::mat4> light_matrices;
    //make sure all light matrices align with the order we were defining in OmniShadowMap
    //GL_TEXTURE_CUBE_MAP_POSITIVE_X+i; therefoe start off with glm::vec3(1.0, 0.0,0.0)
    //+x,-x
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(1.0, 0.0,0.0), glm::vec3(0.0, -1.0, 0.0)));
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

    //+y,-y
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

    //+z,-z
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    light_matrices.push_back(light_proj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    return light_matrices;
}

GLfloat PointLight::get_far_plane()
{
    return far_plane;
}

glm::vec3 PointLight::get_position()
{
    return position;
}

PointLight::~PointLight()
{
}
