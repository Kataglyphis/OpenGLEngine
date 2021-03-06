#pragma once
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <GLFW\glfw3.h>

class Camera
{
public:

	Camera();

	Camera(glm::vec3 start_position, glm::vec3 start_up, GLfloat start_yaw, GLfloat start_pitch, GLfloat start_move_speed, GLfloat start_turn_speed);

	void key_control(bool* keys, GLfloat delta_time);
	void mouse_control(GLfloat x_change, GLfloat y_change);

	glm::vec3 get_camera_position();
	glm::vec3 get_camera_direction();

	glm::mat4 calculate_viewmatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 world_up;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movement_speed;
	GLfloat turn_speed;

	void update();
};

