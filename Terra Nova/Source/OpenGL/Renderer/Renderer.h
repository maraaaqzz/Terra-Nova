#pragma once

#include "../Shader.h"
#include "../Texture.h"
#include <glm/glm.hpp>

#include "Camera.h"

struct vertex
{
	unsigned int vertex_buffer;
	unsigned int index_buffer;
	unsigned int vertex_array;
};

/*
asta este o clasa statica => nu pot sa creez obiecte de tipul asta si toate variabilele sunt globale
static -> inseamna ca toate obiectele clasei au campul ala egal, de exemplu:

class persoana
{
public:
	static int planeta; // in cazul asta toate persoana existente sunt pe aceeasi planeta
};

persoana cata, mara, viitorul_presedinte_al_americii;
cata::planeta = 5; // aici nu mai chemama cu "." pentru ca planeta este un camp comun
// dupa ce am executat linia asta de cod variabila planeta a tuturor obiectelor de tip persoana e 5
mara::planeta = 3; // acum e 3 pentru toti
cout << cata::planeta; // se afiseaza "3"

*/

class renderer
{
public:
	static void init();
	static void close();
	static void clear(float r, float g, float b);

	static void draw(const texture& tex, glm::vec2 pos, glm::vec2 size, float px = 1.0f, float py = 1.0f);
	static void set_camera(const camera& cam);

private:
	static vertex vertex_elements;

	static shader* shader_program;
	static glm::mat4 renderer_camera;

private:
	// constructorul si destrucotrul sunt functii private pentru ca nu vreau sa creez obiecte de tipul "renderer"
	renderer() = default;
	~renderer() = default;
};