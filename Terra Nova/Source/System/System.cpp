#include "System.h"

GLFWwindow* input::window;
int input::scoll_state = 0;

void input::init(GLFWwindow* wnd)
{
	window = wnd;
}

bool input::is_key_pressed(int code)
{
	if (glfwGetKey(window, code) == GLFW_PRESS)
		return true;
	return false;
}

bool input::is_mouse_key_pressed(int code)
{
	if (glfwGetMouseButton(window, code) == GLFW_PRESS)
		return true;
	return false;
}

glm::vec2 input::get_mouse_position()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return { x, y };
}
