#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/*
o clasa asemanatoare cu cea a renderului dar e legata de input
*/
class input
{
public:
	static void init(GLFWwindow* wnd);

	static bool is_key_pressed(int code);
	static bool is_mouse_key_pressed(int code);
	static glm::vec2 get_mouse_position();

	static int scoll_state;

private:
	static GLFWwindow* window;
};