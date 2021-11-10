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
#include "GTEngine/mesh.h"
#include "cglm/mat4.h"
#include "cglm/vec3.h"
#include <GTEngine/game_object.h>
#include <GTEngine/engine.h>
#include <glad/glad.h>
#include <stdlib.h>

game_object_t *game_object_create(model_t *model)
{
	game_object_t *g = malloc(sizeof(game_object_t));
	if(g)
	{
		g->childrenNum = 0;
		g->enabled = 1;
		g->parent = NULL;
		g->model = model;

		glm_mat4_identity(g->model_matrix);
		glm_perspective(glm_rad(45.0f), evars->window->aspect_ratio, 0, 100, g->model_matrix);
		glm_vec3_zero(g->velocity);
	}
	return g;
}

void game_object_destroy(game_object_t *g)
{
	free(g);
}

void game_object_draw(game_object_t *g, shader_t *s)
{
	glUniformMatrix4fv(s->umPos, 1, GL_FALSE, *g->model_matrix);
	model_draw(g->model, s);
}
