/*
  * This file is for testing only, since I'm trying to relearn openGL.
*/

#include "GTEngine/vector.h"
#include <GTEngine/engine.h>
#include <GTEngine/shader.h>
#include <GTEngine/fileio.h>
#include <GTEngine/mesh.h>
#include <GTEngine/model.h>
#include <GTEngine/game_object.h>
#include "cglm/cglm.h"

#include <glad/glad.h>

float vertices_d[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f,   // top left
};
unsigned int indices_d[] = {
    0, 1, 3,   // first triangle
    1, 2, 3,    // second triangle
};


static model_t *model;
static shader_t *shader;

int program_setup()
{
	shader = shader_create("data/shaders/test.vs", "data/shaders/test.fs", "default");

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

	mesh_t *mesh = mesh_create(vertices, indices, NULL, shader);

	model = model_create();
	vector_push(model->meshes, &mesh);
	game_object_create(model, NULL, 0);

	return 0;
}

void program_exit(void)
{
	model_destroy(model);
	shader_destroy(shader);
}

void program_update()
{
	glUseProgram(shader->id);
	mat4 test;
	glm_mat4_identity(test);
	unsigned int model_matrix_position = glGetUniformLocation(shader->id, "model_matrix");
	LOGD("%d", model_matrix_position);
	glUniformMatrix4fv(model_matrix_position, 1, GL_FALSE, *test);
	model_draw(model);
}
