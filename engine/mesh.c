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
#include <GTEngine/engine.h>
#include <GTEngine/output.h>
#include <glad/glad.h>

void mesh_draw(mesh_t *m, shader_t *s)
{
	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	glVertexAttribPointer(s->vPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
	glVertexAttribPointer(s->nPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normals));
	glVertexAttribPointer(s->tPos, 2,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texCoords));
	glEnableVertexAttribArray(0);

	// Draw elements
	glDrawElements(GL_TRIANGLES, m->indices->size, GL_UNSIGNED_SHORT, 0);

	// Draw children
	for(size_t i = 0; i < m->children->size; i++)
		mesh_draw(vector_get(m->children, i), s);
}
