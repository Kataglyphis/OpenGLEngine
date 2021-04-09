#pragma once

#include <stdio.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class MyWindow
{
public:
	MyWindow();
	MyWindow(GLint window_width, GLint window_height);

	int initialize();

	GLfloat get_buffer_width() { return (GLfloat)buffer_width; }
	GLfloat get_buffer_height() { return (GLfloat)buffer_height; }

	bool get_should_close() { return glfwWindowShouldClose(main_window); }

	bool* get_keys() { return keys; }
	GLfloat get_x_change();
	GLfloat get_y_change();

	void swap_buffers() { glfwSwapBuffers(main_window); }

	~MyWindow();

private:
	GLFWwindow* main_window;

	GLint width, height;
	GLint buffer_width, buffer_height;

	bool keys[1024];

	GLfloat last_x;
	GLfloat last_y;
	GLfloat x_change;
	GLfloat y_change;
	bool mouse_first_moved;


	void create_callbacks();

	static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handle_mouse(GLFWwindow* window, double x_pos, double y_pos);
};

