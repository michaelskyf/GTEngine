/*
  * This file is for testing only, since I'm trying to relearn openGL.
*/

#include "GTEngine/vector.h"
#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>
#include <GTEngine/mesh.h>

#include <glad/glad.h>

float vertices_d[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};
unsigned int indices_d[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};


static mesh_t *mesh;
static shader_t *shader;

int program_setup()
{
	shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs", NULL);

	if(!shader)
	{
		LOGE("Failed to create shader program");
		return -1;
	}

	Vector *vertices = vector_create(0, sizeof(vertex_t));
	Vector *indices = vector_create(0, sizeof(unsigned int));

	for(int i = 0; i < sizeof(vertices_d)/sizeof(*vertices_d); i += 3)
	{
		vertex_t vertex;
		vertex.position[0] = vertices_d[i];
		vertex.position[1] = vertices_d[i+1];
		vertex.position[2] = vertices_d[i+2];

		vector_push(vertices, &vertex);
	}

	for(int i = 0; i < sizeof(indices_d)/sizeof(*indices_d); i++)
	{
		vector_push(indices, &indices_d[i]);
	}

	mesh = mesh_create(vertices, indices, NULL, shader);

	return 0;
}

void program_update()
{
	glUseProgram(shader->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
