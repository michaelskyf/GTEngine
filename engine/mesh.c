/*
    This file is part of GTEngine.

    GTEngine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTEngine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTEngine. If not, see <https://www.gnu.org/licenses/>.
*/

#include <GTEngine/mesh.h>
#include <GTEngine/output.h>

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

	mesh_t *m = malloc(sizeof(mesh_t));
	if(m)
	{

		if(!textures)
		{
			LOGW("Creating a mesh with no texture");
			m->textures = NULL;
		}

		m->vertices = vertices;
		m->indices = indices;
		m->shader = shader;
		if(textures)
			m->textures = textures;

		if(mesh_setup(m, shader))
		{
			LOGE("Error while setting up mesh");
			free(m);
			return NULL;
		}
	}
	return m;
}

void mesh_draw(mesh_t *m, mat4 *model_matrix)
{
	glUseProgram(m->shader->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	unsigned int model_matrix_loc = glGetUniformLocation(m->shader->id, "model_matrix");
	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, **model_matrix);
	glDrawElements(GL_TRIANGLES, vector_size(m->indices), GL_UNSIGNED_INT, 0);
}

void mesh_destroy(mesh_t *m)
{
	vector_destroy(m->vertices);
	vector_destroy(m->indices);
	// m->textures can be NULL, if there are none
	if(m->textures)
		vector_destroy(m->textures);

	// We shouldn't destroy shader here, since other meshes can still be using it

	free(m);
}

static int mesh_setup(mesh_t *m, shader_t *s)
{
	// Generate buffers
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ebo);

	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBufferData(GL_ARRAY_BUFFER, vector_size(m->vertices) * sizeof(vertex_t), vector_start(m->vertices), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vector_size(m->indices) * sizeof(unsigned int), vector_start(m->indices), GL_STATIC_DRAW);

	// Get attributes
	unsigned int vertex_position = glGetAttribLocation(s->id, "position");
	glEnableVertexAttribArray(vertex_position);

	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glVertexAttribPointer(vertex_position, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;
}
