#include "Shader.h"

#include <fstream>
#include <iostream>

#include <GL/glew.h>

shader::shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
    shader_id = glCreateProgram();

    unsigned int vertex_shader;
    unsigned int fragment_shader;

    vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_path);
    fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_path);

    glAttachShader(shader_id, vertex_shader);
    glAttachShader(shader_id, fragment_shader);

    glLinkProgram(shader_id);
    glValidateProgram(shader_id);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

shader::~shader()
{
    glDeleteProgram(shader_id);
}

void shader::bind()
{
    glUseProgram(shader_id);
}

void shader::set_uniform_1i(const std::string& uniform_name, int value)
{
    glUniform1i(glGetUniformLocation(shader_id, uniform_name.c_str()), value);
}

void shader::set_uniform_1f(const std::string& uniform_name, float value)
{
    glUniform1f(glGetUniformLocation(shader_id, uniform_name.c_str()), value);
}

void shader::set_uniform_mat4(const std::string& uniform_name, const glm::mat4& value)
{
    glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name.c_str()), 1, false, &value[0][0]);
}

std::string shader::read_shader(const std::string& shader_path)
{
    std::string shader_source;
    std::ifstream shader_file(shader_path);

    std::string current_line;
    while (std::getline(shader_file, current_line))
        shader_source += current_line + '\n';

    shader_file.close();
    return shader_source;
}

unsigned int shader::compile_shader(unsigned int shader_type, const std::string& shader_path)
{
    unsigned int shader_program = glCreateShader(shader_type);

    std::string shader_source_code = read_shader(shader_path);
    const char* shader_source = shader_source_code.c_str();
    glShaderSource(shader_program, 1, &shader_source, 0);
    glCompileShader(shader_program);

    int result;
    glGetShaderiv(shader_program, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        int lenght;
        glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &lenght);

        char message[512];
        glGetShaderInfoLog(shader_program, lenght, &lenght, message);
        std::cout << message << std::endl;

        glDeleteShader(shader_program);

        return 0;
    }

    return shader_program;
}
