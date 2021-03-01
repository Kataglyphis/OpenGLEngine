#include "MyWindow.h"

MyWindow::MyWindow() {
	width = 800;
	height = 600;
}

MyWindow::MyWindow(GLint window_width, GLint window_height) {
	width = window_width;
	height = window_height;
}

int MyWindow::initialize() {
    if (!glfwInit()) {

        printf("GLFW Init failed!");
        glfwTerminate();
        return 1;

    }

    // setup glfw window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // core profile = no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    main_window = glfwCreateWindow(width, height, "Gothic Remaster", NULL, NULL);

    if (!main_window) {

        printf("GLFW Window creation failed!");
        glfwTerminate();
        return 1;

    }

    // get buffer size information
    glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

    // set context for GLEW to use
    glfwMakeContextCurrent(main_window);

    // allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {

        printf("GLEW Init failed!");
        glfwDestroyWindow(main_window);
        glfwTerminate();
        return 1;

    }

    glEnable(GL_DEPTH_TEST);

    // setup viewport size
    glViewport(0, 0, buffer_width, buffer_height);
}

MyWindow::~MyWindow() {
    glfwDestroyWindow(main_window);
    glfwTerminate();
}