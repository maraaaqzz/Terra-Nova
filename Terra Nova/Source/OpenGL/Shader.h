#pragma once
#include <string>
#include <glm/glm.hpp>

/*
aceasi poveste ca la textura
*/
class shader
{
public:
	shader() = default;
	shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
	~shader();

	void bind();

	void set_uniform_1i(const std::string& uniform_name, int value);
	void set_uniform_1f(const std::string& uniform_name, float value);
	void set_uniform_mat4(const std::string& uniform_name, const glm::mat4& value);

private:
	unsigned int shader_id;

private:
	std::string read_shader(const std::string& shader_path);
	unsigned int compile_shader(unsigned int shader_type, const std::string& shader_path);
};