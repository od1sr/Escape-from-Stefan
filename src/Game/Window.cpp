#include "Window.h"
#include <iostream>
#include "logging.h"

using namespace sgl;

float Window::mouse_last_x,
Window::mouse_last_y;
GLFWwindow *Window::win;
sgl::CursorMovementCallback Window::cursor_callback = NULL;
void *Window::user_pointer_for_cursor_callback = NULL;
int Window::screen_w, Window::screen_h;
bool Window::is_paused = false;

void glfwError(int id, const char *description);

void sgl::Window::init(const char *window_name, int height, int width)
{
	glfwSetErrorCallback(glfwError);
	glfwInit();
	if (height == FULL_SCREEN || width == FULL_SCREEN)
	{
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *mode = glfwGetVideoMode(monitor);
		screen_h = height == FULL_SCREEN ? mode->height : height;
		screen_w = width == FULL_SCREEN ? mode->width : width;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	win = glfwCreateWindow(screen_w, screen_h, window_name, NULL, NULL);
	if (!win)
	{
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		exit(2);
	}
#ifndef _DEBUG
	glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
#endif // !_DEBUG
	glfwSetFramebufferSizeCallback(win,
		framebufferSizeCallback
	);
	glViewport(0, 0, screen_w, screen_h);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win,
		mouseMovementCallback
	);
	glfwSetCursorPos(win, mouse_last_x = screen_w / 2.f, mouse_last_y = screen_h / 2.f);
}

void glfwError(int id, const char *description)
{
	int log_len = 40 + strlen(description);
	char *log = new char[log_len];
	sprintf_s(log, log_len, "[GLFW Error] (%d: %s)\n", id, description);
	save_error_log(log);
	delete[] log;
}

void Window::framebufferSizeCallback(GLFWwindow *win, int w, int h)
{
	screen_h = h;
	screen_w = w;
	glViewport(0, 0, w, h);
}

void Window::mouseMovementCallback(GLFWwindow *win, double xpos, double ypos)
{
	if (cursor_callback && !is_paused)
		cursor_callback(mouse_last_x - xpos, mouse_last_y - ypos, user_pointer_for_cursor_callback);
	mouse_last_x = xpos;
	mouse_last_y = ypos;
}

void Window::setCursorMovementCallback(CursorMovementCallback callback, void *user_pointer)
{
	cursor_callback = callback;
	user_pointer_for_cursor_callback = user_pointer;
}

void sgl::Window::handleEvents()
{
	glfwPollEvents();
}

bool sgl::Window::isPaused()
{
	return false;
}

void sgl::Window::terminate()
{
	glfwTerminate();
}
