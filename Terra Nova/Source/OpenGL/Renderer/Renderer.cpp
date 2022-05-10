#include "Renderer.h"

#include "GL/glew.h"
#include <glm/gtc/matrix_transform.hpp>

shader* renderer::shader_program;
glm::mat4 renderer::renderer_camera;
vertex renderer::vertex_elements;

void renderer::init()
{
    float VertexData[] = {
       -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f,  0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
       -0.5f,  0.5f, 0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        0 ,1, 3
    };

    glCreateVertexArrays(1, &vertex_elements.vertex_array);
    glBindVertexArray(vertex_elements.vertex_array);

    glGenBuffers(1, &vertex_elements.vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_elements.vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), VertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &vertex_elements.index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_elements.index_buffer);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void*)(2 * sizeof(float)));
    shader_program = new shader("Resources/Shaders/QuadVertex.txt", "Resources/Shaders/QuadFragment.txt");
}

void renderer::close()
{
    delete shader_program;
}

void renderer::clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::draw(const texture& tex, glm::vec2 pos, glm::vec2 size, float px, float py)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

    glBindVertexArray(vertex_elements.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_elements.vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_elements.index_buffer);

    shader_program->bind();

    tex.bind();
    shader_program->set_uniform_1i("tex", 0);
    shader_program->set_uniform_mat4("ortho_matrix", renderer_camera);
    shader_program->set_uniform_mat4("model_matrix", model);

    shader_program->set_uniform_1f("u_px", px);
    shader_program->set_uniform_1f("u_py", py);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderer::set_camera(const camera& cam)
{
    renderer_camera = cam.get_projection();
}


/*
Shader
void load_shader() .. 
.. 

int sh = load_shader()

class shader
{
public:
    shader();

private:
    int id;
}

shader img_shader;

*/