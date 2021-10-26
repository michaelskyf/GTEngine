#include <GTEngine/mesh.h>
#include <GTEngine/macros.h>

#include <glad/glad.h>
#include <stdlib.h>

static int mesh_setup(mesh_t *, shader_t *);

mesh_t *mesh_create(Vector *vertices, Vector *indices, Vector *textures, shader_t *shader)
{
	if(!vertices || !indices)
	{
		LOGE("Invalid vertices or indices");
		return NULL;
	}

	if(!shader)
	{
		LOGE("Invalid shader");
		return NULL;
	}

	if(!textures)
	{
		LOGW("Creating a mesh with no texture");
	}

	mesh_t *m = malloc(sizeof(mesh_t));

	m->vertices = vertices;
	m->indices = indices;
	if(textures)
		m->textures = textures;

	if(mesh_setup(m, shader))
	{
		LOGE("Error while setting up mesh");
		free(m);
		return NULL;
	}

	return m;
}

static int mesh_setup(mesh_t *m, shader_t *s)
{
	// Generate buffers
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ebo);

	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, vector_size(m->vertices) * sizeof(vertex_t), vector_start(m->vertices), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vector_size(m->indices) * sizeof(vertex_t), vector_start(m->indices), GL_STATIC_DRAW);

	// Get attributes
	unsigned int vertex_position = glGetAttribLocation(s->id, "position");
	glEnableVertexAttribArray(vertex_position);

	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glVertexAttribPointer(vertex_position, 3,  GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}
