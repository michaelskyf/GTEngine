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

void mesh_draw(mesh_t *m)
{
	// Bind shader
	glUseProgram(m->material->shader->id);
	// Bind buffers
	glBindBuffer(GL_ARRAY_BUFFER, m->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->ebo);

	mat4 projection;
	glm_perspective_rh_no(glm_rad(89.0f), (float)1920/1080, 0, 100, projection);
	int pLoc = glGetUniformLocation(m->material->shader->id, "projection");
	glad_glUniformMatrix4fv(pLoc, 1, GL_FALSE, *projection);
	// Draw elements
	glDrawElements(GL_TRIANGLES, m->indicesCount, GL_UNSIGNED_INT, 0);
	LOGD("%ld %ld", m->indicesCount, m->verticesCount);
}
