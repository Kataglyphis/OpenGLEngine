#include "MyWindow.h"

MyWindow::MyWindow() {
	width = 800;
	height = 600;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }

    x_change = 0.0f;
    y_change = 0.0f;
}

MyWindow::MyWindow(GLint window_width, GLint window_height) {
	width = window_width;
	height = window_height;

    for (size_t i = 0; i < 1024; i++) {
        keys[i] = 0;
    }

    x_change = 0.0f;
    y_change = 0.0f;
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

    //Handle key + mouse Input
    create_callbacks();
    glfwSetInputMode(main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    glfwSetWindowUserPointer(main_window, this);
    
    return 0;
}

void MyWindow::create_callbacks() {
    glfwSetKeyCallback(main_window, handle_keys);
    glfwSetCursorPosCallback(main_window, handle_mouse);
}

GLfloat MyWindow::get_x_change() {

    GLfloat the_change = x_change;
    x_change = 0.0f;
    return the_change;

}

GLfloat MyWindow::get_y_change() {

    GLfloat the_change = y_change;
    y_change = 0.0f;
    return the_change;
}

void MyWindow::handle_keys(GLFWwindow* window, int key, int code, int action, int mode) {

    MyWindow* the_window = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            the_window->keys[key] = true;
            //printf("Pressed: %d\n", key);
        }
        else if (action == GLFW_RELEASE) {
            the_window->keys[key] = false;
            //printf("Released: %d\n", key);
        }
    }

}

void MyWindow::handle_mouse(GLFWwindow* window, double x_pos, double y_pos) {

    MyWindow* the_window = static_cast<MyWindow*>(glfwGetWindowUserPointer(window));

    if (the_window->mouse_first_moved) {
        the_window->last_x = x_pos;
        the_window->last_y = y_pos;
        the_window->mouse_first_moved = false;
    }

    the_window->x_change = x_pos - the_window->last_x;
    the_window->y_change = the_window->last_y - y_pos;

    the_window->last_x = x_pos;
    the_window->last_y = y_pos;

    //printf("x:%.6f,  y:%.6f\n", the_window->x_change, the_window->y_change);
}

MyWindow::~MyWindow() {
    glfwDestroyWindow(main_window);
    glfwTerminate();
}