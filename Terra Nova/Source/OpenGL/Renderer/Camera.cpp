#include "Camera.h"

#include "../../System/System.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

camera::camera(float x, float y) : ortho_projection(glm::ortho(0.0f, x, 0.0f, y)), res_x(x), res_y(y)
{ }

bool camera::update(float delta_time, bool can_move)
{
	bool moved = false;
	if (input::scoll_state > 0)
	{
		if (zoom_level <= max_zoom)
			zoom_level += zoom_speed * delta_time;
	}
	else if (input::scoll_state < 0)
		if (zoom_level - zoom_speed * delta_time >= min_zoom)
		{
			zoom_level -= zoom_speed * delta_time;
			moved = true;
		}
		else zoom_level = min_zoom;
		
	glm::vec2 mousePos = input::get_mouse_position() * 0.03f;
	glm::vec2 delta = (mousePos - mouse_position);
	mouse_position = mousePos;

	if (input::is_mouse_key_pressed(GLFW_MOUSE_BUTTON_RIGHT) && can_move)
	{
		position.x += delta.x * (pan_speed / zoom_level * 1.5f) * delta_time;
		position.y -= delta.y * (pan_speed / zoom_level * 1.5f) * delta_time;
		moved = true;
	}

	int right = res_x / 2.0f + position.x + (res_x / 2.0f / zoom_level);
	int left = res_x / 2.0f + position.x - (res_x / 2.0f / zoom_level);
	int top = res_y / 2.0f + position.y + (res_y / 2.0f / zoom_level);
	int bottom = res_y / 2.0f + position.y - (res_y / 2.0f / zoom_level);

	if (right > res_x)
		position.x = res_x / 2.0f - (res_x / 2.0f / zoom_level);
	if (left < 0)
		position.x = -res_x / 2.0f + (res_x / 2.0f / zoom_level);

	if (top > res_y)
		position.y = res_y / 2.0f - (res_y / 2.0f / zoom_level);
	if (bottom < 0)
		position.y = -res_y / 2.0f + (res_y / 2.0f / zoom_level);

	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), { zoom_level, zoom_level, 1.0f });
	glm::vec3 center(res_x / 2.0f - position.x, res_y / 2.0f - position.y, 1.0f);

	glm::mat4 view = trans * (glm::translate(glm::mat4(1.0f), center) * scale * glm::translate(glm::mat4(1.0f), -center));
	ortho_projection = glm::ortho(0.0f, res_x, 0.0f, res_y) * view;

	return moved;
}

glm::vec2 camera::to_world_coord(const glm::vec2& pos)
{
	int left = res_x / 2.0f - position.x - (res_x / 2.0f / zoom_level);
	int bottom = res_y / 2.0f - position.y - (res_y / 2.0f / zoom_level);

	return { left + pos.x / zoom_level, bottom + pos.y / zoom_level };
}

bool camera::move_towards(glm::vec2 target, float speed)
{
	bool location_reached = false;

	target -= glm::vec2(res_x / 2.0f, res_y / 2.0f);
	target = -target;
	glm::vec2 tract = target - position;

	float remaining_distance = glm::length(tract);
	if (remaining_distance <= speed || remaining_distance <= 0.0f)
	{
		position = target;
		location_reached = true;
	}
	else position = position + tract / remaining_distance * speed;

	return location_reached;
}
