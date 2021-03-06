#pragma once
#include "Light.h"
#include <vector>
#include "OmniShadowMap.h"

class PointLight :
    public Light
{

public:
    PointLight();
    PointLight(GLfloat shadow_width, GLfloat shadow_height,
                        GLfloat near, GLfloat far,
                        GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat a_intensity, GLfloat d_intensity,
                        GLfloat x_pos, GLfloat y_pos, GLfloat z_pos, 
                        GLfloat con, GLfloat lin, GLfloat exp);

    void use_light(GLuint ambient_intensity_location, GLuint ambient_color_location,
                               GLuint diffuse_intensity_location, GLuint position_location,
                                GLuint constant_location, GLuint linear_location, GLuint exponent_location);

    std::vector<glm::mat4> calculate_light_transform();

    GLfloat get_far_plane();
    glm::vec3 get_position();

    ~PointLight();

protected:

    glm::vec3 position;

    GLfloat constant, linear, exponent;

    GLfloat far_plane;

};

