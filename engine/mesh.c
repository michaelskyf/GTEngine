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
#include "GTEngine/texture.h"
#include "GTEngine/vector.h"
#include <GTEngine/mesh.h>
#include <glad/glad.h>
#include <GTEngine/output.h>

void mesh_draw(mesh_t *m, shader_t *s)
{
	// Bind diffuse TODO: in more organized way

	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	glVertexAttribPointer(s->vPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
	glEnableVertexAttribArray(s->vPos);
	glVertexAttribPointer(s->nPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normals));
	glEnableVertexAttribArray(s->nPos);
	glVertexAttribPointer(s->tPos, 2,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texCoords));
	glEnableVertexAttribArray(s->tPos);

	glActiveTexture(GL_TEXTURE0);
	unsigned int diffPos = glGetUniformLocation(s->id, "diffuse");
	texture_t *tex = vector_get(m->material->textures, 0);
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glUniform1i(diffPos, 0);

	// Draw elements
	glDrawElements(GL_TRIANGLES, m->indices->size, GL_UNSIGNED_SHORT, 0);

	// Clean-up
	glActiveTexture(GL_TEXTURE0);
}
