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
#include "cglm/clipspace/persp_rh_no.h"
#include "cglm/util.h"
#include <GTEngine/mesh.h>
#include <GTEngine/output.h>
#include <glad/glad.h>

static int mesh_setup(mesh_t *);

mesh_t *mesh_create(vertex_t *vertices, size_t vCount, unsigned int *indices, size_t iCount,
		material_t *material)
{
	mesh_t *m = malloc(sizeof(mesh_t));
	if(m)
	{
		m->vertices = vertices;
		m->vCount = vCount;
		m->indices = indices;
		m->iCount = iCount;
		m->material = material;

		// If setup failed, destroy mesh and
		// return NULL
		if(mesh_setup(m))
		{
			LOGE("Failed to create mesh");
			free(m);
			return NULL;
		}
	}
	return m;
}

static int mesh_setup(mesh_t *m)
{
	// If no texture, warn the user
	if(!m->material->tCount)
		LOGW("Mesh using a material with no texture");

	// Generate buffers
	glGenBuffers(1, &m->vbo);
	glGenBuffers(1, &m->ebo);

	// Bind the buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	// Setup buffers data
	glBufferData(GL_ARRAY_BUFFER, m->vCount * sizeof(vertex_t), m->vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->iCount * sizeof(unsigned int), m->indices, GL_STATIC_DRAW);

	// Get attributes
	// vertex position
	unsigned int vPos = glGetAttribLocation(m->material->shader->id, "vPos");
	// normals position
	unsigned int nPos = glGetAttribLocation(m->material->shader->id, "nPos");
	// texture coords position
	unsigned int tPos = glGetAttribLocation(m->material->shader->id, "tPos");

	// Setup attribute pointers
	// vertex position
	glEnableVertexAttribArray(vPos);
	glVertexAttribPointer(vPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
	// normals position
	glEnableVertexAttribArray(nPos);
	glVertexAttribPointer(nPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normals));
	// texture coords position
	glEnableVertexAttribArray(tPos);
	glVertexAttribPointer(tPos, 2,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texCoords));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}

void mesh_draw(mesh_t *m)
{
	// Bind shader
	glUseProgram(m->material->shader->id);
	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	mat4 projection;
	glm_perspective_rh_no(glm_rad(45.0f), (float)1920/1080, 0, 100, projection);
	int pLoc = glGetUniformLocation(m->material->shader->id, "projection");
	glad_glUniformMatrix4fv(pLoc, 1, GL_FALSE, *projection);
	// Draw elements
	glDrawElements(GL_TRIANGLES, m->iCount, GL_UNSIGNED_INT, 0);
}
