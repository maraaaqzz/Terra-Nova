#pragma once

#include <glm/glm.hpp>

class camera
{
public:
	camera(float x, float y);

	bool update(float deltaTime, bool can_move = true);
	glm::mat4 get_projection() const { return ortho_projection; }

	float get_zoom_level() const { return zoom_level; }
	glm::vec2 to_world_coord(const glm::vec2& pos);
	bool move_towards(glm::vec2 target, float speed);
	const glm::vec2& get_position() { return position; }

private:
	glm::mat4 ortho_projection;
	glm::vec2 position = {0.0f, 0.0f};
	glm::vec2 mouse_position = {0.0f, 0.0f};

	float res_x, res_y;

	float pan_speed = 1000.0f;

	float zoom_level = 1.0f;
	float max_zoom = 3.5f;
	float min_zoom = 1.0f;

	float zoom_speed = 1.1f;
};
