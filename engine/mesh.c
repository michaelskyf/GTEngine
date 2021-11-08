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

void mesh_draw(mesh_t *m)
{
	// Bind shader
	glUseProgram(m->material->shader->id);

	mat4 projection;
	glm_perspective_rh_no(glm_rad(45.0f), evars->window->aspect_ratio, 0, 100, projection);
	int pLoc = glGetUniformLocation(m->material->shader->id, "projection");
	glad_glUniformMatrix4fv(pLoc, 1, GL_FALSE, *projection);

	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	glVertexAttribPointer(m->material->vPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), 0);
	glVertexAttribPointer(m->material->nPos, 3,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normals));
	glVertexAttribPointer(m->material->tPos, 2,  GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, texCoords));
	glEnableVertexAttribArray(0);

	// Draw elements
	glDrawElements(GL_TRIANGLES, m->indicesCount, GL_UNSIGNED_SHORT, 0);
}
