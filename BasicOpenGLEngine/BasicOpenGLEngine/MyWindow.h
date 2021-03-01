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

	GLfloat get_buffer_width() { return buffer_width; }
	GLfloat get_buffer_height() { return buffer_height; }

	bool get_should_close() { return glfwWindowShouldClose(main_window); }

	void swap_buffers() { glfwSwapBuffers(main_window); }

	~MyWindow();

private:
	GLFWwindow* main_window;

	GLint width, height;
	GLint buffer_width, buffer_height;
};

